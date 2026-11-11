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
*   @file    ui_task.c
*   @version M4_VERSION
*   @brief   This file provides firmware functions that implement the features
*            of the User Interface component of the Stellar Motor Control.
*          
*   @details This file provides firmware functions that implement the features
*            of the User Interface component of the Stellar Motor Control.
*
*/
/* Includes ------------------------------------------------------------------*/
/* Pre-compiler coherency check */
#include "ui_task.h"
#include "mc_config.h"
#include "parameters_conversion.h"
#include "mc_api.h"

#define OPT_DACX  0x20 /*!<Bit field indicating that the UI uses SPI AD7303 DAC.*/

DAC_UI_Handle_t * pDAC = MC_NULL;
extern DAC_UI_Handle_t DAC_UI_Params;

MCP_Handle_t * pMCP = MC_NULL;
MCP_Handle_t MCP_UI_Params;

static volatile uint16_t  bUITaskCounter;
static volatile uint16_t  bCOMTimeoutCounter;
static volatile uint16_t  bCOMATRTimeCounter = SERIALCOM_ATR_TIME_TICKS;

void UI_TaskInit( uint32_t* pUICfg, uint8_t bMCNum, MCI_Handle_t* pMCIList[],
                  MCT_Handle_t* pMCTList[],const char* s_fwVer )
{
      pDAC = &DAC_UI_Params;
      pDAC->_Super = UI_Params;

      UI_Init( &pDAC->_Super, bMCNum, pMCIList, pMCTList, pUICfg ); /* Init UI and link MC obj */
#if (DAC_FUNCTIONALITY == TRUE)
      UI_DACInit( &pDAC->_Super ); /* Init DAC */
      if (pDAC->hDAC_CH1_ENABLED == TRUE)
      {          
         UI_SetDAC( &pDAC->_Super, DAC_CH0, DEFAULT_DAC_CHANNEL_1 );
      }
      if (pDAC->hDAC_CH2_ENABLED == TRUE)
      {                
         UI_SetDAC( &pDAC->_Super, DAC_CH1, DEFAULT_DAC_CHANNEL_2 );
      }
#endif
    pMCP = &MCP_UI_Params;
    pMCP->_Super = UI_Params;
#ifdef SERIAL_COMMUNICATION
    UFCP_Init( & pUSART );
    MCP_Init(pMCP, (FCP_Handle_t *) & pUSART, & UFCP_Send, & UFCP_Receive, & UFCP_AbortReceive, pDAC, s_fwVer);
#endif    
#ifdef CAN_COMMUNICATION
    CFCP_Init( & pFDCAN );
    MCP_Init(pMCP, (FCP_Handle_t *) & pFDCAN, & CFCP_Send, & CFCP_Receive, & CFCP_AbortReceive, pDAC, s_fwVer);
#endif    

    UI_Init( &pMCP->_Super, bMCNum, pMCIList, pMCTList, pUICfg ); /* Initialize UI and link MC components */

}

__weak void UI_Scheduler(void)
{
  if(bUITaskCounter > 0u)
  {
    bUITaskCounter--;
  }

  if(bCOMTimeoutCounter > 1u)
  {
    bCOMTimeoutCounter--;
  }

  if(bCOMATRTimeCounter > 1u)
  {
    bCOMATRTimeCounter--;
  }
}

__weak void UI_DACUpdate(uint8_t bMotorNbr)
{
  if (UI_GetSelectedMC(&pDAC->_Super) == bMotorNbr)
  {
    UI_DACExec(&pDAC->_Super); /* Exec DAC update */
  }
}

__weak void MC_SetDAC(DAC_Channel_t bChannel, MC_Protocol_REG_t bVariable)
{
  UI_SetDAC(&pDAC->_Super, bChannel, bVariable);
}

__weak void MC_SetUserDAC(DAC_UserChannel_t bUserChNumber, int16_t hValue)
{
  UI_SetUserDAC(&pDAC->_Super, bUserChNumber, hValue);
}

__weak UI_Handle_t * GetDAC(void)
{
  return &pDAC->_Super;
}

__weak MCP_Handle_t * GetMCP(void)
{
  return pMCP;
}

__weak bool UI_IdleTimeHasElapsed(void)
{
  bool retVal = false;
  if (bUITaskCounter == 0u)
  {
    retVal = true;
  }
  return (retVal);
}

__weak void UI_SetIdleTime(uint16_t SysTickCount)
{
  bUITaskCounter = SysTickCount;
}

__weak bool UI_SerialCommunicationTimeOutHasElapsed(void)
{
  bool retVal = false;
  if (bCOMTimeoutCounter == 1u)
  {
    bCOMTimeoutCounter = 0u;
    retVal = true;
  }
  return (retVal);
}

__weak bool UI_SerialCommunicationATRTimeHasElapsed(void)
{
  bool retVal = false;
  if (bCOMATRTimeCounter == 1u)
  {
    bCOMATRTimeCounter = 0u;
    retVal = true;
  }
  return (retVal);
}

__weak void UI_SerialCommunicationTimeOutStop(void)
{
  bCOMTimeoutCounter = 0u;
}

__weak void UI_SerialCommunicationTimeOutStart(void)
{
  bCOMTimeoutCounter = SERIALCOM_TIMEOUT_OCCURENCE_TICKS;
}

__weak void UI_HandleStartStopButton_cb (void)
{
/* USER CODE BEGIN START_STOP_BTN */
  if (MC_GetSTMStateMotor1() == IDLE)
  {
    /* Ramp parameters should be tuned for the actual motor */
    MC_StartMotor1();
  }
  else
  {
    MC_StopMotor1();
  }
/* USER CODE END START_STOP_BTN */
}

/******************* (C) COPYRIGHT 2023 STMicroelectronics *****END OF FILE****/
