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
*   @file    r_divider_bus_voltage_sensor.h
*   @version M4_VERSION
*   @brief   This file provides firmware functions that implement the features
*            of the Resistor Divider Bus Voltage Sensor component of the Stellar Motor Control.
*
*   @details This file provides firmware functions that implement the features
*            of the Resistor Divider Bus Voltage Sensor component of the Stellar Motor Control.
*
*   @ingroup RDividerBusVoltageSensor
 */
/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __RDIVIDER_BUSVOLTAGESENSOR_H
#define __RDIVIDER_BUSVOLTAGESENSOR_H

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

/* Includes ------------------------------------------------------------------*/
#include "regular_conversion_manager.h"
#include "bus_voltage_sensor.h"

/** @addtogroup STELLAR_PMSM_MC_FOC_LIBRARY
  * @{
  */

/** @addtogroup BusVoltageSensor
  * @{
  */

/** @addtogroup RDividerBusVoltageSensor
  * @{
  */


/**
  * @brief  Rdivider class parameters definition
  */
typedef struct
{
  BusVoltageSensor_Handle_t _Super;
#ifndef USE_VBUS_EXT_ADC_OUT
  RegConv_t      VbusRegConv; 
#endif
  uint16_t       LowPassFilterBW;       /*!< Use this number to configure the Vbus
                                             first order software filter bandwidth.
                                             hLowPassFilterBW = VBS_CalcBusReading
                                             call rate [Hz]/ FilterBandwidth[Hz] */
  uint16_t       OverVoltageThreshold;  /*!< It represents the over voltage protection
                                             intervention threshold. To be expressed
                                             in digital value through formula:
                                             hOverVoltageThreshold (digital value) =
                                             Over Voltage Threshold (V) * 65536
                                             / hConversionFactor */
  uint16_t       UnderVoltageThreshold; /*!< It represents the under voltage protection
                                             intervention threshold. To be expressed
                                             in digital value through formula:
                                             hUnderVoltageThreshold (digital value)=
                                             Under Voltage Threshold (V) * 65536
                                             / hConversionFactor */
  uint16_t       *aBuffer;                /*!< Buffer used to compute average value.*/
  uint8_t        elem;                  /*!< Number of stored elements in the average buffer.*/
  uint8_t        index;                 /*!< Index of last stored element in the average buffer.*/
  uint8_t        convHandle;            /*!< handle to the regular conversion */

} RDivider_Handle_t;

/* Exported functions ------------------------------------------------------- */
void RVBS_Init( RDivider_Handle_t * pHandle );
void RVBS_Clear( RDivider_Handle_t * pHandle );
uint16_t RVBS_CalcAvVbusFilt( RDivider_Handle_t * pHandle );
uint16_t RVBS_CalcAvVbus( RDivider_Handle_t * pHandle );
uint16_t RVBS_CheckFaultState( RDivider_Handle_t * pHandle );

/**
  * @}
  */

/**
  * @}
  */

/** @} */

#ifdef __cplusplus
}
#endif /* __cpluplus */

#endif /* __RDividerBusVoltageSensor_H */

/************************ (C) COPYRIGHT 2023 STMicroelectronics *****END OF FILE****/

