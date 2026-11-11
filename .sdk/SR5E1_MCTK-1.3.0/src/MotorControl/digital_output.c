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
*   @file    digital_output.c
*   @version M4_VERSION
*   @brief   This file provides firmware functions that implement the Digital
*            Output component of the Stellar Motor Control.
*          
*   @details This file provides firmware functions that implement the Digital
*            Output component of the Stellar Motor Control.
 */

/* Includes ------------------------------------------------------------------*/
#include "digital_output.h"
#include "mc_type.h"

/** @addtogroup STELLAR_PMSM_MC_FOC_LIBRARY
  * @{
  */

/** @defgroup DigitalOutput Digital Output
  * @brief digital output component of the Stellar Motor Control
  *
  * @{
  */


/**
* @brief Accordingly with selected polarity, it sets to active or inactive the
*        digital output
* @param pHandle handler address of the digital output component.
* @param OutputState_t New requested state
* @retval none
*/
__weak void DOUT_SetOutputState( DOUT_handle_t * pHandle, DOutputState_t State )
{

  if ( State == ACTIVE )
  {
    if ( pHandle->bDOutputPolarity == DOutputActiveHigh )
    {
      LL_GPIO_SetOutputPin( pHandle->hDOutputPort, pHandle->hDOutputPin );
    }
    else
    {
      LL_GPIO_ResetOutputPin( pHandle->hDOutputPort, pHandle->hDOutputPin );
    }
  }
  else if ( pHandle->bDOutputPolarity == DOutputActiveHigh )
  {
    LL_GPIO_ResetOutputPin( pHandle->hDOutputPort, pHandle->hDOutputPin );
  }
  else
  {
    LL_GPIO_SetOutputPin( pHandle->hDOutputPort, pHandle->hDOutputPin );
  }
  pHandle->OutputState = State;
}

/**
* @brief It returns the state of the digital output
* @param pHandle pointer on related component instance
* @retval OutputState_t Digital output state (ACTIVE or INACTIVE)
*/
__weak DOutputState_t DOUT_GetOutputState( DOUT_handle_t * pHandle )
{
  return ( pHandle->OutputState );
}

/**
  * @}
  */

/**
  * @}
  */

/************************ (C) COPYRIGHT 2023 STMicroelectronics *****END OF FILE****/
