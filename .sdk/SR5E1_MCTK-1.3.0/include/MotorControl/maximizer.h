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
 
/*
*   @file    maximizer.h
*   @version M4_VERSION
*   @brief   This file provides firmware functions that implement the maximizer
*            features component of the Stellar Motor Control.
*
*   @details This file provides firmware functions that implement the maximizer
*            features component of the Stellar Motor Control.
*
 */
#ifndef __MAXIMIZER_H
#define __MAXIMIZER_H

#include "MC_type.h"

typedef struct
{
  float delta;
  float dir;
  float yprev;
  float x;
  float xmax;
  float xmin;
  uint16_t initAlg;
} maximizer_t;

void maximizer_Init(maximizer_t *this, float xmin, float xmax);
float maximizer_Calc(maximizer_t *this, float y);

#endif /* __MAXIMIZER_H */