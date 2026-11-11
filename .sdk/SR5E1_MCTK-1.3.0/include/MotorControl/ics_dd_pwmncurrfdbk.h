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
*   @file    ics_dd_pwmncurrfdbk.h
*   @version M4_VERSION
*   @brief   This file contains all definitions and functions prototypes for the
*            ICS DD PWM current feedback component of the Stellar Motor Control.
*
*   @details This file contains all definitions and functions prototypes for the
*            ICS DD PWM current feedback component of the Stellar Motor Control.
*
*   @ingroup PWMnCurrFdbk_ICS_DD
 */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __ICS_DD_PWMNCURRFDBK_H
#define __ICS_DD_PWMNCURRFDBK_H

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

/** @addtogroup STELLAR_PMSM_MC_FOC_LIBRARY
  * @{
  */

/** @addtogroup pwm_curr_fdbk
  * @{
  */

/** @defgroup PWMnCurrFdbk_ICS_DD Insulated Current Sensing Parameters
 *
 *  @brief Common definitions for Insulated Current Sensors based PWM & Current Feedback components
  * @{
  */

#define GPIO_NoRemap_TIM1 ((uint32_t)(0))
#define SHIFTED_TIMs      ((uint8_t) 1)
#define NO_SHIFTED_TIMs   ((uint8_t) 0)


/**
  * @brief  ICS_DD parameters definition
  */
typedef struct
{
  /* Dual MC parameters --------------------------------------------------------*/
  uint8_t InstanceNbr;              /**< Instance number with reference to PWMC
                                          base class. It is necessary to properly
                                          synchronize TIM8 with TIM1 at
                                          peripheral initializations */
  uint16_t Tw;                       /**< It is used for switching the context
                                           in dual MC. It contains biggest delay
                                           (expressed in counter ticks) between
                                           the counter crest and ADC latest
                                           trigger  */
  uint8_t  FreqRatio;               /**< It is used in case of dual MC to
                                           synchronize TIM1 and TIM8. It has
                                           effect only on the second instanced
                                           object and must be equal to the
                                           ratio between the two PWM frequencies
                                           (higher/lower). Supported values are
                                           1, 2 or 3 */
  uint8_t  IsHigherFreqTim;         /**< When bFreqRatio is greather than 1
                                          this param is used to indicate if this
                                          instance is the one with the highest
                                          frequency. Allowed value are: HIGHER_FREQ
                                          or LOWER_FREQ */
  /* Current reading A/D Conversions initialization -----------------------------*/
  uint8_t IaChannel;                  /**< ADC channel used for conversion of
                                           current Ia. It must be equal to
                                           ADC_CHANNEL_x x= 0, ..., 15*/
  uint8_t IbChannel;                  /**< ADC channel used for conversion of
                                           current Ib. It must be equal to
                                           ADC_CHANNEL_x x= 0, ..., 15*/

  /* PWM generation parameters --------------------------------------------------*/
  uint8_t  RepetitionCounter;         /**< It expresses the number of PWM
                                           periods to be elapsed before compare
                                           registers are updated again. In
                                           particular:
                                           RepetitionCounter= (2* #PWM periods)-1
                                         */
  TIM_TypeDef * TIMx;                   /**< It contains the pointer to the timer
                                           used for PWM generation. It must
                                           equal to TIM1 if bInstanceNbr is
                                           equal to 1, to TIM8 otherwise */
  /* PWM Driving signals initialization ----------------------------------------*/
  LowSideOutputsFunction_t LowSideOutputs; /**< Low side or enabling signals
                                                generation method are defined
                                                here.*/
  GPIO_TypeDef * pwm_en_u_port;           /**< enable signal phase U port.*/
  uint32_t      pwm_en_u_pin;             /**< enable signal phase U pin.*/
  GPIO_TypeDef * pwm_en_v_port;           /**< enable signal phase V port.*/
  uint32_t      pwm_en_v_pin;             /**< enable signal phase V pin.*/
  GPIO_TypeDef * pwm_en_w_port;           /**< enable signal phase W port.*/
  uint32_t      pwm_en_w_pin;             /**< enable signal phase W pin.*/

  /* Emergency input signal initialization -------------------------------------*/
  FunctionalState EmergencyStop;        /**< It enable/disable the management of
                                            an emergency input instantaneously
                                            stopping PWM generation. It must be
                                            either equal to ENABLE or DISABLE */
} ICS_Params_t;
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
#endif /* __cpluplus */

#endif /*__ICS_DD_PWMNCURRFDBK_H*/
/************************ (C) COPYRIGHT 2023 STMicroelectronics *****END OF FILE****/
