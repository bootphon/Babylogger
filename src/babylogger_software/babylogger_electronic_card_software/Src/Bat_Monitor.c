/**********************
* Team : Coml
* Year : 2020-2021
**********************/
#include "Bat_Monitor.h"
#include "stc3115_Driver.h"

/* Private typedef -----------------------------------------------------------*/

STC3115_ConfigData_TypeDef ConfigData;
STC3115_BatteryData_TypeDef BatteryData;

/* Private prototypes ---------------------------------------------------------*/

/* Private variables ---------------------------------------------------------*/

int State_of_Charge;  /* The value of State of charge per mille */ 
int Voltage;	  /* The value of battery's voltage in mV */
int Current;	  /* The current used by the load circuit in mA */

void Bat_Monitor_AlarmSetSOCThreshold (int Soc_Value)
{
	STC3115_AlarmSetSOCThreshold(&ConfigData, Soc_Value);
}
void Bat_Monitor_AlarmClear(void)
{
	STC3115_AlarmClear();
}
void Bat_Monitor_AlarmDisable(void)
{
	STC3115_AlarmStop();
}
void Bat_Monitor_AlarmEnable(void)
{
	STC3115_AlarmSet();
}


/*******************************************************************************
* Function Name  : Bat_Monitor_Update_Values 
* Description    : utility function to update battery's parameters
* Input          : No input
* Return         : No return value
*******************************************************************************/

void Bat_Monitor_Update_Values (void)
{
	/* This function is called from a Timer. No need for a high frequency */
	
	/* Read the battery's parameters and fill it in the battery output structure */ 
	GasGauge_Task(&ConfigData,&BatteryData); 
	
	/* Update the variables */
	State_of_Charge = BatteryData.SOC;	
	Voltage = BatteryData.Voltage;
	Current = BatteryData.Current;
	
}

/*******************************************************************************
* Function Name  : Sensors_Init_Check_Bat 
* Description    : utility function to know if the battery is enough charged at startup sequence
* Input          : No input
* Return         : return 0 if battery is enough charged, else 1. 
*******************************************************************************/

uint8_t Sensors_Init_Check_Bat(void)
{
	/* Read the battery's parameters and fill it in the battery output structure */ 
	GasGauge_Task(&ConfigData,&BatteryData);
	
	/* Update the variables */
	State_of_Charge = BatteryData.SOC;
	Voltage = BatteryData.Voltage;
	Current = BatteryData.Current;
	
	
	/* Check if the battery is enough chargerd */ 
	if (State_of_Charge < Bat_Min )
	{
		return 0;
	}	
	
	else 
	{
		if (State_of_Charge < (Bat_Min + 150 ))
		{
			Bat_Monitor_Set_Flag_Low_Battery(); /* Notifie that the threshold has been reached */
			return 1;
		}
		else
		{
			if (State_of_Charge < 500)
			{
				return 2;
			}
			else
			{
				if (State_of_Charge < 750)
				{
					return 3;
				}
				else return 4;
			}
		}			
	}	

}

/*******************************************************************************
* Function Name  : Sensors_Bat_Is_Full 
* Description    : utility function to know if the battery is full.
* Input          : No input
* Return         : return 1 if battery full, else 0. 
*******************************************************************************/
uint8_t Sensors_Bat_Is_Full(void)
{
	/* This fucnction is called in "Charge Mode" only to know if the task is completed */
	
	
	if (State_of_Charge > Bat_Max  ) /* Check if state of charge exceed the predefined value */
	{
		return 1;
	}	
	else return 0; 
	
}

/*******************************************************************************
* Function Name  : Sensors_Get_State_of_Charge_Progress
* Description    : utility function to know the SoC value out from this driver.
* Input          : No input
* Return         : return Soc value. 
*******************************************************************************/

int Sensors_Get_State_of_Charge_Progress(void)
{
	return State_of_Charge;
}


/*******************************************************************************
* Function Name  : Sensors_Callback_STC3115
* Description    : utility function to handle alarm's interrupt of the STC3115.
* Input          : No input
* Return         : No returned value. 
*******************************************************************************/

void Sensors_Callback_STC3115(void){
	
	HAL_NVIC_DisableIRQ(EXTI9_5_IRQn); /* Disable interrupt for EXTI lines 5 to 9 */
	HAL_NVIC_ClearPendingIRQ(EXTI9_5_IRQn); /* Clear flag of inerrupts for EXTI lines 5 to 9*/

	Bat_Monitor_Set_Flag_Low_Battery(); /* Notifie that the threshold has been reached */ 
	
	HAL_NVIC_EnableIRQ(EXTI9_5_IRQn); /* Enable interrupt for EXTI lines 5 to 9 */
}

/*******************************************************************************
* Function Name  : Bat_Monitor_Set_Flag_Low_Battery
* Description    : function to be redefined where the information of "low battery" is needed.
* Input          : No input
* Return         : No returned value. 
*******************************************************************************/
__weak void Bat_Monitor_Set_Flag_Low_Battery(void)
{
	// re-defined in sensor_acquisition
}

/*******************************************************************************
* Function Name  : Bat_Monitor_Stop_System_Low_Battery
* Description    : function to be redefined where is needed to turn off the system because of low battery.
* Input          : No input
* Return         : No returned value. 
*******************************************************************************/
__weak void Bat_Monitor_Stop_System_Low_Battery(void)
{
	// re-defined in main 
}

/*******************************************************************************
* Function Name  : Bat_Monitor_Stop
* Description    : function to be called before turnig of the device to save context in the STC3115's Ram, and saving energy (no need to acquire data when the device is off). 
* Input          : No input
* Return         : No returned value. 
*******************************************************************************/

void Bat_Monitor_Stop(void)
{
	GasGauge_Stop();
}	

/*******************************************************************************
* Function Name  : Bat_Monitor_Init
* Description    : function to be called at startup sequence to initialize the chip 
* Input          : No input
* Return         : No returned value. 
*******************************************************************************/

void Bat_Monitor_Init(void)
{
	/* Initialize the chip with the parameters of "stc3115_Battery.h" */
	GasGauge_Initialization(&ConfigData,&BatteryData);
		
	/* Enable alarm */ 
	STC3115_AlarmSet();
	
	/* Display the last values of the battery befor turning off */
	State_of_Charge = BatteryData.SOC;
	Voltage = BatteryData.Voltage;
	Current = BatteryData.Current;
	
}

