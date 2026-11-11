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
*   @file    motorcontrol.c
*   @version M4_VERSION
*   @brief   Motor Control Subsystem initialization functions.
*
*   @details Motor Control Subsystem initialization functions.
 */

#include "main.h"
#include "mc_tuning.h"
#include "mc_interface.h"
#include "mc_tasks.h"
#include "ui_task.h"
#include "motorcontrol.h"
#include "clock.h"

/** @addtogroup STELLAR_PMSM_MC_APPLICATION
  * @{
  */

/** @addtogroup MCAPI
  * @{
  */

#define FIRMWARE_VERS "MCTK FOC SW LIB\tRTM.1.3.B"
const char s_fwVer[32] = FIRMWARE_VERS;

MCI_Handle_t* pMCI[NBR_OF_MOTORS];
MCT_Handle_t* pMCT[NBR_OF_MOTORS];
uint32_t wConfig[NBR_OF_MOTORS] = {UI_CONFIG_M1,UI_CONFIG_M2};

/**
 * @brief Initializes and configures the Motor Control Subsystem
 *
 *  This function initializes and configures all the structures and components needed
 * for the Motor Control subsystem required by the Application. It expects that
 * all the peripherals needed for Motor Control purposes are already configured but
 * that their interrupts are not enabled yet.
 *
 * CubeMX calls this function after all peripherals initializations and
 * before the NVIC is configured
 */
__weak void MX_MotorControl_Init(void)
{

  /* Peripheral clock enable */
  clock_enable_CORDIC(true);
  
  /* Reconfigure the SysTick interrupt to fire every 500 us. */
  SysTick_Config(SystemCoreClock/SYS_TICK_FREQUENCY);
  /* Initialize the Motor Control Subsystem */
  MCboot(pMCI,pMCT);

  /* Initialize the MC User Interface */
  UI_TaskInit(wConfig,NBR_OF_MOTORS,pMCI,pMCT,s_fwVer);
}

/**
  * @}
  */

/**
  * @}
  */

/******************* (C) COPYRIGHT 2023 STMicroelectronics *****END OF FILE****/
