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
*   @file    PLL.c
*   @version M4_VERSION
*   @brief   This file provides firmware functions that implement the features
*            of the PLL component of the Stellar Motor Control.
*
*   @details This file provides firmware functions that implement the features
*            of the PLL component of the Stellar Motor Control.
*
 */

#include "PLL.h"
#include "MC_math.h"

/**
 * @brief  This function compute the PLL. 
 * @param  pHandle: handler of the current instance of the PID component
 * @param  hSin: SIN
 * @param  hCos: COS
 * @retval PLL output
 */
int16_t PLL_Calc(PLL *this, int16_t hSin, int16_t hCos)
{
  int32_t wAlfa_Sin_tmp, wBeta_Cos_tmp;
  int16_t hRotor_Speed;
  Trig_Components Local_Components;
  int16_t hAux1, hAux2;
    
  Local_Components = MCM_Trig_Functions(this->hElAngle);
  
  /* Alfa & Beta BEMF multiplied by Cos & Sin*/
  wAlfa_Sin_tmp = (int32_t)(hSin)*(int32_t)Local_Components.hSin;
  wBeta_Cos_tmp = (int32_t)(hCos)*(int32_t)Local_Components.hCos;
  
#ifdef FULL_MISRA_C_COMPLIANCY
  hAux1 = (int16_t)(wBeta_Cos_tmp/32768);
#else
  hAux1 = (int16_t)(wBeta_Cos_tmp >> 15);
#endif
  
#ifdef FULL_MISRA_C_COMPLIANCY
  hAux2 = (int16_t)(wAlfa_Sin_tmp/32768); 
#else
  hAux2 = (int16_t)(wAlfa_Sin_tmp >> 15);
#endif
  
  /* Speed PI regulator */
  hRotor_Speed = PI_Calc(&(this->pi), (int32_t)(hAux1)-hAux2);
  
  this->hElAngle += hRotor_Speed;
  this->hSpeedDpp = hRotor_Speed;
  
  return (hRotor_Speed);
}