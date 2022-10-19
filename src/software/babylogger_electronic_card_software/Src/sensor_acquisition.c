#include "sensor_acquisition.h"
#include "RTC.h"
#include "I2C_Com.h"
#include "IHM.h"

/* Private typedef -----------------------------------------------------------*/

TIM_HandleTypeDef TimHandle;

/* Private variables ---------------------------------------------------------*/

uint16_t 				Index_Gyr 				= 0;
uint16_t 				Index_Acc 				= 5;

uint8_t					RTC_Buff_Acti[7] = {0};

/* Flags ---------------------------------------------------------------------*/

uint8_t 				Gyr_Buff_Cplt 	= 0;
uint8_t 				Gyr_Buff_Half 	= 0;
uint8_t 				Acc_Buff_Cplt		= 0;
uint8_t 				Acc_Buff_Half		= 0;

uint8_t 				data_ready_acc 	= 0;
uint8_t 				data_ready_gyro = 0;

uint8_t 				check_status_drdy_rqst_env = 0;
uint8_t 				check_status_drdy_rqst_mov = 0;

uint8_t 				check_temp = 0;
uint8_t 				low_battery = 0;

/* variables of debeug  -------------------------------------------------------*/

uint32_t DBG_Gyr_half = 0;
uint32_t DBG_Acc_half = 0;

uint32_t DBG_Gyr_cplt = 0;
uint32_t DBG_Acc_cplt = 0;

uint32_t DBG_GyrAcc_half = 0;
uint32_t DBG_GyrAcc_cplt = 0;
uint32_t DBG_INT_Motion  = 0;

uint32_t i1 = 0;
uint32_t i2 = 0;
uint32_t i3 = 0;

/*******************************************************************************
* Function Name  : Sensors_Acti_Get_Debug_Variables
* Description    : Write debug variable in header buffer.
* Input          : pointer to Buffer
* Return         : No returned value. 
*******************************************************************************/
void Sensors_Acti_Get_Debug_Variables(uint8_t* pDebeugVar)
{
	
	pDebeugVar[0]  = (uint8_t) 	DBG_Gyr_half;
	pDebeugVar[1]  = (uint8_t) ((DBG_Gyr_half >> 8)  & 0x000000FF);
	pDebeugVar[2]  = (uint8_t) ((DBG_Gyr_half >> 16) & 0x000000FF);
	pDebeugVar[3]  = (uint8_t) ((DBG_Gyr_half >> 24) & 0x000000FF);
	
	pDebeugVar[4]  = (uint8_t)	DBG_Acc_half;
	pDebeugVar[5]  = (uint8_t) ((DBG_Acc_half >> 8)  & 0x000000FF);
	pDebeugVar[6]  = (uint8_t) ((DBG_Acc_half >> 16) & 0x000000FF);
	pDebeugVar[7]  = (uint8_t) ((DBG_Acc_half >> 24) & 0x000000FF);
	
	pDebeugVar[8]  = (uint8_t)	DBG_Gyr_cplt;
	pDebeugVar[9]  = (uint8_t) ((DBG_Gyr_cplt >> 8)  & 0x000000FF);
	pDebeugVar[10] = (uint8_t) ((DBG_Gyr_cplt >> 16) & 0x000000FF);
	pDebeugVar[11] = (uint8_t) ((DBG_Gyr_cplt >> 24) & 0x000000FF);
	
	pDebeugVar[12] = (uint8_t)	DBG_Acc_cplt;
	pDebeugVar[13] = (uint8_t) ((DBG_Acc_cplt >> 8)  & 0x000000FF);
	pDebeugVar[14] = (uint8_t) ((DBG_Acc_cplt >> 16) & 0x000000FF);
	pDebeugVar[15] = (uint8_t) ((DBG_Acc_cplt >> 24) & 0x000000FF);
	
	
	pDebeugVar[16] = (uint8_t)	DBG_GyrAcc_half;
	pDebeugVar[17] = (uint8_t) ((DBG_GyrAcc_half >> 8)  & 0x000000FF);
	pDebeugVar[18] = (uint8_t) ((DBG_GyrAcc_half >> 16) & 0x000000FF);
	pDebeugVar[19] = (uint8_t) ((DBG_GyrAcc_half >> 24) & 0x000000FF);
	
	
	pDebeugVar[20] = (uint8_t)	DBG_GyrAcc_cplt;
	pDebeugVar[21] = (uint8_t) ((DBG_GyrAcc_cplt >> 8)  & 0x000000FF);
	pDebeugVar[22] = (uint8_t) ((DBG_GyrAcc_cplt >> 16) & 0x000000FF);
	pDebeugVar[23] = (uint8_t) ((DBG_GyrAcc_cplt >> 24) & 0x000000FF);
	
	pDebeugVar[24] = (uint8_t)	DBG_INT_Motion;
	pDebeugVar[25] = (uint8_t) ((DBG_INT_Motion >> 8)  & 0x000000FF);
	pDebeugVar[26] = (uint8_t) ((DBG_INT_Motion >> 16) & 0x000000FF);
	pDebeugVar[27] = (uint8_t) ((DBG_INT_Motion >> 24) & 0x000000FF);
	

	
}



/*******************************************************************************
* Function Name  : Motion_Buff_Half_Ready
* Description    : To be redefined where the inforamtion about state acquisition
									 are needed.
* Input          : No input.
* Return         : No returned value. 
*******************************************************************************/
__weak void Motion_Buff_Half_Ready(void)
{
	// redefined in fatfs.c
}

/*******************************************************************************
* Function Name  : Motion_Buff_Cplt_Ready
* Description    : To be redefined where the inforamtion about state acquisition
									 are needed.
* Input          : No input.
* Return         : No returned value. 
*******************************************************************************/

__weak void Motion_Buff_Cplt_Ready(void)
{
	// redefined in fatfs.c
}

/*******************************************************************************
* Function Name  : Acti_Init_Debug_Variables
* Description    : reset debug variable at the beginig of new file.
* Input          : No input.
* Return         : No returned value. 
*******************************************************************************/
void Acti_Init_Debug_Variables(void)
{
	DBG_Gyr_half = 0;
	DBG_Acc_half = 0;

	DBG_Gyr_cplt = 0;
	DBG_Acc_cplt = 0;

	DBG_GyrAcc_half = 0;
	DBG_GyrAcc_cplt = 0;
	DBG_INT_Motion  = 0;
}

/*******************************************************************************
* Function Name  : Acti_Init_Debug_Variables
* Description    : Reset Acquisition (no need to use it in countinious mode).
* Input          : No input.
* Return         : No returned value. 
*******************************************************************************/
void Acti_Init_Acquisition_Variables(void)
{
	/* This functions is used at the beginnig of an acquisition.
		 No need to call it between two consecutifs files because 
		 the size of the buffer allows us to close file and open a 
		 new one and have a continious acquisition splited in 
		 successifs files */ 
	
	Index_Gyr 				= 0;
  Index_Acc 				= 5;
	
	Gyr_Buff_Cplt 		= 0;
	Gyr_Buff_Half 		= 0;
	Acc_Buff_Cplt			= 0;
	Acc_Buff_Half			= 0;
	
}
/*******************************************************************************
* Function Name  : Sensors_Process
* Description    : Handle the sensors acquisition.
* Input          : No input.
* Return         : No returned value. 
*******************************************************************************/

void Sensors_Process(void) 																																							
{
	uint8_t Tmp;
				

	if (check_status_drdy_rqst_mov == 1) /* Handle data status request */
	{
			
		Tmp = Acc_Gyr_Is_Data_Ready(); /* Finde the origine off "data ready" interrupt  */
			
		switch(Tmp)
		{
			/* i1, i2 and i3 are used for debug. they allow us to check that Acc & Gyr are
			synchronized. This acquisition works with a same output data rate for Acc & Gyr */
				
				
			case 1:
				i1++;
				data_ready_acc = 1; /* Notify that new data of acceleration are ready to be read */ 
				RTC_Get_time_date(&RTC_Buff_Acti[0]); /* Update timestamp */ 
			break;
				
			case 2:
				i2++;
				data_ready_gyro =1;	/* Notify that new gyroscopic data are ready to be read */
				RTC_Get_time_date(&RTC_Buff_Acti[0]);  /* Update timestamp */ 
			break;
				
			case 3:
				i3++;
				data_ready_acc = 1; /* Notify that new data of acceleration are ready to be read */ 
				data_ready_gyro =1; /* Notify that new gyroscopic data are ready to be read */
				RTC_Get_time_date(&RTC_Buff_Acti[0]); /* Update timestamp */
				break;
			}
			check_status_drdy_rqst_mov = 0;  /* Reset the flag (data ready checked) */
		}
		
		
		if (data_ready_gyro == 1)  /* handle timestamp and gyroscope data ready */
		{

			Acc_Gyr_Get_Timestamp(&RTC_Buff_Acti[0],Index_Gyr); /* Copy timestamp buffer in the acquisition buffer */ 
			Index_Gyr += 2; /* Increment index of acquisition buffer (4 bytes for timestamp = 2 Cells) */ 
			
			Acc_Gyr_Get_Gyr_data(Index_Gyr); /* Get gyroscope data from sensor, and store it in acquisition buffer */ 
			Index_Gyr += 6;	/* Increment index of acquisition buffer */
			
			data_ready_gyro = 0; /* Update flag */
				
			/* handle double buffering for the gyroscope */
			if (Index_Gyr == GYR_BUFF_CPLT) /* Check if the second part of the buffer have been completely filled */ 
			{
				DBG_Gyr_cplt++;		/* Increment debug variable */
				Gyr_Buff_Cplt = 1;		/* gyroscopic data of the second buffer are ready to be saved in sd card */
				Index_Gyr = 0;		/* Satart to fill the first buffer */
			}
			if (Index_Gyr == GYR_BUFF_HALF) /* Check if the first part of the buffer have been completely filled */ 
			{
				DBG_Gyr_half++;		/* Increment debug variable */
				Gyr_Buff_Half = 1;		/* gyroscopic data of the first buffer are ready to be saved in sd card */
			}
			
		}
		if (data_ready_acc == 1 )		/* handle accelorometric data ready */
		{

			Acc_Gyr_Get_Acc_data(Index_Acc);	/* Get accelorometric data from sensor, and store it in acquisition buffer */ 
			Index_Acc += 8; /* Increment index of acquisition buffer */

			data_ready_acc = 0;		/* Update flag */
			
			/* handle double buffering for the accelerometer */
			
			if (Index_Acc == ACC_BUFF_CPLT) /* Check if the second part of the buffer have been completely filled */
			{
				DBG_Acc_cplt++;		/* Increment debug variable */
				Acc_Buff_Cplt = 1;	/* acclerometric data of the second buffer are ready to be saved in sd card */
				Index_Acc = 5;	/* Satart to fill the first buffer */
			}
			if (Index_Acc == ACC_BUFF_HALF)		/* Check if the first part of the buffer have been completely filled */ 
			{
				DBG_Acc_half++;		/* Increment debug variable */
				Acc_Buff_Half = 1;	/* accelerometric data of the first buffer are ready to be saved in sd card */	
			}
		}
		
		if ((Gyr_Buff_Cplt == 1) && (Acc_Buff_Cplt == 1)) /* Check if the second buffer is ready */
		{
			/* Update flag */
			Gyr_Buff_Cplt = 0;
			Acc_Buff_Cplt = 0;
		
			Motion_Buff_Cplt_Ready();		/* Handle complete second buffer */
			
			DBG_GyrAcc_cplt++;	/* Increment debug variable */
		}
		
		if ((Gyr_Buff_Half == 1) && (Acc_Buff_Half == 1))
		{
			/* Update flag */ 		
			Gyr_Buff_Half = 0;
			Acc_Buff_Half = 0;
			
			Motion_Buff_Half_Ready();		/* Handle complete first buffer */
			
			DBG_GyrAcc_half++;	/* Increment debug variable */			
		}
		
		if(check_temp == 1)	/* Handle internal operating sensor */
		{
			
			Temp_Hum_Get_Temperature(); /* Handle temperature sensor */ 
			Bat_Monitor_Update_Values();	/* Handle battery monitor  */ 
			
			check_temp = 0; /* Update flag */
		}
		 
		if (low_battery == 1) /* Handle low battery state */ 
		{
			//
			Bat_Monitor_AlarmClear();
			Bat_Monitor_AlarmDisable();
			low_battery++; /* Update flag */
			IHM_LED_Start(LED_PWM_Mode, LED1);
			Bat_Monitor_AlarmSetSOCThreshold (10);
			Bat_Monitor_AlarmEnable();
		}
		if (low_battery == 3) /* Handle low battery state */ 
		{
			Sensor_Stop_System(); /* Start turnig off the device */
		}
}

__weak void Sensor_Stop_System (void)
{
	
}

/*******************************************************************************
* Function Name  : Bat_Monitor_Set_Flag_Low_Battery
* Description    : Re-defenition of weak function (callback interrupt battery 
									 monitor).
* Input          : No input.
* Return         : No returned value. 
*******************************************************************************/
void Bat_Monitor_Set_Flag_Low_Battery(void)
{
	low_battery ++;
}



/*******************************************************************************
* Function Name  : Sensors_Callback_LSM6DSL
* Description    : Callback motion data ready.
* Input          : No input.
* Return         : No returned value. 
*******************************************************************************/
void Sensors_Callback_LSM6DSL(void)
{
	check_status_drdy_rqst_mov = 1; /* Data ready request */
	DBG_INT_Motion++;		/* Increment debeug flag */
}

/*******************************************************************************
* Function Name  : HAL_TIM_PeriodElapsedCallback
* Description    : Re-defenition of Timer Callback for managing sensors. 
* Input          : No input.
* Return         : No returned value. 
*******************************************************************************/

void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
	check_temp = 1;
}

/*******************************************************************************
* Function Name  : Sensors_Stop
* Description    : Disable sensors acquisition. 
* Input          : No input.
* Return         : No returned value. 
*******************************************************************************/
void Sensors_Stop(void)
{
	Acc_Gyr_Stop(); 	/* Disable motion sensor */
	Temp_Hum_Stop();	/* Disable environment sensor */
	Bat_Monitor_Stop(); /* Disable Battery monitor */
	Tim3_Stop();	/* Disable timer in charge of sampling */
}

/*******************************************************************************
* Function Name  : Tim3_Stop
* Description    : Interface : Disable Timer 3. 
* Input          : No input.
* Return         : No returned value. 
*******************************************************************************/
void Tim3_Stop(void)
{
	HAL_TIM_Base_Stop_IT(&TimHandle);
}

/*******************************************************************************
* Function Name  : Tim3_Start
* Description    : Interface :Enable Timer 3 in interrupt mode. 
* Input          : No input.
* Return         : No returned value. 
*******************************************************************************/
void Tim3_Start(void)
{
	HAL_TIM_Base_Start_IT(&TimHandle);
}

/*******************************************************************************
* Function Name  : Sensors_Start
* Description    : Enable sensors acquisition. 
* Input          : No input.
* Return         : No returned value. 
*******************************************************************************/
void Sensors_Start(void){
	Acc_Gyr_Start();		/* Enable motion sensor */ 
	Temp_Hum_Start();		/* Enable enviroment sensor */
	Tim3_Start();			/* Enable Timer 3 for sampling (Battery state & temperature */
}



/*******************************************************************************
* Function Name  : Sensors_Init
* Description    : Initialize peripherals and sensors in charge of the acquisition. 
* Input          : No input.
* Return         : No returned value. 
*******************************************************************************/
void Sensors_Init(void)
{
	I2C_Init();
	Acc_Gyr_Init();
	Bat_Monitor_Init();
	Temp_Hum_Init();
	Tim3_Init();
}

/*******************************************************************************
* Function Name  : Tim3_Init
* Description    : Initialize timer 3 to interrupt the cpu evry 30 sec. 
* Input          : No input.
* Return         : No returned value. 
*******************************************************************************/
void Tim3_Init(void)
{	
	TimHandle.Instance=TIM3;
	TimHandle.Init.ClockDivision=TIM_CLOCKDIVISION_DIV4; /* req_TIM3 = 80MHz/4 =20MHz */
	TimHandle.Init.CounterMode=TIM_COUNTERMODE_UP;				
	TimHandle.Init.Period=20000-1; 										/* freq = freq_TIM3/((PSC+1)(ARR+1)) */
	TimHandle.Init.Prescaler=10000-1; 							/* PSC = Prescaler & ARR = Period */ 
	HAL_TIM_Base_Init(&TimHandle);
}
/*******************************************************************************
* Function Name  : Tim3_DeInit
* Description    : Interface : DeInitialize timer 3. 
* Input          : No input.
* Return         : No returned value. 
*******************************************************************************/
void Tim3_DeInit(void)
{
	HAL_TIM_Base_DeInit(&TimHandle);
}

/*******************************************************************************
* Function Name  : Sensors_DeInit
* Description    : DeInitialize sensors. 
* Input          : No input.
* Return         : No returned value. 
*******************************************************************************/
void Sensors_DeInit(void){
	Acc_Gyr_DeInit();
	Temp_Hum_DeInit();
	Tim3_DeInit();
}

/*******************************************************************************
* Function Name  : Sensors_DeInit
* Description    : ReDefinition of the function in charge of hardware initialization
									 for timer 3. 
* Input          : No input.
* Return         : No returned value. 
*******************************************************************************/
void HAL_TIM_Base_MspInit(TIM_HandleTypeDef *htim){
	
	__HAL_RCC_TIM3_CLK_ENABLE(); /* Enable clock for timer 3 */
	HAL_NVIC_SetPriority(TIM3_IRQn,3,0); /* Define priority of the iterrupt*/
	HAL_NVIC_EnableIRQ(TIM3_IRQn); /* Enable the intrrupt */ 
}

