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
 * @file    psi5_private.h
 * @brief   PSI5 driver private header file.
 *
 * @addtogroup DRIVERS
 * @addtogroup COMMS
 * @ingroup DRIVERS
 * @addtogroup PSI5
 * @ingroup COMMS
 * @{
 */

#ifndef _PSI5_PRIVATE_H_
#define _PSI5_PRIVATE_H_

#include <tim.h>

/*===========================================================================*/
/* Module constants.                                                         */
/*===========================================================================*/

/*===========================================================================*/
/* Module pre-compile time settings.                                         */
/*===========================================================================*/

/*===========================================================================*/
/* Derived constants and error checks.                                       */
/*===========================================================================*/

/*===========================================================================*/
/* Module data structures and types.                                         */
/*===========================================================================*/

/**
 * @brief   PSI5 driver structure.
 * @note    This structure is internal to the PSI5 driver.
 */
struct psi5_driver {
    /**
     * @brief   Pointer to TIM instance used for PSI5 emulation.
     */
    tim_driver_t             *tim;

    /**
     * @brief   TIM interrupt priority.
     */
    uint32_t                 prio;
    /**
     * @brief   DMA configurations.
     * @note    Since PSI5 emulation is based on two TIM channels, two DMA
     *          configurations are required.
     */
    psi5_dma_conf_t          dma_conf[2];
    /**
     * @brief   TIM channels allocated DMA descriptors.
     */
    const dma_descriptor_t   *dma_descriptor[2];
    /**
     * @brief   Baudrate (125kbs or 189kbs).
     */
    psi5_baudrate_t          baudrate;
    /**
     * @brief   Number of bits of data [10:28]
     */
    uint8_t                  dlength;
    /**
     * @brief   Control type (parity bit or CRC bits)
     */
    psi5_ctrl_t              ctrl;
    /**
     * @brief   Frequency [Hz] of TIM used for PSI5 emulation.
     */
    uint32_t                 clock;
    /**
     * @brief   Bit time for a single bit [tick].
     */
    uint32_t                 tbit;
    /**
     * @brief   Decoding tollerance [tick] as percentage of tbit.
     */
    uint8_t                  tolerance;
    /**
     * @brief   Glitch filter size [ns].
     */
    /**
     * @brief   Sampling clock of TIM used for PSI5 emulation.
     */
    tim_ckd_t                ckd;
    /**
     * @brief   Input capture filter of TIM used for PSI5 emulation.
     */
    tim_icf_t                icf;
    /**
     * @brief   Received rising edges.
     */
    uint16_t                 re[PSI5_MAX_DECODE_EDGE_NUM];
    /**
     * @brief   Received falling edges.
     */
    uint16_t                 fe[PSI5_MAX_DECODE_EDGE_NUM];
    /**
     * @brief   Received data.
     */
    uint32_t                 data;
    /**
     * @brief   Pointers to PSI5 data callback function.
     * @note    Set to @p NULL if not required.
     */
    psi5_cb_t                cb;
    /**
     * @brief   Pointers to PSI5 error callback function.
     * @note    Set to @p NULL if not required.
     */
    psi5_cb_t                err_cb;
};

/*===========================================================================*/
/* Module macros.                                                            */
/*===========================================================================*/

/*===========================================================================*/
/* External declarations.                                                    */
/*===========================================================================*/

/*===========================================================================*/
/* Module inline functions.                                                  */
/*===========================================================================*/

/*===========================================================================*/
/* Module late inclusions.                                                   */
/*===========================================================================*/

#endif /* _PSI5_PRIVATE_H_ */

/** @} */
