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
*   @file    sr5e1_ll_exti.h
*   @version M4_VERSION
*   @brief   Header file of EXTI LL module
*
*   @details Header file of EXTI LL module
*
 */
/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __SR5E1_LL_EXTI_H_
#define __SR5E1_LL_EXTI_H_

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "platform.h"
#include "sr5e1_macros.h"

/** @addtogroup SR5E1_LL_Driver
  * @{
  */


/** @defgroup EXTI_LL EXTI
  * @{
  */

/* Private types -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private constants ---------------------------------------------------------*/
/* Private Macros ------------------------------------------------------------*/
/** @defgroup EXTI_LL_Private_Macros EXTI Private Macros
  * @{
  */
/**
  * @}
  */
/* Exported types ------------------------------------------------------------*/
/** @defgroup EXTI_LL_ES_INIT EXTI Exported Init structure
  * @{
  */
typedef struct
{

  uint32_t Line_0_31;           /*!< Specifies the EXTI lines to be enabled or disabled for Lines in range 0 to 31
                                     This parameter can be any combination of @ref EXTI_LL_EC_LINE */

  uint32_t Line_32_63;          /*!< Specifies the EXTI lines to be enabled or disabled for Lines in range 32 to 63
                                     This parameter can be any combination of @ref EXTI_LL_EC_LINE */

  FunctionalState LineCommand;  /*!< Specifies the new state of the selected EXTI lines.
                                     This parameter can be set either to ENABLE or DISABLE */

  uint8_t Mode;                 /*!< Specifies the mode for the EXTI lines.
                                     This parameter can be a value of @ref EXTI_LL_EC_MODE. */

  uint8_t Trigger;              /*!< Specifies the trigger signal active edge for the EXTI lines.
                                     This parameter can be a value of @ref EXTI_LL_EC_TRIGGER. */
} LL_EXTI_InitTypeDef;

/**
  * @}
  */

/* Exported constants --------------------------------------------------------*/
/** @defgroup EXTI_LL_Exported_Constants EXTI Exported Constants
  * @{
  */

/** @defgroup EXTI_LL_EC_LINE LINE
  * @{
  */
#define LL_EXTI_LINE_0                 EXTI_IMR1_IM0           /*!< Extended line 0 */
#define LL_EXTI_LINE_1                 EXTI_IMR1_IM1           /*!< Extended line 1 */
#define LL_EXTI_LINE_2                 EXTI_IMR1_IM2           /*!< Extended line 2 */
#define LL_EXTI_LINE_3                 EXTI_IMR1_IM3           /*!< Extended line 3 */
#define LL_EXTI_LINE_4                 EXTI_IMR1_IM4           /*!< Extended line 4 */
#define LL_EXTI_LINE_5                 EXTI_IMR1_IM5           /*!< Extended line 5 */
#define LL_EXTI_LINE_6                 EXTI_IMR1_IM6           /*!< Extended line 6 */
#define LL_EXTI_LINE_7                 EXTI_IMR1_IM7           /*!< Extended line 7 */
#define LL_EXTI_LINE_8                 EXTI_IMR1_IM8           /*!< Extended line 8 */
#define LL_EXTI_LINE_9                 EXTI_IMR1_IM9           /*!< Extended line 9 */
#define LL_EXTI_LINE_10                EXTI_IMR1_IM10          /*!< Extended line 10 */
#define LL_EXTI_LINE_11                EXTI_IMR1_IM11          /*!< Extended line 11 */
#define LL_EXTI_LINE_12                EXTI_IMR1_IM12          /*!< Extended line 12 */
#define LL_EXTI_LINE_13                EXTI_IMR1_IM13          /*!< Extended line 13 */
#define LL_EXTI_LINE_14                EXTI_IMR1_IM14          /*!< Extended line 14 */
#define LL_EXTI_LINE_15                EXTI_IMR1_IM15          /*!< Extended line 15 */
#define LL_EXTI_LINE_18                EXTI_IMR1_IM18          /*!< Extended line 18 */
#define LL_EXTI_LINE_20                EXTI_IMR1_IM20          /*!< Extended line 20 */
#define LL_EXTI_LINE_21                EXTI_IMR1_IM21          /*!< Extended line 21 */
#define LL_EXTI_LINE_22                EXTI_IMR1_IM22          /*!< Extended line 22 */
#define LL_EXTI_LINE_23                EXTI_IMR1_IM23          /*!< Extended line 23 */
#define LL_EXTI_LINE_24                EXTI_IMR1_IM24          /*!< Extended line 24 */
#define LL_EXTI_LINE_25                EXTI_IMR1_IM25          /*!< Extended line 25 */
#define LL_EXTI_LINE_26                EXTI_IMR1_IM26          /*!< Extended line 26 */
#define LL_EXTI_LINE_28                EXTI_IMR1_IM28          /*!< Extended line 28 */
#define LL_EXTI_LINE_29                EXTI_IMR1_IM29          /*!< Extended line 29 */
#define LL_EXTI_LINE_30                EXTI_IMR1_IM30          /*!< Extended line 30 */
#define LL_EXTI_LINE_31                EXTI_IMR1_IM31          /*!< Extended line 31 */
#define LL_EXTI_LINE_ALL_0_31          EXTI_IMR1_IM            /*!< All Extended line not reserved*/

#define LL_EXTI_LINE_32                EXTI_IMR2_IM32          /*!< Extended line 32 */
#define LL_EXTI_LINE_33                EXTI_IMR2_IM33          /*!< Extended line 33 */
#define LL_EXTI_LINE_34                EXTI_IMR2_IM34          /*!< Extended line 34 */
#define LL_EXTI_LINE_44                EXTI_IMR2_IM44          /*!< Extended line 44 */
#define LL_EXTI_LINE_45                EXTI_IMR2_IM45          /*!< Extended line 45 */
#define LL_EXTI_LINE_ALL_32_63         EXTI_IMR2_IM            /*!< All Extended line not reserved*/


#define LL_EXTI_LINE_ALL               (0xFFFFFFFFU)  /*!< All Extended line */

#define LL_EXTI_LINE_NONE              (0x00000000U)  /*!< None Extended line */

/**
  * @}
  */

/** @defgroup EXTI_LL_EC_MODE Mode
  * @{
  */
#define LL_EXTI_MODE_IT               ((uint8_t)0x01U)      /*!< Cortex-M7 Interrupt Mode  */
#define LL_EXTI_MODE_EVENT            ((uint8_t)0x02U)      /*!< Cortex-M7 Event Mode */
#define LL_EXTI_MODE_IT_EVENT         ((uint8_t)0x03U)      /*!< Cortex-M7 Interrupt & Event Mode */

#define LL_EXTI_MODE_C1_IT            LL_EXTI_MODE_IT       /*!< Cortex-M7 Interrupt Mode */
#define LL_EXTI_MODE_C1_EVENT         LL_EXTI_MODE_EVENT    /*!< Cortex-M7 Event Mode */
#define LL_EXTI_MODE_C1_IT_EVENT      LL_EXTI_MODE_IT_EVENT /*!< Cortex-M7 Interrupt & Event Mode */

#define LL_EXTI_MODE_C2_IT            ((uint8_t)0x10U)      /*!< Cortex-M4 Interrupt Mode  */
#define LL_EXTI_MODE_C2_EVENT         ((uint8_t)0x20U)      /*!< Cortex-M4 Event Mode  */
#define LL_EXTI_MODE_C2_IT_EVENT      ((uint8_t)0x30U)      /*!< Cortex-M4 Interrupt & Event Mode */

/**
  * @}
  */

/** @defgroup EXTI_LL_EC_TRIGGER Edge Trigger
  * @{
  */
#define LL_EXTI_TRIGGER_NONE            ((uint8_t)0x00U) /*!< No Trigger Mode */
#define LL_EXTI_TRIGGER_RISING          ((uint8_t)0x01U) /*!< Trigger Rising Mode */
#define LL_EXTI_TRIGGER_FALLING         ((uint8_t)0x02U) /*!< Trigger Falling Mode */
#define LL_EXTI_TRIGGER_RISING_FALLING  ((uint8_t)0x03U) /*!< Trigger Rising & Falling Mode */

/**
  * @}
  */


/**
  * @}
  */

/* Exported macro ------------------------------------------------------------*/
/** @defgroup EXTI_LL_Exported_Macros EXTI Exported Macros
  * @{
  */

/** @defgroup EXTI_LL_EM_WRITE_READ Common Write and read registers Macros
  * @{
  */

/**
  * @brief  Write a value in EXTI register
  * @param  __REG__ Register to be written
  * @param  __VALUE__ Value to be written in the register
  * @retval None
  */
#define LL_EXTI_WriteReg(__REG__, __VALUE__) WRITE_REG(EXTI->__REG__, (__VALUE__))

/**
  * @brief  Read a value in EXTI register
  * @param  __REG__ Register to be read
  * @retval Register value
  */
#define LL_EXTI_ReadReg(__REG__) READ_REG(EXTI->__REG__)

/**
  * @}
  */


/**
  * @}
  */



/* Exported functions --------------------------------------------------------*/
/** @defgroup EXTI_LL_Exported_Functions EXTI Exported Functions
 * @{
 */
/** @defgroup EXTI_LL_EF_IT_Management IT_Management
  * @{
  */

/**
  * @brief  Enable ExtiLine Interrupt request for Lines in range 0 to 31
  * @rmtoll IMR1         IMx           LL_EXTI_EnableIT_0_31
  * @param  ExtiLine This parameter can be one of the following values:
  *         @arg @ref LL_EXTI_LINE_0
  *         @arg @ref LL_EXTI_LINE_1
  *         @arg @ref LL_EXTI_LINE_2
  *         @arg @ref LL_EXTI_LINE_3
  *         @arg @ref LL_EXTI_LINE_4
  *         @arg @ref LL_EXTI_LINE_5
  *         @arg @ref LL_EXTI_LINE_6
  *         @arg @ref LL_EXTI_LINE_7
  *         @arg @ref LL_EXTI_LINE_8
  *         @arg @ref LL_EXTI_LINE_9
  *         @arg @ref LL_EXTI_LINE_10
  *         @arg @ref LL_EXTI_LINE_11
  *         @arg @ref LL_EXTI_LINE_12
  *         @arg @ref LL_EXTI_LINE_13
  *         @arg @ref LL_EXTI_LINE_14
  *         @arg @ref LL_EXTI_LINE_15
  *         @arg @ref LL_EXTI_LINE_18
  *         @arg @ref LL_EXTI_LINE_20
  *         @arg @ref LL_EXTI_LINE_21
  *         @arg @ref LL_EXTI_LINE_22
  *         @arg @ref LL_EXTI_LINE_23
  *         @arg @ref LL_EXTI_LINE_24
  *         @arg @ref LL_EXTI_LINE_25
  *         @arg @ref LL_EXTI_LINE_26
  *         @arg @ref LL_EXTI_LINE_28
  *         @arg @ref LL_EXTI_LINE_29
  *         @arg @ref LL_EXTI_LINE_30
  *         @arg @ref LL_EXTI_LINE_31
  *         @arg @ref LL_EXTI_LINE_ALL_0_31
  * @retval None
  */
__STATIC_INLINE void LL_EXTI_EnableIT_0_31(uint32_t ExtiLine)
{
  SET_BIT(EXTI->C1IMR1, ExtiLine);
}

/**
  * @brief  Enable ExtiLine Interrupt request for Lines in range 32 to 63
  * @rmtoll IMR2         IMx           LL_EXTI_EnableIT_32_63
  * @param  ExtiLine This parameter can be one of the following values:
  *         @arg @ref LL_EXTI_LINE_32
  *         @arg @ref LL_EXTI_LINE_33
  *         @arg @ref LL_EXTI_LINE_34
  *         @arg @ref LL_EXTI_LINE_44
  *         @arg @ref LL_EXTI_LINE_ALL_32_63
  * @retval None
  */
__STATIC_INLINE void LL_EXTI_EnableIT_32_63(uint32_t ExtiLine)
{
  SET_BIT(EXTI->C1IMR2, ExtiLine);
}

/**
  * @brief  Disable ExtiLine Interrupt request for Lines in range 0 to 31
  * @rmtoll IMR1         IMx           LL_EXTI_DisableIT_0_31
  * @param  ExtiLine This parameter can be one of the following values:
  *         @arg @ref LL_EXTI_LINE_0
  *         @arg @ref LL_EXTI_LINE_1
  *         @arg @ref LL_EXTI_LINE_2
  *         @arg @ref LL_EXTI_LINE_3
  *         @arg @ref LL_EXTI_LINE_4
  *         @arg @ref LL_EXTI_LINE_5
  *         @arg @ref LL_EXTI_LINE_6
  *         @arg @ref LL_EXTI_LINE_7
  *         @arg @ref LL_EXTI_LINE_8
  *         @arg @ref LL_EXTI_LINE_9
  *         @arg @ref LL_EXTI_LINE_10
  *         @arg @ref LL_EXTI_LINE_11
  *         @arg @ref LL_EXTI_LINE_12
  *         @arg @ref LL_EXTI_LINE_13
  *         @arg @ref LL_EXTI_LINE_14
  *         @arg @ref LL_EXTI_LINE_15
  *         @arg @ref LL_EXTI_LINE_18
  *         @arg @ref LL_EXTI_LINE_20
  *         @arg @ref LL_EXTI_LINE_21
  *         @arg @ref LL_EXTI_LINE_22
  *         @arg @ref LL_EXTI_LINE_23
  *         @arg @ref LL_EXTI_LINE_24
  *         @arg @ref LL_EXTI_LINE_25
  *         @arg @ref LL_EXTI_LINE_26
  *         @arg @ref LL_EXTI_LINE_28
  *         @arg @ref LL_EXTI_LINE_29
  *         @arg @ref LL_EXTI_LINE_30
  *         @arg @ref LL_EXTI_LINE_31
  *         @arg @ref LL_EXTI_LINE_ALL_0_31
  * @retval None
  */
__STATIC_INLINE void LL_EXTI_DisableIT_0_31(uint32_t ExtiLine)
{
  CLEAR_BIT(EXTI->C1IMR1, ExtiLine);
}

/**
  * @brief  Disable ExtiLine Interrupt request for Lines in range 32 to 63
  * @rmtoll IMR2         IMx           LL_EXTI_DisableIT_32_63
  * @param  ExtiLine This parameter can be one of the following values:
  *         @arg @ref LL_EXTI_LINE_32
  *         @arg @ref LL_EXTI_LINE_33
  *         @arg @ref LL_EXTI_LINE_34
  *         @arg @ref LL_EXTI_LINE_44
  *         @arg @ref LL_EXTI_LINE_ALL_32_63
  * @retval None
  */
__STATIC_INLINE void LL_EXTI_DisableIT_32_63(uint32_t ExtiLine)
{
  CLEAR_BIT(EXTI->C1IMR2, ExtiLine);
}

/**
  * @brief  Indicate if ExtiLine Interrupt request is enabled for Lines in range 0 to 31
  * @rmtoll IMR1         IMx           LL_EXTI_IsEnabledIT_0_31
  * @param  ExtiLine This parameter can be one of the following values:
  *         @arg @ref LL_EXTI_LINE_0
  *         @arg @ref LL_EXTI_LINE_1
  *         @arg @ref LL_EXTI_LINE_2
  *         @arg @ref LL_EXTI_LINE_3
  *         @arg @ref LL_EXTI_LINE_4
  *         @arg @ref LL_EXTI_LINE_5
  *         @arg @ref LL_EXTI_LINE_6
  *         @arg @ref LL_EXTI_LINE_7
  *         @arg @ref LL_EXTI_LINE_8
  *         @arg @ref LL_EXTI_LINE_9
  *         @arg @ref LL_EXTI_LINE_10
  *         @arg @ref LL_EXTI_LINE_11
  *         @arg @ref LL_EXTI_LINE_12
  *         @arg @ref LL_EXTI_LINE_13
  *         @arg @ref LL_EXTI_LINE_14
  *         @arg @ref LL_EXTI_LINE_15
  *         @arg @ref LL_EXTI_LINE_18
  *         @arg @ref LL_EXTI_LINE_20
  *         @arg @ref LL_EXTI_LINE_21
  *         @arg @ref LL_EXTI_LINE_22
  *         @arg @ref LL_EXTI_LINE_23
  *         @arg @ref LL_EXTI_LINE_24
  *         @arg @ref LL_EXTI_LINE_25
  *         @arg @ref LL_EXTI_LINE_26
  *         @arg @ref LL_EXTI_LINE_28
  *         @arg @ref LL_EXTI_LINE_29
  *         @arg @ref LL_EXTI_LINE_30
  *         @arg @ref LL_EXTI_LINE_31
  *         @arg @ref LL_EXTI_LINE_ALL_0_31
  * @retval State of bit (1 or 0).
  */
__STATIC_INLINE uint32_t LL_EXTI_IsEnabledIT_0_31(uint32_t ExtiLine)
{
  return ((READ_BIT(EXTI->C1IMR1, ExtiLine) == (ExtiLine)) ? 1UL : 0UL);
}

/**
  * @brief  Indicate if ExtiLine Interrupt request is enabled for Lines in range 32 to 63
  * @rmtoll IMR2         IMx           LL_EXTI_IsEnabledIT_32_63
  * @param  ExtiLine This parameter can be one of the following values:
  *         @arg @ref LL_EXTI_LINE_32
  *         @arg @ref LL_EXTI_LINE_33
  *         @arg @ref LL_EXTI_LINE_34
  *         @arg @ref LL_EXTI_LINE_44
  *         @arg @ref LL_EXTI_LINE_ALL_32_63
  * @retval State of bit (1 or 0).
  */
__STATIC_INLINE uint32_t LL_EXTI_IsEnabledIT_32_63(uint32_t ExtiLine)
{
  return ((READ_BIT(EXTI->C1IMR2, ExtiLine) == (ExtiLine)) ? 1UL : 0UL);
}

/**
  * @brief  Enable ExtiLine Interrupt request for Lines in range 0 to 31 for cpu2
  * @rmtoll C2IMR1         IMx           LL_C2_EXTI_EnableIT_0_31
  * @param  ExtiLine This parameter can be one of the following values:
  *         @arg @ref LL_EXTI_LINE_0
  *         @arg @ref LL_EXTI_LINE_1
  *         @arg @ref LL_EXTI_LINE_2
  *         @arg @ref LL_EXTI_LINE_3
  *         @arg @ref LL_EXTI_LINE_4
  *         @arg @ref LL_EXTI_LINE_5
  *         @arg @ref LL_EXTI_LINE_6
  *         @arg @ref LL_EXTI_LINE_7
  *         @arg @ref LL_EXTI_LINE_8
  *         @arg @ref LL_EXTI_LINE_9
  *         @arg @ref LL_EXTI_LINE_10
  *         @arg @ref LL_EXTI_LINE_11
  *         @arg @ref LL_EXTI_LINE_12
  *         @arg @ref LL_EXTI_LINE_13
  *         @arg @ref LL_EXTI_LINE_14
  *         @arg @ref LL_EXTI_LINE_15
  *         @arg @ref LL_EXTI_LINE_18
  *         @arg @ref LL_EXTI_LINE_20
  *         @arg @ref LL_EXTI_LINE_21
  *         @arg @ref LL_EXTI_LINE_22
  *         @arg @ref LL_EXTI_LINE_23
  *         @arg @ref LL_EXTI_LINE_24
  *         @arg @ref LL_EXTI_LINE_25
  *         @arg @ref LL_EXTI_LINE_26
  *         @arg @ref LL_EXTI_LINE_28
  *         @arg @ref LL_EXTI_LINE_29
  *         @arg @ref LL_EXTI_LINE_30
  *         @arg @ref LL_EXTI_LINE_31
  *         @arg @ref LL_EXTI_LINE_ALL_0_31
  * @retval None
  */
__STATIC_INLINE void LL_C2_EXTI_EnableIT_0_31(uint32_t ExtiLine)
{
  SET_BIT(EXTI->C2IMR1, ExtiLine);
}

/**
  * @brief  Enable ExtiLine Interrupt request for Lines in range 32 to 63 for cpu2
  * @rmtoll C2IMR2         IMx           LL_C2_EXTI_EnableIT_32_63
  * @param  ExtiLine This parameter can be one of the following values:
  *         @arg @ref LL_EXTI_LINE_32
  *         @arg @ref LL_EXTI_LINE_33
  *         @arg @ref LL_EXTI_LINE_34
  *         @arg @ref LL_EXTI_LINE_45
  *         @arg @ref LL_EXTI_LINE_ALL_32_63
  * @retval None
  */
__STATIC_INLINE void LL_C2_EXTI_EnableIT_32_63(uint32_t ExtiLine)
{
  SET_BIT(EXTI->C2IMR2, ExtiLine);
}

/**
  * @brief  Disable ExtiLine Interrupt request for Lines in range 0 to 31 for cpu2
  * @rmtoll C2IMR1         IMx           LL_C2_EXTI_DisableIT_0_31
  * @param  ExtiLine This parameter can be one of the following values:
  *         @arg @ref LL_EXTI_LINE_0
  *         @arg @ref LL_EXTI_LINE_1
  *         @arg @ref LL_EXTI_LINE_2
  *         @arg @ref LL_EXTI_LINE_3
  *         @arg @ref LL_EXTI_LINE_4
  *         @arg @ref LL_EXTI_LINE_5
  *         @arg @ref LL_EXTI_LINE_6
  *         @arg @ref LL_EXTI_LINE_7
  *         @arg @ref LL_EXTI_LINE_8
  *         @arg @ref LL_EXTI_LINE_9
  *         @arg @ref LL_EXTI_LINE_10
  *         @arg @ref LL_EXTI_LINE_11
  *         @arg @ref LL_EXTI_LINE_12
  *         @arg @ref LL_EXTI_LINE_13
  *         @arg @ref LL_EXTI_LINE_14
  *         @arg @ref LL_EXTI_LINE_15
  *         @arg @ref LL_EXTI_LINE_18
  *         @arg @ref LL_EXTI_LINE_20
  *         @arg @ref LL_EXTI_LINE_21
  *         @arg @ref LL_EXTI_LINE_22
  *         @arg @ref LL_EXTI_LINE_23
  *         @arg @ref LL_EXTI_LINE_24
  *         @arg @ref LL_EXTI_LINE_25
  *         @arg @ref LL_EXTI_LINE_26
  *         @arg @ref LL_EXTI_LINE_28
  *         @arg @ref LL_EXTI_LINE_29
  *         @arg @ref LL_EXTI_LINE_30
  *         @arg @ref LL_EXTI_LINE_31
  *         @arg @ref LL_EXTI_LINE_ALL_0_31
  * @retval None
  */
__STATIC_INLINE void LL_C2_EXTI_DisableIT_0_31(uint32_t ExtiLine)
{
  CLEAR_BIT(EXTI->C2IMR1, ExtiLine);
}

/**
  * @brief  Disable ExtiLine Interrupt request for Lines in range 32 to 63 for cpu2
  * @rmtoll C2IMR2         IMx           LL_C2_EXTI_DisableIT_32_63
  * @param  ExtiLine This parameter can be one of the following values:
  *         @arg @ref LL_EXTI_LINE_32
  *         @arg @ref LL_EXTI_LINE_33
  *         @arg @ref LL_EXTI_LINE_34
  *         @arg @ref LL_EXTI_LINE_45
  *         @arg @ref LL_EXTI_LINE_ALL_32_63
  * @retval None
  */
__STATIC_INLINE void LL_C2_EXTI_DisableIT_32_63(uint32_t ExtiLine)
{
  CLEAR_BIT(EXTI->C2IMR2, ExtiLine);
}

/**
  * @brief  Indicate if ExtiLine Interrupt request is enabled for Lines in range 0 to 31 for cpu2
  * @rmtoll C2IMR1         IMx           LL_C2_EXTI_IsEnabledIT_0_31
  * @param  ExtiLine This parameter can be one of the following values:
  *         @arg @ref LL_EXTI_LINE_0
  *         @arg @ref LL_EXTI_LINE_1
  *         @arg @ref LL_EXTI_LINE_2
  *         @arg @ref LL_EXTI_LINE_3
  *         @arg @ref LL_EXTI_LINE_4
  *         @arg @ref LL_EXTI_LINE_5
  *         @arg @ref LL_EXTI_LINE_6
  *         @arg @ref LL_EXTI_LINE_7
  *         @arg @ref LL_EXTI_LINE_8
  *         @arg @ref LL_EXTI_LINE_9
  *         @arg @ref LL_EXTI_LINE_10
  *         @arg @ref LL_EXTI_LINE_11
  *         @arg @ref LL_EXTI_LINE_12
  *         @arg @ref LL_EXTI_LINE_13
  *         @arg @ref LL_EXTI_LINE_14
  *         @arg @ref LL_EXTI_LINE_15
  *         @arg @ref LL_EXTI_LINE_18
  *         @arg @ref LL_EXTI_LINE_20
  *         @arg @ref LL_EXTI_LINE_21
  *         @arg @ref LL_EXTI_LINE_22
  *         @arg @ref LL_EXTI_LINE_23
  *         @arg @ref LL_EXTI_LINE_24
  *         @arg @ref LL_EXTI_LINE_25
  *         @arg @ref LL_EXTI_LINE_26
  *         @arg @ref LL_EXTI_LINE_28
  *         @arg @ref LL_EXTI_LINE_29
  *         @arg @ref LL_EXTI_LINE_30
  *         @arg @ref LL_EXTI_LINE_31
  *         @arg @ref LL_EXTI_LINE_ALL_0_31
  * @retval State of bit (1 or 0).
  */
__STATIC_INLINE uint32_t LL_C2_EXTI_IsEnabledIT_0_31(uint32_t ExtiLine)
{
  return ((READ_BIT(EXTI->C2IMR1, ExtiLine) == (ExtiLine)) ? 1UL : 0UL);
}

/**
  * @brief  Indicate if ExtiLine Interrupt request is enabled for Lines in range 32 to 63 for cpu2
  * @rmtoll C2IMR2         IMx           LL_C2_EXTI_IsEnabledIT_32_63
  * @param  ExtiLine This parameter can be one of the following values:
  *         @arg @ref LL_EXTI_LINE_32
  *         @arg @ref LL_EXTI_LINE_33
  *         @arg @ref LL_EXTI_LINE_34
  *         @arg @ref LL_EXTI_LINE_45
  *         @arg @ref LL_EXTI_LINE_ALL_32_63
  * @retval State of bit (1 or 0).
  */
__STATIC_INLINE uint32_t LL_C2_EXTI_IsEnabledIT_32_63(uint32_t ExtiLine)
{
  return ((READ_BIT(EXTI->C2IMR2, ExtiLine) == (ExtiLine))? 1UL : 0UL);
}

/**
  * @}
  */

/** @defgroup EXTI_LL_EF_Event_Management Event_Management
  * @{
  */

/**
  * @brief  Enable ExtiLine Event request for Lines in range 0 to 31
  * @rmtoll EMR1         EMx           LL_EXTI_EnableEvent_0_31
  * @param  ExtiLine This parameter can be one of the following values:
  *         @arg @ref LL_EXTI_LINE_0
  *         @arg @ref LL_EXTI_LINE_1
  *         @arg @ref LL_EXTI_LINE_2
  *         @arg @ref LL_EXTI_LINE_3
  *         @arg @ref LL_EXTI_LINE_4
  *         @arg @ref LL_EXTI_LINE_5
  *         @arg @ref LL_EXTI_LINE_6
  *         @arg @ref LL_EXTI_LINE_7
  *         @arg @ref LL_EXTI_LINE_8
  *         @arg @ref LL_EXTI_LINE_9
  *         @arg @ref LL_EXTI_LINE_10
  *         @arg @ref LL_EXTI_LINE_11
  *         @arg @ref LL_EXTI_LINE_12
  *         @arg @ref LL_EXTI_LINE_13
  *         @arg @ref LL_EXTI_LINE_14
  *         @arg @ref LL_EXTI_LINE_15
  *         @arg @ref LL_EXTI_LINE_18
  *         @arg @ref LL_EXTI_LINE_20
  *         @arg @ref LL_EXTI_LINE_21
  *         @arg @ref LL_EXTI_LINE_22
  *         @arg @ref LL_EXTI_LINE_23
  *         @arg @ref LL_EXTI_LINE_24
  *         @arg @ref LL_EXTI_LINE_25
  *         @arg @ref LL_EXTI_LINE_26
  *         @arg @ref LL_EXTI_LINE_28
  *         @arg @ref LL_EXTI_LINE_29
  *         @arg @ref LL_EXTI_LINE_30
  *         @arg @ref LL_EXTI_LINE_31
  *         @arg @ref LL_EXTI_LINE_ALL_0_31
  * @retval None
  */
__STATIC_INLINE void LL_EXTI_EnableEvent_0_31(uint32_t ExtiLine)
{
  SET_BIT(EXTI->C1EMR1, ExtiLine);
}

/**
  * @brief  Enable ExtiLine Event request for Lines in range 32 to 63
  * @rmtoll EMR2         EMx           LL_EXTI_EnableEvent_32_63
  * @param  ExtiLine This parameter can be a combination of the following values:
  *         @arg @ref LL_EXTI_LINE_32
  *         @arg @ref LL_EXTI_LINE_33
  *         @arg @ref LL_EXTI_LINE_34
  *         @arg @ref LL_EXTI_LINE_44
  *         @arg @ref LL_EXTI_LINE_ALL_32_63
  * @retval None
  */
__STATIC_INLINE void LL_EXTI_EnableEvent_32_63(uint32_t ExtiLine)
{
  SET_BIT(EXTI->C1EMR2, ExtiLine);
}

/**
  * @brief  Disable ExtiLine Event request for Lines in range 0 to 31
  * @rmtoll EMR1         EMx           LL_EXTI_DisableEvent_0_31
  * @param  ExtiLine This parameter can be one of the following values:
  *         @arg @ref LL_EXTI_LINE_0
  *         @arg @ref LL_EXTI_LINE_1
  *         @arg @ref LL_EXTI_LINE_2
  *         @arg @ref LL_EXTI_LINE_3
  *         @arg @ref LL_EXTI_LINE_4
  *         @arg @ref LL_EXTI_LINE_5
  *         @arg @ref LL_EXTI_LINE_6
  *         @arg @ref LL_EXTI_LINE_7
  *         @arg @ref LL_EXTI_LINE_8
  *         @arg @ref LL_EXTI_LINE_9
  *         @arg @ref LL_EXTI_LINE_10
  *         @arg @ref LL_EXTI_LINE_11
  *         @arg @ref LL_EXTI_LINE_12
  *         @arg @ref LL_EXTI_LINE_13
  *         @arg @ref LL_EXTI_LINE_14
  *         @arg @ref LL_EXTI_LINE_15
  *         @arg @ref LL_EXTI_LINE_18
  *         @arg @ref LL_EXTI_LINE_20
  *         @arg @ref LL_EXTI_LINE_21
  *         @arg @ref LL_EXTI_LINE_22
  *         @arg @ref LL_EXTI_LINE_23
  *         @arg @ref LL_EXTI_LINE_24
  *         @arg @ref LL_EXTI_LINE_25
  *         @arg @ref LL_EXTI_LINE_26
  *         @arg @ref LL_EXTI_LINE_28
  *         @arg @ref LL_EXTI_LINE_29
  *         @arg @ref LL_EXTI_LINE_30
  *         @arg @ref LL_EXTI_LINE_31
  *         @arg @ref LL_EXTI_LINE_ALL_0_31
  * @retval None
  */
__STATIC_INLINE void LL_EXTI_DisableEvent_0_31(uint32_t ExtiLine)
{
  CLEAR_BIT(EXTI->C1EMR1, ExtiLine);
}

/**
  * @brief  Disable ExtiLine Event request for Lines in range 32 to 63
  * @rmtoll EMR2         EMx           LL_EXTI_DisableEvent_32_63
  * @param  ExtiLine This parameter can be a combination of the following values:
  *         @arg @ref LL_EXTI_LINE_32
  *         @arg @ref LL_EXTI_LINE_33
  *         @arg @ref LL_EXTI_LINE_34
  *         @arg @ref LL_EXTI_LINE_44
  *         @arg @ref LL_EXTI_LINE_ALL_32_63
  * @retval None
  */
__STATIC_INLINE void LL_EXTI_DisableEvent_32_63(uint32_t ExtiLine)
{
  CLEAR_BIT(EXTI->C1EMR2, ExtiLine);
}

/**
  * @brief  Indicate if ExtiLine Event request is enabled for Lines in range 0 to 31
  * @rmtoll EMR1         EMx           LL_EXTI_IsEnabledEvent_0_31
  * @param  ExtiLine This parameter can be one of the following values:
  *         @arg @ref LL_EXTI_LINE_0
  *         @arg @ref LL_EXTI_LINE_1
  *         @arg @ref LL_EXTI_LINE_2
  *         @arg @ref LL_EXTI_LINE_3
  *         @arg @ref LL_EXTI_LINE_4
  *         @arg @ref LL_EXTI_LINE_5
  *         @arg @ref LL_EXTI_LINE_6
  *         @arg @ref LL_EXTI_LINE_7
  *         @arg @ref LL_EXTI_LINE_8
  *         @arg @ref LL_EXTI_LINE_9
  *         @arg @ref LL_EXTI_LINE_10
  *         @arg @ref LL_EXTI_LINE_11
  *         @arg @ref LL_EXTI_LINE_12
  *         @arg @ref LL_EXTI_LINE_13
  *         @arg @ref LL_EXTI_LINE_14
  *         @arg @ref LL_EXTI_LINE_15
  *         @arg @ref LL_EXTI_LINE_18
  *         @arg @ref LL_EXTI_LINE_20
  *         @arg @ref LL_EXTI_LINE_21
  *         @arg @ref LL_EXTI_LINE_22
  *         @arg @ref LL_EXTI_LINE_23
  *         @arg @ref LL_EXTI_LINE_24
  *         @arg @ref LL_EXTI_LINE_25
  *         @arg @ref LL_EXTI_LINE_26
  *         @arg @ref LL_EXTI_LINE_28
  *         @arg @ref LL_EXTI_LINE_29
  *         @arg @ref LL_EXTI_LINE_30
  *         @arg @ref LL_EXTI_LINE_31
  *         @arg @ref LL_EXTI_LINE_ALL_0_31
  * @retval State of bit (1 or 0).
  */
__STATIC_INLINE uint32_t LL_EXTI_IsEnabledEvent_0_31(uint32_t ExtiLine)
{
  return ((READ_BIT(EXTI->C1EMR1, ExtiLine) == (ExtiLine)) ? 1UL : 0UL);
}

/**
  * @brief  Indicate if ExtiLine Event request is enabled for Lines in range 32 to 63
  * @rmtoll EMR2         EMx           LL_EXTI_IsEnabledEvent_32_63
  * @param  ExtiLine This parameter can be a combination of the following values:
  *         @arg @ref LL_EXTI_LINE_32
  *         @arg @ref LL_EXTI_LINE_33
  *         @arg @ref LL_EXTI_LINE_34
  *         @arg @ref LL_EXTI_LINE_44
  *         @arg @ref LL_EXTI_LINE_ALL_32_63
  * @retval State of bit (1 or 0).
  */
__STATIC_INLINE uint32_t LL_EXTI_IsEnabledEvent_32_63(uint32_t ExtiLine)
{
  return ((READ_BIT(EXTI->C1EMR2, ExtiLine) == (ExtiLine)) ? 1UL : 0UL);
}

/**
  * @brief  Enable ExtiLine Event request for Lines in range 0 to 31 for cpu2
  * @rmtoll C2EMR1         EMx           LL_C2_EXTI_EnableEvent_0_31
  * @param  ExtiLine This parameter can be one of the following values:
  *         @arg @ref LL_EXTI_LINE_0
  *         @arg @ref LL_EXTI_LINE_1
  *         @arg @ref LL_EXTI_LINE_2
  *         @arg @ref LL_EXTI_LINE_3
  *         @arg @ref LL_EXTI_LINE_4
  *         @arg @ref LL_EXTI_LINE_5
  *         @arg @ref LL_EXTI_LINE_6
  *         @arg @ref LL_EXTI_LINE_7
  *         @arg @ref LL_EXTI_LINE_8
  *         @arg @ref LL_EXTI_LINE_9
  *         @arg @ref LL_EXTI_LINE_10
  *         @arg @ref LL_EXTI_LINE_11
  *         @arg @ref LL_EXTI_LINE_12
  *         @arg @ref LL_EXTI_LINE_13
  *         @arg @ref LL_EXTI_LINE_14
  *         @arg @ref LL_EXTI_LINE_15
  *         @arg @ref LL_EXTI_LINE_18
  *         @arg @ref LL_EXTI_LINE_20
  *         @arg @ref LL_EXTI_LINE_21
  *         @arg @ref LL_EXTI_LINE_22
  *         @arg @ref LL_EXTI_LINE_23
  *         @arg @ref LL_EXTI_LINE_24
  *         @arg @ref LL_EXTI_LINE_25
  *         @arg @ref LL_EXTI_LINE_26
  *         @arg @ref LL_EXTI_LINE_28
  *         @arg @ref LL_EXTI_LINE_29
  *         @arg @ref LL_EXTI_LINE_30
  *         @arg @ref LL_EXTI_LINE_31
  *         @arg @ref LL_EXTI_LINE_ALL_0_31
  * @retval None
  */
__STATIC_INLINE void LL_C2_EXTI_EnableEvent_0_31(uint32_t ExtiLine)
{
  SET_BIT(EXTI->C2EMR1, ExtiLine);
}

/**
  * @brief  Enable ExtiLine Event request for Lines in range 32 to 63 for cpu2
  * @rmtoll C2EMR2         EMx           LL_C2_EXTI_EnableEvent_32_63
  * @param  ExtiLine This parameter can be a combination of the following values:
  *         @arg @ref LL_EXTI_LINE_32
  *         @arg @ref LL_EXTI_LINE_33
  *         @arg @ref LL_EXTI_LINE_34
  *         @arg @ref LL_EXTI_LINE_45
  *         @arg @ref LL_EXTI_LINE_ALL_32_63
  * @retval None
  */
__STATIC_INLINE void LL_C2_EXTI_EnableEvent_32_63(uint32_t ExtiLine)
{
  SET_BIT(EXTI->C2EMR2, ExtiLine);
}

/**
  * @brief  Disable ExtiLine Event request for Lines in range 0 to 31 for cpu2
  * @rmtoll C2EMR1         EMx           LL_C2_EXTI_DisableEvent_0_31
  * @param  ExtiLine This parameter can be one of the following values:
  *         @arg @ref LL_EXTI_LINE_0
  *         @arg @ref LL_EXTI_LINE_1
  *         @arg @ref LL_EXTI_LINE_2
  *         @arg @ref LL_EXTI_LINE_3
  *         @arg @ref LL_EXTI_LINE_4
  *         @arg @ref LL_EXTI_LINE_5
  *         @arg @ref LL_EXTI_LINE_6
  *         @arg @ref LL_EXTI_LINE_7
  *         @arg @ref LL_EXTI_LINE_8
  *         @arg @ref LL_EXTI_LINE_9
  *         @arg @ref LL_EXTI_LINE_10
  *         @arg @ref LL_EXTI_LINE_11
  *         @arg @ref LL_EXTI_LINE_12
  *         @arg @ref LL_EXTI_LINE_13
  *         @arg @ref LL_EXTI_LINE_14
  *         @arg @ref LL_EXTI_LINE_15
  *         @arg @ref LL_EXTI_LINE_18
  *         @arg @ref LL_EXTI_LINE_20
  *         @arg @ref LL_EXTI_LINE_21
  *         @arg @ref LL_EXTI_LINE_22
  *         @arg @ref LL_EXTI_LINE_23
  *         @arg @ref LL_EXTI_LINE_24
  *         @arg @ref LL_EXTI_LINE_25
  *         @arg @ref LL_EXTI_LINE_26
  *         @arg @ref LL_EXTI_LINE_28
  *         @arg @ref LL_EXTI_LINE_29
  *         @arg @ref LL_EXTI_LINE_30
  *         @arg @ref LL_EXTI_LINE_31
  *         @arg @ref LL_EXTI_LINE_ALL_0_31
  * @retval None
  */
__STATIC_INLINE void LL_C2_EXTI_DisableEvent_0_31(uint32_t ExtiLine)
{
  CLEAR_BIT(EXTI->C2EMR1, ExtiLine);
}

/**
  * @brief  Disable ExtiLine Event request for Lines in range 32 to 63 for cpu2
  * @rmtoll C2EMR2         EMx           LL_C2_EXTI_DisableEvent_32_63
  * @param  ExtiLine This parameter can be a combination of the following values:
  *         @arg @ref LL_EXTI_LINE_32
  *         @arg @ref LL_EXTI_LINE_33
  *         @arg @ref LL_EXTI_LINE_34
  *         @arg @ref LL_EXTI_LINE_45
  *         @arg @ref LL_EXTI_LINE_ALL_32_63
  * @retval None
  */
__STATIC_INLINE void LL_C2_EXTI_DisableEvent_32_63(uint32_t ExtiLine)
{
  CLEAR_BIT(EXTI->C2EMR2, ExtiLine);
}

/**
  * @brief  Indicate if ExtiLine Event request is enabled for Lines in range 0 to 31 for cpu2
  * @rmtoll C2EMR1         EMx           LL_C2_EXTI_IsEnabledEvent_0_31
  * @param  ExtiLine This parameter can be one of the following values:
  *         @arg @ref LL_EXTI_LINE_0
  *         @arg @ref LL_EXTI_LINE_1
  *         @arg @ref LL_EXTI_LINE_2
  *         @arg @ref LL_EXTI_LINE_3
  *         @arg @ref LL_EXTI_LINE_4
  *         @arg @ref LL_EXTI_LINE_5
  *         @arg @ref LL_EXTI_LINE_6
  *         @arg @ref LL_EXTI_LINE_7
  *         @arg @ref LL_EXTI_LINE_8
  *         @arg @ref LL_EXTI_LINE_9
  *         @arg @ref LL_EXTI_LINE_10
  *         @arg @ref LL_EXTI_LINE_11
  *         @arg @ref LL_EXTI_LINE_12
  *         @arg @ref LL_EXTI_LINE_13
  *         @arg @ref LL_EXTI_LINE_14
  *         @arg @ref LL_EXTI_LINE_15
  *         @arg @ref LL_EXTI_LINE_18
  *         @arg @ref LL_EXTI_LINE_20
  *         @arg @ref LL_EXTI_LINE_21
  *         @arg @ref LL_EXTI_LINE_22
  *         @arg @ref LL_EXTI_LINE_23
  *         @arg @ref LL_EXTI_LINE_24
  *         @arg @ref LL_EXTI_LINE_25
  *         @arg @ref LL_EXTI_LINE_26
  *         @arg @ref LL_EXTI_LINE_28
  *         @arg @ref LL_EXTI_LINE_29
  *         @arg @ref LL_EXTI_LINE_30
  *         @arg @ref LL_EXTI_LINE_31
  *         @arg @ref LL_EXTI_LINE_ALL_0_31
  * @retval State of bit (1 or 0).
  */
__STATIC_INLINE uint32_t LL_C2_EXTI_IsEnabledEvent_0_31(uint32_t ExtiLine)
{
  return ((READ_BIT(EXTI->C2EMR1, ExtiLine) == (ExtiLine)) ? 1UL : 0UL);
}

/**
  * @brief  Indicate if ExtiLine Event request is enabled for Lines in range 32 to 63 for cpu2
  * @rmtoll C2EMR2         EMx           LL_C2_EXTI_IsEnabledEvent_32_63
  * @param  ExtiLine This parameter can be a combination of the following values:
  *         @arg @ref LL_EXTI_LINE_32
  *         @arg @ref LL_EXTI_LINE_33
  *         @arg @ref LL_EXTI_LINE_34
  *         @arg @ref LL_EXTI_LINE_45
  *         @arg @ref LL_EXTI_LINE_ALL_32_63
  * @retval State of bit (1 or 0).
  */
__STATIC_INLINE uint32_t LL_C2_EXTI_IsEnabledEvent_32_63(uint32_t ExtiLine)
{
  return ((READ_BIT(EXTI->C2EMR2, ExtiLine) == (ExtiLine)) ? 1UL : 0UL);
}

/**
  * @}
  */

/** @defgroup EXTI_LL_EF_Rising_Trigger_Management Rising_Trigger_Management
  * @{
  */

/**
  * @brief  Enable ExtiLine Rising Edge Trigger for Lines in range 0 to 31
  * @note The configurable wakeup lines are edge-triggered. No glitch must be
  *       generated on these lines. If a rising edge on a configurable interrupt
  *       line occurs during a write operation in the EXTI_RTSR register, the
  *       pending bit is not set.
  *       Rising and falling edge triggers can be set for
  *       the same interrupt line. In this case, both generate a trigger
  *       condition.
  * @rmtoll RTSR1        RTx           LL_EXTI_EnableRisingTrig_0_31
  * @param  ExtiLine This parameter can be a combination of the following values:
  *         @arg @ref LL_EXTI_LINE_0
  *         @arg @ref LL_EXTI_LINE_1
  *         @arg @ref LL_EXTI_LINE_2
  *         @arg @ref LL_EXTI_LINE_3
  *         @arg @ref LL_EXTI_LINE_4
  *         @arg @ref LL_EXTI_LINE_5
  *         @arg @ref LL_EXTI_LINE_6
  *         @arg @ref LL_EXTI_LINE_7
  *         @arg @ref LL_EXTI_LINE_8
  *         @arg @ref LL_EXTI_LINE_9
  *         @arg @ref LL_EXTI_LINE_10
  *         @arg @ref LL_EXTI_LINE_11
  *         @arg @ref LL_EXTI_LINE_12
  *         @arg @ref LL_EXTI_LINE_13
  *         @arg @ref LL_EXTI_LINE_14
  *         @arg @ref LL_EXTI_LINE_15
  *         @arg @ref LL_EXTI_LINE_18
  *         @arg @ref LL_EXTI_LINE_20
  *         @arg @ref LL_EXTI_LINE_21
  *         @arg @ref LL_EXTI_LINE_22
  *         @arg @ref LL_EXTI_LINE_29
  *         @arg @ref LL_EXTI_LINE_30
  *         @arg @ref LL_EXTI_LINE_31
  * @retval None
  */
__STATIC_INLINE void LL_EXTI_EnableRisingTrig_0_31(uint32_t ExtiLine)
{
  SET_BIT(EXTI->RTSR1, ExtiLine);

}

/**
  * @brief  Enable ExtiLine Rising Edge Trigger for Lines in range 32 to 63
  * @note The configurable wakeup lines are edge-triggered. No glitch must be
  *       generated on these lines. If a rising edge on a configurable interrupt
  *       line occurs during a write operation in the EXTI_RTSR register, the
  *       pending bit is not set.Rising and falling edge triggers can be set for
  *       the same interrupt line. In this case, both generate a trigger
  *       condition.
  * @rmtoll RTSR2        RTx           LL_EXTI_EnableRisingTrig_32_63
  * @param  ExtiLine This parameter can be a combination of the following values:
  *         @arg @ref LL_EXTI_LINE_32
  *         @arg @ref LL_EXTI_LINE_33
  *         @arg @ref LL_EXTI_LINE_34
  * @retval None
  */
__STATIC_INLINE void LL_EXTI_EnableRisingTrig_32_63(uint32_t ExtiLine)
{
  SET_BIT(EXTI->RTSR2, ExtiLine);
}

/**
  * @brief  Disable ExtiLine Rising Edge Trigger for Lines in range 0 to 31
  * @note The configurable wakeup lines are edge-triggered. No glitch must be
  *       generated on these lines. If a rising edge on a configurable interrupt
  *       line occurs during a write operation in the EXTI_RTSR register, the
  *       pending bit is not set.
  *       Rising and falling edge triggers can be set for
  *       the same interrupt line. In this case, both generate a trigger
  *       condition.
  * @rmtoll RTSR1        RTx           LL_EXTI_DisableRisingTrig_0_31
  * @param  ExtiLine This parameter can be a combination of the following values:
  *         @arg @ref LL_EXTI_LINE_0
  *         @arg @ref LL_EXTI_LINE_1
  *         @arg @ref LL_EXTI_LINE_2
  *         @arg @ref LL_EXTI_LINE_3
  *         @arg @ref LL_EXTI_LINE_4
  *         @arg @ref LL_EXTI_LINE_5
  *         @arg @ref LL_EXTI_LINE_6
  *         @arg @ref LL_EXTI_LINE_7
  *         @arg @ref LL_EXTI_LINE_8
  *         @arg @ref LL_EXTI_LINE_9
  *         @arg @ref LL_EXTI_LINE_10
  *         @arg @ref LL_EXTI_LINE_11
  *         @arg @ref LL_EXTI_LINE_12
  *         @arg @ref LL_EXTI_LINE_13
  *         @arg @ref LL_EXTI_LINE_14
  *         @arg @ref LL_EXTI_LINE_15
  *         @arg @ref LL_EXTI_LINE_18
  *         @arg @ref LL_EXTI_LINE_20
  *         @arg @ref LL_EXTI_LINE_21
  *         @arg @ref LL_EXTI_LINE_22
  *         @arg @ref LL_EXTI_LINE_29
  *         @arg @ref LL_EXTI_LINE_30
  *         @arg @ref LL_EXTI_LINE_31
  * @retval None
  */
__STATIC_INLINE void LL_EXTI_DisableRisingTrig_0_31(uint32_t ExtiLine)
{
  CLEAR_BIT(EXTI->RTSR1, ExtiLine);

}

/**
  * @brief  Disable ExtiLine Rising Edge Trigger for Lines in range 32 to 63
  * @note The configurable wakeup lines are edge-triggered. No glitch must be
  *       generated on these lines. If a rising edge on a configurable interrupt
  *       line occurs during a write operation in the EXTI_RTSR register, the
  *       pending bit is not set.
  *       Rising and falling edge triggers can be set for
  *       the same interrupt line. In this case, both generate a trigger
  *       condition.
  * @rmtoll RTSR2        RTx           LL_EXTI_DisableRisingTrig_32_63
  * @param  ExtiLine This parameter can be a combination of the following values:
  *         @arg @ref LL_EXTI_LINE_32
  *         @arg @ref LL_EXTI_LINE_33
  *         @arg @ref LL_EXTI_LINE_34
  * @retval None
  */
__STATIC_INLINE void LL_EXTI_DisableRisingTrig_32_63(uint32_t ExtiLine)
{
  CLEAR_BIT(EXTI->RTSR2, ExtiLine);
}

/**
  * @brief  Check if rising edge trigger is enabled for Lines in range 0 to 31
  * @rmtoll RTSR1        RTx           LL_EXTI_IsEnabledRisingTrig_0_31
  * @param  ExtiLine This parameter can be a combination of the following values:
  *         @arg @ref LL_EXTI_LINE_0
  *         @arg @ref LL_EXTI_LINE_1
  *         @arg @ref LL_EXTI_LINE_2
  *         @arg @ref LL_EXTI_LINE_3
  *         @arg @ref LL_EXTI_LINE_4
  *         @arg @ref LL_EXTI_LINE_5
  *         @arg @ref LL_EXTI_LINE_6
  *         @arg @ref LL_EXTI_LINE_7
  *         @arg @ref LL_EXTI_LINE_8
  *         @arg @ref LL_EXTI_LINE_9
  *         @arg @ref LL_EXTI_LINE_10
  *         @arg @ref LL_EXTI_LINE_11
  *         @arg @ref LL_EXTI_LINE_12
  *         @arg @ref LL_EXTI_LINE_13
  *         @arg @ref LL_EXTI_LINE_14
  *         @arg @ref LL_EXTI_LINE_15
  *         @arg @ref LL_EXTI_LINE_18
  *         @arg @ref LL_EXTI_LINE_20
  *         @arg @ref LL_EXTI_LINE_21
  *         @arg @ref LL_EXTI_LINE_22
  *         @arg @ref LL_EXTI_LINE_29
  *         @arg @ref LL_EXTI_LINE_30
  *         @arg @ref LL_EXTI_LINE_31
  * @retval State of bit (1 or 0).
  */
__STATIC_INLINE uint32_t LL_EXTI_IsEnabledRisingTrig_0_31(uint32_t ExtiLine)
{
  return ((READ_BIT(EXTI->RTSR1, ExtiLine) == (ExtiLine)) ? 1UL : 0UL);
}

/**
  * @brief  Check if rising edge trigger is enabled for Lines in range 32 to 63
  * @rmtoll RTSR2        RTx           LL_EXTI_IsEnabledRisingTrig_32_63
  * @param  ExtiLine This parameter can be a combination of the following values:
  *         @arg @ref LL_EXTI_LINE_32
  *         @arg @ref LL_EXTI_LINE_33
  *         @arg @ref LL_EXTI_LINE_34
  * @retval State of bit (1 or 0).
  */
__STATIC_INLINE uint32_t LL_EXTI_IsEnabledRisingTrig_32_63(uint32_t ExtiLine)
{
  return ((READ_BIT(EXTI->RTSR2, ExtiLine) == (ExtiLine)) ? 1UL : 0UL);
}

/**
  * @}
  */

/** @defgroup EXTI_LL_EF_Falling_Trigger_Management Falling_Trigger_Management
  * @{
  */

/**
  * @brief  Enable ExtiLine Falling Edge Trigger for Lines in range 0 to 31
  * @note The configurable wakeup lines are edge-triggered. No glitch must be
  *       generated on these lines. If a falling edge on a configurable interrupt
  *       line occurs during a write operation in the EXTI_FTSR register, the
  *       pending bit is not set.
  *       Rising and falling edge triggers can be set for
  *       the same interrupt line. In this case, both generate a trigger
  *       condition.
  * @rmtoll FTSR1        FTx           LL_EXTI_EnableFallingTrig_0_31
  * @param  ExtiLine This parameter can be a combination of the following values:
  *         @arg @ref LL_EXTI_LINE_0
  *         @arg @ref LL_EXTI_LINE_1
  *         @arg @ref LL_EXTI_LINE_2
  *         @arg @ref LL_EXTI_LINE_3
  *         @arg @ref LL_EXTI_LINE_4
  *         @arg @ref LL_EXTI_LINE_5
  *         @arg @ref LL_EXTI_LINE_6
  *         @arg @ref LL_EXTI_LINE_7
  *         @arg @ref LL_EXTI_LINE_8
  *         @arg @ref LL_EXTI_LINE_9
  *         @arg @ref LL_EXTI_LINE_10
  *         @arg @ref LL_EXTI_LINE_11
  *         @arg @ref LL_EXTI_LINE_12
  *         @arg @ref LL_EXTI_LINE_13
  *         @arg @ref LL_EXTI_LINE_14
  *         @arg @ref LL_EXTI_LINE_15
  *         @arg @ref LL_EXTI_LINE_18
  *         @arg @ref LL_EXTI_LINE_20
  *         @arg @ref LL_EXTI_LINE_21
  *         @arg @ref LL_EXTI_LINE_22
  *         @arg @ref LL_EXTI_LINE_29
  *         @arg @ref LL_EXTI_LINE_30
  *         @arg @ref LL_EXTI_LINE_31
  * @retval None
  */
__STATIC_INLINE void LL_EXTI_EnableFallingTrig_0_31(uint32_t ExtiLine)
{
  SET_BIT(EXTI->FTSR1, ExtiLine);
}

/**
  * @brief  Enable ExtiLine Falling Edge Trigger for Lines in range 32 to 63
  * @note The configurable wakeup lines are edge-triggered. No glitch must be
  *       generated on these lines. If a Falling edge on a configurable interrupt
  *       line occurs during a write operation in the EXTI_FTSR register, the
  *       pending bit is not set.
  *       Rising and falling edge triggers can be set for
  *       the same interrupt line. In this case, both generate a trigger
  *       condition.
  * @rmtoll FTSR2        FTx           LL_EXTI_EnableFallingTrig_32_63
  * @param  ExtiLine This parameter can be a combination of the following values:
  *         @arg @ref LL_EXTI_LINE_32
  *         @arg @ref LL_EXTI_LINE_33
  *         @arg @ref LL_EXTI_LINE_34
  * @retval None
  */
__STATIC_INLINE void LL_EXTI_EnableFallingTrig_32_63(uint32_t ExtiLine)
{
  SET_BIT(EXTI->FTSR2, ExtiLine);
}

/**
  * @brief  Disable ExtiLine Falling Edge Trigger for Lines in range 0 to 31
  * @note The configurable wakeup lines are edge-triggered. No glitch must be
  *       generated on these lines. If a Falling edge on a configurable interrupt
  *       line occurs during a write operation in the EXTI_FTSR register, the
  *       pending bit is not set.
  *       Rising and falling edge triggers can be set for the same interrupt line.
  *       In this case, both generate a trigger condition.
  * @rmtoll FTSR1        FTx           LL_EXTI_DisableFallingTrig_0_31
  * @param  ExtiLine This parameter can be a combination of the following values:
  *         @arg @ref LL_EXTI_LINE_0
  *         @arg @ref LL_EXTI_LINE_1
  *         @arg @ref LL_EXTI_LINE_2
  *         @arg @ref LL_EXTI_LINE_3
  *         @arg @ref LL_EXTI_LINE_4
  *         @arg @ref LL_EXTI_LINE_5
  *         @arg @ref LL_EXTI_LINE_6
  *         @arg @ref LL_EXTI_LINE_7
  *         @arg @ref LL_EXTI_LINE_8
  *         @arg @ref LL_EXTI_LINE_9
  *         @arg @ref LL_EXTI_LINE_10
  *         @arg @ref LL_EXTI_LINE_11
  *         @arg @ref LL_EXTI_LINE_12
  *         @arg @ref LL_EXTI_LINE_13
  *         @arg @ref LL_EXTI_LINE_14
  *         @arg @ref LL_EXTI_LINE_15
  *         @arg @ref LL_EXTI_LINE_18
  *         @arg @ref LL_EXTI_LINE_20
  *         @arg @ref LL_EXTI_LINE_21
  *         @arg @ref LL_EXTI_LINE_22
  *         @arg @ref LL_EXTI_LINE_29
  *         @arg @ref LL_EXTI_LINE_30
  *         @arg @ref LL_EXTI_LINE_31
  * @retval None
  */
__STATIC_INLINE void LL_EXTI_DisableFallingTrig_0_31(uint32_t ExtiLine)
{
  CLEAR_BIT(EXTI->FTSR1, ExtiLine);
}

/**
  * @brief  Disable ExtiLine Falling Edge Trigger for Lines in range 32 to 63
  * @note The configurable wakeup lines are edge-triggered. No glitch must be
  *       generated on these lines. If a Falling edge on a configurable interrupt
  *       line occurs during a write operation in the EXTI_FTSR register, the
  *       pending bit is not set.
  *       Rising and falling edge triggers can be set for the same interrupt line.
  *       In this case, both generate a trigger condition.
  * @rmtoll FTSR2        FTx           LL_EXTI_DisableFallingTrig_32_63
  * @param  ExtiLine This parameter can be a combination of the following values:
  *         @arg @ref LL_EXTI_LINE_32
  *         @arg @ref LL_EXTI_LINE_33
  *         @arg @ref LL_EXTI_LINE_34
  * @retval None
  */
__STATIC_INLINE void LL_EXTI_DisableFallingTrig_32_63(uint32_t ExtiLine)
{
  CLEAR_BIT(EXTI->FTSR2, ExtiLine);
}

/**
  * @brief  Check if falling edge trigger is enabled for Lines in range 0 to 31
  * @rmtoll FTSR1        FTx           LL_EXTI_IsEnabledFallingTrig_0_31
  * @param  ExtiLine This parameter can be a combination of the following values:
  *         @arg @ref LL_EXTI_LINE_0
  *         @arg @ref LL_EXTI_LINE_1
  *         @arg @ref LL_EXTI_LINE_2
  *         @arg @ref LL_EXTI_LINE_3
  *         @arg @ref LL_EXTI_LINE_4
  *         @arg @ref LL_EXTI_LINE_5
  *         @arg @ref LL_EXTI_LINE_6
  *         @arg @ref LL_EXTI_LINE_7
  *         @arg @ref LL_EXTI_LINE_8
  *         @arg @ref LL_EXTI_LINE_9
  *         @arg @ref LL_EXTI_LINE_10
  *         @arg @ref LL_EXTI_LINE_11
  *         @arg @ref LL_EXTI_LINE_12
  *         @arg @ref LL_EXTI_LINE_13
  *         @arg @ref LL_EXTI_LINE_14
  *         @arg @ref LL_EXTI_LINE_15
  *         @arg @ref LL_EXTI_LINE_18
  *         @arg @ref LL_EXTI_LINE_20
  *         @arg @ref LL_EXTI_LINE_21
  *         @arg @ref LL_EXTI_LINE_22
  *         @arg @ref LL_EXTI_LINE_29
  *         @arg @ref LL_EXTI_LINE_30
  *         @arg @ref LL_EXTI_LINE_31
  * @retval State of bit (1 or 0).
  */
__STATIC_INLINE uint32_t LL_EXTI_IsEnabledFallingTrig_0_31(uint32_t ExtiLine)
{
  return ((READ_BIT(EXTI->FTSR1, ExtiLine) == (ExtiLine)) ? 1UL : 0UL);
}

/**
  * @brief  Check if falling edge trigger is enabled for Lines in range 32 to 63
  * @rmtoll FTSR2        FTx           LL_EXTI_IsEnabledFallingTrig_32_63
  * @param  ExtiLine This parameter can be a combination of the following values:
  *         @arg @ref LL_EXTI_LINE_32
  *         @arg @ref LL_EXTI_LINE_33
  *         @arg @ref LL_EXTI_LINE_34
  * @retval State of bit (1 or 0).
  */
__STATIC_INLINE uint32_t LL_EXTI_IsEnabledFallingTrig_32_63(uint32_t ExtiLine)
{
  return ((READ_BIT(EXTI->FTSR2, ExtiLine) == (ExtiLine)) ? 1UL : 0UL);
}

/**
  * @}
  */

/** @defgroup EXTI_LL_EF_Software_Interrupt_Management Software_Interrupt_Management
  * @{
  */

/**
  * @brief  Generate a software Interrupt Event for Lines in range 0 to 31
  * @note If the interrupt is enabled on this line in the EXTI_C1IMR1, writing a 1 to
  *       this bit when it is at '0' sets the corresponding pending bit in EXTI_PR1
  *       resulting in an interrupt request generation.
  *       This bit is cleared by clearing the corresponding bit in the EXTI_PR1
  *       register (by writing a 1 into the bit)
  * @rmtoll SWIER1       SWIx          LL_EXTI_GenerateSWI_0_31
  * @param  ExtiLine This parameter can be a combination of the following values:
  *         @arg @ref LL_EXTI_LINE_0
  *         @arg @ref LL_EXTI_LINE_1
  *         @arg @ref LL_EXTI_LINE_2
  *         @arg @ref LL_EXTI_LINE_3
  *         @arg @ref LL_EXTI_LINE_4
  *         @arg @ref LL_EXTI_LINE_5
  *         @arg @ref LL_EXTI_LINE_6
  *         @arg @ref LL_EXTI_LINE_7
  *         @arg @ref LL_EXTI_LINE_8
  *         @arg @ref LL_EXTI_LINE_9
  *         @arg @ref LL_EXTI_LINE_10
  *         @arg @ref LL_EXTI_LINE_11
  *         @arg @ref LL_EXTI_LINE_12
  *         @arg @ref LL_EXTI_LINE_13
  *         @arg @ref LL_EXTI_LINE_14
  *         @arg @ref LL_EXTI_LINE_15
  *         @arg @ref LL_EXTI_LINE_18
  *         @arg @ref LL_EXTI_LINE_20
  *         @arg @ref LL_EXTI_LINE_21
  *         @arg @ref LL_EXTI_LINE_22
  *         @arg @ref LL_EXTI_LINE_29
  *         @arg @ref LL_EXTI_LINE_30
  *         @arg @ref LL_EXTI_LINE_31
  * @retval None
  */
__STATIC_INLINE void LL_EXTI_GenerateSWI_0_31(uint32_t ExtiLine)
{
  SET_BIT(EXTI->SWIER1, ExtiLine);
}

/**
  * @brief  Generate a software Interrupt Event for Lines in range 32 to 63
  * @note If the interrupt is enabled on this line in the EXTI_IMR2, writing a 1 to
  *       this bit when it is at '0' sets the corresponding pending bit in EXTI_PR2
  *       resulting in an interrupt request generation.
  *       This bit is cleared by clearing the corresponding bit in the EXTI_PR2
  *       register (by writing a 1 into the bit)
  * @rmtoll SWIER2       SWIx          LL_EXTI_GenerateSWI_32_63
  * @param  ExtiLine This parameter can be a combination of the following values:
  *         @arg @ref LL_EXTI_LINE_32
  *         @arg @ref LL_EXTI_LINE_33
  *         @arg @ref LL_EXTI_LINE_34
  * @retval None
  */
__STATIC_INLINE void LL_EXTI_GenerateSWI_32_63(uint32_t ExtiLine)
{
  SET_BIT(EXTI->SWIER2, ExtiLine);
}

/**
  * @}
  */

/** @defgroup EXTI_LL_EF_Flag_Management Flag_Management
  * @{
  */

/**
  * @brief  Check if the ExtLine Flag is set or not for Lines in range 0 to 31
  * @note This bit is set when the selected edge event arrives on the interrupt
  *       line. This bit is cleared by writing a 1 to the bit.
  * @rmtoll PR1          PIFx           LL_EXTI_IsActiveFlag_0_31
  * @param  ExtiLine This parameter can be a combination of the following values:
  *         @arg @ref LL_EXTI_LINE_0
  *         @arg @ref LL_EXTI_LINE_1
  *         @arg @ref LL_EXTI_LINE_2
  *         @arg @ref LL_EXTI_LINE_3
  *         @arg @ref LL_EXTI_LINE_4
  *         @arg @ref LL_EXTI_LINE_5
  *         @arg @ref LL_EXTI_LINE_6
  *         @arg @ref LL_EXTI_LINE_7
  *         @arg @ref LL_EXTI_LINE_8
  *         @arg @ref LL_EXTI_LINE_9
  *         @arg @ref LL_EXTI_LINE_10
  *         @arg @ref LL_EXTI_LINE_11
  *         @arg @ref LL_EXTI_LINE_12
  *         @arg @ref LL_EXTI_LINE_13
  *         @arg @ref LL_EXTI_LINE_14
  *         @arg @ref LL_EXTI_LINE_15
  *         @arg @ref LL_EXTI_LINE_18
  *         @arg @ref LL_EXTI_LINE_20
  *         @arg @ref LL_EXTI_LINE_21
  *         @arg @ref LL_EXTI_LINE_22
  *         @arg @ref LL_EXTI_LINE_29
  *         @arg @ref LL_EXTI_LINE_30
  *         @arg @ref LL_EXTI_LINE_31
  *         @arg @ref LL_EXTI_LINE_ALL_0_31
  * @retval State of bit (1 or 0).
  */
__STATIC_INLINE uint32_t LL_EXTI_IsActiveFlag_0_31(uint32_t ExtiLine)
{
  return ((READ_BIT(EXTI->C1PR1, ExtiLine) == (ExtiLine)) ? 1UL : 0UL);
}

/**
  * @brief  Check if the ExtLine Flag is set or not for  Lines in range 32 to 63
  * @note This bit is set when the selected edge event arrives on the interrupt
  *       line. This bit is cleared by writing a 1 to the bit.
  * @rmtoll PR2          PIFx           LL_EXTI_IsActiveFlag_32_63
  * @param  ExtiLine This parameter can be a combination of the following values:
  *         @arg @ref LL_EXTI_LINE_32
  *         @arg @ref LL_EXTI_LINE_33
  *         @arg @ref LL_EXTI_LINE_34
  *         @arg @ref LL_EXTI_LINE_ALL_32_63
  * @retval State of bit (1 or 0).
  */
__STATIC_INLINE uint32_t LL_EXTI_IsActiveFlag_32_63(uint32_t ExtiLine)
{
  return ((READ_BIT(EXTI->C1PR2, ExtiLine) == (ExtiLine)) ? 1UL : 0UL);
}

/**
  * @brief  Read ExtLine Combination Flag for Lines in range 0 to 31
  * @note This bit is set when the selected edge event arrives on the interrupt
  *       line. This bit is cleared by writing a 1 to the bit.
  * @rmtoll PR1          PIFx           LL_EXTI_ReadFlag_0_31
  * @param  ExtiLine This parameter can be a combination of the following values:
  *         @arg @ref LL_EXTI_LINE_0
  *         @arg @ref LL_EXTI_LINE_1
  *         @arg @ref LL_EXTI_LINE_2
  *         @arg @ref LL_EXTI_LINE_3
  *         @arg @ref LL_EXTI_LINE_4
  *         @arg @ref LL_EXTI_LINE_5
  *         @arg @ref LL_EXTI_LINE_6
  *         @arg @ref LL_EXTI_LINE_7
  *         @arg @ref LL_EXTI_LINE_8
  *         @arg @ref LL_EXTI_LINE_9
  *         @arg @ref LL_EXTI_LINE_10
  *         @arg @ref LL_EXTI_LINE_11
  *         @arg @ref LL_EXTI_LINE_12
  *         @arg @ref LL_EXTI_LINE_13
  *         @arg @ref LL_EXTI_LINE_14
  *         @arg @ref LL_EXTI_LINE_15
  *         @arg @ref LL_EXTI_LINE_18
  *         @arg @ref LL_EXTI_LINE_20
  *         @arg @ref LL_EXTI_LINE_21
  *         @arg @ref LL_EXTI_LINE_22
  *         @arg @ref LL_EXTI_LINE_29
  *         @arg @ref LL_EXTI_LINE_30
  *         @arg @ref LL_EXTI_LINE_31
  * @retval @note This bit is set when the selected edge event arrives on the interrupt
  */
__STATIC_INLINE uint32_t LL_EXTI_ReadFlag_0_31(uint32_t ExtiLine)
{
  return (uint32_t)(READ_BIT(EXTI->C1PR1, ExtiLine));
}

/**
  * @brief  Read ExtLine Combination Flag for  Lines in range 32 to 63
  * @note This bit is set when the selected edge event arrives on the interrupt
  *       line. This bit is cleared by writing a 1 to the bit.
  * @rmtoll PR2          PIFx           LL_EXTI_ReadFlag_32_63
  * @param  ExtiLine This parameter can be a combination of the following values:
  *         @arg @ref LL_EXTI_LINE_32
  *         @arg @ref LL_EXTI_LINE_33
  *         @arg @ref LL_EXTI_LINE_34
  * @retval @note This bit is set when the selected edge event arrives on the interrupt
  */
__STATIC_INLINE uint32_t LL_EXTI_ReadFlag_32_63(uint32_t ExtiLine)
{
  return (uint32_t)(READ_BIT(EXTI->C1PR2, ExtiLine));
}

/**
  * @brief  Clear ExtLine Flags for Lines in range 0 to 31
  * @note This bit is set when the selected edge event arrives on the interrupt
  *       line. This bit is cleared by writing a 1 to the bit.
  * @rmtoll PR1          PIFx           LL_EXTI_ClearFlag_0_31
  * @param  ExtiLine This parameter can be a combination of the following values:
  *         @arg @ref LL_EXTI_LINE_0
  *         @arg @ref LL_EXTI_LINE_1
  *         @arg @ref LL_EXTI_LINE_2
  *         @arg @ref LL_EXTI_LINE_3
  *         @arg @ref LL_EXTI_LINE_4
  *         @arg @ref LL_EXTI_LINE_5
  *         @arg @ref LL_EXTI_LINE_6
  *         @arg @ref LL_EXTI_LINE_7
  *         @arg @ref LL_EXTI_LINE_8
  *         @arg @ref LL_EXTI_LINE_9
  *         @arg @ref LL_EXTI_LINE_10
  *         @arg @ref LL_EXTI_LINE_11
  *         @arg @ref LL_EXTI_LINE_12
  *         @arg @ref LL_EXTI_LINE_13
  *         @arg @ref LL_EXTI_LINE_14
  *         @arg @ref LL_EXTI_LINE_15
  *         @arg @ref LL_EXTI_LINE_18
  *         @arg @ref LL_EXTI_LINE_20
  *         @arg @ref LL_EXTI_LINE_21
  *         @arg @ref LL_EXTI_LINE_22
  *         @arg @ref LL_EXTI_LINE_29
  *         @arg @ref LL_EXTI_LINE_30
  *         @arg @ref LL_EXTI_LINE_31
  * @retval None
  */
__STATIC_INLINE void LL_EXTI_ClearFlag_0_31(uint32_t ExtiLine)
{
  WRITE_REG(EXTI->C1PR1, ExtiLine);
}

/**
  * @brief  Clear ExtLine Flags for Lines in range 32 to 63
  * @note This bit is set when the selected edge event arrives on the interrupt
  *       line. This bit is cleared by writing a 1 to the bit.
  * @rmtoll PR2          PIFx           LL_EXTI_ClearFlag_32_63
  * @param  ExtiLine This parameter can be a combination of the following values:
  *         @arg @ref LL_EXTI_LINE_32
  *         @arg @ref LL_EXTI_LINE_33
  *         @arg @ref LL_EXTI_LINE_34
  * @retval None
  */
__STATIC_INLINE void LL_EXTI_ClearFlag_32_63(uint32_t ExtiLine)
{
  WRITE_REG(EXTI->C1PR2, ExtiLine);
}

/**
  * @brief  Check if the ExtLine Flag is set or not for Lines in range 0 to 31 for cpu2
  * @note This bit is set when the selected edge event arrives on the interrupt
  *       line. This bit is cleared by writing a 1 to the bit.
  * @rmtoll C2PR1          PIFx           LL_C2_EXTI_IsActiveFlag_0_31
  * @param  ExtiLine This parameter can be a combination of the following values:
  *         @arg @ref LL_EXTI_LINE_0
  *         @arg @ref LL_EXTI_LINE_1
  *         @arg @ref LL_EXTI_LINE_2
  *         @arg @ref LL_EXTI_LINE_3
  *         @arg @ref LL_EXTI_LINE_4
  *         @arg @ref LL_EXTI_LINE_5
  *         @arg @ref LL_EXTI_LINE_6
  *         @arg @ref LL_EXTI_LINE_7
  *         @arg @ref LL_EXTI_LINE_8
  *         @arg @ref LL_EXTI_LINE_9
  *         @arg @ref LL_EXTI_LINE_10
  *         @arg @ref LL_EXTI_LINE_11
  *         @arg @ref LL_EXTI_LINE_12
  *         @arg @ref LL_EXTI_LINE_13
  *         @arg @ref LL_EXTI_LINE_14
  *         @arg @ref LL_EXTI_LINE_15
  *         @arg @ref LL_EXTI_LINE_18
  *         @arg @ref LL_EXTI_LINE_20
  *         @arg @ref LL_EXTI_LINE_21
  *         @arg @ref LL_EXTI_LINE_22
  *         @arg @ref LL_EXTI_LINE_29
  *         @arg @ref LL_EXTI_LINE_30
  *         @arg @ref LL_EXTI_LINE_31
  *         @arg @ref LL_EXTI_LINE_ALL_0_31
  * @retval State of bit (1 or 0).
  */
__STATIC_INLINE uint32_t LL_C2_EXTI_IsActiveFlag_0_31(uint32_t ExtiLine)
{
  return ((READ_BIT(EXTI->C2PR1, ExtiLine) == (ExtiLine)) ? 1UL : 0UL);
}

/**
  * @brief  Check if the ExtLine Flag is set or not for  Lines in range 32 to 63 for cpu2
  * @note This bit is set when the selected edge event arrives on the interrupt
  *       line. This bit is cleared by writing a 1 to the bit.
  * @rmtoll C2PR2          PIFx           LL_C2_EXTI_IsActiveFlag_32_63
  * @param  ExtiLine This parameter can be a combination of the following values:
  *         @arg @ref LL_EXTI_LINE_32
  *         @arg @ref LL_EXTI_LINE_33
  *         @arg @ref LL_EXTI_LINE_34
  *         @arg @ref LL_EXTI_LINE_ALL_32_63
  * @retval State of bit (1 or 0).
  */
__STATIC_INLINE uint32_t LL_C2_EXTI_IsActiveFlag_32_63(uint32_t ExtiLine)
{
  return ((READ_BIT(EXTI->C2PR2, ExtiLine) == (ExtiLine)) ? 1UL : 0UL);
}

/**
  * @brief  Read ExtLine Combination Flag for Lines in range 0 to 31 for cpu2
  * @note This bit is set when the selected edge event arrives on the interrupt
  *       line. This bit is cleared by writing a 1 to the bit.
  * @rmtoll C2PR1          PIFx           LL_C2_EXTI_ReadFlag_0_31
  * @param  ExtiLine This parameter can be a combination of the following values:
  *         @arg @ref LL_EXTI_LINE_0
  *         @arg @ref LL_EXTI_LINE_1
  *         @arg @ref LL_EXTI_LINE_2
  *         @arg @ref LL_EXTI_LINE_3
  *         @arg @ref LL_EXTI_LINE_4
  *         @arg @ref LL_EXTI_LINE_5
  *         @arg @ref LL_EXTI_LINE_6
  *         @arg @ref LL_EXTI_LINE_7
  *         @arg @ref LL_EXTI_LINE_8
  *         @arg @ref LL_EXTI_LINE_9
  *         @arg @ref LL_EXTI_LINE_10
  *         @arg @ref LL_EXTI_LINE_11
  *         @arg @ref LL_EXTI_LINE_12
  *         @arg @ref LL_EXTI_LINE_13
  *         @arg @ref LL_EXTI_LINE_14
  *         @arg @ref LL_EXTI_LINE_15
  *         @arg @ref LL_EXTI_LINE_18
  *         @arg @ref LL_EXTI_LINE_20
  *         @arg @ref LL_EXTI_LINE_21
  *         @arg @ref LL_EXTI_LINE_22
  *         @arg @ref LL_EXTI_LINE_29
  *         @arg @ref LL_EXTI_LINE_30
  *         @arg @ref LL_EXTI_LINE_31
  * @retval @note This bit is set when the selected edge event arrives on the interrupt
  */
__STATIC_INLINE uint32_t LL_C2_EXTI_ReadFlag_0_31(uint32_t ExtiLine)
{
  return (uint32_t)(READ_BIT(EXTI->C2PR1, ExtiLine));
}

/**
  * @brief  Read ExtLine Combination Flag for  Lines in range 32 to 63 for cpu2
  * @note This bit is set when the selected edge event arrives on the interrupt
  *       line. This bit is cleared by writing a 1 to the bit.
  * @rmtoll C2PR2          PIFx           LL_C2_EXTI_ReadFlag_32_63
  * @param  ExtiLine This parameter can be a combination of the following values:
  *         @arg @ref LL_EXTI_LINE_32
  *         @arg @ref LL_EXTI_LINE_33
  *         @arg @ref LL_EXTI_LINE_34
  * @retval @note This bit is set when the selected edge event arrives on the interrupt
  */
__STATIC_INLINE uint32_t LL_C2_EXTI_ReadFlag_32_63(uint32_t ExtiLine)
{
  return (uint32_t)(READ_BIT(EXTI->C2PR2, ExtiLine));
}

/**
  * @brief  Clear ExtLine Flags for Lines in range 0 to 31 for cpu2
  * @note This bit is set when the selected edge event arrives on the interrupt
  *       line. This bit is cleared by writing a 1 to the bit.
  * @rmtoll C2PR1          PIFx           LL_C2_EXTI_ClearFlag_0_31
  * @param  ExtiLine This parameter can be a combination of the following values:
  *         @arg @ref LL_EXTI_LINE_0
  *         @arg @ref LL_EXTI_LINE_1
  *         @arg @ref LL_EXTI_LINE_2
  *         @arg @ref LL_EXTI_LINE_3
  *         @arg @ref LL_EXTI_LINE_4
  *         @arg @ref LL_EXTI_LINE_5
  *         @arg @ref LL_EXTI_LINE_6
  *         @arg @ref LL_EXTI_LINE_7
  *         @arg @ref LL_EXTI_LINE_8
  *         @arg @ref LL_EXTI_LINE_9
  *         @arg @ref LL_EXTI_LINE_10
  *         @arg @ref LL_EXTI_LINE_11
  *         @arg @ref LL_EXTI_LINE_12
  *         @arg @ref LL_EXTI_LINE_13
  *         @arg @ref LL_EXTI_LINE_14
  *         @arg @ref LL_EXTI_LINE_15
  *         @arg @ref LL_EXTI_LINE_18
  *         @arg @ref LL_EXTI_LINE_20
  *         @arg @ref LL_EXTI_LINE_21
  *         @arg @ref LL_EXTI_LINE_22
  *         @arg @ref LL_EXTI_LINE_29
  *         @arg @ref LL_EXTI_LINE_30
  *         @arg @ref LL_EXTI_LINE_31
  * @retval None
  */
__STATIC_INLINE void LL_C2_EXTI_ClearFlag_0_31(uint32_t ExtiLine)
{
  WRITE_REG(EXTI->C2PR1, ExtiLine);
}

/**
  * @brief  Clear ExtLine Flags for Lines in range 32 to 63 for cpu2
  * @note This bit is set when the selected edge event arrives on the interrupt
  *       line. This bit is cleared by writing a 1 to the bit.
  * @rmtoll C2PR2          PIFx           LL_C2_EXTI_ClearFlag_32_63
  * @param  ExtiLine This parameter can be a combination of the following values:
  *         @arg @ref LL_EXTI_LINE_32
  *         @arg @ref LL_EXTI_LINE_33
  *         @arg @ref LL_EXTI_LINE_34
  * @retval None
  */
__STATIC_INLINE void LL_C2_EXTI_ClearFlag_32_63(uint32_t ExtiLine)
{
  WRITE_REG(EXTI->C2PR2, ExtiLine);
}

/**
  * @}
  */

/** @defgroup EXTI_LL_EF_Init Initialization and de-initialization functions
  * @{
  */

ErrorStatus LL_EXTI_Init(LL_EXTI_InitTypeDef *EXTI_InitStruct);
ErrorStatus LL_EXTI_DeInit(void);
void LL_EXTI_StructInit(LL_EXTI_InitTypeDef *EXTI_InitStruct);


/**
  * @}
  */

/**
  * @}
  */

/**
  * @}
  */

/**
  * @}
  */

#ifdef __cplusplus
}
#endif

#endif /* __SR5E1_LL_EXTI_H_ */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
