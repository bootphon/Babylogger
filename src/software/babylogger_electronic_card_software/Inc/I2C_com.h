#include "stm32l4xx_hal.h"




#define EEPROM_ADDR 0xA0


#define SENSOR_I2C_X 					I2C4
#define SENSOR_I2C_PORT				GPIOB
#define SENSOR_I2C_SDA				GPIO_PIN_11
#define SENSOR_I2C_SCL				GPIO_PIN_10
#define SENSOR_I2C_AF 				GPIO_AF3_I2C4

int32_t I2C_ReadReg(uint16_t DevAddr, uint16_t Reg, uint8_t *pData, uint16_t Length) ;
int32_t I2C_WriteReg(uint16_t DevAddr, uint16_t Reg, uint8_t *pData, uint16_t Length);
int32_t I2C_Init(void);
int32_t I2C_DeInit(void);
int32_t SENSOR_IO_GetTick(void);
void SENSOR_DMA_Start(uint16_t DevAddress, uint16_t MemAddress, uint16_t MemAddSize, uint8_t *pData, uint16_t Size);

//Link fonctions drivers
#define SENSOR_I2C_Init I2C_Init
#define SENSOR_I2C_DeInit I2C_DeInit
#define SENSOR_I2C_ReadReg I2C_ReadReg
#define SENSOR_I2C_WriteReg I2C_WriteReg
#define SENSOR_GetTick SENSOR_IO_GetTick


/* BSP Common Error codes */
#define BSP_ERROR_NONE                    0
#define BSP_ERROR_NO_INIT                -1
#define BSP_ERROR_WRONG_PARAM            -2
#define BSP_ERROR_BUSY                   -3
#define BSP_ERROR_PERIPH_FAILURE         -4
#define BSP_ERROR_COMPONENT_FAILURE      -5
#define BSP_ERROR_UNKNOWN_FAILURE        -6
#define BSP_ERROR_UNKNOWN_COMPONENT      -7 
#define BSP_ERROR_BUS_FAILURE            -8
#define BSP_ERROR_CLOCK_FAILURE          -9
#define BSP_ERROR_MSP_FAILURE            -10
#define BSP_ERROR_FEATURE_NOT_SUPPORTED      -11
   
   
/* BSP BUS error codes */
#define BSP_ERROR_BUS_TRANSACTION_FAILURE    -100
#define BSP_ERROR_BUS_ARBITRATION_LOSS       -101
#define BSP_ERROR_BUS_ACKNOWLEDGE_FAILURE    -102
#define BSP_ERROR_BUS_PROTOCOL_FAILURE       -103

#define BSP_ERROR_BUS_MODE_FAULT             -104
#define BSP_ERROR_BUS_FRAME_ERROR            -105
#define BSP_ERROR_BUS_CRC_ERROR              -106
#define BSP_ERROR_BUS_DMA_FAILURE            -107



