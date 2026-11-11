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
 * @file    psi5.c
 * @brief   PSI5 driver source file.
 *
 * @addtogroup DRIVERS
 * @addtogroup COMMS
 * @ingroup DRIVERS
 * @addtogroup PSI5
 * @ingroup COMMS
 * @{
 */

#include <psi5.h>
#include <psi5_private.h>

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

static uint8_t next_dma_index = 0U;
static uint8_t prev_dma_count = PSI5_MAX_DECODE_EDGE_NUM;

/*===========================================================================*/
/* Local function prototypes                                                 */
/*===========================================================================*/

/*===========================================================================*/
/* Module local functions.                                                   */
/*===========================================================================*/

/**
 * @brief   Returns the pointer to the PSI5 driver related to a specified TIM
 *          driver.
 *
 * @param[in] tdp       pointer to an @p tim_driver_t structure
 *
 * @notapi
 */
static psi5_driver_t *psi5_lld_get_drv(tim_driver_t *tdp) {

    if (tdp == &DRV_TIM1) {
        return (&DRV_PSI5_1);
    } else {
        return (&DRV_PSI5_2);
    }
}

/**
 * @brief   Calculates the sampling clock (tDTS) and the input capture filter
 *          based on the glitch filter size specified by the API psi5_set_fsize.
 *
 * @param[in] pdp       pointer to an @p psi5_driver_t structure
 * @param[in] fsize     glitch filter size [ns]
 * @return              filter size [ns] closest to the required one (fsize)
 *                      achivable with the TIM used for PSI5 emulation.
 *
 * @notapi
 */
uint32_t psi5_lld_set_fsize(psi5_driver_t *pdp, uint32_t fsize) {

    uint32_t flength[] = {12U, 16U, 24U, 32U, 48U, 64U, 80U, 96U, 128U, 160U, 192U, 256U};

    float t_dts[3];
    uint8_t i, j, dts_index, flength_index;
    float res;

    if (fsize == 0U) {
        pdp->ckd = TIM_CKD_1;
        pdp->icf = TIM_ICF_NO_FILTER;
        return 0U;
    } else {
        t_dts[0] = ((float)1000000000 / pdp->clock); /* tDTS = ttim_ker_ck     */
        t_dts[1] = ((float)2000000000 / pdp->clock); /* tDTS = 2 * ttim_ker_ck */
        t_dts[2] = ((float)4000000000 / pdp->clock); /* tDTS = 4 * ttim_ker_ck */

        for (i = 0U; i < 3U; i++) {
            for (j = 0; j < 12U; j++) {
                if ((flength[j] * t_dts[i]) >= fsize) {
                    break;
                }
            }
            if (i == 0U && j == 0U) {
                dts_index = 0U;
                flength_index = 0U;
                res = (flength[0] * t_dts[0]);
                break;
            }
            j--;
            if (i == 0U) {
                dts_index = 0U;
                flength_index = j;
                res = (flength[j] * t_dts[0]);
            } else {
                if ((float)(fsize - (flength[j] * t_dts[i])) < (float)(fsize - res)) {
                    dts_index = i;
                    flength_index = j;
                    res = (flength[j] * t_dts[i]);
                }
            }
        }

        /* Configure tDTS.*/
        switch(dts_index) {
            case 0:
                pdp->ckd = TIM_CKD_1;
                break;
            case 1:
                pdp->ckd = TIM_CKD_2;
                break;
            case 3:
            default: 
                pdp->ckd = TIM_CKD_4;
                break;
        }

        /* Configure input capture filter length.*/
        switch(flength_index) {
            case 0:  /* flength = 12 * tDTS */
                pdp->icf = TIM_ICF_DTS_CK_DIV2_N6;
                break;
            case 1:  /* flength = 16 * tDTS */
                pdp->icf = TIM_ICF_DTS_CK_DIV2_N8;
                break;
            case 2:  /* flength = 24 * tDTS */
                pdp->icf = TIM_ICF_DTS_CK_DIV4_N6;
                break;
            case 3:  /* flength = 32 * tDTS */
                pdp->icf = TIM_ICF_DTS_CK_DIV4_N8;
                break;
            case 4:  /* flength = 48 * tDTS */
                pdp->icf = TIM_ICF_DTS_CK_DIV8_N6;
                break;
            case 5:  /* flength = 64 * tDTS */
                pdp->icf = TIM_ICF_DTS_CK_DIV8_N8;
                break;
            case 6:  /* flength = 80 * tDTS */
                pdp->icf = TIM_ICF_DTS_CK_DIV16_N5;
                break;
            case 7:  /* flength = 96 * tDTS */
                pdp->icf = TIM_ICF_DTS_CK_DIV16_N6;
                break;
            case 8:  /* flength = 108 * tDTS */
                pdp->icf = TIM_ICF_DTS_CK_DIV16_N8;
                break;
            case 9:  /* flength = 160 * tDTS */
                pdp->icf = TIM_ICF_DTS_CK_DIV32_N5;
                break;
            case 10: /* flength = 192 * tDTS */
                pdp->icf = TIM_ICF_DTS_CK_DIV32_N6;
                break;
            case 11: /* flength = 256 * tDTS */
            default:
                pdp->icf = TIM_ICF_DTS_CK_DIV32_N8;
                break;
        }
	    
        return (uint32_t)res;
    }
}

/**
 * @brief   Calculates even parity bit.
 *
 * @param[in] data      data on which to calculate the even parity bit
 *
 * @notapi
 */
static uint8_t psi5_lld_calculate_parity(uint32_t data) {

    uint8_t parity = 0U;

    while (data != 0U) {
        parity = parity ^ ((uint8_t)(data & 1U));
        data = data >> 1U;
    }

    return parity;
}

/**
 * @brief   Calculates 3bits CRC based on polynomial x3+x+1
 *
 * @param[in] data      data on which to calculate the CRC
 *
 * @notapi
 */
static uint8_t psi5_lld_calculate_crc(uint32_t data, uint8_t length) {

    uint8_t crc[3] = {1U, 1U, 1U};
    uint8_t new_crc[3] = {0U, 0U, 0U};
    uint8_t i, j;

    /* Data length is increased of 3 bits since, as specified in the PSI5
       specification, the data bits must be extended by three zeros (as MSBs).*/
    for(i = 0U; i < length + 3U; i++) {
        new_crc[2] = crc[1];
        new_crc[1] = crc[0] ^ crc[2];
        new_crc[0] = crc[2] ^ ((uint8_t)((data >> i) & 0x1U));
        for(j = 0; j < 3U; j++) {
            crc[j] = new_crc[j];
        }
    }

    return (((0x1U & new_crc[2]) << 2U) | ((0x1U & new_crc[1]) << 1U) | (0x1U & new_crc[0]));
}

/**
 * @brief   Update callback related to the TIM used for PSI5 emulation.
 *
 * @param[out] tdp      pointer to an @p tim_driver_t structure
 *
 * @notapi
 */
static void tim_update_callback(tim_driver_t *tdp) {

    uint8_t edge_num;
    uint8_t re_dma_count, fe_dma_count;
    uint16_t delta_min, delta_max;
    uint8_t i;
    uint8_t previous_bit;
    uint8_t bit[PSI5_MAX_FRAME_BIT_NUM] = {0U};
    uint8_t j = 0U;
    psi5_driver_t *pdp;

    bool data_valid = true;

    /* Get pointer to the PSI5 driver.*/
    pdp = psi5_lld_get_drv(tdp);

    /* Get the number of edges sampled by the DMA.*/
    re_dma_count = (uint8_t)tim_get_channel_dma_count(tdp, TIM_DMA_OPS_CH1);
    fe_dma_count = (uint8_t)tim_get_channel_dma_count(tdp, TIM_DMA_OPS_CH2);

    if (re_dma_count != fe_dma_count) {
        /* Decoding error.*/
        if (pdp->err_cb != NULL) {
            pdp->err_cb(pdp);
        }
        return;
    }

    /* Calculate the number of edges related to the current frame and update
       the DMA count for the decoding of the next frame.*/
    if (prev_dma_count < re_dma_count) {
        prev_dma_count += PSI5_MAX_DECODE_EDGE_NUM;
    }
    edge_num = (prev_dma_count - re_dma_count);
    prev_dma_count = re_dma_count;

    /* Decode Manchester frame.*/

    /* Calculate delta_min equal to T_BIT/2 minus a tollerance in pecentage and
       delta_max equal to T_BIT/2 plus a tollerance in pecentage.*/
    delta_min = (uint16_t)((pdp->tbit / 2U) - ((uint32_t)((pdp->tbit / 2U) * (pdp->tolerance))) / 100UL);
    delta_max = (uint16_t)((pdp->tbit / 2U) + ((uint32_t)((pdp->tbit / 2U) * (pdp->tolerance))) / 100UL);

    /* Check first bit is a 0 (S0).*/
    /* if (falling[0] - rising[0] != T_BIT/2) { */
    if (!((((uint16_t)(pdp->fe[next_dma_index] - pdp->re[next_dma_index])) > delta_min) &&
          (((uint16_t)(pdp->fe[next_dma_index] - pdp->re[next_dma_index])) < delta_max))) {
        /* Decoding error.*/
        /* Update the DMA index for the decoding of the next frame.*/
        next_dma_index = (next_dma_index + edge_num) % PSI5_MAX_DECODE_EDGE_NUM;
        if (pdp->err_cb != NULL) {
            pdp->err_cb(pdp);
        }
        return;
    } else {
        bit[j++] = 0U;
        previous_bit = 0U;
    }

    for (i = next_dma_index + 1U; i < next_dma_index + edge_num; i++) {
        /* Decode after a bit to 0.*/
        if (previous_bit == 0U) {
            /* if ((rising[i] - falling[i - 1]) == T_BIT/2) { */
            if ((((uint16_t)(pdp->re[(i % PSI5_MAX_DECODE_EDGE_NUM)] - pdp->fe[((i - 1U) % PSI5_MAX_DECODE_EDGE_NUM)])) > delta_min) &&
                (((uint16_t)(pdp->re[(i % PSI5_MAX_DECODE_EDGE_NUM)] - pdp->fe[((i - 1U) % PSI5_MAX_DECODE_EDGE_NUM)])) < delta_max)) {
                /* if ((falling[i] - rising[i]) == T_BIT/2) { */
                if ((((uint16_t)(pdp->fe[(i % PSI5_MAX_DECODE_EDGE_NUM)] - pdp->re[(i % PSI5_MAX_DECODE_EDGE_NUM)])) > delta_min) &&
                    (((uint16_t)(pdp->fe[(i % PSI5_MAX_DECODE_EDGE_NUM)] - pdp->re[(i % PSI5_MAX_DECODE_EDGE_NUM)])) < delta_max)) {
                    bit[j++] = 0U;
                    previous_bit = 0U;
                /* } else if ((falling[i] - rising[i]) == T_BIT) { */
                } else if ((((uint16_t)(pdp->fe[(i % PSI5_MAX_DECODE_EDGE_NUM)] - pdp->re[(i % PSI5_MAX_DECODE_EDGE_NUM)])) > (2U * delta_min)) &&
                           (((uint16_t)(pdp->fe[(i % PSI5_MAX_DECODE_EDGE_NUM)] - pdp->re[(i % PSI5_MAX_DECODE_EDGE_NUM)])) < (2U * delta_max))) {
                    bit[j++] = 0U;
                    bit[j++] = 1U;
                    previous_bit = 1U;
                } else {
                    /* Decoding error.*/
                    data_valid = false;
                    break;
                }
            } else {
                /* Decoding error.*/
                data_valid = false;
                break;
            }
            /* Check second bit is a 0 (S1).*/
            if (i == 1U && bit[0] != 0U) {
                /* Decoding error.*/
                data_valid = false;
                break;
            }
        /* Decode after a bit to 1.*/
        } else {
            /* if (((rising[i] - falling[i - 1]) == T_BIT/2) && ((falling[i] - rising[i]) == T_BIT/2)) { */
            if (((((uint16_t)(pdp->re[(i % PSI5_MAX_DECODE_EDGE_NUM)] - pdp->fe[((i - 1U) % PSI5_MAX_DECODE_EDGE_NUM)])) > delta_min) &&
                 (((uint16_t)(pdp->re[(i % PSI5_MAX_DECODE_EDGE_NUM)] - pdp->fe[((i - 1U) % PSI5_MAX_DECODE_EDGE_NUM)])) < delta_max)) &&
                ((((uint16_t)(pdp->fe[(i % PSI5_MAX_DECODE_EDGE_NUM)] - pdp->re[(i % PSI5_MAX_DECODE_EDGE_NUM)])) > delta_min) &&
                 (((uint16_t)(pdp->fe[(i % PSI5_MAX_DECODE_EDGE_NUM)] - pdp->re[(i % PSI5_MAX_DECODE_EDGE_NUM)])) < delta_max))) {
                bit[j++] = 1U;
                previous_bit = 1U;
            /* } else if ((rising[i] - falling[i - 1]) == T_BIT) { */
            } else if ((((uint16_t)(pdp->re[(i % PSI5_MAX_DECODE_EDGE_NUM)] - pdp->fe[((i - 1U) % PSI5_MAX_DECODE_EDGE_NUM)])) > (2U * delta_min)) &&
                       (((uint16_t)(pdp->re[(i % PSI5_MAX_DECODE_EDGE_NUM)] - pdp->fe[((i - 1U) % PSI5_MAX_DECODE_EDGE_NUM)])) < (2U * delta_max))) {
                /* if ((falling[i] - rising[i]) == T_BIT/2)) { */
                if ((((uint16_t)(pdp->fe[(i % PSI5_MAX_DECODE_EDGE_NUM)] - pdp->re[(i % PSI5_MAX_DECODE_EDGE_NUM)])) > delta_min) &&
                    (((uint16_t)(pdp->fe[(i % PSI5_MAX_DECODE_EDGE_NUM)] - pdp->re[(i % PSI5_MAX_DECODE_EDGE_NUM)])) < delta_max)) {
                    bit[j++] = 0U;
                    previous_bit = 0U;
                /* } else if ((falling[i] - rising[i]) == T_BIT)) { */
                } else if ((((uint16_t)(pdp->fe[(i % PSI5_MAX_DECODE_EDGE_NUM)] - pdp->re[(i % PSI5_MAX_DECODE_EDGE_NUM)])) > (2U * delta_min)) &&
                           (((uint16_t)(pdp->fe[(i % PSI5_MAX_DECODE_EDGE_NUM)] - pdp->re[(i % PSI5_MAX_DECODE_EDGE_NUM)])) < (2U * delta_max))) {
                    bit[j++] = 0U;
                    bit[j++] = 1U;
                    previous_bit = 1U;
                } else {
                    /* Decoding error.*/
                    data_valid = false;
                    break;
                }
    
            } else {
                /* Decoding error.*/
                data_valid = false;
                break;
            }
        }
    }

    /* Update the DMA index for the decoding of the next frame.*/
    next_dma_index = (next_dma_index + edge_num) % PSI5_MAX_DECODE_EDGE_NUM;

    /* Reinitialize the PSI5 data.*/
    pdp->data = 0U;

    /* If no error is occurred ...*/
    if ((bool)data_valid == true) {
        /* Data bits are transmitted starting from the LSB. Skip first 2 bits
           (start bits).*/
        /*lint -e690 */
        for (i = 0U; i < pdp->dlength; i++) {
            pdp->data |= (((uint32_t)bit[i + 2U]) << i);
        }
        /*lint +e690 */
        /* Check control bits (parity or CRC).*/
        if (pdp->ctrl == PSI5_CTRL_PARITY) {
            if (psi5_lld_calculate_parity(pdp->data) == bit[pdp->dlength + 2U]) {
                if (pdp->cb != NULL) {
                    pdp->cb(pdp);
                }
            } else {
                if (pdp->err_cb != NULL) {
                    pdp->err_cb(pdp);
                }
            }
        } else {
            uint8_t crc = 0U;
            /* CRC bits are transmitted starting from the MSB.*/
            for (i = 0U; i < 3U; i++) {
                crc |= (uint8_t)((uint32_t)bit[i + pdp->dlength + 2U] << (2U - i));
            }
            if (psi5_lld_calculate_crc(pdp->data, pdp->dlength) == crc) {
                if (pdp->cb != NULL) {
                    pdp->cb(pdp);
                }
            } else {
                if (pdp->err_cb != NULL) {
                    pdp->err_cb(pdp);
                }
            }
        }
    /* ... otherwise.*/
    } else {
        if (pdp->err_cb != NULL) {
            pdp->err_cb(pdp);
        }
    }
}

/**
 * @brief   Resets PSI5 parameters.
 *
 * @param[out] pdp      pointer to an @p psi5_driver_t structure to
 *                      be reset
 *
 * @notapi
 */
static void psi5_lld_reset_paramters(psi5_driver_t *pdp) {

    uint8_t i;

    /* Set all driver configuration parameters to default values.*/
    pdp->tim       = NULL;
    pdp->prio      = IRQ_MIN_PRIORITY;
    for (i = 0U; i < 2U; i++) {
        pdp->dma_conf[i].dma_stream_id       = DMA1_STREAM1_ID;
        pdp->dma_conf[i].dma_stream_bus_prio = DMA_PRIORITY_MAX;
        pdp->dma_conf[i].dma_stream_irq_prio = IRQ_PRIORITY_10;
    }
    pdp->baudrate  = PSI5_BAUDRATE_125kbps;
    pdp->dlength   = 8U;
    pdp->ctrl      = PSI5_CTRL_PARITY;
    pdp->clock     = 0U;
    pdp->tbit      = 0U;
    pdp->tolerance = 1U;
    pdp->ckd       = TIM_CKD_1;
    pdp->icf       = TIM_ICF_NO_FILTER;
    for (i = 0U; i < PSI5_MAX_DECODE_EDGE_NUM; i++) {
        pdp->re[i] = 0U;
        pdp->fe[i] = 0U;
    }
    pdp->data      = 0U;
    pdp->cb        = NULL;
    pdp->err_cb    = NULL;
}

/*===========================================================================*/
/* Module exported functions.                                                */
/*===========================================================================*/

void psi5_init(psi5_driver_t *pdp) {

    /* Reset PSI5 parameters.*/
    psi5_lld_reset_paramters(pdp);

    /* Set PSI5 driver instance as private data for TIM.*/
    if (pdp == &DRV_PSI5_1) {
        pdp->tim = &DRV_TIM1;
    } else {
        pdp->tim = &DRV_TIM8;
    }

    /* Initialize TIM driver instance.*/
    tim_init(pdp->tim);

    /* Get clock of TIM used for the PSI5 emulation based on clocktree
       configuration.*/
    pdp->clock = tim_get_clock(pdp->tim);
}

uint32_t psi5_set_prio(psi5_driver_t *pdp, uint32_t prio) {

    uint32_t prev_prio;

    prev_prio = pdp->prio;
    pdp->prio = prio;

    return prev_prio;
}

void psi5_set_dma_conf(psi5_driver_t *pdp, uint8_t ch,
                       psi5_dma_conf_t *dma_conf) {

    pdp->dma_conf[ch].dma_stream_id       = dma_conf->dma_stream_id;
    pdp->dma_conf[ch].dma_stream_bus_prio = dma_conf->dma_stream_bus_prio;
    pdp->dma_conf[ch].dma_stream_irq_prio = dma_conf->dma_stream_id;
}

psi5_baudrate_t psi5_set_baudrate(psi5_driver_t *pdp, psi5_baudrate_t baudrate) {

    psi5_baudrate_t prev_baudrate;

    prev_baudrate = pdp->baudrate;
    pdp->baudrate = baudrate;

    return prev_baudrate;
}

uint8_t psi5_set_dlength(psi5_driver_t *pdp, uint8_t dlength) {

    uint8_t prev_dlength;

    prev_dlength = pdp->dlength;
    pdp->dlength = dlength;

    return prev_dlength;
}

psi5_ctrl_t psi5_set_ctrl(psi5_driver_t *pdp, psi5_ctrl_t ctrl) {

    psi5_ctrl_t prev_ctrl;

    prev_ctrl = pdp->ctrl;
    pdp->ctrl = ctrl;

    return prev_ctrl;
}

uint8_t psi5_set_tolerance(psi5_driver_t *pdp, uint8_t tolerance) {

    uint8_t prev_tolerance;

    prev_tolerance = pdp->tolerance;
    pdp->tolerance = tolerance;

    return prev_tolerance;  
}

uint32_t psi5_set_fsize(psi5_driver_t *pdp, uint32_t fsize) {

    return psi5_lld_set_fsize(pdp, fsize);
}

psi5_cb_t psi5_set_cb(psi5_driver_t *pdp, psi5_cb_t cb) {

    psi5_cb_t prev_cb;

    prev_cb = pdp->cb;
    pdp->cb = cb;

    return prev_cb;
}

psi5_cb_t psi5_set_err_cb(psi5_driver_t *pdp, psi5_cb_t err_cb) {

    psi5_cb_t prev_err_cb;

    prev_err_cb = pdp->err_cb;
    pdp->err_cb = err_cb;

    return prev_err_cb;
}

void psi5_start(psi5_driver_t *pdp) {

    uint8_t frame_length;
    uint16_t frame_duration_us;
    uint16_t frame_duration_tick;
    uint8_t  control_length;
    uint32_t baudrate_kbs;

    if (pdp->ctrl == PSI5_CTRL_PARITY) {
        control_length = PSI5_PARITY_BIT_NUM;
    } else {
        control_length = PSI5_CRC_BIT_NUM;
    }

    if (pdp->baudrate == PSI5_BAUDRATE_125kbps) {
        baudrate_kbs = 125000;
    } else {
        baudrate_kbs = 189000;
    }

    frame_length = PSI5_START_BIT_NUM + pdp->dlength + control_length;
    frame_duration_us = (uint16_t)(((uint32_t)frame_length * 1000000U) / baudrate_kbs);

    /* Since TIM clock frequency is set to 100MHz => in each us there are 100
       ticks.*/
    frame_duration_tick = (uint16_t)(frame_duration_us * 100U);

    /* Since TIM clock frequency is set to 100MHz => the bit time in terms of
       ticks is equal to the ratio between the TIM clock frequency and the
       baudrate.*/
    pdp->tbit = (100000000UL / baudrate_kbs);

    /* Configure TIM input capture channel 1 DMA.*/
    tim_set_dma_conf(pdp->tim, TIM_DMA_OPS_CH1, &pdp->dma_conf[0]);

    /* Configure TIM input capture channel 2 DMA.*/
    tim_set_dma_conf(pdp->tim, TIM_DMA_OPS_CH2, &pdp->dma_conf[1]);

    /* Configure TIM driver as ICU with CC2 mapped on channel 1 (using the
       indirect mode) to capture the falling edge and CC1 mapped on channel 1
       to capture the rising edge.*/
    (void)tim_set_ch_mode(pdp->tim, 1U, TIM_MODE_ICU_RISING_EDGE);
    (void)tim_set_ch_mode(pdp->tim, 2U, TIM_MODE_ICU_FALLING_EDGE_IND);

    /* Configure TIM clock to 100MHz.*/
    (void)tim_set_presc(pdp->tim, ((tim_presc_t)TIM_DIVIDE_BY(tim_get_clock(pdp->tim) / 100000000UL)));

    /* Configure TIM in trigger mode: the counter starts on the first rising
       edge, but it is not reset.*/
    (void)tim_set_sms(pdp->tim, TIM_SMODE_TRIGGER);

    /* Enable one pulse mode.*/
    (void)tim_enable_opm(pdp->tim, true);

    /* Configure TIM update callback (it will be invoked at the end of the
       frame transmission).*/
    (void)tim_set_cb(pdp->tim, TIM_CB_OPS_UPDATE, tim_update_callback);

    /* Configure channel 1 edge detector as trigger.*/
    (void)tim_set_trgi(pdp->tim, TIM_TRGI_TI1F_ED);

    /* Configure channel 1 and channel 2 glitch filter sizes.*/
    if (pdp->icf != TIM_ICF_NO_FILTER) {
        (void)tim_set_ckd(pdp->tim, pdp->ckd);
        (void)tim_set_ic_filter(pdp->tim, 1U, pdp->icf);
        (void)tim_set_ic_filter(pdp->tim, 2U, pdp->icf);
    }

    /* Apply TIM configurations.*/
    tim_start(pdp->tim);

    /* Enable TIM update callback.*/
    tim_enable_update_callback(pdp->tim);

    /* Configure TIM period (TIM period is equal to the frame duration. In this
       way, when the TIM update interrupt will occur, the frame will be fully
       received).*/
    tim_set_interval(pdp->tim, frame_duration_tick);

    /* Enable TIM channels 1 and 2 in DMA mode.*/
    tim_enable_channel_dma_ic(pdp->tim, 1U, pdp->re, PSI5_MAX_DECODE_EDGE_NUM);
    tim_enable_channel_dma_ic(pdp->tim, 2U, pdp->fe, PSI5_MAX_DECODE_EDGE_NUM);
}

uint32_t psi5_get_data(psi5_driver_t *pdp) {

    return pdp->data;
}

void psi5_stop(psi5_driver_t *pdp) {

    /* Disable TIM used for PSI5 emulation.*/
    tim_disable(pdp->tim);

    /* Stop TIM used for PSI5 emulation.*/
    tim_stop(pdp->tim);
}

void psi5_deinit(psi5_driver_t *pdp) {

    /* Set pointer to TIM register block to NULL.*/
    pdp->tim = NULL;

    /* Reset PSI5 parameters.*/
    psi5_lld_reset_paramters(pdp);
}

/** @} */
