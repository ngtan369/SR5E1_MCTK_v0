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
*   @file    dac_rctimer_ui.h
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
/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __DACRCTIMERUI_H
#define __DACRCTIMERUI_H

#include "user_interface.h"

#ifdef __cplusplus
 extern "C" {
#endif /* __cplusplus */

/** @addtogroup STELLAR_PMSM_MC_UI_LIBRARY
  * @{
  */

/**
 * @addtogroup MCUI
 * @{
 */

/** @addtogroup dac_rctimer_ui
  * @{
  */

#define DAC_CH_NBR 2
#define DAC_CH_USER 2



void DACT_Init(UI_Handle_t *pHandle);

void DACT_Exec(UI_Handle_t *pHandle);


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

#ifdef __cplusplus
}
#endif /* __cpluplus */

#endif /* __DACRCTIMERUI_H */

 /************************ (C) COPYRIGHT 2018 STMicroelectronics *****END OF FILE****/
