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
*   @file    ics_SR5E1_pwm_curr_fdbk.c
*   @version M4_VERSION
*   @brief   This file contains the functions that implement the features of
*            single motor ICS current reading.
*
*   @details This file contains the functions that implement the features of
*            single motor ICS current reading.
 */

/* Includes ------------------------------------------------------------------*/
#include <test_env.h>
#include "ics_SR5E1_pwm_curr_fdbk.h"
#include "pwm_common.h"
#include "mc_type.h"
#include "clock.h"
#include "mc_config.h"
#include "parameters_conversion.h"
#include "gpio.h"


/** @addtogroup STELLAR_PMSM_MC_FOC_LIBRARY
  * @{
  */

/** @addtogroup pwm_curr_fdbk
  * @{
  */


/* #define ICS_SENSING_ON_UW_PHASES */

/**
 * @defgroup ICS_SR5E1_pwm_curr_fdbk ICS 2 ADCs PWM & Current Feedback
 *
 * @brief SR5E1, Shared Resources, ICS PWM & Current Feedback implementation
 *
 * This component is used in applications based on an SR5E1 MCU, using an ICS
 * current sensing topology and 2 ADC peripherals to acquire the current values.
 *
 *
 * @{
 */

/* Private defines -----------------------------------------------------------*/
#define TIMxCCER_MASK_CH123        ((uint16_t)  (TIMx_UH_CHANNEL|TIMx_UL_CHANNEL|\
                                                 TIMx_VH_CHANNEL|TIMx_VL_CHANNEL|\
                                                 TIMx_WH_CHANNEL|TIMx_WL_CHANNEL))

/* Private typedef -----------------------------------------------------------*/

/* Private function prototypes -----------------------------------------------*/
static void ICS_TIMx_Init(TIM_TypeDef * TIMx);
static void ICS_PWM_Init( TIM_TypeDef * TIMx, PWMC_Handle_t * pHdl );
static void ICS_ADC_Init(ADC_TypeDef * ADCx, uint8_t ADC_Channel, TIM_TypeDef * TIMx);
static void ICS_ADC_ReGConv_Init(ADC_TypeDef * ADCx);
static void ICS_ADCxEnable( ADC_TypeDef * ADCx );
static void ICS_HFCurrentsPolarization( PWMC_Handle_t * pHdl,ab_t * Iab );

static const uint8_t ADC_ENABLE_INIT[5] = {INIT_SARADC_UNIT_1,INIT_SARADC_UNIT_2,INIT_SARADC_UNIT_3,INIT_SARADC_UNIT_4,INIT_SARADC_UNIT_5};

/**
  * @brief  It initializes TIMx, ADC for current reading
  *         in ICS topology using SR5E1 and shared ADC
  * @param  pHandle: handler of the current instance of the PWM component
  * @retval none
  */
__weak void ICS_Init( PWMC_ICS_Handle_t * pHandle )
{
  TIM_TypeDef * TIMx = pHandle->pParams_str->TIMx;
  ADC_TypeDef * ADCx_1 = pHandle->pParams_str->ADCx_1;
  ADC_TypeDef * ADCx_2 = pHandle->pParams_str->ADCx_2;
  uint8_t bIndexAdc = 0;

  ICS_TIMx_Init(TIMx);
  ICS_ADC_Init(ADCx_1,PHASE_IA_CURR_CHANNEL,TIMx);
  ICS_ADC_Init(ADCx_2,PHASE_IB_CURR_CHANNEL,TIMx);
 
  /* Initialize ADC Unit for VBUS/TEMP/USER ADC (eventually) */
  for(bIndexAdc = 0U; bIndexAdc < MAX_ADC_MODULE; bIndexAdc++){
    if(ADC_ENABLE_INIT[bIndexAdc] == TRUE)
    {
      switch ( bIndexAdc )
      {
        case 0: /* ADC 1 */
        {
          ICS_ADC_ReGConv_Init(ADC1);
        }
        break;
        case 1: /* ADC 2 */
        {
          ICS_ADC_ReGConv_Init(ADC2);
        }
        break;
        case 2: /* ADC 3 */
        {
          ICS_ADC_ReGConv_Init(ADC3);
        }
        break;
        case 3: /* ADC 4 */
        {
          ICS_ADC_ReGConv_Init(ADC4);
        }
        break;
        case 4: /* ADC 5 */
        {
          ICS_ADC_ReGConv_Init(ADC5);
        }
        break;
        default:
          break;
      }
    }
  }
  
  /*Check that _Super is the first member of the structure PWMC_ICS_Handle_t */
  if ( ( uint32_t )pHandle == ( uint32_t )&pHandle->_Super )
  {
    /* disable IT and flags in case of LL driver usage
     * workaround for unwanted interrupt enabling done by LL driver */
    LL_ADC_DisableIT_EOC( ADCx_1 );
    LL_ADC_ClearFlag_EOC( ADCx_1 );
    LL_ADC_DisableIT_JEOC( ADCx_1 );
    LL_ADC_ClearFlag_JEOC( ADCx_1 );
    LL_ADC_DisableIT_EOC( ADCx_2 );
    LL_ADC_ClearFlag_EOC( ADCx_2 );
    LL_ADC_DisableIT_JEOC( ADCx_2 );
    LL_ADC_ClearFlag_JEOC( ADCx_2 );

    if ( TIMx == TIM1 )
    {
      /* TIM1 Counter Clock stopped when the core is halted */
      DBGMCU->APB2_C1_FZ_REG |= DBGMCU_APB2_C1_FZ_REG_TIM1_PWM_FRZ;
    }
    else
    {
      /* TIM8 Counter Clock stopped when the core is halted */
      DBGMCU->APB2_C1_FZ_REG |= DBGMCU_APB2_C1_FZ_REG_TIM8_PWM_FRZ;
    }
    

    if (LL_ADC_IsEnabled (ADCx_1) == 0)
    {
      ICS_ADCxEnable (ADCx_1);
      /* Only the Interrupt of the first ADC is enabled. 
       * As Both ADCs are fired by HW at the same moment 
       * It is safe to consider that both conversion are ready at the same time*/
      LL_ADC_ClearFlag_JEOS( ADCx_1 );
      LL_ADC_EnableIT_JEOS( ADCx_1 );
    }
    else 
    {
      /* Nothing to do ADCx_1 already configured */
    }
    if (LL_ADC_IsEnabled (ADCx_2) == 0)
    {
      ICS_ADCxEnable (ADCx_2);
    }    
    else 
    {
      /* Nothing to do ADCx_2 already configured */
    }
    ICS_PWM_Init( TIMx, &pHandle->_Super );
  }
}

static void ICS_ADCxEnable( ADC_TypeDef * ADCx )
{
  /* - Exit from deep-power-down mode */     
  LL_ADC_DisableDeepPowerDown(ADCx);
   
  if ( LL_ADC_IsInternalRegulatorEnabled(ADCx) == 0u)
  {
    /* Enable ADC internal voltage regulator */
    LL_ADC_EnableInternalRegulator(ADCx);
  
    /* Wait for Regulator Startup time, once for both */
    /* Note: Variable divided by 2 to compensate partially              */
    /*       CPU processing cycles, scaling in us split to not          */
    /*       exceed 32 bits register capacity and handle low frequency. */
    volatile uint32_t wait_loop_index = ((LL_ADC_DELAY_INTERNAL_REGUL_STAB_US / 10UL) * (SystemCoreClock / (100000UL * 2UL)));      
    while(wait_loop_index != 0UL)
    {
      wait_loop_index--;
    }
  }
  LL_SYSCFG_EnableADCGlobalBias();
  LL_ADC_StartCalibration( ADCx, LL_ADC_SINGLE_ENDED );
  while ( LL_ADC_IsCalibrationOnGoing( ADCx) == 1u) 
  {}
  /* ADC Enable (must be done after calibration) */
  /* ADC5-140924: Enabling the ADC by setting ADEN bit soon after polling ADCAL=0 
  * following a calibration phase, could have no effect on ADC 
  * within certain AHB/ADC clock ratio.
  */
  while (  LL_ADC_IsActiveFlag_ADRDY( ADCx ) == 0u)  
  { 
    LL_ADC_Enable(  ADCx );
  }
  /* Clear JSQR from CubeMX setting to avoid not wanting conversion*/
  LL_ADC_INJ_StartConversion( ADCx ); 
  LL_ADC_INJ_StopConversion(ADCx);
  /* TODO: check if not already done by MX */
  LL_ADC_INJ_SetQueueMode( ADCx, LL_ADC_INJ_QUEUE_2CONTEXTS_END_EMPTY );

  /* dummy conversion (ES0431 doc chap. 2.5.4) */
  LL_ADC_REG_StartConversion(ADCx);
 }


/**
  * @brief TIMx Initialization Function
  * @param None
  * @retval None
  */
static void ICS_TIMx_Init(TIM_TypeDef * TIMx)
{
  LL_TIM_InitTypeDef TIM_InitStruct = {0};
  LL_TIM_OC_InitTypeDef TIM_OC_InitStruct = {0};
  LL_TIM_BDTR_InitTypeDef TIM_BDTRInitStruct = {0};
  if(TIMx == TIM1)
  {
    /* Peripheral clock enable */
    clock_enable_TIM1(true);
  }  
  if(TIMx == TIM8)
  {
    /* Peripheral clock enable */
    clock_enable_TIM8(true);
  }
  TIM_InitStruct.Prescaler = ((TIM_CLOCK_DIVIDER) - 1);
  TIM_InitStruct.CounterMode = LL_TIM_COUNTERMODE_CENTER_UP;
  TIM_InitStruct.Autoreload = ((PWM_PERIOD_CYCLES) / 2);
  TIM_InitStruct.ClockDivision = LL_TIM_CLOCKDIVISION_DIV2;
  TIM_InitStruct.RepetitionCounter = (REP_COUNTER);
  LL_TIM_Init(TIMx, &TIM_InitStruct);
  LL_TIM_DisableARRPreload(TIMx);
  TIM_InitStruct.Autoreload = ((PWM_PERIOD_CYCLES) / 2);
  LL_TIM_OC_EnablePreload(TIMx, TIMx_UH_CHANNEL);
  TIM_OC_InitStruct.OCMode = LL_TIM_OCMODE_PWM1;
  TIM_OC_InitStruct.OCState = LL_TIM_OCSTATE_DISABLE;
  TIM_OC_InitStruct.OCNState = LL_TIM_OCSTATE_DISABLE;
  TIM_OC_InitStruct.CompareValue = 0;
  TIM_OC_InitStruct.OCPolarity = PHASE_UH_POLARITY;
  TIM_OC_InitStruct.OCNPolarity = PHASE_UL_POLARITY;
  TIM_OC_InitStruct.OCIdleState = PWM_HIGH_SIDE_IDLE_STATE;
  TIM_OC_InitStruct.OCNIdleState = PWM_LOW_SIDE_IDLE_STATE;
  LL_TIM_OC_Init(TIMx, TIMx_UH_CHANNEL, &TIM_OC_InitStruct);
  LL_TIM_OC_DisableFast(TIMx, TIMx_UH_CHANNEL);
  TIM_InitStruct.Autoreload = ((PWM_PERIOD_CYCLES) / 2);
  LL_TIM_OC_EnablePreload(TIMx, TIMx_VH_CHANNEL);
  TIM_OC_InitStruct.OCState = LL_TIM_OCSTATE_DISABLE;
  TIM_OC_InitStruct.OCNState = LL_TIM_OCSTATE_DISABLE;
  TIM_OC_InitStruct.CompareValue = 0;
  TIM_OC_InitStruct.OCPolarity = PHASE_VH_POLARITY;
  TIM_OC_InitStruct.OCNPolarity = PHASE_VL_POLARITY;
  LL_TIM_OC_Init(TIMx, TIMx_VH_CHANNEL, &TIM_OC_InitStruct);
  LL_TIM_OC_DisableFast(TIMx, TIMx_VH_CHANNEL);
  TIM_InitStruct.Autoreload = ((PWM_PERIOD_CYCLES) / 2);
  LL_TIM_OC_EnablePreload(TIMx, TIMx_WH_CHANNEL);
  TIM_OC_InitStruct.OCState = LL_TIM_OCSTATE_DISABLE;
  TIM_OC_InitStruct.OCNState = LL_TIM_OCSTATE_DISABLE;
  TIM_OC_InitStruct.CompareValue = 0;
  TIM_OC_InitStruct.OCPolarity = PHASE_WH_POLARITY;
  TIM_OC_InitStruct.OCNPolarity = PHASE_WL_POLARITY;  
  LL_TIM_OC_Init(TIMx, TIMx_WH_CHANNEL, &TIM_OC_InitStruct);
  LL_TIM_OC_DisableFast(TIMx, TIMx_WH_CHANNEL);
  TIM_InitStruct.Autoreload = ((PWM_PERIOD_CYCLES) / 2);
  LL_TIM_OC_EnablePreload(TIMx, TIMx_ADC_TRG_CH);
  TIM_OC_InitStruct.OCMode = LL_TIM_OCMODE_PWM2;
  TIM_OC_InitStruct.OCState = LL_TIM_OCSTATE_DISABLE;
  TIM_OC_InitStruct.OCNState = LL_TIM_OCSTATE_DISABLE;
  TIM_OC_InitStruct.CompareValue = 0;
  TIM_OC_InitStruct.OCNPolarity = LL_TIM_OCPOLARITY_HIGH;
  LL_TIM_OC_Init(TIMx, TIMx_ADC_TRG_CH, &TIM_OC_InitStruct);
  LL_TIM_OC_DisableFast(TIMx, TIMx_ADC_TRG_CH);
  LL_TIM_SetTriggerInput(TIMx, LL_TIM_TS_ITR1);
  LL_TIM_SetSlaveMode(TIMx, LL_TIM_SLAVEMODE_TRIGGER);
  LL_TIM_DisableIT_TRIG(TIMx);
  LL_TIM_DisableDMAReq_TRIG(TIMx);
  LL_TIM_SetTriggerOutput(TIMx, LL_TIM_TRGO_OC4REF);
  LL_TIM_SetTriggerOutput2(TIMx, LL_TIM_TRGO2_RESET);
  LL_TIM_DisableMasterSlaveMode(TIMx);
  /* LL_TIM_SetBreakInputSourcePolarity(TIMx, LL_TIM_BREAK_INPUT_BKIN2, LL_TIM_BKIN_SOURCE_BKIN, LL_TIM_BKIN_POLARITY_LOW); */
  TIM_BDTRInitStruct.OSSRState = LL_TIM_OSSR_ENABLE;
  TIM_BDTRInitStruct.OSSIState = LL_TIM_OSSI_ENABLE;
  TIM_BDTRInitStruct.LockLevel = LL_TIM_LOCKLEVEL_1;
  TIM_BDTRInitStruct.DeadTime = ((DEAD_TIME_COUNTS) / 2);
  /* Enable/Disable TIMx BRK */
  TIM_BDTRInitStruct.BreakState = SW_OV_CURRENT_PROT_ENABLING; 
  TIM_BDTRInitStruct.BreakPolarity = OVERCURR_FEEDBACK_POLARITY;
  TIM_BDTRInitStruct.BreakFilter = OVERCURR_BREAK_FILTER;
  TIM_BDTRInitStruct.BreakAFMode = LL_TIM_BREAK_AFMODE_INPUT;
  TIM_BDTRInitStruct.Break2State = ICS_BREAK2_ENABLING;
  TIM_BDTRInitStruct.Break2Polarity = ICS_BREAK2_FEEDBACK_POLARITY;
  TIM_BDTRInitStruct.Break2Filter = ICS_BREAK2_FILTER;
  TIM_BDTRInitStruct.Break2AFMode = LL_TIM_BREAK_AFMODE_INPUT;
  TIM_BDTRInitStruct.AutomaticOutput = LL_TIM_AUTOMATICOUTPUT_DISABLE;
  LL_TIM_BDTR_Init(TIMx, &TIM_BDTRInitStruct);
}

/**
  * @brief Ia ADC Initialization Function
  * @param None
  * @retval None
  */
static void ICS_ADC_Init(ADC_TypeDef * ADCx, uint8_t ADC_Channel, TIM_TypeDef * TIMx)
{

  LL_ADC_InitTypeDef ADC_InitStruct = {0};
  LL_ADC_REG_InitTypeDef ADC_REG_InitStruct = {0};
  LL_ADC_CommonInitTypeDef ADC_CommonInitStruct = {0};
  LL_ADC_INJ_InitTypeDef ADC_INJ_InitStruct = {0};
  if(ADCx == ADC1 || ADCx == ADC2)
  {
    /* Peripheral clock enable */
    clock_enable_ADC12(true);
  }  
  if(ADCx == ADC3 || ADCx == ADC4 || ADCx == ADC5)
  {
    /* Peripheral clock enable */
    clock_enable_ADC345(true);
  }

  ADC_InitStruct.Resolution = LL_ADC_RESOLUTION_12B;
  ADC_InitStruct.DataAlignment = LL_ADC_DATA_ALIGN_LEFT;
  ADC_InitStruct.LowPowerMode = LL_ADC_LP_MODE_NONE;
  LL_ADC_Init(ADCx, &ADC_InitStruct);
  ADC_REG_InitStruct.SequencerLength = LL_ADC_REG_SEQ_SCAN_DISABLE;
  ADC_REG_InitStruct.SequencerDiscont = LL_ADC_REG_SEQ_DISCONT_DISABLE;
  ADC_REG_InitStruct.ContinuousMode = LL_ADC_REG_CONV_SINGLE;
  ADC_REG_InitStruct.DMATransfer = LL_ADC_REG_DMA_TRANSFER_NONE;
  ADC_REG_InitStruct.Overrun = LL_ADC_REG_OVR_DATA_PRESERVED;
  LL_ADC_REG_Init(ADCx, &ADC_REG_InitStruct);
  LL_ADC_SetOverSamplingScope(ADCx, LL_ADC_OVS_DISABLE);

  /* Disable ADC deep power down (enabled by default after reset state) */
  LL_ADC_DisableDeepPowerDown(ADCx);
  /* Enable ADC internal voltage regulator */
  LL_ADC_EnableInternalRegulator(ADCx);
  /* Delay for ADC internal voltage regulator stabilization. */
  /* Compute number of CPU cycles to wait for, from delay in us. */
  /* Note: Variable divided by 2 to compensate partially */
  /* CPU processing cycles (depends on compilation optimization). */
  /* Note: If system core clock frequency is below 200kHz, wait time */
  /* is only a few CPU processing cycles. */
  uint32_t wait_loop_index;
  wait_loop_index = ((LL_ADC_DELAY_INTERNAL_REGUL_STAB_US * (SystemCoreClock / (100000 * 2))) / 10);
  while(wait_loop_index != 0)
  {
    wait_loop_index--;
  }
  ADC_CommonInitStruct.CommonClock = LL_ADC_CLOCK_ASYNC_DIV4;
  ADC_CommonInitStruct.Multimode = LL_ADC_MULTI_INDEPENDENT;
  LL_ADC_CommonInit(__LL_ADC_COMMON_INSTANCE(ADCx), &ADC_CommonInitStruct);
  if(TIMx == TIM1)
  {
  ADC_INJ_InitStruct.TriggerSource = LL_ADC_INJ_TRIG_EXT_TIM1_TRGO;
  }
  else
  {
     ADC_INJ_InitStruct.TriggerSource =  LL_ADC_INJ_TRIG_EXT_TIM8_TRGO;
  }
  ADC_INJ_InitStruct.SequencerLength = LL_ADC_INJ_SEQ_SCAN_DISABLE;
  ADC_INJ_InitStruct.SequencerDiscont = LL_ADC_INJ_SEQ_DISCONT_DISABLE;
  ADC_INJ_InitStruct.TrigAuto = LL_ADC_INJ_TRIG_INDEPENDENT;
  LL_ADC_INJ_Init(ADCx, &ADC_INJ_InitStruct);
  LL_ADC_INJ_SetQueueMode(ADCx, LL_ADC_INJ_QUEUE_DISABLE);
  LL_ADC_SetOverSamplingScope(ADCx, LL_ADC_OVS_DISABLE);
  LL_ADC_INJ_SetTriggerEdge(ADCx, LL_ADC_INJ_TRIG_EXT_RISING);
  /** Configure Injected Channel
  */
  LL_ADC_SetChannelSamplingTime(ADCx, ADC_Channel, ADC_SAMPLING_TIME);
  LL_ADC_SetChannelSingleDiff(ADCx, ADC_Channel, LL_ADC_SINGLE_ENDED);
  LL_ADC_INJ_SetSequencerRanks(ADCx, LL_ADC_INJ_RANK_1, ADC_Channel);

}

static void ICS_ADC_ReGConv_Init(ADC_TypeDef * ADCx)
{
  LL_ADC_InitTypeDef ADC_InitStruct = {0};
  LL_ADC_REG_InitTypeDef ADC_REG_InitStruct = {0};
  LL_ADC_CommonInitTypeDef ADC_CommonInitStruct = {0};

  if(ADCx == ADC1 || ADCx == ADC2)
  {
    /* Peripheral clock enable */
    clock_enable_ADC12(true);
  }  
  if(ADCx == ADC3 || ADCx == ADC4 || ADCx == ADC5)
  {
    /* Peripheral clock enable */
    clock_enable_ADC345(true);
  }

  /** Common config
  */
  ADC_InitStruct.Resolution = LL_ADC_RESOLUTION_12B;
  ADC_InitStruct.DataAlignment = LL_ADC_DATA_ALIGN_LEFT;
  ADC_InitStruct.LowPowerMode = LL_ADC_LP_MODE_NONE;
  LL_ADC_Init(ADCx, &ADC_InitStruct);
  ADC_REG_InitStruct.SequencerLength = LL_ADC_REG_SEQ_SCAN_DISABLE;
  ADC_REG_InitStruct.SequencerDiscont = LL_ADC_REG_SEQ_DISCONT_DISABLE;
  ADC_REG_InitStruct.ContinuousMode = LL_ADC_REG_CONV_SINGLE;
  ADC_REG_InitStruct.DMATransfer = LL_ADC_REG_DMA_TRANSFER_NONE;
  ADC_REG_InitStruct.Overrun = LL_ADC_REG_OVR_DATA_PRESERVED;
  LL_ADC_REG_Init(ADCx, &ADC_REG_InitStruct);
  LL_ADC_SetOverSamplingScope(ADCx, LL_ADC_OVS_DISABLE);

  /* Disable ADC deep power down (enabled by default after reset state) */
  LL_ADC_DisableDeepPowerDown(ADCx);
  /* Enable ADC internal voltage regulator */
  LL_ADC_EnableInternalRegulator(ADCx);
  /* Delay for ADC internal voltage regulator stabilization. */
  /* Compute number of CPU cycles to wait for, from delay in us. */
  /* Note: Variable divided by 2 to compensate partially */
  /* CPU processing cycles (depends on compilation optimization). */
  /* Note: If system core clock frequency is below 200kHz, wait time */
  /* is only a few CPU processing cycles. */
  uint32_t wait_loop_index;
  wait_loop_index = ((LL_ADC_DELAY_INTERNAL_REGUL_STAB_US * (SystemCoreClock / (100000 * 2))) / 10);
  while(wait_loop_index != 0)
  {
    wait_loop_index--;
  }
  ADC_CommonInitStruct.CommonClock = LL_ADC_CLOCK_ASYNC_DIV4;
  ADC_CommonInitStruct.Multimode = LL_ADC_MULTI_INDEPENDENT;
  LL_ADC_CommonInit(__LL_ADC_COMMON_INSTANCE(ADCx), &ADC_CommonInitStruct);

}

/**
  * @brief  It initializes PWM peripheral for PWM generation
  * @param TIMx: Timer to be initialized
  * @param pHandle: handler of the current instance of the PWM component
  * @retval none
  */
static void ICS_PWM_Init( TIM_TypeDef * TIMx, PWMC_Handle_t * pHdl )
{
#if defined (__ICCARM__)
  #pragma cstat_disable = "MISRAC2012-Rule-11.3"
#endif /* __ICCARM__ */
  PWMC_ICS_Handle_t * pHandle = ( PWMC_ICS_Handle_t * )pHdl;
#if defined (__ICCARM__)
  #pragma cstat_restore = "MISRAC2012-Rule-11.3"
#endif /* __ICCARM__ */
  uint32_t Brk2Timeout = 1000;

  /* disable main TIM counter to ensure
   * a synchronous start by TIM2 trigger */
  LL_TIM_DisableCounter( TIMx );
  
  LL_TIM_SetTriggerOutput(TIMx, LL_TIM_TRGO_RESET);

  /* Enables the TIMx Preload on CC1 Register */
  LL_TIM_OC_EnablePreload( TIMx, TIMx_UH_CHANNEL );
  /* Enables the TIMx Preload on CC2 Register */
  LL_TIM_OC_EnablePreload( TIMx, TIMx_VH_CHANNEL );
  /* Enables the TIMx Preload on CC3 Register */
  LL_TIM_OC_EnablePreload( TIMx, TIMx_WH_CHANNEL );
  /* Enables the TIMx Preload on CC4 Register */
  LL_TIM_OC_EnablePreload( TIMx, TIMx_ADC_TRG_CH );
  /* Prepare timer for synchronization */
  LL_TIM_GenerateEvent_UPDATE( TIMx );
  if ( pHandle->pParams_str->FreqRatio == 2u )
  {
    if ( pHandle->pParams_str->IsHigherFreqTim == HIGHER_FREQ )
    {
      if ( pHandle->pParams_str->RepetitionCounter == 3u )
      {
        /* Set TIMx repetition counter to 1 */
        LL_TIM_SetRepetitionCounter( TIMx, 1 );
        LL_TIM_GenerateEvent_UPDATE( TIMx );
        /* Repetition counter will be set to 3 at next Update */
        LL_TIM_SetRepetitionCounter( TIMx, 3 );
      }
    }
    LL_TIM_SetCounter( TIMx, ( uint32_t )( pHandle->Half_PWMPeriod ) - 1u );
  }
  else /* bFreqRatio equal to 1 or 3 */
  {
    if ( pHandle->_Super.Motor == M1 )
    {
      if ( pHandle->pParams_str->RepetitionCounter == 1u )
      {
        LL_TIM_SetCounter( TIMx, ( uint32_t )( pHandle->Half_PWMPeriod ) - 1u );
      }
      else if ( pHandle->pParams_str->RepetitionCounter == 3u )
      {
        /* Set TIMx repetition counter to 1 */
        LL_TIM_SetRepetitionCounter( TIMx, 1 );
        LL_TIM_GenerateEvent_UPDATE( TIMx );
        /* Repetition counter will be set to 3 at next Update */
        LL_TIM_SetRepetitionCounter( TIMx, 3 );
      }
      else
      {
      }
    }
    else
    {
    }
  }
  LL_TIM_ClearFlag_BRK( TIMx );
  
  if ( ( pHandle->pParams_str->BKIN2Mode ) != NONE )
  {
    while ((LL_TIM_IsActiveFlag_BRK2 (TIMx) == 1u) && (Brk2Timeout != 0u) )
    {
      LL_TIM_ClearFlag_BRK2( TIMx );
      Brk2Timeout--;
    }   
  }
  LL_TIM_EnableIT_BRK( TIMx );

  /* Enable PWM channel */
  LL_TIM_CC_EnableChannel( TIMx, TIMxCCER_MASK_CH123 );

}

/**
  * @brief  It stores into the component the voltage present on Ia and
  *         Ib current feedback analog channels when no current is flowing into the
  *         motor
  * @param  pHdl: handler of the current instance of the PWM component
  * @retval none
  */
__weak void ICS_CurrentReadingPolarization( PWMC_Handle_t * pHdl )
{
#if defined (__ICCARM__)
  #pragma cstat_disable = "MISRAC2012-Rule-11.3"
#endif /* __ICCARM__ */
  PWMC_ICS_Handle_t * pHandle = ( PWMC_ICS_Handle_t * )pHdl;
#if defined (__ICCARM__)
  #pragma cstat_restore = "MISRAC2012-Rule-11.3"
#endif /* __ICCARM__ */
  TIM_TypeDef * TIMx = pHandle->pParams_str->TIMx;
  ADC_TypeDef * ADCx_1 = pHandle->pParams_str->ADCx_1;
  ADC_TypeDef * ADCx_2 = pHandle->pParams_str->ADCx_2;


  /* reset offset and counter */
  pHandle->PhaseAOffset = 0u;
  pHandle->PhaseBOffset = 0u;
  pHandle->PolarizationCounter = 0u;

  /* It forces inactive level on TIMx CHy and CHyN */
  LL_TIM_CC_DisableChannel(TIMx, TIMxCCER_MASK_CH123);

  /* Offset calibration for all phases */
  /* Change function to be executed in ADCx_ISR */
  pHandle->_Super.pFctGetPhaseCurrents = &ICS_HFCurrentsPolarization;
  ICS_SwitchOnPWM( &pHandle->_Super );
  
  /* IF CH4 is enabled, it means that JSQR is now configured to sample polarization current*/
  while ( ((TIMx->CR2) & TIM_CR2_MMS_Msk) != LL_TIM_TRGO_OC4REF )
  {
 }
  /* It is the right time to start the ADC without unwanted conversion */
  /* Start ADC to wait for external trigger. This is series dependent*/
  LL_ADC_INJ_StartConversion( ADCx_1 );
  LL_ADC_INJ_StartConversion( ADCx_2 );
  
  /* Wait for NB_CONVERSIONS to be executed */
  waitForPolarizationEnd( TIMx,
                          &pHandle->_Super.SWerror,
                          pHandle->pParams_str->RepetitionCounter,
                          &pHandle->PolarizationCounter );

  ICS_SwitchOffPWM( &pHandle->_Super );
  
  pHandle->PhaseAOffset /= NB_CONVERSIONS;
  pHandle->PhaseBOffset /= NB_CONVERSIONS;

  /* Change back function to be executed in ADCx_ISR */
  pHandle->_Super.pFctGetPhaseCurrents = &ICS_GetPhaseCurrents;
  pHandle->_Super.pFctSetADCSampPointSectX = &ICS_WriteTIMRegisters;

  /* It over write TIMx CCRy wrongly written by FOC during calibration so as to
     force 50% duty cycle on the three inverer legs */
  /* Disable TIMx preload */
  LL_TIM_OC_DisablePreload(TIMx,  TIMx_UH_CHANNEL);
  LL_TIM_OC_DisablePreload(TIMx,  TIMx_VH_CHANNEL);  
  LL_TIM_OC_DisablePreload(TIMx,  TIMx_WH_CHANNEL);
  LL_TIM_OC_SetCompareCH1 (TIMx, pHandle->Half_PWMPeriod);
  LL_TIM_OC_SetCompareCH2 (TIMx, pHandle->Half_PWMPeriod);
  LL_TIM_OC_SetCompareCH3 (TIMx, pHandle->Half_PWMPeriod);
  /* Enable TIMx preload */
  LL_TIM_OC_EnablePreload(TIMx,  TIMx_UH_CHANNEL);
  LL_TIM_OC_EnablePreload(TIMx,  TIMx_VH_CHANNEL);  
  LL_TIM_OC_EnablePreload(TIMx,  TIMx_WH_CHANNEL);

  /* It re-enable drive of TIMx CHy and CHyN by TIMx CHyRef*/
  LL_TIM_CC_EnableChannel(TIMx, TIMxCCER_MASK_CH123);

  pHandle->BrakeActionLock = false;

}

/**
  * @brief  It computes and return latest converted motor phase currents motor
  * @param  pHdl: handler of the current instance of the PWM component
  * @retval Ia and Ib current in Curr_Components format
  */ 
#if defined (MCTK_ITCM)
__weak void __attribute__((section(".itcm"))) ICS_GetPhaseCurrents( PWMC_Handle_t * pHdl, ab_t * Iab )
#else
__weak void ICS_GetPhaseCurrents( PWMC_Handle_t * pHdl, ab_t * Iab )
#endif
{
#if defined (__ICCARM__)
  #pragma cstat_disable = "MISRAC2012-Rule-11.3"
#endif /* __ICCARM__ */
  PWMC_ICS_Handle_t * pHandle = ( PWMC_ICS_Handle_t * )pHdl;
#if defined (__ICCARM__)
  #pragma cstat_restore = "MISRAC2012-Rule-11.3"
#endif /* __ICCARM__ */
  TIM_TypeDef * TIMx = pHandle->pParams_str->TIMx;
  ADC_TypeDef * ADCx_1 = pHandle->pParams_str->ADCx_1;
  ADC_TypeDef * ADCx_2 = pHandle->pParams_str->ADCx_2;
  int32_t aux;
  uint16_t reg;
  
  /* disable ADC trigger source */
  LL_TIM_SetTriggerOutput(TIMx, LL_TIM_TRGO_RESET);
  
  /* Ia = (hPhaseAOffset)-(PHASE_A_ADC_CHANNEL value)  */
  reg = ( uint16_t )( ADCx_1->JDR1 );
  aux = ( int32_t )( reg ) - ( int32_t )( pHandle->PhaseAOffset );

  if(pHandle->pParams_str->ShuntPosition == ICS_SHUNT_LEGS)
  {
     aux =- aux;
  }

  /* Saturation of Ia */
  if ( aux < -INT16_MAX )
  {
      Iab->a = -INT16_MAX;
  }
  else  if ( aux > INT16_MAX )
  {
      Iab->a = INT16_MAX;
  }
  else
  {
      Iab->a = ( int16_t )aux;
  }

  /* Ib = (hPhaseBOffset)-(PHASE_B_ADC_CHANNEL value) */
  reg = ( uint16_t )( ADCx_2->JDR1 );
  aux = ( int32_t )( reg ) - ( int32_t )( pHandle->PhaseBOffset );

  if(pHandle->pParams_str->ShuntPosition == ICS_SHUNT_LEGS)
  {
     aux =- aux;
  }
  
  if(pHandle->pParams_str->SensingSelection == ICS_SENSING_ON_UW_PHASES)
  {
     /* sensing on UW phase */
     pHandle->_Super.Ia = Iab->a;
     aux = - pHandle->_Super.Ia - aux;
  }
  else if (pHandle->pParams_str->SensingSelection == ICS_SENSING_ON_UW_PHASES)
  {
    /* sensing on VW phase */
    pHandle->_Super.Ib = Iab->a;
    aux = - pHandle->_Super.Ib - aux;
  }
  else
  {
  }

  /* Saturation of Ib */
  if ( aux < -INT16_MAX )
  {
      Iab->b = -INT16_MAX;
  }
  else  if ( aux > INT16_MAX )
  {
      Iab->b = INT16_MAX;
  }
  else
  {
      Iab->b = ( int16_t )aux;
  }
  if(pHandle->pParams_str->SensingSelection == ICS_SENSING_ON_UV_PHASES)
  {
     pHandle->_Super.Ia = Iab->a;
     pHandle->_Super.Ib = Iab->b;
  }
  else if (pHandle->pParams_str->SensingSelection == ICS_SENSING_ON_VW_PHASES)
  {
     pHandle->_Super.Ia = Iab->b;
  } 
  else if (pHandle->pParams_str->SensingSelection == ICS_SENSING_ON_UW_PHASES)
  {
    pHandle->_Super.Ib = Iab->b;
  }
  else
  {
  }
  pHandle->_Super.Ic = -Iab->a - Iab->b;
}

/**
  * @brief  Stores into the component's handle the voltage present on Ia and
  *         Ib current feedback analog channels when no current is flowing into the
  *         motor
  * @param  pHandle handler of the current instance of the PWM component
  * @retval none
  */
#if defined (MCTK_ITCM)
__weak uint16_t __attribute__((section(".itcm"))) ICS_WriteTIMRegisters( PWMC_Handle_t * pHdl )
#else
__weak uint16_t ICS_WriteTIMRegisters( PWMC_Handle_t * pHdl )
#endif
{
#if defined (__ICCARM__)
  #pragma cstat_disable = "MISRAC2012-Rule-11.3"
#endif /* __ICCARM__ */
  PWMC_ICS_Handle_t * pHandle = ( PWMC_ICS_Handle_t * )pHdl;
#if defined (__ICCARM__)
  #pragma cstat_restore = "MISRAC2012-Rule-11.3"
#endif /* __ICCARM__ */
  TIM_TypeDef * TIMx = pHandle->pParams_str->TIMx;
  uint16_t Aux;


  LL_TIM_OC_SetCompareCH1 ( TIMx, (uint32_t) pHandle->_Super.CntPhA );
  LL_TIM_OC_SetCompareCH2 ( TIMx, (uint32_t) pHandle->_Super.CntPhB );
  LL_TIM_OC_SetCompareCH3 ( TIMx, (uint32_t) pHandle->_Super.CntPhC );

  /* Limit for update event */
  if (((TIMx->CR2) & TIM_CR2_MMS_Msk) != LL_TIM_TRGO_RESET )
  {
    Aux = MC_FOC_DURATION;
  }
  else
  {
    Aux = MC_NO_ERROR;
  }
  return Aux;
}
/**
  * @brief  Implementation of PWMC_GetPhaseCurrents to be performed during
  *         calibration. It sum up injected conversion data into PhaseAOffset and
  *         PhaseBOffset to compute the offset introduced in the current feedback
  *         network. It is required to proper configure ADC inputs before to enable
  *         the offset computation.
  * @param  pHdl Pointer on the target component instance
  * @retval It always returns {0,0} in Curr_Components format
  */
static void ICS_HFCurrentsPolarization( PWMC_Handle_t * pHdl, ab_t * Iab )
{
#if defined (__ICCARM__)
  #pragma cstat_disable = "MISRAC2012-Rule-11.3"
#endif /* __ICCARM__ */
  PWMC_ICS_Handle_t * pHandle = ( PWMC_ICS_Handle_t * )pHdl;
#if defined (__ICCARM__)
  #pragma cstat_restore = "MISRAC2012-Rule-11.3"
#endif /* __ICCARM__ */
  TIM_TypeDef * TIMx = pHandle->pParams_str->TIMx;
  ADC_TypeDef * ADCx_1 = pHandle->pParams_str->ADCx_1;
  ADC_TypeDef * ADCx_2 = pHandle->pParams_str->ADCx_2;
   
  /* disable ADC trigger source */
    LL_TIM_SetTriggerOutput(TIMx, LL_TIM_TRGO_RESET);

  if ( pHandle->PolarizationCounter < NB_CONVERSIONS )
  {
    pHandle-> PhaseAOffset += ADCx_1->JDR1;
    pHandle-> PhaseBOffset += ADCx_2->JDR1;
    pHandle->PolarizationCounter++;
  }

  /* during offset calibration no current is flowing in the phases */
  Iab->a = 0;
  Iab->b = 0;
}

/**
  * @brief  It turns on low sides switches. This function is intended to be
  *         used for charging boot capacitors of driving section. It has to be
  *         called each motor start-up when using high voltage drivers
  * @param  pHdl: handler of the current instance of the PWM component
  * @retval none
  */
__weak void ICS_TurnOnLowSides( PWMC_Handle_t * pHdl )
{
#if defined (__ICCARM__)
  #pragma cstat_disable = "MISRAC2012-Rule-11.3"
#endif /* __ICCARM__ */
  PWMC_ICS_Handle_t * pHandle = ( PWMC_ICS_Handle_t * )pHdl;
#if defined (__ICCARM__)
  #pragma cstat_restore = "MISRAC2012-Rule-11.3"
#endif /* __ICCARM__ */
  TIM_TypeDef * TIMx = pHandle->pParams_str->TIMx;

  pHandle->_Super.TurnOnLowSidesAction = true;

  /* Clear Update Flag */
  LL_TIM_ClearFlag_UPDATE( pHandle->pParams_str->TIMx );

  /*Turn on the three low side switches */
  LL_TIM_OC_SetCompareCH1( TIMx, 0u );
  LL_TIM_OC_SetCompareCH2( TIMx, 0u );
  LL_TIM_OC_SetCompareCH3( TIMx, 0u );

  /* Wait until next update */
  while ( LL_TIM_IsActiveFlag_UPDATE( TIMx ) == 0u )
  {}

  /* Main PWM Output Enable */
  LL_TIM_EnableAllOutputs( TIMx );

  if ( ( pHandle->pParams_str->LowSideOutputs ) == ES_GPIO )
  {
	gpio_set_pin(pHandle->pParams_str->pwm_en_u_pin );
	gpio_set_pin(pHandle->pParams_str->pwm_en_v_pin );
	gpio_set_pin(pHandle->pParams_str->pwm_en_w_pin );

  }
  return;
}


/**
  * @brief  It enables PWM generation on the proper Timer peripheral acting on MOE
  *         bit
  * @param  pHdl: handler of the current instance of the PWM component
  * @retval none
  */
__weak void ICS_SwitchOnPWM( PWMC_Handle_t * pHdl )
{
#if defined (__ICCARM__)
  #pragma cstat_disable = "MISRAC2012-Rule-11.3"
#endif /* __ICCARM__ */
  PWMC_ICS_Handle_t * pHandle = ( PWMC_ICS_Handle_t * )pHdl;
#if defined (__ICCARM__)
  #pragma cstat_restore = "MISRAC2012-Rule-11.3"
#endif /* __ICCARM__ */
  TIM_TypeDef * TIMx = pHandle->pParams_str->TIMx;

  /* We forbid ADC usage for regular conversion on Systick cut 2.2 patch*/
  pHandle->ADCRegularLocked = true;

  pHandle->_Super.TurnOnLowSidesAction = false;

  /* Set all duty to 50% */
  LL_TIM_OC_SetCompareCH1(TIMx, ((uint32_t) pHandle->Half_PWMPeriod / (uint32_t) 2));
  LL_TIM_OC_SetCompareCH2(TIMx, ((uint32_t) pHandle->Half_PWMPeriod / (uint32_t) 2));
  LL_TIM_OC_SetCompareCH3(TIMx, ((uint32_t) pHandle->Half_PWMPeriod / (uint32_t) 2));
  LL_TIM_OC_SetCompareCH4(TIMx, ((uint32_t) pHandle->Half_PWMPeriod - (uint32_t) 5));

  /* wait for a new PWM period */
  LL_TIM_ClearFlag_UPDATE( TIMx );
  while ( LL_TIM_IsActiveFlag_UPDATE( TIMx ) == 0u )
  {}
  LL_TIM_ClearFlag_UPDATE( TIMx );

  /* Main PWM Output Enable */
  TIMx->BDTR |= LL_TIM_OSSI_ENABLE;
  LL_TIM_EnableAllOutputs ( TIMx );

  if ( ( pHandle->pParams_str->LowSideOutputs ) == ES_GPIO )
  {
    if ( ( TIMx->CCER & TIMxCCER_MASK_CH123 ) != 0u )
    {
      gpio_set_pin(pHandle->pParams_str->pwm_en_u_pin );
      gpio_set_pin(pHandle->pParams_str->pwm_en_v_pin );
      gpio_set_pin(pHandle->pParams_str->pwm_en_w_pin );

    }
    else
    {
      /* TBD */        
      /* It is executed during calibration phase the EN signal shall stay off */
      /* LL_GPIO_ResetOutputPin( pHandle->pParams_str->pwm_en_u_port, pHandle->pParams_str->pwm_en_u_pin ); */
      /* LL_GPIO_ResetOutputPin( pHandle->pParams_str->pwm_en_v_port, pHandle->pParams_str->pwm_en_v_pin ); */
      /* LL_GPIO_ResetOutputPin( pHandle->pParams_str->pwm_en_w_port, pHandle->pParams_str->pwm_en_w_pin ); */
      gpio_clear_pin(pHandle->pParams_str->pwm_en_u_pin );
      gpio_clear_pin(pHandle->pParams_str->pwm_en_v_pin );
      gpio_clear_pin(pHandle->pParams_str->pwm_en_w_pin );

    }
  }
  /* Clear Update Flag */
  LL_TIM_ClearFlag_UPDATE( TIMx );
  /* Enable Update IRQ */
  LL_TIM_EnableIT_UPDATE( TIMx );
}


/**
  * @brief  It disables PWM generation on the proper Timer peripheral acting on
  *         MOE bit
  * @param  pHdl: handler of the current instance of the PWM component
  * @retval none
  */
__weak void ICS_SwitchOffPWM( PWMC_Handle_t * pHdl )
{
#if defined (__ICCARM__)
  #pragma cstat_disable = "MISRAC2012-Rule-11.3"
#endif /* __ICCARM__ */
  PWMC_ICS_Handle_t * pHandle = ( PWMC_ICS_Handle_t * )pHdl;
#if defined (__ICCARM__)
  #pragma cstat_restore = "MISRAC2012-Rule-11.3"
#endif /* __ICCARM__ */
  TIM_TypeDef * TIMx = pHandle->pParams_str->TIMx;

  /* Disable UPDATE ISR */
  LL_TIM_DisableIT_UPDATE( TIMx );

  pHandle->_Super.TurnOnLowSidesAction = false;
  
  /* Main PWM Output Disable */
  if ( pHandle->BrakeActionLock == true )
  {
  }
  else
  {
    TIMx->BDTR &= ~( ( uint32_t )( LL_TIM_OSSI_ENABLE ) );
    LL_TIM_DisableAllOutputs( TIMx );

    if ( ( pHandle->pParams_str->LowSideOutputs ) == ES_GPIO )
    {
      gpio_clear_pin(pHandle->pParams_str->pwm_en_u_pin );
      gpio_clear_pin(pHandle->pParams_str->pwm_en_v_pin );
      gpio_clear_pin(pHandle->pParams_str->pwm_en_w_pin );

    }
  }

  /* wait for a new PWM period to flush last HF task */
  LL_TIM_ClearFlag_UPDATE( TIMx );
  while ( LL_TIM_IsActiveFlag_UPDATE( TIMx ) == 0u )
  {}
  LL_TIM_ClearFlag_UPDATE( TIMx );

  /* We allow ADC usage for regular conversion on Systick, cut 2.2 patch */
   pHandle->ADCRegularLocked=false;
}

/**
  * @brief  It contains the TIMx Update event interrupt
  * @param  pHandle: handler of the current instance of the PWM component
  * @retval none
  */
#if defined (MCTK_ITCM)
__weak void * __attribute__((section(".itcm"))) ICS_TIMx_UP_IRQHandler( PWMC_ICS_Handle_t * pHandle )
#else
__weak void * ICS_TIMx_UP_IRQHandler( PWMC_ICS_Handle_t * pHandle )
#endif
{
  TIM_TypeDef * TIMx = pHandle->pParams_str->TIMx;
  ADC_TypeDef * ADCx_1 = pHandle->pParams_str->ADCx_1;
  ADC_TypeDef * ADCx_2 = pHandle->pParams_str->ADCx_2;
  

  ADCx_1->JSQR = ( uint32_t ) pHandle->pParams_str->ADCConfig1;
  ADCx_2->JSQR = ( uint32_t ) pHandle->pParams_str->ADCConfig2;

  /* enable ADC trigger source */
  LL_TIM_SetTriggerOutput(TIMx, LL_TIM_TRGO_OC4REF);

  return &( pHandle->_Super.Motor );
}

/**
  * @brief  It contains the TIMx Break2 event interrupt
  * @param  pHandle: handler of the current instance of the PWM component
  * @retval none
  */
#if defined (MCTK_ITCM)
__weak void * __attribute__((section(".itcm"))) ICS_BRK2_IRQHandler( PWMC_ICS_Handle_t * pHandle )
#else
__weak void * ICS_BRK2_IRQHandler( PWMC_ICS_Handle_t * pHandle )
#endif
{
  if ( pHandle->BrakeActionLock == false )
  {
    if ( ( pHandle->pParams_str->LowSideOutputs ) == ES_GPIO )
    {
        gpio_clear_pin(pHandle->pParams_str->pwm_en_u_pin );
        gpio_clear_pin(pHandle->pParams_str->pwm_en_v_pin );
        gpio_clear_pin(pHandle->pParams_str->pwm_en_w_pin );

    }
  }
  pHandle->OverCurrentFlag = true;

  return &( pHandle->_Super.Motor );
}

/**
  * @brief  It contains the TIMx Break1 event interrupt
  * @param  pHandle: handler of the current instance of the PWM component
  * @retval none
  */
#if defined (MCTK_ITCM)
__weak void * __attribute__((section(".itcm"))) ICS_BRK_IRQHandler( PWMC_ICS_Handle_t * pHandle )
#else
__weak void * ICS_BRK_IRQHandler( PWMC_ICS_Handle_t * pHandle )
#endif
{

  pHandle->pParams_str->TIMx->BDTR |= LL_TIM_OSSI_ENABLE;
  pHandle->OverVoltageFlag = true;
  pHandle->BrakeActionLock = true;

  return &( pHandle->_Super.Motor );
}


/**
  * @brief  It is used to check if an overcurrent occurred since last call.
  * @param  pHdl Pointer on the target component instance
  * @retval uint16_t It returns MC_BREAK_IN whether an overcurrent has been
  *                  detected since last method call, MC_NO_FAULTS otherwise.
  */
__weak uint16_t ICS_IsOverCurrentOccurred( PWMC_Handle_t * pHdl )
{
#if defined (__ICCARM__)
  #pragma cstat_disable = "MISRAC2012-Rule-11.3"
#endif /* __ICCARM__ */
  PWMC_ICS_Handle_t * pHandle = ( PWMC_ICS_Handle_t * )pHdl;
#if defined (__ICCARM__)
  #pragma cstat_restore = "MISRAC2012-Rule-11.3"
#endif /* __ICCARM__ */
  uint16_t retVal = MC_NO_FAULTS;

  if ( pHandle->OverVoltageFlag == true )
  {
    retVal = MC_OVER_VOLT;
    pHandle->OverVoltageFlag = false;
  }

  if ( pHandle->OverCurrentFlag == true )
  {
    retVal |= MC_BREAK_IN;
    pHandle->OverCurrentFlag = false;
  }

  return retVal;
}

/**
 * @}
 */

/**
 * @}
 */

/**
 * @}
 */

/************************ (C) COPYRIGHT 2023 STMicroelectronics *****END OF FILE****/
