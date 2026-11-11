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
*   @file    can_frame_communication_protocol.h
*   @version M4_VERSION
*   @brief   This file provides firmware functions that implement the features
*            of the Frame Communication Protocol for CAN component of the Stellar Motor Control.
*          
*   @details This file provides firmware functions that implement the features
*            of the Frame Communication Protocol for CAN component of the Stellar Motor Control.
 */
/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __CFCP_H
#define __CFCP_H

#ifdef __cplusplus
 extern "C" {
#endif /* __cplusplus */

/* Includes ------------------------------------------------------------------*/
#include "frame_communication_protocol.h"

/** @addtogroup STELLAR_PMSM_MC_UI_LIBRARY
  * @{
  */

/**
 * @addtogroup MCUI
 * @{
 */

/** @addtogroup CFCP
  * @{
  */

 /**
  * @brief Error Code for an Overrun FCP message frame.
  *
  * Such a payload is sent to the remote when an CAN overrun error occurs.
  *
  * The value of the error code is set by the Frame Communication Protocol and is taken in
  * a value space that is shared with the Motor Control Protocol. It thus cannot be
  * changed.
  */
#define UFCP_MSG_OVERRUN 0x08

 /* Exported types ------------------------------------------------------------*/
typedef struct {
  FCP_Handle_t _Super;
  FDCAN_TypeDef * FDCANx;
   uint32_t CANClockSource;
  GPIO_TypeDef * RxPort;
  uint16_t RxPin;
  GPIO_TypeDef * TxPort;
  uint16_t TxPin;
  uint8_t UIIRQn;

} CFCP_Handle_t;

/* Exported functions ------------------------------------------------------- */

void CFCP_Init( CFCP_Handle_t * pHandle );

uint8_t CFCP_Receive( FCP_Handle_t * pHandle );

uint8_t CFCP_Send( FCP_Handle_t * pHandle, uint8_t code, uint8_t *buffer, uint8_t size);

void CFCP_AbortReceive( FCP_Handle_t * pHandle );

void * CFCP_RX_IRQ_Handler( CFCP_Handle_t * pHandle, unsigned short rx_data );

void CFCP_TX_IRQ_Handler( CFCP_Handle_t * pHandle );

void CFCP_OVR_IRQ_Handler( CFCP_Handle_t * pHandle );

void CFCP_TIMEOUT_IRQ_Handler( CFCP_Handle_t * pHandle );

/**
  * @}
  */

/**
  * @}
  */

/**
 * @}
 */

#ifdef __cplusplus
}
#endif /* __cpluplus */

#endif /* __CFCP_H */

/******************* (C) COPYRIGHT 2022 STMicroelectronics *****END OF FILE****/
