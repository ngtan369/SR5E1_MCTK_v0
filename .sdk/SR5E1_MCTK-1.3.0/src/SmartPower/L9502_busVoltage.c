/****************************************************************************
*
* Copyright (c) 2024 STMicroelectronics - All Rights Reserved
*
* License terms: STMicroelectronics Proprietary in accordance with licensing
* terms SLA0098 at www.st.com.
*
* THIS SOFTWARE IS DISTRIBUTED "AS IS," AND ALL WARRANTIES ARE DISCLAIMED,
* INCLUDING MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE.
*****************************************************************************/
/**
*   @file    L9502_busVoltage.c
*   @version M4_VERSION
*   @brief   This file provides firmware functions that implement the features
*            of the L9502 bus voltage.
*          
*   @details This file provides firmware functions that implement the features
*            of the L9502 bus voltage.
*
 */
#include "L9502.h"
#include "L9502_busVoltage.h"
#include "r_divider_bus_voltage_sensor.h"
#include "drive_parameters.h"
#include "mc_config.h"
#include "filter.h"

/*
 * @addtogroup STELLAR_L9502_Library
 * @{
 */
 
#ifdef USE_VBUS_EXT_ADC_OUT

Filter_t vdcFilter;

 /**
  * @brief	This function initialiaze L9502 Bus Sensing\n
  *			
  * @param	RDivider_Handle_t * pHandle
  * @return void
  */
void RVBS_Init( RDivider_Handle_t * pHandle )
{
  (void)pHandle; /* not used */
  FilterInit(&vdcFilter);
}

 /**
  * @brief	This function compute L9502 Bus voltage\n
  *			
  * @param	RDivider_Handle_t * pHandle
  * @return void
  */
uint16_t RVBS_CalcAvVbus(RDivider_Handle_t *pHandle) {
  uint16_t retVal;
  uint16_t BUSVolt = VBS_GetAvBusVoltage_V(&pHandle->_Super);
  if (BUSVolt > OV_VOLTAGE_THRESHOLD_V) {
    retVal = MC_OVER_VOLT;
  } else if (BUSVolt < UD_VOLTAGE_THRESHOLD_V) {
    retVal = MC_UNDER_VOLT;
  } else {
    retVal = MC_NO_ERROR;
  }
  pHandle->_Super.AvBusVoltage_d = (uint16_t)((int32_t)(BUSVolt* 65536)/pHandle->_Super.ConversionFactor);
  return retVal;
}



static float L9502_ConvertADC(uint16_t value) {
  float v1 = 0.0f;
  float v2 = 5.7f;
  return v1 + ((value * (v2 - v1)) / 1023.0f);
}

 /**
  * @brief	This function get L9502 Bus voltage\n
  *			
  * @param	BusVoltageSensor_Handle_t * pHandle
  * @return void
  */
uint16_t VBS_GetAvBusVoltage_V( BusVoltageSensor_Handle_t* pHandle )
{
  (void)pHandle; /* not used */

  uint16_t AIN_RAW_Value_PWMIN = ExtADC_GetADCValue(&VBUS_EXT_ADCOUT_M1);

  float AIN_IN_Volt_PWM = L9502_ConvertADC(AIN_RAW_Value_PWMIN);

  float vBUSVolt = convertBusVolt(AIN_IN_Volt_PWM, VBUS_NETWORK_PULLUP_RES_OHM, VBUS_NETWORK_PULLDW_RES_OHM);

  uint16_t retVal = (uint16_t)(FilterNewSample(&vdcFilter, vBUSVolt));

  return (retVal);
}

float convertBusVolt(float adcVolt, float pullUpRes, float pullDownRes) {
  return adcVolt * (pullUpRes + pullDownRes) / pullDownRes;
}

#endif /* #ifdef USE_VBUS_EXT_ADC_OUT */
