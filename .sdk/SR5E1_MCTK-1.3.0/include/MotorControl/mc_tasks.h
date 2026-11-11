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
*   @file    mc_tasks.h
*   @version M4_VERSION
*   @brief   This file implements tasks definition.
*
*   @details This file implements tasks definition.
*
*   @ingroup MCTasks
 */
/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __MCTASKS_H
#define __MCTASKS_H

/* Includes ------------------------------------------------------------------*/
#include "mc_tuning.h"
#include "mc_interface.h"

/** @addtogroup STELLAR_PMSM_MC_APPLICATION
  * @{
  */

/** @defgroup MCTasks Motor Control Tasks
  *
  * @brief Motor Control subsystem configuration and operation routines.
  *
  * @{
  */

/* Initializes the Motor subsystem core according to user defined parameters. */
void MCboot( MCI_Handle_t* pMCIList[], MCT_Handle_t* pMCTList[] );

/* Runs all the Tasks of the Motor Control cockpit */
void MC_RunMotorControlTasks(void);

/* Executes the Medium Frequency Task functions for each drive instance */
void MC_Scheduler(void);

/* Executes safety checks (e.g. bus voltage and temperature) for all drive instances */
void TSK_SafetyTask(void);

/* Executes the Motor Control duties that require a high frequency rate and a precise timing */
uint8_t TSK_HighFrequencyTask(void);

/* Reserves FOC execution on ADC ISR half a PWM period in advance */
void TSK_DualDriveFIFOUpdate(uint8_t Motor);

/* Puts the Motor Control subsystem in in safety conditions on a Hard Fault */
void TSK_HardwareFaultTask(void);

/**
  * @}
  */

/**
  * @}
  */

#endif /* __MCTASKS_H */

/******************* (C) COPYRIGHT 2023 STMicroelectronics *****END OF FILE****/
