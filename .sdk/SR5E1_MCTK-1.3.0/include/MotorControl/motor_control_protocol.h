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
*   @file    motor_control_protocol.h
*   @version M4_VERSION
*   @brief   This file provides firmware functions that implement the features
*            of the motor control protocol component of the Stellar Motor Control.
*
*   @details This file provides firmware functions that implement the features
*            of the motor control protocol component of the Stellar Motor Control.
*
 */
/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __MOTOR_CONTROL_PROTOCOL_H
#define __MOTOR_CONTROL_PROTOCOL_H

#ifdef __cplusplus
 extern "C" {
#endif /* __cplusplus */

/* Includes ------------------------------------------------------------------*/
#include "frame_communication_protocol.h"

#include "user_interface.h"
#include "dac_ui.h"

/**
 * @addtogroup STELLAR_PMSM_MC_UI_LIBRARY
 * @{
 */

/**
 * @addtogroup MCUI
 * @{
 */

/**
 * @addtogroup motor_control_protocol
 * @{
 */

 /**
  * @brief  Handle structure of motor control protocol component.
  *
  */
typedef struct MCP_Handle_s
{
  UI_Handle_t _Super;     /**< Handle structure of User Interface. */

  FCP_Handle_t *pFCP;
  FCP_SendFct_t fFcpSend;
  FCP_ReceiveFct_t fFcpReceive;
  FCP_AbortReceiveFct_t fFcpAbortReceive;
  uint8_t BufferFrame[FCP_MAX_PAYLOAD_SIZE];  /**< buffer containing data */
  const char *s_fwVer;    /**< String of FW version used */
  DAC_UI_Handle_t * pDAC; /**< Pointer on DAC handle structure. */
  uint8_t BufferSize;      /**< Frame buffer size */

} MCP_Handle_t;

/* Exported types ------------------------------------------------------------*/

/*  Function used to initialize and configure the motor control protocol Component */
void MCP_Init( MCP_Handle_t *pHandle,
               FCP_Handle_t * pFCP,
               FCP_SendFct_t fFcpSend,
               FCP_ReceiveFct_t fFcpReceive,
               FCP_AbortReceiveFct_t fFcpAbortReceive,
               DAC_UI_Handle_t * pDAC,
               const char* s_fwVer);
void MCP_OnTimeOut(MCP_Handle_t *pHandle);

/*  Function used for data decoding */
void MCP_ReceivedFrame(MCP_Handle_t *pHandle, uint8_t Code, uint8_t *buffer, uint8_t Size);

/*  Function used for data transmission */
void MCP_SentFrame(MCP_Handle_t *pHandle, uint8_t Code, uint8_t *buffer, uint8_t Size);

/*  Function used to check next reception frame. */
void MCP_WaitNextFrame(MCP_Handle_t *pHandle);

/*  Allow to report the overrun error message. */
void MCP_SendOverrunMessage(MCP_Handle_t *pHandle);

/*  Allow to report the time out error message. */
void MCP_SendTimeoutMessage(MCP_Handle_t *pHandle);

/*  Allow to send an ATR message. */
void MCP_SendATRMessage(MCP_Handle_t *pHandle);

/*  Allow to send back a BAD CRC message. */
void MCP_SendBadCRCMessage(MCP_Handle_t *pHandle);

/**
  * @}
  */

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

#endif /* __MOTOR_CONTROL_PROTOCOL_H */

/************************ (C) COPYRIGHT 2023 STMicroelectronics *****END OF FILE****/
