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
 * @file    psi5.h
 * @brief   PSI5 driver header file.
 *
 * @addtogroup DRIVERS
 * @addtogroup COMMS
 * @ingroup DRIVERS
 * @addtogroup PSI5
 * @ingroup COMMS
 * @{
 */

#ifndef _PSI5_H_
#define _PSI5_H_

#include <typedefs.h>
#include <tim.h>
#include <dma.h>
#include <clock.h>
#include <irq.h>

/*===========================================================================*/
/* Module constants.                                                         */
/*===========================================================================*/

/**
 * @name    Number of start bits
 * @{
 */
#define PSI5_START_BIT_NUM                  2U
/** @} */

/**
 * @name    Minimum number of data bits
 * @{
 */
#define PSI5_MIN_DATA_BIT_NUM               10U
/** @} */

/**
 * @name    Maximum number of data bits
 * @{
 */
#define PSI5_MAX_DATA_BIT_NUM               28U
/** @} */

/**
 * @name    Number of parity bits
 * @{
 */
#define PSI5_PARITY_BIT_NUM                 1U
/** @} */

/**
 * @name    Number of CRC bits
 * @{
 */
#define PSI5_CRC_BIT_NUM                    3U
/** @} */

/**
 * @name    Maximum number of frame bits
 * @{
 */
#define PSI5_MAX_FRAME_BIT_NUM              (PSI5_START_BIT_NUM + PSI5_MAX_DATA_BIT_NUM + PSI5_CRC_BIT_NUM)
/** @} */

/**
 * @name    Maximum number of elements of the DMA circular buffer.
 * @note    This parameters must be at least (2 * PSI5_MAX_FRAME_BIT_NUM).
 * @{
 */
#define PSI5_MAX_DECODE_EDGE_NUM            30U
/** @} */

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
 * @brief   Type of an PSI5 driver.
 */
typedef struct psi5_driver psi5_driver_t;

/**
 * @enum psi5_baudrate_t
 * PSI5 supported baudrates.
 */
typedef enum {
    PSI5_BAUDRATE_125kbps,   /**< Baudrate = 125kbps */
    PSI5_BAUDRATE_189kbps    /**< Baudrate = 189kbps */
} psi5_baudrate_t;

/**
 * @enum psi5_ctrl_t
 * PSI5 supported control configurations.
 */
typedef enum {
    PSI5_CTRL_PARITY,        /**< Parity bit */
    PSI5_CTRL_CRC            /**< CRC bits   */
} psi5_ctrl_t;

/**
 * @brief   Type of PSI5 DMA configuration.
 *
 */
typedef tim_dma_conf_t psi5_dma_conf_t;

/**
 * @brief   Type of PSI5 callbacks.
 *
 * @param[in] pdp       pointer to the @p psi5_driver_t object
 */
typedef void (*psi5_cb_t)(psi5_driver_t *pdp);


/*===========================================================================*/
/* Module macros.                                                            */
/*===========================================================================*/

/*===========================================================================*/
/* External declarations.                                                    */
/*===========================================================================*/

/* Export driver modules.*/
#include <psi5_instances.h>

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief   Initializes PSI5 instance.
 *
 * @param[in] pdp       pointer to a @p psi5_driver_t structure
 *
 * @api
 */
void psi5_init(psi5_driver_t *pdp);

/**
 * @brief   Configures PSI5 interrupt priority.
 * @note    This configuration becomes effective only after psi5_start.
 *
 * @param[in] pdp       pointer to a @p psi5_driver_t structure
 * @param[in] prio      interrupt priority to be configured
 * @return              previous interrupt priority.
 *
 * @api
 */
uint32_t psi5_set_prio(psi5_driver_t *pdp, uint32_t prio);

/**
 * @brief   Configures channel DMA.
 * @note    This configuration becomes effective only after psi5_start.
 * @note    Since the PSI5 emulation is based on the usage of the first two
 *          channels of the TIM used for the emulation, two DMA configurations
 *          are required.
 *
 * @param[in] pdp       pointer to a @p psi5_driver_t structuree
 * @param[in] ch        channel to be configured [0:1].
 * @param[in] dma_conf  pointer to DMA configuration.
 *
 * @api
 */
void psi5_set_dma_conf(psi5_driver_t *pdp, uint8_t ch,
                       psi5_dma_conf_t *dma_conf);

/**
 * @brief   Configures PSI5 baudrate.
 * @note    This configuration becomes effective only after psi5_start.
 *
 * @param[in] pdp       pointer to a @p psi5_driver_t structure
 * @param[in] baudrate  baudrate to be configured. It can be one of the
 *                      following values:
 *            @arg @ref PSI5_BAUDRATE_125kbps
 *            @arg @ref PSI5_BAUDRATE_189kbps
 * @return              previous baudrate value.
 *
 * @api
 */
psi5_baudrate_t psi5_set_baudrate(psi5_driver_t *pdp, psi5_baudrate_t baudrate);

/**
 * @brief   Configures PSI5 data length.
 * @note    This configuration becomes effective only after psi5_start.
 *
 * @param[in] pdp       pointer to a @p psi5_driver_t structure
 * @param[in] dlength   data length to be configured. It can be in the range
 *                      [8:28].
 * @return              previous data length value.
 *
 * @api
 */
uint8_t psi5_set_dlength(psi5_driver_t *pdp, uint8_t dlength);

/**
 * @brief   Configures PSI5 control algorithm (parity bit or CRC bits).
 * @note    This configuration becomes effective only after psi5_start.
 *
 * @param[in] pdp       pointer to a @p psi5_driver_t structure
 * @param[in] ctrl      control algorithm to be used. It can be one of the
 *                      following values:
 *            @arg @ref PSI5_CTRL_PARITY
 *            @arg @ref PSI5_CTRL_CRC
 * @return              previous control algorithm.
 *
 * @api
 */
psi5_ctrl_t psi5_set_ctrl(psi5_driver_t *pdp, psi5_ctrl_t ctrl);

/**
 * @brief   Configures PSI5 decoding tolerance in percentage [1:100].
 * @note    This configuration becomes effective only after psi5_start.
 * @note    The PSI5 decoding is based on the comparison of the intervals
 *          between rising and falling edges of the PSI5 signal with a
 *          granularity of T_BIT/2. Tolerance is the percentage of T_BIT/2
 *          used as margin in the comparison.
 *
 * @param[in] pdp       pointer to a @p psi5_driver_t structure
 * @param[in] tolerance decoding tolerance in percentage. It must be an integer
 *                      in the range [1:100]
 * @return              previous decoding tolerance.
 *
 * @api
 */
uint8_t psi5_set_tolerance(psi5_driver_t *pdp, uint8_t tolerance);

/**
 * @brief   Configures PSI5 glitch filter size duration [ns].
 * @note    This configuration becomes effective only after psi5_start.
 * @note    This API allows to specify the minimum duration of PSI5 edges below
 *          which an edges is considered as a glitch and is discarded.
 *
 * @param[in] pdp       pointer to a @p psi5_driver_t structure
 * @param[in] fsize     glitch filter size [ns].
 * @return              filter size [ns] closest to the required one (fsize)
 *                      achivable with the TIM used for PSI5 emulation.
 *
 * @api
 */
uint32_t psi5_set_fsize(psi5_driver_t *pdp, uint32_t fsize);

/**
 * @brief   Configures pointers to PSI5 data callback function.
 * @note    This configuration becomes effective only after psi5_start.
 *
 * @param[in] pdp       pointer to a @p psi5_driver_t structure
 * @param[in] cb        pointer to PSI5 data callback function
 * @return              pointer to previuos PSI5 data callback function.
 *
* @api
 */
psi5_cb_t psi5_set_cb(psi5_driver_t *pdp, psi5_cb_t cb);

/**
 * @brief   Configures pointers to PSI5 error callback function.
 * @note    This configuration becomes effective only after psi5_start.
 *
 * @param[in] pdp       pointer to a @p psi5_driver_t structure
 * @param[in] cb        pointer to PSI5 error callback function
 * @return              pointer to previuos PSI5 error callback function.
 *
* @api
 */
psi5_cb_t psi5_set_err_cb(psi5_driver_t *pdp, psi5_cb_t err_cb);

/**
 * @brief   Starts a PSI5 instance.
 *
 * @param[in] pdp       pointer to a @p psi5_driver_t structure.
 *
 * @api
 */
void psi5_start(psi5_driver_t *pdp);

/**
 * @brief   Return PSI5 received data.
 * @note    This API must be invoked after a data callback is raised to
 *          indicate a new data is available.
 *
 * @param[in] pdp       pointer to a @p psi5_driver_t structure.
 *
 * @api
 */
uint32_t psi5_get_data(psi5_driver_t *pdp);

/**
 * @brief   Stops PSI5 instance.
 *
 * @param[in] pdp       pointer to a @p psi5_driver_t structure.
 *
 * @api
 */
void psi5_stop(psi5_driver_t *pdp);

/**
 * @brief   Deinitializes PSI5 instance.
 *
 * @param[in] pdp       pointer to a @p psi5_driver_t structure.
 *
 * @api
 */
void psi5_deinit(psi5_driver_t *pdp);

#ifdef __cplusplus
}
#endif

/*===========================================================================*/
/* Module inline functions.                                                  */
/*===========================================================================*/

/*===========================================================================*/
/* Module late inclusions.                                                   */
/*===========================================================================*/

#endif /* _PSI5_H_ */

/** @} */
