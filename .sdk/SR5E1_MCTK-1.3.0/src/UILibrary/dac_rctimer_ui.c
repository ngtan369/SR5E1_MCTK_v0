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
*   @file    dac_rctimer_ui.c
*   @version M4_VERSION
*   @brief   This file provides firmware functions that implement the features
*            of the DAC component using PWM/TIM modules of the Stellar Motor Control.
*          
*   @details This file provides firmware functions that implement the features
*            of the DAC component using PWM/TIM modules of theStellar Motor Control:
*             + Set up the DAC/TIM outputs
*             + get the current DAC channel selected output.
*             + Set the value of the "User DAC channel".
*             + get the value of the "User DAC channel".
*/
/* Includes ------------------------------------------------------------------*/
#include "dac_rctimer_ui.h"
#include "dac_common_ui.h"
#include "mc_config.h"
#include "clock.h"
/** @addtogroup STELLAR_PMSM_MC_UI_LIBRARY
  * @{
  */

/**
 * @addtogroup MCUI
 * @{
 */
#ifdef TIM_DAC
/** @defgroup dac_rctimer_ui DAC RC Timer User Interface
  *
  * @brief RC Timer DAC User Interface implementation
  *
  * Digital to Analog Converter common component. In Motor Control context, the DAC is used for
  * debug purposes by outputting information with low overhead. It is particularly useful to
  * output fast changing analog-like data as voltage references or measured currents to compare
  * them with measurements made with other means, for instance.
  *
  * The RC Timer DAC component aims at using a PWM Timer coupled with an RC network as a DAC
  *
  *
  * @{
  */

/**
  * @brief  Hardware and software initialization of the DAC object. This is the
  *         implementation of the virtual function.
  * @param  pHandle related component instance.
  */
__weak void DACT_Init(UI_Handle_t *pHandle)
{
  LL_TIM_InitTypeDef TIM_InitStruct = {0};
  LL_TIM_OC_InitTypeDef TIM_OC_InitStruct = {0};
  DAC_UI_Handle_t *pDacHandle = (DAC_UI_Handle_t *)pHandle;

  /* Peripheral clock enable */
  if(DAC_MODULE == TIM2)
  {
     clock_enable_TIM2(true);
  }
  else if (DAC_MODULE == TIM3)
  {
     clock_enable_TIM3(true);
  }
  else if (DAC_MODULE == TIM4)
  {
     clock_enable_TIM4(true);
  }
  else if (DAC_MODULE == TIM5)
  {
     clock_enable_TIM5(true);
  }
  else
  {
  }  
  TIM_InitStruct.Prescaler = 0;
  TIM_InitStruct.CounterMode = LL_TIM_COUNTERMODE_UP;
  TIM_InitStruct.Autoreload = 0x800;
  TIM_InitStruct.ClockDivision = LL_TIM_CLOCKDIVISION_DIV1;
  LL_TIM_Init(DAC_MODULE, &TIM_InitStruct);
  LL_TIM_DisableARRPreload(DAC_MODULE);

  TIM_OC_InitStruct.OCMode = LL_TIM_OCMODE_PWM1;
  TIM_OC_InitStruct.OCState = LL_TIM_OCSTATE_DISABLE;
  TIM_OC_InitStruct.OCNState = LL_TIM_OCSTATE_DISABLE;
  TIM_OC_InitStruct.CompareValue = 0x400;
  TIM_OC_InitStruct.OCPolarity = LL_TIM_OCPOLARITY_HIGH;
  
  if (pDacHandle->hDAC_CH1_ENABLED == TRUE)
  {
     /* CH1 configuration  */
     LL_TIM_OC_EnablePreload(DAC_MODULE, DAC_RESOURCE_CH1);
     LL_TIM_OC_Init(DAC_MODULE, DAC_RESOURCE_CH1, &TIM_OC_InitStruct);
     LL_TIM_OC_DisableFast(DAC_MODULE, DAC_RESOURCE_CH1);
     
     LL_TIM_CC_EnableChannel(DAC_MODULE, DAC_RESOURCE_CH1);
     LL_TIM_OC_DisablePreload(DAC_MODULE, DAC_RESOURCE_CH1);
  }
  if (pDacHandle->hDAC_CH2_ENABLED == TRUE)
  {
     /* CH2 configuration  */
     LL_TIM_OC_EnablePreload(DAC_MODULE, DAC_RESOURCE_CH2);
     LL_TIM_OC_Init(DAC_MODULE, DAC_RESOURCE_CH2, &TIM_OC_InitStruct);
     LL_TIM_OC_DisableFast(DAC_MODULE, DAC_RESOURCE_CH2);
     
     LL_TIM_CC_EnableChannel(DAC_MODULE, DAC_RESOURCE_CH2);
     LL_TIM_OC_DisablePreload(DAC_MODULE, DAC_RESOURCE_CH2);
  }

  LL_TIM_SetTriggerOutput(DAC_MODULE, LL_TIM_TRGO_RESET);
  LL_TIM_DisableMasterSlaveMode(DAC_MODULE);
    
  /* start Timer */
  LL_TIM_EnableCounter(DAC_MODULE);

}

/**
  * @brief  This method is used to update the DAC outputs. The selected
  *         variables will be provided in the related output channels. This is
  *         the implementation of the virtual function.
  * @param  pHandle related component instance.
  * @retval none.
  */
__weak void DACT_Exec(UI_Handle_t *pHandle)
{
  DAC_UI_Handle_t *pDacHandle = (DAC_UI_Handle_t *)pHandle;
  MC_Protocol_REG_t bCh1_var;
  MC_Protocol_REG_t bCh2_var;

  bCh1_var = pDacHandle->bChannel_variable[DAC_CH0];
  bCh2_var = pDacHandle->bChannel_variable[DAC_CH1];
  if (pDacHandle->hDAC_CH1_ENABLED == TRUE)
  {  
     LL_TIM_OC_SetCompareCH1(DAC_MODULE, ((uint16_t)((int16_t)(((int16_t)UI_GetReg(pHandle,bCh1_var,MC_NULL)+32768)/32))));
  }
  if (pDacHandle->hDAC_CH2_ENABLED == TRUE)
  {
     LL_TIM_OC_SetCompareCH2(DAC_MODULE, ((uint16_t)((int16_t)(((int16_t)UI_GetReg(pHandle,bCh2_var,MC_NULL)+32768)/32))));
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

/******************* (C) COPYRIGHT 2023 STMicroelectronics *****END OF FILE****/
