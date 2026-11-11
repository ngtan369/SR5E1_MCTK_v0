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
*   @file    inrush_current_limiter.h
*   @version M4_VERSION
*   @brief   This file provides firmware functions implementing the
*            InrushCurrentLimiter feature of the Stellar Motor Control.
*
*   @details This file provides firmware functions implementing the
*            InrushCurrentLimiter feature of the Stellar Motor Contro:
*
*            - ICL_Init() to initialize dedicated variables
*            - ICL_Exec() to manage the ICL state machine
*            - ICL_GetState() to get the current ICL state machine
*
*   @ingroup ICL
 */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __INRUSHCURRENTLIMITER_H
#define __INRUSHCURRENTLIMITER_H

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

/* Includes ------------------------------------------------------------------*/
#include "mc_type.h"
#include "bus_voltage_sensor.h"
#include "digital_output.h"

/** @addtogroup STELLAR_PMSM_MC_FOC_LIBRARY
  * @{
  */

/** @addtogroup ICL
  * @{
  */

/* Exported types ------------------------------------------------------------*/
/*
  * @brief ICL_State_t defines all the existing ICL states of the state machine
  */
typedef enum
{
  ICL_IDLE,             /* stable state */
  ICL_ACTIVATION,       /* transition state */
  ICL_ACTIVE,           /* stable state */
  ICL_DEACTIVATION,     /* transition state */
  ICL_INACTIVE          /* stable state */
} ICL_State_t;


/**
  * @brief  ICL_Handle_t is used to handle an instance of the InrushCurrentLimiter component
  */
typedef struct
{
  BusVoltageSensor_Handle_t * pVBS; /*!< CVBS object used for the automatic ICL component activation/deactivation */
  DOUT_handle_t * pDOUT;                 /*!< DOUT object used to physically activate/deactivate the ICL component */

  ICL_State_t ICLstate;         /*!< Current state of the ICL state machine */
  uint16_t hICLTicksCounter;    /*!< Number of clock events remaining to complete the ICL activation/deactivation */
  uint16_t hICLTotalTicks;      /*!< Total number of clock events to complete the ICL activation/deactivation */
  uint16_t hICLFrequencyHz;     /*!< Clock frequency used (Hz) to trigger the ICL_Exec() method */
  uint16_t hICLDurationms;      /*!< ICL activation/deactivation duration (ms)*/
} ICL_Handle_t;


/* Exported constants --------------------------------------------------------*/
/* Exported macro ------------------------------------------------------------*/
/* Exported functions ------------------------------------------------------- */
void ICL_Init( ICL_Handle_t * pHandle, BusVoltageSensor_Handle_t * pVBS, DOUT_handle_t * pDOUT );
ICL_State_t ICL_Exec( ICL_Handle_t * pHandle );
ICL_State_t ICL_GetState( ICL_Handle_t * pHandle );

/**
  * @}
  */

/**
  * @}
  */

#ifdef __cplusplus
}
#endif /* __cpluplus */

#endif /* __INRUSHCURRENTLIMITER_H */

/************************ (C) COPYRIGHT 2023 STMicroelectronics *****END OF FILE****/
