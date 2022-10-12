#include "stm32l4xx_hal.h"




#define Global_App_Run_Mode				0x00  		
#define Global_App_Off_Mode				0x01
#define Global_App_LowPower_Mode 	0x02
#define Global_App_Charge_Mode		0x03
#define Global_App_first_Init			0x04

// prototypes 

void PWR_Init(void);
void PWR_Reset_System(void);
void PWR_Go_To_Shutdown_Mode(void);
void PWR_Check_Previous_Mode(void);

