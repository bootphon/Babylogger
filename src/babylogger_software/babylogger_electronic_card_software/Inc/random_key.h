#include "stm32l4xx_hal.h"
#include "I2C_com.h"



void RNG_Get_Key(uint32_t *key);
void RNG_Init(void);
void EEPROM_Save_Key(uint32_t *key);
void EEPROM_Read_Key(uint32_t *key, uint8_t n_key);

