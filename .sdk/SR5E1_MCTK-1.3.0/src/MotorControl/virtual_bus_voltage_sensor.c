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
*   @file    virtual_bus_voltage_sensor.c
*   @version M4_VERSION
*   @brief   This file provides firmware functions that implement the features
*            of the Virtual Bus Voltage Sensor component of the Stellar Motor Control.
*
*   @details This file provides firmware functions that implement the features
*            of the Virtual Bus Voltage Sensor component of the Stellar Motor Control.
*
 */

/* Includes ------------------------------------------------------------------*/
#include "virtual_bus_voltage_sensor.h"


/** @addtogroup STELLAR_PMSM_MC_FOC_LIBRARY
  * @{
  */

/** @addtogroup BusVoltageSensor
  * @{
  */

/** @defgroup VirtualBusVoltageSensor Virtual Bus Voltage Sensor
  * @brief Virtual Bus Voltage Sensor implementation.
  *
  * @todo Document the Virtual Bus Voltage Sensor "module".
  *
  * @{
  */

/**
  * @brief  It initializes bus voltage conversion for virtual bus voltage sensor
  * @param  pHandle related Handle of VirtualBusVoltageSensor_Handle_t
  * @retval none
  */
__weak void VVBS_Init( VirtualBusVoltageSensor_Handle_t * pHandle )
{
  pHandle->_Super.FaultState = MC_NO_ERROR;
  pHandle->_Super.LatestConv = pHandle->ExpectedVbus_d;
  pHandle->_Super.AvBusVoltage_d = pHandle->ExpectedVbus_d;
}

/**
  * @brief  It simply returns in virtual Vbus sensor implementation
  * @param  pHandle related Handle of VirtualBusVoltageSensor_Handle_t
  * @retval none
  */
__weak void VVBS_Clear( VirtualBusVoltageSensor_Handle_t * pHandle )
{
  (void)pHandle; /* not used */
  return;
}

/**
  * @brief  It returns MC_NO_ERROR
  * @param  pHandle related Handle of VirtualBusVoltageSensor_Handle_t
* @retval uint16_t Fault code error: MC_NO_ERROR
  */
__weak uint16_t VVBS_NoErrors( VirtualBusVoltageSensor_Handle_t * pHandle )
{
  (void) pHandle; /* not used */
  return ( MC_NO_ERROR );
}

/**
  * @}
  */

/**
  * @}
  */

/** @} */

/************************ (C) COPYRIGHT 2023 STMicroelectronics *****END OF FILE****/

