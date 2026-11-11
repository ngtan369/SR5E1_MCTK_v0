/****************************************************************************
 *
 * Copyright (c) 2023 STMicroelectronics - All Rights Reserved
 *
 * License terms: STMicroelectronics Proprietary in accordance with licensing
 * terms SLA0098 at www.st.com.
 *
 * THIS SOFTWARE IS DISTRIBUTED "AS IS," AND ALL WARRANTIES ARE DISCLAIMED,
 * INCLUDING MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE.
 *
 *****************************************************************************/
/**
 * @file    io.c
 * @brief   Runtime IO module source file.
 *
 * @addtogroup RUNTIME
 * @addtogroup IO
 * @ingroup RUNTIME
 * @{
 */

#include <io.h>

#include <stdio.h>
#include <errno.h>
#include <stdint.h>

/*===========================================================================*/
/* Module local definitions.                                                 */
/*===========================================================================*/

/*===========================================================================*/
/* Module exported variables.                                                */
/*===========================================================================*/

/*===========================================================================*/
/* Module local types.                                                       */
/*===========================================================================*/

/*===========================================================================*/
/* Module local variables.                                                   */
/*===========================================================================*/

/** @brief   Pointer to the UART driver used for IO redirection. */
static uart_driver_t *io_udp = NULL;

/*===========================================================================*/
/* Local function prototypes                                                 */
/*===========================================================================*/

size_t __write(int fd, void *buf, size_t count);
size_t __read(int fd, void *buf, size_t count);

/*===========================================================================*/
/* Module local functions.                                                   */
/*===========================================================================*/

/*===========================================================================*/
/* Module exported functions.                                                */
/*===========================================================================*/

void io_init(uart_driver_t *udp) {

    if (io_udp == NULL) {
        io_udp = udp;
    }
}

void io_start(void) {

    if (io_udp != NULL) {
        /* Configure UART used for IO redirection in interrupt synchronous
           mode.*/
        (void)uart_set_rx_drv_mode(io_udp, UART_RX_DRV_MODE_INT_SYNC);
        (void)uart_set_tx_drv_mode(io_udp, UART_TX_DRV_MODE_INT_SYNC);

        /* Start operation on UART used for IO redirection */
        uart_start(io_udp);
    }
}

uart_driver_t* io_getptr(void) {

    /* Return pointer to uart_driver_t used for IO redirection or NULL if no
       UART has been selected.*/
    return io_udp;
}

void io_stop(void) {

    if (io_udp != NULL) {
        /* Stop operation on UART used for IO redirection */
        uart_stop(io_udp);
    }
}

void io_deinit(void) {

    if (io_udp != NULL) {
        /* Deinitialize the UART used for IO redirection */
        uart_deinit(io_udp);
    }
    io_udp = NULL;
}

/**
 * @brief   Write to IO.
 * @note    This function redirect IO write on UART.
 * @details Write up to count bytes from buffer buf to the file related to the
 *          file descriptor fd.
 *
 * @param[in] fd            file descriptor
 * @param[in] buf           buffer from which to write
 * @param[in] count         number of bytes to be written
 * @return                  number of written bytes.
 *
 * @special
 */
size_t __write(int fd, void *buf, size_t count) {

    size_t trasmitted = 0;

    (void)fd;

    if (io_udp != NULL) {
        trasmitted = uart_write(io_udp, (uint8_t *)buf, count);
    }

    return ((size_t)(trasmitted));
}

/**
 * @brief   Read from IO.
 * @note    This function redirect IO read on UART.
 * @details Attempt to read up to count bytes from the file related to the file
 *          descriptor fd into the buffer buf.
 *
 * @param[in] fd            file descriptor
 * @param[in] buf           buffer in which the read data are stored
 * @param[in] count         number of bytes to be read
 * @return                  number of read bytes.
 *
 * @special
 */
size_t __read(int fd, void *buf, size_t count) {

    size_t received = 0;

    (void)fd;

    if (io_udp != NULL) {
        received = uart_read(io_udp, (uint8_t *)buf, count);
    }

    return ((size_t)(received));
}

/** @} */
