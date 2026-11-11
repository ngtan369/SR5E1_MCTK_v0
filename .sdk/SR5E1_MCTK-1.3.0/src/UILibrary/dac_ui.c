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
*   @file    dac_ui.c
*   @version M4_VERSION
*   @brief   This file provides firmware functions that implement the features
*            of the DAC component of the Stellar Motor Control.
*          
*   @details This file provides firmware functions that implement the features
*            of the DAC component of the Stellar Motor Control:
*              + dac initialization
*              + dac execution
*
*/
/* Includes ------------------------------------------------------------------*/
#include "dac_common_ui.h"
#include "dac_rctimer_ui.h"
#include "dac_ui.h"
#include "mc_config.h"
#include "clock.h"

#define DACOFF 32768

/** @addtogroup STELLAR_PMSM_MC_UI_LIBRARY
  * @{
  */

/** @addtogroup MCUI
  * @{
  */
#ifndef TIM_DAC
/**
 * @defgroup DAC_UserInterface DAC User Interface
 *
 * @brief DAC User Interface
 *
 * @todo Complete Documentation. What difference with dac_common_ui?
 * @{
 */

/**
  * @brief  Hardware and software initialization of the DAC object. This is the
  *         implementation of the virtual function.
  * @param  pHandle pointer on related component instance.
  */
__weak void DAC_Init(UI_Handle_t *pHandle)
{
  DAC_UI_Handle_t *pDacHandle = (DAC_UI_Handle_t *)pHandle;
  LL_DAC_InitTypeDef DAC_InitStruct = {0};
  
  clock_enable_BDAC1(true);
  
  if (pDacHandle->hDAC_CH1_ENABLED == TRUE)
  {  
     LL_DAC_SetSignedFormat(DAC_MODULE, DAC_RESOURCE_CH1, LL_DAC_SIGNED_FORMAT_DISABLE);
     DAC_InitStruct.TriggerSource = LL_DAC_TRIG_SOFTWARE;
     DAC_InitStruct.TriggerSource2 = LL_DAC_TRIG_SOFTWARE;
     DAC_InitStruct.WaveAutoGeneration = LL_DAC_WAVE_AUTO_GENERATION_NONE;
     DAC_InitStruct.OutputBuffer = LL_DAC_OUTPUT_BUFFER_DISABLE;
     DAC_InitStruct.OutputConnection = LL_DAC_OUTPUT_CONNECT_GPIO;
     DAC_InitStruct.OutputMode = LL_DAC_OUTPUT_MODE_NORMAL;
     LL_DAC_Init(DAC_MODULE, DAC_RESOURCE_CH1, &DAC_InitStruct);
     LL_DAC_EnableTrigger(DAC_MODULE, DAC_RESOURCE_CH1);
     LL_DAC_DisableDMADoubleDataMode(DAC_MODULE, DAC_RESOURCE_CH1);
     /* Enable DAC Channel1 */
     LL_DAC_Enable(DAC_MODULE, DAC_RESOURCE_CH1);
  }
  if (pDacHandle->hDAC_CH2_ENABLED == TRUE)
  {  
     LL_DAC_SetSignedFormat(DAC_MODULE, DAC_RESOURCE_CH2, LL_DAC_SIGNED_FORMAT_DISABLE);
     DAC_InitStruct.TriggerSource = LL_DAC_TRIG_SOFTWARE;
     DAC_InitStruct.TriggerSource2 = LL_DAC_TRIG_SOFTWARE;
     DAC_InitStruct.WaveAutoGeneration = LL_DAC_WAVE_AUTO_GENERATION_NONE;
     DAC_InitStruct.OutputBuffer = LL_DAC_OUTPUT_BUFFER_DISABLE;
     DAC_InitStruct.OutputConnection = LL_DAC_OUTPUT_CONNECT_GPIO;
     DAC_InitStruct.OutputMode = LL_DAC_OUTPUT_MODE_NORMAL;
     LL_DAC_Init(DAC_MODULE, DAC_RESOURCE_CH2, &DAC_InitStruct);
     LL_DAC_EnableTrigger(DAC_MODULE, DAC_RESOURCE_CH2);
     LL_DAC_DisableDMADoubleDataMode(DAC_MODULE, DAC_RESOURCE_CH2);
     /* Enable DAC Channel1 */
     LL_DAC_Enable(DAC_MODULE, DAC_RESOURCE_CH2);
  }  
}

/**
  * @brief  This method is used to update the DAC outputs. The selected
  *         variables will be provided in the related output channels. This is
  *         the implementation of the virtual function.
  * @param  pHandle pointer on related component instance.
  */
__weak void DAC_Exec(UI_Handle_t *pHandle)
{
  DAC_UI_Handle_t *pDacHandle = (DAC_UI_Handle_t *)pHandle;
  MC_Protocol_REG_t bCh1_var;
  MC_Protocol_REG_t bCh2_var;

  bCh1_var = pDacHandle->bChannel_variable[DAC_CH0];
  bCh2_var = pDacHandle->bChannel_variable[DAC_CH1];
  if (pDacHandle->hDAC_CH1_ENABLED == TRUE)
  {
     LL_DAC_ConvertData12LeftAligned(DAC_MODULE, DAC_RESOURCE_CH1,
                                     DACOFF + ((int16_t)UI_GetReg(pHandle,bCh1_var,MC_NULL)));
     LL_DAC_TrigSWConversion(DAC_MODULE, DAC_RESOURCE_CH1);                                     
  }
  if (pDacHandle->hDAC_CH2_ENABLED == TRUE)
  {    
    LL_DAC_ConvertData12LeftAligned(DAC_MODULE, DAC_RESOURCE_CH2,
                                     DACOFF + ((int16_t)UI_GetReg(pHandle,bCh2_var,MC_NULL)));
    LL_DAC_TrigSWConversion(DAC_MODULE, DAC_RESOURCE_CH2);
  }
}

#endif
/**
  * @}
  */

/**
  * @}
  */

/**
  * @}
  */

/**
 * @}
 */

/******************* (C) COPYRIGHT 2023 STMicroelectronics *****END OF FILE****/
