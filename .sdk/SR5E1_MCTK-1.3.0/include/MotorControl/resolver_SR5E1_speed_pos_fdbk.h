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
*   @file    resolver_SR5E1_speed_pos_fdbk.h
*   @version M4_VERSION
*   @brief   This file provides firmware functions that implement the features
*            of the resolver component of the Stellar Motor Control.
*
*   @details This file provides firmware functions that implement the features
*            of the resolver component of the Stellar Motor Control.
*
*   @ingroup SpeednPosFdbk
*
 */
#ifndef __RESOLVER_SR5E1_SPEEDNPOSFDBKCLASS_H
#define __RESOLVER_SR5E1_SPEEDNPOSFDBKCLASS_H

/** @addtogroup STELLAR_PMSM_MC_FOC_LIBRARY
  * @{
  */
  
/** @addtogroup SpeednPosFdbk
   * @{
   */  

/** @addtogroup Resolver
  * @{
  */


#include "MC_type.h"
#include "PLL.h"
#include "maximizer.h"
#include "speed_pos_fdbk.h"

typedef struct
{
  SpeednPosFdbk_Handle_t _Super;
  ADC_TypeDef * ADCx_SIN_COS;        /*!< ADC SIN/COS peripherals - used only for single ADC*/
  ADC_TypeDef * ADCx_SIN;        /*!< ADC SIN peripheral */
  ADC_TypeDef * ADCx_COS;        /*!< ADC COS peripheral*/
  DAC_TypeDef * DACx_ExcSignal;  /*!< DAC peripheral used for exciting signal */
  DMA_TypeDef * DMAx_DAC;        /*!< DMA peripheral used to update output register of the DAC (exciting signal) */
  TIM_TypeDef * TIMx_DAC;        /*!< High frequency timer to clock the BDAC update of values */
  TIM_TypeDef * TIMx_ADC;        /*!< High frequency timer to clock the BDAC update of values */
 
  PLL pll;
  maximizer_t max_sinAmp;
  maximizer_t max_speed;
  int16_t  hResAngle;
  int16_t  hResSpdDPP;
  int16_t  hAvrgResSpdRPM;
  int16_t  hAvrgMechSpdRPM;
  int32_t  wResSpdRPMSum;
  uint16_t hResSpdRPMCnt;
  float    fMaxSpeedRPM;
  
  uint16_t hSin[64];
  uint32_t hResSinCos[4]; // Sin, Cos, BUS, Temp
  uint8_t  bNConversions;
  int32_t wMod;
  float fExcitingFrequency;
  float fSetExcitingFrequency;
  float fExciting_ampl_percentage;
  int16_t hResolverInvertDirection;
  float fOffset;
  uint16_t hTim15ccr1;
  uint8_t bResolverToMechanicalRatio;

  int16_t testMode;
  uint8_t bDacCh;
  uint8_t bDacOffline;
  uint8_t bBootCompleted;
  
  uint8_t bVBusADC_Channel;
  uint8_t bTempADC_Channel;
  
  // Open Loop
  int16_t  hOLCurrentS16;
  uint32_t wTimeCnt;
  float    fOLTetha;
  float    fOLOmegaDpp;
  float    fOLAlphaDppDpp;
  float    fOLEndSpeedRPM;
  float    fOLEndSpeedDpp;
  float    fOlStartDuration;
  uint32_t wDurationCnt;
} Resolver_SR5E1_Handle_t;

void RES_Init(Resolver_SR5E1_Handle_t *this);
void RES_Clear(Resolver_SR5E1_Handle_t *this);
void RES_IRQHandler(Resolver_SR5E1_Handle_t *this);
uint16_t RES_ExecRegularConv(Resolver_SR5E1_Handle_t *this, uint8_t bChannel);
void RES_BootComplete(Resolver_SR5E1_Handle_t *this);
void RES_UpdateFrequency(Resolver_SR5E1_Handle_t * this, float excitingFrequency);
void RES_UpdateModulation(Resolver_SR5E1_Handle_t *this, int32_t mod);
bool RES_GetAvrgMecSpeed01Hz(Resolver_SR5E1_Handle_t *this, int16_t *pMecSpeed01Hz);
int16_t RES_GetElAngle(Resolver_SR5E1_Handle_t *this);
void RES_SetPhaseShift(float PhaseShift);
void RES_GetPhaseShift(float *phaseShift);
void RES_SetDemodulationTriggerDelay(Resolver_SR5E1_Handle_t *this, uint16_t DemTrgDelay);
void RES_GetDemodulationTriggerDelay(uint16_t *DemTrgDelay);
void RES_GetResADCMaxResolution(uint16_t *ResMaxADCResolution);

/**
  * @}
  */

/**
  * @}
  */

/** @} */

#endif /* __RESOLVER_SR5E1_SPEEDNPOSFDBKCLASS_H */