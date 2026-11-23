/****************************************************************************
*
* Copyright (c) 2022-2026 STMicroelectronics - All Rights Reserved
*
* License terms: STMicroelectronics Proprietary in accordance with licensing
* terms SLA0098 at www.st.com.
*
* THIS SOFTWARE IS DISTRIBUTED "AS IS," AND ALL WARRANTIES ARE DISCLAIMED,
* INCLUDING MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE.
*
* PLEASE NOTE THAT THIS FILE IS GENERATED FROM A CONFIGURATION FILE,
* USING com.st.stellar.pinmap.gpiocfg, Version 2.1.0.202610050803
* MODIFYING IT IMPLIES POTENTIAL LOSS OF YOUR MODIFICATIONS.
*
*****************************************************************************/
/**
 * @file    evbe7000s.h
 * @brief   SR5E1-MYBOARD board header file.
 *
 * @addtogroup PLATFORM
 * @addtogroup BOARD
 * @ingroup PLATFORM
 * @addtogroup EVBE7000S
 * @ingroup BOARD
 * @{
 */

#ifndef _EVBE7000S_H_
#define _EVBE7000S_H_

#include <gpio.h>
#include <platform.h>
#include <typedefs.h>


/*===========================================================================*/
/* Module constants.                                                         */
/*===========================================================================*/

#define MYBOARD	1

/*--------------------- CAN1 pins definitions ---------------------*/
#define CAN1_RX gpio_iopack(GPIO_PORT_A, GPIO_PIN_4) /* PA[4] */
#define CAN1_RX_CFG                                 \
	(GPIO_MODE_MODER_ALTERNATE |                    \
	 GPIO_MODE_OTYPER_PUSHPULL |                    \
	 GPIO_MODE_OSPEEDR_LOW |                        \
	 GPIO_MODE_PUPDR_FLOATING |                     \
	 GPIO_MODE_AFR(6U) |                            \
	 GPIO_MODE_IHYSTR_CMOS |                        \
	 GPIO_MODE_TRIGENR_IN_ENABLED |                 \
	 GPIO_MODE_SAFESELR_DISABLED |                  \
	 GPIO_MODE_SAFEVALR_LOW)

#define CAN1_TX gpio_iopack(GPIO_PORT_A, GPIO_PIN_5) /* PA[5] */
#define CAN1_TX_CFG                                 \
	(GPIO_MODE_MODER_ALTERNATE |                    \
	 GPIO_MODE_OTYPER_PUSHPULL |                    \
	 GPIO_MODE_OSPEEDR_LOW |                        \
	 GPIO_MODE_PUPDR_FLOATING |                     \
	 GPIO_MODE_AFR(6U) |                            \
	 GPIO_MODE_IHYSTR_CMOS |                        \
	 GPIO_MODE_TRIGENR_IN_DISABLED |                \
	 GPIO_MODE_SAFESELR_DISABLED |                  \
	 GPIO_MODE_SAFEVALR_LOW)


/*--------------------- SAR1 pins definitions ---------------------*/
#define CIW gpio_iopack(GPIO_PORT_B, GPIO_PIN_2) /* PB[2] */
#define CIW_CFG                                     \
	(GPIO_MODE_MODER_ANALOG |                       \
	 GPIO_MODE_OTYPER_PUSHPULL |                    \
	 GPIO_MODE_OSPEEDR_LOW |                        \
	 GPIO_MODE_PUPDR_FLOATING |                     \
	 GPIO_MODE_AFR(0U) |                            \
	 GPIO_MODE_IHYSTR_CMOS |                        \
	 GPIO_MODE_TRIGENR_IN_DISABLED |                \
	 GPIO_MODE_SAFESELR_DISABLED |                  \
	 GPIO_MODE_SAFEVALR_LOW)


/*--------------------- SAR2 pins definitions ---------------------*/
#define CIV gpio_iopack(GPIO_PORT_B, GPIO_PIN_7) /* PB[7] */
#define CIV_CFG                                     \
	(GPIO_MODE_MODER_ANALOG |                       \
	 GPIO_MODE_OTYPER_PUSHPULL |                    \
	 GPIO_MODE_OSPEEDR_LOW |                        \
	 GPIO_MODE_PUPDR_FLOATING |                     \
	 GPIO_MODE_AFR(0U) |                            \
	 GPIO_MODE_IHYSTR_CMOS |                        \
	 GPIO_MODE_TRIGENR_IN_DISABLED |                \
	 GPIO_MODE_SAFESELR_DISABLED |                  \
	 GPIO_MODE_SAFEVALR_LOW)


/*--------------------- SAR3 pins definitions ---------------------*/
#define CIU gpio_iopack(GPIO_PORT_B, GPIO_PIN_12) /* PB[12] */
#define CIU_CFG                                     \
	(GPIO_MODE_MODER_ANALOG |                       \
	 GPIO_MODE_OTYPER_PUSHPULL |                    \
	 GPIO_MODE_OSPEEDR_LOW |                        \
	 GPIO_MODE_PUPDR_FLOATING |                     \
	 GPIO_MODE_AFR(0U) |                            \
	 GPIO_MODE_IHYSTR_CMOS |                        \
	 GPIO_MODE_TRIGENR_IN_DISABLED |                \
	 GPIO_MODE_SAFESELR_DISABLED |                  \
	 GPIO_MODE_SAFEVALR_LOW)

#define PIN_SAR3_IN2 gpio_iopack(GPIO_PORT_B, GPIO_PIN_13) /* PB[13] */
#define PIN_SAR3_IN2_CFG                            \
	(GPIO_MODE_MODER_ANALOG |                       \
	 GPIO_MODE_OTYPER_PUSHPULL |                    \
	 GPIO_MODE_OSPEEDR_LOW |                        \
	 GPIO_MODE_PUPDR_FLOATING |                     \
	 GPIO_MODE_AFR(0U) |                            \
	 GPIO_MODE_IHYSTR_CMOS |                        \
	 GPIO_MODE_TRIGENR_IN_DISABLED |                \
	 GPIO_MODE_SAFESELR_DISABLED |                  \
	 GPIO_MODE_SAFEVALR_LOW)


/*--------------------- SAR4 pins definitions ---------------------*/
#define TEMP gpio_iopack(GPIO_PORT_C, GPIO_PIN_3) /* PC[3] */
#define TEMP_CFG                                    \
	(GPIO_MODE_MODER_ANALOG |                       \
	 GPIO_MODE_OTYPER_PUSHPULL |                    \
	 GPIO_MODE_OSPEEDR_LOW |                        \
	 GPIO_MODE_PUPDR_FLOATING |                     \
	 GPIO_MODE_AFR(0U) |                            \
	 GPIO_MODE_IHYSTR_CMOS |                        \
	 GPIO_MODE_TRIGENR_IN_DISABLED |                \
	 GPIO_MODE_SAFESELR_DISABLED |                  \
	 GPIO_MODE_SAFEVALR_LOW)


/*--------------------- SAR5 pins definitions ---------------------*/
#define VBUS gpio_iopack(GPIO_PORT_C, GPIO_PIN_9) /* PC[9] */
#define VBUS_CFG                                    \
	(GPIO_MODE_MODER_ANALOG |                       \
	 GPIO_MODE_OTYPER_PUSHPULL |                    \
	 GPIO_MODE_OSPEEDR_LOW |                        \
	 GPIO_MODE_PUPDR_FLOATING |                     \
	 GPIO_MODE_AFR(0U) |                            \
	 GPIO_MODE_IHYSTR_CMOS |                        \
	 GPIO_MODE_TRIGENR_IN_DISABLED |                \
	 GPIO_MODE_SAFESELR_DISABLED |                  \
	 GPIO_MODE_SAFEVALR_LOW)

#define VSPEED gpio_iopack(GPIO_PORT_C, GPIO_PIN_10) /* PC[10] */
#define VSPEED_CFG                                  \
	(GPIO_MODE_MODER_ANALOG |                       \
	 GPIO_MODE_OTYPER_PUSHPULL |                    \
	 GPIO_MODE_OSPEEDR_LOW |                        \
	 GPIO_MODE_PUPDR_FLOATING |                     \
	 GPIO_MODE_AFR(0U) |                            \
	 GPIO_MODE_IHYSTR_CMOS |                        \
	 GPIO_MODE_TRIGENR_IN_DISABLED |                \
	 GPIO_MODE_SAFESELR_DISABLED |                  \
	 GPIO_MODE_SAFEVALR_LOW)

#define NTC gpio_iopack(GPIO_PORT_C, GPIO_PIN_11) /* PC[11] */
#define NTC_CFG                                     \
	(GPIO_MODE_MODER_ANALOG |                       \
	 GPIO_MODE_OTYPER_PUSHPULL |                    \
	 GPIO_MODE_OSPEEDR_LOW |                        \
	 GPIO_MODE_PUPDR_FLOATING |                     \
	 GPIO_MODE_AFR(0U) |                            \
	 GPIO_MODE_IHYSTR_CMOS |                        \
	 GPIO_MODE_TRIGENR_IN_DISABLED |                \
	 GPIO_MODE_SAFESELR_DISABLED |                  \
	 GPIO_MODE_SAFEVALR_LOW)


/*--------------------- TIM1 pins definitions ---------------------*/
#define LIN1 gpio_iopack(GPIO_PORT_C, GPIO_PIN_14) /* PC[14] */
#define LIN1_CFG                                    \
	(GPIO_MODE_MODER_ALTERNATE |                    \
	 GPIO_MODE_OTYPER_PUSHPULL |                    \
	 GPIO_MODE_OSPEEDR_HIGH |                       \
	 GPIO_MODE_PUPDR_FLOATING |                     \
	 GPIO_MODE_AFR(4U) |                            \
	 GPIO_MODE_IHYSTR_CMOS |                        \
	 GPIO_MODE_TRIGENR_IN_DISABLED |                \
	 GPIO_MODE_SAFESELR_DISABLED |                  \
	 GPIO_MODE_SAFEVALR_LOW)

#define HIN1 gpio_iopack(GPIO_PORT_C, GPIO_PIN_15) /* PC[15] */
#define HIN1_CFG                                    \
	(GPIO_MODE_MODER_ALTERNATE |                    \
	 GPIO_MODE_OTYPER_PUSHPULL |                    \
	 GPIO_MODE_OSPEEDR_HIGH |                       \
	 GPIO_MODE_PUPDR_FLOATING |                     \
	 GPIO_MODE_AFR(4U) |                            \
	 GPIO_MODE_IHYSTR_CMOS |                        \
	 GPIO_MODE_TRIGENR_IN_DISABLED |                \
	 GPIO_MODE_SAFESELR_DISABLED |                  \
	 GPIO_MODE_SAFEVALR_LOW)

#define LIN2 gpio_iopack(GPIO_PORT_D, GPIO_PIN_0) /* PD[0] */
#define LIN2_CFG                                    \
	(GPIO_MODE_MODER_ALTERNATE |                    \
	 GPIO_MODE_OTYPER_PUSHPULL |                    \
	 GPIO_MODE_OSPEEDR_HIGH |                       \
	 GPIO_MODE_PUPDR_FLOATING |                     \
	 GPIO_MODE_AFR(4U) |                            \
	 GPIO_MODE_IHYSTR_CMOS |                        \
	 GPIO_MODE_TRIGENR_IN_DISABLED |                \
	 GPIO_MODE_SAFESELR_DISABLED |                  \
	 GPIO_MODE_SAFEVALR_LOW)

#define LIN3 gpio_iopack(GPIO_PORT_D, GPIO_PIN_3) /* PD[3] */
#define LIN3_CFG                                    \
	(GPIO_MODE_MODER_ALTERNATE |                    \
	 GPIO_MODE_OTYPER_PUSHPULL |                    \
	 GPIO_MODE_OSPEEDR_HIGH |                       \
	 GPIO_MODE_PUPDR_FLOATING |                     \
	 GPIO_MODE_AFR(4U) |                            \
	 GPIO_MODE_IHYSTR_CMOS |                        \
	 GPIO_MODE_TRIGENR_IN_DISABLED |                \
	 GPIO_MODE_SAFESELR_DISABLED |                  \
	 GPIO_MODE_SAFEVALR_LOW)

#define HIN3 gpio_iopack(GPIO_PORT_D, GPIO_PIN_4) /* PD[4] */
#define HIN3_CFG                                    \
	(GPIO_MODE_MODER_ALTERNATE |                    \
	 GPIO_MODE_OTYPER_PUSHPULL |                    \
	 GPIO_MODE_OSPEEDR_HIGH |                       \
	 GPIO_MODE_PUPDR_FLOATING |                     \
	 GPIO_MODE_AFR(4U) |                            \
	 GPIO_MODE_IHYSTR_CMOS |                        \
	 GPIO_MODE_TRIGENR_IN_DISABLED |                \
	 GPIO_MODE_SAFESELR_DISABLED |                  \
	 GPIO_MODE_SAFEVALR_LOW)

#define HIN2 gpio_iopack(GPIO_PORT_D, GPIO_PIN_1) /* PD[1] */
#define HIN2_CFG                                    \
	(GPIO_MODE_MODER_ALTERNATE |                    \
	 GPIO_MODE_OTYPER_PUSHPULL |                    \
	 GPIO_MODE_OSPEEDR_LOW |                        \
	 GPIO_MODE_PUPDR_FLOATING |                     \
	 GPIO_MODE_AFR(4U) |                            \
	 GPIO_MODE_IHYSTR_CMOS |                        \
	 GPIO_MODE_TRIGENR_IN_DISABLED |                \
	 GPIO_MODE_SAFESELR_DISABLED |                  \
	 GPIO_MODE_SAFEVALR_LOW)


/*--------------------- TIM2 pins definitions ---------------------*/
#define MC_HALL_CH2 gpio_iopack(GPIO_PORT_H, GPIO_PIN_3) /* PH[3] */
#define MC_HALL_CH2_CFG                             \
	(GPIO_MODE_MODER_ALTERNATE |                    \
	 GPIO_MODE_OTYPER_PUSHPULL |                    \
	 GPIO_MODE_OSPEEDR_LOW |                        \
	 GPIO_MODE_PUPDR_FLOATING |                     \
	 GPIO_MODE_AFR(1U) |                            \
	 GPIO_MODE_IHYSTR_CMOS |                        \
	 GPIO_MODE_TRIGENR_IN_ENABLED |                 \
	 GPIO_MODE_SAFESELR_DISABLED |                  \
	 GPIO_MODE_SAFEVALR_LOW)

#define MC_HALL_CH3 gpio_iopack(GPIO_PORT_H, GPIO_PIN_4) /* PH[4] */
#define MC_HALL_CH3_CFG                             \
	(GPIO_MODE_MODER_ALTERNATE |                    \
	 GPIO_MODE_OTYPER_PUSHPULL |                    \
	 GPIO_MODE_OSPEEDR_LOW |                        \
	 GPIO_MODE_PUPDR_FLOATING |                     \
	 GPIO_MODE_AFR(1U) |                            \
	 GPIO_MODE_IHYSTR_CMOS |                        \
	 GPIO_MODE_TRIGENR_IN_ENABLED |                 \
	 GPIO_MODE_SAFESELR_DISABLED |                  \
	 GPIO_MODE_SAFEVALR_LOW)

#define MC_HALL_CH1 gpio_iopack(GPIO_PORT_H, GPIO_PIN_2) /* PH[2] */
#define MC_HALL_CH1_CFG                             \
	(GPIO_MODE_MODER_ALTERNATE |                    \
	 GPIO_MODE_OTYPER_PUSHPULL |                    \
	 GPIO_MODE_OSPEEDR_LOW |                        \
	 GPIO_MODE_PUPDR_FLOATING |                     \
	 GPIO_MODE_AFR(1U) |                            \
	 GPIO_MODE_IHYSTR_CMOS |                        \
	 GPIO_MODE_TRIGENR_IN_ENABLED |                 \
	 GPIO_MODE_SAFESELR_DISABLED |                  \
	 GPIO_MODE_SAFEVALR_LOW)


/*--------------------- GPIO pins definitions ---------------------*/
#define STDRIVE_EN gpio_iopack(GPIO_PORT_B, GPIO_PIN_1) /* PB[1] */
#define STDRIVE_EN_CFG                              \
	(GPIO_MODE_MODER_OUTPUT |                       \
	 GPIO_MODE_OTYPER_PUSHPULL |                    \
	 GPIO_MODE_OSPEEDR_LOW |                        \
	 GPIO_MODE_PUPDR_FLOATING |                     \
	 GPIO_MODE_AFR(0U) |                            \
	 GPIO_MODE_IHYSTR_CMOS |                        \
	 GPIO_MODE_TRIGENR_IN_DISABLED |                \
	 GPIO_MODE_SAFESELR_DISABLED |                  \
	 GPIO_MODE_SAFEVALR_LOW)

#define STDRIVE_FAULT gpio_iopack(GPIO_PORT_D, GPIO_PIN_2) /* PD[2] */
#define STDRIVE_FAULT_CFG                           \
	(GPIO_MODE_MODER_INPUT |                        \
	 GPIO_MODE_OTYPER_PUSHPULL |                    \
	 GPIO_MODE_OSPEEDR_LOW |                        \
	 GPIO_MODE_PUPDR_FLOATING |                     \
	 GPIO_MODE_AFR(0U) |                            \
	 GPIO_MODE_IHYSTR_CMOS |                        \
	 GPIO_MODE_TRIGENR_IN_DISABLED |                \
	 GPIO_MODE_SAFESELR_DISABLED |                  \
	 GPIO_MODE_SAFEVALR_LOW)

#define S1 gpio_iopack(GPIO_PORT_A, GPIO_PIN_13) /* PA[13] */
#define S1_CFG                                      \
	(GPIO_MODE_MODER_INPUT |                        \
	 GPIO_MODE_OTYPER_PUSHPULL |                    \
	 GPIO_MODE_OSPEEDR_LOW |                        \
	 GPIO_MODE_PUPDR_FLOATING |                     \
	 GPIO_MODE_AFR(0U) |                            \
	 GPIO_MODE_IHYSTR_CMOS |                        \
	 GPIO_MODE_TRIGENR_IN_DISABLED |                \
	 GPIO_MODE_SAFESELR_DISABLED |                  \
	 GPIO_MODE_SAFEVALR_LOW)

#define S2 gpio_iopack(GPIO_PORT_A, GPIO_PIN_14) /* PA[14] */
#define S2_CFG                                      \
	(GPIO_MODE_MODER_INPUT |                        \
	 GPIO_MODE_OTYPER_PUSHPULL |                    \
	 GPIO_MODE_OSPEEDR_LOW |                        \
	 GPIO_MODE_PUPDR_FLOATING |                     \
	 GPIO_MODE_AFR(0U) |                            \
	 GPIO_MODE_IHYSTR_CMOS |                        \
	 GPIO_MODE_TRIGENR_IN_DISABLED |                \
	 GPIO_MODE_SAFESELR_DISABLED |                  \
	 GPIO_MODE_SAFEVALR_LOW)

#define S3 gpio_iopack(GPIO_PORT_A, GPIO_PIN_15) /* PA[15] */
#define S3_CFG                                      \
	(GPIO_MODE_MODER_INPUT |                        \
	 GPIO_MODE_OTYPER_PUSHPULL |                    \
	 GPIO_MODE_OSPEEDR_LOW |                        \
	 GPIO_MODE_PUPDR_FLOATING |                     \
	 GPIO_MODE_AFR(0U) |                            \
	 GPIO_MODE_IHYSTR_CMOS |                        \
	 GPIO_MODE_TRIGENR_IN_DISABLED |                \
	 GPIO_MODE_SAFESELR_DISABLED |                  \
	 GPIO_MODE_SAFEVALR_LOW)

#define USER_LED_3 gpio_iopack(GPIO_PORT_E, GPIO_PIN_1) /* PE[1] */
#define USER_LED_3_CFG                              \
	(GPIO_MODE_MODER_OUTPUT |                       \
	 GPIO_MODE_OTYPER_PUSHPULL |                    \
	 GPIO_MODE_OSPEEDR_LOW |                        \
	 GPIO_MODE_PUPDR_FLOATING |                     \
	 GPIO_MODE_AFR(0U) |                            \
	 GPIO_MODE_IHYSTR_CMOS |                        \
	 GPIO_MODE_TRIGENR_IN_DISABLED |                \
	 GPIO_MODE_SAFESELR_DISABLED |                  \
	 GPIO_MODE_SAFEVALR_LOW)

#define USER_LED_2 gpio_iopack(GPIO_PORT_E, GPIO_PIN_2) /* PE[2] */
#define USER_LED_2_CFG                              \
	(GPIO_MODE_MODER_OUTPUT |                       \
	 GPIO_MODE_OTYPER_PUSHPULL |                    \
	 GPIO_MODE_OSPEEDR_LOW |                        \
	 GPIO_MODE_PUPDR_FLOATING |                     \
	 GPIO_MODE_AFR(0U) |                            \
	 GPIO_MODE_IHYSTR_CMOS |                        \
	 GPIO_MODE_TRIGENR_IN_DISABLED |                \
	 GPIO_MODE_SAFESELR_DISABLED |                  \
	 GPIO_MODE_SAFEVALR_LOW)

#define USER_LED_1 gpio_iopack(GPIO_PORT_E, GPIO_PIN_3) /* PE[3] */
#define USER_LED_1_CFG                              \
	(GPIO_MODE_MODER_OUTPUT |                       \
	 GPIO_MODE_OTYPER_PUSHPULL |                    \
	 GPIO_MODE_OSPEEDR_LOW |                        \
	 GPIO_MODE_PUPDR_FLOATING |                     \
	 GPIO_MODE_AFR(0U) |                            \
	 GPIO_MODE_IHYSTR_CMOS |                        \
	 GPIO_MODE_TRIGENR_IN_DISABLED |                \
	 GPIO_MODE_SAFESELR_DISABLED |                  \
	 GPIO_MODE_SAFEVALR_LOW)


/*--------------------- UART1 pins definitions ---------------------*/
#define UART1_TX gpio_iopack(GPIO_PORT_A, GPIO_PIN_1) /* PA[1] */
#define UART1_TX_CFG                                \
	(GPIO_MODE_MODER_ALTERNATE |                    \
	 GPIO_MODE_OTYPER_PUSHPULL |                    \
	 GPIO_MODE_OSPEEDR_LOW |                        \
	 GPIO_MODE_PUPDR_FLOATING |                     \
	 GPIO_MODE_AFR(7U) |                            \
	 GPIO_MODE_IHYSTR_CMOS |                        \
	 GPIO_MODE_TRIGENR_IN_DISABLED |                \
	 GPIO_MODE_SAFESELR_DISABLED |                  \
	 GPIO_MODE_SAFEVALR_LOW)

#define UART1_RX gpio_iopack(GPIO_PORT_A, GPIO_PIN_0) /* PA[0] */
#define UART1_RX_CFG                                \
	(GPIO_MODE_MODER_ALTERNATE |                    \
	 GPIO_MODE_OTYPER_PUSHPULL |                    \
	 GPIO_MODE_OSPEEDR_LOW |                        \
	 GPIO_MODE_PUPDR_FLOATING |                     \
	 GPIO_MODE_AFR(7U) |                            \
	 GPIO_MODE_IHYSTR_CMOS |                        \
	 GPIO_MODE_TRIGENR_IN_ENABLED |                 \
	 GPIO_MODE_SAFESELR_DISABLED |                  \
	 GPIO_MODE_SAFEVALR_LOW)


/*--------------------- CAN2 pins definitions ---------------------*/
#define CAN2_TX gpio_iopack(GPIO_PORT_A, GPIO_PIN_11) /* PA[11] */
#define CAN2_TX_CFG                                 \
	(GPIO_MODE_MODER_ALTERNATE |                    \
	 GPIO_MODE_OTYPER_PUSHPULL |                    \
	 GPIO_MODE_OSPEEDR_LOW |                        \
	 GPIO_MODE_PUPDR_FLOATING |                     \
	 GPIO_MODE_AFR(6U) |                            \
	 GPIO_MODE_IHYSTR_CMOS |                        \
	 GPIO_MODE_TRIGENR_IN_DISABLED |                \
	 GPIO_MODE_SAFESELR_DISABLED |                  \
	 GPIO_MODE_SAFEVALR_LOW)

#define CAN2_RX gpio_iopack(GPIO_PORT_A, GPIO_PIN_12) /* PA[12] */
#define CAN2_RX_CFG                                 \
	(GPIO_MODE_MODER_ALTERNATE |                    \
	 GPIO_MODE_OTYPER_PUSHPULL |                    \
	 GPIO_MODE_OSPEEDR_LOW |                        \
	 GPIO_MODE_PUPDR_FLOATING |                     \
	 GPIO_MODE_AFR(6U) |                            \
	 GPIO_MODE_IHYSTR_CMOS |                        \
	 GPIO_MODE_TRIGENR_IN_ENABLED |                 \
	 GPIO_MODE_SAFESELR_DISABLED |                  \
	 GPIO_MODE_SAFEVALR_LOW)


/*--------------------- TIM4 pins definitions ---------------------*/
#define RBK_HIN gpio_iopack(GPIO_PORT_F, GPIO_PIN_2) /* PF[2] */
#define RBK_HIN_CFG                                 \
	(GPIO_MODE_MODER_ALTERNATE |                    \
	 GPIO_MODE_OTYPER_PUSHPULL |                    \
	 GPIO_MODE_OSPEEDR_LOW |                        \
	 GPIO_MODE_PUPDR_FLOATING |                     \
	 GPIO_MODE_AFR(2U) |                            \
	 GPIO_MODE_IHYSTR_CMOS |                        \
	 GPIO_MODE_TRIGENR_IN_DISABLED |                \
	 GPIO_MODE_SAFESELR_DISABLED |                  \
	 GPIO_MODE_SAFEVALR_LOW)

#define RBK_LIN gpio_iopack(GPIO_PORT_F, GPIO_PIN_3) /* PF[3] */
#define RBK_LIN_CFG                                 \
	(GPIO_MODE_MODER_ALTERNATE |                    \
	 GPIO_MODE_OTYPER_PUSHPULL |                    \
	 GPIO_MODE_OSPEEDR_LOW |                        \
	 GPIO_MODE_PUPDR_FLOATING |                     \
	 GPIO_MODE_AFR(2U) |                            \
	 GPIO_MODE_IHYSTR_CMOS |                        \
	 GPIO_MODE_TRIGENR_IN_DISABLED |                \
	 GPIO_MODE_SAFESELR_DISABLED |                  \
	 GPIO_MODE_SAFEVALR_LOW)


/*--------------------- CAN3 pins definitions ---------------------*/
#define CAN3_TX gpio_iopack(GPIO_PORT_H, GPIO_PIN_10) /* PH[10] */
#define CAN3_TX_CFG                                 \
	(GPIO_MODE_MODER_ALTERNATE |                    \
	 GPIO_MODE_OTYPER_PUSHPULL |                    \
	 GPIO_MODE_OSPEEDR_LOW |                        \
	 GPIO_MODE_PUPDR_FLOATING |                     \
	 GPIO_MODE_AFR(6U) |                            \
	 GPIO_MODE_IHYSTR_CMOS |                        \
	 GPIO_MODE_TRIGENR_IN_DISABLED |                \
	 GPIO_MODE_SAFESELR_DISABLED |                  \
	 GPIO_MODE_SAFEVALR_LOW)

#define CAN3_RX gpio_iopack(GPIO_PORT_H, GPIO_PIN_11) /* PH[11] */
#define CAN3_RX_CFG                                 \
	(GPIO_MODE_MODER_ALTERNATE |                    \
	 GPIO_MODE_OTYPER_PUSHPULL |                    \
	 GPIO_MODE_OSPEEDR_LOW |                        \
	 GPIO_MODE_PUPDR_FLOATING |                     \
	 GPIO_MODE_AFR(6U) |                            \
	 GPIO_MODE_IHYSTR_CMOS |                        \
	 GPIO_MODE_TRIGENR_IN_ENABLED |                 \
	 GPIO_MODE_SAFESELR_DISABLED |                  \
	 GPIO_MODE_SAFEVALR_LOW)


/*--------------------- CAN4 pins definitions ---------------------*/
#define CAN4_TX gpio_iopack(GPIO_PORT_H, GPIO_PIN_12) /* PH[12] */
#define CAN4_TX_CFG                                 \
	(GPIO_MODE_MODER_ALTERNATE |                    \
	 GPIO_MODE_OTYPER_PUSHPULL |                    \
	 GPIO_MODE_OSPEEDR_LOW |                        \
	 GPIO_MODE_PUPDR_FLOATING |                     \
	 GPIO_MODE_AFR(6U) |                            \
	 GPIO_MODE_IHYSTR_CMOS |                        \
	 GPIO_MODE_TRIGENR_IN_DISABLED |                \
	 GPIO_MODE_SAFESELR_DISABLED |                  \
	 GPIO_MODE_SAFEVALR_LOW)

#define CAN4_RX gpio_iopack(GPIO_PORT_H, GPIO_PIN_13) /* PH[13] */
#define CAN4_RX_CFG                                 \
	(GPIO_MODE_MODER_ALTERNATE |                    \
	 GPIO_MODE_OTYPER_PUSHPULL |                    \
	 GPIO_MODE_OSPEEDR_LOW |                        \
	 GPIO_MODE_PUPDR_FLOATING |                     \
	 GPIO_MODE_AFR(6U) |                            \
	 GPIO_MODE_IHYSTR_CMOS |                        \
	 GPIO_MODE_TRIGENR_IN_ENABLED |                 \
	 GPIO_MODE_SAFESELR_DISABLED |                  \
	 GPIO_MODE_SAFEVALR_LOW)



/*===========================================================================*/
/* Module pre-compile time settings.                                         */
/*===========================================================================*/

/*===========================================================================*/
/* Derived constants and error checks.                                       */
/*===========================================================================*/

/*===========================================================================*/
/* Module data structures and types.                                         */
/*===========================================================================*/

/*===========================================================================*/
/* Module macros.                                                            */
/*===========================================================================*/

/*===========================================================================*/
/* External declarations.                                                    */
/*===========================================================================*/

#if !defined(_FROM_ASM_)
#ifdef __cplusplus
extern "C" {
#endif

#ifdef __cplusplus
}
#endif
#endif /* _FROM_ASM_ */

/*===========================================================================*/
/* Module inline functions.                                                  */
/*===========================================================================*/


__STATIC_INLINE void evbe7000s_init(void) {
	
	/* CAN1 */
	gpio_set_pin_mode(CAN1_RX, CAN1_RX_CFG);
	gpio_set_pin_mode(CAN1_TX, CAN1_TX_CFG);

	/* SAR1 */
	gpio_set_pin_mode(CIW, CIW_CFG);

	/* SAR2 */
	gpio_set_pin_mode(CIV, CIV_CFG);

	/* SAR3 */
	gpio_set_pin_mode(CIU, CIU_CFG);
	gpio_set_pin_mode(PIN_SAR3_IN2, PIN_SAR3_IN2_CFG);

	/* SAR4 */
	gpio_set_pin_mode(TEMP, TEMP_CFG);

	/* SAR5 */
	gpio_set_pin_mode(VBUS, VBUS_CFG);
	gpio_set_pin_mode(VSPEED, VSPEED_CFG);
	gpio_set_pin_mode(NTC, NTC_CFG);

	/* TIM1 */
	gpio_set_pin_mode(LIN1, LIN1_CFG);
	gpio_set_pin_mode(HIN1, HIN1_CFG);
	gpio_set_pin_mode(LIN2, LIN2_CFG);
	gpio_set_pin_mode(LIN3, LIN3_CFG);
	gpio_set_pin_mode(HIN3, HIN3_CFG);
	gpio_set_pin_mode(HIN2, HIN2_CFG);

	/* TIM2 */
	gpio_set_pin_mode(MC_HALL_CH2, MC_HALL_CH2_CFG);
	gpio_set_pin_mode(MC_HALL_CH3, MC_HALL_CH3_CFG);
	gpio_set_pin_mode(MC_HALL_CH1, MC_HALL_CH1_CFG);

	/* GPIO */
	gpio_set_pin_mode(STDRIVE_EN, STDRIVE_EN_CFG);
	gpio_set_pin_mode(STDRIVE_FAULT, STDRIVE_FAULT_CFG);
	gpio_set_pin_mode(S1, S1_CFG);
	gpio_set_pin_mode(S2, S2_CFG);
	gpio_set_pin_mode(S3, S3_CFG);
	gpio_set_pin_mode(USER_LED_3, USER_LED_3_CFG);
	gpio_set_pin_mode(USER_LED_2, USER_LED_2_CFG);
	gpio_set_pin_mode(USER_LED_1, USER_LED_1_CFG);

	/* UART1 */
	gpio_set_pin_mode(UART1_TX, UART1_TX_CFG);
	gpio_set_pin_mode(UART1_RX, UART1_RX_CFG);

	/* CAN2 */
	gpio_set_pin_mode(CAN2_TX, CAN2_TX_CFG);
	gpio_set_pin_mode(CAN2_RX, CAN2_RX_CFG);

	/* TIM4 */
	gpio_set_pin_mode(RBK_HIN, RBK_HIN_CFG);
	gpio_set_pin_mode(RBK_LIN, RBK_LIN_CFG);

	/* CAN3 */
	gpio_set_pin_mode(CAN3_TX, CAN3_TX_CFG);
	gpio_set_pin_mode(CAN3_RX, CAN3_RX_CFG);

	/* CAN4 */
	gpio_set_pin_mode(CAN4_TX, CAN4_TX_CFG);
	gpio_set_pin_mode(CAN4_RX, CAN4_RX_CFG);

}

/*===========================================================================*/
/* Module late inclusions.                                                   */
/*===========================================================================*/

#endif /* _EVBE7000S_H_ */

/** @} */

