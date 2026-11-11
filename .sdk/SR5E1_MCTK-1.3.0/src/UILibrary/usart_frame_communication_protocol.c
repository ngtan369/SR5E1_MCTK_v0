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
*   @file    usart_frame_communication_protocol.c
*   @version M4_VERSION
*   @brief   This file provides firmware functions that implement the features
*            of the Frame Communication Protocol for USART component of the Stellar Motor Control.
*          
*   @details This file provides firmware functions that implement the features
*            of the Frame Communication Protocol for USART component of the Stellar Motor Control.
*
*/
/* Includes ------------------------------------------------------------------*/
#include "usart_frame_communication_protocol.h"
#include "ui_irq_handler.h"
#include "clock.h"
#include "mc_config.h"

/** @addtogroup STELLAR_PMSM_MC_UI_LIBRARY
  * @{
  */

/**
 * @addtogroup MCUI
 * @{
 */

/** @defgroup UFCP USART Frame Communication Protocol
  * @brief UFCP UFCP component of the Motor Control SDK
  *
  * Detailed documentation for the component.
  * @{
  */
#ifdef SERIAL_COMMUNICATION

/* Private macros ------------------------------------------------------------*/
#define UFCP_IRQ_FLAG_RX      0
#define UFCP_IRQ_FLAG_TX      1
#define UFCP_IRQ_FLAG_OVERRUN 2
#define UFCP_IRQ_FLAG_TIMEOUT 3
#define UFCP_IRQ_FLAG_ATR     4

/* Private function prototypes -----------------------------------------------*/
static void MC_UART_Init(USART_TypeDef * MC_USARTx);

/* Private variables ---------------------------------------------------------*/
static const uint16_t UFCP_Usart_Timeout_none = 0;
static const uint16_t UFCP_Usart_Timeout_start = 1;
static const uint16_t UFCP_Usart_Timeout_stop = 2;

/* Functions ---------------------------------------------------------*/

__weak void UFCP_Init( UFCP_Handle_t * pHandle )
{
  MC_UART_Init(pHandle->USARTx);
  
  /* Initialize generic component part */
  FCP_Init( & pHandle->_Super );
}

static void MC_UART_Init(USART_TypeDef * MC_USARTx)
{

  LL_USART_InitTypeDef USART_InitStruct = {0};

  if(MC_USARTx == USART1)
  {
    /* Peripheral clock enable */
    clock_enable_UART1(true);
  }  
  if(MC_USARTx == USART2)
  {
    /* Peripheral clock enable */
    clock_enable_UART2(true);
  }  
  if(MC_USARTx == USART3)
  {
    /* Peripheral clock enable */
    clock_enable_UART3(true);
  }  

  USART_InitStruct.PrescalerValue = LL_USART_PRESCALER_DIV1;
  USART_InitStruct.BaudRate = USART_SPEED;
  USART_InitStruct.DataWidth = LL_USART_DATAWIDTH_8B;
  USART_InitStruct.StopBits = LL_USART_STOPBITS_1;
  USART_InitStruct.Parity = LL_USART_PARITY_NONE;
  USART_InitStruct.TransferDirection = LL_USART_DIRECTION_TX_RX;
  USART_InitStruct.HardwareFlowControl = LL_USART_HWCONTROL_NONE;
  USART_InitStruct.OverSampling = LL_USART_OVERSAMPLING_16;
  LL_USART_Init(MC_USARTx, &USART_InitStruct);
  /* TX/RX pins swapping */
  LL_USART_SetTXRXSwap(MC_USARTx, USART_SWAP_TXRX_PIN);
  LL_USART_SetTXFIFOThreshold(MC_USARTx, LL_USART_FIFOTHRESHOLD_1_8);
  LL_USART_SetRXFIFOThreshold(MC_USARTx, LL_USART_FIFOTHRESHOLD_1_8);
  LL_USART_DisableFIFO(MC_USARTx);
  LL_USART_ConfigAsyncMode(MC_USARTx);

  LL_USART_Enable(MC_USARTx);

  /* Polling USART_SELECTION initialisation */
  while((!(LL_USART_IsActiveFlag_TEACK(MC_USARTx))) || (!(LL_USART_IsActiveFlag_REACK(MC_USARTx))))
  {
  }

}
/*
 *
 */
__weak void * UFCP_RX_IRQ_Handler( UFCP_Handle_t * pHandle, unsigned short rx_data )
{
  void * ret_val = (void *) & UFCP_Usart_Timeout_none;
  FCP_Handle_t * pBaseHandle = & pHandle->_Super;
  uint8_t error_code;

  if ( FCP_TRANSFER_IDLE != pBaseHandle->RxFrameState )
  {
    uint8_t rx_byte = (uint8_t) rx_data;

    switch ( pBaseHandle->RxFrameLevel )
    {
      case 0: // First Byte received --> The Code
        pBaseHandle->RxFrame.Code = rx_byte;
        /* Need to ask the caller to start our timeout... TODO: Is this really useful? */
        ret_val = (void *) & UFCP_Usart_Timeout_start;

        /* Start Rx Timeout */
        pBaseHandle->RxTimeoutCountdown = pBaseHandle->RxTimeout;
        pBaseHandle->RxFrameLevel++;
        break;

      case 1: // Second Byte received --> Size of the payload
        pBaseHandle->RxFrame.Size = rx_byte;
        pBaseHandle->RxFrameLevel++;
        if ( pBaseHandle->RxFrame.Size >= FCP_MAX_PAYLOAD_SIZE)
        { /* Garbage data received decoded with a payload size that exceeds max*/
          pBaseHandle->RxFrameLevel =0 ;
        }
        break;

      default: // In the payload or the "CRC"
        if ( pBaseHandle->RxFrameLevel < pBaseHandle->RxFrame.Size + FCP_HEADER_SIZE )
        {
          // read byte is for the payload
          pBaseHandle->RxFrame.Buffer[pBaseHandle->RxFrameLevel - FCP_HEADER_SIZE] = rx_byte;
          pBaseHandle->RxFrameLevel++;
        }
        else
        {
          // read byte is for the "CRC"
          pBaseHandle->RxFrame.FrameCRC = rx_byte;

          /* Need to ask the caller to stop our timeout... TODO: Is this really useful? */
          ret_val = (void *) & UFCP_Usart_Timeout_stop;

          /* Stop Rx Timeout */
          pBaseHandle->RxTimeoutCountdown = 0;
          /* Disable the reception IRQ */
          LL_USART_DisableIT_RXNE(pHandle->USARTx);
          /* Indicate the reception is complete. */
          pBaseHandle->RxFrameState = FCP_TRANSFER_IDLE;

          /* Check the Control Sum */
          if ( FCP_CalcCRC( & pBaseHandle->RxFrame ) == pBaseHandle->RxFrame.FrameCRC )
          {
            /* OK. the frame is considered correct. Let's forward to client. */
            pBaseHandle->ClientFrameReceivedCallback( pBaseHandle->ClientEntity,
                                                      pBaseHandle->RxFrame.Code,
                                                      pBaseHandle->RxFrame.Buffer,
                                                      pBaseHandle->RxFrame.Size );
          }
          else
          {
            error_code = FCP_MSG_RX_BAD_CRC;
            (void) UFCP_Send( pBaseHandle, FCP_CODE_NACK, & error_code, 1 );
          }
        }
    } /* end of switch ( pBaseHandle->RxFrameLevel ) */
  } /* end of if ( FCP_TRANSFER_IDLE != pBaseHandle->RxFrameState ) */

  return ret_val;
}

/*
 *
 */
__weak void UFCP_TX_IRQ_Handler( UFCP_Handle_t * pHandle )
{
  FCP_Handle_t * pBaseHandle = & pHandle->_Super;

  if ( FCP_TRANSFER_IDLE != pBaseHandle->TxFrameState )
  {
    uint16_t tx_data;

    switch ( pBaseHandle->TxFrameLevel )
    {
      case 0:
        tx_data = (uint16_t) pBaseHandle->TxFrame.Code;
        break;

      case 1:
        tx_data = (uint16_t) pBaseHandle->TxFrame.Size;
        break;

      default:
        if ( pBaseHandle->TxFrameLevel < pBaseHandle->TxFrame.Size + FCP_HEADER_SIZE )
        {
          tx_data = (uint16_t) pBaseHandle->TxFrame.Buffer[ pBaseHandle->TxFrameLevel - FCP_HEADER_SIZE ];
        }
        else
        {
          tx_data = (uint16_t) pBaseHandle->TxFrame.FrameCRC;
        }
    } /* end of switch ( pBaseHandle->TxFrameLevel ) */

    /* Send the data byte */
    LL_USART_TransmitData8(pHandle->USARTx, tx_data);

    if ( pBaseHandle->TxFrameLevel < pBaseHandle->TxFrame.Size + FCP_HEADER_SIZE )
    {
      pBaseHandle->TxFrameLevel++;
    }
    else
    {
      LL_USART_DisableIT_TXE(pHandle->USARTx);
      pBaseHandle->TxFrameState = FCP_TRANSFER_IDLE;

      pBaseHandle->ClientFrameSentCallback( pBaseHandle->ClientEntity );
    }

  } /* end of if ( FCP_TRANSFER_IDLE != pBaseHandle->TxFrameState ) */
}

/*
 *
 */
__weak void UFCP_OVR_IRQ_Handler( UFCP_Handle_t * pHandle )
{
  FCP_Handle_t * pBaseHandle = & pHandle->_Super;
  uint8_t error_code;

  error_code = UFCP_MSG_OVERRUN;
  (void) UFCP_Send( pBaseHandle, FCP_CODE_NACK, & error_code, 1 );

}

/*
 *
 */
__weak void UFCP_TIMEOUT_IRQ_Handler( UFCP_Handle_t * pHandle )
{
  FCP_Handle_t * pBaseHandle = & pHandle->_Super;
  uint8_t error_code;

  error_code = FCP_MSG_RX_TIMEOUT;
  (void) UFCP_Send( pBaseHandle, FCP_CODE_NACK, & error_code, 1 );

}

__weak uint8_t UFCP_Receive( FCP_Handle_t * pHandle )
{
  uint8_t ret_val;

  if ( FCP_TRANSFER_IDLE == pHandle->RxFrameState )
  {
    UFCP_Handle_t * pActualHandle = (UFCP_Handle_t *) pHandle;

    pHandle->RxFrameLevel = 0;
    pHandle->RxFrameState = FCP_TRANSFER_ONGOING;

    LL_USART_EnableIT_RXNE(pActualHandle->USARTx);
    ret_val = FCP_STATUS_WAITING_TRANSFER;
  }
  else
  {
    ret_val = FCP_STATUS_TRANSFER_ONGOING;
  }

  return ret_val;
}

__weak uint8_t UFCP_Send( FCP_Handle_t * pHandle, uint8_t code, uint8_t *buffer, uint8_t size)
{
  uint8_t ret_val;

  if ( FCP_TRANSFER_IDLE == pHandle->TxFrameState )
  {
    UFCP_Handle_t * pActualHandle = (UFCP_Handle_t *) pHandle;
    uint8_t *dest = pHandle->TxFrame.Buffer;

    pHandle->TxFrame.Code = code;
    pHandle->TxFrame.Size = size;
    while ( size-- ) *dest++ = *buffer++;
    pHandle->TxFrame.FrameCRC = FCP_CalcCRC( & pHandle->TxFrame );

    pHandle->TxFrameLevel = 0;
    pHandle->TxFrameState = FCP_TRANSFER_ONGOING;

    LL_USART_EnableIT_TXE(pActualHandle->USARTx);
    ret_val = FCP_STATUS_WAITING_TRANSFER;
  }
  else
  {
    ret_val = FCP_STATUS_TRANSFER_ONGOING;
  }

  return ret_val;
}

__weak void UFCP_AbortReceive( FCP_Handle_t * pHandle )
{
  pHandle->RxFrameState = FCP_TRANSFER_IDLE;
}
#endif

/**
  * @}
  */

/**
  * @}
  */

/**
  * @}
  */

/************************ (C) COPYRIGHT 2023 STMicroelectronics *****END OF FILE****/
