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
*   @file    PLL.h
*   @version M4_VERSION
*   @brief   This file provides firmware functions that implement the features
*            of the PLL component of the Stellar Motor Control.
*
*   @details This file provides firmware functions that implement the features
*            of the PLL component of the Stellar Motor Control.
*
 */
#ifndef __PLL_H
#define __PLL_H

#include "MC_type.h"
#include "PI.h"

typedef struct
{
  PIReg pi;
  int16_t hElAngle;
  int16_t hSpeedDpp;
} PLL;

int16_t PLL_Calc(PLL *this, int16_t hSin, int16_t hCos);

#endif /* __PLL_H */