#include "UART_com.h"
#include "string.h"
#include "stdio.h"
//#include "stdlib.h"
#include "RTC.h"
#include "PWR.h"

#ifdef USE_USART1_FOR_WIFI
/* Private typedef -----------------------------------------------------------*/

UART_HandleTypeDef UartHandle;

/* Defenition buffer ---------------------------------------------------------*/

uint8_t UartWifiBuff_Tx[10]={0};
uint8_t UartWifiBuff_Rx[10]={0};

/* Defenition flags ----------------------------------------------------------*/

uint8_t		   Start_Uart_Com = 0;		
__IO uint8_t UartReady = 0;
__IO uint8_t UartTxCplt = 0;
__IO uint8_t UartRxCplt = 0;

/* Private typedef enum ------------------------------------------------------*/

UART_WIFI_CMD     	Com_Wifi_Tx_State = No_Cmd;
UART_WIFI_WAIT_RX 	Com_Wifi_Rx_State = No_Wait;

/*******************************************************************************
* Function Name  : UART_Com_Wifi_Process 
* Description    : Manage the communication between STM32 and ESP32.
* Input          : No input
* Return         : No return value
*******************************************************************************/
void UART_Com_Wifi_Process (void)
{
	
	if (UartReady == UART_Ready) /* Check if there is no pending operation */
	{
		
		if ( (Start_Uart_Com == 0) && (Com_Wifi_Tx_State == No_Cmd) && (Com_Wifi_Rx_State == No_Wait) )
		{
			/* Initialize the communication */
			
			// add code to turn on wifi /* Turn on wifi chip */
			
			UART_Com_Wifi_Receve_Report(Wait_Ready); /* Prépare uart to receve the status "Ready" */
			Start_Uart_Com = 1;  /* Increment to prevent recursve entrance in this state */ 
		}
		
		if (UartTxCplt == 1) /* Check if there is an notification of end sendig command */
		{
			switch(Com_Wifi_Tx_State) /* Finde which command was sent */
			{
				
				/* Prepare receving the answer of the command */
				
				case Get_Time :
					
					UART_Com_Wifi_Receve_Report(Wait_Time);
				
				break;
				
				case Get_Date : 
					
					UART_Com_Wifi_Receve_Report(Wait_Date);
				
				break;
				
				case Init_Ftp : 
					
					UART_Com_Wifi_Receve_Report(Wait_Init_Ftp);
				
				break;
				
				case Start_Ftp :
					
					UART_Com_Wifi_Receve_Report(Wait_Start_Ftp);
					
				break;
				
				default:
				break;
			}
			
			UartTxCplt = 0; /* Update flag */
			
		}
		
		if (UartRxCplt == 1)  /* Check if there is an notification of end receiving answer */
		{
			
			switch(Com_Wifi_Rx_State) /* Finde for which answer we are wating */
			{
				
				case Wait_Ready: /* Wait for the initialization of ESP32 */
					
					if (strncmp((char*)&UartWifiBuff_Rx[0], "ESP32Ready", 10) == 0 ) /* Check if e received the expected answer */
					{
						UART_Com_Wifi_Send_Cmd (Get_Time);  /* Send new command */
					}
					else
					{
						/* Repete the precedent step */
						UART_Com_Wifi_Receve_Report(Wait_Ready); /* wait for good initialization of the ESP32 */
					
						// Add code to restart wifi chip  /* Reset wifi chip */
					}
					
				break;
					
					
				case Wait_Time: /* Wait for the current time */
					
					RTC_Set_time(&UartWifiBuff_Rx[0]);  /* Set the RTC time */
					UART_Com_Wifi_Send_Cmd (Get_Date);  /* Send a date request  */
				
				break;
				
				case Wait_Date : /* Wait for the current date */ 
					
					//ajouter une fonction pour régler le calendrier /* Set the RTC date */ 
					UART_Com_Wifi_Send_Cmd (Init_Ftp);	/* Send a request to initialize the FTP */
					
				break;
				
				case Wait_Init_Ftp : /* wait for report */
					
					if (strncmp((char*)&UartWifiBuff_Rx[0], "InitFtp_OK", 10) == 0 ) /* Check if the FTP Initialization succeeded */
					{
						UART_Com_Wifi_Send_Cmd (Start_Ftp); /* Send command to start the file transfer */
					}
					else
					{
						UART_Com_Wifi_Send_Cmd (Init_Ftp); /* If the initialization failed, repeate the step */ 
					}
					
					
				case Wait_Start_Ftp : /* wait for report */
					
					if (strncmp((char*)&UartWifiBuff_Rx[0], "StartTx_OK", 10) == 0 ) /* Check if the transfer starts */
					{
						UART_Com_Wifi_Receve_Report(Wait_End_Ftp); /* prepare the uart to be notify that the transfer is completed */
					}
					else
					{
						UART_Com_Wifi_Send_Cmd (Start_Ftp); /* Try again to start transfer */
					}
						
				break;
					
				case Wait_End_Ftp :
					
					
					if (strncmp((char*)&UartWifiBuff_Rx[0], "END_FTP_OK", 10) == 0 ) /* Check if the transfer is completed */
					{
						// ajouter la fonction qui s'execute apres le transfert.
					}
				
				break; 
				
				default:
					
				break;
			}
			
			UartRxCplt = 0; /* Update flag */
			
		}
		
	}
	
}

/*******************************************************************************
* Function Name  : UART_Com_Wifi_Send_Cmd 
* Description    : Send predefined command to the ESP32.
* Input          : (type : UART_WIFI_CMD) a specific request
* Return         : No return value
*******************************************************************************/
void UART_Com_Wifi_Send_Cmd (UART_WIFI_CMD cmd)
{
	
	switch (cmd)
	{
		case No_Cmd:
			Com_Wifi_Tx_State = No_Cmd;
		break;
		
		/* for all case it's the same, just the message change */
		
		case Get_Time: 
			
			strcpy((char*)&UartWifiBuff_Tx[0],"Time__Rqst"); /* prepare message to be sent */
			
			if(HAL_UART_Transmit_IT(&UartHandle, &UartWifiBuff_Tx[0], ComBuffSize)== HAL_OK)
			{
				/* if succeed */
				UartReady = UART_Busy; /* Update the state of data bus */
				Com_Wifi_Tx_State = Get_Time; /* save the command sent */
			}
		
		break;
		
			
			
		case Get_Date : 
			
			strcpy((char*)&UartWifiBuff_Tx[0],"Date__Rqst");
		
			if(HAL_UART_Transmit_IT(&UartHandle, &UartWifiBuff_Tx[0], ComBuffSize)== HAL_OK)
			{
				UartReady = UART_Busy;
				Com_Wifi_Tx_State = Get_Date;
			}
			
		break; 
		
			
			
		case Init_Ftp:
			
			strcpy((char*)&UartWifiBuff_Tx[0],"InitFtpRqt");
		
			if(HAL_UART_Transmit_IT(&UartHandle, &UartWifiBuff_Tx[0], ComBuffSize)== HAL_OK)
			{
				UartReady = UART_Busy;
				Com_Wifi_Tx_State = Init_Ftp;
			}
			
		break;
		
			
			
		case Start_Ftp : 
			
			strcpy((char*)&UartWifiBuff_Tx[0],"Start__Ftp");
		
			if(HAL_UART_Transmit_IT(&UartHandle, &UartWifiBuff_Tx[0], ComBuffSize)== HAL_OK)
			{
				UartReady = UART_Busy;
				Com_Wifi_Tx_State = Start_Ftp;
			}
			
		break;
		
			
			
		default :
			
		
		break;
			
	}	
}

/*******************************************************************************
* Function Name  : UART_Com_Wifi_Receve_Report 
* Description    : Called after sending a command to prepare the reception of the 
									 answer.
* Input          : (type : UART_WIFI_WAIT_RX) The answer expected to be received
* Return         : No return value
*******************************************************************************/
void UART_Com_Wifi_Receve_Report (UART_WIFI_WAIT_RX Rx_Log)
{
	
	switch(Rx_Log)
	{
		
		case No_Wait:
			Com_Wifi_Rx_State = No_Wait;
		break; 
		
		/* for all case it's the same, just the expected answer change */
		
		case Wait_Ready:
				/* Prepare uart to receive the answer of the command in interrupt mode */
			if(HAL_UART_Receive_IT(&UartHandle, &UartWifiBuff_Rx[0], ComBuffSize)== HAL_OK)
			{
				/* if succeed */
				UartReady = UART_Busy;  /* Update state of data bus */
				Com_Wifi_Rx_State = Wait_Ready; /* Update the answer expected */
			}
		
		break;
		
			
			
		case Wait_Time :
			
			if(HAL_UART_Receive_IT(&UartHandle, &UartWifiBuff_Rx[0], ComBuffSize)== HAL_OK)
			{
				UartReady = UART_Busy;
				Com_Wifi_Rx_State = Wait_Time;
			}
			
		break;
		
			
			
		case Wait_Date : 
			
			if(HAL_UART_Receive_IT(&UartHandle, &UartWifiBuff_Rx[0], ComBuffSize)== HAL_OK)
			{
				UartReady = UART_Busy;
				Com_Wifi_Rx_State = Wait_Date;
			}
			
		break; 
		
			
			
		case Wait_Init_Ftp:
			
			if(HAL_UART_Receive_IT(&UartHandle, &UartWifiBuff_Rx[0], ComBuffSize)== HAL_OK)
			{
				UartReady = UART_Busy;
				Com_Wifi_Rx_State = Wait_Init_Ftp;
			}
		
		break;
		
			
			
		case Wait_Start_Ftp: 
			
			if(HAL_UART_Receive_IT(&UartHandle, &UartWifiBuff_Rx[0], ComBuffSize)== HAL_OK)
			{
				UartReady = UART_Busy;
				Com_Wifi_Rx_State = Wait_Start_Ftp;
			}
		
		break;
		
			
			
		case Wait_End_Ftp :
			
			if(HAL_UART_Receive_IT(&UartHandle, &UartWifiBuff_Rx[0], ComBuffSize)== HAL_OK)
			{
				UartReady = UART_Busy;
				Com_Wifi_Rx_State = Wait_End_Ftp;
			}
		
		break;
			
			
			
		default :
			
		break;	
	}
	
}




















/*******************************************************************************
* Function Name  : UART_Init 
* Description    : This function is called to initiaze the data bus.
* Input          : No input.
* Return         : No return value.
*******************************************************************************/
void UART_Init(void)
{
	/* Instance to be initialize*/
  UartHandle.Instance            = USARTx;
	/* Parameters of working */
  UartHandle.Init.BaudRate       = 115200;
  UartHandle.Init.WordLength     = UART_WORDLENGTH_8B;
  UartHandle.Init.StopBits       = UART_STOPBITS_1;
  UartHandle.Init.Parity         = UART_PARITY_NONE;
  UartHandle.Init.HwFlowCtl      = UART_HWCONTROL_NONE;
  UartHandle.Init.Mode           = UART_MODE_TX_RX;
  UartHandle.Init.OverSampling   = UART_OVERSAMPLING_16;
  UartHandle.AdvancedInit.AdvFeatureInit = UART_ADVFEATURE_NO_INIT;

	/* Reste the peripheral */
  if(HAL_UART_DeInit(&UartHandle) != HAL_OK)
  {
		/* Error management */
    PWR_Reset_System();
  }  
	/* init the peripheral */ 
  if(HAL_UART_Init(&UartHandle) != HAL_OK)
  {
		/* Error management */
    PWR_Reset_System();
  }
	
}

/*******************************************************************************
* Function Name  : HAL_UART_TxCpltCallback
* Description    : Redefined callback to notify that Tx operation is completed.
* Input          : Uart Handel Structure.
* Return         : No return value.
*******************************************************************************/
void HAL_UART_TxCpltCallback(UART_HandleTypeDef *UartHandle)
{
  UartReady = UART_Ready;  /* Update the status of the bus */
	UartTxCplt = 1;  /* Set transmission flag: transfer complete */ 
}

/*******************************************************************************
* Function Name  : HAL_UART_RxCpltCallback
* Description    : Redefined callback to notify that Rx operation is completed.
* Input          : Uart Handel Structure.
* Return         : No return value.
*******************************************************************************/
void HAL_UART_RxCpltCallback(UART_HandleTypeDef *UartHandle)
{
  UartReady = UART_Ready;  /* Update the status of the bus */
	UartRxCplt = 1;	 /* Set reception flag: transfer complete */
}

/*******************************************************************************
* Function Name  : HAL_UART_RxCpltCallback
* Description    : Redefined callback to notify that an error occured.
* Input          : Uart Handel Structure.
* Return         : No return value.
*******************************************************************************/
void HAL_UART_ErrorCallback(UART_HandleTypeDef *UartHandle)
{
  /* Function for re send the command must be developpend and called from hear */
}

/*******************************************************************************
* Function Name  : HAL_UART_MspInit
* Description    : Redefined to initialize low level parameters.
* Input          : Uart Handel Structure.
* Return         : No return value.
*******************************************************************************/
void HAL_UART_MspInit(UART_HandleTypeDef *huart)
{
  GPIO_InitTypeDef  GPIO_InitStruct;
  
  /*##-1- Enable peripherals and GPIO Clocks #################################*/
  /* Enable GPIO TX/RX clock */
  USARTx_TX_GPIO_CLK_ENABLE();
  USARTx_RX_GPIO_CLK_ENABLE();


  /* Enable USARTx clock */
  USARTx_CLK_ENABLE();
  
  /*##-2- Configure peripheral GPIO ##########################################*/
  /* UART TX GPIO pin configuration  */
  GPIO_InitStruct.Pin       = USARTx_TX_PIN;
  GPIO_InitStruct.Mode      = GPIO_MODE_AF_PP;
  GPIO_InitStruct.Pull      = GPIO_PULLUP;
  GPIO_InitStruct.Speed     = GPIO_SPEED_FREQ_HIGH;
  GPIO_InitStruct.Alternate = USARTx_TX_AF;

  HAL_GPIO_Init(USARTx_TX_GPIO_PORT, &GPIO_InitStruct);

  /* UART RX GPIO pin configuration  */
  GPIO_InitStruct.Pin = USARTx_RX_PIN;
  GPIO_InitStruct.Alternate = USARTx_RX_AF;

  HAL_GPIO_Init(USARTx_RX_GPIO_PORT, &GPIO_InitStruct);

  /*##-3- Configure the NVIC for UART ########################################*/
  /* NVIC for USART1 */
  HAL_NVIC_SetPriority(USARTx_IRQn, 0, 1);
  HAL_NVIC_EnableIRQ(USARTx_IRQn);
}

/*******************************************************************************
* Function Name  : HAL_UART_MspInit
* Description    : Redefined to Deinitialize low level parameters.
* Input          : Uart Handel Structure.
* Return         : No return value.
*******************************************************************************/

void HAL_UART_MspDeInit(UART_HandleTypeDef *huart)
{
  /*##-1- Reset peripherals ##################################################*/
  USARTx_FORCE_RESET();
  USARTx_RELEASE_RESET();

  /*##-2- Disable peripherals and GPIO Clocks #################################*/
  /* De-Initialize USART1 Tx */
  HAL_GPIO_DeInit(USARTx_TX_GPIO_PORT, USARTx_TX_PIN);
  /* De-Initialize USART1 Rx */
  HAL_GPIO_DeInit(USARTx_RX_GPIO_PORT, USARTx_RX_PIN);
  
  /*##-3- Disable the NVIC for UART ###########################################*/
  HAL_NVIC_DisableIRQ(USARTx_IRQn);
}


#endif




