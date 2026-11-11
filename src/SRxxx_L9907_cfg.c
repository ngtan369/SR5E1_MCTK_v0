/****************************************************************************
*
* Copyright (c) 2023-2026 STMicroelectronics - All Rights Reserved
*
* License terms: STMicroelectronics Proprietary in accordance with licensing
* terms SLA0098 at www.st.com.
*
* THIS SOFTWARE IS DISTRIBUTED "AS IS," AND ALL WARRANTIES ARE DISCLAIMED,
* INCLUDING MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE.
*****************************************************************************/
/**
*   @file    SRxxx_L9907_cfg.c
*   @brief   L9907 configuration file
*
*/
#include "SRxxx_L9907_cfg.h"
#include "typedefs.h"

extern volatile uint8_t sch_checkerror;


/**
 * @addtogroup L9907
 * @{
 */
 
/*
VERY IMPORTANT: in SRxxx IRQ Component RLA create a new  Enabled Vector with Number 44 and Priority higher than 10, and insert  a call to the function L9907_irqHandler in the IRQ Body's code.
*/

void L9907_faultRecovery(void) {

    /* faultRecovery body here*/
    sch_checkerror = 1U;

} // end faultRecovery
/** @} */
