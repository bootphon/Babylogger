/**
  ******************************************************************************
  * @file  : custom.c
  * @brief : Source file for the BSP Common driver
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

/* Includes ------------------------------------------------------------------*/ 
#include "UART_DEBUG.h"
#include "stm32l4xx_hal_exti.h"

/** @defgroup BSP BSP
 * @{
 */ 

/** @defgroup CUSTOM CUSTOM
 * @{
 */   
    
/** @defgroup CUSTOM_LOW_LEVEL CUSTOM LOW LEVEL
 *  @brief This file provides set of firmware functions to manage Leds and push-button
 *         available on STM32L4xx-Nucleo Kit from STMicroelectronics.
 * @{
 */ 

/**
 * @}
 */ 

/** @defgroup CUSTOM_LOW_LEVEL_Private_Defines CUSTOM LOW LEVEL Private Defines
 * @{
 */ 

/** @defgroup CUSTOM_LOW_LEVEL_FunctionPrototypes CUSTOM LOW LEVEL Private Function Prototypes
 * @{
 */
typedef void (* BSP_EXTI_LineCallback) (void);

/**
 * @}
 */ 

/** @defgroup CUSTOM_LOW_LEVEL_Private_Variables CUSTOM LOW LEVEL Private Variables
 * @{
 */

USART_TypeDef* COM_USART[COMn] = {COM1_UART};
UART_HandleTypeDef hcom_uart[COMn];


#if (USE_COM_LOG > 0)
static COM_TypeDef COM_ActiveLogPort;
#endif
#if (USE_HAL_UART_REGISTER_CALLBACKS == 1)
static uint32_t IsUsart1MspCbValid = 0;
#endif
__weak HAL_StatusTypeDef MX_USART1_UART_Init(UART_HandleTypeDef* huart);
/**
 * @}
 */ 

/** @defgroup CUSTOM_LOW_LEVEL_Private_Functions CUSTOM LOW LEVEL Private Functions
 * @{
 */ 

#if (USE_BSP_COM_FEATURE > 0)
static void USART1_MspInit(UART_HandleTypeDef *huart);
static void USART1_MspDeInit(UART_HandleTypeDef *huart); 
#endif
/**
 * @brief  This method returns the STM32L4xx NUCLEO BSP Driver revision
 * @retval version: 0xXYZR (8bits for each decimal, R for RC)
 */
int32_t BSP_GetVersion(void)
{
  return (int32_t)__CUSTOM_BSP_VERSION;
}


#if (USE_BSP_COM_FEATURE > 0)
/**
 * @brief  Configures COM port.
 * @param  COM: COM port to be configured.
 *              This parameter can be COM1
 * @param  UART_Init: Pointer to a UART_HandleTypeDef structure that contains the
 *                    configuration information for the specified USART peripheral.
 * @retval BSP error code
 */
int32_t BSP_COM_Init(COM_TypeDef COM) 
{
  int32_t ret = BSP_ERROR_NONE;
  
  if(COM > COMn)
  {
    ret = BSP_ERROR_WRONG_PARAM;
  }
  else
  {  
     hcom_uart[COM].Instance = COM_USART[COM];
#if (USE_HAL_UART_REGISTER_CALLBACKS == 0)
    /* Init the UART Msp */
    USART1_MspInit(&hcom_uart[COM]);
#else
    if(IsUsart1MspCbValid == 0U)
    {
      if(BSP_COM_RegisterDefaultMspCallbacks(COM) != BSP_ERROR_NONE)
      {
        return BSP_ERROR_MSP_FAILURE;
      }
    }
#endif
  
    if (MX_USART1_UART_Init(&hcom_uart[COM]))
    {
      ret = BSP_ERROR_PERIPH_FAILURE;
    }
  }

  return ret;
}

/**
 * @brief  DeInit COM port.
 * @param  COM COM port to be configured.
 *             This parameter can be COM1
 * @retval BSP status
 */
int32_t BSP_COM_DeInit(COM_TypeDef COM)
{
  int32_t ret = BSP_ERROR_NONE;
  
  if(COM > COMn)
  {
    ret = BSP_ERROR_WRONG_PARAM;
  }
  else
  {  
    /* USART configuration */
    hcom_uart[COM].Instance = COM_USART[COM];
  
    #if (USE_HAL_UART_REGISTER_CALLBACKS == 0)  
      USART1_MspDeInit(&hcom_uart[COM]);  
    #endif /* (USE_HAL_UART_REGISTER_CALLBACKS == 0) */
  
    if(HAL_UART_DeInit(&hcom_uart[COM]) != HAL_OK)
    {
      ret = BSP_ERROR_PERIPH_FAILURE;
    }
  }
  
  return ret;
}

/**
 * @brief  Configures COM port.
 * @param  huart USART handle
 *               This parameter can be COM1
 * @param  COM_Init Pointer to a UART_HandleTypeDef structure that contains the
 *                  configuration information for the specified USART peripheral.
 * @retval HAL error code
 */

/* USART1 init function */ 

__weak HAL_StatusTypeDef MX_USART1_UART_Init(UART_HandleTypeDef* huart)
{
  HAL_StatusTypeDef ret = HAL_OK;
  huart->Instance = USART1;
  huart->Init.BaudRate = 115200;
  huart->Init.WordLength = UART_WORDLENGTH_8B;
  huart->Init.StopBits = UART_STOPBITS_1;
  huart->Init.Parity = UART_PARITY_NONE;
  huart->Init.Mode = UART_MODE_TX_RX;
  huart->Init.HwFlowCtl = UART_HWCONTROL_NONE;
  huart->Init.OverSampling = UART_OVERSAMPLING_16;
  huart->Init.OneBitSampling = UART_ONE_BIT_SAMPLE_DISABLE;
  huart->AdvancedInit.AdvFeatureInit = UART_ADVFEATURE_NO_INIT;
  if (HAL_UART_Init(huart) != HAL_OK)
  {
    ret = HAL_ERROR;
  }

  return ret;
}

#endif 
#if (USE_HAL_UART_REGISTER_CALLBACKS == 1) 
/**
 * @brief Register Default USART1 Bus Msp Callbacks
 * @retval BSP status
 */
int32_t BSP_COM_RegisterDefaultMspCallbacks(COM_TypeDef COM)
{
  int32_t ret = BSP_ERROR_NONE;
  
  if(COM >= COMn)
  {
    ret = BSP_ERROR_WRONG_PARAM;
  }
  else
  {  
    
    __HAL_UART_RESET_HANDLE_STATE(&hcom_uart[COM]);
  
    /* Register default MspInit/MspDeInit Callback */
    if(HAL_UART_RegisterCallback(&hcom_uart[COM], HAL_UART_MSPINIT_CB_ID, USART1_MspInit) != HAL_OK)
    {
      ret = BSP_ERROR_PERIPH_FAILURE;
    }
    else if(HAL_UART_RegisterCallback(&hcom_uart[COM], HAL_UART_MSPDEINIT_CB_ID, USART1_MspDeInit) != HAL_OK)
    {
      ret = BSP_ERROR_PERIPH_FAILURE;
    }
    else
    {
      IsUsart1MspCbValid = 1U;
    }
  }
  
  /* BSP status */  
  return ret;
}

/**
 * @brief Register USART1 Bus Msp Callback registering
 * @param Callbacks pointer to USART1 MspInit/MspDeInit callback functions
 * @retval BSP status
 */
int32_t BSP_COM_RegisterMspCallbacks (COM_TypeDef COM , BSP_COM_Cb_t *Callback)
{
  int32_t ret = BSP_ERROR_NONE;
  
  if(COM >= COMn)
  {
    ret = BSP_ERROR_WRONG_PARAM;
  }
  else
  {
    __HAL_UART_RESET_HANDLE_STATE(&hcom_uart[COM]);
  
    /* Register MspInit/MspDeInit Callbacks */
    if(HAL_UART_RegisterCallback(&hcom_uart[COM], HAL_UART_MSPINIT_CB_ID, Callback->pMspInitCb) != HAL_OK)
    {
      ret = BSP_ERROR_PERIPH_FAILURE;
    }
    else if(HAL_UART_RegisterCallback(&hcom_uart[COM], HAL_UART_MSPDEINIT_CB_ID, Callback->pMspDeInitCb) != HAL_OK)
    {
      ret = BSP_ERROR_PERIPH_FAILURE;
    }
    else
    {
      IsUsart1MspCbValid = 1U;
    }
  }
  
  /* BSP status */  
  return ret; 
}
#endif /* USE_HAL_UART_REGISTER_CALLBACKS */

#if (USE_COM_LOG > 0)
/**
 * @brief  Select the active COM port.
 * @param  COM COM port to be activated.
 *             This parameter can be COM1
 * @retval BSP status
 */
int32_t BSP_COM_SelectLogPort(COM_TypeDef COM)
{ 
  if(COM_ActiveLogPort != COM)
  {
    COM_ActiveLogPort = COM;
  } 
  return BSP_ERROR_NONE; 
}

#ifdef __GNUC__ 
 int __io_putchar (int ch) 
#else 
 int fputc (int ch, FILE *f) 
#endif /* __GNUC__ */ 
{ 
  (void)HAL_UART_Transmit(&hcom_uart[COM_ActiveLogPort], (uint8_t *)&ch, 1, COM_POLL_TIMEOUT); 
  return ch;
}
#endif /* USE_COM_LOG */ 
/**
 * @brief  Initializes USART1 MSP.
 * @param  huart USART1 handle
 * @retval None
 */

static void USART1_MspInit(UART_HandleTypeDef* uartHandle)
{
  GPIO_InitTypeDef GPIO_InitStruct;
  /* USER CODE BEGIN USART1_MspInit 0 */

  /* USER CODE END USART1_MspInit 0 */
    /* Enable Peripheral clock */
    __HAL_RCC_USART1_CLK_ENABLE();
  
    __HAL_RCC_GPIOB_CLK_ENABLE();
    /**USART1 GPIO Configuration    
    PB6     ------> USART1_TX
    PB7     ------> USART1_RX 
    */
    GPIO_InitStruct.Pin = GPIO_PIN_6|GPIO_PIN_7;
    GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
    GPIO_InitStruct.Alternate = GPIO_AF7_USART1;
    HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

  /* USER CODE BEGIN USART1_MspInit 1 */

  /* USER CODE END USART1_MspInit 1 */
}

static void USART1_MspDeInit(UART_HandleTypeDef* uartHandle)
{
  /* USER CODE BEGIN USART1_MspDeInit 0 */

  /* USER CODE END USART1_MspDeInit 0 */
    /* Peripheral clock disable */
    __HAL_RCC_USART1_CLK_DISABLE();
  
    /**USART1 GPIO Configuration    
    PB6     ------> USART1_TX
    PB7     ------> USART1_RX 
    */
    HAL_GPIO_DeInit(GPIOB, GPIO_PIN_6|GPIO_PIN_7);

  /* USER CODE BEGIN USART1_MspDeInit 1 */

  /* USER CODE END USART1_MspDeInit 1 */
}

/**
 * @}
 */ 

/**
 * @}
 */

/**
 * @}
 */    

/**
 * @}
 */  
/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/

