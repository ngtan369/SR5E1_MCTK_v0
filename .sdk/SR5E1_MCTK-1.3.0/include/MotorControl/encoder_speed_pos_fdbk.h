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
*   @file    encoder_speed_pos_fdbk.h
*   @version M4_VERSION
*   @brief   This file provides firmware functions that implement the following features
*            of the Encoder component of the Stellar Motor Control.
*
*   @details This file provides firmware functions that implement the following features
*            of the Encoder component of the Stellar Motor Control:
*
*           - computes & stores average mechanical speed
*           - computes & stores average mechanical acceleration
*           - computes & stores  the instantaneous electrical speed
*           - calculates the rotor electrical and mechanical angle
*
*   @ingroup SpeednPosFdbk
 */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __ENCODER_SPEEDNPOSFDBK_H
#define __ENCODER_SPEEDNPOSFDBK_H

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

/* Includes ------------------------------------------------------------------*/
#include "speed_pos_fdbk.h"
#include "parameters_conversion.h"

/** @addtogroup STELLAR_PMSM_MC_FOC_LIBRARY
  * @{
  */

/** @addtogroup SpeednPosFdbk
   * @{
   */

/** @addtogroup Encoder
 * @{
 */

/* Exported constants --------------------------------------------------------*/

#define GPIO_NoRemap_TIMx ((uint32_t)(0))
#define ENC_DMA_PRIORITY DMA_Priority_High
#define ENC_SPEED_ARRAY_SIZE  ((uint8_t)16)    /* 2^4 */

/**
  * @brief  ENCODER class parameters definition
  */
typedef struct
{
  SpeednPosFdbk_Handle_t _Super;
  
  TIM_TypeDef * TIMx;   /*!< Timer used for ENCODER sensor management.*/
  
  uint32_t SpeedSamplingFreqUnit; /*!< Frequency at which motor speed is to be
                                   computed. It must be equal to the frequency
                                   at which function SPD_CalcAvrgMecSpeedUnit
                                   is called.*/
  int32_t DeltaCapturesBuffer[ENC_SPEED_ARRAY_SIZE]; /*!< Buffer used to store
                                        captured variations of timer counter*/
                                        
  
  uint32_t U32MAXdivPulseNumber;       /*! <It stores U32MAX/hPulseNumber*/

  uint16_t SpeedSamplingFreqHz;        /*! <Frequency (Hz) at which motor speed
                                        is to be computed. */             
                                        
  /* SW Settings */
  uint16_t PulseNumber; /*!< Number of pulses per revolution, provided by each
                              of the two encoder signals, multiplied by 4 */
  
  volatile uint16_t TimerOverflowNb;    /*!< Number of overflows occurred since
                                        last speed measurement event*/
  uint16_t PreviousCapture;            /*!< Timer counter value captured during
                                        previous speed measurement event*/
 
  FunctionalState RevertSignal; /*!< To be enabled if measured speed is opposite
                                     to real one (ENABLE/DISABLE)*/

  uint8_t SpeedBufferSize; /*!< Size of the buffer used to calculate the average
                             speed. It must be <= 16.*/


  bool SensorIsReliable;            /*!< Flag to indicate sensor/decoding is not
                                         properly working.*/

  uint8_t ICx_Filter;                  /*!< Input Capture filter selection */

  volatile uint8_t DeltaCapturesIndex; /*! <Buffer index*/

  bool TimerOverflowError;              /*!< true if the number of overflow
                                        occurred is greater than 'define'
                                        ENC_MAX_OVERFLOW_NB*/

} ENCODER_Handle_t;


void * ENC_IRQHandler( void * pHandleVoid );
void ENC_Init( ENCODER_Handle_t * pHandle );
void ENC_Clear( ENCODER_Handle_t * pHandle );
int16_t ENC_CalcAngle( ENCODER_Handle_t * pHandle );
bool ENC_CalcAvrgMecSpeedUnit( ENCODER_Handle_t * pHandle, int16_t * pMecSpeedUnit );
void ENC_SetMecAngle( ENCODER_Handle_t * pHandle, int16_t hMecAngle );

/**
  * @}
  */

/**
  * @}
  */

/** @} */

#ifdef __cplusplus
}
#endif /* __cpluplus */

#endif /*__ENCODER_SPEEDNPOSFDBK_H*/

/******************* (C) COPYRIGHT 2023 STMicroelectronics *****END OF FILE****/
