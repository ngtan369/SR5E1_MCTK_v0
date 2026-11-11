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
*   @file    bus_voltage_sensor.h
*   @version M4_VERSION
*   @brief   This file provides firmware functions that implement the features
*            of the BusVoltageSensor component of the Stellar Motor Control.
*          
*   @details This file provides firmware functions that implement the features
*            of the BusVoltageSensor component of the Stellar Motor Control.
 */
/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __BUSVOLTAGESENSOR_H
#define __BUSVOLTAGESENSOR_H

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

/* Includes ------------------------------------------------------------------*/
#include "mc_type.h"
#include "pwm_curr_fdbk.h"

/** @addtogroup STELLAR_PMSM_MC_FOC_LIBRARY
  * @{
  */

/** @addtogroup BusVoltageSensor
  * @{
  */

/**
  * @brief  BusVoltageSensor handle definition
  */
typedef struct
{
  SensorType_t SensorType;    /*!< It contains the information about the type
                                   of instanced bus voltage sensor object.
                                   It can be equal to REAL_SENSOR or
                                   VIRTUAL_SENSOR */
  uint16_t ConversionFactor;  /*!< It is used to convert bus voltage from
                                   u16Volts into real Volts (V).
                                   1 u16Volt = 65536/hConversionFactor Volts
                                   For real sensors hConversionFactor it's
                                   equal to the product between the expected MCU
                                   voltage and the voltage sensing network
                                   attenuation. For virtual sensors it must
                                   be equal to 500 */

  uint16_t LatestConv;        /*!< It contains latest Vbus converted value
                                   expressed in u16Volts format */
  uint16_t AvBusVoltage_d;    /*!< It contains latest available average Vbus
                                   expressed in digit */
  uint16_t FaultState;        /*!< It contains latest Fault code (MC_NO_ERROR,
                                   MC_OVER_VOLT or MC_UNDER_VOLT) */
} BusVoltageSensor_Handle_t;


/* Exported functions ------------------------------------------------------- */
uint16_t VBS_GetBusVoltage_d( BusVoltageSensor_Handle_t * pHandle );
uint16_t VBS_GetAvBusVoltage_d( BusVoltageSensor_Handle_t * pHandle );
uint16_t VBS_GetAvBusVoltage_V( BusVoltageSensor_Handle_t * pHandle );
uint16_t VBS_CheckVbus( BusVoltageSensor_Handle_t * pHandle );

/**
  * @}
  */

/**
  * @}
  */

#ifdef __cplusplus
}
#endif /* __cpluplus */

#endif /* __BusVoltageSensor_H */

/************************ (C) COPYRIGHT 2023 STMicroelectronics *****END OF FILE****/
