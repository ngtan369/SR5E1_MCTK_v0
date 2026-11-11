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
*   @file    motorcontrol.h
*   @version M4_VERSION
*   @brief   Motor Control Subsystem initialization functions.
*
*   @details Motor Control Subsystem initialization functions.
*
*   @ingroup MCAPI
 */
/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __MOTORCONTROL_H
#define __MOTORCONTROL_H
#include "mc_config.h"
#include "parameters_conversion.h"
#include "mc_api.h"

#ifdef __cplusplus
 extern "C" {
#endif /* __cplusplus */

/** @addtogroup STELLAR_PMSM_MC_UI_LIBRARY
  * @{
  */

/** @addtogroup MCAPI
  * @{
  */

/* Initializes the Motor Control Subsystem */
void MX_MotorControl_Init(void);

/* Do not remove the definition of this symbol. */
#define MC_HAL_IS_USED
/**
  * @}
  */

/**
  * @}
  */

#ifdef __cplusplus
}
#endif /* __cpluplus */

#endif /* __MOTORCONTROL_H */
/******************* (C) COPYRIGHT 2023 STMicroelectronics *****END OF FILE****/
