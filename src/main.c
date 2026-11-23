/****************************************************************************
*
* Copyright (c) 2023-2026 STMicroelectronics - All Rights Reserved.
*
* License terms: STMicroelectronics Proprietary in accordance with licensing
* terms SLA0098 at www.st.com.
*
* THIS SOFTWARE IS DISTRIBUTED "AS IS," AND ALL WARRANTIES ARE DISCLAIMED,
* INCLUDING MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE.
*****************************************************************************/
/**
*   @file    main.c
*   @brief   Demo projetc
*
*/
#include <stdint.h>
#include <stdbool.h>
#include "main.h"
#include "motorcontrol.h"
#include "sr5e1_ll_exti.h"
//#include "../cfg/can_cfg.h"
//#include <can_cfg.h>
#ifdef USE_L9907
#include "SRxxx_L9907.h"
#endif
#ifdef USE_L9502
#include "L9502.h"
#endif

#include <test_env.h>
#include "clock_cfg.h"
#include "anti_rollback.h"
#include "ics_SR5E1_pwm_curr_fdbk.h"
#include "mc_interface.h"
#include "parameters_conversion.h"
#include "mc_config.h"
#include "ui_task.h"
#include "sr5e1_ll_usart.h"
//extern Mci_Handle_t oMCInterface[M1];
volatile int16_t debug_Ia, elec_ang;
volatile int16_t debug_Ib, dbg_vd = -500;
volatile int32_t raw_ADC1_JDR1 = 99;
volatile int32_t raw_ADC2_JDR1 = 99;
volatile uint16_t debug_VSPEED_raw = 0;
volatile uint32_t target_patch_address, dbg_handle = 99;
volatile uint32_t dbg_measured_cpr = 0;
volatile uint32_t dbg_measured_ppr = 0;
volatile uint32_t dbg_z_pulse_count = 0;
extern PWMC_ICS_Handle_t PWM_Handle_M1;
uint8_t rcm_state = 99;

#define DUTY_PHA_DEBUG_SIZE 1200
extern uint16_t DutyPhABufferDebug[DUTY_PHA_DEBUG_SIZE];
extern uint16_t DutyPhBBufferDebug[DUTY_PHA_DEBUG_SIZE];
extern uint16_t DutyPhCBufferDebug[DUTY_PHA_DEBUG_SIZE];

extern int16_t DebugCurrentIa[MCTK_DEBUG_CURRENT_SIZE];
extern int16_t DebugCurrentIb[MCTK_DEBUG_CURRENT_SIZE];

extern uint32_t wIndex;
volatile uint16_t dbg_dutyA, dbg_dutyB, dbg_dutyC;   /* FOC CntPh (timer ticks) */
volatile uint16_t dbg_ccrA, dbg_ccrB, dbg_ccrC;     /* TIM1 CCR actually loaded */
volatile uint16_t dbg_dutyA_pct, dbg_dutyB_pct, dbg_dutyC_pct; /* 0..100 % */

volatile uint8_t can_led_test_value, faults = 1U;
volatile uint8_t hall_position = 99U;

static void EXTI_Init(void);
static void STDRIVE601_Init(void);
#if defined(LED_ONLY_TEST) && !defined(OPEN_LOOP_FOC_TEST)
static void UART_LED_Test_Init(void);
static void UART_LED_Test_Poll(void);
static void UART_LED_Test_ReportHall(void);
static void UART_LED_Test_Wait(void);
static void UART_LED_Test_Blink(void);
#endif
static void UART_LED_Test_SendString(const char *text);
static void OpenLoop_UART_PollCommand(void);
static void OpenLoop_UART_ReportAngles(void);
static void OpenLoop_UpdateLEDs(void);
#ifdef USE_L9502
void L9502_En_Button(void);
#endif
#ifdef USE_L9907
void L9907_En2_Button(void);
#endif



/* Enable/disable Test ADC regular conversion */
/* #define TEST_ADC_REG_CONV */
#ifdef EXTERNAL_THROTTLE
#define TEST_ADC_REG_CONV
#endif

#ifdef TEST_ADC_REG_CONV
  RegConv_t testRegConvADC;
  uint8_t handleRegADC;
  uint16_t testRegConvADCData;
#endif

/* DEBUG PIN */
/* #define GPIOE13    gpio_iopack(GPIO_PORT_E, GPIO_PIN_13) */


/* DEBUG PIN ADC TRIG SIN /COS (Reolver) */
/* #define TIM15_CH1 gpio_iopack(GPIO_PORT_H, GPIO_PIN_6) */
/* #define PADPD2 gpio_iopack(GPIO_PORT_D, GPIO_PIN_2) */

/* ISR priority */
/* ADC EOC - TSK High freq. */
#define IRQ_HF_ADC_PRIO   2U
/* TIMx UP */
#define IRQ_TIMx_UP_PRIO  0U
/* encoder priority */
#define IRQ_ENC_TIMx_PRIO 3U
/* hall priority */
#define IRQ_HALL_TIMx_PRIO 3U
/*  ext adc out  */
#define IRQ_TIM2_PRIO     3U
/* ext adc out  */
#define IRQ_TIM3_PRIO     3U
/* UART Live Monitor */
#define IRQ_UART1_PRIO    3U
/* FDCAN1 Live Monitor */
#define IRQ_FDCAN1_PRIO   3U
/* TIM BRK IN */ 
#define IRQ_TIMBRK_PRIO   5U

/* External Interrupt */
#define IRQ_EXTI_PRIO     4U
/* ADC EOC - Resolver */
#define IRQ_RES_ADCx_PRIO     2U


/* Define Motor Timer (Adjust to your actual timer instance, e.g., TIM1, TIM8) */
#define MOTOR_TIM TIM1

/* Define generic macros for Phase States */
#define PHASE_OFF       (0U)  /* Both High-side and Low-side switches are OFF */
#define PHASE_PWM       (1U)  /* High-side is PWMing, Low-side is OFF */
#define PHASE_GND       (2U)  /* High-side is OFF, Low-side is ON (connected to GND) */

/* Define Hall Sensor valid states */
#define HALL_STATE_ERR1 (0U)  /* Invalid state (000) */
#define HALL_STATE_ERR2 (7U)  /* Invalid state (111) */

/* Test LED */
//#ifndef TEST_TOGGLE_PIN_LED
//#define TEST_TOGGLE_PIN_LED
//#endif

State_t StateMotor;
volatile bool startMot, throttle_zero_check, gear_D, gear_R = FALSE;
volatile bool gear_N = TRUE;
volatile int16_t chart_avg_Iq = 0;
extern int16_t DebugAngle;
extern FOCVars_t FOCVars[NBR_OF_MOTORS];
int16_t duty, current;
uint8_t current_hall;
volatile int16_t dbg_foc, dbg_hall;

#ifdef OPEN_LOOP_FOC_TEST
/* Replace the MCTK protocol scheduler: UART1 is used as a text console here. */
void UI_Scheduler(void)
{
}

static char *OpenLoop_AppendInt(char *buffer, int32_t value)
{
    char digits[11];
    uint32_t magnitude;
    uint8_t count = 0U;

    if (value < 0)
    {
        *buffer++ = '-';
        magnitude = (uint32_t)(-(value + 1)) + 1U;
    }
    else
    {
        magnitude = (uint32_t)value;
    }

    do
    {
        digits[count++] = (char)('0' + (magnitude % 10U));
        magnitude /= 10U;
    } while (magnitude != 0U);

    while (count != 0U)
    {
        *buffer++ = digits[--count];
    }
    return buffer;
}

static void OpenLoop_UART_PollCommand(void)
{
    if (LL_USART_IsActiveFlag_RXNE(UART1) != 0U)
    {
        uint8_t command = LL_USART_ReceiveData8(UART1);

        if ((command == 's' || command == 'S') &&
            (MC_GetSTMStateMotor1() == IDLE) &&
            (MC_GetCurrentFaultsMotor1() == 0U))
        {
            /* Open-loop voltage is applied by the FOC task; issue a valid MCI
             * command first so the MCTK state machine accepts the start. */
            MC_ProgramTorqueRampMotor1(0, 0U);
            if (MC_StartMotor1())
            {
                UART_LED_Test_SendString("Open-loop start requested\r\n");
            }
            else
            {
                UART_LED_Test_SendString("Open-loop start rejected\r\n");
            }
        }
        else if (command == 'x' || command == 'X')
        {
            (void)MC_StopMotor1();
            UART_LED_Test_SendString("Stop requested\r\n");
        }
        else if (command == '?')
        {
            UART_LED_Test_SendString("s=start, x=stop, ?=help\r\n");
        }
    }
}

static void OpenLoop_UART_ReportAngles(void)
{
    static uint64_t last_report_ms;
    uint64_t now_ms = osal_tk2ms(osal_get_tick());

    if (MC_GetSTMStateMotor1() != RUN)
    {
        return;
    }

    if ((now_ms - last_report_ms) >= 100U)
    {
        int16_t foc_angle = FOCVars[M1].hElAngle;
        int16_t hall_angle = HALL_M1._Super.hElAngle;
        int32_t delta = (int32_t)foc_angle - (int32_t)hall_angle;
        if (delta > 32767) { delta -= 65536; }
        else if (delta < -32768) { delta += 65536; }
        int16_t delta_dpp = (int16_t)delta;
        int32_t delta_degrees = ((int32_t)delta_dpp * 360) / 65536;
        char line[96];
        char *out = line;

        last_report_ms = now_ms;
        out = OpenLoop_AppendInt(out, foc_angle);
        *out++ = ',';
        out = OpenLoop_AppendInt(out, hall_angle);
        *out++ = ',';
        out = OpenLoop_AppendInt(out, delta_dpp);
        *out++ = ',';
        out = OpenLoop_AppendInt(out, delta_degrees);
        *out++ = '\r';
        *out++ = '\n';
        *out = '\0';
        UART_LED_Test_SendString("ANGLE_DPP foc,hall,delta,delta_deg: ");
        UART_LED_Test_SendString(line);
    }
}

static void OpenLoop_UpdateLEDs(void)
{
    static uint64_t last_change_ms;
    static uint8_t led_index;
    uint64_t now_ms = osal_tk2ms(osal_get_tick());

    if ((now_ms - last_change_ms) >= 250U)
    {
        last_change_ms = now_ms;
        gpio_clear_pin(USER_LED_1);
        gpio_clear_pin(USER_LED_2);
        gpio_clear_pin(USER_LED_3);

        if (led_index == 0U) { gpio_set_pin(USER_LED_1); }
        else if (led_index == 1U) { gpio_set_pin(USER_LED_2); }
        else { gpio_set_pin(USER_LED_3); }

        led_index = (uint8_t)((led_index + 1U) % 3U);
    }
}
#endif /* OPEN_LOOP_FOC_TEST */


/**
 * @brief   Callback function triggered when CAN2 receives a frame.
 * @details Evaluates the frame ID to control either LEDs or Motor behavior.
 */

/**
 * @brief Reads the current logic level of the 3 Hall sensor pins.
 * @return A 3-bit value representing the Hall state (H3-H2-H1).
 */
static inline uint8_t HW_Read_Hall_Sensors(void)
{
    return (uint8_t)((gpio_read_pin(MC_HALL_CH3) << 2U) |
                     (gpio_read_pin(MC_HALL_CH2) << 1U) |
                      gpio_read_pin(MC_HALL_CH1));
}

/* Standalone LED/Hall bring-up UART initialization. */
#if defined(LED_ONLY_TEST) && !defined(OPEN_LOOP_FOC_TEST)
static void UART_LED_Test_Init(void)
{
    LL_USART_InitTypeDef config = {0};

    clock_enable_UART1(true);
    config.PrescalerValue = LL_USART_PRESCALER_DIV1;
    config.BaudRate = USART_SPEED;
    config.DataWidth = LL_USART_DATAWIDTH_8B;
    config.StopBits = LL_USART_STOPBITS_1;
    config.Parity = LL_USART_PARITY_NONE;
    config.TransferDirection = LL_USART_DIRECTION_TX_RX;
    config.HardwareFlowControl = LL_USART_HWCONTROL_NONE;
    config.OverSampling = LL_USART_OVERSAMPLING_16;

    LL_USART_Init(UART1, &config);
    LL_USART_SetTXRXSwap(UART1, LL_USART_TXRX_STANDARD);
    LL_USART_DisableFIFO(UART1);
    LL_USART_ConfigAsyncMode(UART1);
    LL_USART_Enable(UART1);
    while ((LL_USART_IsActiveFlag_TEACK(UART1) == 0U) ||
           (LL_USART_IsActiveFlag_REACK(UART1) == 0U)) { }
}
#endif

/* Blocking UART1 transmit helper for low-rate diagnostic text. */
static void UART_LED_Test_SendString(const char *text)
{
    while (*text != '\0')
    {
        while (LL_USART_IsActiveFlag_TXE(UART1) == 0U) { }
        LL_USART_TransmitData8(UART1, (uint8_t)*text++);
    }
    while (LL_USART_IsActiveFlag_TC(UART1) == 0U) { }
}

#if defined(LED_ONLY_TEST) && !defined(OPEN_LOOP_FOC_TEST)
static void UART_LED_Test_Poll(void)
{
    if (LL_USART_IsActiveFlag_RXNE(UART1) != 0U)
    {
        uint8_t byte = LL_USART_ReceiveData8(UART1);
        while (LL_USART_IsActiveFlag_TXE(UART1) == 0U) { }
        LL_USART_TransmitData8(UART1, byte);
    }
}

static void UART_LED_Test_ReportHall(void)
{
    static uint8_t previous = 0xFFU;
    uint8_t hall = HW_Read_Hall_Sensors();
    char message[] = "HALL H3=0 H2=0 H1=0 raw=00\r\n";

    if (hall == previous) { return; }
    previous = hall;
    message[8] = (char)('0' + ((hall >> 2U) & 1U));
    message[13] = (char)('0' + ((hall >> 1U) & 1U));
    message[18] = (char)('0' + (hall & 1U));
    message[25] = (char)('0' + (hall / 10U));
    message[26] = (char)('0' + (hall % 10U));
    UART_LED_Test_SendString(message);
}

static void UART_LED_Test_Wait(void)
{
    for (volatile uint32_t i = 0U; i < 3000000U; ++i)
    {
        if ((i & 0x3FFU) == 0U)
        {
            UART_LED_Test_Poll();
            UART_LED_Test_ReportHall();
        }
    }
}

static void UART_LED_Test_Blink(void)
{
    gpio_set_pin(USER_LED_1);
    UART_LED_Test_Wait();
    gpio_clear_pin(USER_LED_1);

    gpio_set_pin(USER_LED_2);
    UART_LED_Test_Wait();
    gpio_clear_pin(USER_LED_2);

    gpio_set_pin(USER_LED_3);
    UART_LED_Test_Wait();
    gpio_clear_pin(USER_LED_3);
}
#endif

/**
 * @brief Disables all PWM outputs immediately to prevent shoot-through.
 */
static inline void HW_Disable_All_Phases(void)
{
    /* Clear all CCxE and CCxNE bits for CH1, CH2, CH3 in CCER Register */
	LL_TIM_CC_DisableChannel(MOTOR_TIM, LL_TIM_CHANNEL_CH1 | LL_TIM_CHANNEL_CH1N |
	                                        LL_TIM_CHANNEL_CH2 | LL_TIM_CHANNEL_CH2N |
	                                        LL_TIM_CHANNEL_CH3 | LL_TIM_CHANNEL_CH3N);
}

/**
 * @brief Configures Phase U (Channel 1)
 */
static inline void HW_Set_Phase_U(uint8_t phase_state, uint16_t pwm_duty)
{
    if (phase_state == PHASE_PWM)
    {
    	LL_TIM_OC_SetCompareCH3(MOTOR_TIM, pwm_duty);         /* Set PWM Duty Cycle */
    	LL_TIM_CC_DisableChannel(MOTOR_TIM, LL_TIM_CHANNEL_CH3N);
		LL_TIM_CC_EnableChannel(MOTOR_TIM, LL_TIM_CHANNEL_CH3);
		LL_TIM_CC_EnableChannel(MOTOR_TIM, LL_TIM_CHANNEL_CH3N);
    }
    else if (phase_state == PHASE_GND)
    {
    	LL_TIM_OC_SetCompareCH3(MOTOR_TIM, 0U);
		LL_TIM_CC_DisableChannel(MOTOR_TIM, LL_TIM_CHANNEL_CH3);
		LL_TIM_CC_EnableChannel(MOTOR_TIM, LL_TIM_CHANNEL_CH3);
		LL_TIM_CC_EnableChannel(MOTOR_TIM, LL_TIM_CHANNEL_CH3N);
    }
    else /* PHASE_OFF */
    {
    	LL_TIM_CC_DisableChannel(MOTOR_TIM, LL_TIM_CHANNEL_CH3 | LL_TIM_CHANNEL_CH3N);
    }
}

/**
 * @brief Configures Phase V (Channel 2)
 */
static inline void HW_Set_Phase_V(uint8_t phase_state, uint16_t pwm_duty)
{
	 if (phase_state == PHASE_PWM)
	    {
	    	LL_TIM_OC_SetCompareCH2(MOTOR_TIM, pwm_duty);         /* Set PWM Duty Cycle */
	    	LL_TIM_CC_DisableChannel(MOTOR_TIM, LL_TIM_CHANNEL_CH2N);
			LL_TIM_CC_EnableChannel(MOTOR_TIM, LL_TIM_CHANNEL_CH2);
			LL_TIM_CC_EnableChannel(MOTOR_TIM, LL_TIM_CHANNEL_CH2N);
	    }
	    else if (phase_state == PHASE_GND)
	    {
	    	LL_TIM_OC_SetCompareCH2(MOTOR_TIM, 0U);
			LL_TIM_CC_DisableChannel(MOTOR_TIM, LL_TIM_CHANNEL_CH2);
			LL_TIM_CC_EnableChannel(MOTOR_TIM, LL_TIM_CHANNEL_CH2);
			LL_TIM_CC_EnableChannel(MOTOR_TIM, LL_TIM_CHANNEL_CH2N);
	    }
	    else /* PHASE_OFF */
	    {
	    	LL_TIM_CC_DisableChannel(MOTOR_TIM, LL_TIM_CHANNEL_CH2 | LL_TIM_CHANNEL_CH2N);
	    }
}

/**
 * @brief Configures Phase W (Channel 3)
 */
static inline void HW_Set_Phase_W(uint8_t phase_state, uint16_t pwm_duty)
{
	 if (phase_state == PHASE_PWM)
	    {
	    	LL_TIM_OC_SetCompareCH1(MOTOR_TIM, pwm_duty);         /* Set PWM Duty Cycle */
	    	LL_TIM_CC_DisableChannel(MOTOR_TIM, LL_TIM_CHANNEL_CH1N);
			LL_TIM_CC_EnableChannel(MOTOR_TIM, LL_TIM_CHANNEL_CH1);
			LL_TIM_CC_EnableChannel(MOTOR_TIM, LL_TIM_CHANNEL_CH1N);
	    }
	    else if (phase_state == PHASE_GND)
	    {
	    	LL_TIM_OC_SetCompareCH1(MOTOR_TIM, 0U);
			LL_TIM_CC_DisableChannel(MOTOR_TIM, LL_TIM_CHANNEL_CH1);
			LL_TIM_CC_EnableChannel(MOTOR_TIM, LL_TIM_CHANNEL_CH1);
			LL_TIM_CC_EnableChannel(MOTOR_TIM, LL_TIM_CHANNEL_CH1N);
	    }
	    else /* PHASE_OFF */
	    {
	    	LL_TIM_CC_DisableChannel(MOTOR_TIM, LL_TIM_CHANNEL_CH1 | LL_TIM_CHANNEL_CH1N);
	    }
}


/**
 * @brief Executes one step of the 6-step block commutation based on Hall state.
 * @param target_pwm_duty The desired PWM duty cycle (speed control).
 */
void SixStep_Commutate(uint16_t target_pwm_duty)
{
    current_hall = HW_Read_Hall_Sensors();

    /* Safety Check: If Hall state is invalid, disable everything immediately */
    if ((current_hall == HALL_STATE_ERR1) || (current_hall == HALL_STATE_ERR2))
    {
        HW_Disable_All_Phases();
        gpio_set_pin(USER_LED_2);
        return;
    }

    /* Apply the appropriate phase voltages based on 120-degree logic */
    switch (current_hall)
    {
        /* State 5 (101): Phase U is HIGH, Phase V is LOW, Phase W is OFF */
        case 5U:
            HW_Set_Phase_W(PHASE_OFF, 0U);             /* Turn off floating phase first! */
            HW_Set_Phase_V(PHASE_GND, 0U);             /* Connect Phase V to Ground      */
            HW_Set_Phase_U(PHASE_PWM, target_pwm_duty);/* Apply PWM to Phase U           */
            break;

        /* State 1 (001): Phase U is HIGH, Phase W is LOW, Phase V is OFF */
        case 1U:
            HW_Set_Phase_V(PHASE_OFF, 0U);
            HW_Set_Phase_W(PHASE_GND, 0U);
            HW_Set_Phase_U(PHASE_PWM, target_pwm_duty);
            break;

        /* State 3 (011): Phase V is HIGH, Phase W is LOW, Phase U is OFF */
        case 3U:
            HW_Set_Phase_U(PHASE_OFF, 0U);
            HW_Set_Phase_W(PHASE_GND, 0U);
            HW_Set_Phase_V(PHASE_PWM, target_pwm_duty);
            break;

        /* State 2 (010): Phase V is HIGH, Phase U is LOW, Phase W is OFF */
        case 2U:
            HW_Set_Phase_W(PHASE_OFF, 0U);
            HW_Set_Phase_U(PHASE_GND, 0U);
            HW_Set_Phase_V(PHASE_PWM, target_pwm_duty);
            break;

        /* State 6 (110): Phase W is HIGH, Phase U is LOW, Phase V is OFF */
        case 6U:
            HW_Set_Phase_V(PHASE_OFF, 0U);
            HW_Set_Phase_U(PHASE_GND, 0U);
            HW_Set_Phase_W(PHASE_PWM, target_pwm_duty);
            break;

        /* State 4 (100): Phase W is HIGH, Phase V is LOW, Phase U is OFF */
        case 4U:
            HW_Set_Phase_U(PHASE_OFF, 0U);
            HW_Set_Phase_V(PHASE_GND, 0U);
            HW_Set_Phase_W(PHASE_PWM, target_pwm_duty);
            break;

        /* Default safety fallback */
        default:
            HW_Disable_All_Phases();
            break;
    }
    /*
         * IMPORTANT FOR ST ADVANCED TIMERS:
         * If the Preload feature is enabled (CCPC bit in CR2), changes to CCER
         * and CCMR registers won't take effect until a Commutation Event (COM) is generated.
         * Uncomment the line below to trigger the COM event manually via software.
         */
         MOTOR_TIM->EGR |= TIM_EGR_COMG;
}


/**
 * @brief   Application entry point.
 * @note    Environment has been fully set up at this point.
 */
int main(void)
{

//	uint32_t can_timer_ms = 0U;
//	can_frame_t tx_frame;

    duty = 900;
    test_env_init(TEST_INIT_CLOCK | TEST_INIT_GPIO | TEST_INIT_BOARD |
                  TEST_INIT_IRQ | TEST_INIT_OSAL);

#if defined(LED_ONLY_TEST) && !defined(OPEN_LOOP_FOC_TEST)
    /* Standalone board check: no driver or motor-control initialization. */
    UART_LED_Test_Init();
    UART_LED_Test_SendString("UART1 test: 38400 8-N-1, type to echo.\r\n");
    for (;;)
    {
        UART_LED_Test_Poll();
        UART_LED_Test_ReportHall();
        UART_LED_Test_Blink();
    }
#else
    /* Enable interrupts before starting the motor-control application. */
  osal_sys_unlock();
  

//  /* L9907 Initial configuration */
//  L9907_init();

  gpio_set_pin(USER_LED_1);


  /* STDRIVE601 Initial configuration */
  STDRIVE601_Init();

  /* Enable and setting priority of IRQs used by Motor Control */
  /* ADC EOC - TSK High freq. */
  irq_set_priority(IRQ_HF_ADC_VECTOR, IRQ_HF_ADC_PRIO);
  irq_enable(IRQ_HF_ADC_VECTOR);
//
//  /* TIM BRK IN */
  irq_set_priority(IRQ_TIMx_BRK_VECTOR, IRQ_TIMBRK_PRIO);
  irq_enable(IRQ_TIMx_BRK_VECTOR);
//
//  /* TIMx UP */
  irq_set_priority(IRQ_TIMx_UP_VECTOR, IRQ_TIMx_UP_PRIO);
  irq_enable(IRQ_TIMx_UP_VECTOR);
#if defined (ENCODER) || defined(AUX_ENCODER)
  /* encoder  */
  irq_set_priority(ENC_TIMx_VECTOR, IRQ_ENC_TIMx_PRIO);
  irq_enable(ENC_TIMx_VECTOR);
#endif
//
#if defined(HALL_SENSORS) || defined(AUX_HALL_SENSORS)
  /* Hall Sensors */
  irq_set_priority(HALL_TIMx_VECTOR, IRQ_HALL_TIMx_PRIO);
  irq_enable(HALL_TIMx_VECTOR);
#endif
//
#if defined (USE_VBUS_EXT_ADC_OUT)
  /* VBUS sensing with L9502 */
//  irq_set_priority(VBUS_EXT_ADC_TIM_VECTOR, VBUS_EXT_ADC_TIM_PRIO);
//  irq_enable(VBUS_EXT_ADC_TIM_VECTOR);
#endif
//
#if defined (USE_TEMP_EXT_ADC_OUT)
  /* Temperature sensing with L9502 */
//  irq_set_priority(TEMP_EXT_ADC_TIM_VECTOR, TEMP_EXT_ADC_TIM_PRIO);
//  irq_enable(TEMP_EXT_ADC_TIM_VECTOR);
#endif
//
//#ifdef CAN_COMMUNICATION
//  /* FDCAN1 */
//  irq_set_priority(IRQ_CAN1_LINE0_VECTOR, IRQ_FDCAN1_PRIO);
//  irq_enable(IRQ_CAN1_LINE0_VECTOR);
//#endif  
//
#if defined(SERIAL_COMMUNICATION) && !defined(OPEN_LOOP_FOC_TEST)
  /* UART 1 */
  irq_set_priority(IRQ_UART1_VECTOR, IRQ_UART1_PRIO);
  irq_enable(IRQ_UART1_VECTOR);
#endif
  /* EXTI - Port D Pin 2 - STDRIVE601_FAULT */
  irq_set_priority(IRQ_EXTI2_VECTOR, IRQ_EXTI_PRIO);
  irq_enable(IRQ_EXTI2_VECTOR);
#if defined(RESOLVER) || defined(AUX_RESOLVER)
  /* Resolver ADC end of convers  */
  irq_set_priority(IRQ_RES_ADC_VECTOR, IRQ_RES_ADCx_PRIO);
  irq_enable(IRQ_RES_ADC_VECTOR);
#endif

  /* EXTI - S2 (PA14) & S3 (PA15) */
  irq_set_priority(IRQ_EXTI15_10_VECTOR, IRQ_EXTI_PRIO);
  irq_enable(IRQ_EXTI15_10_VECTOR);
  EXTI_Init();

  MX_MotorControl_Init();
#ifdef OPEN_LOOP_FOC_TEST
  /* Disable the MCTK serial protocol so raw text diagnostics own UART1. */
  LL_USART_DisableIT_RXNE(UART1);
  LL_USART_DisableIT_TXE(UART1);
  UART_LED_Test_SendString("Open-loop FOC angle monitor\r\ns=start, x=stop, ?=help\r\n");
#endif
  LL_TIM_IC_SetActiveInput(TIM2, LL_TIM_CHANNEL_CH3, LL_TIM_ACTIVEINPUT_DIRECTTI);
	LL_TIM_IC_SetPrescaler(TIM2, LL_TIM_CHANNEL_CH3, LL_TIM_ICPSC_DIV1);
	LL_TIM_IC_SetPolarity(TIM2, LL_TIM_CHANNEL_CH3, LL_TIM_IC_POLARITY_RISING);
	LL_TIM_IC_SetFilter(TIM2, LL_TIM_CHANNEL_CH3, LL_TIM_IC_FILTER_FDIV1);
	LL_TIM_CC_EnableChannel(TIM2, LL_TIM_CHANNEL_CH3);
	LL_TIM_ClearFlag_CC3(TIM2);
  LL_TIM_SetCounter(TIM2, 0);
  ICS_CurrentReadingPolarization(&PWM_Handle_M1._Super);

//  can_init(&DRV_CAN1);
//  can_config1(&DRV_CAN1); /* Áp dụng config1 (có filter 2032) cho CAN1 */
//  can_start(&DRV_CAN1);
//
//  can_init(&DRV_CAN2);
//  can_config1(&DRV_CAN2); /* Áp dụng config1 (có filter 2032) cho CAN2 */
//  can_start(&DRV_CAN2);
//
//  /* Chuẩn bị khung truyền tĩnh (Static frame) */
//  tx_frame.TYPE = CAN_ID_STD;
//  tx_frame.ID = 2032U;
//  tx_frame.DLC = 1U; /* Chỉ gửi 1 byte duy nhất */
//  tx_frame.OPERATION = CAN_OPERATION_NORMAL;

   /* enable D-cache */
  //SCB_EnableDCache();

#ifdef TEST_ADC_REG_CONV
  testRegConvADC.regADC = ADC5;
  testRegConvADC.channel = MC_ADC_CHANNEL_2; /* ADC SAR 1 In 8 - PD5 */
  testRegConvADC.samplingTime = LL_ADC_SAMPLING_CYCLE(47); 
  handleRegADC = RCM_RegisterRegConv(&testRegConvADC);
  dbg_handle = (uint32_t)handleRegADC;
#endif
    
//     if(StateMotor == IDLE)
//     {
//        {
//           while(IDLE != MC_GetSTMStateMotor1())
//           {
//           }
//           /* This command sets what will be the first speed ramp after the
//           MCI_StartMotor command. It requires as first parameter the oMCI[0], as
//           second parameter the target mechanical speed in thenth of Hz and as
//           third parameter the speed ramp duration in milliseconds. */
//  #if (OPEN_LOOP_FOC == TRUE)
//           MC_ProgramSpeedRampMotor1(OPEN_LOOP_SPEED_RPM, OPEN_LOOP_SPEED_RAMP_DURATION_MS);
//  #else
//           MC_ProgramSpeedRampMotor1(300/6, 2000);
//  #endif
//           /* This is a user command used to start the motor. The speed ramp shall be
//              pre programmed before the command.*/
////           startMot = MC_StartMotor1();
//
//        }
//     }
//  MC_ProgramSpeedRampMotor1(10, OPEN_LOOP_SPEED_RAMP_DURATION_MS);
//  LL_TIM_EnableCounter(MOTOR_TIM);       /* Cho phép Timer bắt đầu đếm */
//  LL_TIM_EnableAllOutputs(MOTOR_TIM);
//  HALL_Init(&HALL_M1);



//  startMot = MC_StartMotor1();
//  uint32_t timeout = 1000000U;
//  while ((MC_GetSTMStateMotor1() != RUN) && (--timeout > 0U))
//  {
//	osal_delay_millisec(1U);
//  }
#if defined (ENCODER) || defined(AUX_ENCODER)
  MC_AlignEncoderMotor1();
    /* 2. Wait until alignment finishes and motor state returns to IDLE */
    while (MC_GetSTMStateMotor1() != IDLE)
    {
    	MC_RunMotorControlTasks();
    	osal_delay_millisec(10);
    }

#endif

#if defined(TEST_ADC_REG_CONV) && !defined(OPEN_LOOP_FOC_TEST)
  MC_StartMotor1();
#endif
//  MC_StartMotor1();



  /* This is the system idle loop, it is executed when no other tasks are
     ready for execution.*/
   while (true) {
//	 startMot = MC_StartMotor1();
//	  MC_RunMotorControlTasks();
	#if defined (ENCODER) || defined(AUX_ENCODER)
	DebugAngle = ENC_CalcAngle(&ENCODER_M1);
	#endif


	 ab_t raw_currents = MC_GetIabMotor1();

	 /* FOC SVPWM compare (timer ticks). ARR = PWM_PERIOD_CYCLES/2 (center-aligned) */
	 dbg_dutyA = PWM_Handle_M1._Super.CntPhA;
	 dbg_dutyB = PWM_Handle_M1._Super.CntPhB;
	 dbg_dutyC = PWM_Handle_M1._Super.CntPhC;

	 /* Hardware TIM1 CCR: CH1=Phase A, CH2=Phase B, CH3=Phase C */
	 dbg_ccrA = (uint16_t)LL_TIM_OC_GetCompareCH1(MOTOR_TIM);
	 dbg_ccrB = (uint16_t)LL_TIM_OC_GetCompareCH2(MOTOR_TIM);
	 dbg_ccrC = (uint16_t)LL_TIM_OC_GetCompareCH3(MOTOR_TIM);

	 dbg_dutyA_pct = (uint16_t)(((uint32_t)dbg_dutyA * 100U) / (PWM_PERIOD_CYCLES / 2U));
	 dbg_dutyB_pct = (uint16_t)(((uint32_t)dbg_dutyB * 100U) / (PWM_PERIOD_CYCLES / 2U));
	 dbg_dutyC_pct = (uint16_t)(((uint32_t)dbg_dutyC * 100U) / (PWM_PERIOD_CYCLES / 2U));

	 debug_Ia = raw_currents.a;
	 debug_Ib = raw_currents.b;


    /* Idle loop.*/
#if defined(TEST_ADC_REG_CONV) && !defined(OPEN_LOOP_FOC_TEST)
	 rcm_state = RCM_GetUserConvState();
	 if(RCM_GetUserConvState() == RCM_USERCONV_IDLE)
    {
      RCM_RequestUserConv(handleRegADC);
    }
    if(RCM_GetUserConvState() == RCM_USERCONV_EOC)
    {


       testRegConvADCData = RCM_GetUserConv();
       debug_VSPEED_raw = testRegConvADCData;
       if (debug_VSPEED_raw < 100)
       {
    	   throttle_zero_check = TRUE;
    	   gpio_clear_pin(STDRIVE_EN);
    	   MC_StopMotor1();
       }
       else if(throttle_zero_check & (gear_N == FALSE))
	   {
		  gpio_set_pin(STDRIVE_EN);
		  MC_StartMotor1();
	   }
    }
#endif

#ifdef MCTK_MC_MODE
    MC_RunMotorControlTasks();
#endif
#ifdef OPEN_LOOP_FOC_TEST
    OpenLoop_UART_PollCommand();
    OpenLoop_UART_ReportAngles();
    OpenLoop_UpdateLEDs();
#endif
  }
#endif /* LED_ONLY_TEST */
}

/* IRQ handler for STDRIVE_EN pin */
IRQ_HANDLER(IRQ_EXTI2_HANDLER)
{
    if (LL_EXTI_ReadFlag_0_31(LL_EXTI_LINE_2))
    {
        LL_EXTI_ClearFlag_0_31(LL_EXTI_LINE_2);
        faults ++;
        /* A gate-driver fault must stop PWM immediately. */
        (void)MC_StopMotor1();
        gpio_clear_pin(STDRIVE_EN);
        gpio_set_pin(USER_LED_2);

    }
}

/* IRQ handler for S1 & S2 pin */
IRQ_HANDLER(IRQ_EXTI15_10_HANDLER)
{
    /* S1 PA13 */
    if (LL_EXTI_ReadFlag_0_31(LL_EXTI_LINE_13))
    {
        LL_EXTI_ClearFlag_0_31(LL_EXTI_LINE_13);
        gear_D =! gear_D;
        if (gear_R)
		{
			gear_R = FALSE;
			gear_D = FALSE;
			gear_N = TRUE;
		}
        if (gear_D)
        {
        	gpio_set_pin(USER_LED_2);
        	gear_R = FALSE;
        	gear_N = FALSE;
        }
        else
        {
        	gear_N = TRUE;
        	gpio_clear_pin(STDRIVE_EN);
			MC_StopMotor1();
        	gpio_clear_pin(USER_LED_2);
        }
        gpio_clear_pin(USER_LED_3);
        throttle_zero_check = FALSE;
    }
    /* S2 PA14 */
	if (LL_EXTI_ReadFlag_0_31(LL_EXTI_LINE_14))
	{
		LL_EXTI_ClearFlag_0_31(LL_EXTI_LINE_14);
		/* S3 nhấn và nhả -> Bật USER_LED_2 */
		gear_R =! gear_R;
		if (gear_D)
		{
			gear_R = FALSE;
			gear_D = FALSE;
			gear_N = TRUE;
		}
		if (gear_R)
		{
			gpio_set_pin(USER_LED_3);
			gear_N = FALSE;
			gear_D = FALSE;
		}
		else
		{
			gear_N = TRUE;
			gpio_clear_pin(STDRIVE_EN);
			MC_StopMotor1();
			gpio_clear_pin(USER_LED_3);
		}
		gpio_clear_pin(USER_LED_2);
		throttle_zero_check = FALSE;
		// Hoặc dùng gpio_toggle_pin(USER_LED_2); nếu muốn nhấn để bật/tắt luân phiên
	}
}

/**
  * @brief Khởi tạo Driver STDRIVE601 (Kích chân EN lên High)
  */
static void STDRIVE601_Init(void)
{
  /* Pull  STDRIVE_EN High */
	gpio_set_pin(STDRIVE_EN);
#ifdef MCTK_DEBUG_OFF_STDRIVE601
	gpio_clear_pin(STDRIVE_EN);
#endif

}
 
/**
  * @brief  This function is executed in case of error occurrence.
  * @retval None
  */
void Error_Handler(void)
{
  /* USER CODE BEGIN Error_Handler_Debug */
  /* User can add his own implementation to report the HAL error return state */
  __disable_irq();
  while (1)
  {

  }
  /* USER CODE END Error_Handler_Debug */
}


/**
  * @brief GPIO Initialization Function
  * @param None
  * @retval None
  */
static void EXTI_Init(void)
{

  LL_EXTI_InitTypeDef EXTI_InitStruct = {0};

  /* STDRIVE601 FAULT - PAD_PD2  */
  LL_SYSCFG_SetEXTISource(LL_SYSCFG_EXTI_PORTD, LL_SYSCFG_EXTI_LINE2);

  EXTI_InitStruct.Line_0_31 = LL_EXTI_LINE_2;
  EXTI_InitStruct.LineCommand = ENABLE;
  EXTI_InitStruct.Mode = LL_EXTI_MODE_IT;
  EXTI_InitStruct.Trigger = LL_EXTI_TRIGGER_FALLING;
  LL_EXTI_Init(&EXTI_InitStruct);

  /* S2 PA14 and S3 PA15 */
  LL_SYSCFG_SetEXTISource(LL_SYSCFG_EXTI_PORTA, LL_SYSCFG_EXTI_LINE13);
  LL_SYSCFG_SetEXTISource(LL_SYSCFG_EXTI_PORTA, LL_SYSCFG_EXTI_LINE14);

  EXTI_InitStruct.Line_0_31 = LL_EXTI_LINE_13 | LL_EXTI_LINE_14;
  EXTI_InitStruct.LineCommand = ENABLE;
  EXTI_InitStruct.Mode = LL_EXTI_MODE_IT;
  EXTI_InitStruct.Trigger = LL_EXTI_TRIGGER_RISING;
  LL_EXTI_Init(&EXTI_InitStruct);

  /* L9907 FS FLAG - PAD_PI2 (EXTI_POSITION_8  | EXTICR[0]) */
  //LL_SYSCFG_SetEXTISource(LL_SYSCFG_EXTI_PORTH, LL_SYSCFG_EXTI_LINE5);
//  LL_SYSCFG_SetEXTISource(LL_SYSCFG_EXTI_PORTI, LL_SYSCFG_EXTI_LINE2);
//
//  EXTI_InitStruct.Line_0_31 = LL_EXTI_LINE_2;
//  EXTI_InitStruct.LineCommand = ENABLE;
//  EXTI_InitStruct.Mode = LL_EXTI_MODE_IT;
//  EXTI_InitStruct.Trigger = LL_EXTI_TRIGGER_FALLING;
//  LL_EXTI_Init(&EXTI_InitStruct);

}

/**
  * @brief   Callback function triggered upon receiving a CAN frame.
 * @details Parses the incoming control frame to start, stop, or set motor speed.
 *
 * @param[in] cdp   Pointer to the CAN driver object.
 * @param[in] frame Received CAN frame data.
 * @param[in] event CAN event type.
 */

#ifdef USE_L9502
/**
  * @brief  Button to set EN2 on L9907 board.
  * @param  None
  * @retval None
  */
void L9502_En_Button(void)
{
  /* enable DIS pin */
  gpio_set_pin(L9502_DIS); /* LS */
#if defined(L9502_DIS2)
  gpio_set_pin(L9502_DIS2); /* LS */
#endif
}
#endif

#ifdef USE_L9907
/**
  * @brief  Button to set EN2 on L9907 board.
  * @param  None
  * @retval None
  */
void L9907_En2_Button(void)
{
  gpio_set_pin(E2_GPIO);gpio_set_pin(E1_GPIO);
}
#endif
