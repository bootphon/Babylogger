#include "stm32l4xx_hal.h"


//#define USE_USART1_FOR_WIFI

#ifdef USE_USART1_FOR_WIFI
/* Definition for USART 1 -------------------------------------------------------------------- */

#define USARTx                           USART1
#define USARTx_CLK_ENABLE()              __HAL_RCC_USART1_CLK_ENABLE()
#define USARTx_RX_GPIO_CLK_ENABLE()      __HAL_RCC_GPIOB_CLK_ENABLE()
#define USARTx_TX_GPIO_CLK_ENABLE()      __HAL_RCC_GPIOB_CLK_ENABLE()

#define USARTx_FORCE_RESET()             __HAL_RCC_USART1_FORCE_RESET()
#define USARTx_RELEASE_RESET()           __HAL_RCC_USART1_RELEASE_RESET()

/* Definition for USARTx Pins ---------------------------------------------------------------- */

#define USARTx_TX_PIN                    GPIO_PIN_6
#define USARTx_TX_GPIO_PORT              GPIOB
#define USARTx_TX_AF                     GPIO_AF7_USART1
#define USARTx_RX_PIN                    GPIO_PIN_7
#define USARTx_RX_GPIO_PORT              GPIOB
#define USARTx_RX_AF                     GPIO_AF7_USART1

/* Definition for USARTx's NVIC -------------------------------------------------------------- */

#define USARTx_IRQn                      USART1_IRQn
#define USARTx_IRQHandler                USART1_IRQHandler

/* Definition buffer size  ------------------------------------------------------------------- */

#define ComBuffSize											 10

/* Definition state of the communication  ---------------------------------------------------- */

#define UART_Ready											 0
#define UART_Busy												 1

/* Definition structures  -------------------------------------------------------------------- */

/* Struct to define commands to be sent to wifi chip */

typedef enum {
	
	No_Cmd	  = 0,
	Get_Time  = 1,
	Get_Date  = 2,
	Init_Ftp  = 3,
	Start_Ftp = 4
	
} UART_WIFI_CMD;

/* Struct for state of ESP32 answers */

typedef enum{
	
	No_Wait		     = 0,
	Wait_Ready     = 1,
	Wait_Time      = 2,
	Wait_Date	     = 3,
	Wait_Init_Ftp  = 4,
	Wait_Start_Ftp = 5,
	Wait_End_Ftp   = 6
	
} UART_WIFI_WAIT_RX;

/* Prototype  -------------------------------------------------------------------------------- */

void UART_Com_Wifi_Process (void);
void UART_Com_Wifi_Send_Cmd (UART_WIFI_CMD cmd);
void UART_Com_Wifi_Receve_Report (UART_WIFI_WAIT_RX Rx_Log);
void UART_Init(void);

#endif






