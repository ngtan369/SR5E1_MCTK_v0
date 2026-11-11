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
*   @file    unidirectional_fast_com.h
*   @version M4_VERSION
*   @brief   This file provides firmware functions that implement the features
*            unidirectional_fast_com_user_interface component of the Stellar Motor Control.
*          
*   @details This file provides firmware functions that implement the features
*            unidirectional_fast_com_user_interface component of the Stellar Motor Control.
*
*/
/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __UNIDIRECTIONALFASTCOM_USERINTERFACE_H
#define __UNIDIRECTIONALFASTCOM_USERINTERFACE_H

#ifdef __cplusplus
 extern "C" {
#endif /* __cplusplus */

/* Includes ------------------------------------------------------------------*/

/**
 * @addtogroup STELLAR_PMSM_MC_UI_LIBRARY
 * @{
 */

/**
 * @addtogroup MCUI
 * @{
 */

/**
 * @addtogroup UnidirectionalFastCom
 * @{
 */

/**
  * @brief  UnidirectionalFastCom parameters definition
  */
typedef struct
{
  /* HW Settings */
  USART_TypeDef*     USARTx;
  uint32_t           wUSARTRemapping;
  uint32_t           wUSARTClockSource;
  GPIO_TypeDef*      hTxPort;
  uint16_t           hTxPin;
  uint8_t            bUIIRQn;

  /* Functional settings */
  MC_Protocol_REG_t  bDefChannel1;          /*!< Code of default variables to be sent Ch1.*/
  MC_Protocol_REG_t  bDefChannel2;          /*!< Code of default variables to be sent Ch2.*/
  uint8_t            bDefMotor;             /*!< Default motor selected. */
  uint8_t            bCh1ByteNum;           /*!< Number of bytes transmitted for Ch1 */
  uint8_t            bCh2ByteNum;           /*!< Number of bytes transmitted for Ch2 */
  uint8_t            bChNum;                /*!< Number of channel to be transmitted. */
} UDFastCom_Params_t;

typedef struct
{
  UI_Handle_t _Super;      		 /*!<   */

  bool comON;                    /*!< True to establish the communication false to stop it */
  MC_Protocol_REG_t bChannel[2]; /*!< Codes of variables to be sent. */
  uint8_t bChTransmitted;        /*!< Current channel to be transmitted. */
  uint8_t bByteTransmitted;      /*!< Current byte to be transmitted. */
  int32_t wBuffer;               /*!< Transmission buffer 4 bytes. */
  uint8_t bChByteNum[2];         /*!< Number of bytes transmitted. */
  uint8_t bChNum;                /*!< Number of channel to be transmitted. */

  UDFastCom_Params_t Hw;         /*!< Hardware related struct. */
} UDFastCom_Handle_t;

/* Exported functions ------------------------------------------------------- */
void UFC_Init(UDFastCom_Handle_t *pHandle, UDFastCom_Params_t *pParams);
void UFC_StartCom(UDFastCom_Handle_t *pHandle);
void UFC_StopCom(UDFastCom_Handle_t *pHandle);
void UFC_TX_IRQ_Handler(UDFastCom_Handle_t *pHandle);

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

#endif /* __UNIDIRECTIONALFASTCOM_USERINTERFACE_H */

/************************ (C) COPYRIGHT 2023 STMicroelectronics *****END OF FILE****/
