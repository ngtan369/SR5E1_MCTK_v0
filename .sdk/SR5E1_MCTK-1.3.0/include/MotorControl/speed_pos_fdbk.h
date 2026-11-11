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
*   @file    speed_pos_fdbk.h
*   @version M4_VERSION
*   @brief   This file provides firmware functions that implement the features
*            of the Speed & Position Feedback component of the Stellar Motor Control.
*
*   @details This file provides firmware functions that implement the features
*            of the Speed & Position Feedback component of the Stellar Motor Control.
*
*   @ingroup SpeednPosFdbk
 */
/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __SPEEDNPOSFDBK_H
#define __SPEEDNPOSFDBK_H

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

/* Includes ------------------------------------------------------------------*/
#include "stdint.h"
#include "mc_type.h"

/** @addtogroup STELLAR_PMSM_MC_FOC_LIBRARY
  * @{
  */

/** @addtogroup SpeednPosFdbk
  * @{
  */

/* Exported types ------------------------------------------------------------*/
/**
  * @brief  SpeednPosFdbk  handle definition
  */
typedef struct
{

  uint8_t bSpeedErrorNumber;

  uint8_t bElToMecRatio;  /*!< Coefficient used to transform electrical to
                               mechanical quantities and viceversa. It usually
                               coincides with motor pole pairs number*/
  uint8_t SpeedUnit; /*!< The speed unit value is defined into mc_stm_types.h*/   
  
  uint8_t bMaximumSpeedErrorsNumber; /*!< Maximum value of not valid measurements
                                        before an error is reported.*/    
  int16_t hElAngle;

  int16_t hMecAngle;
  
  int32_t wMecAngle;

  int16_t hAvrMecSpeedUnit;

  int16_t hElSpeedDpp;
  int16_t InstantaneousElSpeedDpp;

  int16_t hMecAccelUnitP;
                            
  uint16_t hMaxReliableMecSpeedUnit; /*!< Maximum value of measured mechanical speed that is
                                        considered to be valid. Expressed
                                        in the unit defined by #SPEED_UNIT.*/
  uint16_t hMinReliableMecSpeedUnit; /*!< Minimum value of measured mechanical speed that is
                                        considered to be valid. Expressed
                                        in the unit defined by #SPEED_UNIT.*/
  uint16_t hMaxReliableMecAccelUnitP; /*!< Maximum value of measured acceleration
                                        that is considered to be valid. Expressed in
                                        the unit defined by #SPEED_UNIT */
  uint16_t hMeasurementFrequency;  /*!< Frequency at which the user will request
                                    a measurement of the rotor electrical angle. Expressed in PWM_FREQ_SCALING*Hz.
                                    It is also used to convert measured speed from the unit
                                    defined by #SPEED_UNIT to dpp and viceversa.*/
  uint32_t DPPConvFactor; /* (65536/PWM_FREQ_SCALING) */
  

} SpeednPosFdbk_Handle_t;

/**
  * @brief input structure type definition for SPD_CalcAngle
  */
typedef struct
{
  alphabeta_t  Valfa_beta;
  alphabeta_t  Ialfa_beta;
  uint16_t         Vbus;
} Observer_Inputs_t;


int16_t SPD_GetElAngle( SpeednPosFdbk_Handle_t * pHandle );

int32_t SPD_GetMecAngle( SpeednPosFdbk_Handle_t * pHandle );

int16_t SPD_GetAvrgMecSpeedUnit( SpeednPosFdbk_Handle_t * pHandle );

int16_t SPD_GetElSpeedDpp( SpeednPosFdbk_Handle_t * pHandle );

int16_t SPD_GetInstElSpeedDpp ( SpeednPosFdbk_Handle_t * pHandle );

bool SPD_Check( SpeednPosFdbk_Handle_t * pHandle );

bool SPD_IsMecSpeedReliable( SpeednPosFdbk_Handle_t * pHandle, int16_t * pMecSpeedUnit );

int16_t SPD_GetS16Speed( SpeednPosFdbk_Handle_t * pHandle );

uint8_t SPD_GetElToMecRatio( SpeednPosFdbk_Handle_t * pHandle );

void SPD_SetElToMecRatio( SpeednPosFdbk_Handle_t * pHandle, uint8_t bPP );

/**
  * @}
  */

/**
  * @}
  */

#ifdef __cplusplus
}
#endif /* __cpluplus */

#endif /* __SPEEDNPOSFDBK_H */

/************************ (C) COPYRIGHT 2023 STMicroelectronics *****END OF FILE****/
