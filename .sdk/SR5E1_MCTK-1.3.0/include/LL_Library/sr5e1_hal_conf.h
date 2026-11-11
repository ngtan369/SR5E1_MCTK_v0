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
*   @file    sr5e1_hal_conf.h
*   @version M4_VERSION
*   @brief   HAL configuration file
*
*   @details HAL configuration file
*
*
 */
/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef SR5E1_HAL_CONF_H
#define SR5E1_HAL_CONF_H

#ifdef __cplusplus
 extern "C" {
#endif

/* Should be uncomment. Needed to use HAL FDCAN (CAN_COMMUNICATION) */
#define HAL_GetTick() osal_get_tick()

/* Exported types ------------------------------------------------------------*/
/* Exported constants --------------------------------------------------------*/

/* ########################## Module Selection ############################## */
/**
  * @brief This is the list of modules to be used in the HAL driver
  */

#define HAL_MODULE_ENABLED

  /*#define HAL_ADC_MODULE_ENABLED   */
/*#define HAL_COMP_MODULE_ENABLED   */
/*#define HAL_CORDIC_MODULE_ENABLED   */
/*#define HAL_CRC_MODULE_ENABLED   */
/*#define HAL_CRYP_MODULE_ENABLED   */
/*#define HAL_DAC_MODULE_ENABLED   */
#define HAL_FDCAN_MODULE_ENABLED   
/*#define HAL_FMAC_MODULE_ENABLED   */
/*#define HAL_HRTIM_MODULE_ENABLED   */
/*#define HAL_IRDA_MODULE_ENABLED   */
/*#define HAL_IWDG_MODULE_ENABLED   */
/*#define HAL_I2C_MODULE_ENABLED   */
/*#define HAL_I2S_MODULE_ENABLED   */
/*#define HAL_LPTIM_MODULE_ENABLED   */
/*#define HAL_NAND_MODULE_ENABLED   */
/*#define HAL_NOR_MODULE_ENABLED   */
/*#define HAL_OPAMP_MODULE_ENABLED   */
/*#define HAL_PCD_MODULE_ENABLED   */
/*#define HAL_QSPI_MODULE_ENABLED   */
/*#define HAL_RNG_MODULE_ENABLED   */
/*#define HAL_RTC_MODULE_ENABLED   */
/*#define HAL_SAI_MODULE_ENABLED   */
/*#define HAL_SMARTCARD_MODULE_ENABLED   */
/*#define HAL_SMBUS_MODULE_ENABLED   */
/*#define HAL_SPI_MODULE_ENABLED   */
/*#define HAL_SRAM_MODULE_ENABLED   */
/*#define HAL_TIM_MODULE_ENABLED   */
/*#define HAL_UART_MODULE_ENABLED   */
/*#define HAL_USART_MODULE_ENABLED   */
/*#define HAL_WWDG_MODULE_ENABLED   */
/*#define HAL_GPIO_MODULE_ENABLED */
/*#define HAL_EXTI_MODULE_ENABLED */
/*#define HAL_DMA_MODULE_ENABLED */
/*#define HAL_RCC_MODULE_ENABLED */
/*#define HAL_FLASH_MODULE_ENABLED */
/*#define HAL_PWR_MODULE_ENABLED */
/*#define HAL_CORTEX_MODULE_ENABLED */

/* ########################## Register Callbacks selection ############################## */
/**
  * @brief This is the list of modules where register callback can be used
  */
#define USE_HAL_ADC_REGISTER_CALLBACKS        0U
#define USE_HAL_COMP_REGISTER_CALLBACKS       0U
#define USE_HAL_CORDIC_REGISTER_CALLBACKS     0U
#define USE_HAL_CRYP_REGISTER_CALLBACKS       0U
#define USE_HAL_DAC_REGISTER_CALLBACKS        0U
#define USE_HAL_EXTI_REGISTER_CALLBACKS       0U
#define USE_HAL_FDCAN_REGISTER_CALLBACKS      0U
#define USE_HAL_FMAC_REGISTER_CALLBACKS       0U
#define USE_HAL_HRTIM_REGISTER_CALLBACKS      0U
#define USE_HAL_I2C_REGISTER_CALLBACKS        0U
#define USE_HAL_I2S_REGISTER_CALLBACKS        0U
#define USE_HAL_IRDA_REGISTER_CALLBACKS       0U
#define USE_HAL_LPTIM_REGISTER_CALLBACKS      0U
#define USE_HAL_NAND_REGISTER_CALLBACKS       0U
#define USE_HAL_NOR_REGISTER_CALLBACKS        0U
#define USE_HAL_OPAMP_REGISTER_CALLBACKS      0U
#define USE_HAL_PCD_REGISTER_CALLBACKS        0U
#define USE_HAL_QSPI_REGISTER_CALLBACKS       0U
#define USE_HAL_RNG_REGISTER_CALLBACKS        0U
#define USE_HAL_RTC_REGISTER_CALLBACKS        0U
#define USE_HAL_SAI_REGISTER_CALLBACKS        0U
#define USE_HAL_SMARTCARD_REGISTER_CALLBACKS  0U
#define USE_HAL_SMBUS_REGISTER_CALLBACKS      0U
#define USE_HAL_SPI_REGISTER_CALLBACKS        0U
#define USE_HAL_SRAM_REGISTER_CALLBACKS       0U
#define USE_HAL_TIM_REGISTER_CALLBACKS        0U
#define USE_HAL_UART_REGISTER_CALLBACKS       0U
#define USE_HAL_USART_REGISTER_CALLBACKS      0U
#define USE_HAL_WWDG_REGISTER_CALLBACKS       0U

/* ########################## Oscillator Values adaptation ####################*/
/**
  * @brief Adjust the value of External High Speed oscillator (HSE) used in your application.
  *        This value is used by the RCC HAL module to compute the system frequency
  *        (when HSE is used as system clock source, directly or through the PLL).
  */
#if !defined  (HSE_VALUE)
  #define HSE_VALUE    (24000000UL) /*!< Value of the External oscillator in Hz */
#endif /* HSE_VALUE */

#if !defined  (HSE_STARTUP_TIMEOUT)
  #define HSE_STARTUP_TIMEOUT    (100UL)   /*!< Time out for HSE start up, in ms */
#endif /* HSE_STARTUP_TIMEOUT */

/**
  * @brief Internal High Speed oscillator (HSI) value.
  *        This value is used by the RCC HAL module to compute the system frequency
  *        (when HSI is used as system clock source, directly or through the PLL).
  */
#if !defined  (HSI_VALUE)
  #define HSI_VALUE    (16000000UL) /*!< Value of the Internal oscillator in Hz*/
#endif /* HSI_VALUE */

/**
  * @brief Internal High Speed oscillator (HSI48) value for USB FS and RNG.
  *        This internal oscillator is mainly dedicated to provide a high precision clock to
  *        the USB peripheral by means of a special Clock Recovery System (CRS) circuitry.
  *        When the CRS is not used, the HSI48 RC oscillator runs on it default frequency
  *        which is subject to manufacturing process variations.
  */
#if !defined  (HSI48_VALUE)
  #define HSI48_VALUE   (48000000UL) /*!< Value of the Internal High Speed oscillator for USB FS/RNG in Hz.
                                               The real value my vary depending on manufacturing process variations.*/
#endif /* HSI48_VALUE */

/**
  * @brief Internal Low Speed oscillator (LSI) value.
  */
#if !defined  (LSI_VALUE)
/*!< Value of the Internal Low Speed oscillator in Hz
The real value may vary depending on the variations in voltage and temperature.*/
#define LSI_VALUE  (32000UL)     /*!< LSI Typical Value in Hz*/
#endif /* LSI_VALUE */
/**
  * @brief External Low Speed oscillator (LSE) value.
  *        This value is used by the UART, RTC HAL module to compute the system frequency
  */
#if !defined  (LSE_VALUE)
#define LSE_VALUE  (32768UL)    /*!< Value of the External Low Speed oscillator in Hz */
#endif /* LSE_VALUE */

#if !defined  (LSE_STARTUP_TIMEOUT)
#define LSE_STARTUP_TIMEOUT    (5000UL)   /*!< Time out for LSE start up, in ms */
#endif /* LSE_STARTUP_TIMEOUT */

/**
  * @brief External clock source for I2S and SAI peripherals
  *        This value is used by the I2S and SAI HAL modules to compute the I2S and SAI clock source
  *        frequency, this source is inserted directly through I2S_CKIN pad.
  */
#if !defined  (EXTERNAL_CLOCK_VALUE)
#define EXTERNAL_CLOCK_VALUE    (12288000UL) /*!< Value of the External oscillator in Hz*/
#endif /* EXTERNAL_CLOCK_VALUE */

/* Tip: To avoid modifying this file each time you need to use different HSE,
   ===  you can define the HSE value in your toolchain compiler preprocessor. */

/* ########################### System Configuration ######################### */
/**
  * @brief This is the HAL system configuration section
  */

#define  VDD_VALUE                   (3300UL) /*!< Value of VDD in mv */
#define  TICK_INT_PRIORITY           (4UL)    /*!< tick interrupt priority (lowest by default)  */
#define  USE_RTOS                     0U
#define  PREFETCH_ENABLE              0U
#define  INSTRUCTION_CACHE_ENABLE     1U
#define  DATA_CACHE_ENABLE            1U

/* ########################## Assert Selection ############################## */
/**
  * @brief Uncomment the line below to expanse the "assert_param" macro in the
  *        HAL drivers code
  */
/* #define USE_FULL_ASSERT    1U */

/* ################## SPI peripheral configuration ########################## */

/* CRC FEATURE: Use to activate CRC feature inside HAL SPI Driver
 * Activated: CRC code is present inside driver
 * Deactivated: CRC code cleaned from driver
 */

#define USE_SPI_CRC                   0U

/* Includes ------------------------------------------------------------------*/
/**
  * @brief Include module's header file
  */

#ifdef HAL_FDCAN_MODULE_ENABLED
#include "sr5e1_hal_fdcan.h"
#endif /* HAL_FDCAN_MODULE_ENABLED */

/* Exported macro ------------------------------------------------------------*/
#ifdef  USE_FULL_ASSERT
/**
  * @brief  The assert_param macro is used for function's parameters check.
  * @param  expr: If expr is false, it calls assert_failed function
  *         which reports the name of the source file and the source
  *         line number of the call that failed.
  *         If expr is true, it returns no value.
  * @retval None
  */
#define assert_param(expr) ((expr) ? (void)0U : assert_failed((uint8_t *)__FILE__, __LINE__))
/* Exported functions ------------------------------------------------------- */
void assert_failed(uint8_t *file, uint32_t line);
#else
#define assert_param(expr) ((void)0U)
#endif /* USE_FULL_ASSERT */

#ifdef __cplusplus
}
#endif

#endif /* SR5E1_HAL_CONF_H */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
