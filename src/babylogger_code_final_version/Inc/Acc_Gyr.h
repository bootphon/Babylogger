#include "stm32l4xx_hal.h"


/*Buffer parameters define  ---------------------------------------------------------------- */

/* The buffer is an array of signed 16 bits integer  */ 

/* AccGyr_Data_Unit represent the number of unit of data in the buffer.
	 Each unit is consists of 4 bytes for timestamp, 6 bytes for Gyr data and 
	 6 bytes for Acc data (The sum is 16 bytes ==> equivalent to 8 cells of the buffer)*/
#define 	AccGyr_Data_Unit   	128 
				
#define 	Size_Acc_Gyr_Buff  (AccGyr_Data_Unit * 8)	/* Number of cells in the buffer*/

#define 	GYR_BUFF_HALF 		 (Size_Acc_Gyr_Buff / 2)	/* Index of Gyr acquisition when the buffer is half full */
#define 	GYR_BUFF_CPLT 		 (Size_Acc_Gyr_Buff)			/* Index of Gyr acquisition when the buffer is full */
#define 	ACC_BUFF_HALF 		 (GYR_BUFF_HALF + 5)			/* Index of Acc acquisition when the buffer is half full */
#define 	ACC_BUFF_CPLT 		 (GYR_BUFF_CPLT + 5)			/* Index of Acc acquisition when the buffer is full */

/*Prototype  -------------------------------------------------------------------------------- */

void Acc_Gyr_Set_DRDY_XL_G(uint8_t val);
void Acc_Gyr_Set_DRDY_PulsedMode(void);
float Acc_Gyr_Acc_RAW_Process(int16_t rawdata);
uint8_t Acc_Gyr_Is_Data_Ready(void);
void Acc_Gyr_Get_Timestamp(uint8_t* Buff_Timestamp, uint16_t Index_Gyr);
void Acc_Gyr_Get_Gyr_data(uint16_t Index_Gyr);
void Acc_Gyr_Get_Acc_data(uint16_t Index_Acc);
void Acc_Gyr_Stop(void);
void Acc_Gyr_Start(void);
void Acc_Gyr_Init(void);
void Acc_Gyr_DeInit(void);


