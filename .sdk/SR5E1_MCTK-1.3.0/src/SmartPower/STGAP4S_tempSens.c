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
*	@file	 STGAP4S_tempSens.c
*	@version M4_VERSION
*	@brief	 This file provides firmware functions that implement the features
*			 of the STGAP4S ADC temperature sensing.
*		   
*	@details This file provides firmware functions that implement the features
*			 of the STGAP4S ADC temperature sensing.
 */
#include "STGAP4S.h"
#include "STGAP4S_tempSens.h"
#include "ntc_temperature_sensor.h"
#include "drive_parameters.h"
#include "external_adc_out.h"
#include "mc_config.h"
#include "filter.h"

#ifdef USE_TEMP_EXT_ADC_OUT

#define RMIN 400.0f
#define RMAX 5800.0f
#define TEMP_VALUES_LENGHT 100
float tTable[TEMP_VALUES_LENGHT + 1] = TEMP_VALUES;

Filter_t tempFilter;

#define ADC_CSCUR_MASK 0b00011000
#define ADC_CSCUR_BIT 3

#define ADC_CSCUR(x)  ((x & ADC_CSCUR_MASK)	 >> ADC_CSCUR_BIT )

const float curr_src_uA[] = {0.0f, 300.0f, 600.0f, 1000.0f};

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

int16_t NTC_GetAvTemp_C( NTC_Handle_t * pHandle ) {
  (void)pHandle; /* not used */

  uint8_t NTC_U_RAW_Value_PWMIN = ExtADC_GetADCValue(&TEMP_EXT_ADCOUT_M1);
  float NTC_U_Volt = STGAP4_GetADCVoltFromValue(Devices[TEMP_EXT_ADC_DEVICE_NB], NTC_U_RAW_Value_PWMIN); // Devices[5] measures NTC+_U via PWM IN

  // Use the follwing code with the NTC in the power module
  float res_kOhm = estNTC_resistance_kOhm_fromVIN(Devices[TEMP_EXT_ADC_DEVICE_NB], NTC_U_Volt); // Devices[5] measures NTC+_U
  float temp_deg = getTempFromRes(res_kOhm * 1000.0f); // Check the TEMP_VALUES defined in tempSens.h

  // Jsut for testing using internal temp sensor (Device[5].cfg4)
  //float temp_deg = getTempFromVin(NTC_U_Volt); // Using internal temperature sensor

  int16_t retVal = (int16_t)(FilterNewSample(&tempFilter, temp_deg));

  return (retVal);
}

float estNTC_resistance_kOhm(DeviceType dev) {
  float vin = STGAP4_GetADCVoltFromStatusReg(dev);
  return estNTC_resistance_kOhm_fromVIN(dev, vin);
}

float estNTC_resistance_kOhm_fromVIN(DeviceType dev, float vin) {
  uint8_t cfg = dev.Cfg4.ActualValue;
  float curr_uA = curr_src_uA[ADC_CSCUR(cfg)];
  return (vin / curr_uA) * 1000.0f;
}

float getTempFromRes(float res) {

  //uint8_t index = approxU8(((res - RMIN) * (float)(TEMP_VALUES_LENGHT)) / (RMAX - RMIN));
  uint8_t index = ExtADC_approx(&TEMP_EXT_ADCOUT_M1,((res - RMIN) * (float)(TEMP_VALUES_LENGHT)) / (RMAX - RMIN));

  if (index > TEMP_VALUES_LENGHT) {
	index = TEMP_VALUES_LENGHT;
  }

  float tempDeg = tTable[index];
  return tempDeg;
}

float getTempFromVin(float vin) {
  return (1.5f - vin)/0.00366f;
}

#endif /* #ifdef USE_TEMP_EXT_ADC_OUT */
