#include "stm32l4xx_hal.h"
#include "Acquisition_Audio.h"


/* Prototype  -------------------------------------------------------------------------------- */

uint8_t Crypt_Get_Key_From_EEPROM(void);
uint8_t Cryp_Pending (void);
void Cryp_Init_process_variables(void);
uint32_t Cryp_Get_Debug_Variables(void);
void Crypt_process(void);
void Crypt_fist_half_buffer(void);
void Crypt_second_half_buffer(void);
void CRYPT_Init (uint8_t First_Init);
void DECRYPT_Init (void);
void Crypt_Init_Debug_Variables(void);
__weak void HalfCryptSvgrBuffCpltCallback(void);
__weak void CryptSvgrBuffCpltCallback(void);





