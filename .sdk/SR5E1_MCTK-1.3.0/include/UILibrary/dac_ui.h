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
*   @file    dac_ui.u
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
/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __DACUI_H
#define __DACUI_H

#ifdef __cplusplus
 extern "C" {
#endif /* __cplusplus */

/* Includes ------------------------------------------------------------------*/
#include "user_interface.h"
#include "dac_common_ui.h"


 /** @addtogroup STELLAR_PMSM_MC_UI_LIBRARY
   * @{
   */

 /** @addtogroup MCUI
   * @{
   */

 /** @addtogroup DAC_UserInterface
   * @{
   */

#define DAC_CH_NBR 2
#define DAC_CH_USER 2


void DAC_Init(UI_Handle_t *pHandle);

void DAC_Exec(UI_Handle_t *pHandle);

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

#endif /*__DACUI_H*/

/******************* (C) COPYRIGHT 2023 STMicroelectronics *****END OF FILE****/
