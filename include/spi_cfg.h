/****************************************************************************
 *
 * Copyright (c) 2022-2026 STMicroelectronics - All Rights Reserved
 *
 * License terms: STMicroelectronics Proprietary in accordance with licensing
 * terms SLA0089 at www.st.com.
 *
 * THIS SOFTWARE IS DISTRIBUTED "AS IS," AND ALL WARRANTIES ARE DISCLAIMED,
 * INCLUDING MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE.
 *
 * EVALUATION ONLY - NOT FOR USE IN PRODUCTION
 *****************************************************************************/
/**
 * @file    spi_cfg.h
 * @brief   SPI driver configuration file.
 *
 * @addtogroup DRIVERS
 * @addtogroup COMMS
 * @ingroup DRIVERS
 * @addtogroup SPI
 * @ingroup COMMS
 * @{
 */

#ifndef _SPI_CFG_H_
#define _SPI_CFG_H_

/**
 * @name   SPI units.
 * @{
 */
#define SPI_CFG_USE_SPI1                    TRUE
#define SPI_CFG_USE_SPI2                    FALSE
#define SPI_CFG_USE_SPI3                    FALSE
#define SPI_CFG_USE_SPI4                    FALSE
/** @} */

/**
 * @name    Static IRQ priorities
 * @{
 */
#define SPI_CFG_IRQ_SPI1_PRIO               6
#define SPI_CFG_IRQ_SPI2_PRIO               6
#define SPI_CFG_IRQ_SPI3_PRIO               6
#define SPI_CFG_IRQ_SPI4_PRIO               6
/** @} */

#endif /* _SPI_CFG_H_ */

/** @} */
