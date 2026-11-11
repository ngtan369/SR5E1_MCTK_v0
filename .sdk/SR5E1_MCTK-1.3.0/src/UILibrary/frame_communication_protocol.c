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
*   @file    frame_communication_protocol.c
*   @version M4_VERSION
*   @brief   This file provides firmware functions that implement the features
*            of the Frame Communication Protocol component of the Stellar Motor Control.
*          
*   @details This file provides firmware functions that implement the features
*            of the Frame Communication Protocol component of the Stellar Motor Control.
*
*/
/* Includes ------------------------------------------------------------------*/
#include "user_interface.h"
#include "motor_control_protocol.h"

#include "frame_communication_protocol.h"

/**
 * @addtogroup STELLAR_PMSM_MC_UI_LIBRARY
 * @{
 */

/**
 * @addtogroup MCUI
 * @{
 */

/**
 * @defgroup FCP Frame Communication Protocol
 *
 * @brief A Frame Oriented transmission protocol
 *
 * This protocol is designed to transport the messages of the Motor Control Protocol
 * across physical links such as UARTs.
 *
 * The Frame Communication Protocol component serves as a base to actual implementations
 * of the protocol that operate on specific physical links. So far, the only provided implementation
 * of the protocol uses the USARTs as the physical link.
 *
 * @todo Complete documentation
 * @{
 */

__weak void FCP_Init( FCP_Handle_t * pHandle )
{
  pHandle->RxTimeoutCountdown = 0;

  pHandle->TxFrame.Code = 0x0;
  pHandle->TxFrame.Size = 0;
  pHandle->TxFrame.FrameCRC = 0;
  pHandle->TxFrameState = FCP_TRANSFER_IDLE;
  pHandle->TxFrameLevel = 0;

  pHandle->RxFrame.Code = 0x0;
  pHandle->RxFrame.Size = 0;
  pHandle->RxFrame.FrameCRC = 0;
  pHandle->RxFrameState = FCP_TRANSFER_IDLE;
  pHandle->RxFrameLevel = 0;
}

__weak void FCP_SetClient( FCP_Handle_t * pHandle,
                    struct MCP_Handle_s * pClient,
                    FCP_SentFrameCallback_t pSentFrameCb,
                    FCP_ReceivedFrameCallback_t pReceviedFrameCb,
                    FCP_RxTimeoutCallback_t pRxTimeoutCb )
{
  if ( MC_NULL != pHandle )
  {
    pHandle->ClientEntity = pClient;
    pHandle->ClientFrameSentCallback = pSentFrameCb;
    pHandle->ClientFrameReceivedCallback = pReceviedFrameCb;
    pHandle->ClientRxTimeoutCallback = pRxTimeoutCb;
  }
}

__weak void FCP_SetTimeout( FCP_Handle_t * pHandle, uint16_t Timeout )
{
  if ( MC_NULL != pHandle )
  {
    pHandle->RxTimeout = Timeout;
  }
}

__weak uint8_t FCP_CalcCRC( FCP_Frame_t * pFrame )
{
  uint8_t nCRC = 0;
  uint16_t nSum = 0;
  uint8_t idx;

  if( MC_NULL != pFrame )
  {
    nSum += pFrame->Code;
    nSum += pFrame->Size;

    for ( idx = 0; idx < pFrame->Size; idx++ )
    {
      nSum += pFrame->Buffer[idx];
    }

    nCRC = (uint8_t)(nSum & 0xFF) ; // Low Byte of nSum
    nCRC += (uint8_t) (nSum >> 8) ; // High Byte of nSum
  }

  return nCRC ;
}

__weak uint8_t FCP_IsFrameValid( FCP_Frame_t * pFrame )
{
  if ( MC_NULL != pFrame )
    return FCP_CalcCRC(pFrame) == pFrame->Buffer[pFrame->Size];
  else
    return 0;
}

/**
 * @}
 */

/**
 * @}
 */

/**
 * @}
 */

/******************* (C) COPYRIGHT 2023 STMicroelectronics *****END OF FILE****/
