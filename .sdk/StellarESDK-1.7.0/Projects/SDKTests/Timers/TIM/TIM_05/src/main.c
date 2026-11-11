/****************************************************************************
 *
 * Copyright (c) 2024 STMicroelectronics - All Rights Reserved
 *
 * License terms: STMicroelectronics Proprietary in accordance with licensing
 * terms SLA0098 at www.st.com.
 *
 * THIS SOFTWARE IS DISTRIBUTED "AS IS," AND ALL WARRANTIES ARE DISCLAIMED,
 * INCLUDING MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE.
 *
 *****************************************************************************/

#include <test_env.h>
#include <tim.h>

int main(void) {

    /* Enable interrupts.*/
    osal_sys_unlock();

    test_env_init((TestInit_t)
                  (TEST_INIT_CLOCK    |
                   TEST_INIT_GPIO     |
                   TEST_INIT_BOARD    |
                   TEST_INIT_IRQ      |
                   TEST_INIT_OSAL));

    /* Initialize TIM1 and TIM2 driver instances.*/
    tim_init(&DRV_TIM1);
    tim_init(&DRV_TIM2);

    /* Configure TIM1 and TIM2 drivers.*/

    /* Configure TIM1 tick frequency to 100kHz and channel 1 mode as PWM active
       low.*/
    (void)tim_set_presc(&DRV_TIM1, TIM_DIVIDE_BY(tim_get_clock(&DRV_TIM1) / 100000U));
    (void)tim_set_ch_mode(&DRV_TIM1, 1U, TIM_MODE_PWM_ACTIVE_LOW);

    /* Enable TIM1 one pulse mode.*/
    (void)tim_enable_opm(&DRV_TIM1, true);

    /* Configure TIM1 slave mode to "Combined reset + trigger mode" and TIM2
       as trigger input.*/
    (void)tim_set_sms(&DRV_TIM1, TIM_SMODE_COMBO_RESET_TRIGGER);
    (void)tim_set_trgi(&DRV_TIM1, TIM_TRGI_ITR1);

    /* Configure TIM2 tick frequency to 100kHz and channel 1 mode as PWM active
       high.*/
    (void)tim_set_presc(&DRV_TIM2, TIM_DIVIDE_BY(tim_get_clock(&DRV_TIM2) / 100000U));
    (void)tim_set_ch_mode(&DRV_TIM2, 1U, TIM_MODE_PWM_ACTIVE_HIGH);

    /* Configure TIM2 update event as trigger output.*/
    (void)tim_set_trgo(&DRV_TIM2, TIM_TRGO_UPDATE);

    /* Apply TIM configurations.*/
    tim_start(&DRV_TIM1);
    tim_start(&DRV_TIM2);

    /* Start PWM active low on TIM1 channel 1 (frequency 2Hz, duty cycle 20%).*/
    tim_set_interval(&DRV_TIM1, 50000U);        /* 100kHz/50000 = 2Hz.*/
    tim_set_comparator(&DRV_TIM1, 1U, 40000U);  /* Duty cycle 20%.    */

    /* Start PWM active high on TIM2 channel 1 (frequency 2Hz, duty cycle 25%).*/
    tim_set_interval(&DRV_TIM2, 50000U);        /* 100kHz/50000 = 2Hz.*/
    tim_set_comparator(&DRV_TIM2, 1U, 12500U);  /* Duty cycle 25%.    */

    /* Enable TIM2.*/
    tim_enable(&DRV_TIM2);

    osal_delay_millisec(50U);

    /* Enable TIM1.*/
    tim_enable(&DRV_TIM1);
 
    /* Application main loop */
    for ( ; ; ) {
        /* Blink USER_LED_A.*/
        gpio_toggle_pin(USER_LED_A);
        osal_delay_millisec(500U);
    }
}
