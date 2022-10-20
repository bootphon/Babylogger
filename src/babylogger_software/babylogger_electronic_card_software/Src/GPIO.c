
#include "GPIO.h"
#include "IHM.h"
#include "sensor_acquisition.h"
#include "fatfs.h"

/* private variables */

uint16_t variables_test_chargIn = 0;
extern uint8_t Global_App_State;
uint8_t Wifi_reset_rqst = 0;
uint8_t Wifi_Format_Sd_rqst = 0;

/*******************************************************************************
* Function Name  : 
* Description    : 
* Input          : 
* Return         : 
*******************************************************************************/
/**
  * @brief  Configuration des interruptions externes
  * @retval None
  */
void GPIO_AccGyr_INT_Init(void)
{
  GPIO_InitTypeDef   GPIO_InitStructure;

  /* Enable GPIOB clock */
  __HAL_RCC_GPIOB_CLK_ENABLE();
	
 /*Interruption LSM6DSL pin PB2*/
  GPIO_InitStructure.Mode = GPIO_MODE_IT_RISING;
  GPIO_InitStructure.Pull = GPIO_PULLDOWN;
  GPIO_InitStructure.Speed = GPIO_SPEED_FREQ_LOW;
  GPIO_InitStructure.Pin = GPIO_PIN_2;
  HAL_GPIO_Init(GPIOB, &GPIO_InitStructure);
		
  /* Enable and set EXTI lines 1 to 2 Interrupt to the lowest priority */
  HAL_NVIC_SetPriority(EXTI2_IRQn, 4, 0);
  HAL_NVIC_EnableIRQ(EXTI2_IRQn);

}

void GPIO_Monitor_Bat_INT_Init(void)
{
  GPIO_InitTypeDef   GPIO_InitStructure;
	
  /* Enable GPIOB clock */
  __HAL_RCC_GPIOB_CLK_ENABLE();
	
  /*Interruption STC3115 pin PC5*/
  GPIO_InitStructure.Mode = GPIO_MODE_IT_FALLING;
  GPIO_InitStructure.Pull = GPIO_PULLUP;
  GPIO_InitStructure.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
  GPIO_InitStructure.Pin = GPIO_PIN_5;
  HAL_GPIO_Init(GPIOC, &GPIO_InitStructure);
	
  HAL_NVIC_SetPriority(EXTI9_5_IRQn, 6, 0);
  HAL_NVIC_EnableIRQ(EXTI9_5_IRQn);
	
}

/*******************************************************************************
* Function Name  : 
* Description    : 
* Input          : 
* Return         : 
*******************************************************************************/
void GPIO_BLE_Init(void)
{
  GPIO_InitTypeDef GPIO_InitStruct = {0};

  /* GPIO Ports Clock Enable */
  __HAL_RCC_GPIOA_CLK_ENABLE();

  /*Configure GPIO pin Output Level CS */
  HAL_GPIO_WritePin(GPIOA, GPIO_PIN_4, GPIO_PIN_RESET);

  /*Configure GPIO pin Output Level RST */
  HAL_GPIO_WritePin(GPIOA, GPIO_PIN_1, GPIO_PIN_RESET);
	
  /*Configure GPIO pin : PA3 SPI IRQ*/
  GPIO_InitStruct.Pin = GPIO_PIN_3;
  GPIO_InitStruct.Mode = GPIO_MODE_IT_RISING;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

  /*Configure GPIO pin : PA4 BLE CS */
  GPIO_InitStruct.Pin = GPIO_PIN_4;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

  /*Configure GPIO pin : PA1 BLE Reset */
  GPIO_InitStruct.Pin = GPIO_PIN_1;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

  /* EXTI interrupt init*/
  HAL_NVIC_SetPriority(EXTI3_IRQn, 2, 0);
  HAL_NVIC_EnableIRQ(EXTI3_IRQn);
	
}

/*******************************************************************************
* Function Name  : 
* Description    : 
* Input          : 
* Return         : 
*******************************************************************************/
/**
  * @brief Configuration pin EEPROM_WC
  * @param None
  * @retval None
  */
void GPIO_EEPROM_WC_Init(void){
  
  /* Enable GPIOB clock */
  __HAL_RCC_GPIOB_CLK_ENABLE();
 GPIO_InitTypeDef   GPIO_InitStructure;

 /*Configuration pin PB0*/
  GPIO_InitStructure.Mode = GPIO_MODE_OUTPUT_OD;
  GPIO_InitStructure.Pull = GPIO_NOPULL;
  GPIO_InitStructure.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
  GPIO_InitStructure.Pin = GPIO_PIN_0;
  HAL_GPIO_Init(GPIOB, &GPIO_InitStructure);
	
  //Protection EEPROM read only
  HAL_GPIO_WritePin (GPIOB, GPIO_PIN_0, GPIO_PIN_SET);
}

/*******************************************************************************
* Function Name  : 
* Description    : 
* Input          : 
* Return         : 
*******************************************************************************/
/**
  * @brief Drive wifi Alim
  * @param None
  * @retval None
  */
void GPIO_Wifi_Init(void){
 
  /* Enable GPIOB clock */
  __HAL_RCC_GPIOB_CLK_ENABLE();
  GPIO_InitTypeDef   GPIO_InitStructure;

  /*Configuration pin PB0*/
  GPIO_InitStructure.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStructure.Pull = GPIO_NOPULL;
  GPIO_InitStructure.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
  GPIO_InitStructure.Pin = GPIO_PIN_5;
  HAL_GPIO_Init(GPIOB, &GPIO_InitStructure);
	
}
void GPIO_Wifi_INT_Init (void)
{
  // provisoir PB4 normalement pour mode control non exti
  __HAL_RCC_GPIOB_CLK_ENABLE();
  GPIO_InitTypeDef   GPIO_InitStructure;
  GPIO_InitStructure.Mode = GPIO_MODE_IT_FALLING;
  GPIO_InitStructure.Pull = GPIO_NOPULL;
  GPIO_InitStructure.Speed = GPIO_SPEED_FREQ_LOW;
  GPIO_InitStructure.Pin = GPIO_PIN_4;
  HAL_GPIO_Init(GPIOB, &GPIO_InitStructure);
	
  HAL_NVIC_SetPriority(EXTI4_IRQn, 16 , 0);
  HAL_NVIC_EnableIRQ(EXTI4_IRQn);
	
  /*Interruption on  wifi transfert end on PB15*/
  GPIO_InitStructure.Mode = GPIO_MODE_IT_RISING;
  GPIO_InitStructure.Pull = GPIO_NOPULL;
  GPIO_InitStructure.Speed = GPIO_SPEED_FREQ_LOW;
  GPIO_InitStructure.Pin = GPIO_PIN_15;
  HAL_GPIO_Init(GPIOB, &GPIO_InitStructure);
	
  // active interrupt by charger power good
  // HAL_NVIC_SetPriority(EXTI15_10_IRQn, 10 , 0);
  //HAL_NVIC_EnableIRQ(EXTI15_10_IRQn);
}

void GPIO_Wifi_Off (void)
{
  HAL_GPIO_WritePin (GPIOB, GPIO_PIN_5, GPIO_PIN_SET);
}
void GPIO_Wifi_On (void)
{
  HAL_GPIO_WritePin (GPIOB, GPIO_PIN_5, GPIO_PIN_RESET);
}

void GPIO_Wifi_Force_Reset(void)
{
  GPIO_Wifi_Off();
  HAL_Delay(1000);
  GPIO_Wifi_On();
}

void GPIO_Wifi_Process (void)
{
  if (Wifi_reset_rqst == 1)
     {
	GPIO_Wifi_Force_Reset();
	Wifi_reset_rqst = 0;
      }
	
   if ( Wifi_Format_Sd_rqst == 1 )
      {
	GPIO_Wifi_Off();
	//FATFS_Format_uSD ();	
	Wifi_Format_Sd_rqst = 0;
	IHM_LED_Stop(LED2);	
		
	}
}
    
/**
  * @brief EXTI line detection callbacks
  * @param GPIO_Pin: Specifies the pins connected EXTI line
  * @retval None
  */
void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin)
{

 if (GPIO_Pin == GPIO_PIN_2)
 {
	Sensors_Callback_LSM6DSL();
 }

  if (GPIO_Pin == PB_Pin)
  {    
			
	IHM_PB_Callback();

  }
  if (GPIO_Pin == GPIO_PIN_13)
  {
	GPIO_BAT_Charg_PG_Detect_Callback();
  }
	
  if (GPIO_Pin == GPIO_PIN_4)
  {
	// wifi rest rqst 
	Wifi_reset_rqst = 1;
   }
	
  if (GPIO_Pin == GPIO_PIN_5)
  {
	//Interruption Alarme batterie
	Sensors_Callback_STC3115();
  }
	
  if (GPIO_Pin == GPIO_PIN_15)
  {
	// wifi end task
	//Wifi_Format_Sd_rqst = 1;
		
  }
}

/*////////////////////////////////// Fnctions chargeur batterie /////////////////////////////////*/

void GPIO_BAT_Charg_Init(void){

  /* Enable GPIOB clock */
  __HAL_RCC_GPIOB_CLK_ENABLE();
  /* Enable GPIOC clock */
  __HAL_RCC_GPIOC_CLK_ENABLE();
	
  GPIO_InitTypeDef   GPIO_InitStructure;

  /*Configuration pin Stat1 PB8 as input*/
  GPIO_InitStructure.Mode = GPIO_MODE_INPUT;
  GPIO_InitStructure.Pull = GPIO_NOPULL;
  GPIO_InitStructure.Speed = GPIO_SPEED_FREQ_LOW;
  GPIO_InitStructure.Pin = GPIO_PIN_8;
  HAL_GPIO_Init(GPIOB, &GPIO_InitStructure);
	
   /*Configuration pin Stat2 PB9 as input*/
  GPIO_InitStructure.Mode = GPIO_MODE_INPUT;
  GPIO_InitStructure.Pull = GPIO_NOPULL;
  GPIO_InitStructure.Speed = GPIO_SPEED_FREQ_LOW;
  GPIO_InitStructure.Pin = GPIO_PIN_9;
  HAL_GPIO_Init(GPIOB, &GPIO_InitStructure);
	
  /*Interruption on  BAT Charger Power good pin PC13*/
  GPIO_InitStructure.Mode = GPIO_MODE_IT_RISING_FALLING;
  GPIO_InitStructure.Pull = GPIO_NOPULL;
  GPIO_InitStructure.Speed = GPIO_SPEED_FREQ_LOW;
  GPIO_InitStructure.Pin = GPIO_PIN_13;
  HAL_GPIO_Init(GPIOC, &GPIO_InitStructure);
	
	
  /* Enable and set EXTI lines 1 to 2 Interrupt to the lowest priority */
  HAL_NVIC_SetPriority(EXTI15_10_IRQn, 10 , 0);
  HAL_NVIC_ClearPendingIRQ(EXTI15_10_IRQn);
  HAL_NVIC_EnableIRQ(EXTI15_10_IRQn);	
}

void GPIO_BAT_Charg_PG_Detect_Callback(void)
{
	if ((HAL_GPIO_ReadPin(GPIOC, GPIO_PIN_13) == GPIO_PIN_RESET) && (Global_App_State == Global_App_Run_Mode))
	{
		Buton_Hold_Callback();
	}
	if ((HAL_GPIO_ReadPin(GPIOC, GPIO_PIN_13) == GPIO_PIN_SET) && (Global_App_State == Global_App_Charge_Mode))
	{
		HAL_NVIC_DisableIRQ(EXTI15_10_IRQn);
		HAL_NVIC_ClearPendingIRQ(EXTI15_10_IRQn);
		variables_test_chargIn++;
		HAL_Delay(2000);
		if(HAL_GPIO_ReadPin(GPIOC, GPIO_PIN_13) == GPIO_PIN_SET) 
		{
			Bat_Monitor_Stop();
			PWR_Go_To_Shutdown_Mode();
		}
		else 
		{
			HAL_NVIC_EnableIRQ(EXTI15_10_IRQn);
		}
	}

	
}

uint8_t GPIO_BAT_Charg_Get_State (void) 
{
	uint8_t state;
	
	if (HAL_GPIO_ReadPin(GPIOC, GPIO_PIN_13) == GPIO_PIN_SET)
	{
		state = Bat_Charg_Shutdown; 
	}
	else
	{
		if (HAL_GPIO_ReadPin(GPIOB, GPIO_PIN_8) == GPIO_PIN_SET)
		{
			if (HAL_GPIO_ReadPin(GPIOB, GPIO_PIN_9) == GPIO_PIN_SET)
			{
				state = Bat_Charg_Standby;
			}
			else 
			{
				state = Bat_Charg_Complete ; 
			}
		}
		else
		{
			if (HAL_GPIO_ReadPin(GPIOB, GPIO_PIN_9) == GPIO_PIN_SET)
			{
				state = Bat_Charg_In_Progress ; 
			}
			else 
			{
				state = Bat_Charg_Sys_Test_Mode;
			}
		}
	}
	return state;
}


