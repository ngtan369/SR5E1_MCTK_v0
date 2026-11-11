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
*   @file    pwm_common.c
*   @version M4_VERSION
*   @brief   This file provides firmware functions that implement the features
*            of the PWM common component of the Stellar Motor Control.
*
*   @details This file provides firmware functions that implement the features
*            of the PWM common component of the Stellar Motor Control.
*
 */

/* Includes ------------------------------------------------------------------*/
#include "pwm_common.h"
#include "clock.h"

/** @addtogroup STELLAR_PMSM_MC_FOC_LIBRARY
  * @{
  */

/** @defgroup pwm_curr_fdbk PWM & Current Feedback
  *
  * @brief PWM & Current Feedback components of the Stellar Motor Control
  *
  * These components fulfill two functions in a Motor Control subsystem:
  *
  * - The generation of the Space Vector Pulse Width Modulation on the motor's phases
  * - The sampling of the actual motor's phases current
  *
  * Both these features are closely related as the instants when the values of the phase currents
  * should be sampled by the ADC channels are basically triggered by the timers used to generate
  * the duty cycles for the PWM.
  *
  * Several implementation of PWM and Current Feedback components are provided by the Motor Control
  * SDK to account for the specificities of the application:
  *
  * - The selected MCU: the number of ADCs available on a given MCU, the presence of internal
  * compoarators or OpAmps, for instance, lead to different implementation of this feature
  * - The Current sensing topology also has an impact on the firmware: implementations are provided
  * for Insulated Current Sensors, Single Shunt and Three Shunt resistors current sensing topologies
  *
  * The choice of the implementation mostly depend on these two factors and is performed by the
  * Motor Control Workbench tool.
  *
  * All these implementations are built on a base PWM & Current Feedback component that they extend
  * and that provides the functions and data that are common to all of them. This base component is
  * never used directly as it does not provide a complete implementation of the features. Rather,
  * its handle structure (PWMC_Handle) is reused by all the PWM & Current Feedback specific
  * implementations and the functions it provides form the API of the PWM and Current feedback feature.
  * Calling them results in calling functions of the component that actually implement the feature.
  * See PWMC_Handle for more details on this mechanism.
  * @{
  */

#ifdef TIM2
/**
 * @brief  It perform the start of all the timers required by the control.
 *          It utilizes TIM2 as temporary timer to achieve synchronization between
 *          PWM signals.
 *          When this function is called, TIM1 and/or TIM8 must be in frozen state
 *          with CNT, ARR, REP RATE and trigger correctly set (these setting are
 *          usually performed in the Init method accordingly with the configuration)
 * @param  none
 * @retval none
 */
__weak void startTimers( void )
{
  uint32_t isTIM2ClockOn;
  uint32_t trigOut;

  isTIM2ClockOn = (uint32_t)is_clock_enable_TIM2();
  if ( isTIM2ClockOn == 0 )
  {
    /* Temporary Enable TIM2 clock if not already on */
    clock_enable_TIM2(true);
    LL_TIM_GenerateEvent_UPDATE ( TIM2 );
    clock_disable_TIM2();
  }
  else
  {
    trigOut = LL_TIM_ReadReg( TIM2, CR2 ) & TIM_CR2_MMS;
    LL_TIM_SetTriggerOutput( TIM2, LL_TIM_TRGO_UPDATE );
    LL_TIM_GenerateEvent_UPDATE ( TIM2 );
    LL_TIM_SetTriggerOutput( TIM2, trigOut );
  }
}
#endif

/**
 * @brief  It waits for the end of the polarization. If the polarization exceeds the
 *         the number of needed PWM cycles, it reports an error.
 * @param  TIMx: timer used to generate PWM
 *         SWerror: variable used to report a SW error
 *         repCnt: repetition counter value
 *         cnt: polarization counter value
 * @retval none
 */
__weak void waitForPolarizationEnd( TIM_TypeDef*  TIMx, uint16_t  *SWerror, uint8_t repCnt, volatile uint8_t *cnt )
{
  uint16_t hCalibrationPeriodCounter;
  uint16_t hMaxPeriodsNumber;

  if ( (LL_TIM_GetCounterMode(TIMx) & LL_TIM_COUNTERMODE_CENTER_UP_DOWN) == LL_TIM_COUNTERMODE_CENTER_UP_DOWN)
  {
    hMaxPeriodsNumber=(2*NB_CONVERSIONS)*(((uint16_t)repCnt+1u));
  }
  else
  {
	hMaxPeriodsNumber=(2*NB_CONVERSIONS)*(((uint16_t)repCnt+1u)>>1);
  }

  /* Wait for NB_CONVERSIONS to be executed */
  LL_TIM_ClearFlag_CC1(TIMx);
  hCalibrationPeriodCounter = 0u;
  while (*cnt < NB_CONVERSIONS)
  {
    if (LL_TIM_IsActiveFlag_CC1(TIMx))
    {
      LL_TIM_ClearFlag_CC1(TIMx);
      hCalibrationPeriodCounter++;
      if (hCalibrationPeriodCounter >= hMaxPeriodsNumber)
      {
        if (*cnt < NB_CONVERSIONS)
        {
          *SWerror = 1u;
          break;
        }
      }
    }
  }
}

/**
  * @}
  */

/**
  * @}
  */

/************************ (C) COPYRIGHT 2023 STMicroelectronics *****END OF FILE****/
