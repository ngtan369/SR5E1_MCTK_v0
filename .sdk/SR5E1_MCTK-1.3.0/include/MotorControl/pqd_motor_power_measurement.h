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
*   @file    pqd_motor_power_measurement.h
*   @version M4_VERSION
*   @brief   This file provides firmware functions that implement the features
*            of the PQD Motor Power Measurement component of the Stellar Motor Control.
*
*   @details This file provides firmware functions that implement the features
*            of the PQD Motor Power Measurement component of the Stellar Motor Control.
*
*   @ingroup motorpowermeasurement
 */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __PQD_MOTORPOWERMEASUREMENT_H
#define __PQD_MOTORPOWERMEASUREMENT_H

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

/* Includes ------------------------------------------------------------------*/
#include "motor_power_measurement.h"
#include "bus_voltage_sensor.h"


/** @addtogroup STELLAR_PMSM_MC_FOC_LIBRARY
  * @{
  */

/** @addtogroup motorpowermeasurement
  * @{
  */

/** @addtogroup pqd_motorpowermeasurement
  * @{
  */

typedef struct
{
  MotorPowMeas_Handle_t _super;

  int32_t wConvFact; /* It is the conversion factor used to convert the
                         variables expressed in digit into variables expressed
                         in physical measurement unit. It is used to convert the
                         power in watts. It must be equal to
                         (1000 * 3 * Vddï¿½)/(sqrt(3) * Rshunt * Aop) */

  pFOCVars_t pFOCVars;    /*!< Pointer to FOC vars used by MPM.*/
  BusVoltageSensor_Handle_t * pVBS;              /*!< Bus voltage sensor object used by MPM.*/
} PQD_MotorPowMeas_Handle_t;



/**
  * @brief Implementation of derived class init method. It should be called before each motor restart.
  * @param pHandle related component instance.
  * @retval none.
  */
void PQD_Clear( PQD_MotorPowMeas_Handle_t * pHandle );

/**
  * @brief Implementation of derived class CalcElMotorPower.
  * @param pHandle related component instance.
  * @retval int16_t The measured motor power expressed in watt.
  */
void PQD_CalcElMotorPower( PQD_MotorPowMeas_Handle_t * pHandle );


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
