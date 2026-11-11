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
*   @file    unidirectional_fast_com.c
*   @version M4_VERSION
*   @brief   This file provides firmware functions that implement the features
*            unidirectional_fast_com_user_interface component of the Stellar Motor Control.
*          
*   @details This file provides firmware functions that implement the features
*            unidirectional_fast_com_user_interface component of the Stellar Motor Control.
*
*/
/* Includes ------------------------------------------------------------------*/
#include "user_interface.h"
#include "unidirectional_fast_com.h"
#include "mc_type.h"
#include "ui_irq_handler.h"

/* Private function prototypes -----------------------------------------------*/

/* Private function prototypes -----------------------------------------------*/

/** @addtogroup STELLAR_PMSM_MC_UI_LIBRARY
  * @{
  */

/**
 * @addtogroup MCUI
 * @{
 */

/** @defgroup UnidirectionalFastCom Unidirectional Fast Communication
  * @brief Unidirectional Fast Serial Communication component of the Motor Control SDK
  *
  * @todo Complete documentation
  * @{
  */

/**
  * @brief  Initializes a UnidirectionalFastCom component Handle
  *
  * It initialize all vars.
  *
  * @param  pHandle Pointer on a UDFastCom_Handle_t structure to initialize
  */
__weak void UFC_Init( UDFastCom_Handle_t *pHandle, UDFastCom_Params_t *pParams )
{
  /* Init Hardware. */
  pHandle->Hw = *pParams;

  /* Init Vars */
  pHandle->_Super.bSelectedDrive = pHandle->Hw.bDefMotor;
  pHandle->bChannel[0]           = pHandle->Hw.bDefChannel1;
  pHandle->bChannel[1]           = pHandle->Hw.bDefChannel2;
  pHandle->bChByteNum[0]         = pHandle->Hw.bCh1ByteNum;
  pHandle->bChByteNum[1]         = pHandle->Hw.bCh2ByteNum;
  pHandle->comON                 = false;
  pHandle->bChTransmitted        = 0;
  pHandle->bByteTransmitted      = 0;
  pHandle->bChNum                = pHandle->Hw.bChNum ;
 
  /* Enable the USART */
  LL_USART_Enable(pHandle->Hw.USARTx);
}

/*******************************************************************************
* Function Name  : USART_IRQ_Handler
* Description    : Interrupt function for the serial communication
* Input          : none
* Return         : none
*******************************************************************************/
__weak void UFC_TX_IRQ_Handler(UDFastCom_Handle_t *pHandle)
{
  uint8_t txData = 0;
  uint8_t* pBuff;

  if (pHandle->comON)
  {
    if (pHandle->bByteTransmitted == 0)
    {
      /* First byte to be transmitted, read value and populate the buffer */
      pHandle->wBuffer = UI_GetReg(&pHandle->_Super, pHandle->bChannel[pHandle->bChTransmitted],MC_NULL) >> 8;
    }

    pBuff = (uint8_t*)(&(pHandle->wBuffer));
    txData = pBuff[pHandle->bByteTransmitted];

    /* Write one byte to the transmit data register */
    LL_USART_TransmitData8(pHandle->Hw.USARTx, txData);

    pHandle->bByteTransmitted++;
    if (pHandle->bByteTransmitted == pHandle->bChByteNum[pHandle->bChTransmitted])
    {
      pHandle->bByteTransmitted = 0;
      pHandle->bChTransmitted++;
      if (pHandle->bChTransmitted == pHandle->Hw.bChNum)
      {
        pHandle->bChTransmitted = 0;
      }
    }
  }
}

/**
  * @brief  Starts the fast unidirectional communication.
  * @param  pHandle Handle on the target unidirectional fast communication instance
  */
__weak void UFC_StartCom( UDFastCom_Handle_t *pHandle )
{
  pHandle->comON = true;
  LL_USART_TransmitData8(pHandle->Hw.USARTx, ' ');
  /* Enable USART Transmit interrupts */
  LL_USART_EnableIT_TXE(pHandle->Hw.USARTx);
}

/**
  * @brief  Stops the fast unidirectional communication.
  * @param  pHandle Handle on the target unidirectional fast communication instance
  */
__weak void UFC_StopCom(UDFastCom_Handle_t *pHandle)
{
  pHandle->comON = false;
  /* Disable USART Transmit interrupts */
  LL_USART_DisableIT_TXE(pHandle->Hw.USARTx);
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

/**
 * @}
 */

/************************ (C) COPYRIGHT 2023 STMicroelectronics *****END OF FILE****/
