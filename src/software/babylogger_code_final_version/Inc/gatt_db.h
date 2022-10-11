
#ifndef _GATT_DB_H_
#define _GATT_DB_H_

#include <stdint.h>
#include <stdlib.h>
#include "bluenrg_def.h"



/** 
 * @brief Number of application services
 */
//#define NUMBER_OF_APPLICATION_SERVICES (2)
   


/** Documentation for C union Service_UUID_t */
typedef union Service_UUID_t_s {
  /** 16-bit UUID 
  */
  uint16_t Service_UUID_16;
  /** 128-bit UUID
  */
  uint8_t Service_UUID_128[16];
} Service_UUID_t;

/** Documentation for C union Char_UUID_t */
typedef union Char_UUID_t_s {
  /** 16-bit UUID 
  */
  uint16_t Char_UUID_16;
  /** 128-bit UUID
  */
  uint8_t Char_UUID_128[16];
} Char_UUID_t;

tBleStatus Add_BabyLogger_Service(void);
void Write_Request_CB(uint16_t Con_handle, uint16_t att_handle, uint8_t size, uint8_t* pData);
void Write_Characteristic_CB(uint16_t Con_handle, uint16_t att_handle, uint8_t size, uint8_t* pData);
uint8_t BLE_Task_Is_End(void);
uint16_t BLE_Get_Connection_Handle(void);
__weak void BLE_Serial_Number_Recieved(uint16_t SN);
__weak uint8_t BLE_Key_Received(uint8_t* pData);
__weak void BLE_Time_Recieved(uint8_t* pData);
__weak void BLE_Date_Recieved(uint8_t* pData);
extern uint8_t Services_Max_Attribute_Records[];

#endif /* _GATT_DB_H_ */
