/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2019 STMicroelectronics.
  * All rights reserved.</center></h2>
  *
  * This software component is licensed by ST under Ultimate Liberty license
  * SLA0044, the "License"; You may not use this file except in compliance with
  * the License. You may obtain a copy of the License at:
  *                             www.st.com/SLA0044
  *
  ******************************************************************************
  */
/* USER CODE END Header */

/* Includes ------------------------------------------------------------------*/

#include "main.h"
#include "fatfs.h"
#include "GPIO.h"
#include "sensor_acquisition.h"
#include "random_key.h"
#include "PWR.h"
#include "app_x-cube-ble1.h"


uint16_t BL_Serial_Number = 3;

/* Extern variables ----------------------------------------------------------*/

extern uint8_t Global_App_State;
extern uint8_t		RTC_Buff[7];

/* Flags -------------------------------------------------------------------*/

static uint8_t Stop_Sys_Rqst = 0;
uint8_t charge_state = 0; 

/* Variables to reorgenize ---------------------------------------------------*/

uint32_t key[4];
uint32_t key_read[4];

uint32_t var_test = 0;
/* Private function prototypes -----------------------------------------------*/

void SystemClock_Config(void);
static void Stop_System(void);
static void Common_Init_System (void);
static void Run_Mode_Init_System (void);
static void Run_Mode_Process (void);
static void Charge_Mode_Init_System (void);
static void Charge_Mode_Process (void);
static void Main_Get_Serial_Number(uint16_t* SN);


/* Private user code ---------------------------------------------------------*/


/**
  * @brief  The application entry point.
  * @retval int
  */
int main(void)
{


  /* MCU Configuration--------------------------------------------------------*/

	Common_Init_System ();
	
	if (Global_App_State == Global_App_Run_Mode)
	{
		Run_Mode_Init_System ();
	}
	if (Global_App_State == Global_App_Charge_Mode)
	{
		Charge_Mode_Init_System ();
	}
	
	// test meriem 
	
	
	

  // Initialize all configured peripherals 

	//RNG_Init();

	
	//RNG_Get_Key(&key[0]);
	
	
	//EEPROM_Save_Key(&key[0]);



	// fin test meriem
  while (1)
  {
		
		if (Global_App_State == Global_App_Run_Mode)
		{
			Run_Mode_Process();
		}
		
		if (Global_App_State == Global_App_Charge_Mode)
		{
			Charge_Mode_Process();
		}
		
		// common process
		charge_state = GPIO_BAT_Charg_Get_State ();
		
		Sensors_Process();
		

  }
  /* USER CODE END 3 */
}

static void Common_Init_System (void)
{
  // system init------------------------------------ 
	
  HAL_Init();
  SystemClock_Config();
	
	
	//RTC init---------------------------------------- 
	// pwr init must be called befor rtc because of the enabling of backup access
	PWR_Init();
	RTC_Init();
	
	// Init wifi ctrl power --------------------------
	
	GPIO_Wifi_Init();
	GPIO_Wifi_Off();
	
	// Init Charger statuts --------------------------
	
	GPIO_BAT_Charg_Init();
	
	//LED Init----------------------------------------
	
	IHM_LED_Continious_Init(LED1_2);
	//script_test_led_func();
	
	// Init des capteurs -----------------------------
	//pour décider si les seuils de bat et temp autorise la mise en marche
	
	Sensors_Init();
	Sensors_Start();
	
	GPIO_EEPROM_WC_Init();
	Main_Get_Serial_Number(&BL_Serial_Number);
	

	
	// PWR init---------------------------------------

	
	PWR_Check_Previous_Mode();
	
	//Push Button Init--------------------------------
	
	IHM_PB_Init();

}

static void Run_Mode_Init_System (void)
{
	
	
	//SD init-----------------------------------------

//	Test_FATFS_Init(); 
	
  FATFS_Init();
	
	
	//Audio Init-------------------------------------- 
	
	Audio_Init();

	//Crypt Init-------------------------------------- 
	//The initialisation must be called after initalisation of sd card
	
	CRYPT_Init(1);
	
	// init interrupt line for sensors ---------------
	
	GPIO_AccGyr_INT_Init();
	GPIO_Monitor_Bat_INT_Init();
}

static void Run_Mode_Process (void)
{
		// Sauvegarde process
			
		Svgrd_Process();
		
		// PB process
		
		Tempo_PB_Process();
		
		// Crypt process
		
		Crypt_process();
		
		// wait for turning of the device
	
		if (Stop_Sys_Rqst == 1)
		{
			Stop_System();
			Stop_Sys_Rqst = 0 ;
		}
}

static void Charge_Mode_Init_System (void)
{
		#ifdef USE_USART1_FOR_WIFI
		UART_Init();
		#endif
	
	GPIO_BLE_Init();
	MX_BlueNRG_MS_Init();
	
	
}

static void Charge_Mode_Process (void)
{

	
	#ifdef USE_USART1_FOR_WIFI
	UART_Com_Wifi_Process();
	#endif
	RTC_Get_time_date(&RTC_Buff[0]);
	
	MX_BlueNRG_MS_Process();
	
	if ((Sensors_Bat_Is_Full() == 1) || charge_state == Bat_Charg_Complete)
	{
		Bat_Monitor_Stop();
			// Disable all used wakeup sources: WKUP pin (don't forget to add new wakeup pin charge-in)   
  HAL_PWR_DisableWakeUpPin(PWR_WAKEUP_PIN1);
  // ajouter ici ou mettre un ou !!!
	HAL_PWR_DisableWakeUpPin(PWR_WAKEUP_PIN2);
	// Clear wake up Flag
	__HAL_PWR_CLEAR_FLAG(PWR_FLAG_WUF1);
	// ajouter ici ou mettre un ou !!!
	__HAL_PWR_CLEAR_FLAG(PWR_FLAG_WUF2);

	// ajouter ici ou mettre un ou !!!
	HAL_PWR_EnableWakeUpPin(PWR_WAKEUP_PIN1_HIGH);
	//Set RTC back-up register RTC_BKP31R to indicate later on that system has entered shutdown mode
	 WRITE_REG( RTC->BKP31R, 0x1 );
	// Enter shutdown mode   
  HAL_PWREx_EnterSHUTDOWNMode();
	}
	
}

static void Stop_System(void)
{
	// Stop apps (call hear other functions which closes apps)
	Audio_Stop_Record();
	
	// stop sensors 
	
	Sensors_Stop();
	
	// Close files and unmount sd card 
	FATFS_Force_Stop();
}
void Buton_Hold_Callback (void)
{
	Stop_Sys_Rqst = 1;
}

void FATFS_Stop_System_SD_Space_PB (void)
{
	Stop_Sys_Rqst = 1;
}

void Bat_Monitor_Stop_System_Low_Battery(void)
{
	Stop_Sys_Rqst = 1;
}

void Sensor_Stop_System (void)
{
	Stop_Sys_Rqst = 1;
}

void BLE_Serial_Number_Recieved(uint16_t SN)
{
	BL_Serial_Number=SN;
	/*Save Serial number*/
	// Allow write access
	HAL_GPIO_WritePin (GPIOB, GPIO_PIN_0, GPIO_PIN_RESET);
	// write data
	I2C_WriteReg(EEPROM_ADDR, 0,(uint8_t*)& BL_Serial_Number, 2);
	
	//Set eeprom on Read only mode
	HAL_GPIO_WritePin (GPIOB, GPIO_PIN_0, GPIO_PIN_SET);
}

static void Main_Get_Serial_Number(uint16_t* SN)
{
	I2C_ReadReg(EEPROM_ADDR, 0, (uint8_t*)SN, 2);
}

/**
  * @brief System Clock Configuration
  * @retval None
  */
void SystemClock_Config(void)
{
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};
  RCC_PeriphCLKInitTypeDef PeriphClkInit = {0};

  /** Configure LSE Drive Capability 
  */
  HAL_PWR_EnableBkUpAccess();
  __HAL_RCC_LSEDRIVE_CONFIG(RCC_LSEDRIVE_LOW);
  /** Initializes the CPU, AHB and APB busses clocks 
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_LSE|RCC_OSCILLATORTYPE_MSI;
  RCC_OscInitStruct.LSEState = RCC_LSE_ON;
  RCC_OscInitStruct.MSIState = RCC_MSI_ON;
  RCC_OscInitStruct.MSICalibrationValue = 0;
  RCC_OscInitStruct.MSIClockRange = RCC_MSIRANGE_6;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_MSI;
  RCC_OscInitStruct.PLL.PLLM = 1;
  RCC_OscInitStruct.PLL.PLLN = 40;
  RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV7;
  RCC_OscInitStruct.PLL.PLLQ = RCC_PLLQ_DIV2;
  RCC_OscInitStruct.PLL.PLLR = RCC_PLLR_DIV2;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }
  /** Initializes the CPU, AHB and APB busses clocks 
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV1;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_4) != HAL_OK)
  {
    Error_Handler();
  }
  PeriphClkInit.PeriphClockSelection = RCC_PERIPHCLK_DFSDM1|RCC_PERIPHCLK_SDMMC1
																			| RCC_PERIPHCLK_RTC | RCC_PERIPHCLK_I2C4;
	
  
  PeriphClkInit.Sdmmc1ClockSelection = RCC_SDMMC1CLKSOURCE_PLLSAI1;
  PeriphClkInit.Dfsdm1ClockSelection = RCC_DFSDM1CLKSOURCE_PCLK;
	PeriphClkInit.RTCClockSelection = RCC_RTCCLKSOURCE_LSE;	
	PeriphClkInit.I2c2ClockSelection = RCC_I2C4CLKSOURCE_PCLK1;
	
  PeriphClkInit.PLLSAI1.PLLSAI1Source = RCC_PLLSOURCE_MSI;
  PeriphClkInit.PLLSAI1.PLLSAI1M = 1;
  PeriphClkInit.PLLSAI1.PLLSAI1N = 24;
  PeriphClkInit.PLLSAI1.PLLSAI1P = RCC_PLLP_DIV7;
  PeriphClkInit.PLLSAI1.PLLSAI1Q = RCC_PLLQ_DIV2;
  PeriphClkInit.PLLSAI1.PLLSAI1R = RCC_PLLR_DIV2;
  PeriphClkInit.PLLSAI1.PLLSAI1ClockOut = RCC_PLLSAI1_48M2CLK;
	
	
  if (HAL_RCCEx_PeriphCLKConfig(&PeriphClkInit) != HAL_OK)
  {
    Error_Handler();
  }
  /** Configure the main internal regulator output voltage 
  */
  if (HAL_PWREx_ControlVoltageScaling(PWR_REGULATOR_VOLTAGE_SCALE1) != HAL_OK)
  {
    Error_Handler();
  }
	
  /** Enable MSI Auto calibration 
  */
  HAL_RCCEx_EnableMSIPLLMode();
	
	HAL_NVIC_SetPriority(SysTick_IRQn, 0, 0);
}






/* USER CODE BEGIN 4 */

/* USER CODE END 4 */

/**
  * @brief  This function is executed in case of error occurrence.
  * @retval None
  */
void Error_Handler(void)
{
  /* USER CODE BEGIN Error_Handler_Debug */
  /* User can add his own implementation to report the HAL error return state */

  /* USER CODE END Error_Handler_Debug */
}

#ifdef  USE_FULL_ASSERT
/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(char *file, uint32_t line)
{ 
  /* USER CODE BEGIN 6 */
  /* User can add his own implementation to report the file name and line number,
     tex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
