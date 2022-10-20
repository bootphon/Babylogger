/**
  ******************************************************************************
  * File Name          : app_x-cube-ble1.c
  * Description        : Implementation file
  *             
  ******************************************************************************
  *
  * COPYRIGHT 2019 STMicroelectronics
  *
  * Licensed under MCD-ST Liberty SW License Agreement V2, (the "License");
  * You may not use this file except in compliance with the License.
  * You may obtain a copy of the License at:
  *
  *        http://www.st.com/software_license_agreement_liberty_v2
  *
  * Unless required by applicable law or agreed to in writing, software 
  * distributed under the License is distributed on an "AS IS" BASIS, 
  * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
  * See the License for the specific language governing permissions and
  * limitations under the License.
  ******************************************************************************
  */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __APP_X_CUBE_BLE1_C
#define __APP_X_CUBE_BLE1_C
#ifdef __cplusplus
 extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "app_x-cube-ble1.h"

#include "hci.h"
#include "hci_le.h"
#include "hci_tl.h"
#include "link_layer.h"
#include "BLE_Com.h"
#include "gatt_db.h"

#include "compiler.h"
#include "bluenrg_utils.h"
#include "UART_DEBUG.h"
#include "bluenrg_gap.h"
#include "bluenrg_gap_aci.h"
#include "bluenrg_gatt_aci.h"
#include "bluenrg_hal_aci.h"
#include "sm.h"
#include "stm32l4xx_hal_tim.h"


/* USER CODE BEGIN Includes */

/* USER CODE END Includes */

/* Private defines -----------------------------------------------------------*/
/**
 * 1 to send environmental and motion data when pushing the user button
 * 0 to send environmental and motion data automatically (period = 1 sec)
 */
#define USE_BUTTON 1
 
/* Private macros ------------------------------------------------------------*/

/* Private variables ---------------------------------------------------------*/


extern volatile uint8_t set_connectable;
extern volatile int connected;
uint8_t BLE_Discovrable = 0;

/* at startup, suppose the X-NUCLEO-IDB04A1 is used */
uint8_t bnrg_expansion_board = IDB04A1; 
uint8_t bdaddr[BDADDR_SIZE];

static volatile uint8_t user_button_init_state = 1;
static volatile uint8_t user_button_pressed = 0;

/* USER CODE BEGIN PV */

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
static void User_Process(void);
static void User_Init(void);



static void Set_Random_Address(uint8_t* bdaddr, uint8_t hwVersion, uint16_t fwVersion);

/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

#if PRINT_CSV_FORMAT
extern volatile uint32_t ms_counter;
/**
 * @brief  This function is a utility to print the log time
 *         in the format HH:MM:SS:MSS (DK GUI time format)
 * @param  None
 * @retval None
 */
void print_csv_time(void){
  uint32_t ms = HAL_GetTick();
  PRINT_CSV("%02d:%02d:%02d.%03d", ms/(60*60*1000)%24, ms/(60*1000)%60, (ms/1000)%60, ms%1000);
}
#endif

void MX_BlueNRG_MS_Init(void)
{
  /* USER CODE BEGIN SV */ 

  /* USER CODE END SV */
  
  /* USER CODE BEGIN BlueNRG_MS_Init_PreTreatment */
  
  /* USER CODE END BlueNRG_MS_Init_PreTreatment */

  /* Initialize the peripherals and the BLE Stack */
  const char *name = "BabyLogger";    
  uint16_t service_handle, dev_name_char_handle, appearance_char_handle;
  
  uint8_t  hwVersion;
  uint16_t fwVersion;
  int ret;  
  
  User_Init();
  
    
  hci_init(user_notify, NULL);
  
  /* get the BlueNRG HW and FW versions */
  getBlueNRGVersion(&hwVersion, &fwVersion);
  	
  /* 
   * Reset BlueNRG again otherwise we won't
   * be able to change its MAC address.
   * aci_hal_write_config_data() must be the first
   * command after reset otherwise it will fail.
   */
  hci_reset(); 
  HAL_Delay(100);
 
  PRINTF("HWver %d\nFWver %d\n", hwVersion, fwVersion);
  if (hwVersion > 0x30) { /* X-NUCLEO-IDB05A1 expansion board is used */
    bnrg_expansion_board = IDB05A1;    
  }

  /*
  * Change the MAC address to avoid issues with Android
  * cache if different boards have the same MAC address
  */
  Set_Random_Address(bdaddr, hwVersion, fwVersion);
  
  ret = aci_hal_write_config_data(CONFIG_DATA_PUBADDR_OFFSET,
                                  CONFIG_DATA_PUBADDR_LEN,
                                  bdaddr);
  if (ret) {
    PRINTF("Setting BD_ADDR failed.\n");
  }
  
  /* GATT Init */
  ret = aci_gatt_init();    
  if(ret){
    PRINTF("GATT_Init failed.\n");
  }

  /* GAP Init */
  if (bnrg_expansion_board == IDB05A1) {
    ret = aci_gap_init_IDB05A1(GAP_PERIPHERAL_ROLE_IDB05A1, 0, 0x0A, &service_handle, &dev_name_char_handle, &appearance_char_handle);
  }
  else {
    ret = aci_gap_init_IDB04A1(GAP_PERIPHERAL_ROLE_IDB04A1, &service_handle, &dev_name_char_handle, &appearance_char_handle);
  }
  if (ret != BLE_STATUS_SUCCESS) {
    PRINTF("GAP_Init failed.\n");
  }

  /* Update device name */
  ret = aci_gatt_update_char_value(service_handle, dev_name_char_handle, 0,
                                   strlen(name), (uint8_t *)name);
  if (ret) {
    PRINTF("aci_gatt_update_char_value failed.\n");            
    while(1);
  }
  
  ret = aci_gap_set_auth_requirement(MITM_PROTECTION_REQUIRED,
                                     OOB_AUTH_DATA_ABSENT,
                                     NULL,
                                     7,
                                     16,
                                     USE_FIXED_PIN_FOR_PAIRING,
                                     123456,
                                     BONDING);
  if (ret) {
    PRINTF("aci_gap_set_authentication_requirement failed.\n");
    while(1);
  }
  
  PRINTF("BLE Stack Initialized\n");
  
  ret = Add_BabyLogger_Service();
  if(ret == BLE_STATUS_SUCCESS) {
    PRINTF("Baby logger added successfully.\n");
  } else {
    PRINTF("Error while adding Baby logger service: 0x%02x\r\n", ret);
    while(1);
  }
  


  /* Set output power level */
  ret = aci_hal_set_tx_power_level(1,4);
  
  /* USER CODE BEGIN BlueNRG_MS_Init_PostTreatment */
  
  /* USER CODE END BlueNRG_MS_Init_PostTreatment */
}

/*
 * BlueNRG-MS background task
 */
void MX_BlueNRG_MS_Process(void)
{
  /* USER CODE BEGIN BlueNRG_MS_Process_PreTreatment */
  
  /* USER CODE END BlueNRG_MS_Process_PreTreatment */
  
  User_Process();  
  hci_user_evt_proc();

  /* USER CODE BEGIN BlueNRG_MS_Process_PostTreatment */
  
  /* USER CODE END BlueNRG_MS_Process_PostTreatment */
}

/**
 * @brief  Initialize User process.
 *
 * @param  None
 * @retval None
 */
static void User_Init(void)
{
	#if (DEBUG == 1)
  BSP_COM_Init(COM1);
	#endif
}

/**
 * @brief  Process user input (i.e. pressing the USER button on Nucleo board)
 *         and send the updated acceleration data to the remote client.
 *
 * @param  None
 * @retval None
 */
static void User_Process(void)
{  
  if (set_connectable)
  {
    Set_DeviceConnectable();
		BLE_Discovrable = 1;
    set_connectable = FALSE;
  }  

	if(connected == FALSE)
	{
		if ( (HAL_GetTick()> Timeout_BLE_APP) && (BLE_Discovrable == 1) )
		{
			aci_gap_set_non_discoverable();		
			BLE_Discovrable = 0;
			IHM_LED_Stop(LED2);
		}
	}
	else 
	{
		if (BLE_Task_Is_End()==1)
		{
			/* Disconect BLE*/
			aci_gap_terminate(BLE_Get_Connection_Handle(),1);
			aci_gap_set_non_discoverable();	
			BLE_Discovrable = 0;
			IHM_LED_Stop(LED2);
		}
	}
	
	
	
}



/**
 * @brief  Set a random BLE MAC address
 * @param  Pointer to array where to save the BLE MAC address 
 * @param  Hardware version
 * @param  Firmware version
 * @retval None
 */
static void Set_Random_Address(uint8_t* bdaddr, uint8_t hwVersion, uint16_t fwVersion)
{  
  uint8_t i;
  
  /* Initialize a random seed */
  srand (HAL_GetTick() + hwVersion + fwVersion);
  
  for (i=0; i<5; i++) {
    bdaddr[i] = rand()&0xFF;
  }
  bdaddr[i] = 0xD0; 
}


#ifdef __cplusplus
}
#endif
#endif /* __APP_X_CUBE_BLE1_C */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
