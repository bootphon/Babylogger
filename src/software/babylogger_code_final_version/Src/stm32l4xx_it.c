/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file    stm32l4xx_it.c
  * @brief   Interrupt Service Routines.
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2019 STMicroelectronics.
  * All rights reserved.</center></h2>
  *
  * This software component is licensed by ST under Ultimate Liberty license
  * SLA0044, the "License"; You may not use this file except in compliance with
  * the License. You may obtain a copy of the License at:
  *                             www.st.com/SLA0044
  *
  ******************************************************************************
  */
/* USER CODE END Header */

/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "stm32l4xx_it.h"
//#include "SPI_bus.h"
/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN TD */

/* USER CODE END TD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */
 
/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
/* USER CODE BEGIN PV */

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */

/* USER CODE END 0 */

/* External variables --------------------------------------------------------*/
//extern DMA_HandleTypeDef hdma_sdmmc1_rx;
extern DMA_HandleTypeDef hdma_sdmmc1_tx;
extern SD_HandleTypeDef hsd1;

extern DMA_HandleTypeDef hDfsdmFLT0Dma;
extern DMA_HandleTypeDef hDfsdmFLT1Dma;
extern DMA_HandleTypeDef hDfsdmFLT2Dma;
extern DMA_HandleTypeDef hDfsdmFLT3Dma;

extern RTC_HandleTypeDef RtcHandle;

//extern CRYP_HandleTypeDef CrypHandle;
extern DMA_HandleTypeDef  CryphdmaIn;
extern DMA_HandleTypeDef  CryphdmaOut;

#ifdef USE_USART1_FOR_WIFI
extern UART_HandleTypeDef UartHandle;
#endif

// meriem

extern TIM_HandleTypeDef TimHandle;
extern DMA_HandleTypeDef i2c_hdma_rx;
extern I2C_HandleTypeDef hi2c;

// fin meriem

/* USER CODE BEGIN EV */

/* USER CODE END EV */

/******************************************************************************/
/*           Cortex-M4 Processor Interruption and Exception Handlers          */ 
/******************************************************************************/
/**
  * @brief This function handles Non maskable interrupt.
  */
void NMI_Handler(void)
{
  /* USER CODE BEGIN NonMaskableInt_IRQn 0 */

  /* USER CODE END NonMaskableInt_IRQn 0 */
  /* USER CODE BEGIN NonMaskableInt_IRQn 1 */

  /* USER CODE END NonMaskableInt_IRQn 1 */
}

/**
  * @brief This function handles Hard fault interrupt.
  */
void HardFault_Handler(void)
{
	
	//IHM_Debug_Led(11); // test de proto
	
  /* USER CODE BEGIN HardFault_IRQn 0 */
	PWR_Reset_System();
  /* USER CODE END HardFault_IRQn 0 */
  while (1)
  {
    /* USER CODE BEGIN W1_HardFault_IRQn 0 */
    /* USER CODE END W1_HardFault_IRQn 0 */
  }
}

/**
  * @brief This function handles Memory management fault.
  */
void MemManage_Handler(void)
{
	//IHM_Debug_Led(13); // test de proto
	
  /* USER CODE BEGIN MemoryManagement_IRQn 0 */
	PWR_Reset_System();
  /* USER CODE END MemoryManagement_IRQn 0 */
  while (1)
  {
    /* USER CODE BEGIN W1_MemoryManagement_IRQn 0 */
    /* USER CODE END W1_MemoryManagement_IRQn 0 */
  }
}

/**
  * @brief This function handles Prefetch fault, memory access fault.
  */
void BusFault_Handler(void)
{
	//IHM_Debug_Led(10); // test de proto
	
  /* USER CODE BEGIN BusFault_IRQn 0 */
	PWR_Reset_System();
  /* USER CODE END BusFault_IRQn 0 */
  while (1)
  {
    /* USER CODE BEGIN W1_BusFault_IRQn 0 */
    /* USER CODE END W1_BusFault_IRQn 0 */
  }
}

/**
  * @brief This function handles Undefined instruction or illegal state.
  */
void UsageFault_Handler(void)
{
  /* USER CODE BEGIN UsageFault_IRQn 0 */
	//PWR_Reset_System();
  /* USER CODE END UsageFault_IRQn 0 */
  while (1)
  {
    /* USER CODE BEGIN W1_UsageFault_IRQn 0 */
    /* USER CODE END W1_UsageFault_IRQn 0 */
  }
}

/**
  * @brief This function handles System service call via SWI instruction.
  */
void SVC_Handler(void)
{
  /* USER CODE BEGIN SVCall_IRQn 0 */

  /* USER CODE END SVCall_IRQn 0 */
  /* USER CODE BEGIN SVCall_IRQn 1 */

  /* USER CODE END SVCall_IRQn 1 */
}

/**
  * @brief This function handles Debug monitor.
  */
void DebugMon_Handler(void)
{
  /* USER CODE BEGIN DebugMonitor_IRQn 0 */

  /* USER CODE END DebugMonitor_IRQn 0 */
  /* USER CODE BEGIN DebugMonitor_IRQn 1 */

  /* USER CODE END DebugMonitor_IRQn 1 */
}

/**
  * @brief This function handles Pendable request for system service.
  */
void PendSV_Handler(void)
{
  /* USER CODE BEGIN PendSV_IRQn 0 */

  /* USER CODE END PendSV_IRQn 0 */
  /* USER CODE BEGIN PendSV_IRQn 1 */

  /* USER CODE END PendSV_IRQn 1 */
}

/**
  * @brief This function handles System tick timer.
  */
void SysTick_Handler(void)
{
  /* USER CODE BEGIN SysTick_IRQn 0 */

  /* USER CODE END SysTick_IRQn 0 */
  HAL_IncTick();
  /* USER CODE BEGIN SysTick_IRQn 1 */
	HAL_SYSTICK_IRQHandler();
  /* USER CODE END SysTick_IRQn 1 */
}

/******************************************************************************/
/* STM32L4xx Peripheral Interrupt Handlers                                    */
/* Add here the Interrupt Handlers for the used peripherals.                  */
/* For the available peripheral interrupt handler names,                      */
/* please refer to the startup file (startup_stm32l4xx.s).                    */
/******************************************************************************/

/**
  * @brief This function handles SDMMC1 global interrupt.
  */
void SDMMC1_IRQHandler(void)
{
  /* USER CODE BEGIN SDMMC1_IRQn 0 */

  /* USER CODE END SDMMC1_IRQn 0 */
  HAL_SD_IRQHandler(&hsd1);
  /* USER CODE BEGIN SDMMC1_IRQn 1 */

  /* USER CODE END SDMMC1_IRQn 1 */
}


/**
  * @brief This function handles DMA2 channel5 global interrupt.
  */
void DMA2_Channel5_IRQHandler(void)
{
  /* USER CODE BEGIN DMA2_Channel5_IRQn 0 */

  /* USER CODE END DMA2_Channel5_IRQn 0 */
  HAL_DMA_IRQHandler(&hdma_sdmmc1_tx);
 //Malik
	//HAL_DMA_IRQHandler(&hdma_sdmmc1_rx);
}

/* USER CODE BEGIN 1 */
void DMA1_Channel4_IRQHandler(void)
{
  HAL_DMA_IRQHandler(&hDfsdmFLT0Dma);
}

void DMA1_Channel5_IRQHandler(void)
{
  HAL_DMA_IRQHandler(&hDfsdmFLT1Dma);
	
}
void DMA1_Channel6_IRQHandler(void)
{
  HAL_DMA_IRQHandler(&hDfsdmFLT2Dma);
}

void DMA1_Channel7_IRQHandler(void)
{
  HAL_DMA_IRQHandler(&hDfsdmFLT3Dma);
	
}

void RTC_Alarm_IRQHandler(void)
{
  HAL_RTC_AlarmIRQHandler(&RtcHandle);
}

void EXTI0_IRQHandler(void)
{
  HAL_GPIO_EXTI_IRQHandler(PB_Pin);
}

/*
void DMA2_Channel1_IRQHandler(void)
{
   //CRYP input data 
   HAL_DMA_IRQHandler(&CryphdmaIn);
}


void DMA2_Channel3_IRQHandler(void)
{
  //CRYP output data 
   HAL_DMA_IRQHandler(&CryphdmaOut);
}
*/

/*void AES_IRQHandler(void)
{
	HAL_CRYP_IRQHandler(&CrypHandle);
}*/


/**
  * @brief This function handles TIM3 interrupts.
  */
void TIM3_IRQHandler(void)
{
	HAL_TIM_IRQHandler(&TimHandle);
}

/**
  * @brief This function handles DMA1 Channel5 interrupts.
  */
void DMA2_Channel1_IRQHandler(void)
{
	HAL_DMA_IRQHandler(&i2c_hdma_rx);
}

/**
  * @brief This function handles I2C2 errors interrupts.
  */
void I2C4_ER_IRQHandler (void)
{
	HAL_I2C_ER_IRQHandler(&hi2c);
}

/**
  * @brief This function handles I2C2 events interrupts.
  */
void I2C4_EV_IRQHandler (void)
{
	HAL_I2C_EV_IRQHandler(&hi2c);
}

void EXTI2_IRQHandler(void){
	HAL_GPIO_EXTI_IRQHandler(GPIO_PIN_2);
	
}


#ifdef USE_USART1_FOR_WIFI
void USARTx_IRQHandler(void)
{
  HAL_UART_IRQHandler(&UartHandle);
}
#endif



void EXTI15_10_IRQHandler(void){
	HAL_GPIO_EXTI_IRQHandler(GPIO_PIN_13);
	HAL_GPIO_EXTI_IRQHandler(GPIO_PIN_15);
}

void EXTI4_IRQHandler(void){
	HAL_GPIO_EXTI_IRQHandler(GPIO_PIN_4);
}

void EXTI9_5_IRQHandler(void){
	
	HAL_GPIO_EXTI_IRQHandler(GPIO_PIN_5);
	
}

void EXTI3_IRQHandler(void){
	
	HAL_EXTI_IRQHandler(&H_EXTI_3);
	
}

/* USER CODE END 1 */
/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
