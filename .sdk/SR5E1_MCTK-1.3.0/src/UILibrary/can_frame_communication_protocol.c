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
*   @file    can_frame_communication_protocol.c
*   @version M4_VERSION
*   @brief   This file provides firmware functions that implement the features
*            of the Frame Communication Protocol for CAN component of the Stellar Motor Control.
*          
*   @details This file provides firmware functions that implement the features
*            of the Frame Communication Protocol for CAN component of the Stellar Motor Control.
 */
/* Includes ------------------------------------------------------------------*/
#include "can_frame_communication_protocol.h"
#include "ui_irq_handler.h"
#include "sr5e1_hal_conf.h"
#include "sr5e1_hal_fdcan.h"
#include "clock.h"
#include "mc_config.h"
#include "osal.h"

FDCAN_HandleTypeDef hfdcan1;
static FDCAN_FilterTypeDef sFilterConfig;
extern void Error_Handler(void);
static void MC_FDCAN_Init(FDCAN_TypeDef * MC_FDCANx);

/** @addtogroup STELLAR_PMSM_MC_UI_LIBRARY
  * @{
  */

/**
 * @addtogroup MCUI
 * @{
 */

/** @defgroup CFCP CAN Frame Communication Protocol
  * @brief CFCP CFCP component of Stellar Motor Control
  *
  * Detailed documentation for the component.
  * @{
  */

/* Private macros ------------------------------------------------------------*/
#define UFCP_IRQ_FLAG_RX      0
#define UFCP_IRQ_FLAG_TX      1
#define UFCP_IRQ_FLAG_OVERRUN 2
#define UFCP_IRQ_FLAG_TIMEOUT 3
#define UFCP_IRQ_FLAG_ATR     4

#define CAN_TX_MSG_ID_START    16U
#define CAN_MAX_MSG_LENGTH      8U
/* Private function prototypes -----------------------------------------------*/


/* Private variables ---------------------------------------------------------*/
static const uint16_t CFCP_Can_Timeout_none = 0;
static const uint16_t CFCP_Can_Timeout_start = 1;
static const uint16_t CFCP_Can_Timeout_stop = 2;

/* Functions ---------------------------------------------------------*/

__weak void CFCP_Init( CFCP_Handle_t * pHandle )
{
  MC_FDCAN_Init(pHandle->FDCANx);
  
  /* Initialize generic component part */
  FCP_Init( & pHandle->_Super );
}

static void MC_FDCAN_Init(FDCAN_TypeDef * MC_FDCANx)
{
  clock_enable_FDCAN1(true);
  clock_enable_FDCAN2(true);
  clock_enable_FDCAN3(true);
  clock_enable_FDCAN4(true);
  
  hfdcan1.Instance = MC_FDCANx;

  hfdcan1.Init.FrameFormat = FDCAN_FRAME_CLASSIC;
  hfdcan1.Init.Mode = FDCAN_MODE_NORMAL;

  hfdcan1.Init.AutoRetransmission = ENABLE;
  hfdcan1.Init.TransmitPause = ENABLE;
  hfdcan1.Init.ProtocolException = DISABLE;
  hfdcan1.Init.NominalPrescaler = 2;
  hfdcan1.Init.NominalSyncJumpWidth = 10;
  hfdcan1.Init.NominalTimeSeg1 = 29;
  hfdcan1.Init.NominalTimeSeg2 = 10;

  hfdcan1.Init.DataPrescaler = 2;
  hfdcan1.Init.DataSyncJumpWidth = 10;
  hfdcan1.Init.DataTimeSeg1 = 29;
  hfdcan1.Init.DataTimeSeg2 = 10;
  hfdcan1.Init.StdFiltersNbr = 2;
  hfdcan1.Init.ExtFiltersNbr = 0;
  hfdcan1.Init.TxFifoQueueMode = FDCAN_TX_FIFO_OPERATION;
  if (HAL_FDCAN_Init(&hfdcan1) != HAL_OK)
  {
    Error_Handler();
  }
  
  /*##-1 Configure the FDCAN filters ########################################*/
  /* Configure standard ID reception filter to Rx FIFO 0 */
  sFilterConfig.IdType = FDCAN_STANDARD_ID;
  sFilterConfig.FilterIndex = 0;
  sFilterConfig.FilterType = FDCAN_FILTER_RANGE;
  sFilterConfig.FilterConfig = FDCAN_FILTER_TO_RXFIFO0;
  sFilterConfig.FilterID1 = 0x1;
  sFilterConfig.FilterID2 = 0xA; 
  if (HAL_FDCAN_ConfigFilter(&hfdcan1, &sFilterConfig) != HAL_OK)
  {
    Error_Handler();
  }
  /* Configure global filter:
     Filter all remote frames with STD and EXT ID
     Reject non matching frames with STD ID and EXT ID */
  if (HAL_FDCAN_ConfigGlobalFilter(&hfdcan1, FDCAN_REJECT, FDCAN_REJECT, FDCAN_FILTER_REMOTE, FDCAN_FILTER_REMOTE) != HAL_OK)
  {
    Error_Handler();
  }
  /*##-2 Start FDCAN controller (continuous listening CAN bus) ##############*/
  if (HAL_FDCAN_Start(&hfdcan1) != HAL_OK)
  {
    Error_Handler();
  }
}

/*
 *
 */
__weak void * CFCP_RX_IRQ_Handler( CFCP_Handle_t * pHandle, unsigned short rx_data )
{

  void * ret_val = (void *) & CFCP_Can_Timeout_none;

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
        ret_val = (void *) & CFCP_Can_Timeout_start;

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
          ret_val = (void *) & CFCP_Can_Timeout_stop;

          /* Stop Rx Timeout */
          pBaseHandle->RxTimeoutCountdown = 0;
          /* Disable the reception IRQ */
          HAL_FDCAN_DeactivateNotification(&hfdcan1, FDCAN_IT_RX_FIFO0_NEW_MESSAGE);

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
            (void) CFCP_Send( pBaseHandle, FCP_CODE_NACK, & error_code, 1 );
          }
        }
    } /* end of switch ( pBaseHandle->RxFrameLevel ) */
  } /* end of if ( FCP_TRANSFER_IDLE != pBaseHandle->RxFrameState ) */

  return ret_val;
}

/*
 *
 */
__weak void CFCP_TX_IRQ_Handler( CFCP_Handle_t * pHandle )
{

  FCP_Handle_t * pBaseHandle = & pHandle->_Super;
  FDCAN_TxHeaderTypeDef TxHeader;
  uint8_t NumberOfTrasmit = 0;
  uint8_t count = 0;
  uint8_t index = 0;
  
  if ( FCP_TRANSFER_IDLE != pBaseHandle->TxFrameState )
  {
      uint8_t tx_data[16];
      /* Build the can message to be sent */
      TxHeader.Identifier = CAN_TX_MSG_ID_START;
      TxHeader.IdType = FDCAN_STANDARD_ID;
      TxHeader.TxFrameType = FDCAN_DATA_FRAME;
      TxHeader.ErrorStateIndicator = FDCAN_ESI_ACTIVE;
      TxHeader.BitRateSwitch = FDCAN_BRS_OFF;
      TxHeader.FDFormat = FDCAN_CLASSIC_CAN;
      TxHeader.TxEventFifoControl = FDCAN_NO_TX_EVENTS;
      uint8_t transfert_size = FCP_HEADER_SIZE + pBaseHandle->TxFrame.Size + FCP_CRC_SIZE;
      /* Response message has more than 8 byte  (STD message configuration)*/
      if (transfert_size > CAN_MAX_MSG_LENGTH)
      {
          /* Number of 8 byte message needed to send */
          NumberOfTrasmit = transfert_size>>3; /* divide by CAN_MAX_MSG_LENGTH*/
          if( (transfert_size% CAN_MAX_MSG_LENGTH ) != 0)
          {
            /* spare byte to be sent */
            NumberOfTrasmit++;
          }
          for (count = 0; count < NumberOfTrasmit; count++)
          {
              while ((transfert_size > (pBaseHandle->TxFrameLevel)) &&
                  (index<(CAN_MAX_MSG_LENGTH-1)) )
              {
                  /* fill the message */
                  index = pBaseHandle->TxFrameLevel % CAN_MAX_MSG_LENGTH;
                  if((index == 0) && (count ==0))
                  {
                      tx_data[index] = (uint8_t) pBaseHandle->TxFrame.Code;
                  }
                  else if ((index == 1) && (count == 0))
                  {
                      tx_data[index] = (uint8_t) pBaseHandle->TxFrame.Size;
                  }
                  else
                  {
                      if ( pBaseHandle->TxFrameLevel < pBaseHandle->TxFrame.Size + FCP_HEADER_SIZE )
                      {
                         tx_data[index] = pBaseHandle->TxFrame.Buffer[pBaseHandle->TxFrameLevel - FCP_HEADER_SIZE];
                      }
                      else
                      {
                         tx_data[index] = (uint16_t) pBaseHandle->TxFrame.FrameCRC;
                      }
                  }
                  pBaseHandle->TxFrameLevel++;
             }            
             /* set the length */
             if (index == (CAN_MAX_MSG_LENGTH-1))
                  TxHeader.DataLength = CAN_MAX_MSG_LENGTH << 16 ;
             else
                  //TxHeader.DataLength = (index+1) << 16 ;
                  TxHeader.DataLength = (pBaseHandle->TxFrameLevel % CAN_MAX_MSG_LENGTH) << 16;

            index = 0;
            /* transmit the message */
            if (HAL_FDCAN_AddMessageToTxFifoQ(&hfdcan1, &TxHeader, tx_data) != HAL_OK)
            {
               Error_Handler();
            }

            /* when the buffer exceed 8 byte increase the message ID */
            TxHeader.Identifier++;
         }
         pBaseHandle->TxFrameState = FCP_TRANSFER_IDLE;
         pBaseHandle->ClientFrameSentCallback( pBaseHandle->ClientEntity );
      }
      /* Response message has less than 8 byte (STD message configuration) */
      else
      {
         /* Send the data byte */
         tx_data[0] = (uint8_t) pBaseHandle->TxFrame.Code;
         pBaseHandle->TxFrameLevel++;
         tx_data[1] = (uint8_t) pBaseHandle->TxFrame.Size;
         pBaseHandle->TxFrameLevel++;
         for( count = pBaseHandle->TxFrameLevel; count < (pBaseHandle->TxFrame.Size + FCP_HEADER_SIZE); count++)
         {
          tx_data[count] = (uint16_t) pBaseHandle->TxFrame.Buffer[ count - FCP_HEADER_SIZE ];
          pBaseHandle->TxFrameLevel++;
         }
         tx_data[pBaseHandle->TxFrameLevel] = (uint16_t) pBaseHandle->TxFrame.FrameCRC;
         pBaseHandle->TxFrameLevel++;
         TxHeader.DataLength = pBaseHandle->TxFrameLevel << 16 ;

          if (HAL_FDCAN_AddMessageToTxFifoQ(&hfdcan1, &TxHeader, tx_data) != HAL_OK)
          {
              Error_Handler();
          }             
          pBaseHandle->TxFrameState = FCP_TRANSFER_IDLE;
          pBaseHandle->ClientFrameSentCallback( pBaseHandle->ClientEntity );
      }  
  } /* end of if ( FCP_TRANSFER_IDLE != pBaseHandle->TxFrameState ) */
}

/*
 *
 */
__weak void CFCP_OVR_IRQ_Handler( CFCP_Handle_t * pHandle )
{
  FCP_Handle_t * pBaseHandle = & pHandle->_Super;
  uint8_t error_code;

  //error_code = UFCP_MSG_OVERRUN;
  (void) CFCP_Send( pBaseHandle, FCP_CODE_NACK, & error_code, 1 );

}

/*
 *
 */
__weak void CFCP_TIMEOUT_IRQ_Handler( CFCP_Handle_t * pHandle )
{
  FCP_Handle_t * pBaseHandle = & pHandle->_Super;
  uint8_t error_code;

  error_code = FCP_MSG_RX_TIMEOUT;
  (void) CFCP_Send( pBaseHandle, FCP_CODE_NACK, & error_code, 1 );

}

__weak uint8_t CFCP_Receive( FCP_Handle_t * pHandle )
{
  uint8_t ret_val;
  

  if ( FCP_TRANSFER_IDLE == pHandle->RxFrameState )
  {
    /* CFCP_Handle_t * pActualHandle = (CFCP_Handle_t *) pHandle; */

    pHandle->RxFrameLevel = 0;
    pHandle->RxFrameState = FCP_TRANSFER_ONGOING;
    
    HAL_FDCAN_ActivateNotification(&hfdcan1, FDCAN_IT_RX_FIFO0_NEW_MESSAGE, 0);
    ret_val = FCP_STATUS_WAITING_TRANSFER;
  }
  else
  {
    ret_val = FCP_STATUS_TRANSFER_ONGOING;
  }

  return ret_val;
}

__weak uint8_t CFCP_Send( FCP_Handle_t * pHandle, uint8_t code, uint8_t *buffer, uint8_t size)
{
  uint8_t ret_val;

  if ( FCP_TRANSFER_IDLE == pHandle->TxFrameState )
  {
    /* CFCP_Handle_t * pActualHandle = (CFCP_Handle_t *) pHandle; */
    uint8_t *dest = pHandle->TxFrame.Buffer;

    pHandle->TxFrame.Code = code;
    pHandle->TxFrame.Size = size;
    while ( size-- ) *dest++ = *buffer++;
    pHandle->TxFrame.FrameCRC = FCP_CalcCRC( & pHandle->TxFrame );

    pHandle->TxFrameLevel = 0;
    pHandle->TxFrameState = FCP_TRANSFER_ONGOING;

    CFCP_TX_IRQ_Handler((CFCP_Handle_t *)pHandle);

    ret_val = FCP_STATUS_WAITING_TRANSFER;
  }
  else
  {
    ret_val = FCP_STATUS_TRANSFER_ONGOING;
  }

  return ret_val;
}

__weak void CFCP_AbortReceive( FCP_Handle_t * pHandle )
{
  pHandle->RxFrameState = FCP_TRANSFER_IDLE;
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

/************************ (C) COPYRIGHT 2023 STMicroelectronics *****END OF FILE****/
