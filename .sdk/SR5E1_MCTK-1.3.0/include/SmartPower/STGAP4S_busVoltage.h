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
*   @file    STGAP4S_busVoltage.h
*   @version M4_VERSION
*   @brief   This file provides firmware functions that implement the features
*            of the STGAP4S bus voltage.
*          
*   @details This file provides firmware functions that implement the features
*            of the STGAP4S bus voltage.
*
 */
#ifndef INCLUDE_BUSVOLTAGE_H_
#define INCLUDE_BUSVOLTAGE_H_

/*
 * @addtogroup STELLAR_STGAP4S_Library
 * @{
 */
 
 /** @addtogroup STELLAR_STGAP4S_Library
  * @{
  */

/** @defgroup STGAP4S Gate Sensing Driver (BUS)
  *
  * @brief STGAP4S Gate Sensing Driver (BUS)
  *
  * @{
  */
float convertBusVolt(float adcVolt, float pullUpRes, float pullDownRes);

#endif /* INCLUDE_BUSVOLTAGE_H_ */
