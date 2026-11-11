	/****************************************************************************
	*
	* Copyright (c) 2023-2026 STMicroelectronics - All Rights Reserved
	*
	* License terms: STMicroelectronics Proprietary in accordance with licensing
	* terms SLA0098 at www.st.com.
	*
	* THIS SOFTWARE IS DISTRIBUTED "AS IS," AND ALL WARRANTIES ARE DISCLAIMED,
	* INCLUDING MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE.
	*
	*****************************************************************************/
	/*
	*   @file    Drive parameters.h
	*   @version 1.1.0
	*   @brief   This file contains motor parameters
	*          
	*   @details This file contains motor parameters.
	*
	*/
	
	/* Define to prevent recursive inclusion -------------------------------------*/
	#ifndef __DRIVE_PARAMETERS_H
	#define __DRIVE_PARAMETERS_H

	/******** MAIN AND AUXILIARY SPEED/POSITION SENSOR(S) SETTINGS SECTION ********/


	/*** Speed sensor selection ***/

	/* #define STATE_OBSERVER_PLL */
	/* #define STATE_OBSERVER_CORDIC */
	/* #define HFINJECTION */
	/* #define ENCODER */
	#define HALL_SENSORS
	/* #define RESOLVER */
	/* #define EXTERNAL_SENSOR */

	/*** Auxiliary speed measurement selection ***/
	/* #define AUX_RESOLVER */
	/* #define AUX_STATE_OBSERVER_PLL */
	/* #define AUX_STATE_OBSERVER_CORDIC */
	/* #define AUX_HFINJECTION */
	/* #define AUX_ENCODER */
	/* #define AUX_HALL_SENSORS */
	/* #define AUX_EXTERNAL_SENSOR */

	/*** Drives number selection ***/
	#define SINGLEDRIVE
	/* #define DUALDRIVE */


	/*** Speed measurement settings ***/

	
	#define MAX_APPLICATION_SPEED_RPM       64000 /*!< rpm, mechanical */
	#define MIN_APPLICATION_SPEED_RPM       0 /*!< rpm, mechanical,  
															absolute value */
	#define MEAS_ERRORS_BEFORE_FAULTS       255 /*!< Number of speed  
																measurement errors before 
																main sensor goes in fault */

	/****** Hall sensors ************/
	#define HALL_MEAS_ERRORS_BEFORE_FAULTS  255 /*!< Number of failed
                                                           derived class specific speed
                                                           measurements before main sensor
                                                           goes in fault */

	#define HALL_AVERAGING_FIFO_DEPTH        18 /*!< depth of the FIFO used to
                                                           average mechanical speed in
                                                           0.1Hz resolution */
	#define HALL_MTPA  						false	


	



	


	/* USER CODE BEGIN angle reconstruction M1 */
	#define PARK_ANGLE_COMPENSATION_FACTOR 0
	#define REV_PARK_ANGLE_COMPENSATION_FACTOR 0
	/* USER CODE END angle reconstruction M1 */
	
	/**************************    DRIVE SETTINGS SECTION   **********************/

	/* PWM generation and current reading */
	#define PWM_FREQUENCY                    10000
	#define PWM_FREQ_SCALING                 1
	#define LOW_SIDE_SIGNALS_ENABLING        LS_PWM_TIMER
	#define SW_DEADTIME_NS                   700 /*!< Dead-time to be inserted  
															by FW, only if low side 
															signals are enabled */
																							
	/* Torque and flux regulation loops */
	#define REGULATION_EXECUTION_RATE     1    /*!< FOC execution rate in 
															number of PWM cycles */  
	/* Gains values for torque and flux control loops */
	#define PID_TORQUE_KP_DEFAULT         500       
	#define PID_TORQUE_KI_DEFAULT         3
	#define PID_TORQUE_KD_DEFAULT         100
	#define PID_FLUX_KP_DEFAULT           1820
	#define PID_FLUX_KI_DEFAULT           3
	#define PID_FLUX_KD_DEFAULT           100

	/* Torque/Flux control loop gains dividers*/
	#define TF_KPDIV                      16384
	#define TF_KIDIV                      16384
	#define TF_KDDIV                      8192 /*HARDWIRED*/
	#define TF_KPDIV_LOG                  LOG2(TF_KPDIV)
	#define TF_KIDIV_LOG                  LOG2(TF_KIDIV)
	#define TF_KDDIV_LOG                  LOG2(TF_KDDIV)

	
	#define TFDIFFERENTIAL_TERM_ENABLING  DISABLE

	/* Speed control loop */ 
	#define SPEED_LOOP_FREQUENCY_HZ       500 /*!<Execution rate of speed   
														regulation loop (Hz) */
	#define PID_SPEED_KP_DEFAULT          5620/(SPEED_UNIT/10)
	#define PID_SPEED_KI_DEFAULT          20/(SPEED_UNIT/10)
	#define PID_SPEED_KD_DEFAULT          0/(SPEED_UNIT/10)	/*HARDWIRED*/

	/* Speed PID parameter dividers */
	#define SP_KPDIV                      16
	#define SP_KIDIV                      256
	#define SP_KDDIV                      16
	#define SP_KPDIV_LOG                  LOG2(SP_KPDIV)
	#define SP_KIDIV_LOG                  LOG2(SP_KIDIV)
	#define SP_KDDIV_LOG                  LOG2(SP_KDDIV)

	/* USER CODE BEGIN PID_SPEED_INTEGRAL_INIT_DIV */
	#define PID_SPEED_INTEGRAL_INIT_DIV 1 /*  */
	/* USER CODE END PID_SPEED_INTEGRAL_INIT_DIV */


	#define SPD_DIFFERENTIAL_TERM_ENABLING DISABLE

	/* Default settings */

	#define DEFAULT_CONTROL_MODE           STC_SPEED_MODE  /*!< STC_TORQUE_MODE or 
																STC_SPEED_MODE */  
	#define DEFAULT_TARGET_SPEED_RPM       60
	#define DEFAULT_TARGET_SPEED_UNIT      (DEFAULT_TARGET_SPEED_RPM*SPEED_UNIT/_RPM)
	#define DEFAULT_TORQUE_COMPONENT       595.0
	#define DEFAULT_FLUX_COMPONENT         0.0

	/**************************    FIRMWARE PROTECTIONS SECTION   *****************/


	#define OV_VOLTAGE_PROT_ENABLING        ENABLE /*!< Over voltage protection enabled */
	#define UV_VOLTAGE_PROT_ENABLING        ENABLE /*!< Under voltage protection enabled */
	#define OV_VOLTAGE_THRESHOLD_V          (100U) /*!< Over-voltage threshold */
	#define UD_VOLTAGE_THRESHOLD_V          (20U) /*!< Under-voltage threshold */

	#define ON_OVER_VOLTAGE                 TURN_OFF_PWM /*!< TURN_OFF_PWM, 
															TURN_ON_R_BRAKE or 
															TURN_ON_LOW_SIDES */
	#define R_BRAKE_SWITCH_OFF_THRES_V      10
    
	#define OV_TEMPERATURE_PROT_ENABLING    DISABLE
	#define OV_TEMPERATURE_THRESHOLD_C      (110U) /*!< Celsius degrees */
	#define OV_TEMPERATURE_HYSTERESIS_C     (10U)  /*!< Celsius degrees */

	#define HW_OV_CURRENT_PROT_BYPASS       DISABLE /*!<In case ON_OVER_VOLTAGE  
														is set to TURN_ON_LOW_SIDES
														this feature may be used to
														bypass HW over-current
														protection (if supported by 
														power stage) */
	/******************************   START-UP PARAMETERS   **********************/

	/* Encoder alignment */
	#define ALIGNMENT_DURATION              500    /*!< milliseconds */
	#define ALIGNMENT_ANGLE_DEG             90      /*!< degrees [0...359] */
	#define FINAL_I_ALIGNMENT               992.0    /*!< s16A */
	/* With ALIGNMENT_ANGLE_DEG equal to 90 degrees final alignment */
	/* phase current = (FINAL_I_ALIGNMENT * 1.65/ Av)/(32767 * Rshunt) */
	/* being Av the voltage gain between Rshunt and A/D input */
    
	/* Sensor-less rev-up sequence (BASIC)*/
	/* Phase 1 */
	#define STARTING_ANGLE_DEG             90  /*!< degrees [0...359] */
	#define PHASE1_DURATION                0 /*milliseconds */
	#define PHASE1_FINAL_SPEED_UNIT        (0*SPEED_UNIT/_RPM) /* rpm */
	#define PHASE1_FINAL_CURRENT           19858.0    /*!< s16A */
	/* Phase 2 */
	#define PHASE2_DURATION                1000 /*milliseconds */
	#define PHASE2_FINAL_SPEED_UNIT        0 /* rpm */
	#define PHASE2_FINAL_CURRENT           19858.0    /*!< s16A */
	/* Phase 3 */
	#define PHASE3_DURATION                500 /*milliseconds */
	#define PHASE3_FINAL_SPEED_UNIT        (0*SPEED_UNIT/_RPM)/* rpm */
	#define PHASE3_FINAL_CURRENT           23830.0    /*!< s16A */
	/* Phase 4 */
	#define PHASE4_DURATION                500 /*milliseconds */
	#define PHASE4_FINAL_SPEED_UNIT        (100*SPEED_UNIT/_RPM) /* rpm */
	#define PHASE4_FINAL_CURRENT           23830.0    /*!< s16A */
	/* Phase 5 */
	#define PHASE5_DURATION                0 /* milliseconds */
	#define PHASE5_FINAL_SPEED_UNIT        (100*SPEED_UNIT/_RPM) /* rpm */
	#define PHASE5_FINAL_CURRENT           23830.0    /*!< s16A */

	#define ENABLE_SL_ALGO_FROM_PHASE      3

	/* Observer start-up output conditions */
	#define OBS_MINIMUM_SPEED_RPM          200
	#define NB_CONSECUTIVE_TESTS           1 /* corresponding to 
															former NB_CONSECUTIVE_TESTS/
															(TF_REGULATION_RATE/
															MEDIUM_FREQUENCY_TASK_RATE) */
	#define SPEED_BAND_UPPER_LIMIT         17 /*!< It expresses how much 
																estimated speed can exceed 
																forced stator electrical 
																without being considered wrong. 
																In 1/16 of forced speed */
	#define SPEED_BAND_LOWER_LIMIT         15  /*!< It expresses how much 
																estimated speed can be below 
																forced stator electrical 
																without being considered wrong. 
																In 1/16 of forced speed */
	#define TRANSITION_DURATION            25  /* Switch over duration, ms */
	/******************************   ADDITIONAL FEATURES   **********************/

	/******************************   Bus VOLTAGE Motor 1  **********************/
	#define  M1_VBUS_SAMPLING_TIME  LL_ADC_SAMPLING_CYCLE(47)	/* HARDWIRED */

	/******************************   Temperature sensing Motor 1  **********************/
	#define  M1_TEMP_SAMPLING_TIME  LL_ADC_SAMPLING_CYCLE(47)  /* HARDWIRED */
	
	/******************************   Current sensing Motor 1   **********************/
	#define ADC_SAMPLING_CYCLES (6 + SAMPLING_CYCLE_CORRECTION) //HARDWIRED
	

	/* Flux Weakening parameters */
	#define  FLUX_WEAKENING_ENABLING       ENABLE
	#define  FW_VOLTAGE_REF                1000.0 /*!<Vs reference, tenth 
															of a percent */
	#define  FW_KP_GAIN                    1000 /*!< Default Kp gain */
	#define  FW_KI_GAIN                    50 /*!< Default Ki gain */
	#define  FW_KPDIV                      1024
													/*!< Kp gain divisor.If FULL_MISRA_C_COMPLIANCY
													is not defined the divisor is implemented through
													algebrical right shifts to speed up PIs execution. 
													Only in this case this parameter specifies the 
													number of right shifts to be executed */
	#define  FW_KIDIV                      1024
													/*!< Ki gain divisor.If FULL_MISRA_C_COMPLIANCY
													is not defined the divisor is implemented through
													algebrical right shifts to speed up PIs execution. 
													Only in this case this parameter specifies the 
													number of right shifts to be executed */
	/*  Feed-forward parameters */

	#define FEED_FORWARD_CURRENT_REG_ENABLING   ENABLE
	#define CONSTANT1_Q                    6277
	#define CONSTANT1_D                    6277
	#define CONSTANT2_QD                   847


	/*  Maximum Torque Per Ampere strategy parameters */
	#define MTPA_ENABLING                  DISABLE
	#define IQMAX                          29788
	#define SEGDIV                         0
	#define ANGC                           {0,0,0,0,0,0,0,0}
	#define OFST                           {0,0,0,0,0,0,0,0}
	#define INRUSH_CURRLIMIT_ENABLING        DISABLE
	#define INRUSH_CURRLIMIT_AT_POWER_ON     INACTIVE   /* ACTIVE or INACTIVE */
	#define INRUSH_CURRLIMIT_CHANGE_AFTER_MS 1000       /* milliseconds */
	/******************************** DEBUG ADD-ONs *******************************/
	#define LCD_JOYSTICK_FUNCTIONALITY       DISABLE
	#define LCD_MODE                         LCD_FULL
	#define START_STOP_BTN                   DISABLE
	
	/******************************** PFC ENABLING ********************************/
	/* #define PFC_ENABLED to enable the PFC */ 
	
	
	#endif /*__DRIVE_PARAMETERS_H*/

