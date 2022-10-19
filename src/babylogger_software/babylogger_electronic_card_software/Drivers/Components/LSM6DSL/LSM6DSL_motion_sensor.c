/**
 ******************************************************************************
 * @file    LSM6DSL_motion_sensor.c **Modifié**
 * @author  MEMS Software Solutions Team
 * @brief   This file provides a set of functions needed to manage the motion sensors
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
#include "LSM6DSL_motion_sensor.h"

/** @addtogroup BSP BSP
 * @{
 */

/** @addtogroup LSM6DSL LSM6DSL
 * @{
 */

/** @defgroup LSM6DSL_MOTION_SENSOR LSM6DSL MOTION SENSOR
 * @{
 */

/** @defgroup LSM6DSL_MOTION_SENSOR_Exported_Variables LSM6DSL MOTION SENSOR Exported Variables
 * @{
 */
extern void
*MotionCompObj; /* This "redundant" line is here to fulfil MISRA C-2012 rule 8.4 */
void *MotionCompObj;

/**
 * @}
 */

/** @defgroup LSM6DSL_MOTION_SENSOR_Private_Variables LSM6DSL MOTION SENSOR Private Variables
 * @{
 */

/* We define a jump table in order to get the correct index from the desired function. */
/* This table should have a size equal to the maximum value of a function plus 1.      */
static uint32_t FunctionIndex[5] = {0, 0, 1, 1, 2};
static MOTION_SENSOR_FuncDrv_t *MotionFuncDrv[LSM6DSL_MOTION_FUNCTIONS_NBR];
static MOTION_SENSOR_CommonDrv_t *MotionDrv;
static LSM6DSL_MOTION_SENSOR_Ctx_t MotionCtx;

/**
 * @}
 */

/** @defgroup LSM6DSL_MOTION_SENSOR_Private_Function_Prototypes LSM6DSL MOTION SENSOR Private Function Prototypes
 * @{
 */


static int32_t LSM6DSL_0_Probe(uint32_t Functions);


/**
 * @}
 */

/** @defgroup LSM6DSL_MOTION_SENSOR_Exported_Functions LSM6DSL MOTION SENSOR Exported Functions
 * @{
 */

/**
 * @brief  Initializes the motion sensors
 * @param  Instance Motion sensor instance
 * @param  Functions Motion sensor functions. Could be :
 *         - MOTION_GYRO and/or MOTION_ACCELERO for instance 0
 *         - MOTION_ACCELERO for instance 1
 *         - MOTION_MAGNETO for instance 2
 * @retval BSP status
 */
int32_t LSM6DSL_MOTION_SENSOR_Init(uint32_t Functions)
{
  uint32_t function = MOTION_GYRO;
	int32_t ret = BSP_ERROR_NONE;
  uint32_t i;
  uint32_t component_functions = 0;
  LSM6DSL_MOTION_SENSOR_Capabilities_t cap;

      if (LSM6DSL_0_Probe(Functions) != BSP_ERROR_NONE)
      {
        return BSP_ERROR_NO_INIT;
      }
      if (MotionDrv->GetCapabilities(MotionCompObj, (void *)&cap) != BSP_ERROR_NONE)
      {
        return BSP_ERROR_UNKNOWN_COMPONENT;
      }
      if (cap.Acc == 1U)
      {
        component_functions |= MOTION_ACCELERO;
      }
      if (cap.Gyro == 1U)
      {
        component_functions |= MOTION_GYRO;
      }
      if (cap.Magneto == 1U)
      {
        component_functions |= MOTION_MAGNETO;
      }

	if (ret != BSP_ERROR_NONE)
  {
    return ret;
  }

  for (i = 0; i < LSM6DSL_MOTION_FUNCTIONS_NBR; i++)
  {
    if (((Functions & function) == function) && ((component_functions & function) == function))
    {
      if (MotionFuncDrv[FunctionIndex[function]]->Enable(MotionCompObj) != BSP_ERROR_NONE)
      {
        return BSP_ERROR_COMPONENT_FAILURE;
      }
    }
    function = function << 1;
  }
	return ret;
}

/**
 * @brief  Deinitialize Motion sensor
 * @param  Instance Motion sensor instance
 * @retval BSP status
 */
int32_t LSM6DSL_MOTION_SENSOR_DeInit()
{
  int32_t ret;

  
  if (MotionDrv->DeInit(MotionCompObj) != BSP_ERROR_NONE)
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
 * @brief  Get motion sensor instance capabilities
 * @param  Instance Motion sensor instance
 * @param  Capabilities pointer to motion sensor capabilities
 * @retval BSP status
 */
int32_t LSM6DSL_MOTION_SENSOR_GetCapabilities(LSM6DSL_MOTION_SENSOR_Capabilities_t *Capabilities)
{
  int32_t ret;

  
  if (MotionDrv->GetCapabilities(MotionCompObj, Capabilities) != BSP_ERROR_NONE)
  {
    ret = BSP_ERROR_UNKNOWN_COMPONENT;
  }
  else
  {
    ret = BSP_ERROR_NONE;
  }

  return ret;
}

/**
 * @brief  Get WHOAMI value
 * @param  Instance Motion sensor instance
 * @param  Id WHOAMI value
 * @retval BSP status
 */
int32_t LSM6DSL_MOTION_SENSOR_ReadID( uint8_t *Id)
{
  int32_t ret;

  if (MotionDrv->ReadID(MotionCompObj, Id) != BSP_ERROR_NONE)
  {
    ret = BSP_ERROR_UNKNOWN_COMPONENT;
  }
  else
  {
    ret = BSP_ERROR_NONE;
  }

  return ret;
}

/**
 * @brief  Enable Motion sensor
 * @param  Instance Motion sensor instance
 * @param  Function Motion sensor function. Could be :
 *         - MOTION_GYRO and/or MOTION_ACCELERO for instance 0
 *         - MOTION_ACCELERO for instance 1
 *         - MOTION_MAGNETO for instance 2
 * @retval BSP status
 */
int32_t LSM6DSL_MOTION_SENSOR_Enable(uint32_t Function)
{
  int32_t ret;

    if ((MotionCtx.Functions & Function) == Function)
    {
      if (MotionFuncDrv[FunctionIndex[Function]]->Enable(MotionCompObj) != BSP_ERROR_NONE)
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
      ret = BSP_ERROR_WRONG_PARAM;
    }
  

  return ret;
}

/**
 * @brief  Disable Motion sensor
 * @param  Instance Motion sensor instance
 * @param  Function Motion sensor function. Could be :
 *         - MOTION_GYRO and/or MOTION_ACCELERO for instance 0
 *         - MOTION_ACCELERO for instance 1
 *         - MOTION_MAGNETO for instance 2
 * @retval BSP status
 */
int32_t LSM6DSL_MOTION_SENSOR_Disable(uint32_t Function)
{
  int32_t ret;

  
    if ((MotionCtx.Functions & Function) == Function)
    {
      if (MotionFuncDrv[FunctionIndex[Function]]->Disable(MotionCompObj) != BSP_ERROR_NONE)
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
      ret = BSP_ERROR_WRONG_PARAM;
    }
  

  return ret;
}

/**
 * @brief  Get motion sensor axes data
 * @param  Instance Motion sensor instance
 * @param  Function Motion sensor function. Could be :
 *         - MOTION_GYRO and/or MOTION_ACCELERO for instance 0
 *         - MOTION_ACCELERO for instance 1
 *         - MOTION_MAGNETO for instance 2
 * @param  Axes pointer to axes data structure
 * @retval BSP status
 */
int32_t LSM6DSL_MOTION_SENSOR_GetAxes(uint32_t Function, LSM6DSL_MOTION_SENSOR_Axes_t *Axes)
{
  int32_t ret;

    if ((MotionCtx.Functions & Function) == Function)
    {
      if (MotionFuncDrv[FunctionIndex[Function]]->GetAxes(MotionCompObj, Axes) != BSP_ERROR_NONE)
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
      ret = BSP_ERROR_WRONG_PARAM;
    }
  

  return ret;
}

/**
 * @brief  Get motion sensor axes raw data
 * @param  Instance Motion sensor instance
 * @param  Function Motion sensor function. Could be :
 *         - MOTION_GYRO and/or MOTION_ACCELERO for instance 0
 *         - MOTION_ACCELERO for instance 1
 *         - MOTION_MAGNETO for instance 2
 * @param  Axes pointer to axes raw data structure
 * @retval BSP status
 */
int32_t LSM6DSL_MOTION_SENSOR_GetAxesRaw(uint32_t Function, LSM6DSL_MOTION_SENSOR_AxesRaw_t *Axes)
{
  int32_t ret;

    if ((MotionCtx.Functions & Function) == Function)
    {
      if (MotionFuncDrv[FunctionIndex[Function]]->GetAxesRaw(MotionCompObj, Axes) != BSP_ERROR_NONE)
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
      ret = BSP_ERROR_WRONG_PARAM;
    }
  

  return ret;
}

/**
 * @brief  Get motion sensor sensitivity
 * @param  Instance Motion sensor instance
 * @param  Function Motion sensor function. Could be :
 *         - MOTION_GYRO and/or MOTION_ACCELERO for instance 0
 *         - MOTION_ACCELERO for instance 1
 *         - MOTION_MAGNETO for instance 2
 * @param  Sensitivity pointer to sensitivity read value
 * @retval BSP status
 */
int32_t LSM6DSL_MOTION_SENSOR_GetSensitivity(uint32_t Function, float *Sensitivity)
{
  int32_t ret;

    if ((MotionCtx.Functions & Function) == Function)
    {
      if (MotionFuncDrv[FunctionIndex[Function]]->GetSensitivity(MotionCompObj,
          Sensitivity) != BSP_ERROR_NONE)
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
      ret = BSP_ERROR_WRONG_PARAM;
    }
  

  return ret;
}

/**
 * @brief  Get motion sensor Output Data Rate
 * @param  Instance Motion sensor instance
 * @param  Function Motion sensor function. Could be :
 *         - MOTION_GYRO and/or MOTION_ACCELERO for instance 0
 *         - MOTION_ACCELERO for instance 1
 *         - MOTION_MAGNETO for instance 2
 * @param  Odr pointer to Output Data Rate read value
 * @retval BSP status
 */
int32_t LSM6DSL_MOTION_SENSOR_GetOutputDataRate( uint32_t Function, float *Odr)
{
  int32_t ret;

    if ((MotionCtx.Functions & Function) == Function)
    {
      if (MotionFuncDrv[FunctionIndex[Function]]->GetOutputDataRate(MotionCompObj, Odr) != BSP_ERROR_NONE)
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
      ret = BSP_ERROR_WRONG_PARAM;
    }
  

  return ret;
}

/**
 * @brief  Get motion sensor Full Scale
 * @param  Instance Motion sensor instance
 * @param  Function Motion sensor function. Could be :
 *         - MOTION_GYRO and/or MOTION_ACCELERO for instance 0
 *         - MOTION_ACCELERO for instance 1
 *         - MOTION_MAGNETO for instance 2
 * @param  Fullscale pointer to Fullscale read value
 * @retval BSP status
 */
int32_t LSM6DSL_MOTION_SENSOR_GetFullScale( uint32_t Function, int32_t *Fullscale)
{
  int32_t ret;


    if ((MotionCtx.Functions & Function) == Function)
    {
      if (MotionFuncDrv[FunctionIndex[Function]]->GetFullScale(MotionCompObj,
          Fullscale) != BSP_ERROR_NONE)
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
      ret = BSP_ERROR_WRONG_PARAM;
    }
  

  return ret;
}

/**
 * @brief  Set motion sensor Output Data Rate
 * @param  Instance Motion sensor instance
 * @param  Function Motion sensor function. Could be :
 *         - MOTION_GYRO and/or MOTION_ACCELERO for instance 0
 *         - MOTION_ACCELERO for instance 1
 *         - MOTION_MAGNETO for instance 2
 * @param  Odr Output Data Rate value to be set
 * @retval BSP status
 */
int32_t LSM6DSL_MOTION_SENSOR_SetOutputDataRate(uint32_t Function, float Odr)
{
  int32_t ret;


    if ((MotionCtx.Functions & Function) == Function)
    {
      if (MotionFuncDrv[FunctionIndex[Function]]->SetOutputDataRate(MotionCompObj, Odr) != BSP_ERROR_NONE)
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
      ret = BSP_ERROR_WRONG_PARAM;
    }
  

  return ret;
}

/**
 * @brief  Set motion sensor Full Scale
 * @param  Instance Motion sensor instance
 * @param  Function Motion sensor function. Could be :
 *         - MOTION_GYRO and/or MOTION_ACCELERO for instance 0
 *         - MOTION_ACCELERO for instance 1
 *         - MOTION_MAGNETO for instance 2
 * @param  Fullscale Fullscale value to be set
 * @retval BSP status
 */
int32_t LSM6DSL_MOTION_SENSOR_SetFullScale(uint32_t Function, int32_t Fullscale)
{
  int32_t ret;

  
    if ((MotionCtx.Functions & Function) == Function)
    {
      if (MotionFuncDrv[FunctionIndex[Function]]->SetFullScale(MotionCompObj,
          Fullscale) != BSP_ERROR_NONE)
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
      ret = BSP_ERROR_WRONG_PARAM;
    }
  

  return ret;
}

/**
 * @}
 */

/** @defgroup LSM6DSL_MOTION_SENSOR_Private_Functions LSM6DSL MOTION SENSOR Private Functions
 * @{
 */

/**
 * @brief  Register Bus IOs for instance 0 if component ID is OK
 * @retval BSP status
 */
static int32_t LSM6DSL_0_Probe(uint32_t Functions)
{
  LSM6DSL_IO_t            io_ctx;
  uint8_t                 id;
  static LSM6DSL_Object_t lsm6dsl_obj_0;
  LSM6DSL_Capabilities_t  cap;
  int32_t ret = BSP_ERROR_NONE;

  /* Configure the accelero driver */
  io_ctx.BusType     = LSM6DSL_I2C_BUS; /* I2C */
  io_ctx.Address     = LSM6DSL_I2C_ADD_L;
  io_ctx.Init        = SENSOR_I2C_Init;
  io_ctx.DeInit      = SENSOR_I2C_DeInit;
  io_ctx.ReadReg     = SENSOR_I2C_ReadReg;
  io_ctx.WriteReg    = SENSOR_I2C_WriteReg;
  io_ctx.GetTick     = SENSOR_GetTick;

  if (LSM6DSL_RegisterBusIO(&lsm6dsl_obj_0, &io_ctx) != LSM6DSL_OK)
  {
    ret = BSP_ERROR_UNKNOWN_COMPONENT;
  }
  else if (LSM6DSL_ReadID(&lsm6dsl_obj_0, &id) != LSM6DSL_OK)
  {
    ret = BSP_ERROR_UNKNOWN_COMPONENT;
  }
  else if (id != LSM6DSL_ID)
  {
    ret = BSP_ERROR_UNKNOWN_COMPONENT;
  }
  else
  {
    (void)LSM6DSL_GetCapabilities(&lsm6dsl_obj_0, &cap);
    MotionCtx.Functions = ((uint32_t)cap.Gyro) | ((uint32_t)cap.Acc << 1) | ((uint32_t)cap.Magneto << 2);

    MotionCompObj = &lsm6dsl_obj_0;
    /* The second cast (void *) is added to bypass Misra R11.3 rule */
    MotionDrv = (MOTION_SENSOR_CommonDrv_t *)(void *)&LSM6DSL_COMMON_Driver;

    if ((ret == BSP_ERROR_NONE) && ((Functions & MOTION_GYRO) == MOTION_GYRO) && (cap.Gyro == 1U))
    {
      /* The second cast (void *) is added to bypass Misra R11.3 rule */
      MotionFuncDrv[FunctionIndex[MOTION_GYRO]] = (MOTION_SENSOR_FuncDrv_t *)(void *)&LSM6DSL_GYRO_Driver;

      if (MotionDrv->Init(MotionCompObj) != LSM6DSL_OK)
      {
        ret = BSP_ERROR_COMPONENT_FAILURE;
      }
      else
      {
        ret = BSP_ERROR_NONE;
      }
    }
    if ((ret == BSP_ERROR_NONE) && ((Functions & MOTION_ACCELERO) == MOTION_ACCELERO) && (cap.Acc == 1U))
    {
      /* The second cast (void *) is added to bypass Misra R11.3 rule */
      MotionFuncDrv[FunctionIndex[MOTION_ACCELERO]] = (MOTION_SENSOR_FuncDrv_t *)(
            void *)&LSM6DSL_ACC_Driver;

      if (MotionDrv->Init(MotionCompObj) != LSM6DSL_OK)
      {
        ret = BSP_ERROR_COMPONENT_FAILURE;
      }
      else
      {
        ret = BSP_ERROR_NONE;
      }
    }
    if ((ret == BSP_ERROR_NONE) && ((Functions & MOTION_MAGNETO) == MOTION_MAGNETO))
    {
      /* Return an error if the application try to initialize a function not supported by the component */
      ret = BSP_ERROR_COMPONENT_FAILURE;
    }
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
