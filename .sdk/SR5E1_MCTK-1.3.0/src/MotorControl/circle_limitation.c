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
*   @file    circle_limitation.c
*   @version M4_VERSION
*   @brief   This file provides the functions that implement the circle
*            limitation feature of the Stellar Motor Control.
*          
*   @details This file provides the functions that implement the circle
*            limitation feature of the Stellar Motor Control
 */

/* Includes ------------------------------------------------------------------*/
#include "circle_limitation.h"
#include "mc_math.h"
#include "mc_type.h"
#include "mc_config.h"

/** @addtogroup STELLAR_PMSM_MC_FOC_LIBRARY
  * @{
  */

/** @defgroup CircleLimitation Circle Limitation
  * @brief Circle Limitation component of the Stellar Motor Control
  *
  * @todo Document the Circle Limitation "module".
  *
  * @{
  */

#if defined (CIRCLE_LIMITATION_VD)

#if defined (MCTK_ITCM)
  __weak qd_t __attribute__((section(".itcm"))) Circle_Limitation(CircleLimitation_Handle_t * pHandle, qd_t Vqd)
#else
__weak qd_t Circle_Limitation(CircleLimitation_Handle_t * pHandle, qd_t Vqd)
#endif
{
  int32_t MaxModule;
  int32_t square_q;
  int32_t square_temp;
  int32_t square_d;
  int32_t square_sum;
  int32_t square_limit;
  int32_t vd_square_limit;
  int32_t new_q;
  int32_t new_d;
  qd_t Local_Vqd=Vqd;

  MaxModule = pHandle->MaxModule;

  square_q = (int32_t)(Vqd.q) * Vqd.q;
  square_d = (int32_t)(Vqd.d) * Vqd.d;
  square_limit = MaxModule * MaxModule;
  vd_square_limit = pHandle->MaxVd * pHandle->MaxVd;
  square_sum = square_q + square_d;

  if (square_sum > square_limit)
  {
    if(square_d <= vd_square_limit)
    {
      square_temp = square_limit - square_d;
      new_q = MCM_Sqrt(square_temp);
      if(Vqd.q < 0)
      {
        new_q = -new_q;
      }
      new_d = Vqd.d;
    }
    else
    {
      new_d = pHandle->MaxVd;
      if(Vqd.d < 0)
      {
        new_d = -new_d;
      }

      square_temp = square_limit - vd_square_limit;
      new_q = MCM_Sqrt(square_temp);
      if(Vqd.q < 0)
      {
        new_q = - new_q;
      }
    }
    Local_Vqd.q = new_q;
    Local_Vqd.d = new_d;
  }
  return(Local_Vqd);
}
#else

/**
  * @brief Check whether Vqd.q^2 + Vqd.d^2 <= 32767^2
  *        and if not it applies a limitation keeping constant ratio
  *        Vqd.q / Vqd.d
  * @param  pHandle pointer on the related component instance
  * @param  Vqd Voltage in qd reference frame
  * @retval qd_t Limited Vqd vector
  */
#if defined (MCTK_ITCM)
__weak qd_t __attribute__((section(".itcm"))) Circle_Limitation(CircleLimitation_Handle_t * pHandle, qd_t Vqd)
#else
__weak qd_t Circle_Limitation( CircleLimitation_Handle_t * pHandle, qd_t Vqd )
#endif
{
  uint16_t table_element;
  uint32_t uw_temp;
  int32_t  sw_temp;
  qd_t local_vqd = Vqd;

  sw_temp = ( int32_t )( Vqd.q ) * Vqd.q +
            ( int32_t )( Vqd.d ) * Vqd.d;

  uw_temp = ( uint32_t ) sw_temp;

  /* uw_temp min value 0, max value 32767*32767 */
  if ( uw_temp > ( uint32_t )( pHandle->MaxModule ) * pHandle->MaxModule )
  {

    uw_temp /= ( uint32_t )( 16777216 );

    /* wtemp min value pHandle->Start_index, max value 127 */
    uw_temp -= pHandle->Start_index;

    /* uw_temp min value 0, max value 127 - pHandle->Start_index */
    table_element = pHandle->Circle_limit_table[( uint8_t )uw_temp];

    sw_temp = Vqd.q * ( int32_t )table_element;
    local_vqd.q = ( int16_t )( sw_temp / 32768 );

    sw_temp = Vqd.d * ( int32_t )( table_element );
    local_vqd.d = ( int16_t )( sw_temp / 32768 );
  }

  return ( local_vqd );
}
#endif

/**
  * @}
  */

/**
  * @}
  */

/************************ (C) COPYRIGHT 2023 STMicroelectronics *****END OF FILE****/