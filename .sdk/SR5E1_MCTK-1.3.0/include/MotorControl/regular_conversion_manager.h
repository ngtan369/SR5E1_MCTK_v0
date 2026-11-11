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
*   @file    regular_conversion_manager.h
*   @version M4_VERSION
*   @brief   This file provides firmware functions that implement the features
*            of the regular_conversion_manager component of the Stellar Motor Control.
*
*   @details This file provides firmware functions that implement the features
*            of the regular_conversion_manager component of the Stellar Motor Control:
*
*            - Register conversion with or without callback
*            - Execute regular conv directly from Temperature and VBus sensors
*            - Execute user regular conversion scheduled by medium frequency task
*            - Manage user conversion state machine
*
 */
/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __regular_conversion_manager_h
#define __regular_conversion_manager_h

#ifdef __cplusplus
 extern "C" {
#endif /* __cplusplus */

/* Includes ------------------------------------------------------------------*/
#include "stdint.h"
#include "stdbool.h"
#include "mc_stm_types.h"

/** @addtogroup STELLAR_PMSM_MC_FOC_LIBRARY
  * @{
  */

/** @addtogroup RCM
  * @{
  */

/* Exported types ------------------------------------------------------------*/

/**
  * @brief RegConv_t contains all the parameters required to execute a regular conversion
  *
  * it is used by all regular_conversion_manager's client
  *
  */
typedef struct
{
  ADC_TypeDef * regADC;
  uint8_t  channel;
  uint32_t samplingTime;
} RegConv_t;

typedef enum
{
  RCM_USERCONV_IDLE,
  RCM_USERCONV_REQUESTED,
  RCM_USERCONV_EOC
}RCM_UserConvState_t;

typedef void (*RCM_exec_cb_t)(uint8_t handle, uint16_t data, void *UserData);

/* Exported functions ------------------------------------------------------- */

/*  Function used to register a regular conversion */
uint8_t RCM_RegisterRegConv(RegConv_t * regConv);

/*  Function used to register a regular conversion with a callback attached*/
uint8_t RCM_RegisterRegConv_WithCB (RegConv_t * regConv, RCM_exec_cb_t fctCB, void *data);

/*  Function used to execute an already registered regular conversion */
uint16_t RCM_ExecRegularConv (uint8_t handle);

/* select the handle conversion to be executed during the next call to RCM_ExecUserConv */
bool RCM_RequestUserConv(uint8_t handle);

/* return the latest user conversion value*/
uint16_t RCM_GetUserConv(void);

/* Must be called by MC_TASK only to grantee proper scheduling*/
void RCM_ExecUserConv (void);

/* return the state of the user conversion state machine*/
RCM_UserConvState_t RCM_GetUserConvState(void);

/* Function used to un-schedule a regular conversion exectuted after current sampling in HF task */
bool RCM_PauseRegularConv (uint8_t handle);

/* non blocking function to start conversion inside HF task */
void RCM_ExecNextConv (void);

/* non blocking function used to read back already started regular conversion*/
void RCM_ReadOngoingConv (void);

/**
  * @}
  */

/**
  * @}
  */

#ifdef __cplusplus
}
#endif /* __cpluplus */

#endif /* __regular_conversion_manager_h */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
