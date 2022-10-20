#include "DFSDM.h"
#include "PWR.h"

/* Private functions ---------------------------------------------------------*/

/* Private variables ---------------------------------------------------------*/

DFSDM_Channel_HandleTypeDef  DfsdmChannel0Handle;
DFSDM_Channel_HandleTypeDef  DfsdmChannel1Handle;
DFSDM_Channel_HandleTypeDef  DfsdmChannel2Handle;
DFSDM_Channel_HandleTypeDef  DfsdmChannel3Handle;


DFSDM_Filter_HandleTypeDef   DfsdmFilter0Handle;
DFSDM_Filter_HandleTypeDef   DfsdmFilter1Handle;
DFSDM_Filter_HandleTypeDef   DfsdmFilter2Handle;
DFSDM_Filter_HandleTypeDef   DfsdmFilter3Handle;

DMA_HandleTypeDef            hDfsdmFLT0Dma;
DMA_HandleTypeDef            hDfsdmFLT1Dma;
DMA_HandleTypeDef            hDfsdmFLT2Dma;
DMA_HandleTypeDef            hDfsdmFLT3Dma;


extern int32_t                      RecBuff0[2*BuffSize];
extern int32_t                      RecBuff1[2*BuffSize];
extern int32_t                      RecBuff2[2*BuffSize];
extern int32_t                      RecBuff3[2*BuffSize];



/*******************************************************************************
* Function Name  : 
* Description    : 
* Input          : 
* Return         : 
*******************************************************************************/
void DFSDM_Start_Record (void)
{
	if(HAL_OK != HAL_DFSDM_FilterRegularStart_DMA(&DfsdmFilter0Handle, &RecBuff0[0], 2*BuffSize))
	{
		PWR_Reset_System();
	}
	if(HAL_OK != HAL_DFSDM_FilterRegularStart_DMA(&DfsdmFilter1Handle, &RecBuff1[0], 2*BuffSize))
	{
		PWR_Reset_System();
	}
		if(HAL_OK != HAL_DFSDM_FilterRegularStart_DMA(&DfsdmFilter2Handle, &RecBuff2[0], 2*BuffSize))
	{
		PWR_Reset_System();
	}
	if(HAL_OK != HAL_DFSDM_FilterRegularStart_DMA(&DfsdmFilter3Handle, &RecBuff3[0], 2*BuffSize))
	{
		PWR_Reset_System();
	}
}
/*******************************************************************************
* Function Name  : 
* Description    : 
* Input          : 
* Return         : 
*******************************************************************************/
void DFSDM_Stop_Record (void)
{
	if(HAL_OK != HAL_DFSDM_FilterRegularStop_DMA(&DfsdmFilter0Handle))
	{
		
	}
	if(HAL_OK != HAL_DFSDM_FilterRegularStop_DMA(&DfsdmFilter1Handle))
	{
		
	}
	if(HAL_OK != HAL_DFSDM_FilterRegularStop_DMA(&DfsdmFilter2Handle))
	{
		
	}
	if(HAL_OK != HAL_DFSDM_FilterRegularStop_DMA(&DfsdmFilter3Handle))
	{
		
	}
}	
	
__weak void DmaAudioRecHalfCallback (uint8_t Flt)
{
	
}

__weak void DmaAudioRecCpltCallback (uint8_t Flt)
{
	
}
/*******************************************************************************
* Function Name  : 
* Description    : 
* Input          : 
* Return         : 
*******************************************************************************/

void HAL_DFSDM_FilterRegConvHalfCpltCallback(DFSDM_Filter_HandleTypeDef *hdfsdm_filter)
{	
	if ( hdfsdm_filter == &DfsdmFilter0Handle )
	{
		DmaAudioRecHalfCallback (0);
	}
	if ( hdfsdm_filter == &DfsdmFilter1Handle )
	{
		DmaAudioRecHalfCallback (1);
	}
	if ( hdfsdm_filter == &DfsdmFilter2Handle )
	{
		DmaAudioRecHalfCallback (2);
	}	
		if ( hdfsdm_filter == &DfsdmFilter3Handle )
	{
		DmaAudioRecHalfCallback (3);
	}
}

void HAL_DFSDM_FilterRegConvCpltCallback(DFSDM_Filter_HandleTypeDef *hdfsdm_filter)
{	
	if ( hdfsdm_filter == &DfsdmFilter0Handle )
	{
		DmaAudioRecCpltCallback (0);
	}
	if ( hdfsdm_filter == &DfsdmFilter1Handle )
	{
		DmaAudioRecCpltCallback (1);
	}
	if ( hdfsdm_filter == &DfsdmFilter2Handle )
	{
		DmaAudioRecCpltCallback (2);
	}
		if ( hdfsdm_filter == &DfsdmFilter3Handle )
	{
		DmaAudioRecCpltCallback (3);
	}
}

/*

Fonctions a venir
			
*/
/*******************************************************************************
* Function Name  : 
* Description    : 
* Input          : 
* Return         : 
*******************************************************************************/
void DFSDM_Init(void)
{

	
	  /* Initialize channel 0 */
	
  __HAL_DFSDM_CHANNEL_RESET_HANDLE_STATE(&DfsdmChannel0Handle);
  DfsdmChannel0Handle.Instance                      = DFSDM1_Channel0;
  DfsdmChannel0Handle.Init.OutputClock.Activation   = ENABLE;
  DfsdmChannel0Handle.Init.OutputClock.Selection    = DFSDM_CHANNEL_OUTPUT_CLOCK_AUDIO;
  DfsdmChannel0Handle.Init.OutputClock.Divider      = DFSDM_User_Clock_Divider; /* 11.294MHz/4 = 2.82MHz */
  DfsdmChannel0Handle.Init.Input.Multiplexer        = DFSDM_CHANNEL_EXTERNAL_INPUTS;
  DfsdmChannel0Handle.Init.Input.DataPacking        = DFSDM_CHANNEL_STANDARD_MODE; /* N.U. */
  DfsdmChannel0Handle.Init.Input.Pins               = DFSDM_CHANNEL_FOLLOWING_CHANNEL_PINS;
  DfsdmChannel0Handle.Init.SerialInterface.Type     = DFSDM_CHANNEL_SPI_FALLING;
  DfsdmChannel0Handle.Init.SerialInterface.SpiClock = DFSDM_CHANNEL_SPI_CLOCK_INTERNAL;
  DfsdmChannel0Handle.Init.Awd.FilterOrder          = DFSDM_CHANNEL_FASTSINC_ORDER; /* N.U. */
  DfsdmChannel0Handle.Init.Awd.Oversampling         = 10; /* N.U. */
  DfsdmChannel0Handle.Init.Offset                   = 0;
  DfsdmChannel0Handle.Init.RightBitShift            = DFSDM_User_RightBitShift;
  if(HAL_OK != HAL_DFSDM_ChannelInit(&DfsdmChannel0Handle))
  {
    //Error_Handler();
		PWR_Reset_System();
  }
	
  /* Initialize channel 1 */
	
  __HAL_DFSDM_CHANNEL_RESET_HANDLE_STATE(&DfsdmChannel1Handle);
  DfsdmChannel1Handle.Instance                      = DFSDM1_Channel1;
  DfsdmChannel1Handle.Init.OutputClock.Activation   = ENABLE;
  DfsdmChannel1Handle.Init.OutputClock.Selection    = DFSDM_CHANNEL_OUTPUT_CLOCK_AUDIO;
  DfsdmChannel1Handle.Init.OutputClock.Divider      = DFSDM_User_Clock_Divider; /* 11.294MHz/4 = 2.82MHz */
  DfsdmChannel1Handle.Init.Input.Multiplexer        = DFSDM_CHANNEL_EXTERNAL_INPUTS;
  DfsdmChannel1Handle.Init.Input.DataPacking        = DFSDM_CHANNEL_STANDARD_MODE; /* N.U. */
  DfsdmChannel1Handle.Init.Input.Pins               = DFSDM_CHANNEL_SAME_CHANNEL_PINS;
  DfsdmChannel1Handle.Init.SerialInterface.Type     = DFSDM_CHANNEL_SPI_RISING;
  DfsdmChannel1Handle.Init.SerialInterface.SpiClock = DFSDM_CHANNEL_SPI_CLOCK_INTERNAL;
  DfsdmChannel1Handle.Init.Awd.FilterOrder          = DFSDM_CHANNEL_FASTSINC_ORDER; /* N.U. */
  DfsdmChannel1Handle.Init.Awd.Oversampling         = 10; /* N.U. */
  DfsdmChannel1Handle.Init.Offset                   = 0;
  DfsdmChannel1Handle.Init.RightBitShift            = DFSDM_User_RightBitShift;
  if(HAL_OK != HAL_DFSDM_ChannelInit(&DfsdmChannel1Handle))
  {
    //Error_Handler();
		PWR_Reset_System();
  }
	
		  /* Initialize channel 2 */
	
  __HAL_DFSDM_CHANNEL_RESET_HANDLE_STATE(&DfsdmChannel2Handle);
  DfsdmChannel2Handle.Instance                      = DFSDM1_Channel2;
  DfsdmChannel2Handle.Init.OutputClock.Activation   = ENABLE;
  DfsdmChannel2Handle.Init.OutputClock.Selection    = DFSDM_CHANNEL_OUTPUT_CLOCK_AUDIO;
  DfsdmChannel2Handle.Init.OutputClock.Divider      = DFSDM_User_Clock_Divider; /* 11.294MHz/4 = 2.82MHz */
  DfsdmChannel2Handle.Init.Input.Multiplexer        = DFSDM_CHANNEL_EXTERNAL_INPUTS;
  DfsdmChannel2Handle.Init.Input.DataPacking        = DFSDM_CHANNEL_STANDARD_MODE; /* N.U. */
  DfsdmChannel2Handle.Init.Input.Pins               = DFSDM_CHANNEL_FOLLOWING_CHANNEL_PINS;
  DfsdmChannel2Handle.Init.SerialInterface.Type     = DFSDM_CHANNEL_SPI_FALLING;
  DfsdmChannel2Handle.Init.SerialInterface.SpiClock = DFSDM_CHANNEL_SPI_CLOCK_INTERNAL;
  DfsdmChannel2Handle.Init.Awd.FilterOrder          = DFSDM_CHANNEL_FASTSINC_ORDER; /* N.U. */
  DfsdmChannel2Handle.Init.Awd.Oversampling         = 10; /* N.U. */
  DfsdmChannel2Handle.Init.Offset                   = 0;
  DfsdmChannel2Handle.Init.RightBitShift            = DFSDM_User_RightBitShift;
  if(HAL_OK != HAL_DFSDM_ChannelInit(&DfsdmChannel2Handle))
  {
    //Error_Handler();
		PWR_Reset_System();
  }
	
  /* Initialize channel 3 */
	
  __HAL_DFSDM_CHANNEL_RESET_HANDLE_STATE(&DfsdmChannel3Handle);
  DfsdmChannel3Handle.Instance                      = DFSDM1_Channel3;
  DfsdmChannel3Handle.Init.OutputClock.Activation   = ENABLE;
  DfsdmChannel3Handle.Init.OutputClock.Selection    = DFSDM_CHANNEL_OUTPUT_CLOCK_AUDIO;
  DfsdmChannel3Handle.Init.OutputClock.Divider      = DFSDM_User_Clock_Divider; /* 11.294MHz/4 = 2.82MHz */
  DfsdmChannel3Handle.Init.Input.Multiplexer        = DFSDM_CHANNEL_EXTERNAL_INPUTS;
  DfsdmChannel3Handle.Init.Input.DataPacking        = DFSDM_CHANNEL_STANDARD_MODE; /* N.U. */
  DfsdmChannel3Handle.Init.Input.Pins               = DFSDM_CHANNEL_SAME_CHANNEL_PINS;
  DfsdmChannel3Handle.Init.SerialInterface.Type     = DFSDM_CHANNEL_SPI_RISING;
  DfsdmChannel3Handle.Init.SerialInterface.SpiClock = DFSDM_CHANNEL_SPI_CLOCK_INTERNAL;
  DfsdmChannel3Handle.Init.Awd.FilterOrder          = DFSDM_CHANNEL_FASTSINC_ORDER; /* N.U. */
  DfsdmChannel3Handle.Init.Awd.Oversampling         = 10; /* N.U. */
  DfsdmChannel3Handle.Init.Offset                   = 0;
  DfsdmChannel3Handle.Init.RightBitShift            = DFSDM_User_RightBitShift;
  if(HAL_OK != HAL_DFSDM_ChannelInit(&DfsdmChannel3Handle))
  {
    //Error_Handler();
		PWR_Reset_System();
  }
	
		
	
  /* Initialize filter 0 */
  __HAL_DFSDM_FILTER_RESET_HANDLE_STATE(&DfsdmFilter0Handle);
  DfsdmFilter0Handle.Instance                          = DFSDM1_Filter0;
  DfsdmFilter0Handle.Init.RegularParam.Trigger         = DFSDM_FILTER_SW_TRIGGER;
  DfsdmFilter0Handle.Init.RegularParam.FastMode        = ENABLE;
  DfsdmFilter0Handle.Init.RegularParam.DmaMode         = ENABLE;
  DfsdmFilter0Handle.Init.InjectedParam.Trigger        = DFSDM_FILTER_SW_TRIGGER; /* N.U. */
  DfsdmFilter0Handle.Init.InjectedParam.ScanMode       = ENABLE; /* N.U. */
  DfsdmFilter0Handle.Init.InjectedParam.DmaMode        = DISABLE; /* N.U. */
  DfsdmFilter0Handle.Init.InjectedParam.ExtTrigger     = DFSDM_FILTER_EXT_TRIG_TIM1_TRGO; /* N.U. */
  DfsdmFilter0Handle.Init.InjectedParam.ExtTriggerEdge = DFSDM_FILTER_EXT_TRIG_BOTH_EDGES; /* N.U. */
  DfsdmFilter0Handle.Init.FilterParam.SincOrder        = DFSDM_User_Filter_Order;
  DfsdmFilter0Handle.Init.FilterParam.Oversampling     = DFSDM_User_Oversampling; /* 11.294MHz/(4*64) = 44.1KHz */
  DfsdmFilter0Handle.Init.FilterParam.IntOversampling  = 1;
  if(HAL_OK != HAL_DFSDM_FilterInit(&DfsdmFilter0Handle))
  {
   // Error_Handler();
		PWR_Reset_System();
  }

  /* Configure regular channel and continuous mode for filter 0 */
  if(HAL_OK != HAL_DFSDM_FilterConfigRegChannel(&DfsdmFilter0Handle, DFSDM_CHANNEL_0, DFSDM_CONTINUOUS_CONV_ON))
  {
    //Error_Handler();
		PWR_Reset_System();
  }
	
	  /* Initialize filter 1 */
  __HAL_DFSDM_FILTER_RESET_HANDLE_STATE(&DfsdmFilter1Handle);
  DfsdmFilter1Handle.Instance                          = DFSDM1_Filter1;
  DfsdmFilter1Handle.Init.RegularParam.Trigger         = DFSDM_FILTER_SW_TRIGGER;
  DfsdmFilter1Handle.Init.RegularParam.FastMode        = ENABLE;
  DfsdmFilter1Handle.Init.RegularParam.DmaMode         = ENABLE;
  DfsdmFilter1Handle.Init.InjectedParam.Trigger        = DFSDM_FILTER_SW_TRIGGER; /* N.U. */
  DfsdmFilter1Handle.Init.InjectedParam.ScanMode       = ENABLE; /* N.U. */
  DfsdmFilter1Handle.Init.InjectedParam.DmaMode        = DISABLE; /* N.U. */
  DfsdmFilter1Handle.Init.InjectedParam.ExtTrigger     = DFSDM_FILTER_EXT_TRIG_TIM1_TRGO; /* N.U. */
  DfsdmFilter1Handle.Init.InjectedParam.ExtTriggerEdge = DFSDM_FILTER_EXT_TRIG_BOTH_EDGES; /* N.U. */
  DfsdmFilter1Handle.Init.FilterParam.SincOrder        = DFSDM_User_Filter_Order;
  DfsdmFilter1Handle.Init.FilterParam.Oversampling     = DFSDM_User_Oversampling; /* 11.294MHz/(4*64) = 44.1KHz */
  DfsdmFilter1Handle.Init.FilterParam.IntOversampling  = 1;
  if(HAL_OK != HAL_DFSDM_FilterInit(&DfsdmFilter1Handle))
  {
   // Error_Handler();
		PWR_Reset_System();
  }

  /* Configure regular channel and continuous mode for filter 0 */
  if(HAL_OK != HAL_DFSDM_FilterConfigRegChannel(&DfsdmFilter1Handle, DFSDM_CHANNEL_1, DFSDM_CONTINUOUS_CONV_ON))
  {
    //Error_Handler();
		PWR_Reset_System();
  }
	
	
	
  /* Initialize filter 2 */
  __HAL_DFSDM_FILTER_RESET_HANDLE_STATE(&DfsdmFilter2Handle);
  DfsdmFilter2Handle.Instance                          = DFSDM1_Filter2;
  DfsdmFilter2Handle.Init.RegularParam.Trigger         = DFSDM_FILTER_SW_TRIGGER;
  DfsdmFilter2Handle.Init.RegularParam.FastMode        = ENABLE;
  DfsdmFilter2Handle.Init.RegularParam.DmaMode         = ENABLE;
  DfsdmFilter2Handle.Init.InjectedParam.Trigger        = DFSDM_FILTER_SW_TRIGGER; /* N.U. */
  DfsdmFilter2Handle.Init.InjectedParam.ScanMode       = ENABLE; /* N.U. */
  DfsdmFilter2Handle.Init.InjectedParam.DmaMode        = DISABLE; /* N.U. */
  DfsdmFilter2Handle.Init.InjectedParam.ExtTrigger     = DFSDM_FILTER_EXT_TRIG_TIM1_TRGO; /* N.U. */
  DfsdmFilter2Handle.Init.InjectedParam.ExtTriggerEdge = DFSDM_FILTER_EXT_TRIG_BOTH_EDGES; /* N.U. */
  DfsdmFilter2Handle.Init.FilterParam.SincOrder        = DFSDM_User_Filter_Order;
  DfsdmFilter2Handle.Init.FilterParam.Oversampling     = DFSDM_User_Oversampling; /* 11.294MHz/(4*64) = 44.1KHz */
  DfsdmFilter2Handle.Init.FilterParam.IntOversampling  = 1;
  if(HAL_OK != HAL_DFSDM_FilterInit(&DfsdmFilter2Handle))
  {
   // Error_Handler();
		PWR_Reset_System();
  }

  /* Configure regular channel and continuous mode for filter 0 */
  if(HAL_OK != HAL_DFSDM_FilterConfigRegChannel(&DfsdmFilter2Handle, DFSDM_CHANNEL_2, DFSDM_CONTINUOUS_CONV_ON))
  {
    //Error_Handler();
		PWR_Reset_System();
  }
	
	  /* Initialize filter 3 */
  __HAL_DFSDM_FILTER_RESET_HANDLE_STATE(&DfsdmFilter3Handle);
  DfsdmFilter3Handle.Instance                          = DFSDM1_Filter3;
  DfsdmFilter3Handle.Init.RegularParam.Trigger         = DFSDM_FILTER_SW_TRIGGER;
  DfsdmFilter3Handle.Init.RegularParam.FastMode        = ENABLE;
  DfsdmFilter3Handle.Init.RegularParam.DmaMode         = ENABLE;
  DfsdmFilter3Handle.Init.InjectedParam.Trigger        = DFSDM_FILTER_SW_TRIGGER; /* N.U. */
  DfsdmFilter3Handle.Init.InjectedParam.ScanMode       = ENABLE; /* N.U. */
  DfsdmFilter3Handle.Init.InjectedParam.DmaMode        = DISABLE; /* N.U. */
  DfsdmFilter3Handle.Init.InjectedParam.ExtTrigger     = DFSDM_FILTER_EXT_TRIG_TIM1_TRGO; /* N.U. */
  DfsdmFilter3Handle.Init.InjectedParam.ExtTriggerEdge = DFSDM_FILTER_EXT_TRIG_BOTH_EDGES; /* N.U. */
  DfsdmFilter3Handle.Init.FilterParam.SincOrder        = DFSDM_User_Filter_Order;
  DfsdmFilter3Handle.Init.FilterParam.Oversampling     = DFSDM_User_Oversampling; /* 11.294MHz/(4*64) = 44.1KHz */
  DfsdmFilter3Handle.Init.FilterParam.IntOversampling  = 1;
  if(HAL_OK != HAL_DFSDM_FilterInit(&DfsdmFilter3Handle))
  {
   // Error_Handler();
		PWR_Reset_System();
  }

  /* Configure regular channel and continuous mode for filter 0 */
  if(HAL_OK != HAL_DFSDM_FilterConfigRegChannel(&DfsdmFilter3Handle, DFSDM_CHANNEL_3, DFSDM_CONTINUOUS_CONV_ON))
  {
    //Error_Handler();
		PWR_Reset_System();
  }
}


void HAL_DFSDM_ChannelMspInit(DFSDM_Channel_HandleTypeDef *hdfsdm_channel)
{
  /* Init of clock, gpio and PLLSAI1 clock */
  GPIO_InitTypeDef GPIO_Init;
  RCC_PeriphCLKInitTypeDef RCC_PeriphCLKInitStruct;
  
  /* Enable DFSDM clock */
  __HAL_RCC_DFSDM1_CLK_ENABLE();
  __HAL_RCC_GPIOC_CLK_ENABLE();
	__HAL_RCC_GPIOB_CLK_ENABLE();
	
  /* Configure PC2 for DFSDM_CKOUT and PB12 for DFSDM_DATIN1 and PC7 for dataIN 3 */
  __HAL_RCC_GPIOE_CLK_ENABLE();
  GPIO_Init.Mode      = GPIO_MODE_AF_PP;
  GPIO_Init.Pull      = GPIO_NOPULL;
  GPIO_Init.Speed     = GPIO_SPEED_FREQ_VERY_HIGH;
  GPIO_Init.Alternate = GPIO_AF6_DFSDM1;
  GPIO_Init.Pin = GPIO_PIN_2;
  HAL_GPIO_Init(GPIOC, &GPIO_Init);
  GPIO_Init.Pin = GPIO_PIN_12;
  HAL_GPIO_Init(GPIOB, &GPIO_Init);
	 GPIO_Init.Pin = GPIO_PIN_7;
  HAL_GPIO_Init(GPIOC, &GPIO_Init);
  
  /* Configure and enable PLLSAI1 clock to generate 11.294MHz */
  RCC_PeriphCLKInitStruct.PeriphClockSelection    = RCC_PERIPHCLK_SAI1;
  RCC_PeriphCLKInitStruct.PLLSAI2.PLLSAI2Source   = RCC_PLLSOURCE_MSI;
  RCC_PeriphCLKInitStruct.PLLSAI2.PLLSAI2M        = 1;
	// for 11k 22k 44k 
	//RCC_PeriphCLKInitStruct.PLLSAI2.PLLSAI2N        = 48;
  //RCC_PeriphCLKInitStruct.PLLSAI2.PLLSAI2P        = 17;
	// for 8k 16k 48k 96k
	RCC_PeriphCLKInitStruct.PLLSAI2.PLLSAI2N        = 86;
	RCC_PeriphCLKInitStruct.PLLSAI2.PLLSAI2P        = 7;
  
	RCC_PeriphCLKInitStruct.PLLSAI2.PLLSAI2ClockOut = RCC_PLLSAI2_SAI2CLK;
  RCC_PeriphCLKInitStruct.Sai1ClockSelection      = RCC_SAI1CLKSOURCE_PLLSAI2;
  if(HAL_RCCEx_PeriphCLKConfig(&RCC_PeriphCLKInitStruct) != HAL_OK)
  {
    //Error_Handler();
		PWR_Reset_System();
  }
}


void HAL_DFSDM_FilterMspInit(DFSDM_Filter_HandleTypeDef *hdfsdm_filter)
{
  /* Configure DMA1_Channel4 for filter 0 */
  __HAL_RCC_DMA1_CLK_ENABLE();
	
  hDfsdmFLT0Dma.Init.Request             = DMA_REQUEST_0;
  hDfsdmFLT0Dma.Init.Direction           = DMA_PERIPH_TO_MEMORY;
  hDfsdmFLT0Dma.Init.PeriphInc           = DMA_PINC_DISABLE;
  hDfsdmFLT0Dma.Init.MemInc              = DMA_MINC_ENABLE;
  hDfsdmFLT0Dma.Init.PeriphDataAlignment = DMA_PDATAALIGN_WORD;
  hDfsdmFLT0Dma.Init.MemDataAlignment    = DMA_MDATAALIGN_WORD;
  hDfsdmFLT0Dma.Init.Mode                = DMA_CIRCULAR;
  hDfsdmFLT0Dma.Init.Priority            = DMA_PRIORITY_VERY_HIGH;
  hDfsdmFLT0Dma.Instance                 = DMA1_Channel4;
  __HAL_LINKDMA(&DfsdmFilter0Handle, hdmaReg, hDfsdmFLT0Dma);
  if (HAL_OK != HAL_DMA_Init(&hDfsdmFLT0Dma))
  {
   // Error_Handler();
		PWR_Reset_System();
  }
  HAL_NVIC_SetPriority(DMA1_Channel4_IRQn, 0x03, 0);
  HAL_NVIC_EnableIRQ(DMA1_Channel4_IRQn);
	
	  /* Configure DMA1_Channel5 for filter 1 */
  __HAL_RCC_DMA1_CLK_ENABLE();
  hDfsdmFLT1Dma.Init.Request             = DMA_REQUEST_0;
  hDfsdmFLT1Dma.Init.Direction           = DMA_PERIPH_TO_MEMORY;
  hDfsdmFLT1Dma.Init.PeriphInc           = DMA_PINC_DISABLE;
  hDfsdmFLT1Dma.Init.MemInc              = DMA_MINC_ENABLE;
  hDfsdmFLT1Dma.Init.PeriphDataAlignment = DMA_PDATAALIGN_WORD;
  hDfsdmFLT1Dma.Init.MemDataAlignment    = DMA_MDATAALIGN_WORD;
  hDfsdmFLT1Dma.Init.Mode                = DMA_CIRCULAR;
  hDfsdmFLT1Dma.Init.Priority            = DMA_PRIORITY_VERY_HIGH;
  hDfsdmFLT1Dma.Instance                 = DMA1_Channel5;
  __HAL_LINKDMA(&DfsdmFilter1Handle, hdmaReg, hDfsdmFLT1Dma);
  if (HAL_OK != HAL_DMA_Init(&hDfsdmFLT1Dma))
  {
   // Error_Handler();
		PWR_Reset_System();
  }
	// No loss of data on stereo mode with differente priority 
  HAL_NVIC_SetPriority(DMA1_Channel5_IRQn, 0x03, 0);
  HAL_NVIC_EnableIRQ(DMA1_Channel5_IRQn);
	
	  /* Configure DMA1_Channel6 for filter 2 */
  __HAL_RCC_DMA1_CLK_ENABLE();
	
  hDfsdmFLT2Dma.Init.Request             = DMA_REQUEST_0;
  hDfsdmFLT2Dma.Init.Direction           = DMA_PERIPH_TO_MEMORY;
  hDfsdmFLT2Dma.Init.PeriphInc           = DMA_PINC_DISABLE;
  hDfsdmFLT2Dma.Init.MemInc              = DMA_MINC_ENABLE;
  hDfsdmFLT2Dma.Init.PeriphDataAlignment = DMA_PDATAALIGN_WORD;
  hDfsdmFLT2Dma.Init.MemDataAlignment    = DMA_MDATAALIGN_WORD;
  hDfsdmFLT2Dma.Init.Mode                = DMA_CIRCULAR;
  hDfsdmFLT2Dma.Init.Priority            = DMA_PRIORITY_VERY_HIGH;
  hDfsdmFLT2Dma.Instance                 = DMA1_Channel6;
  __HAL_LINKDMA(&DfsdmFilter2Handle, hdmaReg, hDfsdmFLT2Dma);
  if (HAL_OK != HAL_DMA_Init(&hDfsdmFLT2Dma))
  {
   // Error_Handler();
		PWR_Reset_System();
  }
  HAL_NVIC_SetPriority(DMA1_Channel6_IRQn, 0x03, 0);
  HAL_NVIC_EnableIRQ(DMA1_Channel6_IRQn);
	
	  /* Configure DMA1_Channel7 for filter 3 */
  __HAL_RCC_DMA1_CLK_ENABLE();
  hDfsdmFLT3Dma.Init.Request             = DMA_REQUEST_0;
  hDfsdmFLT3Dma.Init.Direction           = DMA_PERIPH_TO_MEMORY;
  hDfsdmFLT3Dma.Init.PeriphInc           = DMA_PINC_DISABLE;
  hDfsdmFLT3Dma.Init.MemInc              = DMA_MINC_ENABLE;
  hDfsdmFLT3Dma.Init.PeriphDataAlignment = DMA_PDATAALIGN_WORD;
  hDfsdmFLT3Dma.Init.MemDataAlignment    = DMA_MDATAALIGN_WORD;
  hDfsdmFLT3Dma.Init.Mode                = DMA_CIRCULAR;
  hDfsdmFLT3Dma.Init.Priority            = DMA_PRIORITY_VERY_HIGH;
  hDfsdmFLT3Dma.Instance                 = DMA1_Channel7;
  __HAL_LINKDMA(&DfsdmFilter3Handle, hdmaReg, hDfsdmFLT3Dma);
  if (HAL_OK != HAL_DMA_Init(&hDfsdmFLT3Dma))
  {
   // Error_Handler();
		PWR_Reset_System();
  }
	// No loss of data on stereo mode with differente priority 
  HAL_NVIC_SetPriority(DMA1_Channel7_IRQn, 0x03, 0);
  HAL_NVIC_EnableIRQ(DMA1_Channel7_IRQn);
}




