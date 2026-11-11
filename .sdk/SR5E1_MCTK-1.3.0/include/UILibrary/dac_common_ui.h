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
*   @file    dac_common_ui.h
*   @version M4_VERSION
*   @brief   This file provides firmware functions that implement the features
*            of the DAC component of the Stellar Motor Control.
*          
*   @details This file provides firmware functions that implement the features
*            of the DAC component of the Stellar Motor Control:
*             + Set up the DAC outputs
*             + get the current DAC channel selected output.
*             + Set the value of the "User DAC channel".
*             + get the value of the "User DAC channel".
*/
/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __DACCOMMONUI_H
#define __DACCOMMONUI_H

#ifdef __cplusplus
 extern "C" {
#endif /* __cplusplus */

/* Includes ------------------------------------------------------------------*/
#include "user_interface.h"


/** @addtogroup STELLAR_PMSM_MC_UI_LIBRARY
  * @{
  */

/**
 * @addtogroup MCUI
 * @{
 */

/** @addtogroup dac_common_ui
  * @{
  */

#define DAC_CH_NBR 2
#define DAC_CH_USER 2



typedef struct
{
  UI_Handle_t _Super;      							/*!<   */

  MC_Protocol_REG_t bChannel_variable[DAC_CH_NBR]; /*!< Array of codes of
                                                        variables to be provided
                                                        in out to the related
                                                        channel.*/
  int16_t hUserValue[DAC_CH_USER];                 /*!< Array of user defined
                                                        DAC values.*/

  uint16_t hDAC_CH1_ENABLED;  /*!< Set to ENABLE to assign the channel 1 to the
                                   DAC object otherwise set DISABLE */
  uint16_t hDAC_CH2_ENABLED;  /*!< Set to ENABLE to assign the channel 2 to the
                                   DAC object otherwise set DISABLE */
} DAC_UI_Handle_t;


void DAC_SetChannelConfig(UI_Handle_t *pHandle, DAC_Channel_t bChannel,
                              MC_Protocol_REG_t bVariable);

MC_Protocol_REG_t DAC_GetChannelConfig(UI_Handle_t *pHandle, DAC_Channel_t bChannel);

void DAC_SetUserChannelValue(UI_Handle_t *pHandle, uint8_t bUserChNumber,
                              int16_t hValue);

int16_t DAC_GetUserChannelValue(UI_Handle_t *pHandle, uint8_t bUserChNumber);

/** @} */
/** @} */
/** @} */
/** @} */

#ifdef __cplusplus
}
#endif /* __cpluplus */

#endif /* __DACCOMMONUI_H */

 /************************ (C) COPYRIGHT 2018 STMicroelectronics *****END OF FILE****/
