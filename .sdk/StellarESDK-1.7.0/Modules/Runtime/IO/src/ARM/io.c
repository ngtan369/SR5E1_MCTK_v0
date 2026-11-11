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

ssize_t _write(int fd, void *buf, size_t count);
ssize_t _read(int fd, void *buf, size_t count);
void *_sbrk(intptr_t increment);

/*===========================================================================*/
/* Module local functions.                                                   */
/*===========================================================================*/

/*===========================================================================*/
/* Module exported functions.                                                */
/*===========================================================================*/

void io_init(uart_driver_t *udp) {

    setbuf(stdout, NULL);
    setbuf(stderr, NULL);
    setbuf(stdin, NULL);

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
ssize_t _write(int fd, void *buf, size_t count) {

    size_t trasmitted = 0;

    (void)fd;

    if (io_udp != NULL) {
        trasmitted = uart_write(io_udp, (uint8_t *)buf, count);
    }

    return ((ssize_t)(trasmitted));
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
ssize_t _read(int fd, void *buf, size_t count) {

    size_t received = 0;

    (void)fd;

    if (io_udp != NULL) {
        received = uart_read(io_udp, (uint8_t *)buf, count);
    }

    return ((ssize_t)(received));
}

/**
 * @brief   Increment location of program break.
 * @details This function allows to increase the program break. The result is
 *          to allocate memory to the process.
 *
 * @param[in] increment     number of bytes to increment
 * @return                  previuos program break.
 *
 * @special
 */
void *_sbrk(intptr_t increment) {

    /*
     * Using external symbols:
     *   __heap_base__ : is placed at the beginning of the heap chunk
     *   __heap_end__  : is placed at the end of available heap chunk
     *
     * For definition of __heap_base__ and __heap_end__,
     * see linker script file.
     *
     * Limitation:
     *   multiple heap chunks not handled
     *   re-entrance (multi-threading) not handled
     */
    extern uint8_t __heap_base__;
    extern uint8_t __heap_end__;

    static uint8_t *heap_base = &__heap_base__;
    uint8_t *prev_head_base;

    prev_head_base = heap_base;
    if ((heap_base + increment) > (uint8_t*) &__heap_end__) {
        /* not enough memory, set errno and return -1 */
        errno = ENOMEM;
        return (void *) -1;
    }

    /* enough memory, increment heap base */
    heap_base += increment;

    /* return prior head base */
    return prev_head_base;
}

/** @} */
