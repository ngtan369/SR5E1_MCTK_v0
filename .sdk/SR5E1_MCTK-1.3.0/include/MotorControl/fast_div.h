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
*   @file    fast_div.h
*   @version M4_VERSION
*   @brief   This file contains all definitions and functions prototypes for the
*            Fast Division component of the Stellar Motor Control.
*
*   @details This file contains all definitions and functions prototypes for the
*            Fast Division component of the Stellar Motor Control.
*
*   @ingroup FD
 */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __FAST_DIV_H
#define __FAST_DIV_H

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

/* Includes ------------------------------------------------------------------*/
#include "mc_type.h"

/** @addtogroup STELLAR_PMSM_MC_FOC_LIBRARY
  * @{
  */

/* Exported constants --------------------------------------------------------*/
/// Number of divisors that can be stored for fast division
#define FD_MAX_FDIV 5

/* Exported types ------------------------------------------------------------*/

/// Magic number structure (used for software fast division)
typedef struct
{
  int32_t M;
  int32_t s;
  int32_t d;
} FD_FastDivMagicNumber_t;

/// Handle structure of the Fast Division component
typedef struct
{
  int8_t fd_div_element;                  /*@< Pointer to the stored dividers buffer.*/
  FD_FastDivMagicNumber_t fd_m[FD_MAX_FDIV]; /*@< Struct of stored magic numbers.*/
} FastDiv_Handle_t;

/* Exported functions ------------------------------------------------------- */

void FD_Init( FastDiv_Handle_t * pHandle );
int32_t FD_FastDiv( FastDiv_Handle_t * pHandle, int32_t n, int32_t d );


/**
  * @}
  */

/**
  * @}
  */

#ifdef __cplusplus
}
#endif /* __cpluplus */

#endif /* __FAST_DIV_H */

/************************ (C) COPYRIGHT 2023 STMicroelectronics *****END OF FILE****/
