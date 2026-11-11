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
*   @file    usart_frame_communication_protocol.h
*   @version M4_VERSION
*   @brief   This file provides firmware functions that implement the features
*            of the Frame Communication Protocol for USART component of the Stellar Motor Control.
*          
*   @details This file provides firmware functions that implement the features
*            of the Frame Communication Protocol for USART component of the Stellar Motor Control.
*
*/
/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __UFCP_H
#define __UFCP_H

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

/** @addtogroup UFCP
  * @{
  */

 /**
  * @brief Error Code for an Overrun FCP message frame.
  *
  * Such a payload is sent to the remote when an USART overrun error occurs.
  *
  * The value of the error code is set by the Frame Communication Protocol and is taken in
  * a value space that is shared with the Motor Control Protocol. It thus cannot be
  * changed.
  */
#define UFCP_MSG_OVERRUN 0x08

 /* Exported types ------------------------------------------------------------*/
typedef struct {
  FCP_Handle_t _Super;

  USART_TypeDef * USARTx;
  uint32_t USARTRemapping;
  uint32_t USARTClockSource;
  GPIO_TypeDef * RxPort;
  uint16_t RxPin;
  GPIO_TypeDef * TxPort;
  uint16_t TxPin;
  uint8_t UIIRQn;

} UFCP_Handle_t;

/* Exported functions ------------------------------------------------------- */

void UFCP_Init( UFCP_Handle_t * pHandle );

uint8_t UFCP_Receive( FCP_Handle_t * pHandle );

uint8_t UFCP_Send( FCP_Handle_t * pHandle, uint8_t code, uint8_t *buffer, uint8_t size);

void UFCP_AbortReceive( FCP_Handle_t * pHandle );

void * UFCP_RX_IRQ_Handler( UFCP_Handle_t * pHandle, unsigned short rx_data );

void UFCP_TX_IRQ_Handler( UFCP_Handle_t * pHandle );

void UFCP_OVR_IRQ_Handler( UFCP_Handle_t * pHandle );

void UFCP_TIMEOUT_IRQ_Handler( UFCP_Handle_t * pHandle );

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

#endif /* __UFCP_H */

/******************* (C) COPYRIGHT 2023 STMicroelectronics *****END OF FILE****/
