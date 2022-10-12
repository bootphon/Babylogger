#include "Temp_Hum.h"
#include "HTS221_env_sensor.h"
#include "HTS221_env_sensor_ex.h"



/* Private prototypes ---------------------------------------------------------*/


/* Private variables ---------------------------------------------------------*/

float 					temperature;  /* The value of the temperature in °C */ 



/*******************************************************************************
* Function Name  : Temp_Hum_Init_Check_Temperature
* Description    : Function called at startup to know if the temperature of the 
									 device allow it to be used. 
* Input          : No input
* Return         : Return 1 if the temperature exceeds the allowed value, else 0. 
*******************************************************************************/
uint8_t Temp_Hum_Init_Check_Temperature(void)
{
	/* Get temperature value */
	HTS221_ENV_SENSOR_GetValue(ENV_TEMPERATURE, &temperature);
		
	/* Check if the value exceed the threshold */
	if ( temperature >= Temp_Max)
	{
		return 1;
	}
	else return 0; 
}




/*******************************************************************************
* Function Name  : Temp_Hum_Init_Check_Temperature
* Description    : Fonction used on "Run Mode" to prevent overheating. 
* Input          : No input
* Return         : No returned values. 
*******************************************************************************/
void Temp_Hum_Get_Temperature()
{
	/* Get temperature value */
	HTS221_ENV_SENSOR_GetValue(ENV_TEMPERATURE, &temperature);
	
	/* Check if the value exceed the threshold */
	if (temperature >= TEMP_SEUIL)
	{
		//Ajout d'une fonction __weak 
	}
}

/*******************************************************************************
* Function Name  : Temp_Hum_Overheating_Devive
* Description    : function to be redefined to handle overheating issues.
* Input          : No input
* Return         : No returned value. 
*******************************************************************************/
__weak void Temp_Hum_Overheating_Devive(void)
{
	// To Be Define.
}

/*******************************************************************************
* Function Name  : Temp_Hum_Stop
* Description    : function used to disable the chip.
* Input          : No input
* Return         : No returned value. 
*******************************************************************************/
void Temp_Hum_Stop(void)
{
	HTS221_ENV_SENSOR_Disable(ENV_TEMPERATURE); /* Disable temperature sensing */
	HTS221_ENV_SENSOR_Disable(ENV_HUMIDITY); 		/* Disable humidity sensing */
}

/*******************************************************************************
* Function Name  : Temp_Hum_Start
* Description    : function used to enable the chip.
* Input          : No input
* Return         : No returned value. 
*******************************************************************************/
void Temp_Hum_Start(void)
{
	HTS221_ENV_SENSOR_Enable(ENV_TEMPERATURE); /* Enable temperature sensing */
	HTS221_ENV_SENSOR_Enable(ENV_HUMIDITY);		 /* Enable humidity sensing */
}

/*******************************************************************************
* Function Name  : Temp_Hum_Init
* Description    : function used to initialize the chip.
* Input          : No input
* Return         : No returned value. 
*******************************************************************************/
void Temp_Hum_Init(void)
{
	HTS221_ENV_SENSOR_Init(ENV_TEMPERATURE); /* Initialize temperature sensing feature */
	HTS221_ENV_SENSOR_Init(ENV_HUMIDITY);	/* Initialize humidity sensing feature */
}

/*******************************************************************************
* Function Name  : Temp_Hum_DeInit
* Description    : function used to Deinitialize the chip.
* Input          : No input
* Return         : No returned value. 
*******************************************************************************/

void Temp_Hum_DeInit(void)
{
	HTS221_ENV_SENSOR_DeInit();  /* DeInit the chip */
}
