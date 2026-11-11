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
*   @file    PI.h
*   @version M4_VERSION
*   @brief   This file provides firmware functions that implement the features
*            of the PI component of the Stellar Motor Control.
*
*   @details This file provides firmware functions that implement the features
*            of the PI component of the Stellar Motor Control.
*
 */
#ifndef __PI_H
#define __PI_H

#include "MC_type.h"


typedef struct
{
  int16_t hKpGain;
  int16_t hKiGain;
  int32_t wIntegralTerm;

  int16_t hUpperLimit;
  int32_t hKpDivisor;
  int32_t hKiDivisor;
  int32_t wUpperIntegralLimit;
  int32_t wLowerIntegralLimit;
  
} PIReg;

int16_t PI_Calc(PIReg *this, int32_t wProcessVarError);

#endif /* __PI_H */