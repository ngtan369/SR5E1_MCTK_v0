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
*   @file    resolver_SR5E1_speed_pos_fdbk.c
*   @version M4_VERSION
*   @brief   This file provides firmware functions that implement the features
*            of the resolver component of the Stellar Motor Control.
*
*   @details This file provides firmware functions that implement the features
*            of the resolver component of the Stellar Motor Control.
*
 */
#include "resolver_SR5E1_speed_pos_fdbk.h"
#include "parameters_conversion.h"
#include "user_interface.h"
#include "mc_tuning.h"
#include "ui_task.h"
#include "mc_api.h"
#include "clock.h"
#include "mc_config.h"

/** @addtogroup STELLAR_PMSM_MC_FOC_LIBRARY
  * @{
  */

/** @addtogroup SpeednPosFdbk
  * @{
  */

/** @defgroup Resolver Resolver Speed & Position Feedback
  * @brief Resolver based Speed & Position Feedback implementation
  *
  * This component is used in applications controlling a motor equipped with a resolver.
  *
  * A resolver is an absolute mechanical position sensor. Resolver hardware can be viewed 
  * as two inductive position sensors, which, upon a supplied sinusoidal-shaped signal on 
  * input, generate two sinusoidal signals on output. The sinusoidal input reference is
  * amplitude modulated with the sine and cosine of the mechanical angle, respectively. 
  * These two output signals have to be decoded to obtain the angular position. 
  *
  *
  * @{
  */

#if (defined RESOLVER) || (defined AUX_RESOLVER)
#define RES_SPD_BUFF_SIZE 128

#ifdef RES_SINGLE_ADC_RESOURCE  
static void RES_ADCxInit(ADC_TypeDef * ADCx, uint32_t ADC_Channel, uint32_t ADC_Channel_2);
#else
static void RES_ADCxInit(ADC_TypeDef * ADCx, uint32_t ADC_Channel);
#endif
static void RES_ADCxEnable( ADC_TypeDef * ADCx );
static void RES_TIMx_DAC_Update(TIM_TypeDef * RES_TIM_DAC);
static void RES_TIMx_ADC_Init(TIM_TypeDef * RES_TIM_ADC);
static void RES_DACx_Init(DAC_TypeDef * DACx_ExcSignal);
static void RES_ADC_Calibration(Resolver_SR5E1_Handle_t *this);
static void RES_ClearVars(Resolver_SR5E1_Handle_t *this);
void MC_SetUserDAC(DAC_UserChannel_t bUserChNumber, int16_t hValue);

MCT_Handle_t* GetMCT(uint8_t bMotor);

volatile float fOffset = RESOLVER_ANGLE_OFFSET_S16;
volatile uint16_t hTim15ccr1 = 0;
volatile float newFreq = 10000;
volatile float exciting_amplitude_percentage = EXCITING_AMPLITUDE_PERCENTAGE;
volatile int16_t dpp2dacConFact = 100;

extern uint32_t SystemCoreClock;

uint16_t maxSpeedCnt = 0;
int32_t maxSpeedRPMTot = 0;
int16_t maxSpeedRPM = 0;

bool SensorIsReliable;    
uint8_t Res_SAR_ADC_VALID;

#define SPEED_SAMPLES 10000
#define SPEED_DELTA 10.0f

#define RES_DEBUG_TIM_TRG

/* #define RES_DBG_DAC	 */

/* #define RES_ANGLE_DEBUG */
#if defined(RES_ANGLE_DEBUG)
uint16_t ResTmp=0U;

int32_t ResBufTestSin[1200];
int32_t ResBufTestCos[1200];
#endif

uint16_t hSinOffset;
uint16_t hCosOffset;
#define NB_CONVERSIONS 16

/* enable / disable sensor reliability */
#define RES_RELIABILITY TRUE
/* Number of sensor reliability errors before goes in fault */
#define RES_RELIABILITY_MAX_ERRORS_BEFORE_FAULTS 10
#define ADC_TOLLERANCE 150
#if (RES_RELIABILITY==TRUE)
uint8_t resCurrentSpeedNumber = 0;
#endif
 /* #define DEBUG_RES_RELIABILITY  */
#ifdef DEBUG_RES_RELIABILITY
int32_t sinTethaBuf[1200];
int16_t cosTethaBuf[1200];
uint16_t ResIndex= 0;
#endif

const uint32_t hSin_64_Table_rom[] = {
0x00000800,0x000008AD,0x00000959,0x00000A02,0x00000AA5,0x00000B42,0x00000BD8,0x00000C63,
0x00000CE4,0x00000D58,0x00000DC0,0x00000E1A,0x00000E64,0x00000E9E,0x00000EC9,0x00000EE3,
0x00000EEB,0x00000EE3,0x00000EC9,0x00000E9E,0x00000E64,0x00000E1A,0x00000DC0,0x00000D58,
0x00000CE4,0x00000C63,0x00000BD8,0x00000B42,0x00000AA5,0x00000A02,0x00000959,0x000008AD,
0x00000800,0x00000753,0x000006A6,0x000005FD,0x0000055A,0x000004BD,0x00000427,0x0000039C,
0x0000031B,0x000002A7,0x0000023F,0x000001E5,0x0000019B,0x00000161,0x00000136,0x0000011D,
0x00000114,0x0000011D,0x00000136,0x00000161,0x0000019B,0x000001E5,0x0000023F,0x000002A7,
0x0000031B,0x0000039C,0x00000427,0x000004BD,0x0000055A,0x000005FD,0x000006A6,0x00000753};

/* TUNE demodulation trigger delay to maximize the amplitude of the demodulated                   */
/* sin/cos signal adjusting the resolver demodulation delay parameter                             */
/* The ResMaxAdc value  should reach the maximum resolution of the ADC, for i.e. 12 ADC --> 4095  */
/* This ResMaxAdc value can be can be adjusted in Live Monitor tool                               */
/* #define RES_TUNE_DEMODULATION_DELAY */
#define RES_TUNE_SIZE 1023U
uint16_t ResMaxAdc;

#ifdef RES_TUNE_DEMODULATION_DELAY
uint16_t sinTune[RES_TUNE_SIZE];
uint16_t Res_tune_index = 0;
#endif

/**
  * @brief Resolver ADCx Initialization Function
  * @param None
  * @retval None
  */
#ifdef RES_SINGLE_ADC_RESOURCE  
static void RES_ADCxInit(ADC_TypeDef * ADCx, uint32_t ADC_Channel, uint32_t ADC_Channel_2)
#else
static void RES_ADCxInit(ADC_TypeDef * ADCx, uint32_t ADC_Channel)
#endif
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
  ADC_INJ_InitStruct.TriggerSource = LL_ADC_INJ_TRIG_EXT_TIM15_TRGO;
#ifdef RES_SINGLE_ADC_RESOURCE
  ADC_INJ_InitStruct.SequencerLength = LL_ADC_INJ_SEQ_SCAN_ENABLE_3RANKS;
#else
  ADC_INJ_InitStruct.SequencerLength = LL_ADC_INJ_SEQ_SCAN_DISABLE;
#endif	  
  ADC_INJ_InitStruct.SequencerDiscont = LL_ADC_INJ_SEQ_DISCONT_DISABLE;
  ADC_INJ_InitStruct.TrigAuto = LL_ADC_INJ_TRIG_INDEPENDENT;
  LL_ADC_INJ_Init(ADCx, &ADC_INJ_InitStruct);
  LL_ADC_INJ_SetQueueMode(ADCx, LL_ADC_INJ_QUEUE_DISABLE);
  LL_ADC_SetOverSamplingScope(ADCx, LL_ADC_OVS_DISABLE);
  LL_ADC_INJ_SetTriggerEdge(ADCx, LL_ADC_INJ_TRIG_EXT_RISING);

  /** Configure Injected Channel
  */
  LL_ADC_SetChannelSamplingTime(ADCx, ADC_Channel, LL_ADC_SAMPLINGTIME_6CYCLES_5);
  LL_ADC_SetChannelSingleDiff(ADCx, ADC_Channel, LL_ADC_SINGLE_ENDED);
  LL_ADC_INJ_SetSequencerRanks(ADCx, LL_ADC_INJ_RANK_1, ADC_Channel);

#ifdef RES_SINGLE_ADC_RESOURCE
  LL_ADC_SetChannelSamplingTime(ADCx, ADC_Channel_2, LL_ADC_SAMPLINGTIME_6CYCLES_5);
  LL_ADC_SetChannelSingleDiff(ADCx, ADC_Channel_2, LL_ADC_SINGLE_ENDED);
  LL_ADC_INJ_SetSequencerRanks(ADCx, LL_ADC_INJ_RANK_2, ADC_Channel_2);
  LL_ADC_INJ_SetSequencerRanks(ADCx, LL_ADC_INJ_RANK_3, ADC_Channel);
#endif
}

/**
  * @brief Resolver ADCx enable Function
  * @param None
  * @retval None
  */
static void RES_ADCxEnable( ADC_TypeDef * ADCx )
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
}

/**
  * @brief Resolver DAC update function to generate exciting signal
  * @param None
  * @retval None
  */
static void RES_TIMx_DAC_Update(TIM_TypeDef * RES_TIM_DAC)
{

  LL_TIM_InitTypeDef TIM_InitStruct = {0};
  
  if(RES_TIM_DAC == TIM6)
  {
     /* Peripheral clock enable */
     clock_enable_TIM6(true);
  }  
  if(RES_TIM_DAC == TIM7)
  {
     /* Peripheral clock enable */
     clock_enable_TIM7(true);
  }  

  TIM_InitStruct.Prescaler = 0;
  TIM_InitStruct.CounterMode = LL_TIM_COUNTERMODE_UP;
  TIM_InitStruct.Autoreload = 468;
  LL_TIM_Init(RES_TIM_DAC, &TIM_InitStruct);
  LL_TIM_DisableARRPreload(RES_TIM_DAC);


  LL_TIM_SetTriggerOutput(RES_TIM_DAC,LL_TIM_TRGO_UPDATE);
  LL_TIM_DisableMasterSlaveMode(RES_TIM_DAC);
}

/**
  * @brief Resolver TIM initialization 
  * @param None
  * @retval None
  */
static void RES_TIMx_ADC_Init(TIM_TypeDef * RES_TIM_ADC)
{

  LL_TIM_InitTypeDef TIM_InitStruct = {0};
  LL_TIM_OC_InitTypeDef TIM_OC_InitStruct = {0};
  
  if(RES_TIM_ADC == TIM15)
  {
    /* Peripheral clock enable */
    clock_enable_TIM15(true);
  }
  if(RES_TIM_ADC == TIM16)
  {
    /* Peripheral clock enable */
    clock_enable_TIM16(true);
  } 

  TIM_InitStruct.Prescaler = 0;
  TIM_InitStruct.CounterMode = LL_TIM_COUNTERMODE_UP;
  TIM_InitStruct.Autoreload = 30420;
  TIM_InitStruct.ClockDivision = LL_TIM_CLOCKDIVISION_DIV1;
  LL_TIM_Init(RES_TIM_ADC, &TIM_InitStruct);
  LL_TIM_DisableARRPreload(RES_TIM_ADC);

  LL_TIM_SetTriggerOutput(RES_TIM_ADC,LL_TIM_TRGO_OC1REF);
  LL_TIM_DisableMasterSlaveMode(RES_TIM_ADC);

  TIM_OC_InitStruct.OCMode = LL_TIM_OCMODE_PWM2;
  TIM_OC_InitStruct.OCState = LL_TIM_OCSTATE_DISABLE;
  TIM_OC_InitStruct.OCNState = LL_TIM_OCSTATE_DISABLE;
  TIM_OC_InitStruct.CompareValue = 7605;
  TIM_OC_InitStruct.OCPolarity = LL_TIM_OCPOLARITY_HIGH;
  TIM_OC_InitStruct.OCNPolarity = LL_TIM_OCPOLARITY_HIGH;
  TIM_OC_InitStruct.OCIdleState = LL_TIM_OCIDLESTATE_LOW;
  TIM_OC_InitStruct.OCNIdleState = LL_TIM_OCIDLESTATE_LOW;
  LL_TIM_OC_Init(RES_TIM_ADC, RES_TIM_CHANNEL_FOR_ADC, &TIM_OC_InitStruct);
  LL_TIM_OC_DisableFast(RES_TIM_ADC, RES_TIM_CHANNEL_FOR_ADC);

}

/**
  * @brief Resolver DAC initialization 
  * @param None
  * @retval None
  */
static void RES_DACx_Init(DAC_TypeDef * DACx_ExcSignal)
{

  LL_DAC_InitTypeDef DAC_InitStruct = {0};

  clock_enable_BDAC1(true);

  /* resolver exciting signal */
  DAC_InitStruct.TriggerSource = LL_DAC_TRIG_SOFTWARE;
  DAC_InitStruct.WaveAutoGeneration = LL_DAC_WAVE_AUTO_GENERATION_NONE;
  DAC_InitStruct.OutputBuffer = LL_DAC_OUTPUT_BUFFER_ENABLE;
  LL_DAC_Init(DACx_ExcSignal, RES_DAC_CHANNEL, &DAC_InitStruct);
  LL_DAC_SetTriggerSource(DACx_ExcSignal, RES_DAC_CHANNEL, LL_DAC_TRIG_EXT_TIM6_TRGO);
  LL_DAC_EnableTrigger(DACx_ExcSignal, RES_DAC_CHANNEL);
}

/**
  * @brief  It initializes the hardware peripherals (TIMx, ADC and DAC)
            required for the speed position sensor management using RESOLVER
            sensors.
  * @param  pHandle: handler of the current instance of the resolver component
  * @retval none
  */
void RES_Init(Resolver_SR5E1_Handle_t *this)
{
#ifdef RES_SINGLE_ADC_RESOURCE	
  ADC_TypeDef * ADCx_SIN_COS = this->ADCx_SIN_COS;
#else  
  ADC_TypeDef * ADCx_SIN = this->ADCx_SIN;
  ADC_TypeDef * ADCx_COS = this->ADCx_COS;
#endif  
  DAC_TypeDef * RES_DAC = this->DACx_ExcSignal;
  DMA_TypeDef * RES_DMA_Periph = this->DMAx_DAC;
  TIM_TypeDef * RES_TIM_DAC_Periph = this->TIMx_DAC;
  TIM_TypeDef * RES_TIM_ADC_Periph = this->TIMx_ADC;
  
#ifdef RES_SINGLE_ADC_RESOURCE
  RES_ADCxInit(ADCx_SIN_COS,RES_ADC_CHANNEL_SIN,RES_ADC_CHANNEL_COS);
#else
  RES_ADCxInit(ADCx_SIN,RES_ADC_CHANNEL_SIN);
  RES_ADCxInit(ADCx_COS,RES_ADC_CHANNEL_COS);
#endif
  
  for (uint8_t i = 0; i < 64; i++)
  {
    this->hSin[i] = hSin_64_Table_rom[i];
  }
  
  this->bBootCompleted = 0;
  
  if (RES_DMA_Periph == DMA1)
  { 
    clock_enable_DMA1(true);
  }
  if (RES_DMA_Periph == DMA2)
  { 
    clock_enable_DMA2(true);
  }
  if(RES_DMA_MUX == DMAMUX1)
  {
    clock_enable_DMAMUX1(true);
  }  
  
  LL_DMA_SetDataLength   (RES_DMA_Periph, RES_DMA_CHANNEL, 32u);
  LL_DMA_SetMemoryAddress(RES_DMA_Periph, RES_DMA_CHANNEL, (uint32_t)(this->hSin));
  LL_DMA_SetPeriphAddress(RES_DMA_Periph, RES_DMA_CHANNEL, (uint32_t)(&(RES_DAC->DHR12R1)));

  LL_DMA_ConfigTransfer(RES_DMA_Periph, RES_DMA_CHANNEL, LL_DMA_DIRECTION_MEMORY_TO_PERIPH | LL_DMA_PERIPH_NOINCREMENT
                                                | LL_DMA_MEMORY_INCREMENT | LL_DMA_PDATAALIGN_WORD
                                                | LL_DMA_MDATAALIGN_WORD | LL_DMA_MODE_CIRCULAR | LL_DMA_PRIORITY_VERYHIGH);

#ifdef RES_SINGLE_ADC_RESOURCE
  RES_ADCxEnable(ADCx_SIN_COS);
#else
  RES_ADCxEnable(ADCx_SIN);
  RES_ADCxEnable(ADCx_COS);
#endif	
  RES_ADC_Calibration(this);
  
  RES_TIMx_ADC_Init(RES_TIM_ADC_Periph);
  RES_TIMx_DAC_Update(RES_TIM_DAC_Periph);
  
  RES_DACx_Init(RES_DAC);
 
#ifdef RES_SINGLE_ADC_RESOURCE
  LL_ADC_ClearFlag_JEOS( ADCx_SIN_COS );
  LL_ADC_EnableIT_JEOS( ADCx_SIN_COS );
  LL_ADC_INJ_StartConversion(ADCx_SIN_COS);
#else 
  LL_ADC_ClearFlag_JEOS( ADCx_SIN );
  LL_ADC_ClearFlag_JEOS( ADCx_COS );
  LL_ADC_EnableIT_JEOS( ADCx_SIN );
  LL_ADC_INJ_StartConversion(ADCx_SIN);
  LL_ADC_INJ_StartConversion(ADCx_COS);
#endif

  LL_DAC_Enable(RES_DAC, RES_DAC_CHANNEL);

  LL_TIM_EnableDMAReq_UPDATE(RES_TIM_DAC_Periph);
  LL_TIM_EnableCounter(RES_TIM_DAC_Periph);
  LL_TIM_EnableCounter(RES_TIM_ADC_Periph);
  
  /* Init speed reliability */
  SensorIsReliable = TRUE;
  
  this->fExcitingFrequency = EXCITING_FREQUENCY;
  RES_UpdateFrequency(this, this->fExcitingFrequency);

  if (EXCITING_AMPLITUDE_PERCENTAGE != 100.0f)
  {
    this->wMod = EXCITING_AMPLITUDE_S32;
    RES_UpdateModulation(this, this->wMod);
  }
  
#ifdef RES_DEBUG_TIM_TRG
   LL_TIM_EnableAllOutputs(RES_TIM_ADC_Periph);
   LL_TIM_CC_EnableChannel(RES_TIM_ADC_Periph,RES_TIM_CHANNEL_FOR_ADC);
#endif

  /* Setup PLL */
  this->pll.pi.hKpGain =     RESOLVER_PLL_KP_GAIN;
  this->pll.pi.hKiGain =     RESOLVER_PLL_KI_GAIN;
  this->pll.pi.hKpDivisor =  RESOLVER_PLL_KP_DIVIDER;
  this->pll.pi.hKiDivisor =  RESOLVER_PLL_KI_DIVIDER;
  this->pll.pi.hUpperLimit = 5000;
  this->pll.pi.wLowerIntegralLimit = -this->pll.pi.hUpperLimit * this->pll.pi.hKiDivisor;
  this->pll.pi.wUpperIntegralLimit = this->pll.pi.hUpperLimit * this->pll.pi.hKiDivisor;
  this->pll.pi.wIntegralTerm = 0;
  
  /* Setup maximizers */
  maximizer_Init(&(this->max_sinAmp), 1, RES_TIM_ADC_Periph->ARR);
  this->max_sinAmp.delta = 10.0f;
  this->max_sinAmp.x = 1;
  maximizer_Init(&(this->max_speed) , 0, 360);
  this->max_speed.delta = SPEED_DELTA;
  this->max_speed.x = fOffset;
  
  this->wResSpdRPMSum = 0;
  this->hResSpdRPMCnt = 0;
  
  this->bDacCh = 1;
  this->bDacOffline = 1;
  this->testMode = 0;
  
  /* open loop */
  RES_ClearVars(this);
  this->fOLEndSpeedDpp = ((OLENDSPEEDRPM / 60.0f) * 65536.0f)/(float)(this->fExcitingFrequency);  // Exec ramp
  this->wDurationCnt   = (uint32_t)(OLSTARTDURATION * (float)(this->fExcitingFrequency));         // Exec ramp
  this->fOLAlphaDppDpp = this->fOLEndSpeedDpp / (float)(this->wDurationCnt);                     // Exec ramp
}

void RES_UpdateModulation(Resolver_SR5E1_Handle_t *this, int32_t mod)
{
  /* Change modulation index */
  for (uint8_t i = 0; i < 64; i++)
  {
    int32_t val = hSin_64_Table_rom[i] - hSin_64_Table_rom[0];
    int32_t wAux = val * mod;
    wAux /= 32768;
    val = wAux;
    val += hSin_64_Table_rom[0];
    this->hSin[i] = val;
  }
  this->wMod = mod;
}

void RES_UpdateFrequency(Resolver_SR5E1_Handle_t * this, float excitingFrequency)
{
  DMA_TypeDef * RES_DMA_Periph = this->DMAx_DAC;
  TIM_TypeDef * RES_TIM_DAC_Periph = this->TIMx_DAC;
  TIM_TypeDef * RES_TIM_ADC_Periph = this->TIMx_ADC;
  
  LL_DMA_DisableStream(RES_DMA_Periph, RES_DMA_CHANNEL);
  
  uint16_t arr = (uint16_t)(300000000.0f/(64.0f * excitingFrequency));
  RES_TIM_DAC_Periph->ARR = arr;
  uint16_t arr2 = ((arr + 1) * 64) - 1; // To sinch both timers
  RES_TIM_ADC_Periph->ARR = arr2;
  hTim15ccr1 = RESOLVER_DELAY_TIM15_CCR1;
  RES_TIM_ADC_Periph->CCR1 = hTim15ccr1; /* 1/4 of triggering point mean peak of generated sinusoid */
  RES_TIM_DAC_Periph->CNT = 0;
  RES_TIM_ADC_Periph->CNT = 0;

  LL_DMA_SetDataLength   (RES_DMA_Periph, RES_DMA_CHANNEL, 32u);
  LL_DMA_SetMemoryAddress(RES_DMA_Periph, RES_DMA_CHANNEL, (uint32_t)(this->hSin));
  LL_DMAMUX_SetRequestID(RES_DMA_MUX, RES_DMA_MUX_CH, LL_DMAMUX1_REQ_TIM6_UP);
  LL_DMA_EnableStream(RES_DMA_Periph, RES_DMA_CHANNEL);
  
  this->fExcitingFrequency = excitingFrequency;
}

/**
  * @brief  Get Average Speed 
  * @param  pHandle: handler of the current instance of the resolver component
  * @retval bool sensor reliable
  */
bool RES_GetAvrgMecSpeed01Hz(Resolver_SR5E1_Handle_t *this, int16_t *pMecSpeed01Hz)
{
  *pMecSpeed01Hz = this->hAvrgMechSpdRPM / 6;
  return SensorIsReliable;
}

/**
  * @brief  Get Electrical Angle 
  * @param  pHandle: handler of the current instance of the resolver component
  * @retval electrical angle
  */
#if defined (MCTK_ITCM)
int16_t __attribute__((section(".itcm"))) RES_GetElAngle(Resolver_SR5E1_Handle_t *this)
#else
int16_t RES_GetElAngle(Resolver_SR5E1_Handle_t *this)
#endif
{
  return this->_Super.hElAngle;
}

/**
  * @brief  Resolver ADC calibration function 
  * @param  pHandle: handler of the current instance of the resolver component
  * @retval none
  */
static void RES_ADC_Calibration(Resolver_SR5E1_Handle_t *this)
{

  uint8_t bIndex;
  uint32_t SinOffset=0u, CosOffset=0u; 
#ifdef RES_SINGLE_ADC_RESOURCE
  uint32_t ResTrgSin;
  ADC_TypeDef * ADCx_SIN_COS = this->ADCx_SIN_COS;
#else
  uint32_t ResTrgSin,ResTrgCos;
  ADC_TypeDef * ADCx_SIN = this->ADCx_SIN;
  ADC_TypeDef * ADCx_COS = this->ADCx_COS;  
#endif
 
#ifdef RES_SINGLE_ADC_RESOURCE
  ResTrgSin = LL_ADC_INJ_GetTriggerSource(ADCx_SIN_COS);
  LL_ADC_INJ_SetTriggerSource(ADCx_SIN_COS,LL_ADC_INJ_TRIG_SOFTWARE);
#else
  /* ADCx Injected conversions trigger is given by software and enabled */ 
  ResTrgSin = LL_ADC_INJ_GetTriggerSource(ADCx_SIN);
  ResTrgCos = LL_ADC_INJ_GetTriggerSource(ADCx_COS);
  LL_ADC_INJ_SetTriggerSource(ADCx_SIN,LL_ADC_INJ_TRIG_SOFTWARE);
  LL_ADC_INJ_SetTriggerSource(ADCx_COS,LL_ADC_INJ_TRIG_SOFTWARE);
#endif  
  /* ADC Channel used for current reading are read 
  in order to get zero currents ADC values*/ 
  for(bIndex = NB_CONVERSIONS; bIndex !=0u; bIndex--)
  {
#ifdef RES_SINGLE_ADC_RESOURCE
    /* Clear the ADC JEOS pending flag */
    LL_ADC_ClearFlag_EOS(ADCx_SIN_COS);

    LL_ADC_INJ_StartConversion(ADCx_SIN_COS);

    while(!LL_ADC_IsActiveFlag_JEOS(ADCx_SIN_COS)) { }
    
    SinOffset += ADCx_SIN_COS->JDR1;
    CosOffset += ADCx_SIN_COS->JDR2;
#else	  
    /* Clear the ADCx JEOC pending flag */
    LL_ADC_ClearFlag_EOC(ADCx_SIN);
    LL_ADC_ClearFlag_EOC(ADCx_COS);

    LL_ADC_INJ_StartConversion(ADCx_SIN);
    LL_ADC_INJ_StartConversion(ADCx_COS);

    while(!LL_ADC_IsActiveFlag_JEOC(ADCx_SIN)) { }
    while(!LL_ADC_IsActiveFlag_JEOC(ADCx_COS)) { }
    
    SinOffset += ADCx_SIN->JDR1;
    CosOffset += ADCx_COS->JDR1;
#endif	
  }
  
  hSinOffset=  (uint16_t)(SinOffset/NB_CONVERSIONS);
  hCosOffset = (uint16_t)(CosOffset/NB_CONVERSIONS);
#ifdef RES_SINGLE_ADC_RESOURCE
  LL_ADC_ClearFlag_EOS(ADCx_SIN_COS);
#else      
  LL_ADC_ClearFlag_EOC(ADCx_SIN);
  LL_ADC_ClearFlag_EOC(ADCx_COS);
#endif  
  /* check ADC offset values - an offset voltage value must be avaialable */ 
  if( (hSinOffset <= ADC_TOLLERANCE)  ||
      (hCosOffset <= ADC_TOLLERANCE) )
  {
    SensorIsReliable = FALSE;
  }
  else
  {
#ifdef RES_SINGLE_ADC_RESOURCE
     LL_ADC_INJ_SetTriggerSource(ADCx_SIN_COS,ResTrgSin);
#else	  
     LL_ADC_INJ_SetTriggerSource(ADCx_SIN,ResTrgSin);
     LL_ADC_INJ_SetTriggerSource(ADCx_COS,ResTrgCos);
#endif	 
  }
}

/**
  * @brief  IRQ implementation of the RESOLVER
  * @param  pHandle: handler of the current instance of the resolver component
  * @param  flag used to distinguish between various IRQ sources
  * @retval none
  */
#if defined (MCTK_ITCM)
void __attribute__((section(".itcm"))) RES_IRQHandler(Resolver_SR5E1_Handle_t *this)
#else
void RES_IRQHandler(Resolver_SR5E1_Handle_t *this)
#endif
{
#ifdef RES_SINGLE_ADC_RESOURCE
  int32_t sinTetha,cosTetha,sinTetha_2;
  ADC_TypeDef * ADCx_SIN_COS = this->ADCx_SIN_COS;
#else  
  int32_t sinTetha,cosTetha;
  ADC_TypeDef * ADCx_SIN = this->ADCx_SIN;
  ADC_TypeDef * ADCx_COS = this->ADCx_COS;
#endif  
#ifdef RES_DBG_DAC
  TIM_TypeDef * RES_TIM_ADC_Periph = this->TIMx_ADC;
#endif
#if (RES_RELIABILITY==TRUE)
   bool SpeedError = (bool)false;
   int32_t sinValue, cosValue;
#endif

#if (RES_RELIABILITY==TRUE)  
#ifdef RES_SINGLE_ADC_RESOURCE
   sinTetha = (int32_t)(ADCx_SIN_COS->JDR1);
   cosTetha = (int32_t)(ADCx_SIN_COS->JDR2);
   sinTetha_2 = (int32_t)(ADCx_SIN_COS->JDR3);
   sinTetha = (int32_t)(sinTetha + sinTetha_2) >> 1;
#else	
   sinTetha = (int32_t)(ADCx_SIN->JDR1);
   cosTetha = (int32_t)(ADCx_COS->JDR1);
#endif   
  
  /* check ADC offset values - check if the resolver,  */
  /* board is powered                                  */ 
  if( ((uint16_t)sinTetha <= ADC_TOLLERANCE)  ||
      ((uint16_t)cosTetha <= ADC_TOLLERANCE) )
  {
     SpeedError = (bool)true;
  }
  else
  {
#if defined(RES_TUNE_DEMODULATION_DELAY)
    if(Res_tune_index < RES_TUNE_SIZE)
    {
       sinTune[Res_tune_index] = sinTetha >> 4;
       Res_tune_index++;
       if(sinTune[Res_tune_index]>ResMaxAdc)
           ResMaxAdc=sinTune[Res_tune_index];
       if(Res_tune_index == RES_TUNE_SIZE)
       {
           Res_tune_index= 0U;
       }
    }
#endif

     sinTetha = (sinTetha-(int32_t)(hSinOffset));
     cosTetha = (cosTetha-(int32_t)(hCosOffset));
       
     sinValue = (sinTetha < 0 ? (-sinTetha) : (sinTetha));  /*  abs value   */
     cosValue = (cosTetha < 0 ? (-cosTetha) : (cosTetha));  /*  abs value   */
#ifdef DEBUG_RES_RELIABILITY
     if(ResIndex < 1200)
     {
        sinTethaBuf[ResIndex] = sinValue;
        cosTethaBuf[ResIndex] = cosValue;
        ResIndex++;
        if(ResIndex == 1200)
        {
           ResIndex= 0U;
        }
     }
#endif        
     /* check the consistency of ADC SIN/COS values    */
     /* check SIN / COS values are non-zero            */ 
     if( (sinValue <= ADC_TOLLERANCE)  &&
         (cosValue <= ADC_TOLLERANCE) )
     {
       SpeedError = (bool)true;
     }
     else
     {
       Res_SAR_ADC_VALID = 1;
     }
  }
  if (SpeedError == (bool)true)
  {
    if (resCurrentSpeedNumber < RES_RELIABILITY_MAX_ERRORS_BEFORE_FAULTS)
    {
      resCurrentSpeedNumber++;
    }
  }
  else
  {
     if (resCurrentSpeedNumber < RES_RELIABILITY_MAX_ERRORS_BEFORE_FAULTS)
     {
       resCurrentSpeedNumber = 0u;
     }
  }   
  if (resCurrentSpeedNumber == RES_RELIABILITY_MAX_ERRORS_BEFORE_FAULTS)
  { 
    SensorIsReliable = FALSE;
  }
#else
#ifdef RES_SINGLE_ADC_RESOURCE
    sinTetha = (int32_t)((ADCx_SIN_COS->JDR1)-(int32_t)(hSinOffset));
    cosTetha = (int32_t)((ADCx_SIN_COS->JDR2)-(int32_t)(hCosOffset));
#else	
    sinTetha = (int32_t)((ADCx_SIN->JDR1)-(int32_t)(hSinOffset));
    cosTetha = (int32_t)((ADCx_COS->JDR1)-(int32_t)(hCosOffset)); 
#endif	
    Res_SAR_ADC_VALID = 1;
#endif

  if(Res_SAR_ADC_VALID >= 1)
  {

#if defined(RES_ANGLE_DEBUG) 
    if(ResTmp < 1200U)
    {
       ResBufTestSin[ResTmp] = sinTetha ;
       ResBufTestCos[ResTmp] = cosTetha;
       ResTmp++;
       if(ResTmp == 1200U)
       {
          ResTmp= 0U;
       }
    }
#endif

  if (this->bBootCompleted == 1)
  {

    PLL_Calc(&(this->pll), cosTetha * this->hResolverInvertDirection, sinTetha);
    
    this->hResAngle =  this->pll.hElAngle; /* Resolver Angle */
    this->hResSpdDPP = this->pll.hSpeedDpp;  /* Resolver Speed */
    
    this->wResSpdRPMSum += (int16_t)(((int32_t)(this->hResSpdDPP) * (60 * this->fExcitingFrequency))/65536); /* Resolver RPM */
    this->hResSpdRPMCnt++;
    if (this->hResSpdRPMCnt >= RES_SPD_BUFF_SIZE)
    {
      this->hAvrgResSpdRPM = this->wResSpdRPMSum / this->hResSpdRPMCnt; /* Average Resolver RPM */
      this->wResSpdRPMSum = 0;
      this->hResSpdRPMCnt = 0;
    }
    
    this->hAvrgMechSpdRPM = this->hAvrgResSpdRPM / RESOLVER_TO_MECHANICAL_RATIO; // Average Motor RPM
    this->_Super.hAvrMecSpeedUnit = (int16_t)(((int32_t)(this->hAvrgMechSpdRPM) * SPEED_UNIT) / 60); // Average Motor unit 
    this->_Super.hElAngle = (this->hResAngle * this->_Super.bElToMecRatio / RESOLVER_TO_MECHANICAL_RATIO) + fOffset; // Motor Electrical Angle
#ifdef RES_DBG_DAC
    if (GetDAC())
    {
      switch (this->bDacCh)
      {
      case 0:
        {
          MC_SetUserDAC(DAC_USER1, sinTetha);
          MC_SetUserDAC(DAC_USER2, cosTetha);
        }
        break;
      case 1:
        {
          MC_SetUserDAC(DAC_USER1, this->hResAngle);
          MC_SetUserDAC(DAC_USER2, this->hResSpdDPP * dpp2dacConFact);
        }
        break;
      case 2:
        {
          MCT_Handle_t* oMCTMy = GetMCT(M1);
          SpeednPosFdbk_Handle_t* oSPDMy = oMCTMy->pSpeedSensorMain;
          MC_SetUserDAC(DAC_USER1, SPD_GetElAngle(oSPDMy));
        }
        break;
      }
    
      if (this->bDacOffline == 1)
      {
        UI_DACUpdate(M1);
      }
    }
    
    switch (this->testMode)
    {
    case 1:
      {
        // Test mode to update the excitation frequency on-the-fly
        RES_UpdateFrequency(this, newFreq);
        this->testMode = 0;
      }
      break;
    case 2:
      {
        // Test mode to update the amplitude of excitation signal on-the-fly
        int32_t exciting_amplitude_s32 = (int32_t)((exciting_amplitude_percentage * 32768.0f) / 100.0f);
        this->wMod = exciting_amplitude_s32;
        RES_UpdateModulation(this, this->wMod);
        this->testMode = 0;
      }
      break;
    case 3:
      {
        // Test mode to update the demodulation sampling point to maximize the sinTheta value
        hTim15ccr1 = (uint16_t)(maximizer_Calc(&(this->max_sinAmp), sinTetha));
        RES_TIM_ADC_Periph->CCR1 = hTim15ccr1;
      }
      break;
    case 4:
      {
        // Test mode to update the angle offset to maximize the speed in torque mode
        if (maxSpeedCnt < SPEED_SAMPLES)
        {
          maxSpeedRPMTot += this->hAvrgMechSpdRPM;
          maxSpeedCnt++;
        }
        else
        {
          maxSpeedRPM = maxSpeedRPMTot / maxSpeedCnt;
          maxSpeedCnt = 0;
          maxSpeedRPMTot = 0;
          
          fOffset = maximizer_Calc(&(this->max_speed), maxSpeedRPM);
        }
      }
      break;
    case 5:
      {
        // Test mode for OPEN-LOOP
        if (MC_GetSTMStateMotor1() == RUN)
        {
          // Omega integration
          if (this->wTimeCnt < this->wDurationCnt)
          {
            this->fOLOmegaDpp += this->fOLAlphaDppDpp;
            this->wTimeCnt++;
          }
          else
          {
            this->fOLOmegaDpp = this->fOLEndSpeedDpp;
          }
          // Teta integration
          this->fOLTetha += this->fOLOmegaDpp;
          if (this->fOLTetha > 32768.0f)
          {
            this->fOLTetha -= 65536.0f;
          }
          if (this->fOLTetha < -32768.0f)
          {
            this->fOLTetha += 65536.0f;
          }
        }
        this->_Super.hElAngle = (int16_t)(this->fOLTetha);
        this->_Super.hAvrMecSpeedUnit = (int16_t)((float)(SPEED_UNIT) * (float)(this->fOLOmegaDpp) * (float)(this->fExcitingFrequency) / 65536.0f);
      }
      break;
    case 6:
      {
        //this->pll.hTheta = 0;
        this->pll.hElAngle= 0;
        this->pll.pi.wIntegralTerm = 0;
        this->testMode = 0;
      }
      break;
    }
#endif 	/* RES_DBG_DAC*/ 
  }
  }
}

uint16_t RES_ExecRegularConv(Resolver_SR5E1_Handle_t *this, uint8_t bChannel)
{
  uint16_t hRegConv = 0;
  if (bChannel == this->bVBusADC_Channel) // BUS
  {
    hRegConv = this->hResSinCos[2];
  } else if (bChannel == this->bTempADC_Channel) // Temp
  {
    hRegConv = this->hResSinCos[3];
  }
  return hRegConv;
}

void RES_BootComplete(Resolver_SR5E1_Handle_t *this)
{
  this->bBootCompleted = 1;
}

/**
* @brief  This function must be called before starting the motor to initialize
*         the speed measurement process.
* @param  pHandle: handler of the current instance of the resolver component
* @retval none
*/
void RES_Clear(Resolver_SR5E1_Handle_t *this)
{ 
  if (this->testMode == 5)
  {
    qd_t IqdRefOL;
    IqdRefOL.d = 0;
    IqdRefOL.q = OLCURRENTS16;
    MC_SetCurrentReferenceMotor1(IqdRefOL);
  }
  RES_ClearVars(this);
}

static void RES_ClearVars(Resolver_SR5E1_Handle_t *this)
{
  this->wTimeCnt    = 0;
  this->fOLTetha    = 0.0f;
  this->fOLOmegaDpp = 0.0f;
}

/**
* @brief  This function set resolver angle offset
* @param  pHandle: handler of the current instance of the resolver component
* @retval none
*/
void RES_SetPhaseShift(float PhaseShift)
{
  fOffset = PhaseShift;
}

/**
* @brief  This function get resolver angle offset
* @param  pHandle: handler of the current instance of the resolver component
* @retval none
*/
void RES_GetPhaseShift(float *phaseShift)
{
  *phaseShift = fOffset;
}

/**
* @brief  This function set resolver Demodulation Trigger Delay
* @param  pHandle: handler of the current instance of the resolver component
* @retval none
*/
void RES_SetDemodulationTriggerDelay(Resolver_SR5E1_Handle_t *this, uint16_t DemTrgDelay)
{
  TIM_TypeDef * RES_TIM_ADC_Periph = this->TIMx_ADC;	
  hTim15ccr1 = DemTrgDelay;
  RES_TIM_ADC_Periph->CCR1 = hTim15ccr1;
}

/**
* @brief  This function get resolver Demodulation Trigger Delay
* @param  pHandle: handler of the current instance of the resolver component
* @retval none
*/
void RES_GetDemodulationTriggerDelay(uint16_t *DemTrgDelay)
{
  *DemTrgDelay = hTim15ccr1;
}


/**
* @brief  This function get resolver Demodulation Trigger Delay
* @param  pHandle: handler of the current instance of the resolver component
* @retval none
*/
void RES_GetResADCMaxResolution(uint16_t *ResMaxADCResolution)
{
  *ResMaxADCResolution = ResMaxAdc;
}
#endif /* #if (defined RESOLVER) || (defined AUX_RESOLVER) */


/**
  * @}
  */

/**
  * @}
  */

/** @} */


/******************* (C) COPYRIGHT 2023 STMicroelectronics *****END OF FILE****/