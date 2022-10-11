/**
 ******************************************************************************
 * @file    LSM6DSL_motion_sensor_ex.c
 * @author  MEMS Software Solutions Team ** modifié**
 * @brief   This file provides a set of extended functions needed to manage the motion sensors
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
#include "LSM6DSL_motion_sensor_ex.h"

/** @addtogroup BSP BSP
 * @{
 */

/** @addtogroup LSM6DSL LSM6DSL
 * @{
 */

/** @defgroup LSM6DSL_MOTION_SENSOR_EX LSM6DSL MOTION SENSOR EX
 * @{
 */

/** @defgroup LSM6DSL_MOTION_SENSOR_EX_Imported_Variables LSM6DSL MOTION SENSOR EX Imported Variables
 * @{
 */

extern void *MotionCompObj;

/**
 * @}
 */

/** @defgroup LSM6DSL_MOTION_SENSOR_EX_Exported_Functions LSM6DSL MOTION SENSOR EX Exported Functions
 * @{
 */

/**
 * @brief  Enable 6D Orientation (available only for LSM6DSL sensor)
 * @param  IntPin the interrupt pin to be used
 * @note   This function sets the LSM6DSL accelerometer ODR to 416Hz and the LSM6DSL accelerometer full scale to 2g
 * @retval BSP status
 */
int32_t LSM6DSL_MOTION_SENSOR_Enable_6D_Orientation(LSM6DSL_MOTION_SENSOR_IntPin_t IntPin)
{
  int32_t ret;

    
      if (LSM6DSL_ACC_Enable_6D_Orientation(MotionCompObj, (LSM6DSL_SensorIntPin_t)IntPin) != BSP_ERROR_NONE)
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
 * @brief  Disable 6D Orientation (available only for LSM6DSL sensor)
 * @retval BSP status
 */
int32_t LSM6DSL_MOTION_SENSOR_Disable_6D_Orientation()
{
  int32_t ret;


      if (LSM6DSL_ACC_Disable_6D_Orientation(MotionCompObj) != BSP_ERROR_NONE)
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
 * @brief  Set the 6D orientation threshold (available only for LSM6DSL sensor)
 * @param  Threshold the threshold to be set
 * @retval BSP status
 */
int32_t LSM6DSL_MOTION_SENSOR_Set_6D_Orientation_Threshold(uint8_t Threshold)
{
  int32_t ret;

      if (LSM6DSL_ACC_Set_6D_Orientation_Threshold(MotionCompObj, Threshold) != BSP_ERROR_NONE)
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
 * @brief  Enable the free fall detection (available only for LSM6DSL sensor)
 * @param  IntPin the interrupt pin to be used
 * @note   This function sets the LSM6DSL accelerometer ODR to 416Hz and the LSM6DSL accelerometer full scale to 2g
 * @retval BSP status
 */
int32_t LSM6DSL_MOTION_SENSOR_Enable_Free_Fall_Detection(LSM6DSL_MOTION_SENSOR_IntPin_t IntPin)
{
  int32_t ret;

 
      if (LSM6DSL_ACC_Enable_Free_Fall_Detection(MotionCompObj, (LSM6DSL_SensorIntPin_t)IntPin) != BSP_ERROR_NONE)
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
 * @brief  Disable the free fall detection (available only for LSM6DSL sensor)
 * @retval BSP status
 */
int32_t LSM6DSL_MOTION_SENSOR_Disable_Free_Fall_Detection()
{
  int32_t ret;

      if (LSM6DSL_ACC_Disable_Free_Fall_Detection(MotionCompObj) != BSP_ERROR_NONE)
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
 * @brief  Set the free fall detection threshold (available only for LSM6DSL sensor)
 * @param  Threshold the threshold to be set
 * @retval BSP status
 */
int32_t LSM6DSL_MOTION_SENSOR_Set_Free_Fall_Threshold(uint8_t Threshold)
{
  int32_t ret;

      if (LSM6DSL_ACC_Set_Free_Fall_Threshold(MotionCompObj, Threshold) != BSP_ERROR_NONE)
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
 * @brief  Set the free fall detection duration (available only for LSM6DSL sensor)
 * @param  Duration the duration to be set
 * @retval BSP status
 */
int32_t LSM6DSL_MOTION_SENSOR_Set_Free_Fall_Duration(uint8_t Duration)
{
  int32_t ret;

      if (LSM6DSL_ACC_Set_Free_Fall_Duration(MotionCompObj, Duration) != BSP_ERROR_NONE)
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
 * @brief  Enable the pedometer detection (available only for LSM6DSL sensor)
 * @note   This function sets the LSM6DSL accelerometer ODR to 26Hz and the LSM6DSL accelerometer full scale to 2g
 * @retval BSP status
 */
int32_t LSM6DSL_MOTION_SENSOR_Enable_Pedometer()
{
  int32_t ret;

 
      if (LSM6DSL_ACC_Enable_Pedometer(MotionCompObj) != BSP_ERROR_NONE)
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
 * @brief  Disable the pedometer detection (available only for LSM6DSL sensor)
 * @retval BSP status
 */
int32_t LSM6DSL_MOTION_SENSOR_Disable_Pedometer()
{
  int32_t ret;


      if (LSM6DSL_ACC_Disable_Pedometer(MotionCompObj) != BSP_ERROR_NONE)
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
 * @brief  Set the pedometer threshold (available only for LSM6DSL sensor)
 * @param  Threshold the threshold to be set
 * @retval BSP status
 */
int32_t LSM6DSL_MOTION_SENSOR_Set_Pedometer_Threshold(uint8_t Threshold)
{
  int32_t ret;


      if (LSM6DSL_ACC_Set_Pedometer_Threshold(MotionCompObj, Threshold) != BSP_ERROR_NONE)
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
 * @brief  Reset step counter (available only for LSM6DSL sensor)
 * @note   This function sets the LSM6DSL accelerometer ODR to 26Hz and the LSM6DSL accelerometer full scale to 2g
 * @retval BSP status
 */
int32_t LSM6DSL_MOTION_SENSOR_Reset_Step_Counter()
{
  int32_t ret;

      if (LSM6DSL_ACC_Enable_Step_Counter_Reset(MotionCompObj) != BSP_ERROR_NONE)
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
 * @brief  Enable the single tap detection (available only for LSM6DSL sensor)
 * @param  IntPin the interrupt pin to be used
 * @note   This function sets the LSM6DSL accelerometer ODR to 416Hz and the LSM6DSL accelerometer full scale to 2g
 * @retval BSP status
 */
int32_t LSM6DSL_MOTION_SENSOR_Enable_Single_Tap_Detection(LSM6DSL_MOTION_SENSOR_IntPin_t IntPin)
{
  int32_t ret;


      if (LSM6DSL_ACC_Enable_Single_Tap_Detection(MotionCompObj, (LSM6DSL_SensorIntPin_t)IntPin) != BSP_ERROR_NONE)
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
 * @brief  Disable the single tap detection (available only for LSM6DSL sensor)
 * @retval BSP status
 */
int32_t LSM6DSL_MOTION_SENSOR_Disable_Single_Tap_Detection()
{
  int32_t ret;

      if (LSM6DSL_ACC_Disable_Single_Tap_Detection(MotionCompObj) != BSP_ERROR_NONE)
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
 * @brief  Enable the double tap detection (available only for LSM6DSL sensor)
 * @param  IntPin the interrupt pin to be used
 * @note   This function sets the LSM6DSL accelerometer ODR to 416Hz and the LSM6DSL accelerometer full scale to 2g
 * @retval BSP status
 */
int32_t LSM6DSL_MOTION_SENSOR_Enable_Double_Tap_Detection(LSM6DSL_MOTION_SENSOR_IntPin_t IntPin)
{
  int32_t ret;

 
      if (LSM6DSL_ACC_Enable_Double_Tap_Detection(MotionCompObj, (LSM6DSL_SensorIntPin_t)IntPin) != BSP_ERROR_NONE)
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
 * @brief  Disable the double tap detection (available only for LSM6DSL sensor)
 * @retval BSP status
 */
int32_t LSM6DSL_MOTION_SENSOR_Disable_Double_Tap_Detection()
{
  int32_t ret;

 
      if (LSM6DSL_ACC_Disable_Double_Tap_Detection(MotionCompObj) != BSP_ERROR_NONE)
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
 * @brief  Set the tap threshold (available only for LSM6DSL sensor)
 * @param  Threshold the threshold to be set
 * @retval BSP status
 */
int32_t LSM6DSL_MOTION_SENSOR_Set_Tap_Threshold(uint8_t Threshold)
{
  int32_t ret;

      if (LSM6DSL_ACC_Set_Tap_Threshold(MotionCompObj, Threshold) != BSP_ERROR_NONE)
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
 * @brief  Set the tap shock time (available only for LSM6DSL sensor)
 * @param  Time the tap shock time to be set
 * @retval BSP status
 */
int32_t LSM6DSL_MOTION_SENSOR_Set_Tap_Shock_Time(uint8_t Time)
{
  int32_t ret;


      if (LSM6DSL_ACC_Set_Tap_Shock_Time(MotionCompObj, Time) != BSP_ERROR_NONE)
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
 * @brief  Set the tap quiet time (available only for LSM6DSL sensor)
 * @param  Time the tap quiet time to be set
 * @retval BSP status
 */
int32_t LSM6DSL_MOTION_SENSOR_Set_Tap_Quiet_Time(uint8_t Time)
{
  int32_t ret;


      if (LSM6DSL_ACC_Set_Tap_Quiet_Time(MotionCompObj, Time) != BSP_ERROR_NONE)
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
 * @brief  Set the tap duration time (available only for LSM6DSL sensor)
 * @param  Time the tap duration time to be set
 * @retval BSP status
 */
int32_t LSM6DSL_MOTION_SENSOR_Set_Tap_Duration_Time(uint8_t Time)
{
  int32_t ret;

      if (LSM6DSL_ACC_Set_Tap_Duration_Time(MotionCompObj, Time) != BSP_ERROR_NONE)
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
 * @brief  Enable the tilt detection (available only for LSM6DSL sensor)
 * @param  IntPin the interrupt pin to be used
 * @note   This function sets the LSM6DSL accelerometer ODR to 26Hz and the LSM6DSL accelerometer full scale to 2g
 * @retval BSP status
 */
int32_t LSM6DSL_MOTION_SENSOR_Enable_Tilt_Detection(LSM6DSL_MOTION_SENSOR_IntPin_t IntPin)
{
  int32_t ret;

      if (LSM6DSL_ACC_Enable_Tilt_Detection(MotionCompObj, (LSM6DSL_SensorIntPin_t)IntPin) != BSP_ERROR_NONE)
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
 * @brief  Disable the tilt detection (available only for LSM6DSL sensor)
 * @retval BSP status
 */
int32_t LSM6DSL_MOTION_SENSOR_Disable_Tilt_Detection()
{
  int32_t ret;


      if (LSM6DSL_ACC_Disable_Tilt_Detection(MotionCompObj) != BSP_ERROR_NONE)
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
 * @brief  Enable the wake up detection (available only for LSM6DSL sensor)
 * @param  IntPin the interrupt pin to be used
 * @note   This function sets the LSM6DSL accelerometer ODR to 416Hz and the LSM6DSL accelerometer full scale to 2g
 * @retval BSP status
 */
int32_t LSM6DSL_MOTION_SENSOR_Enable_Wake_Up_Detection(LSM6DSL_MOTION_SENSOR_IntPin_t IntPin)
{
  int32_t ret;

      if (LSM6DSL_ACC_Enable_Wake_Up_Detection(MotionCompObj, (LSM6DSL_SensorIntPin_t)IntPin) != BSP_ERROR_NONE)
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
 * @brief  Disable the wake up detection (available only for LSM6DSL sensor)
 * @retval BSP status
 */
int32_t LSM6DSL_MOTION_SENSOR_Disable_Wake_Up_Detection()
{
  int32_t ret;

 
      if (LSM6DSL_ACC_Disable_Wake_Up_Detection(MotionCompObj) != BSP_ERROR_NONE)
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
 * @brief  Set the wake up detection threshold (available only for LSM6DSL sensor)
 * @param  Threshold the threshold to be set
 * @retval BSP status
 */
int32_t LSM6DSL_MOTION_SENSOR_Set_Wake_Up_Threshold(uint8_t Threshold)
{
  int32_t ret;


      if (LSM6DSL_ACC_Set_Wake_Up_Threshold(MotionCompObj, Threshold) != BSP_ERROR_NONE)
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
 * @brief  Set the wake up detection duration (available only for LSM6DSL sensor)
 * @param  Duration the duration to be set
 * @retval BSP status
 */
int32_t LSM6DSL_MOTION_SENSOR_Set_Wake_Up_Duration( uint8_t Duration)
{
  int32_t ret;


      if (LSM6DSL_ACC_Set_Wake_Up_Duration(MotionCompObj, Duration) != BSP_ERROR_NONE)
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
 * @brief  Enable the inactivity detection (available only for LSM6DSL sensor)
 * @param  IntPin the interrupt pin to be used
 * @retval BSP status
 */
int32_t LSM6DSL_MOTION_SENSOR_Enable_Inactivity_Detection(LSM6DSL_MOTION_SENSOR_IntPin_t IntPin)
{
  int32_t ret;

      if (LSM6DSL_ACC_Enable_Inactivity_Detection(MotionCompObj, (LSM6DSL_SensorIntPin_t)IntPin) != BSP_ERROR_NONE)
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
 * @brief  Disable the inactivity detection (available only for LSM6DSL sensor)
 * @retval BSP status
 */
int32_t LSM6DSL_MOTION_SENSOR_Disable_Inactivity_Detection()
{
  int32_t ret;

      if (LSM6DSL_ACC_Disable_Inactivity_Detection(MotionCompObj) != BSP_ERROR_NONE)
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
 * @brief  Set the sleep duration (available only for LSM6DSL sensor)
 * @param  Duration the duration to be set
 * @retval BSP status
 */
int32_t LSM6DSL_MOTION_SENSOR_Set_Sleep_Duration( uint8_t Duration)
{
  int32_t ret;


      if (LSM6DSL_ACC_Set_Sleep_Duration(MotionCompObj, Duration) != BSP_ERROR_NONE)
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
 * @brief  Get the status of all hardware events (available only for LSM6DSL sensor)
 * @param  Status the pointer to the status of all hardware events
 * @retval BSP status
 */
int32_t LSM6DSL_MOTION_SENSOR_Get_Event_Status(LSM6DSL_MOTION_SENSOR_Event_Status_t *Status)
{
  int32_t ret;

      /* The second cast (void *) is added to bypass Misra R11.3 rule */
      if (LSM6DSL_ACC_Get_Event_Status(MotionCompObj, (LSM6DSL_Event_Status_t *)(void *)Status) != BSP_ERROR_NONE)
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
 * @brief  Get the status of data ready bit (available only for LSM6DSL, LSM303AGR sensors)
 * @param  Function Motion sensor function. Could be:
 *         - MOTION_ACCELERO or MOTION_GYRO for instance LSM6DSL_LSM6DSL_0
 *         - MOTION_ACCELERO for instance LSM6DSL_LSM303AGR_ACC_0
 *         - MOTION_MAGNETO for instance LSM6DSL_LSM303AGR_MAG_0
 * @param  Status the pointer to the status
 * @retval BSP status
 */
int32_t LSM6DSL_MOTION_SENSOR_Get_DRDY_Status(uint32_t Function, uint8_t *Status)
{
  int32_t ret;

      if ((Function & MOTION_ACCELERO) == MOTION_ACCELERO)
      {
        if (LSM6DSL_ACC_Get_DRDY_Status(MotionCompObj, Status) != BSP_ERROR_NONE)
        {
          ret = BSP_ERROR_COMPONENT_FAILURE;
        }
        else
        {
          ret = BSP_ERROR_NONE;
        }
      }
      else if ((Function & MOTION_GYRO) == MOTION_GYRO)
      {
        if (LSM6DSL_GYRO_Get_DRDY_Status(MotionCompObj, Status) != BSP_ERROR_NONE)
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
 * @brief  Get 6D Orientation XL (available only for LSM6DSL sensor)
 * @param  xl the pointer to the 6D orientation XL axis
 * @retval BSP status
 */
int32_t LSM6DSL_MOTION_SENSOR_Get_6D_Orientation_XL( uint8_t *xl)
{
  int32_t ret;

      if (LSM6DSL_ACC_Get_6D_Orientation_XL(MotionCompObj, xl) != BSP_ERROR_NONE)
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
 * @brief  Get 6D Orientation XH (available only for LSM6DSL sensor)
 * @param  xh the pointer to the 6D orientation XH axis
 * @retval BSP status
 */
int32_t LSM6DSL_MOTION_SENSOR_Get_6D_Orientation_XH(uint8_t *xh)
{
  int32_t ret;

      if (LSM6DSL_ACC_Get_6D_Orientation_XH(MotionCompObj, xh) != BSP_ERROR_NONE)
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
 * @brief  Get 6D Orientation YL (available only for LSM6DSL sensor)
 * @param  yl the pointer to the 6D orientation YL axis
 * @retval BSP status
 */
int32_t LSM6DSL_MOTION_SENSOR_Get_6D_Orientation_YL(uint8_t *yl)
{
  int32_t ret;

      if (LSM6DSL_ACC_Get_6D_Orientation_YL(MotionCompObj, yl) != BSP_ERROR_NONE)
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
 * @brief  Get 6D Orientation YH (available only for LSM6DSL sensor)
 * @param  yh the pointer to the 6D orientation YH axis
 * @retval BSP status
 */
int32_t LSM6DSL_MOTION_SENSOR_Get_6D_Orientation_YH(uint8_t *yh)
{
  int32_t ret;

      if (LSM6DSL_ACC_Get_6D_Orientation_YH(MotionCompObj, yh) != BSP_ERROR_NONE)
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
 * @brief  Get 6D Orientation ZL (available only for LSM6DSL sensor)
 * @param  zl the pointer to the 6D orientation ZL axis
 * @retval BSP status
 */
int32_t LSM6DSL_MOTION_SENSOR_Get_6D_Orientation_ZL( uint8_t *zl)
{
  int32_t ret;

      if (LSM6DSL_ACC_Get_6D_Orientation_ZL(MotionCompObj, zl) != BSP_ERROR_NONE)
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
 * @brief  Get 6D Orientation ZH (available only for LSM6DSL sensor)
 * @param  zh the pointer to the 6D orientation ZH axis
 * @retval BSP status
 */
int32_t LSM6DSL_MOTION_SENSOR_Get_6D_Orientation_ZH(uint8_t *zh)
{
  int32_t ret;

 
      if (LSM6DSL_ACC_Get_6D_Orientation_ZH(MotionCompObj, zh) != BSP_ERROR_NONE)
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
 * @brief  Get step count (available only for LSM6DSL sensor)
 * @param  StepCount number of steps
 * @retval BSP status
 */
int32_t LSM6DSL_MOTION_SENSOR_Get_Step_Count( uint16_t *StepCount)
{
  int32_t ret;

      if (LSM6DSL_ACC_Get_Step_Count(MotionCompObj, StepCount) != BSP_ERROR_NONE)
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
 * @brief  Get the register value (available only for LSM6DSL, LSM303AGR sensors)
 * @param  Reg address to be read
 * @param  Data pointer where the value is written to
 * @retval BSP status
 */
int32_t LSM6DSL_MOTION_SENSOR_Read_Register(uint8_t Reg, uint8_t *Data)
{
  int32_t ret;

      if (LSM6DSL_Read_Reg(MotionCompObj, Reg, Data) != BSP_ERROR_NONE)
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
 * @brief  Set the register value (available only for LSM6DSL, LSM303AGR sensors)
 * @param  Reg address to be read
 * @param  Data value to be written
 * @retval BSP status
 */
int32_t LSM6DSL_MOTION_SENSOR_Write_Register(uint8_t Reg, uint8_t Data)
{
  int32_t ret;


      if (LSM6DSL_Write_Reg(MotionCompObj, Reg, Data) != BSP_ERROR_NONE)
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
 * @brief  Get number of unread FIFO samples (available only for LSM6DSL sensor)
 * @param  NumSamples number of unread FIFO samples
 * @retval BSP status
 */
int32_t LSM6DSL_MOTION_SENSOR_FIFO_Get_Num_Samples(uint16_t *NumSamples)
{
  int32_t ret;

      if (LSM6DSL_FIFO_Get_Num_Samples(MotionCompObj, NumSamples) != BSP_ERROR_NONE)
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
 * @brief  Get FIFO full status (available only for LSM6DSL sensor)
 * @param  Status FIFO full status
 * @retval BSP status
 */
int32_t LSM6DSL_MOTION_SENSOR_FIFO_Get_Full_Status(uint8_t *Status)
{
  int32_t ret;

      if (LSM6DSL_FIFO_Get_Full_Status(MotionCompObj, Status) != BSP_ERROR_NONE)
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
 * @brief  Set FIFO decimation (available only for LSM6DSL sensor)
 * @param  Function Motion sensor function. Could be:
 *         - MOTION_GYRO or MOTION_ACCELERO for instance LSM6DSL_LSM6DSL_0
 * @param  Decimation FIFO decimation
 * @retval BSP status
 */
int32_t LSM6DSL_MOTION_SENSOR_FIFO_Set_Decimation(uint32_t Function, uint8_t Decimation)
{
  int32_t ret;

      if ((Function & MOTION_ACCELERO) == MOTION_ACCELERO)
      {
        if (LSM6DSL_FIFO_ACC_Set_Decimation(MotionCompObj, Decimation) != BSP_ERROR_NONE)
        {
          ret = BSP_ERROR_COMPONENT_FAILURE;
        }
        else
        {
          ret = BSP_ERROR_NONE;
        }
      }
      else if ((Function & MOTION_GYRO) == MOTION_GYRO)
      {
        if (LSM6DSL_FIFO_GYRO_Set_Decimation(MotionCompObj, Decimation) != BSP_ERROR_NONE)
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
 * @brief  Set FIFO ODR value (available only for LSM6DSL sensor)
 * @param  Odr FIFO ODR value
 * @retval BSP status
 */
int32_t LSM6DSL_MOTION_SENSOR_FIFO_Set_ODR_Value(float Odr)
{
  int32_t ret;


      if (LSM6DSL_FIFO_Set_ODR_Value(MotionCompObj, Odr) != BSP_ERROR_NONE)
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
 * @brief  Set FIFO full interrupt on INT1 pin (available only for LSM6DSL sensor)
 * @param  Status FIFO full interrupt on INT1 pin
 * @retval BSP status
 */
int32_t LSM6DSL_MOTION_SENSOR_FIFO_Set_INT1_FIFO_Full(uint8_t Status)
{
  int32_t ret;


      if (LSM6DSL_FIFO_Set_INT1_FIFO_Full(MotionCompObj, Status) != BSP_ERROR_NONE)
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
 * @brief  Set FIFO watermark level (available only for LSM6DSL sensor)
 * @param  Watermark FIFO watermark level
 * @retval BSP status
 */
int32_t LSM6DSL_MOTION_SENSOR_FIFO_Set_Watermark_Level( uint16_t Watermark)
{
  int32_t ret;

      if (LSM6DSL_FIFO_Set_Watermark_Level(MotionCompObj, Watermark) != BSP_ERROR_NONE)
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
 * @brief  Set FIFO stop on watermark (available only for LSM6DSL sensor)
 * @param  Status FIFO stop on watermark status
 * @retval BSP status
 */
int32_t LSM6DSL_MOTION_SENSOR_FIFO_Set_Stop_On_Fth(uint8_t Status)
{
  int32_t ret;

      if (LSM6DSL_FIFO_Set_Stop_On_Fth(MotionCompObj, Status) != BSP_ERROR_NONE)
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
 * @brief  Set FIFO mode (available only for LSM6DSL sensor)
 * @param  Mode FIFO mode
 * @retval BSP status
 */
int32_t LSM6DSL_MOTION_SENSOR_FIFO_Set_Mode(uint8_t Mode)
{
  int32_t ret;


      if (LSM6DSL_FIFO_Set_Mode(MotionCompObj, Mode) != BSP_ERROR_NONE)
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
 * @brief  Get FIFO pattern (available only for LSM6DSL sensor)
 * @param  Pattern FIFO pattern
 * @retval BSP status
 */
int32_t LSM6DSL_MOTION_SENSOR_FIFO_Get_Pattern(uint16_t *Pattern)
{
  int32_t ret;

      if (LSM6DSL_FIFO_Get_Pattern(MotionCompObj, Pattern) != BSP_ERROR_NONE)
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
 * @brief  Get FIFO single axis data (available only for LSM6DSL sensor)
 * @param  Function Motion sensor function. Could be:
 *         - MOTION_GYRO or MOTION_ACCELERO for instance LSM6DSL_LSM6DSL_0
 * @param  Data FIFO single axis data
 * @retval BSP status
 */
int32_t LSM6DSL_MOTION_SENSOR_FIFO_Get_Axis(uint32_t Function, int32_t *Data)
{
  int32_t ret;

      if ((Function & MOTION_ACCELERO) == MOTION_ACCELERO)
      {
        if (LSM6DSL_FIFO_ACC_Get_Axis(MotionCompObj, Data) != BSP_ERROR_NONE)
        {
          ret = BSP_ERROR_COMPONENT_FAILURE;
        }
        else
        {
          ret = BSP_ERROR_NONE;
        }
      }
      else if ((Function & MOTION_GYRO) == MOTION_GYRO)
      {
        if (LSM6DSL_FIFO_GYRO_Get_Axis(MotionCompObj, Data) != BSP_ERROR_NONE)
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
 * @brief  Set accelero self-test (available only for LSM6DSL sensor)
 * @param  Function Motion sensor function. Could be:
 *         - MOTION_GYRO or MOTION_ACCELERO for instance LSM6DSL_LSM6DSL_0
 * @param  Mode self-test mode (0: disable self-test, 1: positive self-test, 2: negative self-test)
 * @retval BSP status
 */
int32_t LSM6DSL_MOTION_SENSOR_Set_SelfTest(uint32_t Function, uint8_t Mode)
{
  int32_t ret;

      if ((Function & MOTION_ACCELERO) == MOTION_ACCELERO)
      {
        if (LSM6DSL_ACC_Set_SelfTest(MotionCompObj, Mode) != BSP_ERROR_NONE)
        {
          ret = BSP_ERROR_COMPONENT_FAILURE;
        }
        else
        {
          ret = BSP_ERROR_NONE;
        }
      }
      else if ((Function & MOTION_GYRO) == MOTION_GYRO)
      {
        if (LSM6DSL_GYRO_Set_SelfTest(MotionCompObj, Mode) != BSP_ERROR_NONE)
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
