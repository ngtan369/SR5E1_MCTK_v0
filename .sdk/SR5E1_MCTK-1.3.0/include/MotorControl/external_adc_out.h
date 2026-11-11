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
*   @file    external_adc_out.h
*   @version M4_VERSION
*   @brief   This file provides firmware functions that implement the features
*            of the External ADC component of the Stellar Motor Control.
*          
*   @details This file provides firmware functions that implement the features
*            of the External ADC component of the Stellar Motor Control.
 */
/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __EXTERNALADCOUT_H
#define __EXTERNALADCOUT_H

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

/* Includes ------------------------------------------------------------------*/
#include "mc_type.h"

/** @addtogroup STELLAR_PMSM_MC_FOC_LIBRARY
  * @{
  */

/** @addtogroup ExternalADCOut
  * @{
  */

/**
  * @brief  ExternalADCOut handle definition
  */
typedef struct
{

  TIM_TypeDef * TIMx;   /*!< Timer used for ADC Input Capture */
  uint8_t InputChannel; /*!< Input Capture channel */
  uint32_t InputTimFilter;     /*!< Input Filter */
  uint8_t MeasuredChannel; /*!< Measuered channel */
  uint16_t ADC_res;     /*!< ADC resolution */
  float ADC_min_duty;     /*!< ADC min duty */
  float ADC_max_duty;     /*!< ADC max duty */
  volatile uint16_t ADC_OUT_value; /* ADC out value */
} Ext_ADC_OUT_t;


/* Exported functions ------------------------------------------------------- */
void ExtADC_Init(Ext_ADC_OUT_t * pHandle);
uint16_t ExtADC_GetADCValue(Ext_ADC_OUT_t * pHandle);
void * ExtADC_IRQHandler( void * pHandleVoid );
uint16_t ExtADC_approx(Ext_ADC_OUT_t * pHandle, float value);
/**
  * @}
  */

/**
  * @}
  */

#ifdef __cplusplus
}
#endif /* __cpluplus */

#endif /* __ExternalAdcOut_H */

/************************ (C) COPYRIGHT 2023 STMicroelectronics *****END OF FILE****/
