/*******************************************************************************
  * @file    anti_rollback.h
  * @brief   Header file for Anti-Rollback / Hill Start Assist (HSA) Controller
  * @details Declarations and public APIs for zero-speed torque control.
  *****************************************************************************/

#ifndef ANTI_ROLLBACK_H
#define ANTI_ROLLBACK_H

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include <stdint.h>
#include <stdbool.h>

/* Exported Types ------------------------------------------------------------*/
#ifndef MISRA_FLOAT32_T_DEFINED
#define MISRA_FLOAT32_T_DEFINED
typedef float float32_t;
#endif

/* Exported Constants --------------------------------------------------------*/
#ifndef MAX_MOTOR_RPM
#define MAX_MOTOR_RPM    (1000U)  /* Default maximum motor speed in RPM */
#endif

/* Edit by Nhat */
#ifndef AR_BASE_IQ
#define AR_BASE_IQ    200.0f  /* Default base Iq */
#endif

#ifndef AR_MAX_IQ
#define AR_MAX_IQ     2000.0f  /* Default maximum Iq */
#endif

#ifndef AR_MIN_IQ
#define AR_MIN_IQ    -1000.0f  /* Default minimum Iq */
#endif
/* Edit by Nhat */

/* Exported Functions Prototypes ---------------------------------------------*/

/**
  * @brief  Resets the Anti-Rollback PI integrator and output state.
  * @param  None
  * @retval None
  */
void AntiRollback_Reset(void);

/**
  * @brief  Executes the Anti-Rollback PI control algorithm.
  * @param  hMeasuredSpeed_RPM Currently measured motor speed in RPM.
  * @retval float32_t Required torque current reference (Iq).
  */
float32_t AntiRollback_Execute(float32_t hMeasuredSpeed_RPM);

/**
  * @brief  Manages the Hill Start Assist state machine and transitions.
  * @param  is_brake_pressed  State of the brake input (true = pressed).
  * @param  throttle_percent  Throttle position in percentage (0.0f to 100.0f).
  * @param  measured_speed_rpm Current motor speed in RPM.
  * @retval None
  */
void Hill_Start_Assist_Task(bool is_brake_pressed,
                             float32_t throttle_percent,
                             float32_t measured_speed_rpm);

#ifdef __cplusplus
}
#endif

#endif /* ANTI_ROLLBACK_H */
