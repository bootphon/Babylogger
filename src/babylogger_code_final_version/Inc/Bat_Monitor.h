#include "stm32l4xx_hal.h"




/*Threshold application parameters define  -------------------------------------------------- */

#define Bat_Min 	103
#define Bat_Max		1000

/*Prototype  -------------------------------------------------------------------------------- */

void Bat_Monitor_AlarmSetSOCThreshold(int Soc_Value);
void Bat_Monitor_AlarmClear(void);
void Bat_Monitor_AlarmDisable(void);
void Bat_Monitor_AlarmEnable(void);
void Bat_Monitor_Update_Values (void);
uint8_t Sensors_Init_Check_Bat(void);
uint8_t Sensors_Bat_Is_Full(void);
int Sensors_Get_State_of_Charge_Progress(void);
void Sensors_Callback_STC3115(void);
__weak void Bat_Monitor_Set_Flag_Low_Battery(void);
__weak void Bat_Monitor_Stop_System_Low_Battery(void);
void Bat_Monitor_Stop(void);
void Bat_Monitor_Init(void);



