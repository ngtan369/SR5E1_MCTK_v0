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
*   @file    PI.c
*   @version M4_VERSION
*   @brief   This file provides firmware functions that implement the features
*            of the PI component of the Stellar Motor Control.
*
*   @details This file provides firmware functions that implement the features
*            of the PI component of the Stellar Motor Control.
*
 */

#include "PI.h"
#include "mc_config.h"

/**
  * @brief  This function compute the output of a PI regulator sum of its 
  *         proportional and integral terms
  * @param  CPI PI regulator object
  * @param  int32_t Present process variable error, intended as the reference 
  *         value minus the present process variable value
  * @retval int16_t PI output
  */
#if defined (MCTK_ITCM)
int16_t __attribute__((section(".itcm"))) PI_Calc(PIReg *this, int32_t wProcessVarError)
#else
int16_t PI_Calc(PIReg *this, int32_t wProcessVarError)
#endif
{
  int32_t wProportional_Term, wIntegral, wOutput_32,wIntegral_sum_temp;
  int32_t wDischarge = 0;
  int16_t hUpperOutputLimit = this->hUpperLimit;
  int16_t hLowerOutputLimit = -this->hUpperLimit;
  
  /* Proportional term computation*/
  wProportional_Term = this->hKpGain * wProcessVarError;
  
  /* Integral term computation */
  if (this->hKiGain == 0)
  {
    this->wIntegralTerm = 0;
  }
  else
  { 
    wIntegral = this->hKiGain * wProcessVarError;
    wIntegral_sum_temp = this->wIntegralTerm + wIntegral;
    
    if (wIntegral_sum_temp < 0)
    {
      if (this->wIntegralTerm > 0)
      {
        if (wIntegral > 0)
        {
          wIntegral_sum_temp = INT32_MAX;
        }
      }
    }
    else
    {
      if (this->wIntegralTerm < 0)
      {
        if (wIntegral < 0)
        {
          wIntegral_sum_temp = -INT32_MAX;
        }
      }
    }		

    if (wIntegral_sum_temp > this->wUpperIntegralLimit)
    {
      this->wIntegralTerm = this->wUpperIntegralLimit;
    }
    else if (wIntegral_sum_temp < this->wLowerIntegralLimit)
    { 
      this->wIntegralTerm = this->wLowerIntegralLimit;
    }
    else
    {
      this->wIntegralTerm = wIntegral_sum_temp;
    }
  }
  
  wOutput_32 = (wProportional_Term/(int32_t)(this->hKpDivisor))+
               (this->wIntegralTerm/(int32_t)(this->hKiDivisor));

  if (wOutput_32 > hUpperOutputLimit)
  {
    
    wDischarge = hUpperOutputLimit - wOutput_32;
    wOutput_32 = hUpperOutputLimit;		  			 	
  }
  else if (wOutput_32 < hLowerOutputLimit)
  {
    
    wDischarge = hLowerOutputLimit - wOutput_32; 
    wOutput_32 = hLowerOutputLimit;
  }
  else
  {}
  
  this->wIntegralTerm += wDischarge;
  
  return((int16_t)(wOutput_32)); 	
}