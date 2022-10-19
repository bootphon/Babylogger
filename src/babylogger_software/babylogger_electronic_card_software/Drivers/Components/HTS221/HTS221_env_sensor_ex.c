/**
 ******************************************************************************
 * @file    HTS221_env_sensors_ex.c
 * @author  MEMS Software Solutions Team
 * @brief   This file provides a set of extended functions needed to manage the environmental sensors
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

/* Includes ------------------------------------------------------------------*/
#include "HTS221_env_sensor_ex.h"

/** @addtogroup BSP BSP
 * @{
 */

/** @addtogroup HTS221 HTS221
 * @{
 */

/** @defgroup HTS221_ENV_SENSOR_EX HTS221 ENV SENSOR EX
 * @{
 */

/** @defgroup HTS221_ENV_SENSOR_EX_Imported_Variables HTS221 ENV SENSOR EX Imported Variables
 * @{
 */

extern void *EnvCompObj;

/**
 * @}
 */

/** @defgroup HTS221_ENV_SENSOR_EX_Exported_Functions HTS221 ENV SENSOR EX Exported Functions
 * @{
 */




/**
 * @brief  Get the status of data ready bit
 * @param  Function Environmental sensor function. Could be:
 *         - ENV_HUMIDITY or ENV_TEMPERATURE
 * @param  Status the pointer to the status
 * @retval BSP status
 */
int32_t HTS221_ENV_SENSOR_Get_DRDY_Status(uint32_t Function, uint8_t *Status)
{
  int32_t ret;


      if ((Function & ENV_HUMIDITY) == ENV_HUMIDITY)
      {
        if (HTS221_HUM_Get_DRDY_Status(EnvCompObj, Status) != BSP_ERROR_NONE)
        {
          ret = BSP_ERROR_COMPONENT_FAILURE;
        }
        else
        {
          ret = BSP_ERROR_NONE;
        }
      }
      else if ((Function & ENV_TEMPERATURE) == ENV_TEMPERATURE)
      {
        if (HTS221_TEMP_Get_DRDY_Status(EnvCompObj, Status) != BSP_ERROR_NONE)
        {
          ret = BSP_ERROR_COMPONENT_FAILURE;
        }
        else
        {
          ret = BSP_ERROR_NONE;
        }
      }
      else
      {
        ret = BSP_ERROR_COMPONENT_FAILURE;
      }



  return ret;
}

/**
 * @brief  Get the register value
 * @param  Reg address to be read
 * @param  Data pointer where the value is written to
 * @retval BSP status
 */
int32_t HTS221_ENV_SENSOR_Read_Register(uint8_t Reg, uint8_t *Data)
{
  int32_t ret;

 

      if (HTS221_Read_Reg(EnvCompObj, Reg, Data) != BSP_ERROR_NONE)
      {
        ret = BSP_ERROR_COMPONENT_FAILURE;
      }
      else
      {
        ret = BSP_ERROR_NONE;
      }

  return ret;
}

/**
 * @brief  Set the register value 
 * @param  Reg address to be read
 * @param  Data value to be written
 * @retval BSP status
 */
int32_t HTS221_ENV_SENSOR_Write_Register(uint8_t Reg, uint8_t Data)
{
  int32_t ret;

 
      if (HTS221_Write_Reg(EnvCompObj, Reg, Data) != BSP_ERROR_NONE)
      {
        ret = BSP_ERROR_COMPONENT_FAILURE;
      }
      else
      {
        ret = BSP_ERROR_NONE;
      }


  return ret;
}

/**
 * @brief  Set environmental sensor one shot mode
 * @retval BSP status
 */
int32_t HTS221_ENV_SENSOR_Set_One_Shot(void)
{
   int32_t ret;


      if (HTS221_Set_One_Shot(EnvCompObj) != BSP_ERROR_NONE)
      {
        ret = BSP_ERROR_COMPONENT_FAILURE;
      }
      else
      {
        ret = BSP_ERROR_NONE;
      }

  return ret;
}

/**
 * @brief  Get environmental sensor one shot status
 * @param  Status pointer to the one shot status (1 means measurements available, 0 means measurements not available yet)
 * @retval BSP status
 */
int32_t HTS221_ENV_SENSOR_Get_One_Shot_Status( uint8_t *Status)
{
   int32_t ret;

      if (HTS221_Get_One_Shot_Status(EnvCompObj, Status) != BSP_ERROR_NONE)
      {
        ret = BSP_ERROR_COMPONENT_FAILURE;
      }
      else
      {
        ret = BSP_ERROR_NONE;
      }

  return ret;
}

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

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
