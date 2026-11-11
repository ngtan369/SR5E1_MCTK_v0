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
*   @file    maximizer.c
*   @version M4_VERSION
*   @brief   This file provides firmware functions that implement the maximizer
*            features component of the Stellar Motor Control.
*
*   @details This file provides firmware functions that implement the maximizer
*            features component of the Stellar Motor Control.
*
 */

#include "maximizer.h"

void maximizer_Init(maximizer_t *this, float xmin, float xmax)
{
  this->initAlg = 0;
  this->xmax = xmax;
  this->xmin = xmin;
}

float maximizer_Calc(maximizer_t *this, float y)
{
  if (this->initAlg == 0)
  {
    this->initAlg = 1;
  }
  else if (this->initAlg == 1)
  {
    this->dir = 1.0f;
    this->yprev = y;
    this->x += this->delta;
    this->initAlg = 2;
  }
  else
  {
    float grad = (y - this->yprev)/this->dir;
    if (grad > 0.0f)
    {
      this->dir = 1.0f;
      this->x += this->delta;
    }
    else
    {
      this->dir = -1.0f;
      this->x -= this->delta;
    }
    this->yprev = y;
  }
  
  if (this->x < this->xmin)
  {
    this->x = this->xmax;
  }
  
  if (this->x > this->xmax)
  {
    this->x = this->xmin;
  }
  
  return this->x;
}