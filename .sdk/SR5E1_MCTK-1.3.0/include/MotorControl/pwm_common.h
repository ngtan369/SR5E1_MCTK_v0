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
*   @file    pwm_common.h
*   @version M4_VERSION
*   @brief   This file provides firmware functions that implement the features
*            of the PWM common component of the Stellar Motor Control.
*
*   @details This file provides firmware functions that implement the features
*            of the PWM common component of the Stellar Motor Control.
*
*   @ingroup pwm_curr_fdbk
 */
/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __PWMNCOMMON_H
#define __PWMNCOMMON_H

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

/* Includes ------------------------------------------------------------------*/
#include "mc_type.h"
#define NB_CONVERSIONS 16u
/** @addtogroup STELLAR_PMSM_MC_FOC_LIBRARY
  * @{
  */

/** @addtogroup pwm_curr_fdbk
  * @{
  */

/* Exported defines ----------------------------------------------------------*/

/* Exported types ------------------------------------------------------------*/

/* Exported functions --------------------------------------------------------*/
/**
 * @brief  It perform the start of all the timers required by the control.
 *          It utilizes TIM2 as temporary timer to achieve synchronization between
 *          PWM signals.
 *          When this function is called, TIM1 and/or TIM8 must be in frozen state
 *          with CNT, ARR, REP RATE and trigger correctly set (these setting are
 *          usually performed in the Init method accordingly with the configuration)
 */
void startTimers( void );

/**
 * @brief  It waits for the end of the polarization. If the polarization exceeds the
 *         the number of needed PWM cycles, it reports an error.
 */
void waitForPolarizationEnd( TIM_TypeDef*  TIMx, uint16_t  *SWerror, uint8_t repCnt, volatile uint8_t *cnt );
/**
  * @}
  */

/**
  * @}
  */

#ifdef __cplusplus
}
#endif /* __cpluplus */

#endif /* __PWMNCOMMON_H */

/******************* (C) COPYRIGHT 2023 STMicroelectronics *****END OF FILE****/
