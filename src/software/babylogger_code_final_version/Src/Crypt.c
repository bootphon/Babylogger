#include "Crypt.h"
#include "PWR.h"
#include "gatt_db.h"
#include "I2C_com.h"



/* Private typedef -----------------------------------------------------------*/

CRYP_HandleTypeDef     CrypHandle;

/* Defenition buffer ---------------------------------------------------------*/

int16_t                      CryptSvgrdBuff[(4*8*BuffSize)+16];
int16_t*              	     pCryptSvgrdHalfBuff = &CryptSvgrdBuff[0];
int16_t*              	     pCryptSvgrdCpltBuff = &CryptSvgrdBuff[(16*BuffSize)+8];

/* External variables --------------------------------------------------------*/

extern int16_t*              pSvgrdHalfBuff;
extern int16_t*              pSvgrdCpltBuff;

/* Flags ---------------------------------------------------------------------*/

uint8_t 		     HalfSvgrdBuffCplt      = 0;
uint8_t 		     SvgrdBuffCplt          = 0;

/* Debug variables -----------------------------------------------------------*/

uint32_t		    hz = 0;
uint32_t		    cz = 0;

/* AES parameters ------------------------------------------------------------*/
uint8_t aAES128Key[16] = 
                         {0x2b, 0x7e, 0x15, 0x16, 0x28, 0xae, 0xd2, 0xa6,
                          0xab, 0xf7, 0x15, 0x88, 0x09, 0xcf, 0x4f, 0x3c
                         };
												 
uint8_t InitVect[16]	 = {0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07,
                          0x08, 0x09, 0x0a, 0x0b, 0x0c, 0x0d, 0x0e, 0x0f
                         };

												 
/*******************************************************************************
* Function Name  : BLE_Key_Received 
* Description    : Redefined to react after a BLE key rx event.
* Input          : Array of 128 bits (array of 16 uint8)
* Return         : Status: 0 = succeed, > 0 Failed (number of Byte diff)
*******************************************************************************/												 
uint8_t BLE_Key_Received(uint8_t* pData)
{
	uint8_t index = 0;
	uint8_t status = 0;
	
	for (index=0;index<16;index++)
	{
		aAES128Key[index] = pData[index];
	}
	
		HAL_GPIO_WritePin (GPIOB, GPIO_PIN_0, GPIO_PIN_RESET);
	
	// write data
	I2C_WriteReg(EEPROM_ADDR, 16,(uint8_t*)&aAES128Key[0] , 16);
	
	HAL_Delay(15);
	
	I2C_WriteReg(EEPROM_ADDR, 32,(uint8_t*)&aAES128Key[0] , 16);
	
	//Set eeprom on Read only mode
	HAL_GPIO_WritePin (GPIOB, GPIO_PIN_0, GPIO_PIN_SET);
	
	HAL_Delay(15);
	
	status = Crypt_Get_Key_From_EEPROM();
	
	return status;
	
}			

/*******************************************************************************
* Function Name  : BLE_Key_Received 
* Description    : Redefined to react after a BLE key rx event.
* Input          : No Input
* Return         : Status: 0 = succeed, > 0 Failed (number of Byte diff)
*******************************************************************************/												 
uint8_t Crypt_Get_Key_From_EEPROM(void)
{
	uint8_t index = 0;
	uint8_t Key_tmp[16];
	uint8_t status = 0;
	
	
	I2C_ReadReg(EEPROM_ADDR, 16, (uint8_t*)&aAES128Key[0], 16);
	HAL_Delay(15);
	I2C_ReadReg(EEPROM_ADDR, 32, (uint8_t*)&Key_tmp[0], 16);
	HAL_Delay(15);
	
	for (index=0;index<16;index++)
	{
		if (aAES128Key[index] != Key_tmp[index]) status++;
	}
	
	return status;
	
}		
/*******************************************************************************
* Function Name  : HalfSvgrBuffCpltCallback 
* Description    : Redefined to notify that data are ready to be crypt (first half).
* Input          : No input
* Return         : No return value
*******************************************************************************/												 
void HalfSvgrBuffCpltCallback(void)
{
	HalfSvgrdBuffCplt = 1;
}

/*******************************************************************************
* Function Name  : SvgrBuffCpltCallback 
* Description    : Redefined to notify that data are ready to be crypt (second half).
* Input          : No input
* Return         : No return value
*******************************************************************************/
void SvgrBuffCpltCallback(void)
{
	SvgrdBuffCplt = 1;
}

/*******************************************************************************
* Function Name  : HalfCryptSvgrBuffCpltCallback 
* Description    : Callback to notify that encryption for the first half is complete
* Input          : No input
* Return         : No return value
*******************************************************************************/
__weak void HalfCryptSvgrBuffCpltCallback(void)
{
	/* Redefined in fatfs.c */
}

/*******************************************************************************
* Function Name  : CryptSvgrBuffCpltCallback 
* Description    : Callback to notify that encryption for the second half is complete
* Input          : No input
* Return         : No return value
*******************************************************************************/
__weak void CryptSvgrBuffCpltCallback(void)
{
	/* Redefined in fatfs.c */
}

/*******************************************************************************
* Function Name  : Cryp_Pending 
* Description    : Check if there are data waiting to be encrypted.
* Input          : No input
* Return         : Return 0 if there is no data pending, else 1.
*******************************************************************************/
uint8_t Cryp_Pending (void)
{
	uint8_t tmp;
	
	if((HalfSvgrdBuffCplt == 0) && (SvgrdBuffCplt == 0))
	{
		tmp = 0;
	}
	else
	{
		tmp = 1;
	}
	return tmp;
}	

/*******************************************************************************
* Function Name  : Cryp_Init_process_variables 
* Description    : Reste flags.
* Input          : No input
* Return         : No return value.
*******************************************************************************/
void Cryp_Init_process_variables(void)
{
	HalfSvgrdBuffCplt = 0;
	SvgrdBuffCplt 		= 0;
}
/*******************************************************************************
* Function Name  : Cryp_Get_Debug_Variables 
* Description    : Get the total number of encrypted buffer.
* Input          : No input
* Return         : Return the total number of encrypted buffer.
*******************************************************************************/	
uint32_t Cryp_Get_Debug_Variables(void)
{
	uint32_t tmp; 
	tmp = hz+cz;
	return tmp;
}

/*******************************************************************************
* Function Name  : Crypt_process 
* Description    : Manage the encryption.
* Input          : No input
* Return         : No return value.
*******************************************************************************/
void Crypt_process(void)
{		
	if (HalfSvgrdBuffCplt == 1)
	{
		Crypt_fist_half_buffer();
		HalfSvgrdBuffCplt = 0;
		hz++;
		HalfCryptSvgrBuffCpltCallback();
	} 
	
	if  (SvgrdBuffCplt == 1)
	{
		Crypt_second_half_buffer();
		SvgrdBuffCplt = 0;
		cz++;
		CryptSvgrBuffCpltCallback();
	} 	
}

/*******************************************************************************
* Function Name  : Crypt_Init_Debug_Variables 
* Description    : Reset debug variables.
* Input          : No input
* Return         : No return value.
*******************************************************************************/
void Crypt_Init_Debug_Variables(void)	
{
	hz=cz=0;
}

/*******************************************************************************
* Function Name  : Crypt_fist_half_buffer 
* Description    : Interface : Encrypt first buffer.
* Input          : No input
* Return         : No return value.
*******************************************************************************/
void Crypt_fist_half_buffer(void)
{
	if (HAL_CRYPEx_AES(&CrypHandle,(uint8_t*) pSvgrdHalfBuff, ((32*BuffSize)+16),(uint8_t*)&CryptSvgrdBuff[0],5) != HAL_OK)
  {
    /* Error management */
  }
}
/*******************************************************************************
* Function Name  : Crypt_fist_half_buffer 
* Description    : Interface : Encrypt second buffer.
* Input          : No input
* Return         : No return value.
*******************************************************************************/
void Crypt_second_half_buffer(void)
{	
	if (HAL_CRYPEx_AES(&CrypHandle,(uint8_t*)pSvgrdCpltBuff, ((32*BuffSize)+16),(uint8_t*) &CryptSvgrdBuff[(16*BuffSize)+8],5) != HAL_OK)
  {
    /* Error management */ 
  }
}
	
/*******************************************************************************
* Function Name  : CRYPT_Init 
* Description    : Configure AES peripheral working for encryption.
* Input          : Is first Init
* Return         : No return value.
*******************************************************************************/

void CRYPT_Init (uint8_t First_Init)
{
	
	/* Set the key if it's the first Init only to prevent time loss during file swaping */
	if (First_Init == 1)
	{
		while (Crypt_Get_Key_From_EEPROM() != 0 )
		{
		
		}
	}
	
  CrypHandle.Instance = AES;
  CrypHandle.Init.DataType      = CRYP_DATATYPE_8B;
  CrypHandle.Init.pKey          = aAES128Key;
  CrypHandle.Init.KeySize       = CRYP_KEYSIZE_128B;
  CrypHandle.Init.OperatingMode = CRYP_ALGOMODE_ENCRYPT;
  CrypHandle.Init.ChainingMode  = CRYP_CHAINMODE_AES_CTR;
  CrypHandle.Init.KeyWriteFlag  = CRYP_KEY_WRITE_ENABLE; 
  CrypHandle.Init.pInitVect	= InitVect;
	
  if (HAL_CRYP_Init(&CrypHandle) != HAL_OK)
  {
		/* Error management */
			PWR_Reset_System(); 
  }	
}

/*******************************************************************************
* Function Name  : DECRYPT_Init 
* Description    : Configure AES peripheral working for Decryption.
  PS: Used for test only, and Must be called after CRYPT_Init
* Input          : No input
* Return         : No return value.
*******************************************************************************/
void DECRYPT_Init (void)
{
	CrypHandle.Init.OperatingMode = CRYP_ALGOMODE_DECRYPT;
	if (HAL_CRYP_Init(&CrypHandle) != HAL_OK)
  {
			/* Error management */
  }
}
/*******************************************************************************
* Function Name  : HAL_CRYP_MspInit 
* Description    : Redefined to configure low level parameters. 
* Input          : No input
* Return         : No return value.
*******************************************************************************/
void HAL_CRYP_MspInit(CRYP_HandleTypeDef *hcryp)
{
  __HAL_RCC_AES_CLK_ENABLE();
  __HAL_RCC_AES_FORCE_RESET();   
  __HAL_RCC_AES_RELEASE_RESET();
}

/*******************************************************************************
* Function Name  : HAL_CRYP_MspDeInit
* Description    : Redefined to reset low level parameters. 
* Input          : No input
* Return         : No return value.
*******************************************************************************/
void HAL_CRYP_MspDeInit(CRYP_HandleTypeDef *hcryp)
{
  /* Disable CRYP clock */
  __HAL_RCC_AES_CLK_DISABLE();
}



