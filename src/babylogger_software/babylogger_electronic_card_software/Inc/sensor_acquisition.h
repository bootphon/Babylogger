#include "stm32l4xx_hal.h"
#include "Acc_Gyr.h"
#include "Bat_Monitor.h"
#include "Temp_Hum.h"



/*Prototype  -------------------------------------------------------------------------------- */


void Sensors_Acti_Get_Debug_Variables(uint8_t* pDebeugVar);
__weak void Motion_Buff_Half_Ready(void);
__weak void Motion_Buff_Cplt_Ready(void);
void Acti_Init_Debug_Variables(void);
void Acti_Init_Acquisition_Variables(void);
void Sensors_Init(void);
void Sensors_Start(void);
void Sensors_Process(void);
void Sensors_Stop(void);
void Sensors_DeInit(void);
void Sensors_Callback_LSM6DSL(void);
void Tim3_Stop(void);
void Tim3_Start(void);
void Tim3_Init(void);
void Tim3_DeInit(void);
__weak void Sensor_Stop_System (void);



