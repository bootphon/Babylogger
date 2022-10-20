#include "IHM.h"

// internal variables

uint32_t PB_Time_Press 	 = 0xFFFFFFFF; 
uint32_t PB_Time_Release = 0x00000000;
uint8_t  PB_Wait_Release = 0x00;

uint8_t	 PB_Constructor_Press_Counter = 0;
uint32_t PB_Constructor_Time_First_press = 0;
// External Variables

extern uint8_t Global_App_State;

// variable de debug
uint32_t var_test_exti_reset 	=	0;
uint32_t var_test_exti_set 	=	0;
uint32_t var_test_exti_sysoff =	0;
// fonctions defenition

TIM_HandleTypeDef    TimerLED;
TIM_OC_InitTypeDef sConfig;
uint32_t uhPrescalerValue = 0;
	
void script_test_led_func(void)
{
	IHM_LED_Start(LED_Continious_Mode,LED1);
	HAL_Delay(2000);
	
	IHM_LED_Start(LED_Continious_Mode,LED2);
	HAL_Delay(2000);
	
	IHM_LED_Stop(LED1);
	HAL_Delay(2000);
	
	IHM_LED_Stop(LED2);
	HAL_Delay(2000);
	
	IHM_LED_Start(LED_PWM_Mode,LED1);
	HAL_Delay(2000);
	
	IHM_LED_Start(LED_PWM_Mode,LED2);
	HAL_Delay(2000);
	
	
	IHM_LED_Stop(LED1);
	HAL_Delay(2000);
	
	IHM_LED_Stop(LED2);
	HAL_Delay(2000);
	
	IHM_LED_Start(LED_Continious_Mode,LED1_2);
	HAL_Delay(2000);
	
	IHM_LED_Stop(LED1_2);
	HAL_Delay(2000);
	
	
	IHM_LED_Start(LED_PWM_Mode,LED1_2);
	HAL_Delay(2000);
	
	IHM_LED_Stop(LED1_2);
	
	while(1);
	
	
	
}

typedef enum {
	LED_Non_Init = 0,
	LED_OutputPP_Init = 1,
	LED_AfPWM_Init = 2
} LED_INIT_STATE_TypeDef;

LED_INIT_STATE_TypeDef 	LED1_Init_State = LED_Non_Init;
LED_INIT_STATE_TypeDef	LED2_Init_State = LED_Non_Init;
/*
	The goal of this function is to show the origin of an err. 
	We have to call it from a infinit loop. It desplays a special led animation for each err
	The parameter define the type of err
	0   --> open audio file
	1   --> allocate audio file space
	2   --> write first half audio 
  3   --> write seconde half audio 
  4   --> close audio file 
	5   --> open acti file
	6   --> allocate acti file space
  7   -->	write first half acti
	8   --> write seconde half acti
	9   --> close acti file 
	10	--> Bus fault 
	11	--> Hard fault 
	12	--> Fatfs err handel
	13 	--> MemManagement 
*/
/*
void IHM_Debug_Led(uint8_t Err_Origine)
{
	
	while(1)
	{
	IHM_LED_Stop(LED1_2);
	HAL_Delay(2000);
	
	
	switch(Err_Origine)
	{
		case 0: // open audio
			IHM_LED_Start(LED_Continious_Mode , LED2) ;
			HAL_Delay(500);
		break;
		
		case 1:  // allocate audio 
			IHM_LED_Start(LED_Continious_Mode , LED2) ;
			HAL_Delay(500);
			IHM_LED_Stop(LED2);
			HAL_Delay(500);
			IHM_LED_Start(LED_Continious_Mode , LED2) ;
			HAL_Delay(500);
		break;
		
		case 2: // Write first half
			IHM_LED_Start(LED_Continious_Mode , LED2) ;
			HAL_Delay(500);
			IHM_LED_Stop(LED2);
			HAL_Delay(500);
			IHM_LED_Start(LED_Continious_Mode , LED2) ;
			HAL_Delay(500);
			IHM_LED_Stop(LED2);
			HAL_Delay(500);
			IHM_LED_Start(LED_Continious_Mode , LED2) ;
			HAL_Delay(500);
		break;
		
		case 3: //write seconde half  
			IHM_LED_Start(LED_Continious_Mode , LED2) ;
			HAL_Delay(500);
			IHM_LED_Stop(LED2);
			HAL_Delay(500);
			IHM_LED_Start(LED_Continious_Mode , LED2) ;
			HAL_Delay(500);
			IHM_LED_Stop(LED2);
			HAL_Delay(500);
			IHM_LED_Start(LED_Continious_Mode , LED2) ;
			HAL_Delay(500);
			IHM_LED_Stop(LED2);
			HAL_Delay(500);
			IHM_LED_Start(LED_Continious_Mode , LED2) ;
			HAL_Delay(500);
		break;
		
		case 4: // close audio file
			IHM_LED_Start(LED_Continious_Mode , LED2) ;
			HAL_Delay(500);
			IHM_LED_Stop(LED2);
			HAL_Delay(500);
			IHM_LED_Start(LED_Continious_Mode , LED2) ;
			HAL_Delay(500);
			IHM_LED_Stop(LED2);
			HAL_Delay(500);
			IHM_LED_Start(LED_Continious_Mode , LED2) ;
			HAL_Delay(500);
			IHM_LED_Stop(LED2);
			HAL_Delay(500);
			IHM_LED_Start(LED_Continious_Mode , LED2) ;
			HAL_Delay(500);
			IHM_LED_Stop(LED2);
			HAL_Delay(500);
			IHM_LED_Start(LED_Continious_Mode , LED2) ;
			HAL_Delay(500);
		break;
		
		case 5: // open acti file 
			IHM_LED_Start(LED_Continious_Mode , LED1) ;
			HAL_Delay(500);
		break;
		
		case 6: // allocte mem for acti file
			IHM_LED_Start(LED_Continious_Mode , LED1) ;
			HAL_Delay(500);
			IHM_LED_Stop(LED1);
			HAL_Delay(500);
			IHM_LED_Start(LED_Continious_Mode , LED1) ;
			HAL_Delay(500);
		break;
		
		case 7: // write first half acti file
			IHM_LED_Start(LED_Continious_Mode , LED1) ;
			HAL_Delay(500);
			IHM_LED_Stop(LED1);
			HAL_Delay(500);
			IHM_LED_Start(LED_Continious_Mode , LED1) ;
			HAL_Delay(500);
			IHM_LED_Stop(LED1);
			HAL_Delay(500);
			IHM_LED_Start(LED_Continious_Mode , LED1) ;
			HAL_Delay(500);
			
		break;
		
		case 8: // write seconde half acti file 
			IHM_LED_Start(LED_Continious_Mode , LED1) ;
			HAL_Delay(500);
			IHM_LED_Stop(LED1);
			HAL_Delay(500);
			IHM_LED_Start(LED_Continious_Mode , LED1) ;
			HAL_Delay(500);
			IHM_LED_Stop(LED1);
			HAL_Delay(500);
			IHM_LED_Start(LED_Continious_Mode , LED1) ;
			HAL_Delay(500);
			IHM_LED_Stop(LED1);
			HAL_Delay(500);
			IHM_LED_Start(LED_Continious_Mode , LED1) ;
			HAL_Delay(500);
		break;
		
		case 9: // close acti file 
			IHM_LED_Start(LED_Continious_Mode , LED1) ;
			HAL_Delay(500);
			IHM_LED_Stop(LED1);
			HAL_Delay(500);
			IHM_LED_Start(LED_Continious_Mode , LED1) ;
			HAL_Delay(500);
			IHM_LED_Stop(LED1);
			HAL_Delay(500);
			IHM_LED_Start(LED_Continious_Mode , LED1) ;
			HAL_Delay(500);
			IHM_LED_Stop(LED1);
			HAL_Delay(500);
			IHM_LED_Start(LED_Continious_Mode , LED1) ;
			HAL_Delay(500);
			IHM_LED_Stop(LED1);
			HAL_Delay(500);
			IHM_LED_Start(LED_Continious_Mode , LED1) ;
			HAL_Delay(500);
		break;
		
		case 10: // bus fault
			IHM_LED_Start(LED_Continious_Mode , LED1_2) ;
			HAL_Delay(500);
		break;
		
		case 11: // Hard fault 
			IHM_LED_Start(LED_Continious_Mode , LED1_2) ;
			HAL_Delay(500);
			IHM_LED_Stop(LED1_2);
			HAL_Delay(500);
			IHM_LED_Start(LED_Continious_Mode , LED1_2) ;
			HAL_Delay(500);
		break;
		
		case 12: // fatfs err handler  
			IHM_LED_Start(LED_Continious_Mode , LED1) ;
			while(1);
		break;
		
		case 13:
			IHM_LED_Start(LED_Continious_Mode , LED1_2) ;
			HAL_Delay(500);
			IHM_LED_Stop(LED1_2);
			HAL_Delay(500);
			IHM_LED_Start(LED_Continious_Mode , LED1_2) ;
			HAL_Delay(500);
			IHM_LED_Stop(LED1_2);
			HAL_Delay(500);
			IHM_LED_Start(LED_Continious_Mode , LED1_2) ;
			HAL_Delay(500);
		break; 
		
		default:
			
		break;
		
	}
	
	
	


	}
}*/

void IHM_LED_Stop(uint8_t LEDx)
{
	
	switch(LEDx)
	{
		case LED1:
			if (LED1_Init_State == LED_AfPWM_Init)
			{
				IHM_LED_Stop_Blink(LED1);
			}
			else if (LED1_Init_State == LED_OutputPP_Init)
			{
				IHM_LED_Off(LED1);
			}
			else 
			{
				
			}
		break;
			
		case LED2:
			if (LED2_Init_State == LED_AfPWM_Init)
			{
				IHM_LED_Stop_Blink(LED2);
			}
			else if (LED2_Init_State == LED_OutputPP_Init)
			{
				IHM_LED_Off(LED2);
			}
			else 
			{
				
			}
			
		break;
		
		case LED1_2: 
			if (LED1_Init_State == LED_AfPWM_Init)
			{
				IHM_LED_Stop_Blink(LED1);
			}
			else if (LED1_Init_State == LED_OutputPP_Init)
			{
				IHM_LED_Off(LED1);
			}
			else 
			{
				
			}
			
			if (LED2_Init_State == LED_AfPWM_Init)
			{
				IHM_LED_Stop_Blink(LED2);
			}
			else if (LED2_Init_State == LED_OutputPP_Init)
			{
				IHM_LED_Off(LED2);
			}
			else 
			{
				
			}
		break; 
		
		default:
			
		break;
		
	}
	
}


void IHM_LED_Start(uint8_t Mode, uint8_t LEDx)
{
	
	if (Mode == LED_Continious_Mode)
	{
		switch(LEDx)
		{
			case LED1:
				if ( LED1_Init_State == LED_OutputPP_Init)
				{
					IHM_LED_On(LED1);
				}
				else 
				{
					IHM_LED_Continious_Init(LED1);
					IHM_LED_On(LED1);
				}
			break;
			
			case LED2 : 
				if ( LED2_Init_State == LED_OutputPP_Init)
				{
					IHM_LED_On(LED2);
				}
				else 
				{
					IHM_LED_Continious_Init(LED2);
					IHM_LED_On(LED2);
				}				
			break;
			
			case LED1_2:
				if ((LED1_Init_State == LED_OutputPP_Init) && (LED2_Init_State == LED_OutputPP_Init))
				{
					IHM_LED_On(LED1_2);
				}
				else 
				{
					IHM_LED_Continious_Init(LED1_2);
					IHM_LED_On(LED1_2);
				}
			break;
			
			default:
				
			break;
		}
	}
	else if (Mode == LED_PWM_Mode)
	{
		switch(LEDx)
		{
			case LED1:
				if ( LED1_Init_State == LED_AfPWM_Init)
				{
					IHM_LED_Start_Blink(LED1);
				}
				else 
				{
					IHM_LED_PWM_Init(LED1);
					IHM_LED_Start_Blink(LED1);
				}
			break;
			
			case LED2 : 
				if ( LED2_Init_State == LED_AfPWM_Init)
				{
					IHM_LED_Start_Blink(LED2);
				}
				else 
				{
					IHM_LED_PWM_Init(LED2);
					IHM_LED_Start_Blink(LED2);
				}				
			break;
			
			case LED1_2:
				if ((LED1_Init_State == LED_AfPWM_Init) && (LED2_Init_State == LED_AfPWM_Init))
				{
					IHM_LED_Start_Blink(LED1_2);
				}
				else 
				{
					IHM_LED_PWM_Init(LED1_2);
					IHM_LED_Start_Blink(LED1_2);
				}
			break;
			
			default:
				
			break;
		}
	}
	
	
}



/**
* @brief Activation du Timer en mode PWM
* @param val : 
						0 : la LED1 clignote rouge
						1 : la LED2 clignote vert 
						2 : les 2 LED clignotent
* @retval None
*/
void IHM_LED_Start_Blink(uint8_t val){
	switch(val){
		case 0 :
			HAL_TIM_PWM_Start(&TimerLED, TIM_CHANNEL_3);
		break;
		case 1 :
			HAL_TIM_PWM_Start(&TimerLED, TIM_CHANNEL_4);
		break;
		case 2 :
			HAL_TIM_PWM_Start(&TimerLED, TIM_CHANNEL_3);
			HAL_TIM_PWM_Start(&TimerLED, TIM_CHANNEL_4);
		break;
	}
}

/**
* @brief Activation du Timer en mode PWM
* @param val : 
						0 : LED1 stop
						1 : LED2 stop
						2 : les 2 LED stop
* @retval None
*/
void IHM_LED_Stop_Blink(uint8_t val){
	switch(val){
		case 0 :
			HAL_TIM_PWM_Stop(&TimerLED, TIM_CHANNEL_3);
		break;
		case 1 :
			HAL_TIM_PWM_Stop(&TimerLED, TIM_CHANNEL_4);
		break;
		case 2 :
			HAL_TIM_PWM_Stop(&TimerLED, TIM_CHANNEL_3);
			HAL_TIM_PWM_Stop(&TimerLED, TIM_CHANNEL_4);
		break;
	}
	
}



/**
* @brief Initialization du Timer en mode PWM
* Fréquence de clignotement fixée à 1 seconde / duty cycle 50%
* @param None
* @retval None
*/
void IHM_LED_PWM_Init(uint8_t LEDx)
{
		
	if (LEDx == LED1)
	{	
			GPIO_InitTypeDef GPIO_InitPWM;
		__HAL_RCC_GPIOA_CLK_ENABLE();//PA10 -> TIM1_CH3 
	
		GPIO_InitPWM.Speed = GPIO_SPEED_LOW;
		GPIO_InitPWM.Mode = GPIO_MODE_AF_PP;
		GPIO_InitPWM.Pull = GPIO_NOPULL;
	
		GPIO_InitPWM.Alternate= GPIO_AF1_TIM1;
		GPIO_InitPWM.Pin = LED_1_Pin;
		HAL_GPIO_Init(LED_1_2_Port, &GPIO_InitPWM); 
	
	
	
	
		TimerLED.Instance=TIM1; //LED1/LED2 -> PA10/PA11 -> TIM1CH3/TIM1CH4
		TimerLED.Init.ClockDivision=TIM_CLOCKDIVISION_DIV1;
		TimerLED.Init.CounterMode=TIM_COUNTERMODE_UP;
		TimerLED.Init.Period=10000-1; //ARR value
		TimerLED.Init.Prescaler=8000-1; //PSC 
		HAL_TIM_PWM_Init(&TimerLED);
	
		sConfig.OCMode       = TIM_OCMODE_PWM1;
		sConfig.OCPolarity   = TIM_OCPOLARITY_HIGH;
		sConfig.OCFastMode   = TIM_OCFAST_DISABLE;
		sConfig.OCNPolarity  = TIM_OCNPOLARITY_HIGH;
		sConfig.OCNIdleState = TIM_OCNIDLESTATE_RESET;
		sConfig.OCIdleState  = TIM_OCIDLESTATE_RESET;
		sConfig.Pulse = 5000;//period/2
	
		HAL_TIM_PWM_ConfigChannel(&TimerLED, &sConfig, TIM_CHANNEL_3);
		
		LED1_Init_State = LED_AfPWM_Init;
	}
	
	if (LEDx == LED2)
	{	
			GPIO_InitTypeDef GPIO_InitPWM;
		__HAL_RCC_GPIOA_CLK_ENABLE();// PA11 -> TIM1_CH4
	
		GPIO_InitPWM.Speed = GPIO_SPEED_LOW;
		GPIO_InitPWM.Mode = GPIO_MODE_AF_PP;
		GPIO_InitPWM.Pull = GPIO_NOPULL;
		GPIO_InitPWM.Alternate= GPIO_AF1_TIM1;
		GPIO_InitPWM.Pin = LED_2_Pin;
		HAL_GPIO_Init(LED_1_2_Port, &GPIO_InitPWM);

	
		TimerLED.Instance=TIM1; //LED1/LED2 -> PA10/PA11 -> TIM1CH3/TIM1CH4
		TimerLED.Init.ClockDivision=TIM_CLOCKDIVISION_DIV1;
		TimerLED.Init.CounterMode=TIM_COUNTERMODE_UP;
		TimerLED.Init.Period=10000-1; //ARR value
		TimerLED.Init.Prescaler=8000-1; //PSC 
		HAL_TIM_PWM_Init(&TimerLED);
	
		sConfig.OCMode       = TIM_OCMODE_PWM1;
		sConfig.OCPolarity   = TIM_OCPOLARITY_HIGH;
		sConfig.OCFastMode   = TIM_OCFAST_DISABLE;
		sConfig.OCNPolarity  = TIM_OCNPOLARITY_HIGH;
		sConfig.OCNIdleState = TIM_OCNIDLESTATE_RESET;
		sConfig.OCIdleState  = TIM_OCIDLESTATE_RESET;
		sConfig.Pulse = 5000;//period/2

		HAL_TIM_PWM_ConfigChannel(&TimerLED, &sConfig, TIM_CHANNEL_4);
		
		LED2_Init_State = LED_AfPWM_Init;
	}	
	
	if (LEDx == LED1_2)
	{	
			GPIO_InitTypeDef GPIO_InitPWM;
		__HAL_RCC_GPIOA_CLK_ENABLE();//PA10 -> TIM1_CH3 / PA11 -> TIM1_CH4
	
		GPIO_InitPWM.Speed = GPIO_SPEED_LOW;
		GPIO_InitPWM.Mode = GPIO_MODE_AF_PP;
		GPIO_InitPWM.Pull = GPIO_NOPULL;
	
		GPIO_InitPWM.Alternate= GPIO_AF1_TIM1;
		GPIO_InitPWM.Pin = LED_1_Pin;
		HAL_GPIO_Init(LED_1_2_Port, &GPIO_InitPWM); 
	
		GPIO_InitPWM.Alternate= GPIO_AF1_TIM1;
		GPIO_InitPWM.Pin = LED_2_Pin;
		HAL_GPIO_Init(LED_1_2_Port, &GPIO_InitPWM);
	
	
	
		TimerLED.Instance=TIM1; //LED1/LED2 -> PA10/PA11 -> TIM1CH3/TIM1CH4
		TimerLED.Init.ClockDivision=TIM_CLOCKDIVISION_DIV1;
		TimerLED.Init.CounterMode=TIM_COUNTERMODE_UP;
		TimerLED.Init.Period=10000-1; //ARR value
		TimerLED.Init.Prescaler=8000-1; //PSC 
		HAL_TIM_PWM_Init(&TimerLED);
	
		sConfig.OCMode       = TIM_OCMODE_PWM1;
		sConfig.OCPolarity   = TIM_OCPOLARITY_HIGH;
		sConfig.OCFastMode   = TIM_OCFAST_DISABLE;
		sConfig.OCNPolarity  = TIM_OCNPOLARITY_HIGH;
		sConfig.OCNIdleState = TIM_OCNIDLESTATE_RESET;
		sConfig.OCIdleState  = TIM_OCIDLESTATE_RESET;
		sConfig.Pulse = 5000;//period/2
	
		HAL_TIM_PWM_ConfigChannel(&TimerLED, &sConfig, TIM_CHANNEL_3);
		HAL_TIM_PWM_ConfigChannel(&TimerLED, &sConfig, TIM_CHANNEL_4);
		
		LED1_Init_State = LED_AfPWM_Init;
		LED2_Init_State = LED_AfPWM_Init;
	}
}	

/**
* @brief De-Initialization du Timer en mode PWM
* @param None
* @retval None
*/
void IHM_LED_DeInit(void){
	HAL_TIM_PWM_DeInit(&TimerLED);
}


/**
* @brief TIM PWM Initialization
* @param htim: TIM handle pointer
* @retval None
*/
void HAL_TIM_PWM_MspInit(TIM_HandleTypeDef *htim){
	

	
	__HAL_RCC_TIM1_CLK_ENABLE();
	

}



void IHM_LED_Continious_Init(uint8_t LEDx)
{
  GPIO_InitTypeDef  GPIO_InitStruct = {0};

  /* Enable the GPIO_LED Clock */
  __HAL_RCC_GPIOA_CLK_ENABLE();

	if(LEDx == LED1)
	{
		GPIO_InitStruct.Pin   = LED_1_Pin;
		GPIO_InitStruct.Mode  = GPIO_MODE_OUTPUT_PP;
		GPIO_InitStruct.Pull  = GPIO_NOPULL;
		GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
		HAL_GPIO_Init(LED_1_2_Port, &GPIO_InitStruct);
		
		LED1_Init_State = LED_OutputPP_Init;
		
	}

	if(LEDx == LED2)
	{
		GPIO_InitStruct.Pin   = LED_2_Pin;
		GPIO_InitStruct.Mode  = GPIO_MODE_OUTPUT_PP;
		GPIO_InitStruct.Pull  = GPIO_NOPULL;
		GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
		HAL_GPIO_Init(LED_1_2_Port, &GPIO_InitStruct);
		
		LED2_Init_State = LED_OutputPP_Init;
	}

	
	if (LEDx == LED1_2)
	{
  /* Configure the GPIO_LED pin */
		GPIO_InitStruct.Pin   = LED_1_Pin;
		GPIO_InitStruct.Mode  = GPIO_MODE_OUTPUT_PP;
		GPIO_InitStruct.Pull  = GPIO_NOPULL;
		GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
		HAL_GPIO_Init(LED_1_2_Port, &GPIO_InitStruct);
	
		GPIO_InitStruct.Pin   = LED_2_Pin;
		GPIO_InitStruct.Mode  = GPIO_MODE_OUTPUT_PP;
		GPIO_InitStruct.Pull  = GPIO_NOPULL;
		GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
		HAL_GPIO_Init(LED_1_2_Port, &GPIO_InitStruct);
		
		LED1_Init_State = LED_OutputPP_Init;
		LED2_Init_State = LED_OutputPP_Init;
	}
}





void IHM_LED_On(uint8_t LEDx)
{
	if ( LEDx == 0 ) 
	{
		HAL_GPIO_WritePin(LED_1_2_Port, LED_1_Pin, GPIO_PIN_SET);
	}
	
	if ( LEDx == 1 ) 
	{
		HAL_GPIO_WritePin(LED_1_2_Port, LED_2_Pin, GPIO_PIN_SET);
	}
	
	if ( LEDx == 2 ) 
	{
		HAL_GPIO_WritePin(LED_1_2_Port, LED_1_Pin, GPIO_PIN_SET);
		HAL_GPIO_WritePin(LED_1_2_Port, LED_2_Pin, GPIO_PIN_SET);
	}
}


void IHM_LED_Off(uint8_t LEDx)
{
	if ( LEDx == 0 ) 
	{
		HAL_GPIO_WritePin(LED_1_2_Port, LED_1_Pin, GPIO_PIN_RESET);
	}
	
	if ( LEDx == 1 ) 
	{
		HAL_GPIO_WritePin(LED_1_2_Port, LED_2_Pin, GPIO_PIN_RESET);
	}
	
	if ( LEDx == 2 ) 
	{
		HAL_GPIO_WritePin(LED_1_2_Port, LED_1_Pin, GPIO_PIN_RESET);
		HAL_GPIO_WritePin(LED_1_2_Port, LED_2_Pin, GPIO_PIN_RESET);
	}
}






void IHM_PB_Init(void)
{
	GPIO_InitTypeDef GPIO_InitStruct = {0};
	
	  // Enable the GPIOC Clock 
  __HAL_RCC_GPIOC_CLK_ENABLE();
	
	  // Configure PB pin as exti interrupt on rising and falling  
	GPIO_InitStruct.Pin    = PB_Pin;
  GPIO_InitStruct.Mode   = GPIO_MODE_IT_RISING_FALLING;
	GPIO_InitStruct.Pull   = GPIO_NOPULL;
	GPIO_InitStruct.Speed  = GPIO_SPEED_FREQ_HIGH;
	HAL_GPIO_Init(PB_Port, &GPIO_InitStruct);  
	
	// Wait until realise of the PB to set interrupt  
	//NORMALEMENT OK!!!!( don't forget to switch state of the IO with the new card)
	
	while(IHM_PB_GetState() == GPIO_PIN_SET){} 
		
	// Enable exti interrupt with the lowest priority (TBD with the other part of the project)
		// other exti line for the button on final card. (Line 0)
    HAL_NVIC_SetPriority(PB_Exti_IRQn, 0x0F, 0);
    HAL_NVIC_EnableIRQ(PB_Exti_IRQn);
	// in case of using other exti line between 10 and 15 the priority have to be reevaluated
} 

uint32_t IHM_PB_GetState(void)
{
  return HAL_GPIO_ReadPin(PB_Port,PB_Pin);
}


void IHM_PB_Callback (void)
{
		// sur carte nucléo appuie PB = GND, sur notre carte appuie PB = 3V3 ==> logique inverse
   // NORMALEMENT OK!!!! while(BSP_PB_GetState(BUTTON_USER) == GPIO_PIN_RESET){}

		
	if (Global_App_State == Global_App_Charge_Mode)
	{
		if (HAL_GetTick()>= PB_Constructor_Time_First_press + 30000)
		{
			PB_Constructor_Time_First_press = 0;
			PB_Constructor_Press_Counter = 0;
		}
		
		HAL_NVIC_DisableIRQ(PB_Exti_IRQn);
		HAL_NVIC_ClearPendingIRQ(PB_Exti_IRQn);
		HAL_Delay(500);
		
		if (PB_Constructor_Press_Counter == 0)
		{
			PB_Constructor_Time_First_press = HAL_GetTick();
		}

		PB_Constructor_Press_Counter++;
		HAL_NVIC_EnableIRQ(PB_Exti_IRQn);
		
		if (PB_Constructor_Press_Counter == 15)
		{
			PB_Constructor_Time_First_press = 0;
			PB_Constructor_Press_Counter = 0;
			IHM_PB_Go_To_Factory_Mode();
		}
		
	}
	else
	{
		if ( (IHM_PB_GetState() == GPIO_PIN_SET) && (Global_App_State == Global_App_Run_Mode))
		{
			PB_Time_Press = HAL_GetTick();
		
			HAL_NVIC_DisableIRQ(PB_Exti_IRQn);
			HAL_NVIC_ClearPendingIRQ(PB_Exti_IRQn);
			
			PB_Wait_Release = 1;
			
			var_test_exti_set++;
		}
		if ( (IHM_PB_GetState() == GPIO_PIN_RESET) && (Global_App_State == Global_App_Run_Mode))
		{
			PB_Time_Release = HAL_GetTick();
			
			if ((PB_Wait_Release == 1) &&(PB_Time_Release >= (PB_Time_Press + 3000)))
			{

				// turn off the device (add code to stop evry thing and go to shutdown mode)
				var_test_exti_sysoff++;
				Buton_Hold_Callback();
	
			}
				
			
			PB_Time_Press 		= 0xFFFFFFFF;
			PB_Time_Release		= 0;
			PB_Wait_Release		= 0; 
			
			var_test_exti_reset++;		
		}
	}
}

void Tempo_PB_Process(void)
{

	if ((PB_Wait_Release == 1) && (HAL_GetTick() >= (PB_Time_Press + 500)))
	{
		HAL_NVIC_EnableIRQ(PB_Exti_IRQn);
	}
	if ((PB_Wait_Release == 1) &&(HAL_GetTick() >= (PB_Time_Press + 3500)))
	{

	// turn off the device (add code to stop evry thing and go to shutdown mode)
	var_test_exti_sysoff++;
	Buton_Hold_Callback();
	PB_Time_Press 		= 0xFFFFFFFF;
	PB_Time_Release		= 0;
	PB_Wait_Release		= 0; 
	}
}

__weak void IHM_PB_Go_To_Factory_Mode(void)
{
	
}

__weak void Buton_Hold_Callback (void)
{
	
}


