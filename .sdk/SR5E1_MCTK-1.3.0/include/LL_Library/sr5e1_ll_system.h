/****************************************************************************
*
* Copyright (c) 2023 STMicroelectronics - All Rights Reserved
*
* License terms: STMicroelectronics Proprietary in accordance with licensing
* terms SLA0098 at www.st.com.
*
* THIS SOFTWARE IS DISTRIBUTED "AS IS," AND ALL WARRANTIES ARE DISCLAIMED,
* INCLUDING MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE.
*****************************************************************************/
/**
*   @file    sr5e1_ll_system.h
*   @version M4_VERSION
*   @brief   Header file of SYSTEM LL module
*
*   @details Header file of SYSTEM LL module
*
 */
/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __SR5E1_LL_SYSTEM_H
#define __SR5E1_LL_SYSTEM_H

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "platform.h"
#include "sr5e1_macros.h"

/** @addtogroup SR5E1_LL_Driver
  * @{
  */

#if defined (FLASH) || defined (SYSCFG) || defined (DBGMCU)

/** @defgroup SYSTEM_LL SYSTEM
  * @{
  */

/* Private types -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/

/* Private constants ---------------------------------------------------------*/
/** @defgroup SYSTEM_LL_Private_Constants SYSTEM Private Constants
  * @{
  */
#if 0 /* SR5E1_Fix: DBGMCU and FLASH macros not implemented yet */
/* Defines used for position in the register */
#define DBGMCU_REVID_POSITION         (uint32_t)POSITION_VAL(DBGMCU_IDCODE_REV_ID)

/**
  * @brief Power-down in Run mode Flash key
  */
#define FLASH_PDKEY1                  0x04152637U /*!< Flash power down key1 */
#define FLASH_PDKEY2                  0xFAFBFCFDU /*!< Flash power down key2: used with FLASH_PDKEY1 
                                                       to unlock the RUN_PD bit in FLASH_ACR */
#endif /* if 0 */

/**
  * @}
  */

/* Private macros ------------------------------------------------------------*/

/* Exported types ------------------------------------------------------------*/
/* Exported constants --------------------------------------------------------*/
/** @defgroup SYSTEM_LL_Exported_Constants SYSTEM Exported Constants
  * @{
  */

/** @defgroup SYSTEM_LL_EC_MIDR1_PACKAGE SYSCFG MIDR1 PACKAGE
  * @{
  */
#define LL_SYSCFG_MIDR1_PACKAGE_QFP100     0x00000009U               /*!< MIDR1 PACKAGE QFP100               */
#define LL_SYSCFG_MIDR1_PACKAGE_QFP176     0x00000011U               /*!< MIDR1 PACKAGE QFP176               */
/**
  * @}
  */

/** @defgroup SYSTEM_LL_EC_EXTI_PORT SYSCFG EXTI PORT
  * @{
  */
#define LL_SYSCFG_EXTI_PORTA               0U                        /*!< EXTI PORT A                        */
#define LL_SYSCFG_EXTI_PORTB               1U                        /*!< EXTI PORT B                        */
#define LL_SYSCFG_EXTI_PORTC               2U                        /*!< EXTI PORT C                        */
#define LL_SYSCFG_EXTI_PORTD               3U                        /*!< EXTI PORT D                        */
#define LL_SYSCFG_EXTI_PORTE               4U                        /*!< EXTI PORT E                        */
#define LL_SYSCFG_EXTI_PORTF               5U                        /*!< EXTI PORT F                        */
#define LL_SYSCFG_EXTI_PORTG               6U                        /*!< EXTI PORT G                        */
#define LL_SYSCFG_EXTI_PORTH               7U                        /*!< EXTI PORT H                        */
#define LL_SYSCFG_EXTI_PORTI               8U                        /*!< EXTI PORT I                        */
/**
  * @}
  */

/** @defgroup SYSTEM_LL_EC_EXTI_LINE SYSCFG EXTI LINE
  * @{
  */
#define LL_SYSCFG_EXTI_LINE0               (uint32_t)((0x000FU << 16U) | 0U)  /* !< EXTI_POSITION_0  | EXTICR[0] */
#define LL_SYSCFG_EXTI_LINE1               (uint32_t)((0x00F0U << 16U) | 0U)  /* !< EXTI_POSITION_4  | EXTICR[0] */
#define LL_SYSCFG_EXTI_LINE2               (uint32_t)((0x0F00U << 16U) | 0U)  /* !< EXTI_POSITION_8  | EXTICR[0] */
#define LL_SYSCFG_EXTI_LINE3               (uint32_t)((0xF000U << 16U) | 0U)  /* !< EXTI_POSITION_12 | EXTICR[0] */
#define LL_SYSCFG_EXTI_LINE4               (uint32_t)((0x000FU << 16U) | 1U)  /* !< EXTI_POSITION_0  | EXTICR[1] */
#define LL_SYSCFG_EXTI_LINE5               (uint32_t)((0x00F0U << 16U) | 1U)  /* !< EXTI_POSITION_4  | EXTICR[1] */
#define LL_SYSCFG_EXTI_LINE6               (uint32_t)((0x0F00U << 16U) | 1U)  /* !< EXTI_POSITION_8  | EXTICR[1] */
#define LL_SYSCFG_EXTI_LINE7               (uint32_t)((0xF000U << 16U) | 1U)  /* !< EXTI_POSITION_12 | EXTICR[1] */
#define LL_SYSCFG_EXTI_LINE8               (uint32_t)((0x000FU << 16U) | 2U)  /* !< EXTI_POSITION_0  | EXTICR[2] */
#define LL_SYSCFG_EXTI_LINE9               (uint32_t)((0x00F0U << 16U) | 2U)  /* !< EXTI_POSITION_4  | EXTICR[2] */
#define LL_SYSCFG_EXTI_LINE10              (uint32_t)((0x0F00U << 16U) | 2U)  /* !< EXTI_POSITION_8  | EXTICR[2] */
#define LL_SYSCFG_EXTI_LINE11              (uint32_t)((0xF000U << 16U) | 2U)  /* !< EXTI_POSITION_12 | EXTICR[2] */
#define LL_SYSCFG_EXTI_LINE12              (uint32_t)((0x000FU << 16U) | 3U)  /* !< EXTI_POSITION_0  | EXTICR[3] */
#define LL_SYSCFG_EXTI_LINE13              (uint32_t)((0x00F0U << 16U) | 3U)  /* !< EXTI_POSITION_4  | EXTICR[3] */
#define LL_SYSCFG_EXTI_LINE14              (uint32_t)((0x0F00U << 16U) | 3U)  /* !< EXTI_POSITION_8  | EXTICR[3] */
#define LL_SYSCFG_EXTI_LINE15              (uint32_t)((0xF000U << 16U) | 3U)  /* !< EXTI_POSITION_12 | EXTICR[3] */
/**
  * @}
  */

/** @defgroup SYSTEM_LL_EC_SDADC1_EXT_GATE SYSCFG SDADC1_EXT_GATE LINE
  * @{
  */
#define LL_SYSCFG_SDADC1_EXT_GATE_TIM1_OC1      0x00000000U                                                                                                                                               /*!< Line 0 selection: TIM1_OC1 is selected */
#define LL_SYSCFG_SDADC1_EXT_GATE_TIM1_OC5      (SYSCFG_SDADCX_EXT_GATE_SEL_SDADC1_EXT_GATE_SEL_0)                                                                                                        /*!< Line 1 selection: TIM1_OC5 is selected */
#define LL_SYSCFG_SDADC1_EXT_GATE_TIM8_OC1      (SYSCFG_SDADCX_EXT_GATE_SEL_SDADC1_EXT_GATE_SEL_1)                                                                                                        /*!< Line 2 selection: TIM8_OC1 is selected */
#define LL_SYSCFG_SDADC1_EXT_GATE_TIM8_OC5      (SYSCFG_SDADCX_EXT_GATE_SEL_SDADC1_EXT_GATE_SEL_1 | SYSCFG_SDADCX_EXT_GATE_SEL_SDADC1_EXT_GATE_SEL_0)                                                     /*!< Line 3 selection: TIM8_OC5 is selected */
#define LL_SYSCFG_SDADC1_EXT_GATE_TIM2_OC4      (SYSCFG_SDADCX_EXT_GATE_SEL_SDADC1_EXT_GATE_SEL_2)                                                                                                        /*!< Line 4 selection: TIM2_OC4 is selected */
#define LL_SYSCFG_SDADC1_EXT_GATE_TIM3_OC4      (SYSCFG_SDADCX_EXT_GATE_SEL_SDADC1_EXT_GATE_SEL_2 | SYSCFG_SDADCX_EXT_GATE_SEL_SDADC1_EXT_GATE_SEL_0)                                                     /*!< Line 5 selection: TIM3_OC4 is selected */
#define LL_SYSCFG_SDADC1_EXT_GATE_TIM4_OC4      (SYSCFG_SDADCX_EXT_GATE_SEL_SDADC1_EXT_GATE_SEL_2 | SYSCFG_SDADCX_EXT_GATE_SEL_SDADC1_EXT_GATE_SEL_1)                                                     /*!< Line 6 selection: TIM4_OC4 is selected */
#define LL_SYSCFG_SDADC1_EXT_GATE_TIM5_OC4      (SYSCFG_SDADCX_EXT_GATE_SEL_SDADC1_EXT_GATE_SEL_2 | SYSCFG_SDADCX_EXT_GATE_SEL_SDADC1_EXT_GATE_SEL_1 | SYSCFG_SDADCX_EXT_GATE_SEL_SDADC1_EXT_GATE_SEL_0)  /*!< Line 7 selection: TIM5_OC4 is selected */
#define LL_SYSCFG_SDADC1_EXT_GATE_TIM15_OC2     (SYSCFG_SDADCX_EXT_GATE_SEL_SDADC1_EXT_GATE_SEL_3)                                                                                                        /*!< Line 8 selection: TIM15_OC2 is selected */
#define LL_SYSCFG_SDADC1_EXT_GATE_HRTIMER1_TE   (SYSCFG_SDADCX_EXT_GATE_SEL_SDADC1_EXT_GATE_SEL_3 | SYSCFG_SDADCX_EXT_GATE_SEL_SDADC1_EXT_GATE_SEL_0)                                                     /*!< Line 9 selection: TE of HRTIMER1 is selected */
#define LL_SYSCFG_SDADC1_EXT_GATE_HRTIMER1_TF   (SYSCFG_SDADCX_EXT_GATE_SEL_SDADC1_EXT_GATE_SEL_3 | SYSCFG_SDADCX_EXT_GATE_SEL_SDADC1_EXT_GATE_SEL_1)                                                     /*!< Line 10 selection: TF of HRTIMER1 is selected */
#define LL_SYSCFG_SDADC1_EXT_GATE_HRTIMER2_TE   (SYSCFG_SDADCX_EXT_GATE_SEL_SDADC1_EXT_GATE_SEL_3 | SYSCFG_SDADCX_EXT_GATE_SEL_SDADC1_EXT_GATE_SEL_1 | SYSCFG_SDADCX_EXT_GATE_SEL_SDADC1_EXT_GATE_SEL_0)  /*!< Line 11 selection: TE of HRTIMER2 is selected */
#define LL_SYSCFG_SDADC1_EXT_GATE_HRTIMER2_TF   (SYSCFG_SDADCX_EXT_GATE_SEL_SDADC1_EXT_GATE_SEL_3 | SYSCFG_SDADCX_EXT_GATE_SEL_SDADC1_EXT_GATE_SEL_2)                                                     /*!< Line 12 selection: TF of HRTIMER2 is selected */
/**
  * @}
  */

/** @defgroup SYSTEM_LL_EC_SDADC2_EXT_GATE SYSCFG SDADC2_EXT_GATE LINE
  * @{
  */
#define LL_SYSCFG_SDADC2_EXT_GATE_TIM1_OC1      0x00000000U                                                                                                                                               /*!< Line 0 selection: TIM1_OC1 is selected */
#define LL_SYSCFG_SDADC2_EXT_GATE_TIM1_OC5      (SYSCFG_SDADCX_EXT_GATE_SEL_SDADC2_EXT_GATE_SEL_0)                                                                                                        /*!< Line 1 selection: TIM1_OC5 is selected */
#define LL_SYSCFG_SDADC2_EXT_GATE_TIM8_OC1      (SYSCFG_SDADCX_EXT_GATE_SEL_SDADC2_EXT_GATE_SEL_1)                                                                                                        /*!< Line 2 selection: TIM8_OC1 is selected */
#define LL_SYSCFG_SDADC2_EXT_GATE_TIM8_OC5      (SYSCFG_SDADCX_EXT_GATE_SEL_SDADC2_EXT_GATE_SEL_1 | SYSCFG_SDADCX_EXT_GATE_SEL_SDADC2_EXT_GATE_SEL_0)                                                     /*!< Line 3 selection: TIM8_OC5 is selected */
#define LL_SYSCFG_SDADC2_EXT_GATE_TIM2_OC4      (SYSCFG_SDADCX_EXT_GATE_SEL_SDADC2_EXT_GATE_SEL_2)                                                                                                        /*!< Line 4 selection: TIM2_OC4 is selected */
#define LL_SYSCFG_SDADC2_EXT_GATE_TIM3_OC4      (SYSCFG_SDADCX_EXT_GATE_SEL_SDADC2_EXT_GATE_SEL_2 | SYSCFG_SDADCX_EXT_GATE_SEL_SDADC2_EXT_GATE_SEL_0)                                                     /*!< Line 5 selection: TIM3_OC4 is selected */
#define LL_SYSCFG_SDADC2_EXT_GATE_TIM4_OC4      (SYSCFG_SDADCX_EXT_GATE_SEL_SDADC2_EXT_GATE_SEL_2 | SYSCFG_SDADCX_EXT_GATE_SEL_SDADC2_EXT_GATE_SEL_1)                                                     /*!< Line 6 selection: TIM4_OC4 is selected */
#define LL_SYSCFG_SDADC2_EXT_GATE_TIM5_OC4      (SYSCFG_SDADCX_EXT_GATE_SEL_SDADC2_EXT_GATE_SEL_2 | SYSCFG_SDADCX_EXT_GATE_SEL_SDADC2_EXT_GATE_SEL_1 | SYSCFG_SDADCX_EXT_GATE_SEL_SDADC2_EXT_GATE_SEL_0)  /*!< Line 7 selection: TIM5_OC4 is selected */
#define LL_SYSCFG_SDADC2_EXT_GATE_TIM15_OC2     (SYSCFG_SDADCX_EXT_GATE_SEL_SDADC2_EXT_GATE_SEL_3)                                                                                                        /*!< Line 8 selection: TIM15_OC2 is selected */
#define LL_SYSCFG_SDADC2_EXT_GATE_HRTIMER1_TE   (SYSCFG_SDADCX_EXT_GATE_SEL_SDADC2_EXT_GATE_SEL_3 | SYSCFG_SDADCX_EXT_GATE_SEL_SDADC2_EXT_GATE_SEL_0)                                                     /*!< Line 9 selection: TE of HRTIMER1 is selected */
#define LL_SYSCFG_SDADC2_EXT_GATE_HRTIMER1_TF   (SYSCFG_SDADCX_EXT_GATE_SEL_SDADC2_EXT_GATE_SEL_3 | SYSCFG_SDADCX_EXT_GATE_SEL_SDADC2_EXT_GATE_SEL_1)                                                     /*!< Line 10 selection: TF of HRTIMER1 is selected */
#define LL_SYSCFG_SDADC2_EXT_GATE_HRTIMER2_TE   (SYSCFG_SDADCX_EXT_GATE_SEL_SDADC2_EXT_GATE_SEL_3 | SYSCFG_SDADCX_EXT_GATE_SEL_SDADC2_EXT_GATE_SEL_1 | SYSCFG_SDADCX_EXT_GATE_SEL_SDADC2_EXT_GATE_SEL_0)  /*!< Line 11 selection: TE of HRTIMER2 is selected */
#define LL_SYSCFG_SDADC2_EXT_GATE_HRTIMER2_TF   (SYSCFG_SDADCX_EXT_GATE_SEL_SDADC2_EXT_GATE_SEL_3 | SYSCFG_SDADCX_EXT_GATE_SEL_SDADC2_EXT_GATE_SEL_2)                                                     /*!< Line 12 selection: TF of HRTIMER2 is selected */
/**
  * @}
  */

/** @defgroup SYSTEM_LL_EC_FCCU_FILTER_WIDTH SYSCFG FCCU_FILTER WIDTH
  * @{
  */
#define LL_SYSCFG_FCCU_FILTER_WIDTH_50US   0x00000000U                                                               /*!< Filters glitches up to 50 us */
#define LL_SYSCFG_FCCU_FILTER_WIDTH_75US   (SYSCFG_FCCU_FILTER1_FILTER_WIDTH_0)                                      /*!< Filters glitches up to 75 us */
#define LL_SYSCFG_FCCU_FILTER_WIDTH_100US  (SYSCFG_FCCU_FILTER1_FILTER_WIDTH_1)                                      /*!< Filters glitches up to 100 us for 16 MHz IRC clock */
#define LL_SYSCFG_FCCU_FILTER_WIDTH_125US  (SYSCFG_FCCU_FILTER1_FILTER_WIDTH_1 | SYSCFG_FCCU_FILTER1_FILTER_WIDTH_0) /*!< Filters glitches up to 100 us for 16 MHz IRC clock */ //SR5E1_Fix: To be checked
/**
  * @}
  */

/** @defgroup SYSTEM_LL_EC_COMP_TESTVREF SYSCFG COMP_TESTVREF VREF
  * @{
  */
#define LL_SYSCFG_COMP_TESTVREF_VREL             0x00000000U                                                                         /*!< VREL */
#define LL_SYSCFG_COMP_TESTVREF_1_3_VREH_VREFL   (SYSCFG_ST_VREF_SEL1_COMP_TESTVREF_SEL_0)                                           /*!< 1/3 * (VREH-VREFL) */
#define LL_SYSCFG_COMP_TESTVREF_2_3_VREH_VREFL   (SYSCFG_ST_VREF_SEL1_COMP_TESTVREF_SEL_1)                                           /*!< 2/3 * (VREH-VREFL) */
#define LL_SYSCFG_COMP_TESTVREF_VREH             (SYSCFG_ST_VREF_SEL1_COMP_TESTVREF_SEL_1 | SYSCFG_ST_VREF_SEL1_COMP_TESTVREF_SEL_0) /*!< VREH */
/**
  * @}
  */

/** @defgroup SYSTEM_LL_EC_COMP_TESTVREF SYSCFG COMP_TESTVREF VREF
  * @{
  */
#define LL_SYSCFG_SAR_TESTVREF_VREL              0x00000000U                                                                         /*!< VREL */
#define LL_SYSCFG_SAR_TESTVREF_1_3_VREH_VREFL    (SYSCFG_ST_VREF_SEL1_SAR_TESTVREF_SEL_0)                                            /*!< 1/3 * (VREH-VREFL) */
#define LL_SYSCFG_SAR_TESTVREF_2_3_VREH_VREFL    (SYSCFG_ST_VREF_SEL1_SAR_TESTVREF_SEL_1)                                            /*!< 2/3 * (VREH-VREFL) */
#define LL_SYSCFG_SAR_TESTVREF_VREH              (SYSCFG_ST_VREF_SEL1_SAR_TESTVREF_SEL_1 | SYSCFG_ST_VREF_SEL1_SAR_TESTVREF_SEL_0)   /*!< VREH */
/**
  * @}
  */

#if 0 /* SR5E1_Fix: DBGMCU and FLASH macros not implemented yet */
/** @defgroup SYSTEM_LL_EC_TRACE DBGMCU TRACE Pin Assignment
  * @{
  */
#define LL_DBGMCU_TRACE_NONE               0x00000000U                                     /*!< TRACE pins not assigned (default state) */
#define LL_DBGMCU_TRACE_ASYNCH             DBGMCU_CR_TRACE_IOEN                            /*!< TRACE pin assignment for Asynchronous Mode */
#define LL_DBGMCU_TRACE_SYNCH_SIZE1        (DBGMCU_CR_TRACE_IOEN | DBGMCU_CR_TRACE_MODE_0) /*!< TRACE pin assignment for Synchronous Mode with a TRACEDATA size of 1 */
#define LL_DBGMCU_TRACE_SYNCH_SIZE2        (DBGMCU_CR_TRACE_IOEN | DBGMCU_CR_TRACE_MODE_1) /*!< TRACE pin assignment for Synchronous Mode with a TRACEDATA size of 2 */
#define LL_DBGMCU_TRACE_SYNCH_SIZE4        (DBGMCU_CR_TRACE_IOEN | DBGMCU_CR_TRACE_MODE)   /*!< TRACE pin assignment for Synchronous Mode with a TRACEDATA size of 4 */
/**
  * @}
  */

/** @defgroup SYSTEM_LL_EC_APB1_GRP1_STOP_IP DBGMCU APB1 GRP1 STOP IP
  * @{
  */
#define LL_DBGMCU_APB1_GRP1_TIM2_STOP      DBGMCU_APB1FZR1_DBG_TIM2_STOP   /*!< The counter clock of TIM2 is stopped when the core is halted*/
#if defined(TIM3)
#define LL_DBGMCU_APB1_GRP1_TIM3_STOP      DBGMCU_APB1FZR1_DBG_TIM3_STOP   /*!< The counter clock of TIM3 is stopped when the core is halted*/
#endif /* TIM3 */
#if defined(TIM4)
#define LL_DBGMCU_APB1_GRP1_TIM4_STOP      DBGMCU_APB1FZR1_DBG_TIM4_STOP   /*!< The counter clock of TIM4 is stopped when the core is halted*/
#endif /* TIM4 */
#if defined(TIM5)
#define LL_DBGMCU_APB1_GRP1_TIM5_STOP      DBGMCU_APB1FZR1_DBG_TIM5_STOP   /*!< The counter clock of TIM5 is stopped when the core is halted*/
#endif /* TIM5 */
#define LL_DBGMCU_APB1_GRP1_TIM6_STOP      DBGMCU_APB1FZR1_DBG_TIM6_STOP   /*!< The counter clock of TIM6 is stopped when the core is halted*/
#if defined(TIM7)
#define LL_DBGMCU_APB1_GRP1_TIM7_STOP      DBGMCU_APB1FZR1_DBG_TIM7_STOP   /*!< The counter clock of TIM7 is stopped when the core is halted*/
#endif /* TIM7 */
#define LL_DBGMCU_APB1_GRP1_RTC_STOP       DBGMCU_APB1FZR1_DBG_RTC_STOP    /*!< The clock of the RTC counter is stopped when the core is halted*/
#define LL_DBGMCU_APB1_GRP1_WWDG_STOP      DBGMCU_APB1FZR1_DBG_WWDG_STOP   /*!< The window watchdog counter clock is stopped when the core is halted*/
#define LL_DBGMCU_APB1_GRP1_IWDG_STOP      DBGMCU_APB1FZR1_DBG_IWDG_STOP   /*!< The independent watchdog counter clock is stopped when the core is halted*/
#define LL_DBGMCU_APB1_GRP1_I2C1_STOP      DBGMCU_APB1FZR1_DBG_I2C1_STOP   /*!< The I2C1 SMBus timeout is frozen*/
#if defined(I2C2)
#define LL_DBGMCU_APB1_GRP1_I2C2_STOP      DBGMCU_APB1FZR1_DBG_I2C2_STOP   /*!< The I2C2 SMBus timeout is frozen*/
#endif /* I2C2 */
#define LL_DBGMCU_APB1_GRP1_I2C3_STOP      DBGMCU_APB1FZR1_DBG_I2C3_STOP   /*!< The I2C3 SMBus timeout is frozen*/
#define LL_DBGMCU_APB1_GRP1_LPTIM1_STOP    DBGMCU_APB1FZR1_DBG_LPTIM1_STOP /*!< The counter clock of LPTIM1 is stopped when the core is halted*/
/**
  * @}
  */

/** @defgroup SYSTEM_LL_EC_APB1_GRP2_STOP_IP DBGMCU APB1 GRP2 STOP IP
  * @{
  */
#if defined(I2C4)
#define LL_DBGMCU_APB1_GRP2_I2C4_STOP      DBGMCU_APB1FZR2_DBG_I2C4_STOP   /*!< The I2C4 SMBus timeout is frozen*/
#endif /* I2C4 */
/**
  * @}
  */

/** @defgroup SYSTEM_LL_EC_APB2_GRP1_STOP_IP DBGMCU APB2 GRP1 STOP IP
  * @{
  */
#define LL_DBGMCU_APB2_GRP1_TIM1_STOP      DBGMCU_APB2FZ_DBG_TIM1_STOP     /*!< The counter clock of TIM1 is stopped when the core is halted*/
#if defined(TIM8)
#define LL_DBGMCU_APB2_GRP1_TIM8_STOP      DBGMCU_APB2FZ_DBG_TIM8_STOP     /*!< The counter clock of TIM8 is stopped when the core is halted*/
#endif /* TIM8 */
#define LL_DBGMCU_APB2_GRP1_TIM15_STOP     DBGMCU_APB2FZ_DBG_TIM15_STOP    /*!< The counter clock of TIM15 is stopped when the core is halted*/
#define LL_DBGMCU_APB2_GRP1_TIM16_STOP     DBGMCU_APB2FZ_DBG_TIM16_STOP    /*!< The counter clock of TIM16 is stopped when the core is halted*/
#if defined(TIM17)
#define LL_DBGMCU_APB2_GRP1_TIM17_STOP     DBGMCU_APB2FZ_DBG_TIM17_STOP    /*!< The counter clock of TIM17 is stopped when the core is halted*/
#endif /* TIM17 */
#if defined(TIM20)
#define LL_DBGMCU_APB2_GRP1_TIM20_STOP     DBGMCU_APB2FZ_DBG_TIM20_STOP    /*!< The counter clock of TIM20 is stopped when the core is halted*/
#endif /* TIM20 */
#if defined(HRTIM1)
#define LL_DBGMCU_APB2_GRP1_HRTIM1_STOP     DBGMCU_APB2FZ_DBG_HRTIM1_STOP    /*!< The counter clock of HRTIM1 is stopped when the core is halted*/
#endif /* HRTIM1 */
/**
  * @}
  */

/** @defgroup SYSTEM_LL_EC_LATENCY FLASH LATENCY
  * @{
  */
#define LL_FLASH_LATENCY_0                 FLASH_ACR_LATENCY_0WS   /*!< FLASH Zero wait state */
#define LL_FLASH_LATENCY_1                 FLASH_ACR_LATENCY_1WS   /*!< FLASH One wait state */
#define LL_FLASH_LATENCY_2                 FLASH_ACR_LATENCY_2WS   /*!< FLASH Two wait states */
#define LL_FLASH_LATENCY_3                 FLASH_ACR_LATENCY_3WS   /*!< FLASH Three wait states */
#define LL_FLASH_LATENCY_4                 FLASH_ACR_LATENCY_4WS   /*!< FLASH Four wait states */
#if defined(FLASH_ACR_LATENCY_5WS)
#define LL_FLASH_LATENCY_5                 FLASH_ACR_LATENCY_5WS   /*!< FLASH five wait state */
#define LL_FLASH_LATENCY_6                 FLASH_ACR_LATENCY_6WS   /*!< FLASH six wait state */
#define LL_FLASH_LATENCY_7                 FLASH_ACR_LATENCY_7WS   /*!< FLASH seven wait states */
#define LL_FLASH_LATENCY_8                 FLASH_ACR_LATENCY_8WS   /*!< FLASH eight wait states */
#define LL_FLASH_LATENCY_9                 FLASH_ACR_LATENCY_9WS   /*!< FLASH nine wait states */
#define LL_FLASH_LATENCY_10                FLASH_ACR_LATENCY_10WS  /*!< FLASH ten wait states */
#define LL_FLASH_LATENCY_11                FLASH_ACR_LATENCY_11WS  /*!< FLASH eleven wait states */
#define LL_FLASH_LATENCY_12                FLASH_ACR_LATENCY_12WS  /*!< FLASH twelve wait states */
#define LL_FLASH_LATENCY_13                FLASH_ACR_LATENCY_13WS  /*!< FLASH thirteen wait states */
#define LL_FLASH_LATENCY_14                FLASH_ACR_LATENCY_14WS  /*!< FLASH fourteen wait states */
#define LL_FLASH_LATENCY_15                FLASH_ACR_LATENCY_15WS  /*!< FLASH fifteen wait states */
#endif /* FLASH_ACR_LATENCY_5WS */
/**
  * @}
  */
#endif /* if 0 */

/**
  * @}
  */

/* Exported macro ------------------------------------------------------------*/

/* Exported functions --------------------------------------------------------*/
/** @defgroup SYSTEM_LL_Exported_Functions SYSTEM Exported Functions
  * @{
  */

/** @defgroup SYSTEM_LL_EF_SYSCFG SYSCFG
  * @{
  */

/**
  * @brief  Get the Device Minor Mask
  * @rmtoll MIDR1        MINOR_MASK      LL_SYSCFG_GetMinorMask
  * @param  None
  * @retval Device Minor Mask Revision
  */
__STATIC_INLINE uint32_t LL_SYSCFG_GetMinorMask(void)
{
  return (uint32_t)(READ_BIT(SYSCFG->MIDR1, SYSCFG_MIDR1_MINOR_MASK) >> (SYSCFG_MIDR1_MINOR_MASK_Pos));
}

/**
  * @brief  Get the Device Major Mask
  * @rmtoll MIDR1        MAJOR_MASK      LL_SYSCFG_GetMajorMask
  * @param  None
  * @retval Device Minor Major Revision
  */
__STATIC_INLINE uint32_t LL_SYSCFG_GetMajorMask(void)
{
  return (uint32_t)(READ_BIT(SYSCFG->MIDR1, SYSCFG_MIDR1_MAJOR_MASK) >> (SYSCFG_MIDR1_MAJOR_MASK_Pos));
}

/**
  * @brief  Get the Device Package
  * @rmtoll MIDR1        PACKAGE         LL_SYSCFG_GetPackage
  * @param  None
  * @retval Returned value can be one of the following values:
  *         @arg @ref LL_SYSCFG_MIDR1_PACKAGE_QFP100
  *         @arg @ref LL_SYSCFG_MIDR1_PACKAGE_QFP176
  */
__STATIC_INLINE uint32_t LL_SYSCFG_GetPackage(void)
{
  return (uint32_t)(READ_BIT(SYSCFG->MIDR1, SYSCFG_MIDR1_PACKAGE) >> (SYSCFG_MIDR1_PACKAGE_Pos));
}

/**
  * @brief  Get the Device Info
  * @rmtoll MIDR1        DEVICE_INFO     LL_SYSCFG_GetDeviceInfo
  * @param  None
  * @retval Device Part Number
  */
__STATIC_INLINE uint32_t LL_SYSCFG_GetDeviceInfo(void)
{
  return (uint32_t)(READ_BIT(SYSCFG->MIDR1, SYSCFG_MIDR1_DEVICE_INFO) >> (SYSCFG_MIDR1_DEVICE_INFO_Pos));
}

/**
  * @brief  Get the Device Serie
  * @rmtoll MIDR2        DEVICE_INFO     LL_SYSCFG_GetDeviceSerie
  * @param  None
  * @retval Device Serie
  */
__STATIC_INLINE uint32_t LL_SYSCFG_GetDeviceSerie(void)
{
  return (uint32_t)(READ_BIT(SYSCFG->MIDR2, SYSCFG_MIDR2_SERIE) >> (SYSCFG_MIDR2_SERIE_Pos));
}

/**
  * @brief  Get the Device NVM Size
  * @rmtoll MIDR2        NVM_SIZE        LL_SYSCFG_GetNVMSize
  * @param  None
  * @retval NVM size in KB.
  */
__STATIC_INLINE uint32_t LL_SYSCFG_GetNVMSize(void)
{
  uint32_t nvm_size_1 = (uint32_t)(READ_BIT(SYSCFG->MIDR2, SYSCFG_MIDR2_NVM_SIZE_1) >> (SYSCFG_MIDR2_NVM_SIZE_1_Pos));
  uint32_t nvm_size_2 = (uint32_t)(READ_BIT(SYSCFG->MIDR2, SYSCFG_MIDR2_NVM_SIZE_2) >> (SYSCFG_MIDR2_NVM_SIZE_2_Pos));
  uint32_t flash_size_1 = (uint32_t)(16U << nvm_size_1);
  uint32_t flash_size_2 = (uint32_t)(nvm_size_2 * (flash_size_1 / 8U));
  return (uint32_t)(flash_size_1 + flash_size_2);
}

/**
  * @brief  Get the Device Serie
  * @rmtoll MIDR2        SF              LL_SYSCFG_GetDeviceManufacturer
  * @param  None
  * @retval Device Manufacturer
  */
__STATIC_INLINE uint32_t LL_SYSCFG_GetDeviceManufacturer(void)
{
  return (uint32_t)(READ_BIT(SYSCFG->MIDR2, SYSCFG_MIDR2_SF) >> (SYSCFG_MIDR2_SF_Pos));
}

/**
  * @brief  Configure source input for the EXTI external interrupt.
  * @rmtoll EXTICR1        EXTIx         LL_SYSCFG_SetEXTISource\n
  *         EXTICR2        EXTIx         LL_SYSCFG_SetEXTISource\n
  *         EXTICR3        EXTIx         LL_SYSCFG_SetEXTISource\n
  *         EXTICR4        EXTIx         LL_SYSCFG_SetEXTISource
  * @param  Port This parameter can be one of the following values:
  *         @arg @ref LL_SYSCFG_EXTI_PORTA
  *         @arg @ref LL_SYSCFG_EXTI_PORTB
  *         @arg @ref LL_SYSCFG_EXTI_PORTC
  *         @arg @ref LL_SYSCFG_EXTI_PORTD
  *         @arg @ref LL_SYSCFG_EXTI_PORTE
  *         @arg @ref LL_SYSCFG_EXTI_PORTF
  *         @arg @ref LL_SYSCFG_EXTI_PORTG
  *         @arg @ref LL_SYSCFG_EXTI_PORTH
  *         @arg @ref LL_SYSCFG_EXTI_PORTI
  *
  *         (*) value not defined in all devices
  * @param  Line This parameter can be one of the following values:
  *         @arg @ref LL_SYSCFG_EXTI_LINE0
  *         @arg @ref LL_SYSCFG_EXTI_LINE1
  *         @arg @ref LL_SYSCFG_EXTI_LINE2
  *         @arg @ref LL_SYSCFG_EXTI_LINE3
  *         @arg @ref LL_SYSCFG_EXTI_LINE4
  *         @arg @ref LL_SYSCFG_EXTI_LINE5
  *         @arg @ref LL_SYSCFG_EXTI_LINE6
  *         @arg @ref LL_SYSCFG_EXTI_LINE7
  *         @arg @ref LL_SYSCFG_EXTI_LINE8
  *         @arg @ref LL_SYSCFG_EXTI_LINE9
  *         @arg @ref LL_SYSCFG_EXTI_LINE10
  *         @arg @ref LL_SYSCFG_EXTI_LINE11
  *         @arg @ref LL_SYSCFG_EXTI_LINE12
  *         @arg @ref LL_SYSCFG_EXTI_LINE13
  *         @arg @ref LL_SYSCFG_EXTI_LINE14
  *         @arg @ref LL_SYSCFG_EXTI_LINE15
  * @retval None
  */
__STATIC_INLINE void LL_SYSCFG_SetEXTISource(uint32_t Port, uint32_t Line)
{
  MODIFY_REG(SYSCFG->EXTICR[Line & 0x3U], (Line >> 16U), Port << (POSITION_VAL((Line >> 16U)) & 0x1FU) );
}

/**
  * @brief  Get the configured defined for specific EXTI Line
  * @rmtoll EXTICR1        EXTIx         LL_SYSCFG_GetEXTISource\n
  *         EXTICR2        EXTIx         LL_SYSCFG_GetEXTISource\n
  *         EXTICR3        EXTIx         LL_SYSCFG_GetEXTISource\n
  *         EXTICR4        EXTIx         LL_SYSCFG_GetEXTISource
  * @param  Line This parameter can be one of the following values:
  *         @arg @ref LL_SYSCFG_EXTI_LINE0
  *         @arg @ref LL_SYSCFG_EXTI_LINE1
  *         @arg @ref LL_SYSCFG_EXTI_LINE2
  *         @arg @ref LL_SYSCFG_EXTI_LINE3
  *         @arg @ref LL_SYSCFG_EXTI_LINE4
  *         @arg @ref LL_SYSCFG_EXTI_LINE5
  *         @arg @ref LL_SYSCFG_EXTI_LINE6
  *         @arg @ref LL_SYSCFG_EXTI_LINE7
  *         @arg @ref LL_SYSCFG_EXTI_LINE8
  *         @arg @ref LL_SYSCFG_EXTI_LINE9
  *         @arg @ref LL_SYSCFG_EXTI_LINE10
  *         @arg @ref LL_SYSCFG_EXTI_LINE11
  *         @arg @ref LL_SYSCFG_EXTI_LINE12
  *         @arg @ref LL_SYSCFG_EXTI_LINE13
  *         @arg @ref LL_SYSCFG_EXTI_LINE14
  *         @arg @ref LL_SYSCFG_EXTI_LINE15
  * @retval Returned value can be one of the following values:
  *         @arg @ref LL_SYSCFG_EXTI_PORTA
  *         @arg @ref LL_SYSCFG_EXTI_PORTB
  *         @arg @ref LL_SYSCFG_EXTI_PORTC
  *         @arg @ref LL_SYSCFG_EXTI_PORTD
  *         @arg @ref LL_SYSCFG_EXTI_PORTE
  *         @arg @ref LL_SYSCFG_EXTI_PORTF
  *         @arg @ref LL_SYSCFG_EXTI_PORTG
  *         @arg @ref LL_SYSCFG_EXTI_PORTH
  *         @arg @ref LL_SYSCFG_EXTI_PORTI
  *
  *         (*) value not defined in all devices
  */
__STATIC_INLINE uint32_t LL_SYSCFG_GetEXTISource(uint32_t Line)
{
  return (uint32_t)(READ_BIT(SYSCFG->EXTICR[Line & 0x3U], (Line >> 16U)) >> (POSITION_VAL(Line >> 16U) & 0x1FU));
}

/**
  * @brief  Configure EXT Gating Selection for SDADC1.
  * @rmtoll SDADCX_EXT_GATE_SEL  SYSCFG_SDADCX_EXT_GATE_SEL_SDADC1_EXT_GATE_SEL  LL_SYSCFG_SetSDADC1EXTGating
  * @param  Line This parameter can be one of the following values:
  *         @arg @ref LL_SYSCFG_SDADC1_EXT_GATE_TIM1_OC1
  *         @arg @ref LL_SYSCFG_SDADC1_EXT_GATE_TIM1_OC5
  *         @arg @ref LL_SYSCFG_SDADC1_EXT_GATE_TIM8_OC1
  *         @arg @ref LL_SYSCFG_SDADC1_EXT_GATE_TIM8_OC5
  *         @arg @ref LL_SYSCFG_SDADC1_EXT_GATE_TIM2_OC4
  *         @arg @ref LL_SYSCFG_SDADC1_EXT_GATE_TIM3_OC4
  *         @arg @ref LL_SYSCFG_SDADC1_EXT_GATE_TIM4_OC4
  *         @arg @ref LL_SYSCFG_SDADC1_EXT_GATE_TIM5_OC4
  *         @arg @ref LL_SYSCFG_SDADC1_EXT_GATE_TIM15_OC2
  *         @arg @ref LL_SYSCFG_SDADC1_EXT_GATE_HRTIMER1_TE
  *         @arg @ref LL_SYSCFG_SDADC1_EXT_GATE_HRTIMER1_TF
  *         @arg @ref LL_SYSCFG_SDADC1_EXT_GATE_HRTIMER2_TE
  *         @arg @ref LL_SYSCFG_SDADC1_EXT_GATE_HRTIMER2_TF
  * @retval None
  */
__STATIC_INLINE void LL_SYSCFG_SetSDADC1EXTGating(uint32_t Line)
{
  MODIFY_REG(SYSCFG->SDADCX_EXT_GATE_SEL, SYSCFG_SDADCX_EXT_GATE_SEL_SDADC1_EXT_GATE_SEL, Line);
}

/**
  * @brief  Get EXT Gating Selection for SDADC1.
  * @rmtoll SDADCX_EXT_GATE_SEL  SYSCFG_SDADCX_EXT_GATE_SEL_SDADC1_EXT_GATE_SEL  LL_SYSCFG_GetSDADC1EXTGating
  * @retval Returned value can be one of the following values:
  *         @arg @ref LL_SYSCFG_SDADC1_EXT_GATE_TIM1_OC1
  *         @arg @ref LL_SYSCFG_SDADC1_EXT_GATE_TIM1_OC5
  *         @arg @ref LL_SYSCFG_SDADC1_EXT_GATE_TIM8_OC1
  *         @arg @ref LL_SYSCFG_SDADC1_EXT_GATE_TIM8_OC5
  *         @arg @ref LL_SYSCFG_SDADC1_EXT_GATE_TIM2_OC4
  *         @arg @ref LL_SYSCFG_SDADC1_EXT_GATE_TIM3_OC4
  *         @arg @ref LL_SYSCFG_SDADC1_EXT_GATE_TIM4_OC4
  *         @arg @ref LL_SYSCFG_SDADC1_EXT_GATE_TIM5_OC4
  *         @arg @ref LL_SYSCFG_SDADC1_EXT_GATE_TIM15_OC2
  *         @arg @ref LL_SYSCFG_SDADC1_EXT_GATE_HRTIMER1_TE
  *         @arg @ref LL_SYSCFG_SDADC1_EXT_GATE_HRTIMER1_TF
  *         @arg @ref LL_SYSCFG_SDADC1_EXT_GATE_HRTIMER2_TE
  *         @arg @ref LL_SYSCFG_SDADC1_EXT_GATE_HRTIMER2_TF
  */
__STATIC_INLINE uint32_t LL_SYSCFG_GetSDADC1EXTGating(void)
{
  return (uint32_t)(READ_BIT(SYSCFG->SDADCX_EXT_GATE_SEL, SYSCFG_SDADCX_EXT_GATE_SEL_SDADC1_EXT_GATE_SEL));
}

/**
  * @brief  Configure EXT Gating Selection for SDADC2.
  * @rmtoll SDADCX_EXT_GATE_SEL  SYSCFG_SDADCX_EXT_GATE_SEL_SDADC2_EXT_GATE_SEL  LL_SYSCFG_SetSDADC2EXTGating
  * @param  Line This parameter can be one of the following values:
  *         @arg @ref LL_SYSCFG_SDADC2_EXT_GATE_TIM1_OC1
  *         @arg @ref LL_SYSCFG_SDADC2_EXT_GATE_TIM1_OC5
  *         @arg @ref LL_SYSCFG_SDADC2_EXT_GATE_TIM8_OC1
  *         @arg @ref LL_SYSCFG_SDADC2_EXT_GATE_TIM8_OC5
  *         @arg @ref LL_SYSCFG_SDADC2_EXT_GATE_TIM2_OC4
  *         @arg @ref LL_SYSCFG_SDADC2_EXT_GATE_TIM3_OC4
  *         @arg @ref LL_SYSCFG_SDADC2_EXT_GATE_TIM4_OC4
  *         @arg @ref LL_SYSCFG_SDADC2_EXT_GATE_TIM5_OC4
  *         @arg @ref LL_SYSCFG_SDADC2_EXT_GATE_TIM15_OC2
  *         @arg @ref LL_SYSCFG_SDADC2_EXT_GATE_HRTIMER1_TE
  *         @arg @ref LL_SYSCFG_SDADC2_EXT_GATE_HRTIMER1_TF
  *         @arg @ref LL_SYSCFG_SDADC2_EXT_GATE_HRTIMER2_TE
  *         @arg @ref LL_SYSCFG_SDADC2_EXT_GATE_HRTIMER2_TF
  * @retval None
  */
__STATIC_INLINE void LL_SYSCFG_SetSDADC2EXTGating(uint32_t Line)
{
  MODIFY_REG(SYSCFG->SDADCX_EXT_GATE_SEL, SYSCFG_SDADCX_EXT_GATE_SEL_SDADC2_EXT_GATE_SEL, Line);
}

/**
  * @brief  Get EXT Gating Selection for SDADC2.
  * @rmtoll SDADCX_EXT_GATE_SEL  SYSCFG_SDADCX_EXT_GATE_SEL_SDADC2_EXT_GATE_SEL  LL_SYSCFG_GetSDADC2EXTGating
  * @retval Returned value can be one of the following values:
  *         @arg @ref LL_SYSCFG_SDADC2_EXT_GATE_TIM1_OC1
  *         @arg @ref LL_SYSCFG_SDADC2_EXT_GATE_TIM1_OC5
  *         @arg @ref LL_SYSCFG_SDADC2_EXT_GATE_TIM8_OC1
  *         @arg @ref LL_SYSCFG_SDADC2_EXT_GATE_TIM8_OC5
  *         @arg @ref LL_SYSCFG_SDADC2_EXT_GATE_TIM2_OC4
  *         @arg @ref LL_SYSCFG_SDADC2_EXT_GATE_TIM3_OC4
  *         @arg @ref LL_SYSCFG_SDADC2_EXT_GATE_TIM4_OC4
  *         @arg @ref LL_SYSCFG_SDADC2_EXT_GATE_TIM5_OC4
  *         @arg @ref LL_SYSCFG_SDADC2_EXT_GATE_TIM15_OC2
  *         @arg @ref LL_SYSCFG_SDADC2_EXT_GATE_HRTIMER1_TE
  *         @arg @ref LL_SYSCFG_SDADC2_EXT_GATE_HRTIMER1_TF
  *         @arg @ref LL_SYSCFG_SDADC2_EXT_GATE_HRTIMER2_TE
  *         @arg @ref LL_SYSCFG_SDADC2_EXT_GATE_HRTIMER2_TF
  */
__STATIC_INLINE uint32_t LL_SYSCFG_GetSDADC2EXTGating(void)
{
  return (uint32_t)(READ_BIT(SYSCFG->SDADCX_EXT_GATE_SEL, SYSCFG_SDADCX_EXT_GATE_SEL_SDADC2_EXT_GATE_SEL));
}

/**
  * @brief  Enable ADC Global Bias.
  * @note   This bit must be written and cleared by the software. Once it is
  *         enabled, the read operation provides a 0x1 after 2us which is the
  *         BIAS startup time. The software must check this bit; when it is
  *         equal to 0x1, the SAR and the SD ADCs can be used (BIAS VBG =
  *         0.9VBG).
  * @rmtoll ADC_BIASEN   GLOBAL_BIAS_EN  LL_SYSCFG_EnableADCGlobalBias
  * @param  None
  * @retval None
  */
__STATIC_INLINE void LL_SYSCFG_EnableADCGlobalBias(void)
{
  SET_BIT(SYSCFG->ADC_BIASEN, SYSCFG_ADC_BIASEN_GLOBAL_BIAS_EN);
  while ((READ_BIT(SYSCFG->ADC_BIASEN, SYSCFG_ADC_BIASEN_GLOBAL_BIAS_EN)) == 0UL);
}

/**
  * @brief  Disable ADC Global Bias.
  * @rmtoll ADC_BIASEN   GLOBAL_BIAS_EN  LL_SYSCFG_DisableADCGlobalBias
  * @param  None
  * @retval None
  */
__STATIC_INLINE void LL_SYSCFG_DisableADCGlobalBias(void)
{
  CLEAR_BIT(SYSCFG->ADC_BIASEN, SYSCFG_ADC_BIASEN_GLOBAL_BIAS_EN);
}

/**
  * @brief  Configure FCCU Glitch Filter 1 Width.
  * @rmtoll FCCU_FILTER1  SYSCFG_FCCU_FILTER1_FILTER_WIDTH  LL_SYSCFG_SetFCCUGlitchFilter1Width
  * @param  Width This parameter can be one of the following values:
  *         @arg @ref LL_SYSCFG_FCCU_FILTER_WIDTH_50US
  *         @arg @ref LL_SYSCFG_FCCU_FILTER_WIDTH_75US
  *         @arg @ref LL_SYSCFG_FCCU_FILTER_WIDTH_100US
  *         @arg @ref LL_SYSCFG_FCCU_FILTER_WIDTH_125US
  * @retval None
  */
__STATIC_INLINE void LL_SYSCFG_SetFCCUGlitchFilter1Width(uint32_t Width)
{
  MODIFY_REG(SYSCFG->FCCU_FILTER1, SYSCFG_FCCU_FILTER1_FILTER_WIDTH, Width);
}

/**
  * @brief  Get FCCU Glitch Filter 1 Width.
  * @rmtoll FCCU_FILTER1  SYSCFG_FCCU_FILTER1_FILTER_WIDTH  LL_SYSCFG_GetFCCUGlitchFilter1Width
  * @retval Returned value can be one of the following values:
  *         @arg @ref LL_SYSCFG_FCCU_FILTER_WIDTH_50US
  *         @arg @ref LL_SYSCFG_FCCU_FILTER_WIDTH_75US
  *         @arg @ref LL_SYSCFG_FCCU_FILTER_WIDTH_100US
  *         @arg @ref LL_SYSCFG_FCCU_FILTER_WIDTH_125US
  */
__STATIC_INLINE uint32_t LL_SYSCFG_GetFCCUGlitchFilter1Width(void)
{
  return (uint32_t)(READ_BIT(SYSCFG->FCCU_FILTER1, SYSCFG_FCCU_FILTER1_FILTER_WIDTH));
}

/**
  * @brief  Enable FCCU Glitch Filter 1 bypass
  * @note   This bit must be written and cleared by the software.
  * @rmtoll FCCU_FILTER1  SYSCFG_FCCU_FILTER1_FILTER_BYPASS  LL_SYSCFG_EnableFCCUGlitchFilter1Bypass
  * @param  None
  * @retval None
  */
__STATIC_INLINE void LL_SYSCFG_EnableFCCUGlitchFilter1Bypass(void)
{
  SET_BIT(SYSCFG->FCCU_FILTER1, SYSCFG_FCCU_FILTER1_FILTER_BYPASS);
}

/**
  * @brief  Disable FCCU Glitch Filter 1 bypass
  * @rmtoll FCCU_FILTER1  SYSCFG_FCCU_FILTER1_FILTER_BYPASS  LL_SYSCFG_DisableFCCUGlitchFilter1Bypass
  * @param  None
  * @retval None
  */
__STATIC_INLINE void LL_SYSCFG_DisableFCCUGlitchFilter1Bypass(void)
{
  CLEAR_BIT(SYSCFG->FCCU_FILTER1, SYSCFG_FCCU_FILTER1_FILTER_BYPASS);
}

/**
  * @brief  Indicates whether the FCCU Glitch Filter 1 bypass is enabled.
  * @rmtoll FCCU_FILTER1  SYSCFG_FCCU_FILTER1_FILTER_BYPASS  LL_SYSCFG_IsEnabledFCCUGlitchFilter1Bypass
  * @param  None
  * @retval State of bit (1 or 0).
  */
__STATIC_INLINE uint32_t LL_SYSCFG_IsEnabledFCCUGlitchFilter1Bypass(void)
{
  return ((READ_BIT(SYSCFG->FCCU_FILTER1, SYSCFG_FCCU_FILTER1_FILTER_BYPASS) == (SYSCFG_FCCU_FILTER1_FILTER_BYPASS)) ? 1UL : 0UL);
}

/**
  * @brief  Configure FCCU Glitch Filter 2 Width.
  * @rmtoll FCCU_FILTER2  SYSCFG_FCCU_FILTER2_FILTER_WIDTH  LL_SYSCFG_SetFCCUGlitchFilter2Width
  * @param  Width This parameter can be one of the following values:
  *         @arg @ref LL_SYSCFG_FCCU_FILTER_WIDTH_50US
  *         @arg @ref LL_SYSCFG_FCCU_FILTER_WIDTH_75US
  *         @arg @ref LL_SYSCFG_FCCU_FILTER_WIDTH_100US
  *         @arg @ref LL_SYSCFG_FCCU_FILTER_WIDTH_125US
  * @retval None
  */
__STATIC_INLINE void LL_SYSCFG_SetFCCUGlitchFilter2Width(uint32_t Width)
{
  MODIFY_REG(SYSCFG->FCCU_FILTER2, SYSCFG_FCCU_FILTER2_FILTER_WIDTH, Width);
}

/**
  * @brief  Get FCCU Glitch Filter 2 Width.
  * @rmtoll FCCU_FILTER2  SYSCFG_FCCU_FILTER2_FILTER_WIDTH  LL_SYSCFG_GetFCCUGlitchFilter2Width
  * @retval Returned value can be one of the following values:
  *         @arg @ref LL_SYSCFG_FCCU_FILTER_WIDTH_50US
  *         @arg @ref LL_SYSCFG_FCCU_FILTER_WIDTH_75US
  *         @arg @ref LL_SYSCFG_FCCU_FILTER_WIDTH_100US
  *         @arg @ref LL_SYSCFG_FCCU_FILTER_WIDTH_125US
  */
__STATIC_INLINE uint32_t LL_SYSCFG_GetFCCUGlitchFilter2Width(void)
{
  return (uint32_t)(READ_BIT(SYSCFG->FCCU_FILTER2, SYSCFG_FCCU_FILTER2_FILTER_WIDTH));
}

/**
  * @brief  Enable FCCU Glitch Filter 2 bypass
  * @note   This bit must be written and cleared by the software.
  * @rmtoll FCCU_FILTER2  SYSCFG_FCCU_FILTER2_FILTER_BYPASS  LL_SYSCFG_EnableFCCUGlitchFilter2Bypass
  * @param  None
  * @retval None
  */
__STATIC_INLINE void LL_SYSCFG_EnableFCCUGlitchFilter2Bypass(void)
{
  SET_BIT(SYSCFG->FCCU_FILTER2, SYSCFG_FCCU_FILTER2_FILTER_BYPASS);
}

/**
  * @brief  Disable FCCU Glitch Filter 2 bypass
  * @rmtoll FCCU_FILTER2  SYSCFG_FCCU_FILTER2_FILTER_BYPASS  LL_SYSCFG_DisableFCCUGlitchFilter2Bypass
  * @param  None
  * @retval None
  */
__STATIC_INLINE void LL_SYSCFG_DisableFCCUGlitchFilter2Bypass(void)
{
  CLEAR_BIT(SYSCFG->FCCU_FILTER2, SYSCFG_FCCU_FILTER2_FILTER_BYPASS);
}

/**
  * @brief  Indicates whether the FCCU Glitch Filter 2 bypass is enabled.
  * @rmtoll FCCU_FILTER2  SYSCFG_FCCU_FILTER2_FILTER_BYPASS  LL_SYSCFG_IsEnabledFCCUGlitchFilter2Bypass
  * @param  None
  * @retval State of bit (1 or 0).
  */
__STATIC_INLINE uint32_t LL_SYSCFG_IsEnabledFCCUGlitchFilter2Bypass(void)
{
  return ((READ_BIT(SYSCFG->FCCU_FILTER2, SYSCFG_FCCU_FILTER2_FILTER_BYPASS) == (SYSCFG_FCCU_FILTER2_FILTER_BYPASS)) ? 1UL : 0UL);
}

/**
  * @brief  Configure Comp VREF for self-test.
  * @rmtoll ST_VREF_SEL1  SYSCFG_ST_VREF_SEL1_COMP_TESTVREF_SEL  LL_SYSCFG_SetCompTestVref
  * @param  Vref This parameter can be one of the following values:
  *         @arg @ref LL_SYSCFG_COMP_TESTVREF_VREL
  *         @arg @ref LL_SYSCFG_COMP_TESTVREF_1_3_VREH_VREFL
  *         @arg @ref LL_SYSCFG_COMP_TESTVREF_2_3_VREH_VREFL
  *         @arg @ref LL_SYSCFG_COMP_TESTVREF_VREH
  * @retval None
  */
__STATIC_INLINE void LL_SYSCFG_SetCompTestVref(uint32_t Vref)
{
  MODIFY_REG(SYSCFG->ST_VREF_SEL1, SYSCFG_ST_VREF_SEL1_COMP_TESTVREF_SEL, Vref);
}

/**
  * @brief  Get Comp VREF for self-test.
  * @rmtoll ST_VREF_SEL1  SYSCFG_ST_VREF_SEL1_COMP_TESTVREF_SEL  LL_SYSCFG_GetCompTestVref
  * @retval Returned value can be one of the following values:
  *         @arg @ref LL_SYSCFG_COMP_TESTVREF_VREL
  *         @arg @ref LL_SYSCFG_COMP_TESTVREF_1_3_VREH_VREFL
  *         @arg @ref LL_SYSCFG_COMP_TESTVREF_2_3_VREH_VREFL
  *         @arg @ref LL_SYSCFG_COMP_TESTVREF_VREH
  */
__STATIC_INLINE uint32_t LL_SYSCFG_GetCompTestVref(void)
{
  return (uint32_t)(READ_BIT(SYSCFG->ST_VREF_SEL1, SYSCFG_ST_VREF_SEL1_COMP_TESTVREF_SEL));
}

/**
  * @brief  Enable Comp Self Test.
  * @note   Enabling must occur after the voltage has been selected
  * @rmtoll ST_VREF_SEL1  SYSCFG_ST_VREF_SEL1_SELFTEST_EN  LL_SYSCFG_EnableCompSelfTest
  * @param  None
  * @retval None
  */
__STATIC_INLINE void LL_SYSCFG_EnableCompSelfTest(void)
{
  SET_BIT(SYSCFG->ST_VREF_SEL1, SYSCFG_ST_VREF_SEL1_SELFTEST_EN);
}

/**
  * @brief  Disable Comp Self Test.
  * @rmtoll ST_VREF_SEL1  SYSCFG_ST_VREF_SEL1_SELFTEST_EN  LL_SYSCFG_DisableCompSelfTest
  * @param  None
  * @retval None
  */
__STATIC_INLINE void LL_SYSCFG_DisableCompSelfTest(void)
{
  CLEAR_BIT(SYSCFG->ST_VREF_SEL1, SYSCFG_ST_VREF_SEL1_SELFTEST_EN);
}

/**
  * @brief  Indicates whether the Comp Self Test is enabled.
  * @rmtoll ST_VREF_SEL1  SYSCFG_ST_VREF_SEL1_SELFTEST_EN  LL_SYSCFG_IsEnabledCompSelfTest
  * @param  None
  * @retval State of bit (1 or 0).
  */
__STATIC_INLINE uint32_t  LL_SYSCFG_IsEnabledCompSelfTest(void)
{
  return ((READ_BIT(SYSCFG->ST_VREF_SEL1, SYSCFG_ST_VREF_SEL1_SELFTEST_EN) == (SYSCFG_ST_VREF_SEL1_SELFTEST_EN)) ? 1UL : 0UL);
}

/**
  * @brief  Enable Comp Bias.
  * @rmtoll ST_VREF_SEL1  SYSCFG_ST_VREF_SEL1_COMPBIAS_EN  LL_SYSCFG_EnableCompBias
  * @param  None
  * @retval None
  */
__STATIC_INLINE void LL_SYSCFG_EnableCompBias(void)
{
  SET_BIT(SYSCFG->ST_VREF_SEL1, SYSCFG_ST_VREF_SEL1_COMPBIAS_EN);
}

/**
  * @brief  Disable Comp Bias.
  * @rmtoll ST_VREF_SEL1  SYSCFG_ST_VREF_SEL1_COMPBIAS_EN  LL_SYSCFG_DisableCompBias
  * @param  None
  * @retval None
  */
__STATIC_INLINE void LL_SYSCFG_DisableCompBias(void)
{
  CLEAR_BIT(SYSCFG->ST_VREF_SEL1, SYSCFG_ST_VREF_SEL1_COMPBIAS_EN);
}

/**
  * @brief  Indicates whether the Comp Bias is enabled.
  * @rmtoll ST_VREF_SEL1  SYSCFG_ST_VREF_SEL1_COMPBIAS_EN  LL_SYSCFG_IsEnabledCompBias
  * @param  None
  * @retval State of bit (1 or 0).
  */
__STATIC_INLINE uint32_t LL_SYSCFG_IsEnabledCompBias(void)
{
  return ((READ_BIT(SYSCFG->ST_VREF_SEL1, SYSCFG_ST_VREF_SEL1_COMPBIAS_EN) == (SYSCFG_ST_VREF_SEL1_COMPBIAS_EN)) ? 1UL : 0UL);
}

/**
  * @brief  Configure SAR VREF for self-test.
  * @rmtoll ST_VREF_SEL2  SYSCFG_ST_VREF_SEL2_SAR_TESTVREF_SEL  LL_SYSCFG_SetSARTestVref
  * @param  Vref This parameter can be one of the following values:
  *         @arg @ref LL_SYSCFG_SAR_TESTVREF_VREL
  *         @arg @ref LL_SYSCFG_SAR_TESTVREF_1_3_VREH_VREFL
  *         @arg @ref LL_SYSCFG_SAR_TESTVREF_2_3_VREH_VREFL
  *         @arg @ref LL_SYSCFG_SAR_TESTVREF_VREH
  * @retval None
  */
__STATIC_INLINE void LL_SYSCFG_SetSARTestVref(uint32_t Vref)
{
  MODIFY_REG(SYSCFG->ST_VREF_SEL2, SYSCFG_ST_VREF_SEL2_SAR_TESTVREF_SEL, Vref);
}

/**
  * @brief  Get SAR VREF for self-test.
  * @rmtoll ST_VREF_SEL2  SYSCFG_ST_VREF_SEL2_SAR_TESTVREF_SEL  LL_SYSCFG_GetSARTestVref
  * @retval Returned value can be one of the following values:
  *         @arg @ref LL_SYSCFG_SAR_TESTVREF_VREL
  *         @arg @ref LL_SYSCFG_SAR_TESTVREF_1_3_VREH_VREFL
  *         @arg @ref LL_SYSCFG_SAR_TESTVREF_2_3_VREH_VREFL
  *         @arg @ref LL_SYSCFG_SAR_TESTVREF_VREH
  */
__STATIC_INLINE uint32_t LL_SYSCFG_GetSARTestVref(void)
{
  return (uint32_t)(READ_BIT(SYSCFG->ST_VREF_SEL2, SYSCFG_ST_VREF_SEL2_SAR_TESTVREF_SEL));
}

/**
  * @brief  Enable SAR Self Test.
  * @note   Enabling must occur after the voltage has been selected
  * @rmtoll ST_VREF_SEL2  SYSCFG_ST_VREF_SEL2_SELFTEST_EN  LL_SYSCFG_EnableSARSelfTest
  * @param  None
  * @retval None
  */
__STATIC_INLINE void LL_SYSCFG_EnableSARSelfTest(void)
{
  SET_BIT(SYSCFG->ST_VREF_SEL2, SYSCFG_ST_VREF_SEL2_SELFTEST_EN);
}

/**
  * @brief  Disable SAR Self Test.
  * @rmtoll ST_VREF_SEL2  SYSCFG_ST_VREF_SEL2_SELFTEST_EN  LL_SYSCFG_DisableSARSelfTest
  * @param  None
  * @retval None
  */
__STATIC_INLINE void LL_SYSCFG_DisableSARSelfTest(void)
{
  CLEAR_BIT(SYSCFG->ST_VREF_SEL2, SYSCFG_ST_VREF_SEL2_SELFTEST_EN);
}

/**
  * @brief  Indicates whether the SAR Self Test is enabled.
  * @rmtoll ST_VREF_SEL2  SYSCFG_ST_VREF_SEL2_SELFTEST_EN  LL_SYSCFG_IsEnabledSARSelfTest
  * @param  None
  * @retval State of bit (1 or 0).
  */
__STATIC_INLINE uint32_t LL_SYSCFG_IsEnabledSARSelfTest(void)
{
  return ((READ_BIT(SYSCFG->ST_VREF_SEL2, SYSCFG_ST_VREF_SEL2_SELFTEST_EN) == (SYSCFG_ST_VREF_SEL2_SELFTEST_EN)) ? 1UL : 0UL);
}

/**
  * @brief  Enable SAR Bias.
  * @rmtoll ST_VREF_SEL2  SYSCFG_ST_VREF_SEL2_SARBIAS_EN  LL_SYSCFG_EnableSARBias
  * @param  None
  * @retval None
  */
__STATIC_INLINE void LL_SYSCFG_EnableSARBias(void)
{
  SET_BIT(SYSCFG->ST_VREF_SEL2, SYSCFG_ST_VREF_SEL2_SARBIAS_EN);
}

/**
  * @brief  Disable SAR Bias.
  * @rmtoll ST_VREF_SEL2  SYSCFG_ST_VREF_SEL2_SARBIAS_EN  LL_SYSCFG_DisableSARBias
  * @param  None
  * @retval None
  */
__STATIC_INLINE void LL_SYSCFG_DisableSARBias(void)
{
  CLEAR_BIT(SYSCFG->ST_VREF_SEL2, SYSCFG_ST_VREF_SEL2_SARBIAS_EN);
}

/**
  * @brief  Indicates whether the SAR Bias is enabled.
  * @rmtoll ST_VREF_SEL2  SYSCFG_ST_VREF_SEL2_SARBIAS_EN  LL_SYSCFG_IsEnabledSARBias
  * @param  None
  * @retval State of bit (1 or 0).
  */
__STATIC_INLINE uint32_t LL_SYSCFG_IsEnabledSARBias(void)
{
  return ((READ_BIT(SYSCFG->ST_VREF_SEL2, SYSCFG_ST_VREF_SEL2_SARBIAS_EN) == (SYSCFG_ST_VREF_SEL2_SARBIAS_EN)) ? 1UL : 0UL);
}

/**
  * @brief  Enable HRTIM Update.
  * @note   This bit must be written to 1 to send the trigger to both HRTIMs,
  *         pulse one clock cycle long. It is automatically cleared by the
  *         hardware.
  * @rmtoll HRTIMX_UPDATE_EN  UPDATE_EN  LL_SYSCFG_EnableHRTIMUpdate
  * @param  None
  * @retval None
  */
__STATIC_INLINE void LL_SYSCFG_EnableHRTIMUpdate(void)
{
  SET_BIT(SYSCFG->HRTIMX_UPDATE_EN, SYSCFG_HRTIMX_UPDATE_EN_UPDATE_EN);
}

/**
  * @brief  Enable HRTIM FCCU Fault.
  * @note   This bit enables the pads entry to safe mode when FCCU is in fault
  *         state.
  * @rmtoll HRTIMXFLT_TIMXBRK_EN  HRTIMERX_FLT_EN  LL_SYSCFG_EnableHRTIMFault
  * @param  None
  * @retval None
  */
__STATIC_INLINE void LL_SYSCFG_EnableHRTIMFault(void)
{
  SET_BIT(SYSCFG->HRTIMXFLT_TIMXBRK_EN, SYSCFG_HRTIMXFLT_TIMXBRK_EN_HRTIMERX_FLT_EN);
}

/**
  * @brief  Disable HRTIM FCCU Fault.
  * @note   This bit disables the pads entry to safe mode when FCCU is in fault
  *         state.
  * @rmtoll HRTIMXFLT_TIMXBRK_EN  HRTIMERX_FLT_EN  LL_SYSCFG_DisableHRTIMFault
  * @param  None
  * @retval None
  */
__STATIC_INLINE void LL_SYSCFG_DisableHRTIMFault(void)
{
  CLEAR_BIT(SYSCFG->HRTIMXFLT_TIMXBRK_EN, SYSCFG_HRTIMXFLT_TIMXBRK_EN_HRTIMERX_FLT_EN);
}

/**
  * @brief  Indicates whether the HRTIM FCCU Fault is enabled.
  * @rmtoll HRTIMXFLT_TIMXBRK_EN  HRTIMERX_FLT_EN  LL_SYSCFG_IsEnabledHRTIMFault
  * @param  None
  * @retval State of bit (1 or 0).
  */
__STATIC_INLINE uint32_t LL_SYSCFG_IsEnabledHRTIMFault(void)
{
  return ((READ_BIT(SYSCFG->HRTIMXFLT_TIMXBRK_EN, SYSCFG_HRTIMXFLT_TIMXBRK_EN_HRTIMERX_FLT_EN) == (SYSCFG_HRTIMXFLT_TIMXBRK_EN_HRTIMERX_FLT_EN)) ? 1UL : 0UL);
}

/**
  * @brief  Enable HRTIM Safe Mode Request.
  * @note   This bit enables the pads entry to safe mode when System received
  *         safe mode request.
  * @rmtoll HRTIMXFLT_TIMXBRK_EN  HRTIMERX_SAFEMODE_EN  LL_SYSCFG_EnableHRTIMSafeModeRequest
  * @param  None
  * @retval None
  */
__STATIC_INLINE void LL_SYSCFG_EnableHRTIMSafeModeRequest(void)
{
  SET_BIT(SYSCFG->HRTIMXFLT_TIMXBRK_EN, SYSCFG_HRTIMXFLT_TIMXBRK_EN_HRTIMERX_SAFEMODE_EN);
}

/**
  * @brief  Disable HRTIM Safe Mode Request.
  * @note   This bit disables the pads entry to safe mode when FCCU is in fault
  *         state.
  * @rmtoll HRTIMXFLT_TIMXBRK_EN  HRTIMERX_SAFEMODE_EN  LL_SYSCFG_DisableHRTIMSafeModeRequest
  * @param  None
  * @retval None
  */
__STATIC_INLINE void LL_SYSCFG_DisableHRTIMSafeModeRequest(void)
{
  CLEAR_BIT(SYSCFG->HRTIMXFLT_TIMXBRK_EN, SYSCFG_HRTIMXFLT_TIMXBRK_EN_HRTIMERX_SAFEMODE_EN);
}

/**
  * @brief  Indicates whether the HRTIM Safe Mode Request is enabled.
  * @rmtoll HRTIMXFLT_TIMXBRK_EN  HRTIMERX_SAFEMODE_EN  LL_SYSCFG_IsEnabledHRTIMSafeModeRequest
  * @param  None
  * @retval State of bit (1 or 0).
  */
__STATIC_INLINE uint32_t LL_SYSCFG_IsEnabledHRTIMSafeModeRequest(void)
{
  return ((READ_BIT(SYSCFG->HRTIMXFLT_TIMXBRK_EN, SYSCFG_HRTIMXFLT_TIMXBRK_EN_HRTIMERX_SAFEMODE_EN) == (SYSCFG_HRTIMXFLT_TIMXBRK_EN_HRTIMERX_SAFEMODE_EN)) ? 1UL : 0UL);
}

/**
  * @brief  Enable TIM FCCU Fault.
  * @note   This bit enables FCCU fault for TIM1, TIM8, TIM15, TIM16.
  * @rmtoll HRTIMXFLT_TIMXBRK_EN  TIMX_FLT_EN  LL_SYSCFG_EnableTIMFault
  * @param  None
  * @retval None
  */
__STATIC_INLINE void  LL_SYSCFG_EnableTIMFault(void)
{
  SET_BIT(SYSCFG->HRTIMXFLT_TIMXBRK_EN, SYSCFG_HRTIMXFLT_TIMXBRK_EN_TIMX_FLT_EN);
}

/**
  * @brief  Disable TIM FCCU Fault.
  * @note   This bit disables FCCU fault for TIM1, TIM8, TIM15, TIM16.
  * @rmtoll HRTIMXFLT_TIMXBRK_EN  TIMX_FLT_EN  LL_SYSCFG_EnableTIMFault
  * @param  None
  * @retval None
  */
__STATIC_INLINE void LL_SYSCFG_DisableTIMFault(void)
{
  CLEAR_BIT(SYSCFG->HRTIMXFLT_TIMXBRK_EN, SYSCFG_HRTIMXFLT_TIMXBRK_EN_TIMX_FLT_EN);
}

/**
  * @brief  Indicates whether the TIM FCCU Fault is enabled.
  * @rmtoll HRTIMXFLT_TIMXBRK_EN  TIMX_FLT_EN  LL_SYSCFG_IsEnabledTIMFault
  * @param  None
  * @retval State of bit (1 or 0).
  */
__STATIC_INLINE uint32_t LL_SYSCFG_IsEnabledTIMFault(void)
{
  return ((READ_BIT(SYSCFG->HRTIMXFLT_TIMXBRK_EN, SYSCFG_HRTIMXFLT_TIMXBRK_EN_TIMX_FLT_EN) == (SYSCFG_HRTIMXFLT_TIMXBRK_EN_TIMX_FLT_EN)) ? 1UL : 0UL);
}

/**
  * @brief  Enable TIM Safe Mode Request.
  * @note   This bit enables safe mode request for TIM1, TIM8, TIM15, TIM16.
  * @rmtoll HRTIMXFLT_TIMXBRK_EN  TIMX_SAFEMODE_EN  LL_SYSCFG_EnableTIMSafeModeRequest
  * @param  None
  * @retval None
  */
__STATIC_INLINE void LL_SYSCFG_EnableTIMSafeModeRequest(void)
{
  SET_BIT(SYSCFG->HRTIMXFLT_TIMXBRK_EN, SYSCFG_HRTIMXFLT_TIMXBRK_EN_TIMX_SAFEMODE_EN);
}

/**
  * @brief  Disable TIM Safe Mode Request.
  * @note   This bit disables safe mode request for TIM1, TIM8, TIM15, TIM16.
  * @rmtoll HRTIMXFLT_TIMXBRK_EN  TIMX_SAFEMODE_EN  LL_SYSCFG_EnableTIMSafeModeRequest
  * @param  None
  * @retval None
  */
__STATIC_INLINE void LL_SYSCFG_DisableTIMSafeModeRequest(void)
{
  CLEAR_BIT(SYSCFG->HRTIMXFLT_TIMXBRK_EN, SYSCFG_HRTIMXFLT_TIMXBRK_EN_TIMX_SAFEMODE_EN);
}

/**
  * @brief  Indicates whether the TIM Safe Mode Request is enabled.
  * @rmtoll HRTIMXFLT_TIMXBRK_EN  TIMX_SAFEMODE_EN  LL_SYSCFG_IsEnabledTIMSafeModeRequest
  * @param  None
  * @retval State of bit (1 or 0).
  */
__STATIC_INLINE uint32_t LL_SYSCFG_IsEnabledTIMSafeModeRequest(void)
{
  return ((READ_BIT(SYSCFG->HRTIMXFLT_TIMXBRK_EN, SYSCFG_HRTIMXFLT_TIMXBRK_EN_TIMX_SAFEMODE_EN) == (SYSCFG_HRTIMXFLT_TIMXBRK_EN_TIMX_SAFEMODE_EN)) ? 1UL : 0UL);
}

/**
  * @}
  */

#if 0 /* SR5E1_Fix: DBGMCU and FLASH macros not implemented yet */
/** @defgroup SYSTEM_LL_EF_DBGMCU DBGMCU
  * @{
  */

/**
  * @brief  Return the device identifier
  * @rmtoll DBGMCU_IDCODE DEV_ID        LL_DBGMCU_GetDeviceID
  * @retval Values between Min_Data=0x00 and Max_Data=0x0FFF (ex: device ID is 0x6415)
  */
__STATIC_INLINE uint32_t LL_DBGMCU_GetDeviceID(void)
{
  return (uint32_t)(READ_BIT(DBGMCU->IDCODE, DBGMCU_IDCODE_DEV_ID));
}

/**
  * @brief  Return the device revision identifier
  * @note This field indicates the revision of the device.
  * @rmtoll DBGMCU_IDCODE REV_ID        LL_DBGMCU_GetRevisionID
  * @retval Values between Min_Data=0x00 and Max_Data=0xFFFF
  */
__STATIC_INLINE uint32_t LL_DBGMCU_GetRevisionID(void)
{
  return (uint32_t)(READ_BIT(DBGMCU->IDCODE, DBGMCU_IDCODE_REV_ID) >> (DBGMCU_REVID_POSITION & 0x1FU));
}

/**
  * @brief  Enable the Debug Module during SLEEP mode
  * @rmtoll DBGMCU_CR    DBG_SLEEP     LL_DBGMCU_EnableDBGSleepMode
  * @retval None
  */
__STATIC_INLINE void LL_DBGMCU_EnableDBGSleepMode(void)
{
  SET_BIT(DBGMCU->CR, DBGMCU_CR_DBG_SLEEP);
}

/**
  * @brief  Disable the Debug Module during SLEEP mode
  * @rmtoll DBGMCU_CR    DBG_SLEEP     LL_DBGMCU_DisableDBGSleepMode
  * @retval None
  */
__STATIC_INLINE void LL_DBGMCU_DisableDBGSleepMode(void)
{
  CLEAR_BIT(DBGMCU->CR, DBGMCU_CR_DBG_SLEEP);
}

/**
  * @brief  Enable the Debug Module during STOP mode
  * @rmtoll DBGMCU_CR    DBG_STOP      LL_DBGMCU_EnableDBGStopMode
  * @retval None
  */
__STATIC_INLINE void LL_DBGMCU_EnableDBGStopMode(void)
{
  SET_BIT(DBGMCU->CR, DBGMCU_CR_DBG_STOP);
}

/**
  * @brief  Disable the Debug Module during STOP mode
  * @rmtoll DBGMCU_CR    DBG_STOP      LL_DBGMCU_DisableDBGStopMode
  * @retval None
  */
__STATIC_INLINE void LL_DBGMCU_DisableDBGStopMode(void)
{
  CLEAR_BIT(DBGMCU->CR, DBGMCU_CR_DBG_STOP);
}

/**
  * @brief  Enable the Debug Module during STANDBY mode
  * @rmtoll DBGMCU_CR    DBG_STANDBY   LL_DBGMCU_EnableDBGStandbyMode
  * @retval None
  */
__STATIC_INLINE void LL_DBGMCU_EnableDBGStandbyMode(void)
{
  SET_BIT(DBGMCU->CR, DBGMCU_CR_DBG_STANDBY);
}

/**
  * @brief  Disable the Debug Module during STANDBY mode
  * @rmtoll DBGMCU_CR    DBG_STANDBY   LL_DBGMCU_DisableDBGStandbyMode
  * @retval None
  */
__STATIC_INLINE void LL_DBGMCU_DisableDBGStandbyMode(void)
{
  CLEAR_BIT(DBGMCU->CR, DBGMCU_CR_DBG_STANDBY);
}

/**
  * @brief  Set Trace pin assignment control
  * @rmtoll DBGMCU_CR    TRACE_IOEN    LL_DBGMCU_SetTracePinAssignment\n
  *         DBGMCU_CR    TRACE_MODE    LL_DBGMCU_SetTracePinAssignment
  * @param  PinAssignment This parameter can be one of the following values:
  *         @arg @ref LL_DBGMCU_TRACE_NONE
  *         @arg @ref LL_DBGMCU_TRACE_ASYNCH
  *         @arg @ref LL_DBGMCU_TRACE_SYNCH_SIZE1
  *         @arg @ref LL_DBGMCU_TRACE_SYNCH_SIZE2
  *         @arg @ref LL_DBGMCU_TRACE_SYNCH_SIZE4
  * @retval None
  */
__STATIC_INLINE void LL_DBGMCU_SetTracePinAssignment(uint32_t PinAssignment)
{
  MODIFY_REG(DBGMCU->CR, DBGMCU_CR_TRACE_IOEN | DBGMCU_CR_TRACE_MODE, PinAssignment);
}

/**
  * @brief  Get Trace pin assignment control
  * @rmtoll DBGMCU_CR    TRACE_IOEN    LL_DBGMCU_GetTracePinAssignment\n
  *         DBGMCU_CR    TRACE_MODE    LL_DBGMCU_GetTracePinAssignment
  * @retval Returned value can be one of the following values:
  *         @arg @ref LL_DBGMCU_TRACE_NONE
  *         @arg @ref LL_DBGMCU_TRACE_ASYNCH
  *         @arg @ref LL_DBGMCU_TRACE_SYNCH_SIZE1
  *         @arg @ref LL_DBGMCU_TRACE_SYNCH_SIZE2
  *         @arg @ref LL_DBGMCU_TRACE_SYNCH_SIZE4
  */
__STATIC_INLINE uint32_t LL_DBGMCU_GetTracePinAssignment(void)
{
  return (uint32_t)(READ_BIT(DBGMCU->CR, DBGMCU_CR_TRACE_IOEN | DBGMCU_CR_TRACE_MODE));
}

/**
  * @brief  Freeze APB1 peripherals (group1 peripherals)
  * @rmtoll DBGMCU_APB1FZR1 DBG_xxxx_STOP  LL_DBGMCU_APB1_GRP1_FreezePeriph
  * @param  Periphs This parameter can be a combination of the following values:
  *         @arg @ref LL_DBGMCU_APB1_GRP1_TIM2_STOP
  *         @arg @ref LL_DBGMCU_APB1_GRP1_TIM3_STOP
  *         @arg @ref LL_DBGMCU_APB1_GRP1_TIM4_STOP
  *         @arg @ref LL_DBGMCU_APB1_GRP1_TIM5_STOP (*)
  *         @arg @ref LL_DBGMCU_APB1_GRP1_TIM6_STOP
  *         @arg @ref LL_DBGMCU_APB1_GRP1_TIM7_STOP
  *         @arg @ref LL_DBGMCU_APB1_GRP1_RTC_STOP
  *         @arg @ref LL_DBGMCU_APB1_GRP1_WWDG_STOP
  *         @arg @ref LL_DBGMCU_APB1_GRP1_IWDG_STOP
  *         @arg @ref LL_DBGMCU_APB1_GRP1_I2C1_STOP
  *         @arg @ref LL_DBGMCU_APB1_GRP1_I2C2_STOP
  *         @arg @ref LL_DBGMCU_APB1_GRP1_I2C3_STOP
  *         @arg @ref LL_DBGMCU_APB1_GRP1_LPTIM1_STOP
  *
  *         (*) value not defined in all devices.
  * @retval None
  */
__STATIC_INLINE void LL_DBGMCU_APB1_GRP1_FreezePeriph(uint32_t Periphs)
{
  SET_BIT(DBGMCU->APB1FZR1, Periphs);
}

/**
  * @brief  Freeze APB1 peripherals (group2 peripherals)
  * @rmtoll DBGMCU_APB1FZR2 DBG_xxxx_STOP  LL_DBGMCU_APB1_GRP2_FreezePeriph
  * @param  Periphs This parameter can be a combination of the following values:
  *         @arg @ref LL_DBGMCU_APB1_GRP2_I2C4_STOP (*)
  *
  *         (*) value not defined in all devices.
  * @retval None
  */
__STATIC_INLINE void LL_DBGMCU_APB1_GRP2_FreezePeriph(uint32_t Periphs)
{
  SET_BIT(DBGMCU->APB1FZR2, Periphs);
}

/**
  * @brief  Unfreeze APB1 peripherals (group1 peripherals)
  * @rmtoll DBGMCU_APB1FZR1 DBG_xxxx_STOP  LL_DBGMCU_APB1_GRP1_UnFreezePeriph
  * @param  Periphs This parameter can be a combination of the following values:
  *         @arg @ref LL_DBGMCU_APB1_GRP1_TIM2_STOP
  *         @arg @ref LL_DBGMCU_APB1_GRP1_TIM3_STOP
  *         @arg @ref LL_DBGMCU_APB1_GRP1_TIM4_STOP
  *         @arg @ref LL_DBGMCU_APB1_GRP1_TIM5_STOP (*)
  *         @arg @ref LL_DBGMCU_APB1_GRP1_TIM6_STOP
  *         @arg @ref LL_DBGMCU_APB1_GRP1_TIM7_STOP
  *         @arg @ref LL_DBGMCU_APB1_GRP1_RTC_STOP
  *         @arg @ref LL_DBGMCU_APB1_GRP1_WWDG_STOP
  *         @arg @ref LL_DBGMCU_APB1_GRP1_IWDG_STOP
  *         @arg @ref LL_DBGMCU_APB1_GRP1_I2C1_STOP
  *         @arg @ref LL_DBGMCU_APB1_GRP1_I2C2_STOP
  *         @arg @ref LL_DBGMCU_APB1_GRP1_I2C3_STOP
  *         @arg @ref LL_DBGMCU_APB1_GRP1_LPTIM1_STOP
  *
  *         (*) value not defined in all devices.
  * @retval None
  */
__STATIC_INLINE void LL_DBGMCU_APB1_GRP1_UnFreezePeriph(uint32_t Periphs)
{
  CLEAR_BIT(DBGMCU->APB1FZR1, Periphs);
}

/**
  * @brief  Unfreeze APB1 peripherals (group2 peripherals)
  * @rmtoll DBGMCU_APB1FZR2 DBG_xxxx_STOP  LL_DBGMCU_APB1_GRP2_UnFreezePeriph
  * @param  Periphs This parameter can be a combination of the following values:
  *         @arg @ref LL_DBGMCU_APB1_GRP2_I2C4_STOP (*)
  *
  *         (*) value not defined in all devices.
  * @retval None
  */
__STATIC_INLINE void LL_DBGMCU_APB1_GRP2_UnFreezePeriph(uint32_t Periphs)
{
  CLEAR_BIT(DBGMCU->APB1FZR2, Periphs);
}

/**
  * @brief  Freeze APB2 peripherals
  * @rmtoll DBGMCU_APB2FZ DBG_TIMx_STOP  LL_DBGMCU_APB2_GRP1_FreezePeriph
  * @param  Periphs This parameter can be a combination of the following values:
  *         @arg @ref LL_DBGMCU_APB2_GRP1_TIM1_STOP
  *         @arg @ref LL_DBGMCU_APB2_GRP1_TIM8_STOP
  *         @arg @ref LL_DBGMCU_APB2_GRP1_TIM15_STOP
  *         @arg @ref LL_DBGMCU_APB2_GRP1_TIM16_STOP
  *         @arg @ref LL_DBGMCU_APB2_GRP1_TIM17_STOP
  *         @arg @ref LL_DBGMCU_APB2_GRP1_TIM20_STOP (*)
  *         @arg @ref LL_DBGMCU_APB2_GRP1_HRTIM1_STOP (*)
  *
  *         (*) value not defined in all devices.
  * @retval None
  */
__STATIC_INLINE void LL_DBGMCU_APB2_GRP1_FreezePeriph(uint32_t Periphs)
{
  SET_BIT(DBGMCU->APB2FZ, Periphs);
}

/**
  * @brief  Unfreeze APB2 peripherals
  * @rmtoll DBGMCU_APB2FZ DBG_TIMx_STOP  LL_DBGMCU_APB2_GRP1_UnFreezePeriph
  * @param  Periphs This parameter can be a combination of the following values:
  *         @arg @ref LL_DBGMCU_APB2_GRP1_TIM1_STOP
  *         @arg @ref LL_DBGMCU_APB2_GRP1_TIM8_STOP
  *         @arg @ref LL_DBGMCU_APB2_GRP1_TIM15_STOP
  *         @arg @ref LL_DBGMCU_APB2_GRP1_TIM16_STOP
  *         @arg @ref LL_DBGMCU_APB2_GRP1_TIM17_STOP
  *         @arg @ref LL_DBGMCU_APB2_GRP1_TIM20_STOP (*)
  *         @arg @ref LL_DBGMCU_APB2_GRP1_HRTIM1_STOP (*)
  *
  *         (*) value not defined in all devices.
  * @retval None
  */
__STATIC_INLINE void LL_DBGMCU_APB2_GRP1_UnFreezePeriph(uint32_t Periphs)
{
  CLEAR_BIT(DBGMCU->APB2FZ, Periphs);
}

/**
  * @}
  */
#endif /* if 0 */

#if 0 /* SR5E1_Fix: DBGMCU and FLASH macros not implemented yet */
/** @defgroup SYSTEM_LL_EF_FLASH FLASH
  * @{
  */

/**
  * @brief  Set FLASH Latency
  * @rmtoll FLASH_ACR    LATENCY       LL_FLASH_SetLatency
  * @param  Latency This parameter can be one of the following values:
  *         @arg @ref LL_FLASH_LATENCY_0
  *         @arg @ref LL_FLASH_LATENCY_1
  *         @arg @ref LL_FLASH_LATENCY_2
  *         @arg @ref LL_FLASH_LATENCY_3
  *         @arg @ref LL_FLASH_LATENCY_4
  *         @arg @ref LL_FLASH_LATENCY_5 (*)
  *         @arg @ref LL_FLASH_LATENCY_6 (*)
  *         @arg @ref LL_FLASH_LATENCY_7 (*)
  *         @arg @ref LL_FLASH_LATENCY_8 (*)
  *         @arg @ref LL_FLASH_LATENCY_9 (*)
  *         @arg @ref LL_FLASH_LATENCY_10 (*)
  *         @arg @ref LL_FLASH_LATENCY_11 (*)
  *         @arg @ref LL_FLASH_LATENCY_12 (*)
  *         @arg @ref LL_FLASH_LATENCY_13 (*)
  *         @arg @ref LL_FLASH_LATENCY_14 (*)
  *         @arg @ref LL_FLASH_LATENCY_15 (*)
  *
  *         (*) value not defined in all devices.
  * @retval None
  */
__STATIC_INLINE void LL_FLASH_SetLatency(uint32_t Latency)
{
  MODIFY_REG(FLASH->ACR, FLASH_ACR_LATENCY, Latency);
}

/**
  * @brief  Get FLASH Latency
  * @rmtoll FLASH_ACR    LATENCY       LL_FLASH_GetLatency
  * @retval Returned value can be one of the following values:
  *         @arg @ref LL_FLASH_LATENCY_0
  *         @arg @ref LL_FLASH_LATENCY_1
  *         @arg @ref LL_FLASH_LATENCY_2
  *         @arg @ref LL_FLASH_LATENCY_3
  *         @arg @ref LL_FLASH_LATENCY_4
  *         @arg @ref LL_FLASH_LATENCY_5 (*)
  *         @arg @ref LL_FLASH_LATENCY_6 (*)
  *         @arg @ref LL_FLASH_LATENCY_7 (*)
  *         @arg @ref LL_FLASH_LATENCY_8 (*)
  *         @arg @ref LL_FLASH_LATENCY_9 (*)
  *         @arg @ref LL_FLASH_LATENCY_10 (*)
  *         @arg @ref LL_FLASH_LATENCY_11 (*)
  *         @arg @ref LL_FLASH_LATENCY_12 (*)
  *         @arg @ref LL_FLASH_LATENCY_13 (*)
  *         @arg @ref LL_FLASH_LATENCY_14 (*)
  *         @arg @ref LL_FLASH_LATENCY_15 (*)
  *
  *         (*) value not defined in all devices.
  */
__STATIC_INLINE uint32_t LL_FLASH_GetLatency(void)
{
  return (uint32_t)(READ_BIT(FLASH->ACR, FLASH_ACR_LATENCY));
}

/**
  * @brief  Enable Prefetch
  * @rmtoll FLASH_ACR    PRFTEN        LL_FLASH_EnablePrefetch
  * @retval None
  */
__STATIC_INLINE void LL_FLASH_EnablePrefetch(void)
{
  SET_BIT(FLASH->ACR, FLASH_ACR_PRFTEN);
}

/**
  * @brief  Disable Prefetch
  * @rmtoll FLASH_ACR    PRFTEN        LL_FLASH_DisablePrefetch
  * @retval None
  */
__STATIC_INLINE void LL_FLASH_DisablePrefetch(void)
{
  CLEAR_BIT(FLASH->ACR, FLASH_ACR_PRFTEN);
}

/**
  * @brief  Check if Prefetch buffer is enabled
  * @rmtoll FLASH_ACR    PRFTEN        LL_FLASH_IsPrefetchEnabled
  * @retval State of bit (1 or 0).
  */
__STATIC_INLINE uint32_t LL_FLASH_IsPrefetchEnabled(void)
{
  return ((READ_BIT(FLASH->ACR, FLASH_ACR_PRFTEN) == (FLASH_ACR_PRFTEN)) ? 1UL : 0UL);
}

/**
  * @brief  Enable Instruction cache
  * @rmtoll FLASH_ACR    ICEN          LL_FLASH_EnableInstCache
  * @retval None
  */
__STATIC_INLINE void LL_FLASH_EnableInstCache(void)
{
  SET_BIT(FLASH->ACR, FLASH_ACR_ICEN);
}

/**
  * @brief  Disable Instruction cache
  * @rmtoll FLASH_ACR    ICEN          LL_FLASH_DisableInstCache
  * @retval None
  */
__STATIC_INLINE void LL_FLASH_DisableInstCache(void)
{
  CLEAR_BIT(FLASH->ACR, FLASH_ACR_ICEN);
}

/**
  * @brief  Enable Data cache
  * @rmtoll FLASH_ACR    DCEN          LL_FLASH_EnableDataCache
  * @retval None
  */
__STATIC_INLINE void LL_FLASH_EnableDataCache(void)
{
  SET_BIT(FLASH->ACR, FLASH_ACR_DCEN);
}

/**
  * @brief  Disable Data cache
  * @rmtoll FLASH_ACR    DCEN          LL_FLASH_DisableDataCache
  * @retval None
  */
__STATIC_INLINE void LL_FLASH_DisableDataCache(void)
{
  CLEAR_BIT(FLASH->ACR, FLASH_ACR_DCEN);
}

/**
  * @brief  Enable Instruction cache reset
  * @note  bit can be written only when the instruction cache is disabled
  * @rmtoll FLASH_ACR    ICRST         LL_FLASH_EnableInstCacheReset
  * @retval None
  */
__STATIC_INLINE void LL_FLASH_EnableInstCacheReset(void)
{
  SET_BIT(FLASH->ACR, FLASH_ACR_ICRST);
}

/**
  * @brief  Disable Instruction cache reset
  * @rmtoll FLASH_ACR    ICRST         LL_FLASH_DisableInstCacheReset
  * @retval None
  */
__STATIC_INLINE void LL_FLASH_DisableInstCacheReset(void)
{
  CLEAR_BIT(FLASH->ACR, FLASH_ACR_ICRST);
}

/**
  * @brief  Enable Data cache reset
  * @note bit can be written only when the data cache is disabled
  * @rmtoll FLASH_ACR    DCRST         LL_FLASH_EnableDataCacheReset
  * @retval None
  */
__STATIC_INLINE void LL_FLASH_EnableDataCacheReset(void)
{
  SET_BIT(FLASH->ACR, FLASH_ACR_DCRST);
}

/**
  * @brief  Disable Data cache reset
  * @rmtoll FLASH_ACR    DCRST         LL_FLASH_DisableDataCacheReset
  * @retval None
  */
__STATIC_INLINE void LL_FLASH_DisableDataCacheReset(void)
{
  CLEAR_BIT(FLASH->ACR, FLASH_ACR_DCRST);
}

/**
  * @brief  Enable Flash Power-down mode during run mode or Low-power run mode
  * @note Flash memory can be put in power-down mode only when the code is executed
  *       from RAM
  * @note Flash must not be accessed when power down is enabled
  * @note Flash must not be put in power-down while a program or an erase operation
  *       is on-going
  * @rmtoll FLASH_ACR    RUN_PD        LL_FLASH_EnableRunPowerDown\n
  *         FLASH_PDKEYR PDKEY1        LL_FLASH_EnableRunPowerDown\n
  *         FLASH_PDKEYR PDKEY2        LL_FLASH_EnableRunPowerDown
  * @retval None
  */
__STATIC_INLINE void LL_FLASH_EnableRunPowerDown(void)
{
  /* Following values must be written consecutively to unlock the RUN_PD bit in
     FLASH_ACR */
  WRITE_REG(FLASH->PDKEYR, FLASH_PDKEY1);
  WRITE_REG(FLASH->PDKEYR, FLASH_PDKEY2);
  SET_BIT(FLASH->ACR, FLASH_ACR_RUN_PD);
}

/**
  * @brief  Disable Flash Power-down mode during run mode or Low-power run mode
  * @rmtoll FLASH_ACR    RUN_PD        LL_FLASH_DisableRunPowerDown\n
  *         FLASH_PDKEYR PDKEY1        LL_FLASH_DisableRunPowerDown\n
  *         FLASH_PDKEYR PDKEY2        LL_FLASH_DisableRunPowerDown
  * @retval None
  */
__STATIC_INLINE void LL_FLASH_DisableRunPowerDown(void)
{
  /* Following values must be written consecutively to unlock the RUN_PD bit in
     FLASH_ACR */
  WRITE_REG(FLASH->PDKEYR, FLASH_PDKEY1);
  WRITE_REG(FLASH->PDKEYR, FLASH_PDKEY2);
  CLEAR_BIT(FLASH->ACR, FLASH_ACR_RUN_PD);
}

/**
  * @brief  Enable Flash Power-down mode during Sleep or Low-power sleep mode
  * @note Flash must not be put in power-down while a program or an erase operation
  *       is on-going
  * @rmtoll FLASH_ACR    SLEEP_PD      LL_FLASH_EnableSleepPowerDown
  * @retval None
  */
__STATIC_INLINE void LL_FLASH_EnableSleepPowerDown(void)
{
  SET_BIT(FLASH->ACR, FLASH_ACR_SLEEP_PD);
}

/**
  * @brief  Disable Flash Power-down mode during Sleep or Low-power sleep mode
  * @rmtoll FLASH_ACR    SLEEP_PD      LL_FLASH_DisableSleepPowerDown
  * @retval None
  */
__STATIC_INLINE void LL_FLASH_DisableSleepPowerDown(void)
{
  CLEAR_BIT(FLASH->ACR, FLASH_ACR_SLEEP_PD);
}

/**
  * @}
  */
#endif /* if 0 */

/**
  * @}
  */

/**
  * @}
  */

#endif /* defined (FLASH) || defined (SYSCFG) || defined (DBGMCU) */

/**
  * @}
  */

#ifdef __cplusplus
}
#endif

#endif /* __SR5E1_LL_SYSTEM_H */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
