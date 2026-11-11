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
*   @file    bus_voltage_sensor.c
*   @version M4_VERSION
*   @brief   This file provides firmware functions that implement the features
*            of the BusVoltageSensor component of the Stellar Motor Control.
*          
*   @details This file provides firmware functions that implement the features
*            of the BusVoltageSensor component of the Stellar Motor Control.
 */

/* Includes ------------------------------------------------------------------*/

#include "bus_voltage_sensor.h"
#include "mc_config.h"


/** @addtogroup STELLAR_PMSM_MC_FOC_LIBRARY
  * @{
  */

/** @defgroup BusVoltageSensor Bus Voltage Sensor
  * @brief Bus Voltage Sensor components of the Stellar Motor Control
  *
  * Two Bus Voltage Sensor implementations are provided:
  *
  * - The @ref RDividerBusVoltageSensor "Resistor Divider Bus Voltage Sensor" operates as the name suggests
  * - The @ref VirtualBusVoltageSensor "Virtual Bus Voltage Sensor" does not make measurement but rather
  *   returns a fixed, application defined value.
  *
  * @todo Document the Bus Voltage Sensor "module".
  *
  * @{
  */

/**
  * @brief  It return latest Vbus conversion result expressed in u16Volt
  * @param  pHandle related Handle of BusVoltageSensor_Handle_t
  * @retval uint16_t Latest Vbus conversion result in digit
  */
__weak uint16_t VBS_GetBusVoltage_d( BusVoltageSensor_Handle_t * pHandle )
{
  return ( pHandle->LatestConv );
}

/**
  * @brief  It return latest averaged Vbus measurement expressed in u16Volt
  * @param  pHandle related Handle of BusVoltageSensor_Handle_t
  * @retval uint16_t Latest averaged Vbus measurement in digit
  */
#if defined (MCTK_ITCM)
__weak uint16_t __attribute__((section(".itcm"))) VBS_GetAvBusVoltage_d( BusVoltageSensor_Handle_t * pHandle )
#else 
__weak uint16_t VBS_GetAvBusVoltage_d( BusVoltageSensor_Handle_t * pHandle )
#endif
{
  return ( pHandle->AvBusVoltage_d );
}

/**
  * @brief  It return latest averaged Vbus measurement expressed in Volts
  * @param  pHandle related Handle of BusVoltageSensor_Handle_t
  * @retval uint16_t Latest averaged Vbus measurement in Volts
  */
__weak uint16_t VBS_GetAvBusVoltage_V( BusVoltageSensor_Handle_t * pHandle )
{
  uint32_t temp;

  temp = ( uint32_t )( pHandle->AvBusVoltage_d );
  temp *= pHandle->ConversionFactor;
  temp /= 65536u;

  return ( ( uint16_t )temp );
}

/**
  * @brief  It returns MC_OVER_VOLT, MC_UNDER_VOLT or MC_NO_ERROR depending on
  *         bus voltage and protection threshold values
  * @param  pHandle related Handle of BusVoltageSensor_Handle_t
  * @retval uint16_t Fault code error
  */
__weak uint16_t VBS_CheckVbus( BusVoltageSensor_Handle_t * pHandle )
{
  return ( pHandle->FaultState );
}

/**
  * @}
  */

/**
  * @}
  */

/************************ (C) COPYRIGHT 2023 STMicroelectronics *****END OF FILE****/
