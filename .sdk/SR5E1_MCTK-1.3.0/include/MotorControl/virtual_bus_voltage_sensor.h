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
*   @file    virtual_bus_voltage_sensor.h
*   @version M4_VERSION
*   @brief   This file provides firmware functions that implement the features
*            of the Virtual Bus Voltage Sensor component of the Stellar Motor Control.
*
*   @details This file provides firmware functions that implement the features
*            of the Virtual Bus Voltage Sensor component of the Stellar Motor Control.
*
*   @ingroup VirtualBusVoltageSensor
 */
/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __VIRTUAL_BUSVOLTAGESENSOR_H
#define __VIRTUAL_BUSVOLTAGESENSOR_H

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

/* Includes ------------------------------------------------------------------*/
#include "bus_voltage_sensor.h"

/** @addtogroup STELLAR_PMSM_MC_FOC_LIBRARY
  * @{
  */

/** @addtogroup BusVoltageSensor
  * @{
  */

/** @addtogroup VirtualBusVoltageSensor
  * @{
  */

/**
  * @brief  Virtual Vbus sensor class parameters definition
  */
typedef struct
{
  BusVoltageSensor_Handle_t _Super;

  uint16_t ExpectedVbus_d;            /*!< Expected Vbus voltage expressed in
                                           digital value
                                           hOverVoltageThreshold(digital value)=
                                           Over Voltage Threshold (V) * 65536
                                           / 500 */
} VirtualBusVoltageSensor_Handle_t;

/* Exported functions ------------------------------------------------------- */
void VVBS_Init( VirtualBusVoltageSensor_Handle_t * pHandle );
void VVBS_Clear( VirtualBusVoltageSensor_Handle_t * pHandle );
uint16_t VVBS_NoErrors( VirtualBusVoltageSensor_Handle_t * pHandle );

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

#endif /* __CCC_H */

/************************ (C) COPYRIGHT 2023 STMicroelectronics *****END OF FILE****/

