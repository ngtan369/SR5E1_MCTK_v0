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
*   @file    external_adc_out.c
*   @version M4_VERSION
*   @brief   This file provides firmware functions that implement the features
*            of the External ADC component of the Stellar Motor Control.
*          
*   @details This file provides firmware functions that implement the features
*            of the External ADC component of the Stellar Motor Control.
 */

/* Includes ------------------------------------------------------------------*/
#include "external_adc_out.h"
#include "mc_config.h"
#include "clock.h"
#include <test_env.h>
#include "gpio.h"

/** @addtogroup STELLAR_PMSM_MC_FOC_LIBRARY
  * @{
  */

/** @defgroup ExternalADCOut External ADC out
  * @brief External ADC out components of the Stellar Motor Control
  *
  * @{
  */
#if defined USE_VBUS_EXT_ADC_OUT || defined USE_TEMP_EXT_ADC_OUT
/* enable/disable debug */
#define EXT_ADC_OUT_DEBUG
uint32_t TimClockF;
#ifdef EXT_ADC_OUT_DEBUG
uint8_t i = 0;
#ifdef USE_VBUS_EXT_ADC_OUT
float Ext_ADC_Duty_VBUS;
uint32_t Frequency_VBUS;
#endif
#ifdef USE_TEMP_EXT_ADC_OUT
uint32_t Frequency_TEMP;
float Ext_ADC_Duty_TEMP;
#endif
#endif

/**
  * @brief L9502_ADCInputCaptureInit Initialization function.
  *        TIM chennnel as configured in combined mode
  *        Channel 1 has been selected in the PWM Input mode on channel 1
  *        Channel 2 is internally connected to the channel 1, and this is 
  *        to measure the duty cycle
  * @param Ext_ADC_OUT_t  External ADC configuration
  * @retval None
  */
void ExtADC_Init(Ext_ADC_OUT_t * pHandle)
{

  LL_TIM_InitTypeDef TIM_InitStruct = {0};
  TIM_TypeDef * TIMx = pHandle->TIMx;
  
  if(TIMx == TIM2)
  {
    /* Peripheral clock enable */
     clock_enable_TIM2(true);
#if defined USE_VBUS_EXT_ADC_OUT || defined USE_TEMP_EXT_ADC_OUT
    TimClockF = CLOCK_TIMCLK2;
#endif 
  }  
  if(TIMx == TIM3)
  {
    /* Peripheral clock enable */
    clock_enable_TIM3(true);
#if defined USE_VBUS_EXT_ADC_OUT || defined USE_TEMP_EXT_ADC_OUT
    TimClockF = CLOCK_TIMCLK3;
#endif 
  }
  if(TIMx == TIM4)
  {
    /* Peripheral clock enable */
    clock_enable_TIM4(true);
#if defined USE_VBUS_EXT_ADC_OUT || defined USE_TEMP_EXT_ADC_OUT
    TimClockF = CLOCK_TIMCLK4;
#endif  
  }
  if(TIMx == TIM5)
  {
    /* Peripheral clock enable */
    clock_enable_TIM5(true);
#if defined USE_VBUS_EXT_ADC_OUT || defined USE_TEMP_EXT_ADC_OUT
    TimClockF = CLOCK_TIMCLK5;
#endif 
  }  
  TIM_InitStruct.Prescaler = 0;
  TIM_InitStruct.CounterMode = LL_TIM_COUNTERMODE_UP;
  TIM_InitStruct.Autoreload = 65535;
  TIM_InitStruct.ClockDivision = LL_TIM_CLOCKDIVISION_DIV1;
  LL_TIM_Init(TIMx, &TIM_InitStruct);
  LL_TIM_DisableARRPreload(TIMx);
  LL_TIM_SetClockSource(TIMx, LL_TIM_CLOCKSOURCE_INTERNAL);
  if(pHandle->InputChannel == LL_TIM_CHANNEL_CH1)
  {
    LL_TIM_SetTriggerInput(TIMx, LL_TIM_TS_TI1FP1);
  }else if(pHandle->InputChannel == LL_TIM_CHANNEL_CH2)
  {
    LL_TIM_SetTriggerInput(TIMx, LL_TIM_TS_TI2FP2);
  }
  else {}
  LL_TIM_SetSlaveMode(TIMx, LL_TIM_SLAVEMODE_RESET);
  LL_TIM_CC_DisableChannel(TIMx, pHandle->InputChannel);
  /* Input Configuration Filter */
  LL_TIM_IC_SetFilter(TIMx, pHandle->InputChannel, pHandle->InputTimFilter);
  LL_TIM_IC_SetPolarity(TIMx, pHandle->InputChannel, LL_TIM_IC_POLARITY_RISING);
  LL_TIM_DisableIT_TRIG(TIMx);
  LL_TIM_DisableDMAReq_TRIG(TIMx);
  LL_TIM_IC_SetActiveInput(TIMx, pHandle->InputChannel, LL_TIM_ACTIVEINPUT_DIRECTTI);
  LL_TIM_IC_SetPrescaler(TIMx, pHandle->InputChannel, LL_TIM_ICPSC_DIV1);
  LL_TIM_IC_SetActiveInput(TIMx, pHandle->MeasuredChannel, LL_TIM_ACTIVEINPUT_INDIRECTTI);
  LL_TIM_IC_SetPrescaler(TIMx, pHandle->MeasuredChannel, LL_TIM_ICPSC_DIV1);
  /* Input Configuration Filter */
  LL_TIM_IC_SetFilter(TIMx, pHandle->MeasuredChannel, pHandle->InputTimFilter);
  LL_TIM_IC_SetPolarity(TIMx, pHandle->MeasuredChannel, LL_TIM_IC_POLARITY_FALLING);
  LL_TIM_SetTriggerOutput(TIMx, LL_TIM_TRGO_RESET);
  LL_TIM_DisableMasterSlaveMode(TIMx);
  
  /* enable interrupt */
  LL_TIM_EnableIT_CC1(TIMx);

  /* enable input capture */
  LL_TIM_CC_EnableChannel(TIMx,pHandle->InputChannel);  

  /* enable input capture */
  LL_TIM_CC_EnableChannel(TIMx,pHandle->MeasuredChannel); 

  /* Enable counter */
  LL_TIM_EnableCounter(TIMx);
}

/*
 * This function is used to read the result of a external ADC conversion.
 * @param Ext_ADC_OUT_t  External ADC configuration
 * @retval external ADC out value
 */
uint16_t ExtADC_GetADCValue(Ext_ADC_OUT_t * pHandle)
{
  return (uint16_t)pHandle->ADC_OUT_value;
}

uint16_t ExtADC_approx(Ext_ADC_OUT_t * pHandle, float value) {
  uint16_t retVal;
  if (value < 0.5f) {
	retVal = 0;
  } else if (value > (float) (pHandle->ADC_res-0.5f)) {
	retVal = pHandle->ADC_res;
  } else {
	uint16_t int_part = (uint16_t) (value);
	float decimal_part = value - int_part;
	uint16_t approx_int;
	if (decimal_part >= 0.5f) {
	  approx_int = int_part + 1;
	} else {
	  approx_int = int_part;
	}
	retVal = approx_int;
  }
  return retVal;
}

/**
  * @brief  IRQ implementation of the External ADC 
  * @param  pHandle: handler of the current instance of the External ADC component
  * @param  flag used to distinguish between various IRQ sources
  * @retval none
  */ 
void * ExtADC_IRQHandler( void * pHandleVoid )
{
  Ext_ADC_OUT_t * pHandle = ( Ext_ADC_OUT_t * ) pHandleVoid;
  static uint8_t first = 0;
  uint32_t ICValue1 = 0;
  uint32_t ICValue2 = 0;
  
  float duty = 0.0f;
  float value_f = 0.0f;
  float dutyMin = pHandle->ADC_min_duty;
  float dutyMax = pHandle->ADC_max_duty;
  uint16_t retVal = 0;
  /* Capture channel 1 */
  if(pHandle->InputChannel == LL_TIM_CHANNEL_CH1)
  {
  ICValue1 = LL_TIM_OC_GetCompareCH1(pHandle->TIMx);
  }
  else
  {
     ICValue1 = LL_TIM_OC_GetCompareCH2(pHandle->TIMx);
  }   

  if (first != 0)
  {
    /* Capture channel 2 (Signal high time) */
    if(pHandle->MeasuredChannel == LL_TIM_CHANNEL_CH1)
    {
       ICValue2 = LL_TIM_OC_GetCompareCH1(pHandle->TIMx);
	}
    else
    {
    ICValue2 = LL_TIM_OC_GetCompareCH2(pHandle->TIMx);
    }
#ifdef EXT_ADC_OUT_DEBUG 
#ifdef USE_VBUS_EXT_ADC_OUT
    if(pHandle->TIMx == VBUS_EXT_ADC_TIMER)
    {
	   Ext_ADC_Duty_VBUS = (float)ICValue2 / (float)ICValue1;
       Frequency_VBUS = TimClockF/ICValue1;
    }
#endif
#endif
#ifdef EXT_ADC_OUT_DEBUG 
#ifdef USE_TEMP_EXT_ADC_OUT
    if(pHandle->TIMx == TEMP_EXT_ADC_TIMER)
    {
	   Ext_ADC_Duty_TEMP = (float)ICValue2 / (float)ICValue1;
       Frequency_TEMP = TimClockF/ICValue1;
    }
#endif
#endif    
    duty = (float)ICValue2 / (float)ICValue1;
    value_f = ((duty - dutyMin) / (dutyMax - dutyMin)) * (float)(pHandle->ADC_res);
	retVal = ExtADC_approx(pHandle,value_f);  
    pHandle->ADC_OUT_value = retVal;
  }
  else
  {
     first = 1;
  }
  return MC_NULL;
}

#endif /* #if defined USE_VBUS_EXT_ADC_OUT || defined USE_TEMP_EXT_ADC_OUT */

/**
  * @}
  */

/**
  * @}
  */

/************************ (C) COPYRIGHT 2023 STMicroelectronics *****END OF FILE****/
