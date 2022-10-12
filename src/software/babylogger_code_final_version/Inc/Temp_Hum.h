#include "stm32l4xx_hal.h"



/*Threshold application parameters define  -------------------------------------------------- */

#define Temp_Max	120
#define TEMP_SEUIL 120

/*Prototype  -------------------------------------------------------------------------------- */

uint8_t Temp_Hum_Init_Check_Temperature(void);
void Temp_Hum_Get_Temperature(void);
__weak void Temp_Hum_Overheating_Devive(void);
void Temp_Hum_Stop(void);
void Temp_Hum_Start(void);
void Temp_Hum_Init(void);
void Temp_Hum_DeInit(void);




