#include "random_key.h"

/* Private typedef -----------------------------------------------------------*/
RNG_HandleTypeDef RngHandle;

/* Private variables ---------------------------------------------------------*/
uint8_t counter = 0;
static uint8_t num_key = 0;


/**
* @brief Generation de clé
* Taille de la clé : 4 x 32 bits = 128 bits
* @param key : pointeur vers la valeur de la clé
* @retval None
*/
void RNG_Get_Key(uint32_t *key){
	for (counter = 0; counter < 4; counter++)
    {
      if (HAL_RNG_GenerateRandomNumber(&RngHandle, &key[counter]) != HAL_OK)
      {
        /* Random number generation error */     
      }
    }
}


/**
* @brief Initialization RNG
* @param None
* @retval None
*/
void RNG_Init(void){
	RngHandle.Instance = RNG;
	HAL_RNG_Init(&RngHandle);
	
}

/**
* @brief RNG MSP Initialization
* @param hrng: RNG handle pointer
* @retval None
*/
void HAL_RNG_MspInit(RNG_HandleTypeDef *hrng)
{  

  RCC_PeriphCLKInitTypeDef PeriphClkInitStruct;

  /*Select MSI output as RNG clock source */
  PeriphClkInitStruct.PeriphClockSelection = RCC_PERIPHCLK_RNG;
  PeriphClkInitStruct.RngClockSelection = RCC_RNGCLKSOURCE_MSI;
  HAL_RCCEx_PeriphCLKConfig(&PeriphClkInitStruct);

  /* RNG Peripheral clock enable */
  __HAL_RCC_RNG_CLK_ENABLE();

}

/**
  * @brief RNG MSP De-Initialization
  * @param hrng: RNG handle pointer
  * @retval None
  */
void HAL_RNG_MspDeInit(RNG_HandleTypeDef *hrng)
{
  /* Enable RNG reset state */
  __HAL_RCC_RNG_FORCE_RESET();

  /* Release RNG from reset state */
  __HAL_RCC_RNG_RELEASE_RESET();
} 


//**********************  Manage Key svgrd eeprom *******************************

/**
* @brief Sauvegarde de clé dans l'EEPROM 
* Taille de la clé : 4 x 32 bits = 128 bits / nombre d'emplacement clé dans l'EEPROM = 16
* @param key : pointeur vers la valeur de la clé
* @retval None
*/
void EEPROM_Save_Key(uint32_t *key){
	//Read/write
	HAL_GPIO_WritePin (GPIOB, GPIO_PIN_0, GPIO_PIN_RESET);
	I2C_WriteReg(EEPROM_ADDR, 16*num_key,(uint8_t*)&key[0], 16);
	
	num_key++;
	//Read only
	HAL_GPIO_WritePin (GPIOB, GPIO_PIN_0, GPIO_PIN_SET);
	
}

/**
* @brief Lecture de clé sauvegardée dans l'EEPROM 
* Clé lue : nmax clés sauvegardés - (nkey + 1)
* Pour lire la dernière clé sauvegardée: n_key = 0
* @param key : pointeur vers la valeur de la clé
* @param n_key : 
* @retval None
*/
void EEPROM_Read_Key(uint32_t *key, uint8_t n_key){
	I2C_ReadReg(EEPROM_ADDR, 16*(num_key -(n_key+1)), (uint8_t*)&key[0], 16);
}


