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
* @file    mc_config.h
* @author  Motor Control SDK Team, ST Microelectronics
* @brief   Motor Control Subsystem components configuration and handler
*          structures declarations.
*/
#ifndef __MC_CONFIG_H
#define __MC_CONFIG_H

#include "pid_regulator.h"
#include "revup_ctrl.h"
#include "speed_torq_ctrl.h"
#include "virtual_speed_sensor.h"
#include "ntc_temperature_sensor.h"
#include "pwm_curr_fdbk.h"
#include "r_divider_bus_voltage_sensor.h"
#include "virtual_bus_voltage_sensor.h"
#include "pqd_motor_power_measurement.h"
#include "user_interface.h"
#include "motor_control_protocol.h"
#include "usart_frame_communication_protocol.h"
#include "can_frame_communication_protocol.h"
#include "dac_common_ui.h"
#include "dac_rctimer_ui.h"
#include "dac_ui.h"
#include "ics_SR5E1_pwm_curr_fdbk.h"
#include "encoder_speed_pos_fdbk.h"
#include "resolver_SR5E1_speed_pos_fdbk.h"
#include "enc_align_ctrl.h"
#include "hall_speed_pos_fdbk.h"        
#include "ramp_ext_mngr.h"
#include "circle_limitation.h"
#include "sto_speed_pos_fdbk.h"
#include "sto_pll_speed_pos_fdbk.h"
#include "external_adc_out.h"

#ifdef USE_STGAP1S
#include "gap_gate_driver_ctrl.h"
#endif

extern RevUpCtrl_Handle_t RevUpControlM1;
extern PID_Handle_t PIDSpeedHandle_M1;
extern PID_Handle_t PIDIqHandle_M1;
extern PID_Handle_t PIDIdHandle_M1;
extern NTC_Handle_t TempSensorParamsM1;
extern PWMC_ICS_Handle_t PWM_Handle_M1;
extern SpeednTorqCtrl_Handle_t SpeednTorqCtrlM1;
extern PQD_MotorPowMeas_Handle_t PQD_MotorPowMeasM1;
extern PQD_MotorPowMeas_Handle_t *pPQD_MotorPowMeasM1;
extern VirtualSpeedSensor_Handle_t VirtualSpeedSensorM1;
extern STO_Handle_t STO_M1;
extern STO_PLL_Handle_t STO_PLL_M1;
extern ENCODER_Handle_t ENCODER_M1;
extern EncAlign_Handle_t EncAlignCtrlM1;
extern HALL_Handle_t HALL_M1;
extern Resolver_SR5E1_Handle_t Resolver_M1;
extern RDivider_Handle_t RealBusVoltageSensorParamsM1;
extern VirtualBusVoltageSensor_Handle_t VirtualBusVoltageSensorParamsM1;
extern CircleLimitation_Handle_t CircleLimitationM1;
extern RampExtMngr_Handle_t RampExtMngrHFParamsM1;
extern UI_Handle_t UI_Params;
extern DAC_UI_Handle_t DAC_UI_Params;
extern UFCP_Handle_t pUSART;
extern CFCP_Handle_t pFDCAN;
extern Ext_ADC_OUT_t VBUS_EXT_ADCOUT_M1;
extern Ext_ADC_OUT_t TEMP_EXT_ADCOUT_M1;        

#ifdef USE_STGAP1S
extern GAP_Handle_t STGAP_M1;
#endif

/* enable ITCM */
#define MCTK_ITCM

#define NBR_OF_MOTORS 1		

/* PWM Module section */
#define PWM_MODULE_SELECTION              TIM1    
/* TIM channel assignment */
#define TIMx_UH_CHANNEL       (LL_TIM_CHANNEL_CH3)  /* Phase A Top transistor channel */
#define TIMx_UL_CHANNEL       (LL_TIM_CHANNEL_CH3N) /* Phase A Bottom transistor channel */
#define TIMx_VH_CHANNEL       (LL_TIM_CHANNEL_CH2)  /* Phase B Top transistor channel */ 
#define TIMx_VL_CHANNEL       (LL_TIM_CHANNEL_CH2N) /* Phase B Bottom transistor channel */ 
#define TIMx_WH_CHANNEL       (LL_TIM_CHANNEL_CH1)  /* Phase C Top transistor channel */
#define TIMx_WL_CHANNEL       (LL_TIM_CHANNEL_CH1N) /* Phase C Bottom transistor channel */
#define TIMx_ADC_TRG_CH       (LL_TIM_CHANNEL_CH4)  /* ADC trigger channel (current sensing) */

#define ICS_ADC_INJ_TRIG_EXT_TIMx_TRGO         LL_ADC_INJ_TRIG_EXT_TIM1_TRGO
/* IDLE state --> to be moved into drive paramters.h file */
#define PWM_HIGH_SIDE_IDLE_STATE (LL_TIM_OCIDLESTATE_LOW)

#define PWM_LOW_SIDE_IDLE_STATE (LL_TIM_OCIDLESTATE_LOW)
/* These pin must be defined only for ES_GPIO configuration */
#define ENABLE_CH1 0
#define ENABLE_CH2 0
#define ENABLE_CH3 0		
/* over current protection */
/* enable/disable in power stage over current protection */
#define SW_OV_CURRENT_PROT_ENABLING (LL_TIM_BREAK_ENABLE)
#define OVERCURR_FEEDBACK_POLARITY  (LL_TIM_BREAK_POLARITY_HIGH)
#define OVERCURR_BREAK_FILTER       (LL_TIM_BREAK_FILTER_FDIV1)

/* PWM BREAK2 */
#define ICS_BREAK2_ENABLING           (LL_TIM_BREAK2_DISABLE)		
#define ICS_BREAK2_FEEDBACK_POLARITY  (LL_TIM_BREAK2_POLARITY_HIGH)
#define ICS_BREAK2_FILTER             (LL_TIM_BREAK2_FILTER_FDIV1)		

/* Currents Sensing ADC section */
#define ADC_0_PERIPH                    (ADC1)
#define ADC_1_PERIPH                    (ADC2)

#define ADC_2_PERIPH                    (ADC3)
#define PHASE_IA_CURR_CHANNEL           (MC_ADC_CHANNEL_1) 
#define PHASE_IB_CURR_CHANNEL           (MC_ADC_CHANNEL_1)
#define PHASE_IC_CURR_CHANNEL           (MC_ADC_CHANNEL_2)

#define ADC_SAMPLING_TIME               (LL_ADC_SAMPLINGTIME_6CYCLES_5) 

#define ADCx_HF_M1_IRQHandler  IRQ_ADC1_HANDLER
#define IRQ_HF_ADC_VECTOR      IRQ_ADC1_VECTOR

/****             DAC    ***/
/* DAC Debug Setting */

#define DAC_FUNCTIONALITY                FALSE

#define DEBUG_DAC_CH1                    TRUE
#define DEBUG_DAC_CH2                    TRUE

#define DEFAULT_DAC_CHANNEL_1            MC_PROTOCOL_REG_OBS_BEMF_ALPHA
#define DEFAULT_DAC_CHANNEL_2            MC_PROTOCOL_REG_OBS_BEMF_BETA

/* DAC as TIM */
#define TIM_DAC
#define DAC_MODULE         TIM2 /* (TIMx Module) */
#define DAC_RESOURCE_CH1   LL_TIM_CHANNEL_CH1 /* DAC TIM timer channel 1 */
#define DAC_RESOURCE_CH2   LL_TIM_CHANNEL_CH2 /* DAC TIM timer channel 2 */
/****    HALL    ***/
/* HALL timer section */
#define HALL_TIMER_SELECTION              (TIM2)
		#define HALL_S1_CH               		  (LL_TIM_CHANNEL_CH1)  
        #define HALL_S2_CH                        (LL_TIM_CHANNEL_CH2)  
		#define HALL_S3_CH                        (LL_TIM_CHANNEL_CH3)
		#define HALL_TIMx_VECTOR                  IRQ_TIM2_VECTOR		

/* VBUS / TEMP sensing */
/* nr. ADC modules on device */
#define MAX_ADC_MODULE 5U

/* ADC module configuration to initialize User conversion.                   */
/* These macro initialiaze SAR ADC when the value is TRUE. The ADC module is */ 
/* used for VBUS/Temperature sensing. The initializaion is done in current sensing module (for i.e. ICS)  */
#define INIT_SARADC_UNIT_1 (FALSE) 
#define INIT_SARADC_UNIT_2 (FALSE) 
#define INIT_SARADC_UNIT_3 (FALSE) 
#define INIT_SARADC_UNIT_4 (TRUE) 
#define INIT_SARADC_UNIT_5 (TRUE)

#define VBUS_ADC_REFERENCE_VOLTAGE ADC_REFERENCE_VOLTAGE

/** VBus readings **/
/* VBUS configured to use internal ADC */
#define VBUS_ADC_MODULE                 (ADC5)        /*!< Adc module used for VBus sensing */
#define VBUS_ADC_CHANNEL                (MC_ADC_CHANNEL_1)  /*!< Adc channel used for VBus sensing */
#define VBUS_ADC_SAMPLING_TIME          (M1_VBUS_SAMPLING_TIME)         /* hardwired */

#define TEMP_ADC_REFERENCE_VOLTAGE ADC_REFERENCE_VOLTAGE


/** Temperature readings **/
/* Temperature sensing configured to use internal ADC */
#define TEMP_FDBK_ADC_MODULE            (ADC4)
#define TEMP_FDBK_ADC_CHANNEL           (MC_ADC_CHANNEL_1)
#define TEMP_ADC_SAMPLING_TIME          (uint16_t)(ADC_PRECHG_VALUE(SLOW_SARADC_PRECHG_VALUE_CONF)|ADC_INPSAMP_VALUE(SLOW_SARADC_INPSAMP_VALUE_CONF)) /* hardwired */

/* Serial communication */
#define SERIAL_COMMUNICATION   /* enable SERIAL User Interface */
#define USART_SELECTION                 UART1
#define USART_SPEED                     38400
#define USART_SWAP_TXRX_PIN             LL_USART_TXRX_STANDARD
#endif /* __MC_CONFIG_H */
/******************* (C) COPYRIGHT 2023 STMicroelectronics *****END OF FILE****/
