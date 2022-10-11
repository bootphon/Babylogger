/**
 ******************************************************************************
 * @file    HTS221_env_sensor.h **modifié**
 * @author  MEMS Software Solutions Team
 * @brief   This file provides a set of functions needed to manage the environmental sensors
 ******************************************************************************
 * @attention
 *
 * <h2><center>&copy; Copyright (c) 2019 STMicroelectronics International N.V.
 * All rights reserved.</center></h2>
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted, provided that the following conditions are met:
 *
 * 1. Redistribution of source code must retain the above copyright notice,
 *    this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright notice,
 *    this list of conditions and the following disclaimer in the documentation
 *    and/or other materials provided with the distribution.
 * 3. Neither the name of STMicroelectronics nor the names of other
 *    contributors to this software may be used to endorse or promote products
 *    derived from this software without specific written permission.
 * 4. This software, including modifications and/or derivative works of this
 *    software, must execute solely and exclusively on microcontroller or
 *    microprocessor devices manufactured by or for STMicroelectronics.
 * 5. Redistribution and use of this software other than as permitted under
 *    this license is void and will automatically terminate your rights under
 *    this license.
 *
 * THIS SOFTWARE IS PROVIDED BY STMICROELECTRONICS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS, IMPLIED OR STATUTORY WARRANTIES, INCLUDING, BUT NOT
 * LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY, FITNESS FOR A
 * PARTICULAR PURPOSE AND NON-INFRINGEMENT OF THIRD PARTY INTELLECTUAL PROPERTY
 * RIGHTS ARE DISCLAIMED TO THE FULLEST EXTENT PERMITTED BY LAW. IN NO EVENT
 * SHALL STMICROELECTRONICS OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT,
 * INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
 * LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA,
 * OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF
 * LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING
 * NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE,
 * EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *
 ******************************************************************************
 */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef HTS221_ENV_SENSOR_H
#define HTS221_ENV_SENSOR_H

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
//#include "HTS221_conf.h"
//#include "env_sensor.h"

#include "I2C_com.h"
#include "hts221.h"

/** @addtogroup ENV_SENSORS ENV_SENSORS
 * @{
 */

/** @addtogroup ENV_SENSORS_Public_Types ENV_SENSORS Public types
 * @{
 */

/**
 * @brief  ENV_SENSORS driver structure definition
 */
typedef struct
{
  int32_t ( *Init              ) ( void * );
  int32_t ( *DeInit            ) ( void * );
  int32_t ( *ReadID            ) ( void *, uint8_t * ); 
  int32_t ( *GetCapabilities   ) ( void *, void * );
} ENV_SENSOR_CommonDrv_t;

typedef struct
{
  int32_t ( *Enable            ) ( void * );
  int32_t ( *Disable           ) ( void * );
  int32_t ( *GetOutputDataRate ) ( void *, float * );
  int32_t ( *SetOutputDataRate ) ( void *, float );
  int32_t ( *GetValue          ) ( void *, float * );
} ENV_SENSOR_FuncDrv_t;

/**
 * @}
 */
/** @addtogroup BSP BSP
 * @{
 */

/** @addtogroup HTS221 HTS221
 * @{
 */

/** @addtogroup HTS221_ENV_SENSORS HTS221 ENV SENSORS
 * @{
 */

/** @defgroup HTS221_ENV_SENSORS_Exported_Types HTS221 ENV SENSORS Exported Types
 * @{
 */

/* Environmental Sensor instance Info */
typedef struct
{
  uint8_t Temperature;
  uint8_t Pressure;
  uint8_t Humidity;
  uint8_t LowPower;
  float   HumMaxOdr;
  float   TempMaxOdr;
  float   PressMaxOdr;
} HTS221_ENV_SENSOR_Capabilities_t;

typedef struct
{
  uint32_t Functions;
} HTS221_ENV_SENSOR_Ctx_t;

/**
 * @}
 */

/** @defgroup HTS221_ENV_SENSOR_Exported_Constants HTS221 ENV SENSOR Exported Constants
 * @{
 */




#ifndef ENV_TEMPERATURE
#define ENV_TEMPERATURE      1U
#endif
#ifndef ENV_PRESSURE
#define ENV_PRESSURE         2U
#endif
#ifndef ENV_HUMIDITY
#define ENV_HUMIDITY         4U
#endif

#define HTS221_ENV_FUNCTIONS_NBR    3U




/**
 * @}
 */

/** @addtogroup HTS221_ENV_SENSORS_Exported_Functions HTS221 ENV SENSOR Exported Functions
 * @{
 */

int32_t HTS221_ENV_SENSOR_Init(uint32_t Functions);
int32_t HTS221_ENV_SENSOR_DeInit(void);
int32_t HTS221_ENV_SENSOR_GetCapabilities(HTS221_ENV_SENSOR_Capabilities_t *Capabilities);
int32_t HTS221_ENV_SENSOR_ReadID(uint8_t *Id);
int32_t HTS221_ENV_SENSOR_Enable(uint32_t Function);
int32_t HTS221_ENV_SENSOR_Disable(uint32_t Function);
int32_t HTS221_ENV_SENSOR_GetOutputDataRate( uint32_t Function, float *Odr);
int32_t HTS221_ENV_SENSOR_SetOutputDataRate( uint32_t Function, float Odr);
int32_t HTS221_ENV_SENSOR_GetValue( uint32_t Function, float *Value);

/**
 * @}
 */

/**
 * @}
 */

/**
 * @}
 */

/**
 * @}
 */

#ifdef __cplusplus
}
#endif

#endif /* HTS221_ENV_SENSOR_H */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
