/**********************
* Team : Coml
**********************/

#include "Acc_Gyr.h"
#include "LSM6DSL_motion_sensor.h"
#include "LSM6DSL_motion_sensor_ex.h"


/* Private prototypes ---------------------------------------------------------*/

/* Private variables ---------------------------------------------------------*/

int16_t 				AccGyrBuff[Size_Acc_Gyr_Buff];

/* Global variables ----------------------------------------------------------*/

int16_t*			  pAccGyrBuff_Half 	= &AccGyrBuff[0];
int16_t*			  pAccGyrBuff_Cplt 	= &AccGyrBuff[GYR_BUFF_HALF];



/*******************************************************************************
* Function Name  : Acc_Gyr_Set_DRDY_XL_G
* Description    : function used to set interrupt feature (INT1) 
* Input          : val : 0 use INT1 for Accelerometer.
			 1 use INT1 for Gyroscope.
			 2 use INT1 for Accelerometer and Gyroscope.
			 3 desable INT1 for Accelerometer and Gyroscope.
* Return         : No returned value. 
*******************************************************************************/

void Acc_Gyr_Set_DRDY_XL_G(uint8_t val){
	uint8_t Reg;
	
	/* Read control register of interupt 1 */
	LSM6DSL_MOTION_SENSOR_Read_Register(LSM6DSL_INT1_CTRL, &Reg);
	
	/* Prepare data to be write to configure INT1, and writing it for the needed case */
	
	switch (val){
		
		case 0 : /* Enable interrupt for Acc only */ 
			Reg |= 0x01;
			LSM6DSL_MOTION_SENSOR_Write_Register(LSM6DSL_INT1_CTRL, Reg);
		break;
		
		case 1 : /* Enable interrupt for Gyr only */
			Reg |= 0x02;
			LSM6DSL_MOTION_SENSOR_Write_Register(LSM6DSL_INT1_CTRL, Reg);
		break;
		
		case 2 : /* Enable interrupt for Acc and Gyr */
			Reg |= 0x03;
			LSM6DSL_MOTION_SENSOR_Write_Register(LSM6DSL_INT1_CTRL, Reg);
		break;
		
		case 3 : /* Disable interrupt for Acc & Gyr */ 
			Reg &= 0x00;
			LSM6DSL_MOTION_SENSOR_Write_Register(LSM6DSL_INT1_CTRL, Reg);
		break; 
	}
}



/*******************************************************************************
* Function Name  : Acc_Gyr_Set_DRDY_PulsedMode
* Description    : Setting the interrupt's signal as a pulse.
* Input          : No input.
* Return         : No returned value. 
*******************************************************************************/
void Acc_Gyr_Set_DRDY_PulsedMode(void){
	uint8_t Reg;
	
	/* Read the value of the register  (for debug)  */
	LSM6DSL_MOTION_SENSOR_Read_Register(LSM6DSL_DRDY_PULSE_CFG_G , &Reg);
	
	
	Reg = 0x80; /* value to write to generate pulses of 75µs when data are ready */
	LSM6DSL_MOTION_SENSOR_Write_Register(LSM6DSL_DRDY_PULSE_CFG_G , Reg); /* writig in register*/
}


/*******************************************************************************
* Function Name  : Acc_Gyr_Acc_RAW_Process
* Description    : Convert accelerometer's raw data to mg.
* Input          : Raw data.
* Return         : Data (mg). 
*******************************************************************************/
float Acc_Gyr_Acc_RAW_Process(int16_t rawdata)
{
	/* Conversion of data*/ 
	float data =(int32_t)((float)((float)rawdata * LSM6DSL_ACC_SENSITIVITY_FS_2G));
	
	/* Return the converted data */ 
	return data;
}

/*******************************************************************************
* Function Name  : Acc_Gyr_Is_Data_Ready
* Description    : Find origine of the interrupt "data ready".
* Input          : No input.
* Return         : return : 1 if the data of the accelerometer only are ready.
			    2 if the data of the gyroscope only are ready.
			    3 if the data of both of them are ready.
*******************************************************************************/
uint8_t Acc_Gyr_Is_Data_Ready(void)
{
	uint8_t Reg;
	uint8_t Tmp;
	
	/* Read status register */
	LSM6DSL_MOTION_SENSOR_Read_Register(LSM6DSL_STATUS_REG, &Reg);
	
	Tmp = Reg & 0x03; /* Aplly Mask */ 
	
	return Tmp; /* Return the status */ 
}


/*******************************************************************************
* Function Name  : Acc_Gyr_Get_Timestamp
* Description    : Fill the buffer with timestamp.
* Input          : Buffer of timestamp, Index of data buffer .
* Return         : No returned value.
*******************************************************************************/

void Acc_Gyr_Get_Timestamp(uint8_t* Buff_Timestamp, uint16_t Index_Gyr)
{
	/* Write the timestamp in the buffer (Before data of Gyr & Acc )*/
	AccGyrBuff[Index_Gyr]  = ((0x0000 | (Buff_Timestamp[1]<<8)) | ((int16_t) Buff_Timestamp[0]));
	AccGyrBuff[Index_Gyr+1]= ((0x0000 | (Buff_Timestamp[3]<<8)) | ((int16_t) Buff_Timestamp[2]));
}

/*******************************************************************************
* Function Name  : Acc_Gyr_Get_Gyr_data
* Description    : Fill the buffer with data of gyroscope.
* Input          : Index of data buffer.
* Return         : No returned value.
*******************************************************************************/
void Acc_Gyr_Get_Gyr_data(uint16_t Index_Gyr)
{
	SENSOR_DMA_Start(LSM6DSL_I2C_ADD_L,LSM6DSL_OUTX_L_G, I2C_MEMADD_SIZE_8BIT, (uint8_t*)&AccGyrBuff[Index_Gyr], 6);
}

/*******************************************************************************
* Function Name  : Acc_Gyr_Get_Acc_data
* Description    : Fill the buffer with data of accelerometer.
* Input          : Index of data buffer.
* Return         : No returned value.
*******************************************************************************/
void Acc_Gyr_Get_Acc_data(uint16_t Index_Acc)
{
	SENSOR_DMA_Start(LSM6DSL_I2C_ADD_L,LSM6DSL_OUTX_L_XL, I2C_MEMADD_SIZE_8BIT, (uint8_t*)&AccGyrBuff[Index_Acc], 6);
}

/*******************************************************************************
* Function Name  : Acc_Gyr_Stop
* Description    : Disable motion sensing feature.
* Input          : No input.
* Return         : No returned value.
*******************************************************************************/

void Acc_Gyr_Stop(void) 
{
	LSM6DSL_MOTION_SENSOR_Disable(MOTION_ACCELERO | MOTION_GYRO);
}

/*******************************************************************************
* Function Name  : Acc_Gyr_Start
* Description    : Enable motion sensing feature.
* Input          : No input.
* Return         : No returned value.
*******************************************************************************/
void Acc_Gyr_Start(void)
{
	LSM6DSL_MOTION_SENSOR_Enable(MOTION_ACCELERO | MOTION_GYRO);
}

/*******************************************************************************
* Function Name  : Acc_Gyr_Init
* Description    : Initialize motion sensing feature.
* Input          : No input.
* Return         : No returned value.
*******************************************************************************/
void Acc_Gyr_Init(void)
{
		
	LSM6DSL_MOTION_SENSOR_Init(MOTION_ACCELERO); /* Initialize accelerometer */
	LSM6DSL_MOTION_SENSOR_Init(MOTION_GYRO); /* Initialize gyroscope */
	
	LSM6DSL_MOTION_SENSOR_SetOutputDataRate( MOTION_ACCELERO , 52); /* Set accelerometer's output data rate */
	LSM6DSL_MOTION_SENSOR_SetOutputDataRate( MOTION_GYRO, 52); 	/* Set gyroscope's output data rate */
			
	Acc_Gyr_Set_DRDY_PulsedMode(); /* Set interrupt in pulsed mode */
	
	/* Reset interupt register */
	Acc_Gyr_Set_DRDY_XL_G(3);	/* Disable Interruprs */
	Acc_Gyr_Set_DRDY_XL_G(2);	/* Enable interrupt for accelerometer and gyroscope */
	
}

/*******************************************************************************
* Function Name  : Acc_Gyr_DeInit
* Description    : DeInitialize motion sensing feature.
* Input          : No input.
* Return         : No returned value.
*******************************************************************************/
void Acc_Gyr_DeInit(void)
{
	LSM6DSL_MOTION_SENSOR_DeInit();
	
}
