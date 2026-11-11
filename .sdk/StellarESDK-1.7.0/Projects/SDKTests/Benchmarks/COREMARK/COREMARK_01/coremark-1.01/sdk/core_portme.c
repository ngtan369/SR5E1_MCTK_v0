/* 
	File : core_portme.c
*/
/*
	Author : Shay Gal-On, EEMBC
	Legal : TODO!
*/ 
#include <stdio.h>
#include <stdlib.h>
#include "coremark.h"

#if 0
#include <typedefs.h>
#include <clock.h>
#include <gpio.h>
#include <irq.h>
#include <osal.h>
#endif

#include <test_env.h>
#include <uart.h>
#include <io.h>

#if VALIDATION_RUN
	volatile ee_s32 seed1_volatile=0x3415;
	volatile ee_s32 seed2_volatile=0x3415;
	volatile ee_s32 seed3_volatile=0x66;
#endif
#if PERFORMANCE_RUN
	volatile ee_s32 seed1_volatile=0x0;
	volatile ee_s32 seed2_volatile=0x0;
	volatile ee_s32 seed3_volatile=0x66;
#endif
#if PROFILE_RUN
	volatile ee_s32 seed1_volatile=0x8;
	volatile ee_s32 seed2_volatile=0x8;
	volatile ee_s32 seed3_volatile=0x8;
#endif
	volatile ee_s32 seed4_volatile=ITERATIONS;
	volatile ee_s32 seed5_volatile=0;
/* Porting : Timing functions
	How to capture time and convert to seconds must be ported to whatever is supported by the platform.
	e.g. Read value from on board RTC, read value from cpu clock cycles performance counter etc. 
	Sample implementation for standard time.h and windows.h definitions included.
*/
/* Define : TIMER_RES_DIVIDER
	Divider to trade off timer resolution and total time that can be measured.

	Use lower values to increase resolution, but make sure that overflow does not occur.
	If there are issues with the return value overflowing, increase this value.
	*/
/*	EE_TICKS_PER_SEC is the number of ticks per second. For example, for a core
	frequency of 306.7MHz, the number of ticks per second must be equal to
	306700000. Since EE_TICKS_PER_SEC is calculated as division between
	NSECS_PER_SEC and TIMER_RES_DIVIDER, to avoid rounding errors the best way
	to proceed is set CLOCKS_PER_SEC equal to the core frequency (306700000)
	and TIMER_RES_DIVIDER to 1.*/
#define CLOCKS_PER_SEC  CLOCK_SYSCLKDIV

#define NSECS_PER_SEC CLOCKS_PER_SEC
#define CORETIMETYPE uint64_t
#define GETMYTIME(_t) (*_t=osal_get_tick())
#define MYTIMEDIFF(fin,ini) ((fin)-(ini))
#define TIMER_RES_DIVIDER 1
#define SAMPLE_TIME_IMPLEMENTATION 1
#define EE_TICKS_PER_SEC (NSECS_PER_SEC / TIMER_RES_DIVIDER)

/** Define Host specific (POSIX), or target specific global time variables. */
static CORETIMETYPE start_time_val, stop_time_val;

/* Function : start_time
	This function will be called right before starting the timed portion of the benchmark.

	Implementation may be capturing a system timer (as implemented in the example code) 
	or zeroing some system parameters - e.g. setting the cpu clocks cycles to 0.
*/
void start_time(void) {
	GETMYTIME(&start_time_val );      
}
/* Function : stop_time
	This function will be called right after ending the timed portion of the benchmark.

	Implementation may be capturing a system timer (as implemented in the example code) 
	or other system parameters - e.g. reading the current value of cpu cycles counter.
*/
void stop_time(void) {
	GETMYTIME(&stop_time_val );      
}
/* Function : get_time
	Return an abstract "ticks" number that signifies time on the system.
	
	Actual value returned may be cpu cycles, milliseconds or any other value,
	as long as it can be converted to seconds by <time_in_secs>.
	This methodology is taken to accomodate any hardware or simulated platform.
	The sample implementation returns millisecs by default, 
	and the resolution is controlled by <TIMER_RES_DIVIDER>
*/
CORE_TICKS get_time(void) {
	CORE_TICKS elapsed=(CORE_TICKS)(MYTIMEDIFF(stop_time_val, start_time_val));
	return elapsed;
}
/* Function : time_in_secs
	Convert the value returned by get_time to seconds.

	The <secs_ret> type is used to accomodate systems with no support for floating point.
	Default implementation implemented by the EE_TICKS_PER_SEC macro above.
*/
secs_ret time_in_secs(CORE_TICKS ticks) {
	secs_ret retval=((secs_ret)ticks) / (secs_ret)EE_TICKS_PER_SEC;
	return retval;
}

ee_u32 default_num_contexts=1;

/* Function : portable_init
	Target specific initialization code 
	Test for some common mistakes.
*/
void portable_init(core_portable *p, int *argc, char *argv[])
{
	(void) argc;
	(void) argv;

	if (sizeof(ee_ptr_int) != sizeof(ee_u8 *)) {
		ee_printf("ERROR! Please define ee_ptr_int to a type that holds a pointer!\n");
	}
	if (sizeof(ee_u32) != 4) {
		ee_printf("ERROR! Please define ee_u32 to a 32b unsigned type!\n");
	}

    /* Enable Data Cache.*/
    SCB_EnableDCache();

    /* Enable interrupts.*/
    osal_sys_unlock();

	/* Initialize required modules */
    clock_init();
    gpio_init();
    BOARD_INIT_FUNC(BOARD_NAME);
    osal_init();

    /* Switch-off user led.*/
    USER_LED_SWITCH_OFF(USER_LED_A);

    /* Initialize UART driver instance used for IO redirection.*/
    uart_init(&UARTD);

    /* Configure UART driver instance used for IO redirection.*/
    (void)uart_set_prio(&UARTD, IRQ_PRIORITY_5);
    (void)uart_set_baud(&UARTD, UART_BAUDRATE_38400);
    (void)uart_set_presc(&UARTD, UART_PRESCALER_DIV1);
    (void)uart_set_parity(&UARTD, UART_PARITY_NONE);
    (void)uart_set_over(&UARTD, UART_OVERSAMPLING_16);
    (void)uart_set_sbit(&UARTD, UART_STOPBIT_1);

    /* Initialize Runtime IO module.*/
    io_init(&UARTD);

    /* Start Runtime IO module.*/
    io_start();

	p->portable_id=1;

    /* Switch-on user led.*/
    USER_LED_SWITCH_ON(USER_LED_A);
}
/* Function : portable_fini
	Target specific final code 
*/
void portable_fini(core_portable *p)
{
    /* Switch-off user led.*/
    USER_LED_SWITCH_OFF(USER_LED_A);
	p->portable_id=0;
}

#if (MEM_METHOD==MEM_MALLOC)
#include <malloc.h>
/* Function: portable_malloc
	Provide malloc() functionality in a platform specific way.
*/
void *portable_malloc(size_t size) {
	return malloc(size);
}
/* Function: portable_free
	Provide free() functionality in a platform specific way.
*/
void portable_free(void *p) {
	free(p);
}
#endif


