#include "stm32l4xx_hal.h"
#include "PWR.h"

// dans la carte final bouton sur gpioa 0

#define LED_1_2_Port					GPIOA
#define LED_1_Pin							GPIO_PIN_10
#define LED_2_Pin 						GPIO_PIN_11


// provisoir 

#define PB_Port 							GPIOA
#define PB_Pin								GPIO_PIN_0
#define PB_Exti_IRQn					EXTI0_IRQn
// fin du provisoir 

#define LED1 0		// rouge  
#define LED2 1  	// vert 
#define LED1_2 2

#define LED_Continious_Mode 0
#define LED_PWM_Mode				1



// proto de fonctions 

void script_test_led_func(void);

//void IHM_Debug_Led(uint8_t Err_Origine);
void IHM_LED_Start(uint8_t Mode, uint8_t LEDx);
void IHM_LED_Stop(uint8_t LEDx);


void IHM_LED_PWM_Init(uint8_t LEDx);
void IHM_LED_Start_Blink(uint8_t val);
void IHM_LED_Stop_Blink(uint8_t val);
void IHM_LED_DeInit(void);


void IHM_LED_Continious_Init(uint8_t LEDx);
void IHM_LED_On(uint8_t LEDx);
void IHM_LED_Off(uint8_t LEDx);
void IHM_LED_Toggle( uint8_t LEDx );

void IHM_PB_Init(void);
uint32_t IHM_PB_GetState(void);
void Tempo_PB_Process(void);
void IHM_PB_Callback (void);
__weak void IHM_PB_Go_To_Factory_Mode(void);
__weak void Buton_Hold_Callback (void);

