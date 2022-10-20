/**********************
* Team : Coml
* Year : 2020-2021
**********************/
#include "PWR.h"
#include "IHM.h"
#include "GPIO.h" 
#include "sensor_acquisition.h" 

/*******************************************************************************
* Function Name  : 
* Description    : 
* Input          : 
* Return         : 
*******************************************************************************/
uint8_t Global_App_State = Global_App_first_Init; 

static void PWR_Reset(void);

void PWR_Init(void)
{
	__HAL_RCC_PWR_CLK_ENABLE(); 
  HAL_PWR_EnableBkUpAccess();
}


// cette fonction permet un reset du system en cas d'erreur
// fonctionne uniquement dans le mode acquisition
// elle utilise le signal PG du chargeur de batterie (état haut = chargeur débranché)
void PWR_Reset_System(void)
{
	// Disable all used wakeup sources: WKUP pin  
	HAL_PWR_DisableWakeUpPin(PWR_WAKEUP_PIN2);
	// Clear wake up Flag
	__HAL_PWR_CLEAR_FLAG(PWR_FLAG_WUF2);
	// Enable wakeup pin WKUP2 (et charge-in: fonctionnalitée à venir)	
	HAL_PWR_EnableWakeUpPin(PWR_WAKEUP_PIN2_HIGH);
	//Set RTC back-up register RTC_BKP31R to indicate later on that system has entered shutdown mode
	 WRITE_REG( RTC->BKP31R, 0x1 );
	// Enter shutdown mode   
  	HAL_PWREx_EnterSHUTDOWNMode();
}


/*
This fuction allow to enter in shutdown after reset all wakeup flag and
writing on backup regester to indicate previous mode at reset.

This function is called when the button on/off is 
and at the end of the cycle of charge/sync (or if it has been interrupted) 
*/
void PWR_Go_To_Shutdown_Mode(void)
{
	    
	// Disable all used wakeup sources: WKUP pin (don't forget to add new wakeup pin charge-in)   
  	HAL_PWR_DisableWakeUpPin(PWR_WAKEUP_PIN1);
  	// ajouter ici ou mettre un ou !!!
	HAL_PWR_DisableWakeUpPin(PWR_WAKEUP_PIN2);
	// Clear wake up Flag
	__HAL_PWR_CLEAR_FLAG(PWR_FLAG_WUF1);
	// ajouter ici ou mettre un ou !!!
	__HAL_PWR_CLEAR_FLAG(PWR_FLAG_WUF2);
	
		
	// Enable wakeup pin WKUP2 (et charge-in: fonctionnalitée à venir)	
	HAL_PWR_EnableWakeUpPin(PWR_WAKEUP_PIN1_HIGH);
	// ajouter ici ou mettre un ou !!!
	HAL_PWR_EnableWakeUpPin(PWR_WAKEUP_PIN2_LOW);
	//Set RTC back-up register RTC_BKP31R to indicate later on that system has entered shutdown mode
	 WRITE_REG( RTC->BKP31R, 0x1 );
	// Enter shutdown mode   
  	HAL_PWREx_EnterSHUTDOWNMode();
}

void PWR_Check_Previous_Mode(void)
{
	uint8_t TMP_Check_Bat = 10;
	
	//WRITE_REG( RTC->BKP31R, 0x0 );
	if ( (Global_App_State == Global_App_first_Init) && (READ_REG(RTC->BKP31R) != 1))
	{

		//first init with new bat plug 
		IHM_LED_Start(LED_PWM_Mode, LED1_2);
		HAL_Delay(3000);
		PWR_Go_To_Shutdown_Mode();
	}
	
	if (READ_REG(RTC->BKP31R) == 1)// out of shutdown mode
  {
		
    WRITE_REG( RTC->BKP31R, 0x0 );  /* reset back-up register */
    
		
    /* Wait that user release the User push-button */
    //BSP_PB_Init(BUTTON_USER, BUTTON_MODE_GPIO);
    //while(BSP_PB_GetState(BUTTON_USER) == GPIO_PIN_RESET){}
		if ( (__HAL_PWR_GET_FLAG(PWR_FLAG_WUF1) == RESET) && (__HAL_PWR_GET_FLAG(PWR_FLAG_WUF2) == RESET))
		{
		
			PWR_Go_To_Shutdown_Mode();

		}

    /* Check and Clear the Wakeup flag */
		if (__HAL_PWR_GET_FLAG(PWR_FLAG_WUF1) != RESET)
		{
			__HAL_PWR_CLEAR_FLAG(PWR_FLAG_WUF1);
		
			// Check bat & Temp 
		
			TMP_Check_Bat = Sensors_Init_Check_Bat();
		
			if (TMP_Check_Bat == 0)				
			{
				IHM_LED_Start(LED_PWM_Mode, LED1);
				HAL_Delay(3000);
				PWR_Go_To_Shutdown_Mode();	
			}
		
			else if (Temp_Hum_Init_Check_Temperature() == 1)
			{
				IHM_LED_PWM_Init(LED1);
				IHM_LED_Start(LED_Continious_Mode,LED1);
			
				HAL_Delay(10000);
				PWR_Go_To_Shutdown_Mode();
			}
		
			else 
			{
		
				// wake up with button
				Global_App_State = Global_App_Run_Mode;
			
				switch(TMP_Check_Bat)
				{
					case 1 :
						IHM_LED_Start(LED_Continious_Mode,LED2);
					break;
					case 2 : /* Bat between 25 & 50 % */
						IHM_LED_Start(LED_Continious_Mode,LED2);
						HAL_Delay(500);
						IHM_LED_Stop(LED2);
						HAL_Delay(500);
						IHM_LED_Start(LED_Continious_Mode,LED2);
					break;
				
					case 3 : /* Bat between 50 & 75 % */
						IHM_LED_Start(LED_PWM_Mode,LED2);
						HAL_Delay(2000);
						IHM_LED_Start(LED_Continious_Mode,LED2);
					break;
				
					case 4 : /* Bat between 75 & 100 % */
						IHM_LED_Start(LED_PWM_Mode,LED2);
						HAL_Delay(3000);
						IHM_LED_Start(LED_Continious_Mode,LED2);
					break;
				
					default: 
						if (TMP_Check_Bat > 4)
						{
							/* Error */
							PWR_Reset_System();
						}
					break;
				}
			}
		}
	
		if (__HAL_PWR_GET_FLAG(PWR_FLAG_WUF2) != RESET)
		{	
			__HAL_PWR_CLEAR_FLAG(PWR_FLAG_WUF2);
			
				// wake up with charge detect it can be a usb detect or a reset 
		
			if ( GPIO_BAT_Charg_Get_State () == Bat_Charg_Shutdown)
			{
					// reset 
				Sensors_Init_Check_Bat();
				Global_App_State = Global_App_Run_Mode;
				IHM_LED_Start(LED_Continious_Mode,LED2);
			}		
			else
			{
				// charge detect
				Global_App_State = Global_App_Charge_Mode;
				IHM_LED_PWM_Init(LED1_2);
				IHM_LED_Start(LED_PWM_Mode,LED1_2);
			}
		}
	
  }
	if ((Global_App_State != Global_App_Charge_Mode) && (Global_App_State != Global_App_Run_Mode))
	{
		PWR_Go_To_Shutdown_Mode();
	}
}


static void PWR_Reset(void)
{
	__HAL_RCC_BACKUPRESET_FORCE();
	
	__HAL_PWR_CLEAR_FLAG(PWR_FLAG_WU);
	__HAL_PWR_CLEAR_FLAG(PWR_FLAG_PVDO);
	__HAL_PWR_CLEAR_FLAG(PWR_FLAG_PVMO1);
	__HAL_PWR_CLEAR_FLAG(PWR_FLAG_PVMO2);
	__HAL_PWR_CLEAR_FLAG(PWR_FLAG_PVMO3);
	__HAL_PWR_CLEAR_FLAG(PWR_FLAG_PVMO4);
	
	__HAL_PWR_CLEAR_FLAG(PWR_FLAG_REGLPF);
	__HAL_PWR_CLEAR_FLAG(PWR_FLAG_REGLPS);
	__HAL_PWR_CLEAR_FLAG(PWR_FLAG_SB);
	__HAL_PWR_CLEAR_FLAG(PWR_FLAG_VOSF);
	__HAL_PWR_CLEAR_FLAG(PWR_FLAG_WUFI);
	
	__HAL_RCC_CLEAR_RESET_FLAGS();
	
	__HAL_RCC_BACKUPRESET_RELEASE();
	
	
	HAL_NVIC_SystemReset();
}

void Sd_DeInit_Callback(void)
{

	if (Global_App_State == Global_App_Run_Mode)
	{
		IHM_LED_Stop(LED1_2);
	}
	if (Global_App_State == Global_App_Charge_Mode)
	{
		IHM_LED_Stop(LED1_2);
	}
	Global_App_State = Global_App_Off_Mode;
	HAL_Delay(2000);
	PWR_Go_To_Shutdown_Mode();
}
