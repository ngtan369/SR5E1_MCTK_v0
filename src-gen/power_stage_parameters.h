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
*   @file    Power stage parameters.h
*   @version 1.1.0
*   @brief   This file contains Power stage parameters
*
*   @details This file contains Power stage parameters
*/

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __POWER_STAGE_PARAMETERS_H
#define __POWER_STAGE_PARAMETERS_H

/************* PWM Driving signals section **************/
/* High side Polarity */
#define PHASE_UH_POLARITY             LL_TIM_OCPOLARITY_LOW 
#define PHASE_VH_POLARITY             LL_TIM_OCPOLARITY_LOW 
#define PHASE_WH_POLARITY             LL_TIM_OCPOLARITY_LOW 

#define HW_COMPLEMENTED_LOW_SIDE      DISABLE 
/* Low side Polarity */
#define PHASE_UL_POLARITY             LL_TIM_OCPOLARITY_LOW 
#define PHASE_VL_POLARITY             LL_TIM_OCPOLARITY_LOW 
#define PHASE_WL_POLARITY             LL_TIM_OCPOLARITY_LOW 

/********** Inrush current limiter signal section *******/
#define INRUSH_CURR_LIMITER_POLARITY  DOUT_ACTIVE_HIGH 

/******* Dissipative brake driving signal section *******/
#define DISSIPATIVE_BRAKE_POLARITY    DOUT_ACTIVE_HIGH

/*********** Bus voltage sensing section ****************/
/* ENABLE/DISABLE Bus voltage sensing */
#define VBUS_VOLTAGE_ENABLE TRUE
#define VBUS_PARTITIONING_FACTOR      0.0081 /*!< It expresses how 
                                                  much the Vbus is attenuated  
                                                  before being converted into 
                                                  digital value */
#define NOMINAL_BUS_VOLTAGE_V         72

#define VBUS_NETWORK_PULLUP_RES_OHM   1220000
#define VBUS_NETWORK_PULLDW_RES_OHM   10000
/******** Current reading parameters section ******/
/*** Topology ***/
#define THREE_SHUNT

#define ICS_SHUNT_SELECTION (ICS_SHUNT_LEGS)

#define RSHUNT                        5.0E-4 

/*  Overall Amplification gain is calculated as shunt resistor times Amplification Network Gain */
#define AMPLIFICATION_GAIN            0.01 
/*Current sensing on legs as configured */
#define ICS_SENSING_SELECTION (ICS_SENSING_ON_UV_PHASES)
/*** Noise parameters ***/
#define TNOISE_NS                     1000 
#define TRISE_NS                      1000 
#define MAX_TNTR_NS TNOISE_NS 

/************ Temperature sensing section ***************/
/* ENABLE/DISABLE Temperature sensing */
#define TEMP_VOLTAGE_ENABLE TRUE
/* V[V]=V0+dV/dT[V/Celsius]*(T-T0)[Celsius]*/
#define V0_V                          1.65 /*!< in Volts */
#define T0_C                          25.0 /*!< in Celsius degrees */                                               
#define dV_dT                         0.032 /*!< V/Celsius degrees */
#define T_MAX                         110 /*!< Sensor measured 
		                                                       temperature at maximum 
		                                                       power stage working 
		                                                       temperature, Celsius degrees */
#endif /*__POWER_STAGE_PARAMETERS_H*/
