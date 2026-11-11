/****************************************************************************
 *
 * Copyright (c) 2023-2026 STMicroelectronics - All Rights Reserved
 *
 * License terms: STMicroelectronics Proprietary in accordance with licensing
 * terms SLA0098 at www.st.com.
 *
 * THIS SOFTWARE IS DISTRIBUTED "AS IS," AND ALL WARRANTIES ARE DISCLAIMED,
 * INCLUDING MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE.
 *
 * DO NOT EDIT: Automatically generated file
 *****************************************************************************/

#ifndef _CAN_CFG_H_
#define _CAN_CFG_H_

#include "can.h"

#ifdef __cplusplus
extern "C" {
#endif

/* Exported callback function prototypes.*/

void canrx_receive_cb(can_driver_t *cdp, can_frame_t frame, can_evt_t event);

void canrx_error_cb(can_driver_t *cdp, uint32_t psr);

/* Exported configuration APIs.*/

void can_config1(can_driver_t *cdp);

#ifdef __cplusplus
}
#endif

#endif /* _CAN_CFG_H_ */
