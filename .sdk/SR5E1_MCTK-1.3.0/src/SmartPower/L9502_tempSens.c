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
*	@file	 L9502_tempSens.c
*	@version M4_VERSION
*	@brief	 This file provides firmware functions that implement the features
*			 of the L9502 ADC temperature sensing.
*		   
*	@details This file provides firmware functions that implement the features
*			 of the L9502 ADC temperature sensing.
 */
#include "L9502.h"
#include "L9502_tempSens.h"
#include "ntc_temperature_sensor.h"
#include "drive_parameters.h"
#include "external_adc_out.h"
#include "mc_config.h"
#include "filter.h"

#ifdef USE_TEMP_EXT_ADC_OUT

Filter_t tempFilter;

#define R_PULLUP_TEMPERATURE 3 /* kOhm */
#define RMIN 400.0f
#define RMAX 5800.0f
#define TEMP_VALUES_LENGHT 100

float tTable[TEMP_VALUES_LENGHT + 1] = TEMP_VALUES;


void NTC_Init( NTC_Handle_t * pHandle )
{
  (void)pHandle; /* not used */
  FilterInit(&tempFilter);
}

uint16_t NTC_CalcAvTemp(NTC_Handle_t *pHandle) {
  uint16_t NTCTempC = NTC_GetAvTemp_C(pHandle);
  if (NTCTempC > OV_TEMPERATURE_THRESHOLD_C) {
	pHandle->hFaultState = MC_OVER_TEMP;
  } else if (NTCTempC < OV_TEMPERATURE_THRESHOLD_C - OV_TEMPERATURE_HYSTERESIS_C) {
	pHandle->hFaultState = MC_NO_ERROR;
  }
  return pHandle->hFaultState;
}


static float L9502_ConvertADC(uint16_t value) {
  float v1 = 0.0f;
  float v2 = TEMP_ADC_REFERENCE_VOLTAGE;

  return v1 + ((value * (v2 - v1)) / 1023.0f);
}

int16_t NTC_GetAvTemp_C( NTC_Handle_t * pHandle ) {
  (void)pHandle; /* not used */
  
  uint16_t NTC_U_RAW_Value_PWMIN = ExtADC_GetADCValue(&TEMP_EXT_ADCOUT_M1);
  
  float AIN_IN_Volt_PWM = L9502_ConvertADC(NTC_U_RAW_Value_PWMIN);
  
  // Use the follwing code with the NTC in the power module
  float res_kOhm = estNTC_resistance_kOhm(AIN_IN_Volt_PWM);
  float temp_deg = getTempFromRes(res_kOhm * 1000.0f); // Check the TEMP_VALUES defined in tempSens.h

#if 0
  int32_t wTemp;
  wTemp = ( int32_t )(  AIN_IN_Volt_PWM *65536/ TEMP_ADC_REFERENCE_VOLTAGE );
  wTemp -= ( int32_t )( pHandle->wV0 );
  wTemp *= pHandle->hSensitivity;
  wTemp = wTemp / 65536 + ( int32_t )( pHandle->hT0 );
#endif
  int16_t retVal = (int16_t)(FilterNewSample(&tempFilter, temp_deg));

  return (retVal);
}

float estNTC_resistance_kOhm(float voltage_Read) {
  float res = (voltage_Read / TEMP_ADC_REFERENCE_VOLTAGE) * R_PULLUP_TEMPERATURE;
  return res;
}

float getTempFromRes(float res) {

  uint8_t index = ExtADC_approx(&TEMP_EXT_ADCOUT_M1,((res - RMIN) * (float)(TEMP_VALUES_LENGHT)) / (RMAX - RMIN));

  if (index > TEMP_VALUES_LENGHT) {
	index = TEMP_VALUES_LENGHT;
  }

  float tempDeg = tTable[index];
  return tempDeg;
}


#endif /* #ifdef USE_TEMP_EXT_ADC_OUT */
