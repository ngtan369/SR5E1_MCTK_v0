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
*   @file    PMSM motor parameters.h
*   @version 1.1.0
*   @brief   This file contains motor parameters
*          
*   @details This file contains motor parameters.
*
*/

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __PMSM_MOTOR_PARAMETERS_H
#define __PMSM_MOTOR_PARAMETERS_H

#define MOTOR_TYPE             PMSM



/***************** MOTOR ELECTRICAL PARAMETERS  ******************************/
#define POLE_PAIR_NUM          14  /* Number of motor pole pairs */
#define RS                     0.012  /* Stator resistance , ohm*/
#define LS                     0.00001500 /* Stator inductance, H
												For I-PMSM it is equal to Lq */

/* When using Id = 0, NOMINAL_CURRENT is utilized to saturate the output of the 
PID for speed regulation (i.e. reference torque). 
Transformation of real currents (A) into s16 format must be done accordingly with 
formula:
Phase current (s16 0-to-peak) = (Phase current (A 0-to-peak)* 32767 * Rshunt *
								*Amplifying network gain)/(MCU supply voltage/2)
*/

#define NOMINAL_CURRENT         29788.0
#define MOTOR_MAX_SPEED_RPM     64000  /*!< Maximum rated speed  */
#define MOTOR_VOLTAGE_CONSTANT  4.8 /*!< Volts RMS ph-ph /kRPM */
#define ID_DEMAG               -29788 /*!< Demagnetization current */


/***************** MOTOR SENSORS PARAMETERS  ******************************/
/* Motor sensors parameters are always generated but really meaningful only 
if the corresponding sensor is actually present in the motor         */


/*** Hall sensors ***/
#define HALL_SENSORS_AVAILABLE  TRUE
#define HALL_SENSORS_PLACEMENT  DEGREES_120 /*!<Define here the  
												mechanical position of the sensors
												with reference to an electrical cycle. 
												It can be either DEGREES_120 or 
												DEGREES_60 */
																				
#define HALL_PHASE_SHIFT        300 /*!< Define here in degrees  
												the electrical phase shift between 
												the low to high transition of 
												signal H1 and the maximum of 
												the Bemf induced on phase A */ 
/*** Quadrature encoder ***/ 
#define ENCODER_AVAILABLE       FALSE
#define M1_ENCODER_PPR             512 /*!< Number of pulses per 
											revolution */
/*** Resolver ***/
#define RESOLVER_SENSORS_AVAILABLE  FALSE
#define RESOLVER_ANGLE_OFFSET_DEGREE        180.0  /*!< Define here in degrees
												the electrical phase shift between
												the Resolver Channel A and the Bemf
												induced on phase A*/

/*** External Sensor ***/
#define EXTERNAL_SENSORS_AVAILABLE  FALSE

#endif /*__PMSM_MOTOR_PARAMETERS_H*/
