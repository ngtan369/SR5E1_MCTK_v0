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
*   @file    ramp_ext_mngr.h
*   @version M4_VERSION
*   @brief   This file provides firmware functions that implement the features
*            of the Ramp Extended Manager component of the Stellar Motor Control.
*
*   @details This file provides firmware functions that implement the features
*            of the Ramp Extended Manager component of the Stellar Motor Control.
*
*   @ingroup RampExtMngr
 */
/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __RAMPEXTMNGR_H
#define __RAMPEXTMNGR_H

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

/* Includes ------------------------------------------------------------------*/
#include "mc_type.h"

#ifdef FASTDIV
#include "fast_div.h"
#endif

/** @addtogroup STELLAR_PMSM_MC_FOC_LIBRARY
  * @{
  */

/** @addtogroup RampExtMngr
  * @{
  */

/**
  * @brief  RampExtMngr Handle Definition.
  */
typedef struct
{
  uint32_t FrequencyHz;             /*!< Execution frequency expressed in Hz */
  int32_t  TargetFinal;             /*!< Backup of hTargetFinal to be applied in the
                                         last step.*/
  int32_t  Ext;                     /*!< Current state variable multiplied by 32768.*/
  uint32_t RampRemainingStep;       /*!< Number of steps remaining to complete the
                                         ramp.*/
  int32_t  IncDecAmount;            /*!< Increment/decrement amount to be applied to
                                         the reference value at each
                                         CalcTorqueReference.*/
  uint32_t ScalingFactor;           /*!< Scaling factor between output value and
                                         its internal representation.*/
#ifdef FASTDIV
  /* (Fast division optimization for cortex-M0 micros)*/
  FastDiv_Handle_t fd;                       /*!< Fast division obj.*/
#endif
} RampExtMngr_Handle_t;

/* Exported functions ------------------------------------------------------- */
void REMNG_Init( RampExtMngr_Handle_t * pHandle );
int32_t REMNG_Calc( RampExtMngr_Handle_t * pHandle );
bool REMNG_ExecRamp( RampExtMngr_Handle_t * pHandle, int32_t TargetFinal, uint32_t Durationms );
int32_t REMNG_GetValue( RampExtMngr_Handle_t * pHandle );
bool REMNG_RampCompleted( RampExtMngr_Handle_t * pHandle );
void REMNG_StopRamp( RampExtMngr_Handle_t * pHandle );

/**
  * @}
  */

/**
  * @}
  */

#ifdef __cplusplus
}
#endif /* __cpluplus */

#endif /* __RAMPEXTMNGR_H */

/************************ (C) COPYRIGHT 2023 STMicroelectronics *****END OF FILE****/

