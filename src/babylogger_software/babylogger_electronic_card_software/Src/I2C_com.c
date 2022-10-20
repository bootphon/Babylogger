#include "I2C_com.h"

/* Private typedef -----------------------------------------------------------*/

I2C_HandleTypeDef hi2c;
DMA_HandleTypeDef i2c_hdma_rx;
/*******************************************************************************
* Function Name  : 
* Description    : 
* Input          : 
* Return         : 
*******************************************************************************/

/**
* @brief Lancement communication DMA
* @param  DevAddress Target device address: The device 7 bits address value in datasheet must be shifted to the left before calling the interface
* @param  MemAddress Internal memory address
* @param  MemAddSize Size of internal memory address
* @param  pData Pointer to data buffer
* @param  Size Amount of data to be read
* @retval None
*/
void SENSOR_DMA_Start(uint16_t DevAddress, uint16_t MemAddress, uint16_t MemAddSize, uint8_t *pData, uint16_t Size){
	while(HAL_I2C_GetState(&hi2c) != HAL_I2C_STATE_READY);
	HAL_I2C_Mem_Read_DMA(&hi2c, DevAddress, MemAddress, I2C_MEMADD_SIZE_8BIT, (uint8_t*)pData, Size);
}


/**
  * @brief  Return system tick in ms
  * @retval Current HAL time base time stamp
  */
int32_t SENSOR_IO_GetTick(void) {
  return HAL_GetTick();
}


/**
* @brief De-Initialization du bus I2C2
* @param None
* @retval BSP status
*/
int32_t I2C_DeInit(void) 
{
  int32_t ret = BSP_ERROR_NONE;
 
  if (HAL_I2C_DeInit(&hi2c) != HAL_OK) 
  {
    ret = BSP_ERROR_BUS_FAILURE;
  }
  
  return ret;
}



/**
  * @brief  Write a value in a register of the device through BUS.
  * @param  DevAddr Device address on Bus.
  * @param  Reg    The target register address to write
  * @param  pData  Pointer to data buffer to write
  * @param  Length Data Length
  * @retval BSP status
  */
int32_t I2C_WriteReg(uint16_t DevAddr, uint16_t Reg, uint8_t *pData, uint16_t Length)
{
  int32_t ret = BSP_ERROR_BUS_FAILURE;
  uint32_t hal_error = HAL_OK;
  
	HAL_I2C_IsDeviceReady(&hi2c, DevAddr, 100, HAL_MAX_DELAY);
  if (HAL_I2C_Mem_Write(&hi2c, (uint8_t)DevAddr,(uint16_t)Reg, I2C_MEMADD_SIZE_8BIT,(uint8_t *)pData, Length, 1000) == HAL_OK)
  {
    ret = BSP_ERROR_NONE;
  }
  else
  {
    hal_error = HAL_I2C_GetError(&hi2c);
    if( hal_error == HAL_I2C_ERROR_AF)
    {
      return BSP_ERROR_BUS_ACKNOWLEDGE_FAILURE;
    }
    else
    {
      ret =  BSP_ERROR_PERIPH_FAILURE;
    }
  }
  return ret;
}


/**
  * @brief  Read a register of the device through BUS
  * @param  DevAddr Device address on Bus.
  * @param  Reg    The target register address to read
  * @param  pData  Pointer to data buffer to read
  * @param  Length Data Length
  * @retval BSP status
  */
int32_t  I2C_ReadReg(uint16_t DevAddr, uint16_t Reg, uint8_t *pData, uint16_t Length) 
{
  int32_t ret = BSP_ERROR_BUS_FAILURE;
  uint32_t hal_error = HAL_OK;
  HAL_I2C_IsDeviceReady(&hi2c, DevAddr, 100, HAL_MAX_DELAY);
  if (HAL_I2C_Mem_Read(&hi2c, DevAddr, (uint16_t)Reg,I2C_MEMADD_SIZE_8BIT, pData, Length, 1000) == HAL_OK)
  {
    ret = BSP_ERROR_NONE;
  }
	
  else
  {
    hal_error = HAL_I2C_GetError(&hi2c);
    if( hal_error == HAL_I2C_ERROR_AF)
    {
      return BSP_ERROR_BUS_ACKNOWLEDGE_FAILURE;
    }
    else
    {
      ret =  BSP_ERROR_PERIPH_FAILURE;
    }
  }
  return ret;
}



/**
* @brief Initialization du bus I2C2
* @param None
* @retval BSP status
*/
int32_t I2C_Init(void)
{
	int32_t ret = BSP_ERROR_NONE;
	
	hi2c.Instance = SENSOR_I2C_X;
	hi2c.Init.Timing = 0x00702991;//0x10909CEC; // mode standard voir config I2C STM32CubeMX //fast mode 0x00702991
	hi2c.Init.AddressingMode = I2C_ADDRESSINGMODE_7BIT;		
	hi2c.Init.OwnAddress1 = 0;	
	hi2c.Init.DualAddressMode = I2C_DUALADDRESS_DISABLE;
	hi2c.Init.GeneralCallMode = I2C_GENERALCALL_DISABLE;
        hi2c.Init.NoStretchMode = I2C_NOSTRETCH_DISABLE;
	
	if (HAL_I2C_Init(&hi2c) != HAL_OK) 
  {
    ret = BSP_ERROR_BUS_FAILURE;
  }
	return ret;
}

/**
* @brief I2C MSP Initialization
* @param hi2c: I2C handle pointer
* @retval None
*/
void HAL_I2C_MspInit(I2C_HandleTypeDef* hi2c)
{
  GPIO_InitTypeDef GPIO_InitStruct = {0};
  if(hi2c->Instance==I2C4)
  {
  
  __HAL_RCC_GPIOB_CLK_ENABLE();		
  // DMA1 Clock Activation
  __HAL_RCC_DMA2_CLK_ENABLE();
		
  /**I2C2 GPIO Configuration    
    PB10     ------> I2C4_SCL
    PB11     ------> I2C4_SDA 
  */
    GPIO_InitStruct.Pin = GPIO_PIN_10|GPIO_PIN_11;
    GPIO_InitStruct.Mode = GPIO_MODE_AF_OD;
    GPIO_InitStruct.Pull = GPIO_PULLUP;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
    GPIO_InitStruct.Alternate = GPIO_AF3_I2C4;
    HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

    /* Peripheral clock enable */
    __HAL_RCC_I2C4_CLK_ENABLE();
		
    		/* Configure the DMA handler for Reception process */
		i2c_hdma_rx.Instance                 = DMA2_Channel1;
		i2c_hdma_rx.Init.Request             = DMA_REQUEST_0;
		i2c_hdma_rx.Init.Direction           = DMA_PERIPH_TO_MEMORY;
		i2c_hdma_rx.Init.PeriphInc           = DMA_PINC_DISABLE;
		i2c_hdma_rx.Init.MemInc              = DMA_MINC_ENABLE;
		i2c_hdma_rx.Init.PeriphDataAlignment = DMA_PDATAALIGN_BYTE;
		i2c_hdma_rx.Init.MemDataAlignment    = DMA_MDATAALIGN_BYTE;
		i2c_hdma_rx.Init.Mode                = DMA_NORMAL;
		i2c_hdma_rx.Init.Priority            = DMA_PRIORITY_HIGH;

		HAL_DMA_Init(&i2c_hdma_rx);
			
		/* Associate the initialized DMA handle to the the I2C handle */
		__HAL_LINKDMA(hi2c, hdmarx, i2c_hdma_rx);
			
		/*##-5- Configure the NVIC for DMA #########################################*/
		/* NVIC configuration for DMA transfer complete interrupt (I2Cx_RX) */
		HAL_NVIC_SetPriority(DMA2_Channel1_IRQn, 1, 0);
		HAL_NVIC_EnableIRQ(DMA2_Channel1_IRQn);
		  /*##-6- Configure the NVIC for I2C ########################################*/   
  /* NVIC for I2Cx */
		HAL_NVIC_SetPriority(I2C4_ER_IRQn, 1, 1);
		HAL_NVIC_EnableIRQ(I2C4_ER_IRQn);
		HAL_NVIC_SetPriority(I2C4_EV_IRQn, 1, 2);
		HAL_NVIC_EnableIRQ(I2C4_EV_IRQn);

  }

}

/**
* @brief I2C MSP De-Initialization
* @param hi2c: I2C handle pointer
* @retval None
*/
void HAL_I2C_MspDeInit(I2C_HandleTypeDef* hi2c)
{
  if(hi2c->Instance==I2C4)
  {

    /* Peripheral clock disable */
    __HAL_RCC_I2C4_CLK_DISABLE();
  
    /**I2C2 GPIO Configuration    
    PB10     ------> I2C4_SCL
    PB11     ------> I2C4_SDA 
    */
    HAL_GPIO_DeInit(GPIOB, GPIO_PIN_10|GPIO_PIN_11);

		/* De-Initialize the DMA Channel associated to reception process */
		HAL_DMA_DeInit(&i2c_hdma_rx);
		
		  /* Disable the NVIC for DMA ###########################################*/
		HAL_NVIC_DisableIRQ(DMA2_Channel1_IRQn);

		/* Disable the NVIC for I2C ##########################################*/
		HAL_NVIC_DisableIRQ(I2C4_ER_IRQn);
		HAL_NVIC_DisableIRQ(I2C4_EV_IRQn);
  }

}



