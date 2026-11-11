/****************************************************************************
 *
 * Copyright (c) 2023-2026 STMicroelectronics - All Rights Reserved
 *
 * License terms: STMicroelectronics Proprietary in accordance with licensing
 * terms SLA0098 at www.st.com.
 *
 * THIS SOFTWARE IS DISTRIBUTED "AS IS," AND ALL WARRANTIES ARE DISCLAIMED,
 * INCLUDING MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE.
 *****************************************************************************/
 /**
 *   @file    parameters_conversion_SR5E1.h
 *   @version 1.1.0
 *   @brief   This file contains the SR5E1 parameters needed for the Stellar
 *            Motor Control.
 *
 *
 */
/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __PARAMETERS_CONVERSION_SR5E1_H
#define __PARAMETERS_CONVERSION_SR5E1_H

#include "pmsm_motor_parameters.h"
#include "drive_parameters.h"
#include "power_stage_parameters.h"
#include "mc_math.h"


#define SYSCLK_FREQ       300000000uL 
#define TIM_CLOCK_DIVIDER  1
#define ADV_TIM_CLK_MHz   300
#define ADC_CLK_MHz     42
#define HALL_TIM_CLK    150000000uL
#define APB1TIM_FREQ    150000000uL

/*************************  IRQ Handler Mapping  *********************/
#define TIMx_UP_M1_IRQHandler IRQ_TIM1_UP_HANDLER

#define TIMx_BRK_M1_IRQHandler IRQ_TIM1_BRK_HANDLER
 
#define IRQ_TIMx_BRK_VECTOR    IRQ_TIM1_BRK_VECTOR
                               
#define IRQ_TIMx_UP_VECTOR     IRQ_TIM1_UP_VECTOR	

#define ADC_TRIG_CONV_LATENCY_CYCLES 3.5
#define ADC_SAR_CYCLES 12.5

#define M1_VBUS_SW_FILTER_BW_FACTOR     6u

#if defined(STATE_OBSERVER_CORDIC) || defined(AUX_STATE_OBSERVER_CORDIC)
#error "Observer Cordic sensor not available in this version"
#endif

#if defined(HFINJECTION) || defined(AUX_HFINJECTION)
#error "HFINJECTION sensor not available in this version"
#endif

#if defined(EXTERNAL_SENSOR) || defined(AUX_EXTERNAL_SENSOR)
#error "External sensor not available in this version"
#endif

#if defined(STATE_OBSERVER_PLL) && defined(AUX_STATE_OBSERVER_PLL)
#error "Observer PLL defined as main and auxliary sensor"
#endif

#if defined(STATE_OBSERVER_CORDIC) && defined(AUX_STATE_OBSERVER_CORDIC)
#error "Observer Cordic defined as main and auxliary sensor"
#endif

#if defined(HFINJECTION) && defined(AUX_HFINJECTION)
#error "HFINJECTION sensor defined as main and auxliary sensor"
#endif

#if defined(ENCODER) && defined(AUX_ENCODER)
#error "ENCODER sensor defined as main and auxliary sensor"
#endif

#if defined(HALL_SENSORS) && defined(AUX_HALL_SENSORS)
#error "Hall sensor defined as main and auxliary sensor"
#endif

#if defined(RESOLVER) && defined(AUX_RESOLVER)
#error "Resolver defined as main and auxliary sensor"
#endif

#if defined(EXTERNAL_SENSOR) && defined(AUX_EXTERNAL_SENSOR)
#error "External sensor defined as main and auxliary sensor"
#endif

#endif /*__PARAMETERS_CONVERSION_SR5E1_H*/
