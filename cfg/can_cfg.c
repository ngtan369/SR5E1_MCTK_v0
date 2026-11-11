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

#include <can_cfg.h>
#include <dma.h>
#include <irq.h>

/* Internal configuration data.*/

/* Exported configuration APIs.*/
void can_config1(can_driver_t *cdp) {
	
	/* Configure can driver instance.*/
	(void)can_set_prio(cdp, IRQ_PRIORITY_10);
	(void)can_set_loopback(cdp, CAN_NO_LOOPBACK);
	(void)can_set_datasize(cdp, CAN_DATASIZE_8BYTE);
	(void)can_set_tx_mode(cdp, CAN_TX_MODE_DEDICATED_BUFF);
	(void)can_set_rx_polling_mode(cdp, false);
	(void)can_set_ts_mode(cdp, CAN_TIMESTAMP_MODE_NONE);
	
	/* Baudrate configuration.*/
	
	/* Nominal CAN configuration.*/
	(void)can_set_nbtp(cdp, 9, 4, 1, 0);
	
	/* Filters configuration.*/
	(void)can_set_filter(cdp, 2032, 0, CAN_STD_FILTER_TYPE, CAN_FILTER_CLASSIC, CAN_FEC_RX_BUF);
	
	/* Buffers configuration.*/
	(void)can_set_buffers(cdp, 4, 4, 4, 4, 4, 4);
	
	
	/* Configure interrupts.*/
	(void)can_set_irq_line(cdp, CAN_IRQ_LINE0);
	
	(void)can_set_irq_mask(cdp, CAN_IRQ_ALL_ENABLED);
	/* Operation callback configuration.*/
	(void)can_set_cb(cdp, canrx_receive_cb);
	
	/* Error callback configuration.*/
	(void)can_set_err_cb(cdp, canrx_error_cb);
}
