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
__asm(".global __use_no_semihosting");

#include <io.h>

#include <stdio.h>
#include <errno.h>
#include <stdint.h>

#include <rt_sys.h>
#include <rt_misc.h>

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

/*===========================================================================*/
/* Module local functions.                                                   */
/*===========================================================================*/

/*===========================================================================*/
/* Module exported functions.                                                */
/*===========================================================================*/

void io_init(uart_driver_t *udp)
{
    FILE *new_stdout;
    FILE *new_stderr;
    FILE *new_stdin;

    /* Set streams to avoid bad IO file descriptor */
    new_stdout = freopen("uart", "w", stdout);
    if (new_stdout) {
        setvbuf(new_stdout, NULL, _IONBF, 0);
    }

    new_stderr = freopen("uart", "w", stderr);
    if (new_stderr) {
        setvbuf(new_stderr, NULL, _IONBF, 0);
    }

    new_stdin = freopen("uart", "r", stdin);
    if (new_stdin) {
        setvbuf(new_stdin, NULL, _IONBF, 0);
    }

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
 * @brief   Open a file.
 * @details Open the file specified by name. 
 *
 * @param[in] name          name of file to open
 * @param[in] openmode      open mode
 * @return                  file handle or -1 if an error occurs.
 *
 * @special
 */
FILEHANDLE _sys_open(const char *name, int openmode) {

    return 1;
}

/**
 * @brief   Close a file.
 *
 * @param[in] fh            file handle of the file to be close
 * @return                  0 if no error occurs, nonzero value otherwise.
 *
 * @special
 */
int _sys_close(FILEHANDLE fh) {

    return 0;
}

/**
 * @brief   Retrive the command line that invoked the current application.
 *
 * @param[in] cmd           pointer to a buffer that can store the command line
 * @param[in] len           buffer length
 * @return                  the pointer to the command line, if succefful, NULL
 *                          otherwise.
 *
 * @special
 */
char *_sys_command_string(char *cmd, int len) {

    return NULL;
}

/**
 * @brief   Write to IO.
 * @note    This function redirect IO write on UART.
 * @details Write up to count bytes from buffer buf to the file related to the
 *          file handle fh.
 *
 * @param[in] fh            file handle
 * @param[in] buf           buffer from which to write
 * @param[in] len           number of bytes to be written
 * @param[in] mode          write mode (not used)
 * @return                  number of bytes not written.
 *
 * @special
 */
int _sys_write(FILEHANDLE fh, const unsigned char *buf, unsigned len, int mode) {

    size_t trasmitted = 0;

    (void)fh;
    (void)mode;

    if (io_udp != NULL) {
        trasmitted = uart_write(io_udp, (uint8_t *)buf, len);
    }

    return ((int)(len - trasmitted));
}

/**
 * @brief   Read from IO.
 * @note    This function redirect IO read on UART.
 * @details Attempt to read up to count bytes from the file related to the file
 *          handle fh into the buffer buf.
 *
 * @param[in] fh            file handle
 * @param[in] buf           buffer in which the read data are stored
 * @param[in] len           number of bytes to be read
 * @param[in] mode          read mode (not used)
 * @return                  number of bytes not read.
 *
 * @special
 */
int _sys_read(FILEHANDLE fh, unsigned char *buf, unsigned len, int mode) {

    size_t received = 0;

    (void)fh;
    (void)mode;

    if (io_udp != NULL) {
        received = uart_read(io_udp, (uint8_t *)buf, len);
    }

    return ((int)(len - received));
}

/**
 * @brief   Write a characted to the console.
 *
 * @param[in] ch            charachter to be written
 *
 * @special
 */
void _ttywrch(int ch) {

}

/**
 * @brief   Check if a file handle is attached to an interactive device.
 *
 * @param[in] fh            file handle to be checked
 * @return                  '0' if the file handle is attached to an
 *                          interactive devide, '1' otherwise.
 *
 * @special
 */
int _sys_istty(FILEHANDLE fh) {

    return 0;
}

/**
 * @brief   Put the file pointer at the offset specified by the parameter pos
 *          from the beginning of the file.
 *
 * @param[in] fh            file handle
 * @param[in] pos           offset to which file handle has to be moved
 * @return                  negative if an error occurs, non-negative otherwise.
 *
 * @special
 */
int _sys_seek(FILEHANDLE fh, long pos) {

    return -1;
}

/**
 * @brief   Return the current length of a file.
 *
 * @param[in] fh            file handle of the file for which to return the
 *                          length
 * @return                  current length of the file or negative if an error
 *                          occurs.
 *
 * @special
 */
long _sys_flen(FILEHANDLE fh) {

    return -1;
}

/**
 * @brief   Terminate the program passing a return code back to the user.
 *
 * @param[in] return_code   return code
 *
 * @special
 */
void _sys_exit(int return_code) {

    while (1)
        ;
}

/** @} */
