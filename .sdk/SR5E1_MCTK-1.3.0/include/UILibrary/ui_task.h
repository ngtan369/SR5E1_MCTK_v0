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
*   @file    ui_task.h
*   @version M4_VERSION
*   @brief   This file provides firmware functions that implement the features
*            of the User Interface component of the Stellar Motor Control.
*          
*   @details This file provides firmware functions that implement the features
*            of the User Interface component of the Stellar Motor Control.
*
*/
/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __UITASK_H
#define __UITASK_H

#include "user_interface.h"
#include "dac_rctimer_ui.h"
#include "dac_ui.h"

#include "motor_control_protocol.h"
#include "frame_communication_protocol.h"
#include "usart_frame_communication_protocol.h"
#include "can_frame_communication_protocol.h"
#include "ui_irq_handler.h"

/* Exported functions --------------------------------------------------------*/
void UI_TaskInit(uint32_t* pUICfg, uint8_t bMCNum, MCI_Handle_t * pMCIList[],
                 MCT_Handle_t* pMCTList[],const char* s_fwVer);

void UI_DACUpdate(uint8_t bMotorNbr);
UI_Handle_t * GetDAC(void);

void UI_Scheduler(void);
MCP_Handle_t * GetMCP(void);

bool UI_IdleTimeHasElapsed(void);
void UI_SetIdleTime(uint16_t SysTickCount);
bool UI_SerialCommunicationTimeOutHasElapsed(void);
bool UI_SerialCommunicationATRTimeHasElapsed(void);
void UI_SerialCommunicationTimeOutStop(void);
void UI_SerialCommunicationTimeOutStart(void);

void UI_HandleStartStopButton_cb (void);

#endif /* __UITASK_H */

/******************* (C) COPYRIGHT 2023 STMicroelectronics *****END OF FILE****/
