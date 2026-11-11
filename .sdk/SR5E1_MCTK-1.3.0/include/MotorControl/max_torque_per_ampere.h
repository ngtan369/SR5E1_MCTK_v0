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
*   @file    max_torque_per_ampere.h
*   @version M4_VERSION
*   @brief   This file provides firmware functions that implement the features
*            of the Maximum Torque Per Ampere (MTPA) Control component of the Stellar Motor Control.
*
*   @details This file provides firmware functions that implement the features
*            of the Maximum Torque Per Ampere (MTPA) Control component of the Stellar Motor Control:
*
*            - Initialize the parameter for  MTPA
*            - Calculate and output id and iq reference based on torque input
*            - Calculate and output id based on iq input
*
*   @ingroup MTPA
 */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __MAXTORQUEPERAMPERE_H
#define __MAXTORQUEPERAMPERE_H

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

/* Includes ------------------------------------------------------------------*/
#include "mc_type.h"
/** @addtogroup STELLAR_PMSM_MC_FOC_LIBRARY
  * @{
  */

/** @addtogroup MTPA Maximum Torque Per Ampere Control
  * @{
  */

/* Exported types ------------------------------------------------------------*/
#define SEGMENT_NUM         ((uint8_t)7)          /* coeff no. -1 */
#define MTPA_ARRAY_SIZE     SEGMENT_NUM+1
/**
 * @brief  Handle structure of max_torque_per_ampere.c
 */
typedef struct
{
  int16_t  SegDiv;               /**< Segments divisor */
  int32_t  AngCoeff[MTPA_ARRAY_SIZE];          /**< Angular coefficients table */
  int32_t  Offset[MTPA_ARRAY_SIZE];            /**< Offsets table */
} MTPA_Handle_t;

/* Exported functions ------------------------------------------------------- */

/*  Function used to set configure an instance of the CCC Component *****/
void MTPA_CalcCurrRefFromIq( MTPA_Handle_t * pHandle, qd_t *Iqdref );

/**
  * @}
  */

/**
  * @}
  */

#ifdef __cplusplus
}
#endif /* __cpluplus */

#endif /* __MAXTORQUEPERAMPERE_H */

/************************ (C) COPYRIGHT 2023 STMicroelectronics *****END OF FILE****/
