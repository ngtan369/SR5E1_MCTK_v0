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
*   @file    max_torque_per_ampere.c
*   @version M4_VERSION
*   @brief   This file provides firmware functions that implement the features
*            of the Maximum Torque Per Ampere (MTPA) Control component of the Stellar Motor Control.
*
*   @details This file provides firmware functions that implement the features
*            of the Maximum Torque Per Ampere (MTPA) Control component of the Stellar Motor Control:
*
*            - Initialize the parameter for  MTPA
*            - Calculate and output id and iq reference based on torque input
*            - Calculate and output id based on iq input
 */

/* Includes ------------------------------------------------------------------*/
#include "max_torque_per_ampere.h"
#include <stdint.h>
/** @addtogroup STELLAR_PMSM_MC_FOC_LIBRARY
  * @{
  */

/** @defgroup MTPA Maximum Torque Per Ampere Control
  * @brief Maximum Torque Per Ampere (MTPA) Control component of the Stellar Motor Control
  *
  *  The torque of the PMSM can be expressed with the equation shown below:
  *
  *  @f[
  *  T_e = \frac{3}{2}\times p \times \phi \times i_q + \frac{3}{2}\times p\times(L_d-L_q)\times i_q\times i_d
  *  @f]
  *
  *    When the motor is surface permanent magnet synchronous motor SPMSM the @f$ L_d @f$ and @f$ L_q @f$
  *  is almost same, so only the @f$ i_q @f$ can influence the torque. For internal permanent
  *  magnet synchronous motor the @f$ L_d @f$  is not equel to @f$ L_q @f$. Both @f$ i_d @f$ and @f$ i_q @f$ will influence
  *  the torque.
  *    The aim of the MTPA (maximum-torque-per-ampere) control is to calculate the
  *  reference currents (@f$ i_qs@f$ , @f$ i_ds@f$) which maximize the ratio between produced
  *  electro-magnetic torque and copper losses.
  *    Call this function after you get the @f$ I_q @f$  refernece.the input of this component
  * is the @f$ i_q @f$  reference, the output of this component is @f$ i_d @f$ reference.
  *
  * @{
  */

/**
  * @brief  Calculate and output the Id based on input Iq
  * @param  Param1: MTPA_Handle_t   *pHandle
  *         Param2: qd_t Iqdref. 
  *
  * @retval Id value
  */
__weak void MTPA_CalcCurrRefFromIq( MTPA_Handle_t * pHandle, qd_t *Iqdref )
{
  int32_t id;
  int16_t aux;
  int16_t iq;
  uint8_t segment;

  iq = ( (Iqdref->q < 0) ? ( -Iqdref->q ) : ( Iqdref->q ) ); /* Teref absolute value */

  aux = iq / pHandle->SegDiv;
  segment = ( uint8_t )aux;

  if ( segment > SEGMENT_NUM )
  {
    segment = SEGMENT_NUM;
  }
  
#if defined (__ICCARM__)
  /* index for array access is verified above */
  #pragma cstat_disable = "MISRAC2012-Rule-18.1_d"
#endif /* __ICCARM__ */
  
  id = (( pHandle->AngCoeff[segment] * iq ) / 32768) + pHandle->Offset[segment];

#if defined (__ICCARM__)
  #pragma cstat_restore = "MISRAC2012-Rule-18.1_d"
#endif /* __ICCARM__ */
  
  Iqdref->d = (int16_t)id;
  
  return;
}

/**
  * @}
  */

/**
  * @}
  */

/************************ (C) COPYRIGHT 2023 STMicroelectronics *****END OF FILE****/
