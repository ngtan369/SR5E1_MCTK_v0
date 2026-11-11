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
*   @file    sr5e1_ll_cordic.c
*   @version M4_VERSION
*   @brief   CORDIC LL module driver.
*
*   @details CORDIC LL module driver.
*
 */
/* Includes ------------------------------------------------------------------*/
#include "sr5e1_ll_cordic.h"
#include <clock.h>

#ifdef  USE_FULL_ASSERT
#include "sr5e1_assert.h"
#else
#define assert_param(expr) ((void)0U)
#endif

/** @addtogroup SR5E1_LL_Driver
  * @{
  */

/** @addtogroup CORDIC_LL
  * @{
  */

/* Private types -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private constants ---------------------------------------------------------*/
/* Private macros ------------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/

/* Exported functions --------------------------------------------------------*/
/** @addtogroup CORDIC_LL_Exported_Functions
  * @{
  */

/** @addtogroup CORDIC_LL_EF_Init
  * @{
  */

/**
  * @brief  De-Initialize CORDIC peripheral registers to their default reset values.
  * @param  CORDICx CORDIC Instance
  * @retval An ErrorStatus enumeration value:
  *          - SUCCESS: CORDIC registers are de-initialized
  *          - ERROR: CORDIC registers are not de-initialized
  */
ErrorStatus LL_CORDIC_DeInit(CORDIC_TypeDef *CORDICx)
{
  ErrorStatus status = SUCCESS;

  /* Check the parameters */
  assert_param(IS_CORDIC_ALL_INSTANCE(CORDICx));

  if (CORDICx == CORDIC)
  {
    clock_reset_CORDIC();
  }
  else
  {
    status = ERROR;
  }

  return (status);
}

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

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
