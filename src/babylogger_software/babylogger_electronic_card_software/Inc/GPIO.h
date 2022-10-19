#include "stm32l4xx_hal.h"


void GPIO_AccGyr_INT_Init(void);
void GPIO_Monitor_Bat_INT_Init(void);
void GPIO_BLE_Init(void);
void GPIO_EEPROM_WC_Init(void);
void GPIO_Wifi_Init(void);
void GPIO_Wifi_INT_Init (void);
void GPIO_Wifi_Off (void);
void GPIO_Wifi_On (void);
void GPIO_Wifi_Force_Reset(void);
void GPIO_Wifi_Process (void);


void GPIO_BAT_Charg_Init(void);
void GPIO_BAT_Charg_PG_Detect_Callback(void);
uint8_t GPIO_BAT_Charg_Get_State (void);


#define Bat_Charg_Shutdown						0 
#define Bat_Charg_Standby							1
#define Bat_Charg_In_Progress					2
#define Bat_Charg_Complete						3
#define Bat_Charg_Sys_Test_Mode				4



