/*******************************************************************************
  * @file    anti_rollback.c
  * @brief   Anti-Rollback / Hill Start Assist (HSA) Controller Implementation
  * @details Implements a PI controller to generate torque current reference (Iq)
  *          maintaining zero speed on inclines.
  *****************************************************************************/

#include <stdint.h>
#include <stdbool.h>
#include "motorcontrol.h"
#include "anti_rollback.h"
#include "drive_parameters.h"

/* Typedef for MISRA C compliance */
typedef float float32_t;

/**
  * @brief Anti-Rollback PI Controller structure definition
  */
typedef struct {
    float32_t kp;               /* Proportional gain */
    float32_t ki;               /* Integral gain */
    float32_t ts;               /* Sampling time in seconds (e.g., 0.001f = 1ms) */
    float32_t integral;         /* Integral term accumulator */
    float32_t iq_out;           /* Output torque current reference */
    float32_t iq_max;           /* Maximum allowable torque current limit */
    float32_t iq_min;           /* Minimum allowable torque current limit */
    float32_t speed_deadband;   /* Speed error deadband threshold (RPM) */
} AntiRollback_PI_t;

/* Static controller instance initialization */
static AntiRollback_PI_t hAntiRollback = {
    .kp = 50.0f,
    .ki = 200.0f,
    .ts = 0.002f,
    .integral = 0.0f,
    .iq_out = 0.0f,
    .iq_max = AR_MAX_IQ,
    .iq_min = AR_MIN_IQ,
    .speed_deadband = 1.5f
};

/**
  * @brief  Resets the Anti-Rollback PI integrator and output state.
  * @param  None
  * @retval None
  */
void AntiRollback_Reset(void) {
    hAntiRollback.integral = 0.0f;
    hAntiRollback.iq_out = 0.0f;
}

/**
  * @brief  Executes the Anti-Rollback PI control algorithm.
  * @param  hMeasuredSpeed_RPM Currently measured motor speed in RPM.
  * @retval float32_t Required torque current reference (Iq).
  */
/* Edit by Nhat */

//float32_t AntiRollback_Execute(float32_t hMeasuredSpeed_RPM) {
//    const float32_t target_speed = 0.0f;
//    float32_t speed_error;
//    float32_t p_term;
//
//    /* Calculate speed error (Rollback -> MeasuredSpeed < 0 -> Positive Error -> Positive Iq) */
//    speed_error = target_speed - hMeasuredSpeed_RPM;
//
//    /* Apply deadband threshold to prevent hunting/vibration at zero speed */
////    if ((speed_error > (-hAntiRollback.speed_deadband)) &&
////        (speed_error < hAntiRollback.speed_deadband)) {
////        speed_error = 0.0f;
////    } else {
////        /* Keep speed_error value */
////    }
//
//    /* Edit by Nhat */
//    if (speed_error > 0.0f) hAntiRollback.integral += (hAntiRollback.ki * speed_error * hAntiRollback.ts);
//    else  hAntiRollback.integral *= 0.80f;
//    /* Edit by Nhat */
//
//    /* Proportional term calculation */
//    p_term = hAntiRollback.kp * speed_error;
//
//    /* Integral term calculation with anti-windup clamping */
////    hAntiRollback.integral += (hAntiRollback.ki * speed_error * hAntiRollback.ts);
//
////    if (hAntiRollback.integral > hAntiRollback.iq_max) {
////        hAntiRollback.integral = hAntiRollback.iq_max;
////    } else if (hAntiRollback.integral < hAntiRollback.iq_min) {
////        hAntiRollback.integral = hAntiRollback.iq_min;
////    } else {
////        /* Do nothing */
////    }
////
////    /* Total control output calculation */
////    hAntiRollback.iq_out = p_term + hAntiRollback.integral;
////
////    /* Output saturation logic */
////    if (hAntiRollback.iq_out > hAntiRollback.iq_max) {
////        hAntiRollback.iq_out = hAntiRollback.iq_max;
////    } else if (hAntiRollback.iq_out < hAntiRollback.iq_min) {
////        hAntiRollback.iq_out = hAntiRollback.iq_min;
////    } else {
////        /* Do nothing */
////    }
//
//    if (hMeasuredSpeed_RPM > 2.0f)
//    {
//		if (hAntiRollback.integral > 0.0f) hAntiRollback.integral = 0.0f;
//	}
//    /* Anti-windup clamping */
//	if (hAntiRollback.integral > AR_MAX_IQ ) hAntiRollback.integral = AR_MAX_IQ;
//	else if (hAntiRollback.integral < AR_MIN_IQ) hAntiRollback.integral = AR_MIN_IQ;
//	/* Total Iq = Base Pre-load (600) + Dynamic PI boost */
//	hAntiRollback.iq_out = AR_BASE_IQ + p_term + hAntiRollback.integral;
//	/* Output saturation logic */
//	if (hAntiRollback.iq_out > AR_MAX_IQ) {
//		hAntiRollback.iq_out = AR_MAX_IQ;
//	} else if (hAntiRollback.iq_out < AR_MIN_IQ) {
//		hAntiRollback.iq_out = AR_MIN_IQ;
//	}
//
//    return hAntiRollback.iq_out;
//}

float32_t AntiRollback_Execute(float32_t hMeasuredSpeed_RPM) {
    float32_t iq_cmd = 0.0f;
    /* CASE 1: Motor is rolling backward (Negative RPM) */
    if (hMeasuredSpeed_RPM < -0.5f) {
        float32_t speed_error = 0.0f - hMeasuredSpeed_RPM; /* Positive error */

        /* Kp = 120 gives 720 digits of immediate torque at the first -6 RPM Hall step */
        float32_t p_term = 120.0f * speed_error;
        /* Integrate to build solid holding torque up to AR_MAX_IQ (e.g. 1200 - 1500) */
        hAntiRollback.integral += (150.0f * speed_error * hAntiRollback.ts);
        if (hAntiRollback.integral > AR_MAX_IQ) {
            hAntiRollback.integral = AR_MAX_IQ;
        }
        iq_cmd = p_term + hAntiRollback.integral;
        if (iq_cmd > AR_MAX_IQ) {
            iq_cmd = AR_MAX_IQ;
        }
    }
    /* CASE 2: Hand released or pushed forward (Positive RPM) */
    else if (hMeasuredSpeed_RPM > 0.5f) {
        /* WIPE holding torque instantly so it never catapults forward! */
        hAntiRollback.integral = 0.0f;
        /* Apply gentle soft braking to stop forward rotation */
        iq_cmd = -30.0f * hMeasuredSpeed_RPM;
        if (iq_cmd < AR_MIN_IQ) {
            iq_cmd = AR_MIN_IQ;
        }
    }
    /* CASE 3: Motor is held at 0 RPM (-0.5 RPM <= speed <= +0.5 RPM) */
    else {
        /* MAINTAIN the holding torque that stopped the rollback!
           Do NOT drain it here, so it continues holding against your hand/incline. */
        iq_cmd = hAntiRollback.integral;
    }
    hAntiRollback.iq_out = iq_cmd;
    return hAntiRollback.iq_out;
}

/* Edit by Nhat */

/**
  * @brief  Manages the Hill Start Assist state machine and transitions.
  * @param  is_brake_pressed  State of the brake input (true = pressed).
  * @param  throttle_percent  Throttle position in percentage (0.0f to 100.0f).
  * @param  measured_speed_rpm Current motor speed in RPM.
  * @retval None
  */
void Hill_Start_Assist_Task(bool is_brake_pressed, float32_t throttle_percent, float32_t measured_speed_rpm) {

	qd_t iqd_cmd;
	/* CONDITION 1: Brake released and no throttle command (Holding position on incline) */
    if ((!is_brake_pressed) && (throttle_percent < 2.0f)) {
        /* Edit by Nhat */
//    	float32_t required_Iq;

//        /* Switch FOC state machine to Torque Control mode */
//        MC_SetControlModeMotor1(MCM_TORQUE_MODE);
//
//        /* Compute anti-rollback current reference */
//        required_Iq = AntiRollback_Execute(measured_speed_rpm);
//
//        /* Apply calculated Iq reference to FOC loop (Flux current Id = 0) */
//        MC_SetCurrentReferenceMotor1((int16_t)required_Iq, 0);

    	float32_t required_Iq = AntiRollback_Execute(measured_speed_rpm);
		/* Apply calculated Iq reference to FOC loop (Flux current Id = 0) */
		iqd_cmd.q = (int16_t)required_Iq;
		iqd_cmd.d = 0;
		MC_SetCurrentReferenceMotor1(iqd_cmd);
		/* Edit by Nhat */
    }

    /* CONDITION 2: Throttle applied by operator (Smooth transition to driving) */
    else if (throttle_percent >= 2.0f) {
        int16_t target_ramp_speed;

        /* Reset integrator state to prevent torque surge upon launch */
        AntiRollback_Reset();

        /* Edit by Nhat */

//        /* Switch back to Speed Control mode */
//        MC_SetControlModeMotor1(MCM_SPEED_MODE);
//
//        /* Calculate speed ramp reference based on throttle input */
//        target_ramp_speed = (int16_t)((throttle_percent * ((float32_t)MAX_MOTOR_RPM)) / 100.0f);

        target_ramp_speed = (int16_t)((throttle_percent * ((float32_t)MAX_APPLICATION_SPEED_RPM)) / 100.0f);

        /* Execute speed ramp with 500ms duration */
        MC_ProgramSpeedRampMotor1(target_ramp_speed, 500U);
    }

    /* CONDITION 3: Brake actively engaged */
    else {
        AntiRollback_Reset();
//        MC_SetCurrentReferenceMotor1(0, 0);
        iqd_cmd.q = 0;
	    iqd_cmd.d = 0;
	    MC_SetCurrentReferenceMotor1(iqd_cmd);
    }
}
