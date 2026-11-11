/****************************************************************************
*
* Copyright (c) 2023 STMicroelectronics - All Rights Reserved
*
* License terms: STMicroelectronics Proprietary in accordance with licensing
* terms SLA0098 at www.st.com.
*
* THIS SOFTWARE IS DISTRIBUTED "AS IS," AND ALL WARRANTIES ARE DISCLAIMED,
* INCLUDING MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE.
*****************************************************************************/
/**
*   @file    motor_power_measurement.h
*   @version M4_VERSION
*   @brief   This file provides firmware functions that implement the features
*            of the Motor Power Measurement component of the Stellar Motor Control.
*
*   @details This file provides firmware functions that implement the features
*            of the Motor Power Measurement component of the Stellar Motor Control:
*
*             - Calculate power of the motor
*             - Clear power measurement
*             - Get Power of the motor
*             - Get average Power of the motor
*
*   @ingroup motorpowermeasurement
 */
/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __MOTORPOWERMEASUREMENT_H
#define __MOTORPOWERMEASUREMENT_H

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

/* Includes ------------------------------------------------------------------*/
#include "mc_type.h"

/** @addtogroup STELLAR_PMSM_MC_FOC_LIBRARY
  * @{
  */

/** @addtogroup motorpowermeasurement
  * @{
  */

#define MPM_BUFFER_LENGHT 128u /*!< Length of buffer used to store the 
                                   instantaneous measurements of motor power.*/
typedef struct
{
  int16_t hMeasBuffer[MPM_BUFFER_LENGHT]; /*!< Buffer used by MPM object to
                                                 store the instantaneous
                                                  measurements of motor power. */
  uint16_t hNextMeasBufferIndex; /*!< Index of the buffer that will contain the
                                        next motor power measurement. */
  uint16_t hLastMeasBufferIndex; /*!< Index of the buffer that contains the last
                                        motor power measurement. */
  int16_t hAvrgElMotorPowerW; /*!< The average measured motor power expressed in
                                     watt. */
} MotorPowMeas_Handle_t;

/**
  * @brief  MotorPowerMeasurement class init struct definition
  */
typedef void * pMPMInitStruct_t;


/**
  * @brief  It should be called before each motor restart. It clears the
  *         measurement buffer and initialize the index.
  * @param power handle.
  * @retval none.
  */
void MPM_Clear( MotorPowMeas_Handle_t * pHandle );

/**
  * @brief  This method should be called with periodicity. It computes and
  *         returns the measured motor power expressed in watt. It is also used
  *         to fill, with that measure, the buffer used to compute the average
  *         motor power.
  * @param power handle.
  * @retval int16_t The measured motor power expressed in watt.
  */
int16_t MPM_CalcElMotorPower( MotorPowMeas_Handle_t * pHandle, int16_t MotorPower );

/**
  * @brief  This method is used to get the last measured motor power
  *         (instantaneous value) expressed in watt.
  * @param power handle.
  * @retval int16_t The last measured motor power (instantaneous value)
  *         expressed in watt.
  */
int16_t MPM_GetElMotorPowerW( MotorPowMeas_Handle_t * pHandle );

/**
  * @brief  This method is used to get the average measured motor power
  *         expressed in watt.
  * @param pHandle related component instance.
  * @retval int16_t The average measured motor power expressed in watt.
  */
int16_t MPM_GetAvrgElMotorPowerW( MotorPowMeas_Handle_t * pHandle );

/**
  * @}
  */

/**
  * @}
  */

#ifdef __cplusplus
}
#endif /* __cpluplus */

#endif /* __CCC_H */

/************************ (C) COPYRIGHT 2023 STMicroelectronics *****END OF FILE****/
