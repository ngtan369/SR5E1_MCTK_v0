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
*   @file    open_loop.h
*   @version M4_VERSION
*   @brief   This file provides firmware functions that implement the features
*            of the Open Loop component of the Stellar Motor Control.
*
*   @details This file provides firmware functions that implement the features
*            of the Open Loop component of the Stellar Motor Control.
*
*   @ingroup OpenLoop
 */
/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __OPENLOOPCLASS_H
#define __OPENLOOPCLASS_H

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

/* Includes ------------------------------------------------------------------*/
#include "mc_type.h"
#include "virtual_speed_sensor.h"

/** @addtogroup STELLAR_PMSM_MC_APPLICATION
  * @{
  */

/** @addtogroup OpenLoop
  * @{
  */

/**
  * @brief  OpenLoop_Handle_t structure used for phases definition
  */
typedef struct
{
  int16_t hDefaultVoltage; /**< Default Open loop phase voltage. */

  bool VFMode;             /**< Flag to enable Voltage versus Frequency mode (V/F mode). */

  int16_t hVFOffset;       /**< Minimum Voltage to apply when frequency is equal to zero. */

  int16_t hVFSlope;        /**< Slope of V/F curve: Voltage = (hVFSlope)*Frequency + hVFOffset. */

  int16_t hVoltage;        /**< Current Open loop phase voltage. */

  VirtualSpeedSensor_Handle_t * pVSS; /**< Allow access on mechanical speed measured. */

} OpenLoop_Handle_t;

/**
* @}
*/

/* Exported functions ------------------------------------------------------- */

/**
  * @brief  Initialize OpenLoop variables.
  * @param  pHandle: Pointer on Handle structure of OpenLoop feature.
  * @param  pVSS: Pointer on virtual speed sensor structure.
  *  @retval none
  */
void OL_Init( OpenLoop_Handle_t * pHandle, VirtualSpeedSensor_Handle_t * pVSS );

/**
  * @brief  Set Vqd according to open loop phase voltage.
  * @param  pHandle: Pointer on Handle structure of OpenLoop feature.
  *  @retval Voltage components Vqd conditioned values.
  */
qd_t OL_VqdConditioning( OpenLoop_Handle_t * pHandle );

/**
  * @brief  Allow to set new open loop phase voltage.
  * @param  pHandle: Pointer on Handle structure of OpenLoop feature.
  * @param  hNewVoltage: New voltage value to apply.
  * @retval None
  */
void OL_UpdateVoltage( OpenLoop_Handle_t * pHandle, int16_t hNewVoltage );

/**
  * @brief  Compute phase voltage to apply according to average mechanical speed (V/F Mode).
  * @param  pHandle: Pointer on Handle structure of OpenLoop feature.
  * @retval None
  */
void OL_Calc( OpenLoop_Handle_t * pHandle );

/**
  * @brief  Allow activation of the Voltage versus Frequency mode (V/F mode).
  * @param  pHandle: Pointer on Handle structure of OpenLoop feature.
  * @param  VFEnabling: Flag to enable the V/F mode.
  * @retval None
  */
void OL_VF( OpenLoop_Handle_t * pHandle, bool VFEnabling );

/**
  * @}
  */

/**
  * @}
  */

#ifdef __cplusplus
}
#endif /* __cpluplus */

#endif /* __OPENLOOPCLASS_H */

/************************ (C) COPYRIGHT 2023 STMicroelectronics *****END OF FILE****/
