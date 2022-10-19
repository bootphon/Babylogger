/******************** (C) COPYRIGHT 2015 STMicroelectronics ********************
* File Name          : gatt_db.c
* Author             : 
* Version            : V1.0.0
* Date               : 16-September-2015
* Description        : Functions to build GATT DB and handle GATT events.
********************************************************************************
* THE PRESENT FIRMWARE WHICH IS FOR GUIDANCE ONLY AIMS AT PROVIDING CUSTOMERS
* WITH CODING INFORMATION REGARDING THEIR PRODUCTS IN ORDER FOR THEM TO SAVE TIME.
* AS A RESULT, STMICROELECTRONICS SHALL NOT BE HELD LIABLE FOR ANY DIRECT,
* INDIRECT OR CONSEQUENTIAL DAMAGES WITH RESPECT TO ANY CLAIMS ARISING FROM THE
* CONTENT OF SUCH FIRMWARE AND/OR THE USE MADE BY CUSTOMERS OF THE CODING
* INFORMATION CONTAINED HEREIN IN CONNECTION WITH THEIR PRODUCTS.
*******************************************************************************/

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "bluenrg_def.h"
#include "gatt_db.h"
#include "bluenrg_conf.h"
#include "bluenrg_gatt_aci.h"

#include "IHM.h"

/** @brief Macro that stores Value into a buffer in Little Endian Format (2 bytes)*/
#define HOST_TO_LE_16(buf, val)    ( ((buf)[0] =  (uint8_t) (val)    ) , \
                                   ((buf)[1] =  (uint8_t) (val>>8) ) )

/** @brief Macro that stores Value into a buffer in Little Endian Format (4 bytes) */
#define HOST_TO_LE_32(buf, val)    ( ((buf)[0] =  (uint8_t) (val)     ) , \
                                   ((buf)[1] =  (uint8_t) (val>>8)  ) , \
                                   ((buf)[2] =  (uint8_t) (val>>16) ) , \
                                   ((buf)[3] =  (uint8_t) (val>>24) ) ) 

#define COPY_UUID_128(uuid_struct, uuid_15, uuid_14, uuid_13, uuid_12, uuid_11, uuid_10, uuid_9, uuid_8, uuid_7, uuid_6, uuid_5, uuid_4, uuid_3, uuid_2, uuid_1, uuid_0) \
do {\
    uuid_struct[0] = uuid_0; uuid_struct[1] = uuid_1; uuid_struct[2] = uuid_2; uuid_struct[3] = uuid_3; \
        uuid_struct[4] = uuid_4; uuid_struct[5] = uuid_5; uuid_struct[6] = uuid_6; uuid_struct[7] = uuid_7; \
            uuid_struct[8] = uuid_8; uuid_struct[9] = uuid_9; uuid_struct[10] = uuid_10; uuid_struct[11] = uuid_11; \
                uuid_struct[12] = uuid_12; uuid_struct[13] = uuid_13; uuid_struct[14] = uuid_14; uuid_struct[15] = uuid_15; \
}while(0)



/* BabyLogger caracteristics Service */
#define COPY_Baby_Logger_SERVICE_UUID(uuid_struct)   COPY_UUID_128(uuid_struct,0x73,0xd1,0x6f,0x9c,0x17,0xab,0x11,0xea,0x8d,0x71,0x36,0x2b,0x9e,0x15,0x56,0x67)
#define COPY_Baby_Logger_SerNum_CHAR_UUID(uuid_struct) COPY_UUID_128(uuid_struct,0x73,0xd1,0x6f,0x9d,0x17,0xab,0x11,0xea,0x8d,0x71,0x36,0x2b,0x9e,0x15,0x56,0x67)
#define COPY_Baby_Logger_Time_CHAR_UUID(uuid_struct) COPY_UUID_128(uuid_struct,0x73,0xd1,0x6f,0x9e,0x17,0xab,0x11,0xea,0x8d,0x71,0x36,0x2b,0x9e,0x15,0x56,0x67)
#define COPY_Baby_Logger_Date_CHAR_UUID(uuid_struct) COPY_UUID_128(uuid_struct,0x73,0xd1,0x6f,0x9f,0x17,0xab,0x11,0xea,0x8d,0x71,0x36,0x2b,0x9e,0x15,0x56,0x67)
#define COPY_Baby_Logger_Key_CHAR_UUID(uuid_struct)  COPY_UUID_128(uuid_struct,0x73,0xd1,0x6f,0xa0,0x17,0xab,0x11,0xea,0x8d,0x71,0x36,0x2b,0x9e,0x15,0x56,0x67)

/* Private variables*/

uint8_t Factory_Mode 						= 0;

uint8_t End_Serial_Number_Rx		= 0;
uint8_t End_Key_Rx							= 0;
uint8_t End_Time_Rx							= 0;
uint8_t End_Date_Rx							= 0;

/* Babylogger service & caracteristics handel*/

uint16_t BabyloggerServHandle, SerialNumerCharHandle, TimeCharHandle, DateCharHandle, KeyCharHandle;

/* UUIDS */
Service_UUID_t service_uuid;
Char_UUID_t char_uuid;



extern uint16_t connection_handle;
extern uint32_t start_time;
 


/**
 * @brief  Add BabyLogger Service to set serial number and time.
 * @param  None
 * @retval tBleStatus Status
 */
tBleStatus Add_BabyLogger_Service(void)
{
  tBleStatus ret;
  uint8_t uuid[16];
  
  /* Add_BabyLogger_Service */
  COPY_Baby_Logger_SERVICE_UUID(uuid);  
  BLUENRG_memcpy(&service_uuid.Service_UUID_128, uuid, 16);
  ret = aci_gatt_add_serv(UUID_TYPE_128, service_uuid.Service_UUID_128, PRIMARY_SERVICE, 
                          1+3*5, &BabyloggerServHandle); 
  if (ret != BLE_STATUS_SUCCESS) 
    return BLE_STATUS_ERROR;    
  
  /* Fill the Serial Number BLE Characteristc */
  COPY_Baby_Logger_SerNum_CHAR_UUID(uuid);
  BLUENRG_memcpy(&char_uuid.Char_UUID_128, uuid, 16);  
  ret =  aci_gatt_add_char(BabyloggerServHandle, UUID_TYPE_128, char_uuid.Char_UUID_128, 
                           2, 
                           CHAR_PROP_WRITE, 
                           ATTR_PERMISSION_AUTHOR_WRITE | ATTR_PERMISSION_AUTHEN_WRITE, 
                           GATT_NOTIFY_WRITE_REQ_AND_WAIT_FOR_APPL_RESP | GATT_NOTIFY_ATTRIBUTE_WRITE,
                           16, 0, &SerialNumerCharHandle);
  if (ret != BLE_STATUS_SUCCESS) 
    return BLE_STATUS_ERROR;
	
	  /* Fill the Serial Number BLE Characteristc */
  COPY_Baby_Logger_Key_CHAR_UUID(uuid);
  BLUENRG_memcpy(&char_uuid.Char_UUID_128, uuid, 16);  
  ret =  aci_gatt_add_char(BabyloggerServHandle, UUID_TYPE_128, char_uuid.Char_UUID_128, 
                           16, 
                           CHAR_PROP_WRITE, 
                           ATTR_PERMISSION_AUTHOR_WRITE | ATTR_PERMISSION_AUTHEN_WRITE, 
                           GATT_NOTIFY_WRITE_REQ_AND_WAIT_FOR_APPL_RESP | GATT_NOTIFY_ATTRIBUTE_WRITE,
                           16, 0, &KeyCharHandle);
  if (ret != BLE_STATUS_SUCCESS) 
    return BLE_STATUS_ERROR;

  /* Fill the Time BLE Characteristc */
  COPY_Baby_Logger_Time_CHAR_UUID(uuid);
  BLUENRG_memcpy(&char_uuid.Char_UUID_128, uuid, 16);  
  ret =  aci_gatt_add_char(BabyloggerServHandle, UUID_TYPE_128, char_uuid.Char_UUID_128, 
                           3, 
                           CHAR_PROP_WRITE, 
                           ATTR_PERMISSION_AUTHOR_WRITE | ATTR_PERMISSION_AUTHEN_WRITE, 
                           GATT_NOTIFY_WRITE_REQ_AND_WAIT_FOR_APPL_RESP | GATT_NOTIFY_ATTRIBUTE_WRITE,
                           16, 0, &TimeCharHandle);
  if (ret != BLE_STATUS_SUCCESS) 
    return BLE_STATUS_ERROR;

  /* Fill the Date BLE Characteristc */
  COPY_Baby_Logger_Date_CHAR_UUID(uuid);
  BLUENRG_memcpy(&char_uuid.Char_UUID_128, uuid, 16);  
  ret =  aci_gatt_add_char(BabyloggerServHandle, UUID_TYPE_128, char_uuid.Char_UUID_128, 
                           3, 
                           CHAR_PROP_WRITE, 
                           ATTR_PERMISSION_AUTHOR_WRITE | ATTR_PERMISSION_AUTHEN_WRITE, 
                           GATT_NOTIFY_WRITE_REQ_AND_WAIT_FOR_APPL_RESP | GATT_NOTIFY_ATTRIBUTE_WRITE,
                           16, 0, &DateCharHandle);
  if (ret != BLE_STATUS_SUCCESS) 
    return BLE_STATUS_ERROR;	
  
  return BLE_STATUS_SUCCESS;
}



/*******************************************************************************
* Function Name  : Write_Request_CB.
* Description    : Update the sensor valuse.
* Input          : connexion handel & attribut handel.
* Return         : None.
*******************************************************************************/
void Write_Request_CB(uint16_t Con_handle, uint16_t att_handle, uint8_t size, uint8_t* pData)
{
  tBleStatus ret;
  
  if(att_handle == SerialNumerCharHandle + 1)
  {  
		PRINTF("Serial number write request \n");
		if ((Factory_Mode == 1) && (End_Serial_Number_Rx == 0))
		{
			aci_gatt_write_response(Con_handle,att_handle,0,0,size,pData);
		}
		else
		{
			aci_gatt_write_response(Con_handle,att_handle,1,1,size,pData);
		}
  }
	
	else if(att_handle == KeyCharHandle + 1)
  {  
		PRINTF("Key write request \n");
		if ((Factory_Mode == 1) && ((End_Key_Rx == 0)))
		{
			aci_gatt_write_response(Con_handle,att_handle,0,0,size,pData);
		}
		else
		{
			aci_gatt_write_response(Con_handle,att_handle,1,1,size,pData);
		}
  }
	
	else if (att_handle == TimeCharHandle + 1 )
	{
		PRINTF("Time write request \n");
    aci_gatt_write_response(Con_handle,att_handle,0,0,size,pData);
	}
	else if (att_handle == DateCharHandle + 1 )	
	{
		PRINTF("Date write request \n");
    aci_gatt_write_response(Con_handle,att_handle,0,0,size,pData);
	}
 /* else if (Con_handle == EnvironmentalCharHandle + 1)
  {

  }

  if(connection_handle !=0)
  { 
		
    ret = aci_gatt_allow_read(connection_handle);
    if (ret != BLE_STATUS_SUCCESS)
    {
      PRINTF("aci_gatt_allow_read() failed: 0x%02x\r\n", ret); 
    }
  }*/
}

/*******************************************************************************
* Function Name  : Write_Done_CB.
* Description    : Update the sensor valuse.
* Input          : connexion handel & attribut handel & data size & data pointer.
* Return         : None.
*******************************************************************************/
void Write_Characteristic_CB(uint16_t Con_handle, uint16_t att_handle, uint8_t size, uint8_t* pData)
{
	/* Provisoir */ uint16_t Serial_Number = 0; 
	
	if(att_handle == SerialNumerCharHandle + 1)
  {    
    PRINTF("Serial number write done \n");
		Serial_Number &= 0x0000;
		Serial_Number |= (pData[1]<<8);
		Serial_Number |= pData[0];
		PRINTF("Device Serial Number : %d \n",Serial_Number);
		
		/* Set serial number*/
		BLE_Serial_Number_Recieved(Serial_Number);		
		End_Serial_Number_Rx = 1;	/* Update flag */
		
		/* Check the end of factory setting */
		if((End_Serial_Number_Rx == 1) && (End_Key_Rx == 1))
		{
			IHM_LED_Start(LED_PWM_Mode,LED1_2);
			Factory_Mode = 0;
			End_Serial_Number_Rx = 0;
			End_Key_Rx = 1;
		}
  }
	
	if(att_handle == KeyCharHandle + 1)
  {    
    PRINTF("Key write done \n");
		
		/* Check if the key received have been correctly saved in th eeprom*/

		if(BLE_Key_Received(pData) == 0)
		{
			End_Key_Rx = 1;	/* Update flag */	
		}
		
		if((End_Serial_Number_Rx == 1) && (End_Key_Rx == 1))
		{
			IHM_LED_Start(LED_PWM_Mode,LED1_2);
			Factory_Mode = 0;
			End_Serial_Number_Rx = 0;
			End_Key_Rx = 1;
		}
  }
	else if (att_handle == TimeCharHandle + 1 )
	{
		PRINTF("Time write done \n");
		PRINTF("Time: %02d:%02d:%02d \n",pData[0],pData[1],pData[2]);
		BLE_Time_Recieved(pData);
		End_Time_Rx = 1;
	}
	else if (att_handle == DateCharHandle + 1 )	
	{
		PRINTF("Date write done \n");
		PRINTF("Date: %02d/%02d/%04d \n",pData[2],pData[1],(pData[0]+2000));
		BLE_Date_Recieved(pData);
		End_Date_Rx = 1;
	}
	
}

uint8_t BLE_Task_Is_End(void)
{
	if ((Factory_Mode == 0) && (End_Date_Rx == 1) && (End_Time_Rx ==1)) 
	{
		return 1;
	}
	else 
	{
		return 0;
	}
}
uint16_t BLE_Get_Connection_Handle(void)
{
	return connection_handle; 
}
void IHM_PB_Go_To_Factory_Mode(void)
{
	Factory_Mode = 1;
	IHM_LED_Start(LED_Continious_Mode,LED1_2);
}

__weak void BLE_Serial_Number_Recieved(uint16_t SN)
{
	
}

__weak uint8_t BLE_Key_Received(uint8_t* pData)
{
	
}

__weak void BLE_Time_Recieved(uint8_t* pData)
{
	
}

__weak void BLE_Date_Recieved(uint8_t* pData)
{
	
}

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
