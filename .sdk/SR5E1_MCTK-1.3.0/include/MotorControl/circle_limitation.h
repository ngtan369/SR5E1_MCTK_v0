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
*   @file    circle_limitation.h
*   @version M4_VERSION
*   @brief   This file provides the functions that implement the circle
*            limitation feature of the Stellar Motor Control.
*          
*   @details This file provides the functions that implement the circle
*            limitation feature of the Stellar Motor Control
*
*   @ingroup CircleLimitation
 */
/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __CIRCLELIMITATION_H
#define __CIRCLELIMITATION_H

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

/* Includes ------------------------------------------------------------------*/
#include "mc_type.h"

/** @addtogroup STELLAR_PMSM_MC_FOC_LIBRARY
  * @{
  */

/** @addtogroup CircleLimitation
  * @{
  */

/**
  * @brief  CircleLimitation component parameters definition
  */
typedef struct
{
  uint16_t MaxModule;               /**<  Circle limitation maximum allowed
                                         module */
  uint16_t MaxVd;                   /**<  Circle limitation maximum allowed
                                         module */
  uint16_t Circle_limit_table[87];  /**<  Circle limitation table */
  uint8_t  Start_index;             /**<  Circle limitation table indexing
                                         start */
} CircleLimitation_Handle_t;

/* Exported functions ------------------------------------------------------- */

qd_t Circle_Limitation( CircleLimitation_Handle_t * pHandle, qd_t Vqd );

/**
  * @}
  */

/**
  * @}
  */

#ifdef __cplusplus
}
#endif /* __cpluplus */

#endif /* __Circle Limitation_H */

/************************ (C) COPYRIGHT 2023 STMicroelectronics *****END OF FILE****/

