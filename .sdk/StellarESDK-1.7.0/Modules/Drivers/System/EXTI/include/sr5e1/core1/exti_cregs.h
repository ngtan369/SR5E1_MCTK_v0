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
 * @file    exti_cregs.h
 * @brief   EXTI core1 registers header file.
 *
 * @addtogroup DRIVERS
 * @addtogroup SYSTEM
 * @ingroup DRIVERS
 * @addtogroup EXTI
 * @ingroup SYSTEM
 * @{
 */

#ifndef _EXTI_CREGS_H_
#define _EXTI_CREGS_H_

/*===========================================================================*/
/* Module constants.                                                         */
/*===========================================================================*/

/*===========================================================================*/
/* Module pre-compile time settings.                                         */
/*===========================================================================*/

/*===========================================================================*/
/* Derived conants and error checks.                                       */
/*===========================================================================*/

/*===========================================================================*/
/* Module data structures and types.                                         */
/*===========================================================================*/

/*===========================================================================*/
/* Module macros.                                                            */
/*===========================================================================*/

/**
 * @brief   Pointer to the EXTI core1 interrupt mask 1 register
 *
 * @param[in] _exti_    pointer to the EXTI registers
 * @return              pointer to the register C1IMR1
 *
 * @api
 */
#define EXTI_CnIMR1(_exti_)                                         \
    ((_exti_)->C1IMR1)

/**
 * @brief   Pointer to the EXTI core1 interrupt mask 2 register
 *
 * @param[in] _exti_    pointer to the EXTI registers
 * @return              pointer to the register C1IMR2
 *
 * @api
 */
#define EXTI_CnIMR2(_exti_)                                         \
    ((_exti_)->C1IMR2)

/**
 * @brief   Pointer to the EXTI core1 event mask 1 register
 *
 * @param[in] _exti_    pointer to the EXTI registers
 * @return              pointer to the register C1EMR1
 *
 * @api
 */
#define EXTI_CnEMR1(_exti_)                                         \
    ((_exti_)->C1EMR1)

/**
 * @brief   Pointer to the EXTI core1 event mask 2 register
 *
 * @param[in] _exti_    pointer to the EXTI registers
 * @return              pointer to the register C1EMR2
 *
 * @api
 */
#define EXTI_CnEMR2(_exti_)                                         \
    ((_exti_)->C1EMR2)

/**
 * @brief   Pointer to the EXTI core1 pending 1 register
 *
 * @param[in] _exti_    pointer to the EXTI registers
 * @return              pointer to the register C1PR1
 *
 * @api
 */
#define EXTI_CnPR1(_exti_)                                          \
    ((_exti_)->C1PR1)

/**
 * @brief   Pointer to the EXTI core1 pending 2 register
 *
 * @param[in] _exti_    pointer to the EXTI registers
 * @return              pointer to the register C1PR1
 *
 * @api
 */
#define EXTI_CnPR2(_exti_)                                          \
    ((_exti_)->C1PR2)

/*===========================================================================*/
/* External declarations.                                                    */
/*===========================================================================*/

/*===========================================================================*/
/* Module inline functions.                                                  */
/*===========================================================================*/

/*===========================================================================*/
/* Module late inclusions.                                                   */
/*===========================================================================*/

#endif /* _EXTI_CREGS_H_ */

/** @} */
