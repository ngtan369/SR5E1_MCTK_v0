/****************************************************************************
*
* Copyright (c) 2023 STMicroelectronics - All Rights Reserved
*
* License terms: STMicroelectronics Proprietary in accordance with licensing
* terms SLA0098 at www.st.com.
*
* THIS SOFTWARE IS DISTRIBUTED "AS IS," AND ALL WARRANTIES ARE DISCLAIMED,
* INCLUDING MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE.
*****************************************************************************/
/**
*   @file    SRxxx_L9907.c
*   @version M4_VERSION
*   @brief   This file provides firmware functions that implement the features
*            of the L9907 component of the Stellar Motor Control.
*          
*   @details This file provides firmware functions that implement the features
*            of the L9907 component of the Stellar Motor Control.
 */
/*
 * @addtogroup STELLAR_L9907_Library
 * @{
 */
#include "SRxxx_L9907.h"
#include "spi.h"
#include "gpio.h"
#include "dma.h"
#include "MC_type.h"

#define SPI_SW_CS

/*===========================================================================*/
/* SPI-related.                                                              */
/*===========================================================================*/
#define SPI_BUFFER_SIZE     1

volatile uint8_t spi1_completed = 0;

uint8_t spi_tx_buffer[SPI_BUFFER_SIZE*2];
uint8_t spi_rx_buffer[SPI_BUFFER_SIZE*2];

static void spi_master_data_cb(spi_driver_t *spd) {

  (void)spd;
#if defined(SPI_SW_CS)  
  gpio_set_pin(SPI1_NSS); 
#endif  
  /* Set spi1_completed to indicate SPI1 data transferring is completed.*/
  spi1_completed = 1;
}

/* This callback is not used, it is here as an example, it can be replaced
   by a NULL in the structure.*/
static void spi_master_err_cb(spi_driver_t *spd) {

  (void)spd;
}

/*===========================================================================*/
/* SPI baud rate calculation                                                 */
/*===========================================================================*/
/* CLOCK_SPI1CLK = CLOCK_PCLK2 = CLOCK_SYSCLKDIV / 2 =                       */
/* = [CLOCK_SYSCLK / (CLOCK_CFG_SYSPRE_VALUE + 1)] / 2 =                     */
/* = [300MHz / (0 + 1)] / 2 = 150MHz                                         */
/*                                                                           */
/* SPI_CR1_BR = DIV32 -> Baud Rate = CLOCK_SPI1CLK / 32 = 150MHz / 32 = 4.6875MHz  */
/*===========================================================================*/

/*===========================================================================*/
/* Driver local variables and types.                                         */
/*===========================================================================*/

/**
 * @brief   Transmit buffer
 */
static uint16_t txbuf;
/**
 * @brief   Receive buffer
 */
static uint16_t rxbuf;
/**
 * @brief   Diagnostic Errors structure
 */
static L9907_Errors L9907_Diag_Errors = {0U,0U};

/**
 * @brief   Scheduler private variables
 */
typedef enum {SCH_TEST_IDLE,SCH_TEST_FAULT,SCH_TEST_RUN} state_t;
uint32_t L9907_errorsNow = 0U;
uint32_t L9907_errorsOccurred = 0U;
volatile bool faultL9907 = (bool)false;
volatile state_t sch_state = SCH_TEST_IDLE;
volatile uint8_t sch_checkerror = 0U;

/*===========================================================================*/
/* Driver local functions.                                                   */
/*===========================================================================*/

/**
 * @brief   Initializes the @p L9907Driver structure, and turns it on.
 *
 * @notapi
 */
int L9907_init(void) {

    L9907_regBody write_configWord =
    {
        {L9907_CMD0_CONFIGURATION}, /* configuration selected from L9907 Component */
        {L9907_CMD1_CONFIGURATION}, /* configuration selected from L9907 Component */
        {L9907_CMD2_CONFIGURATION}, /* configuration selected from L9907 Component */
        {L9907_CMD3_CONFIGURATION}, /* configuration selected from L9907 Component */
        {L9907_CMD4_CONFIGURATION}, /* configuration selected from L9907 Component */
        {0xC000}, /* standard configuration */
        {0xE000} /* standard configuration */
    };
    
    spi_dma_conf_t spi_master_dma_conf;
    
    /* Configure SPI Master DMA.*/
    spi_master_dma_conf.dma_stream_tx_id = DMA1_STREAM0_ID;
    spi_master_dma_conf.dma_stream_rx_id = DMA1_STREAM1_ID;
    spi_master_dma_conf.dma_stream_bus_prio = DMA_PRIORITY_MAX;
    spi_master_dma_conf.dma_stream_irq_prio = IRQ_PRIORITY_6;

    /* Initialize SPI Master driver instance.*/
    (void)spi_init(&DRV_SPI1);

    /* Configure SPI Master driver instance.*/
    (void)spi_set_drv_mode(&DRV_SPI1, SPI_DRV_MODE_DMA_ASYNC);
    (void)spi_set_dma_conf(&DRV_SPI1, &spi_master_dma_conf);
    (void)spi_set_mode(&DRV_SPI1, SPI_MODE_MASTER);
    (void)spi_set_baud(&DRV_SPI1, SPI_BAUDRATE_DIV32);
    (void)spi_set_cpha(&DRV_SPI1, SPI_CPHA_1);
    (void)spi_set_cpol(&DRV_SPI1, SPI_CPOL_0);
    (void)spi_set_fformat(&DRV_SPI1, SPI_FRAMEFORMAT_MSB);
    (void)spi_set_fsize(&DRV_SPI1, SPI_FRAMESIZE_16);
    (void)spi_set_cb(&DRV_SPI1, SPI_CB_OPS_DONE, spi_master_data_cb);
    (void)spi_set_cb(&DRV_SPI1, SPI_CB_OPS_ERR, spi_master_err_cb);

    /* Apply SPI Master driver instance configuration.*/
    spi_start(&DRV_SPI1);
    
    gpio_clear_pin(E1_GPIO);
    gpio_clear_pin(E2_GPIO);

    /* Initializes the L9907Driver structure, and turns it on */
    L9907_writeRegister(CMD0, &write_configWord);
    L9907_writeRegister(CMD1, &write_configWord);
    L9907_writeRegister(CMD2, &write_configWord);
    L9907_writeRegister(CMD3, &write_configWord);
    L9907_writeRegister(CMD4, &write_configWord);

    /* Read Diag Registers */
    L9907_readRegister(DIAG, &write_configWord);
    L9907_readRegister(DIAG2, &write_configWord);

    /* ENABLE - The gate drivers of the device (L9907) are enabled */
    gpio_set_pin(E1_GPIO);
    gpio_set_pin(E2_GPIO);

    /* Initialize the Diagnostic errors structure */
    L9907_Diag_Errors.wL9907_ErrorsNow = 0U;
    L9907_Diag_Errors.wL9907_ErrorsOccurred = 0U;

    return 0;
}

/**
 * @brief   Stop the Driver.
 *
 * @notapi
 */
void L9907_stop(void) {

    /* Stop the SPI peripheral DSPIx */
    spi_stop(&DRV_SPI1);
    spi_deinit(&DRV_SPI1);

    /* ENABLE - The gate drivers of the device (L9907) are enabled */
    gpio_clear_pin(E1_GPIO);
    gpio_clear_pin(E2_GPIO);
}

/**
 * @brief   Set on the Enabler pins.
 *
 * @param[in] L9907_enabler      Enabler ID
 * @return                       Results of the operation
 *
 * @notapi
 */
int setEnabler(L9907_enabler E){
    /* Set PROFILE_3 the enablers */
    if(E == EN1)
        gpio_set_pin(E1_GPIO);
    else if(E == EN2)
        gpio_set_pin(E2_GPIO);
    else
        return 0;

    return 1;
}

/**
 * @brief   Set off the Enabler pins.
 *
 * @param[in] L9907_enabler      Enabler ID
 * @return                       Results of the operation
 *
 * @notapi
 */
int clearEnabler(L9907_enabler E){
    /* Set PROFILE_3 the enablers */
    if(E == EN1)
        gpio_clear_pin(E1_GPIO);
    else if(E == EN2)
        gpio_clear_pin(E2_GPIO);
    else
        return 0;

    return 1;
}

/**
 * @brief   Writes 16 bit in the L9907 Registers using the SPI Driver.
 *
 * @param[in] regID      Register Address
 * @param[in] body       Pointer to the @p L9907Driver structure
 * @return               State of the SPI communication
 *
 * @notapi
 */
L9907_spiState L9907_writeRegister(L9907_regID regID, L9907_regBody *body){

    L9907_spiState state;
    int writable = 0;
    uint16_t odd = 0;
    int i = 0;

    state = SPI_L9907_GENERAL_ERROR;

    if(regID > 6) {
        state = SPI_L9907_ERROR_NO_SUCH_REGISTER;
        return state;
    }

    switch(regID){

    case(CMD0): {
        body->CMD0.B.ADD = L9907_CMD0_ADDRESS;
        body->CMD0.B.WE = 1;
        txbuf = body->CMD0.R;
    } break;

    case(CMD1): {
        /* WARNING: the content of this register concerns diagnostic settings.
                    Disabling drivers before writing (clear one enabler)
                    Check if the register is available for writing */
        writable = (int)(gpio_read_pin(E1_GPIO) * gpio_read_pin(E2_GPIO));
        if(writable) {
            state = SPI_L9907_NO_WRITABLE_REGISTER;
            return state;
        }

        body->CMD1.B.ADD = L9907_CMD1_ADDRESS;
        body->CMD1.B.WE = 1;
        txbuf = body->CMD1.R;

    } break;

    case(CMD2): {
        body->CMD2.B.ADD = L9907_CMD2_ADDRESS;
        body->CMD2.B.WE = 1;
        txbuf = body->CMD2.R;
    } break;

    case(CMD3): {
        body->CMD3.B.ADD = L9907_CMD3_ADDRESS;
        body->CMD3.B.WE = 1;
        txbuf = body->CMD3.R;
    } break;

    case(CMD4): {
        /* WARNING: the content of this register concerns fault effects enabling.
                    Disabling drivers before writing (clear an enabler)
                    Check if the register is available for writing */
        writable = (int)(gpio_read_pin(E1_GPIO) * gpio_read_pin(E2_GPIO));
        if(writable) {
            state = SPI_L9907_NO_WRITABLE_REGISTER;
            return state;
        }

        body->CMD4.B.ADD = L9907_CMD4_ADDRESS;
        body->CMD4.B.WE = 1;
        txbuf = body->CMD4.R;
    } break;

    default: {
        state = SPI_L9907_NO_WRITABLE_REGISTER;
        return state;
    }
    }

    /* check parity */
    txbuf = txbuf & (0xF7FF);
    odd = txbuf & (0x0001);
    for (i=0; i<15; i++){
        odd = ( (txbuf & 0x0001<<(i+1))>>(i+1) ) ^ odd;
    }
    /* par = !par; */
    txbuf = txbuf | ((!odd)<<11);
    /* Clear read buffer */
    rxbuf = 0;
    /* Initialize SPI1 TX buffer and SPI2 RX buffer.*/
    spi_tx_buffer[0] = (uint8_t)txbuf;
    spi_tx_buffer[1] = (uint8_t) (txbuf>>8);
#if defined(SPI_SW_CS)
    gpio_clear_pin(SPI1_NSS);
#endif    
    /* Send data via SPI1.*/
    spi_exchange(&DRV_SPI1, spi_tx_buffer, spi_rx_buffer,
                      SPI_BUFFER_SIZE);
    /* Wait till SPI2 data receiving is completed.*/
    do {
      ;
    } while(spi1_completed == 0);
    spi1_completed = 0;
#if defined(SPI_SW_CS)
    gpio_clear_pin(SPI1_NSS);
#endif    
    /* Wait till SPI2 data receiving is completed.*/
    spi_exchange(&DRV_SPI1, spi_tx_buffer, spi_rx_buffer,
                 SPI_BUFFER_SIZE);
    do {
      ;
    } while(spi1_completed == 0);
    spi1_completed = 0;

    rxbuf = spi_rx_buffer[1] << 8 | spi_rx_buffer[0];
    if(rxbuf != 0xB001)
        state = SPI_L9907_SUCCESS;

    return state;
}

/**
 * @brief   Reads 16 bit in the L9907 Registers using the SPI Driver.
 *
 * @param[in] regID      Register Address
 * @param[in] body       Pointer to the @p L9907Driver structure
 * @return               State of the SPI communication
 *
 * @notapi
 */
L9907_spiState L9907_readRegister(L9907_regID regID, L9907_regBody *body){

    L9907_spiState state = SPI_L9907_GENERAL_ERROR;
    uint16_t odd = 0;
    int i = 0;

    if(regID > 6) {
        state = SPI_L9907_ERROR_NO_SUCH_REGISTER;
        return state;
    }

    switch (regID) {
    case(CMD0): {
        body->CMD0.B.ADD = L9907_CMD0_ADDRESS;
        body->CMD0.B.WE = 0;
        txbuf = body->CMD0.R;
    } break;
    case(CMD1): {
        body->CMD1.B.ADD = L9907_CMD1_ADDRESS;
        body->CMD1.B.WE = 0;
        txbuf = body->CMD1.R;
    } break;
    case(CMD2): {
        body->CMD2.B.ADD = L9907_CMD2_ADDRESS;
        body->CMD2.B.WE = 0;
        txbuf = body->CMD2.R;
    } break;
    case(CMD3): {
        body->CMD3.B.ADD = L9907_CMD3_ADDRESS;
        body->CMD3.B.WE = 0;
        txbuf = body->CMD3.R;
    } break;
    case(CMD4): {
        body->CMD4.B.ADD = L9907_CMD4_ADDRESS;
        body->CMD4.B.WE = 0;
        txbuf = body->CMD4.R;
    } break;
    case(DIAG): {
        body->DIAG.B.ADD = L9907_DIAG_ADDRESS;
        txbuf = body->DIAG.R;
    } break;
    case(DIAG2): {
        body->DIAG2.B.ADD = L9907_DIAG2_ADDRESS;
        txbuf = body->DIAG2.R;
    } break;
    }

    /* check parity */
    txbuf = txbuf & (0xF7FF);
    odd = txbuf & (0x0001);
    for (i=0; i<15; i++){
        odd = ( (txbuf & 0x0001<<(i+1))>>(i+1) ) ^ odd;
    }

    /* par = !par; */
    txbuf = txbuf | ((!odd)<<11);

    /* Clear read buffer */
    rxbuf = 0;

    spi_rx_buffer[0] = 0;
    spi_rx_buffer[1] = 0;
    spi_tx_buffer[0] = (uint8_t)txbuf;
    spi_tx_buffer[1] = (uint8_t) (txbuf>>8);
    
#if defined(SPI_SW_CS)    
    gpio_clear_pin(SPI1_NSS);
#endif    
    spi_exchange(&DRV_SPI1, spi_tx_buffer, spi_rx_buffer,
                 SPI_BUFFER_SIZE);
    /* Wait till SPI1 data receiving is completed.*/
    do {
      ;
    } while(spi1_completed == 0);
    spi1_completed = 0;
#if defined(SPI_SW_CS)    
    gpio_clear_pin(SPI1_NSS);
#endif    
    /* Send data via SPI1.*/
    spi_exchange(&DRV_SPI1, spi_tx_buffer, spi_rx_buffer,
                 SPI_BUFFER_SIZE);
    /* Wait till SPI1 data receiving is completed.*/
    do {
      ;
    } while(spi1_completed == 0);
    spi1_completed = 0;

    rxbuf = spi_rx_buffer[1] << 8 | spi_rx_buffer[0];
    if(rxbuf == 0xB001)  {
        state = SPI_L9907_GENERAL_ERROR;
        return state;
    }
    else
        state = SPI_L9907_SUCCESS;

    switch (regID) {

    case(CMD0): {
        body->CMD0.R = rxbuf;
    } break;
    case(CMD1): {
        body->CMD1.R = rxbuf;
    } break;
    case(CMD2): {
        body->CMD2.R = rxbuf;
    } break;
    case(CMD3): {
        body->CMD3.R = rxbuf;
    } break;
    case(CMD4): {
        body->CMD4.R = rxbuf;
    } break;
    case(DIAG): {
        body->DIAG.R = rxbuf;
    } break;
    case(DIAG2): {
        body->DIAG2.R = rxbuf;
    } break;

    }

    return state;
}

/**
 * @brief   Sets the output current gain of the FET drivers, choosing among four possibilities.
 *
 * @param[in] gain    Select the desired profile
 * @param[in] ground  Select the current reference: 1 means internal reference that is ground
 * @return            State of the SPI communication
 *
 * @notapi
 */
L9907_spiState L9907_setCurrentGain(L9907_profile gain, bool ground){

    L9907_regBody modeReg;      /* CMD2 is the test mode selections register */
    L9907_regBody driverReg;    /* CMD0 is the driver settings register */
    L9907_spiState state = SPI_L9907_GENERAL_ERROR;

    if(gain > 3) {
        state = SPI_L9907_INVALID_SETTING;
        return state;
    }

    /* Sets the current internal/external reference */
    if (L9907_readRegister(CMD2, &modeReg) != SPI_L9907_SUCCESS)
        return state;

    if(ground)
        modeReg.CMD2.B.GCR = 1;
    else
        modeReg.CMD2.B.GCR = 0;

    if (L9907_writeRegister(CMD2, &modeReg) != SPI_L9907_SUCCESS)
        return state;

    /* Sets the gain level */
    if (L9907_readRegister(CMD0, &driverReg) != SPI_L9907_SUCCESS)
        return state;

    switch(gain){
    case(PROFILE_1): {
        driverReg.CMD0.B.IG = L9907_25_PER_CENT;
    } break;

    case(PROFILE_2): {
        driverReg.CMD0.B.IG = L9907_50_PER_CENT;
    } break;

    case(PROFILE_3): {
        driverReg.CMD0.B.IG = L9907_75_PER_CENT;
    } break;

    case(PROFILE_4): {
        driverReg.CMD0.B.IG = L9907_100_PER_CENT;
    } break;
    }

    state = L9907_writeRegister(CMD0, &driverReg);

    return state;
}

/**
 * @brief   Sets the Dead Time to wait in case of short circuit in a FET driver's phase.
 *
 * @param[in] deadTime    Select the desired profile
 * @return                State of the SPI communication
 *
 * @notapi
 */
L9907_spiState L9907_setDeadTime(L9907_profile deadTime){

    L9907_regBody driverReg;    /* CMD0 is the driver settings register */
    L9907_spiState state = SPI_L9907_GENERAL_ERROR;

    if(deadTime > 3) {
        state = SPI_L9907_INVALID_SETTING;
        return state;
    }


    if(L9907_readRegister(CMD0, &driverReg) != SPI_L9907_SUCCESS)
        return state;

    /* Sets the deadTime level */
    switch(deadTime){
    case(PROFILE_1): {
        driverReg.CMD0.B.DT = L9907_100_200_NS;
    } break;

    case(PROFILE_2): {
        driverReg.CMD0.B.DT = L9907_300_500_NS;
    } break;

    case(PROFILE_3): {
        driverReg.CMD0.B.DT = L9907_700_1000_NS;
    } break;

    case(PROFILE_4): {
        driverReg.CMD0.B.DT = L9907_1000_1500_NS;
    } break;
    }

    state = L9907_writeRegister(CMD0, &driverReg);
    return state;
}

/**
 * @brief   Sets the output current gain Current Sense Amplifiers 1, choosing among four possibilities.
 *
 * @param[in] csa1Gain    Select the desired profile
 * @param[in] ground      Select the reference: 1 means ground
 * @return                State of the SPI communication
 *
 * @notapi
 */
L9907_spiState L9907_setCSA1Gain(L9907_profile csa1Gain, bool ground){

    L9907_regBody driverReg;    /* CMD0 is the driver settings register */
    L9907_spiState state = SPI_L9907_GENERAL_ERROR;

    if(csa1Gain > 3){
        state = SPI_L9907_INVALID_SETTING;
        return state;
    }

    if(L9907_readRegister(CMD0, &driverReg) != SPI_L9907_SUCCESS)
        return state;

    /* Sets the offset */
    if(ground)
        driverReg.CMD0.B.OF1 = 0;
    else
        driverReg.CMD0.B.OF1 = 1;

    /* Sets the csa1Gain level */
    switch(csa1Gain){
    case(PROFILE_1): {
        driverReg.CMD0.B.G1 = L9907_10;
    } break;

    case(PROFILE_2): {
        driverReg.CMD0.B.G1 = L9907_30;
    } break;

    case(PROFILE_3): {
        driverReg.CMD0.B.G1 = L9907_50;
    } break;

    case(PROFILE_4): {
        driverReg.CMD0.B.G1 = L9907_100;
    } break;
    }

    state = L9907_writeRegister(CMD0, &driverReg);;
    return state;
}

/**
 * @brief   Sets the output current gain Current Sense Amplifiers 2, choosing among four possibilities.
 *
 * @param[in] csa2Gain    Select the desired profile
 * @param[in] ground      Select the reference: 1 means ground
 * @return                State of the SPI communication
 *
 * @notapi
 */
L9907_spiState L9907_setCSA2Gain(L9907_profile csa2Gain, bool ground){

    L9907_regBody driverReg;    /* CMD0 is the driver settings register */
    L9907_spiState state = SPI_L9907_GENERAL_ERROR;

    if(csa2Gain > 3) {
        state = SPI_L9907_INVALID_SETTING;
        return state;
    }

    /* Sets the current internal/external reference */
    if(L9907_readRegister(CMD0, &driverReg) != SPI_L9907_SUCCESS)
        return state;

    if(ground)
        driverReg.CMD0.B.OF2 = 0;
    else
        driverReg.CMD0.B.OF2 = 1;

    /* Sets the csa1Gain level */
    switch(csa2Gain){
    case(PROFILE_1): {
        driverReg.CMD0.B.G2 = L9907_10;
    } break;

    case(PROFILE_2): {
        driverReg.CMD0.B.G2 = L9907_30;
    } break;

    case(PROFILE_3): {
        driverReg.CMD0.B.G2 = L9907_50;
    } break;

    case(PROFILE_4): {
        driverReg.CMD0.B.G2 = L9907_100;
    } break;
    }

    state = L9907_writeRegister(CMD0, &driverReg);
    return state;
}

/**
 * @brief   Sets the Threshold for low side FET short circuit, choosing among four possibilities.
 *
 * @param[in] lowSideScTh    Select the desired profile
 * @return                   State of the SPI communication
 *
 * @notapi
 */
L9907_spiState L9907_setLowSideTh(L9907_profile lowSideScTh){

    L9907_spiState state = SPI_L9907_GENERAL_ERROR;
    L9907_regBody diagnosticReg;    /* CMD1 is the diagnostic settings register */
    int powerStatus = gpio_read_pin(E1_GPIO) * gpio_read_pin(E2_GPIO);

    if(lowSideScTh > 3) {
        state = SPI_L9907_INVALID_SETTING;
        return state;
    }

    if(L9907_readRegister(CMD1, &diagnosticReg) != SPI_L9907_SUCCESS)
        return state;

    clearEnabler(EN1);  /* Makes CMD1 modifiable */
    clearEnabler(EN2);

    /* Sets the csa1Gain level */
    switch(lowSideScTh){
    case(PROFILE_1): {
        diagnosticReg.CMD1.B.SCLS1 = L9907_10;
    } break;

    case(PROFILE_2): {
        diagnosticReg.CMD1.B.SCLS1 = L9907_30;
    } break;

    case(PROFILE_3): {
        diagnosticReg.CMD1.B.SCLS1 = L9907_50;
    } break;

    case(PROFILE_4): {
        diagnosticReg.CMD1.B.SCLS1 = L9907_100;
    } break;
    }

    state = L9907_writeRegister(CMD1, &diagnosticReg);

    if(powerStatus) {
        setEnabler(EN1);    /* Turn on the drivers */
        setEnabler(EN2);
    }

    return state;
}

/**
 * @brief   Sets the Threshold for High side FET short circuit, choosing among four possibilities.
 *
 * @param[in] highSideScTh    Select the desired profile
 * @return                    State of the SPI communication
 *
 * @notapi
 */
L9907_spiState L9907_setHighSideTh(L9907_profile highSideScTh){

    L9907_spiState state = SPI_L9907_GENERAL_ERROR;
    L9907_regBody diagnosticReg;    /* CMD1 is the diagnostic settings register */
    int powerStatus = gpio_read_pin(E1_GPIO) * gpio_read_pin(E2_GPIO);

    if(highSideScTh > 3) {
        state = SPI_L9907_INVALID_SETTING;
        return state;
    }

    if(L9907_readRegister(CMD1, &diagnosticReg) != SPI_L9907_SUCCESS)
        return state;

    clearEnabler(EN1);  /* Makes CMD1 modifiable */
    clearEnabler(EN2);

    /* Sets the csa1Gain level */
    switch(highSideScTh){
    case(PROFILE_1): {
        diagnosticReg.CMD1.B.SCHS1 = L9907_10;
    } break;

    case(PROFILE_2): {
        diagnosticReg.CMD1.B.SCHS1 = L9907_30;
    } break;

    case(PROFILE_3): {
        diagnosticReg.CMD1.B.SCHS1 = L9907_50;
    } break;

    case(PROFILE_4): {
        diagnosticReg.CMD1.B.SCHS1 = L9907_100;
    } break;
    }

    state = L9907_writeRegister(CMD1, &diagnosticReg);

    if(powerStatus) {
        setEnabler(EN1);    /* Turn on the drivers */
        setEnabler(EN2);
    }

    return state;
}

/**
 * @brief   Sets the Threshold for battery Over Voltage, choosing among two possibilities.
 *
 * @param[in] batOverVoltTh    Select the desired profile
 * @return                     State of the SPI communication
 *
 * @notapi
 */
L9907_spiState L9907_setOverVbTh(L9907_profile batOverVoltTh){

    L9907_spiState state = SPI_L9907_GENERAL_ERROR;
    L9907_regBody diagnosticReg;    /* CMD1 is the diagnostic settings register */
    int powerStatus = gpio_read_pin(E1_GPIO) * gpio_read_pin(E2_GPIO);

    if(batOverVoltTh > 3) {
        state = SPI_L9907_INVALID_SETTING;
        return state;
    }

    /* Sets the current internal/external reference */
    if(L9907_readRegister(CMD1, &diagnosticReg) != SPI_L9907_SUCCESS)
        return state;

    clearEnabler(EN1);  /* Makes CMD1 modifiable */
    clearEnabler(EN2);

    /* Sets the csa1Gain level */
    switch(batOverVoltTh){
    case(PROFILE_1): {
        diagnosticReg.CMD1.B.VBOV = L9907_18_25_V;
    } break;

    case(PROFILE_2): {
        diagnosticReg.CMD1.B.VBOV = L9907_35_42_V;
    } break;

    case(PROFILE_3): {
        state = SPI_L9907_INVALID_SETTING;
    } break;

    case(PROFILE_4): {
        state = SPI_L9907_INVALID_SETTING;;
    } break;
    }

    state = L9907_writeRegister(CMD1, &diagnosticReg);

    if(powerStatus) {
        setEnabler(EN1);    /* Turn on the drivers */
        setEnabler(EN2);
    }

    return state;
}

/**
 * @brief   Sets the Threshold for Vcc Over Voltage, choosing among two possibilities.
 *
 * @param[in] overVccTh    Select the desired profile
 * @return                 State of the SPI communication
 *
 * @notapi
 */
L9907_spiState L9907_setOverVccTh(L9907_profile overVccTh){

    L9907_spiState state = SPI_L9907_GENERAL_ERROR;
    L9907_regBody diagnosticReg;    /* CMD1 is the diagnostic settings register */
    int powerStatus = gpio_read_pin(E1_GPIO) * gpio_read_pin(E2_GPIO);

    if(overVccTh > 3) {
            state = SPI_L9907_INVALID_SETTING;
            return state;
    }

    /* Sets the current internal/external reference */
    if(L9907_readRegister(CMD1, &diagnosticReg) != SPI_L9907_SUCCESS)
        return state;

    clearEnabler(EN1);  /* Makes CMD1 modifiable */
    clearEnabler(EN2);

    /* Sets the csa1Gain level */
    switch(overVccTh){
    case(PROFILE_1): {
        diagnosticReg.CMD1.B.SCLS1 = L9907_3300_MV;
    } break;

    case(PROFILE_2): {
        diagnosticReg.CMD1.B.SCLS1 = L9907_5000_MV;
    } break;

    case(PROFILE_3): {
        state = SPI_L9907_INVALID_SETTING;
    } break;

    case(PROFILE_4): {
        state = SPI_L9907_INVALID_SETTING;;
    } break;
    }

    state = L9907_writeRegister(CMD1, &diagnosticReg);

    if(powerStatus) {
        setEnabler(EN1);    /* Turn on the drivers */
        setEnabler(EN2);
    }

    return state;
}

/**
 * @brief   Switches ON/OFF the selected fault effect.
 *
 * @param[in] faultEffect   Select the fault effect to manage
 * @param[in] faultState    Select ON or OFF
 * @return                  State of the SPI communication
 *
 * @notapi
 */
L9907_functionalityState L9907_faultEffectManager(L9907_faultEffect faultEffect, L9907_functionalityState faultState){

    L9907_regBody faultEffectReg;   /* CMD4 is the fault effect enabler/disabler register */
    L9907_regBody diagnosticReg;    /* CMD1 is the fault effect enabler/disabler register */
    int powerStatus = gpio_read_pin(E1_GPIO) * gpio_read_pin(E2_GPIO);

    if(faultEffect > 9) {
        faultState = UNREACHABLE;
        return faultState;
    }

    if(faultEffect < 3) {

        if(L9907_readRegister(CMD1, &diagnosticReg) != SPI_L9907_SUCCESS || faultState > 1){
            faultState = UNREACHABLE;
            return faultState;
        }

        clearEnabler(EN1);  /* Makes CMD4 modifiable */
        clearEnabler(EN2);  /* Makes CMD4 modifiable */

        if(faultState == ON) {
            switch(faultEffect){
            case(THSD): {
                    diagnosticReg.CMD1.B.E_THSD = 1;
            } break;
            case(VBOV): {
                    diagnosticReg.CMD1.B.E_VBOV = 1;
            } break;
            case(VBUV): {
                    diagnosticReg.CMD1.B.E_VBUV = 1;
            } break;
            default: {
                faultState = UNREACHABLE;
                return faultState;
            }
            }

        } else {
            switch(faultEffect){
            case(THSD): {
                    diagnosticReg.CMD1.B.E_THSD = 0;
            } break;
            case(VBOV): {
                    diagnosticReg.CMD1.B.E_VBOV = 0;
            } break;
            case(VBUV): {
                    diagnosticReg.CMD1.B.E_VBUV = 0;
            } break;
            default: {
                faultState = UNREACHABLE;
                return faultState;
            }
            }

            if(L9907_writeRegister(CMD1, &diagnosticReg) != SPI_L9907_SUCCESS){
                faultState = UNREACHABLE;
                return faultState;
            }

        }

    } else {

        if(L9907_readRegister(CMD4, &faultEffectReg) != SPI_L9907_SUCCESS || faultState > 1){
            faultState = UNREACHABLE;
            return faultState;
        }

        clearEnabler(EN1);  /* Makes CMD4 modifiable */
        clearEnabler(EN2);  /* Makes CMD4 modifiable */

        if(faultState == ON) {

            switch(faultEffect) {
            case(VCCOV): {
                faultEffectReg.CMD4.B.EN_VCCOV = 1;
            } break;
            case(VCCUV): {
                faultEffectReg.CMD4.B.EN_VCCUV = 1;
            } break;
            case(UV_HS): {
                faultEffectReg.CMD4.B.EN_UV_HS= 1;
            } break;
            case(UV_LS): {
                faultEffectReg.CMD4.B.EN_UV_LS = 1;
            } break;
            case(VSCHS1): {
                faultEffectReg.CMD4.B.EN_VSCH1 = 1;
            } break;
            case(VSCHS2): {
                faultEffectReg.CMD4.B.EN_VSCH2 = 1;
            } break;
            case(VSCHS3): {
                faultEffectReg.CMD4.B.EN_VSCH3 = 1;
            } break;
            case(VSCLS1): {
                faultEffectReg.CMD4.B.EN_VSCL1 = 1;
            } break;
            case(VSCLS2): {
                faultEffectReg.CMD4.B.EN_VSCL2 = 1;
            } break;
            case(VSCLS3): {
                faultEffectReg.CMD4.B.EN_VSCL3 = 1;
            } break;
            default: {
                faultState = UNREACHABLE;
                return faultState;
            }
            }

        } else {

            switch(faultEffect) {
            case(VCCOV): {
                faultEffectReg.CMD4.B.EN_VCCOV = 0;
            } break;
            case(VCCUV): {
                faultEffectReg.CMD4.B.EN_VCCUV = 0;
            } break;
            case(UV_HS): {
                faultEffectReg.CMD4.B.EN_UV_HS = 0;
            } break;
            case(UV_LS): {
                faultEffectReg.CMD4.B.EN_UV_LS = 0;
            } break;
            case(VSCHS1): {
                faultEffectReg.CMD4.B.EN_VSCH1 = 0;
            } break;
            case(VSCHS2): {
                faultEffectReg.CMD4.B.EN_VSCH2 = 0;
            } break;
            case(VSCHS3): {
                faultEffectReg.CMD4.B.EN_VSCH3 = 0;
            } break;
            case(VSCLS1): {
                faultEffectReg.CMD4.B.EN_VSCL1 = 0;
            } break;
            case(VSCLS2): {
                faultEffectReg.CMD4.B.EN_VSCL2 = 0;
            } break;
            case(VSCLS3): {
                faultEffectReg.CMD4.B.EN_VSCL3 = 0;
            } break;
            default: {
                faultState = UNREACHABLE;
                return faultState;
            }
            }

            if(L9907_writeRegister(CMD4, &faultEffectReg) != SPI_L9907_SUCCESS){
                faultState = UNREACHABLE;
                return faultState;
            }

        }
    }

    if(powerStatus) {
        setEnabler(EN1);    /* Turn on the drivers */
        setEnabler(EN2);
    }

    return faultState;
}

/**
 * @brief   Switches ON/OFF the boost disabler functionality.
 *
 * @param[in] boostState     Select ON or OFF
 * @return                   State of the SPI communication
 *
 * @notapi
 */
L9907_functionalityState L9907_boostDisabler(L9907_functionalityState boostState){

    L9907_regBody modeReg;  /* CMD2 is the test mode selections register */
    int powerStatus = gpio_read_pin(E1_GPIO) * gpio_read_pin(E2_GPIO);

    if(L9907_readRegister(CMD2, &modeReg) != SPI_L9907_SUCCESS || boostState > 1){
        boostState = UNREACHABLE;
        return boostState;
    }

    clearEnabler(EN1);  /* Makes CMD2 bit 9 editable */
    clearEnabler(EN2);

    /* Sets the Boost State */
    if(boostState == ON)
        modeReg.CMD2.B.E_BST = 1;
    else
        modeReg.CMD2.B.E_BST = 0;

    if(L9907_writeRegister(CMD2, &modeReg) != SPI_L9907_SUCCESS){
        boostState = UNREACHABLE;
        return boostState;
    }

    if(powerStatus) {
        setEnabler(EN1);    /* Turn on the drivers */
        setEnabler(EN2);
    }

    return boostState;
}

/**
 * @brief   Switches ON/OFF the shot-through disabling phase functionality.
 *
 * @param[in] shootThroughState     Select ON or OFF
 * @return                          State of the SPI communication
 *
 * @notapi
 */
L9907_functionalityState L9907_shootThroughPhase(L9907_functionalityState shootThroughState){

    L9907_regBody modeReg;  /* CMD2 is the test mode selections register */
    int powerStatus = gpio_read_pin(E1_GPIO) * gpio_read_pin(E2_GPIO);

    if(L9907_readRegister(CMD2, &modeReg) != SPI_L9907_SUCCESS || shootThroughState > 1){
        shootThroughState = UNREACHABLE;
        return shootThroughState;
    }

    clearEnabler(EN1);  /* Makes CMD2 bit 9 editable */
    clearEnabler(EN2);

    /* Sets the Boost State */
    if(shootThroughState == ON)
        modeReg.CMD2.B.SHT_PH = 1;
    else
        modeReg.CMD2.B.SHT_PH = 0;

    if(L9907_writeRegister(CMD2, &modeReg) != SPI_L9907_SUCCESS){
        shootThroughState = UNREACHABLE;
        return shootThroughState;
    }

    if(powerStatus) {
        setEnabler(EN1);    /* Turn on the drivers */
        setEnabler(EN2);
    }

    return shootThroughState;
}

/**
 * @brief   Switches ON/OFF the short circuit test detection functionality.
 *
 * @param[in] testShortState     Select ON or OFF
 * @return                       State of the SPI communication
 *
 * @notapi
 */
L9907_functionalityState L9907_testShort(L9907_functionalityState testShortState){

    L9907_regBody modeReg;  /* CMD2 is the test mode selections register */

    if(L9907_readRegister(CMD2, &modeReg) != SPI_L9907_SUCCESS || testShortState > 1){
        testShortState = UNREACHABLE;
        return testShortState;
    }

    /* Sets the Boost State */
    if(testShortState == ON)
        modeReg.CMD2.B.VSCTST = 1;
    else
        modeReg.CMD2.B.VSCTST = 0;

    if(L9907_writeRegister(CMD2, &modeReg) != SPI_L9907_SUCCESS){
        testShortState = UNREACHABLE;
        return testShortState;
    }

    return testShortState;
}

/**
 * @brief   Switches ON/OFF the Vcc over voltage test detection functionality.
 *
 * @param[in] overVccState     Select ON or OFF
 * @return                     State of the SPI communication
 *
 * @notapi
 */
L9907_functionalityState L9907_testOverVcc(L9907_functionalityState overVccState){

    L9907_regBody modeReg;  /* CMD2 is the test mode selections register */

    if(L9907_readRegister(CMD2, &modeReg) != SPI_L9907_SUCCESS || overVccState > 1){
        overVccState = UNREACHABLE;
        return overVccState;
    }

    /* Sets the Boost State */
    if(overVccState == ON)
        modeReg.CMD2.B.VOVTST = 1;
    else
        modeReg.CMD2.B.VOVTST = 0;

    if(L9907_writeRegister(CMD2, &modeReg) != SPI_L9907_SUCCESS){
        overVccState = UNREACHABLE;
        return overVccState;
    }

    return overVccState;
}

/**
 * @brief   Switches ON/OFF the short phase disabling functionality.
 *
 * @param[in] shortPhaseState     Select ON or OFF
 * @return                        State of the SPI communication
 *
 * @notapi
 */
L9907_functionalityState L9907_shortPhase(L9907_functionalityState shortPhaseState){

    L9907_regBody modeReg;  /* CMD2 is the test mode selections register */

    if(L9907_readRegister(CMD2, &modeReg) != SPI_L9907_SUCCESS || shortPhaseState > 1){
        shortPhaseState = UNREACHABLE;
        return shortPhaseState;
    }

    /* Sets the Boost State */
    if(shortPhaseState == ON)
        modeReg.CMD2.B.SHFET = 1;
    else
        modeReg.CMD2.B.SHFET = 0;

    if(L9907_writeRegister(CMD2, &modeReg) != SPI_L9907_SUCCESS){
        shortPhaseState = UNREACHABLE;
        return shortPhaseState;
    }

    return shortPhaseState;
}

/**
 * @brief   Switches ON/OFF the booster disabling functionality in case of over voltage.
 *
 * @param[in] boostOverVoltState     Select ON or OFF
 * @return                           State of the SPI communication
 *
 * @notapi
 */
L9907_functionalityState L9907_boostOverVolt(L9907_functionalityState boostOverVoltState){

    L9907_regBody csaReg;   /* CMD3 is the CSA offset calibration register */
    int powerStatus = gpio_read_pin(E1_GPIO) * gpio_read_pin(E2_GPIO);

    if(L9907_readRegister(CMD3, &csaReg) != SPI_L9907_SUCCESS || boostOverVoltState > 1){
        boostOverVoltState = UNREACHABLE;
        return boostOverVoltState;
    }

    clearEnabler(EN1);  /* Makes CMD3 bit 12 editable */
    clearEnabler(EN2);
    /* Sets the Boost State */
    if(boostOverVoltState == ON)
        csaReg.CMD3.B.D_BSTOV = 0;
    else
        csaReg.CMD3.B.D_BSTOV = 1;

    if(L9907_writeRegister(CMD3, &csaReg) != SPI_L9907_SUCCESS){
        boostOverVoltState = UNREACHABLE;
        return boostOverVoltState;
    }

    if(powerStatus) {
        setEnabler(EN1);    /* Turn on the drivers */
        setEnabler(EN2);
    }

    return boostOverVoltState;
}

/**
 * @brief   Sets the Offset Calibration of the Current Sense Amplifiers 1.
 *
 * @param[in] Offset    value in the range: -15, +15 [mV]
 * @return              State of the SPI communication
 *
 * @notapi
 */
L9907_spiState L9907_setCSA1OffsetCalibration(int Offset){

    L9907_regBody csaCalibrationReg;    /* CMD3 is the current sense amplifier offset calibration register */
    L9907_spiState state = SPI_L9907_GENERAL_ERROR;

    if(Offset < -15 || Offset > 15) {
        state = SPI_L9907_INVALID_SETTING;
        return state;
    }

    /* Sets the CSA input offset */
    if(L9907_readRegister(CMD3, &csaCalibrationReg) != SPI_L9907_SUCCESS)
        return state;

    if(Offset < 0)
        csaCalibrationReg.CMD3.B.SIGN1 = 0;
    else
        csaCalibrationReg.CMD3.B.SIGN1 = 1;

    csaCalibrationReg.CMD3.B.TRIM1 = (uint32_t)Offset;

    if(L9907_writeRegister(CMD3, &csaCalibrationReg) != SPI_L9907_SUCCESS)
        return state;

    state = SPI_L9907_SUCCESS;
    return state;
}

/**
 * @brief   Sets the Offset Calibration of the Current Sense Amplifiers 2.
 *
 * @param[in] Offset    value in the range: -15, +15 [mV]
 * @return              State of the SPI communication
 *
 * @notapi
 */
L9907_spiState L9907_setCSA2OffsetCalibration(int Offset){

    L9907_regBody csaCalibrationReg;    /* CMD3 is the current sense amplifier offset calibration register */
    L9907_spiState state = SPI_L9907_GENERAL_ERROR;

    if(Offset < -15 || Offset > 15) {
        state = SPI_L9907_INVALID_SETTING;
        return state;
    }

    /* Sets the CSA input offset */
    if(L9907_readRegister(CMD3, &csaCalibrationReg) != SPI_L9907_SUCCESS)
        return state;

    if(Offset < 0)
        csaCalibrationReg.CMD3.B.SIGN2 = 0;
    else
        csaCalibrationReg.CMD3.B.SIGN2 = 1;

    csaCalibrationReg.CMD3.B.TRIM2 = (uint32_t)Offset;

    if(L9907_writeRegister(CMD3, &csaCalibrationReg) != SPI_L9907_SUCCESS)
        return state;

    state = SPI_L9907_SUCCESS;
    return state;
}

/**
 * @brief   Switches ON/OFF the Reg Off Procedure.
 *
 * @param[in] regOffState     Select ON or OFF
 * @return                    The state of the functionality
 *
 * @notapi
 */
L9907_functionalityState L9907_regOffProcedure(L9907_functionalityState regOffState){

    L9907_regBody modeReg;  /* CMD3 is the CSA offset calibration register */
    int powerStatus = gpio_read_pin(E1_GPIO) * gpio_read_pin(E2_GPIO);

    if(L9907_readRegister(CMD2, &modeReg) != SPI_L9907_SUCCESS || regOffState > 1){
        regOffState = UNREACHABLE;
        return regOffState;
    }

    clearEnabler(EN1);  /* Makes CMD3 bit 12 editable */
    clearEnabler(EN2);
    /* Sets the Boost State */
    if(regOffState == ON)
        modeReg.CMD4.B.E_REGOFF = 1;
    else
        modeReg.CMD4.B.E_REGOFF = 1;

    if(L9907_writeRegister(CMD3, &modeReg) != SPI_L9907_SUCCESS){
        regOffState = UNREACHABLE;
        return regOffState;
    }

    if(powerStatus) {
        setEnabler(EN1);    /* Turn on the drivers */
        setEnabler(EN2);
    }

    return regOffState;
}

/**
 * @brief       Returns Diag and Diag2 registers struct pointer
 *
 * @return      Diag registers pointer
 *
 * @notapi
 */
bool L9907_setDiagRegisters(uint8_t set)
{
  if(set != 1u)
    return false;
  sch_checkerror = set;
  return true;
}

/**
 * @brief       Returns Diag and Diag2 registers struct pointer
 *
 * @return      Diag registers pointer
 *
 * @notapi
 */
L9907_Errors* L9907_getDiagRegisters(void)
{
    return &L9907_Diag_Errors;
}

/**
 * @brief   L9907 fault management function.
 * @details This function checks the fs_flag.
 *          In case of fault condition, the DIAG and DIAG2 registers are read.
 *
 * @param[out] reg1     Value of Diag
 * @param[out] reg2     Value of Diag2
 * @return  bool It returns false if an error occurs, otherwise return true.
 */
bool L9907_fault_detection(L9907_Diag* reg1, L9907_Diag2* reg2)
{
    L9907_regBody L9907_reg = {{0U},{0U},{0U},{0U},{0U},{0U},{0U}};
    bool flag1 = (bool)true;
    bool flag2 = (bool)true;
    if (gpio_read_pin(L9907_FS_FLAG) == 0)
    {
        /* read the Diagnostic DIAG register */
        if (L9907_readRegister(DIAG, &L9907_reg) != SPI_L9907_SUCCESS)
        {
            /* Error found reading DIAG2 register */
            flag1 = (bool)false;
        }
        /* read the Diagnostic DIAG2 register */
        if (L9907_readRegister(DIAG2, &L9907_reg) != SPI_L9907_SUCCESS)
        {
            /* Error found reading DIAG2 register */
            flag2 = (bool)false;
        }
        reg1->DIAG.R = L9907_reg.DIAG.R;
        reg2->DIAG2.R = L9907_reg.DIAG2.R;
    }
    else
    {
        /* no faults found => no errors */
        reg1->DIAG.R = 0U;
        reg2->DIAG2.R = 0U;
    }

    return (bool)((flag1)&(flag2));
}

/**
 * @brief      Check errors of L9907 device.
 * @details    This function check errors and return the errors.
 *
 * @param[out] errorNow buffer of returned bitfields containing error flags 
 *             coming from L9907 device that are currently active.\n
 *             The buffer have to be provided from the caller.
 * @param[out] errorOccurred buffer of returned bitfields containing error flags 
 *             coming from L9907 device that are over.\n
 *             The buffer have to be provided from the caller.
 * @return     bool It returns false if an error occurs  reading DIAG registers, otherwise return true.
 */
bool L9907_CheckErrors(uint32_t* errorNow,uint32_t* errorOccurred)
{
  uint32_t errorFromDevice = (uint32_t)0;
  bool retVal = (bool)FALSE;
  if ((errorNow)&&(errorOccurred))
  {
    L9907_Diag Diag_reg;
    L9907_Diag2 Diag2_reg;

    retVal = L9907_fault_detection(&Diag_reg, &Diag2_reg);
    /* DIAG_bit_15 ... DIAG_bit_0 DIAG2_bit_15 ... DIAG2_bit_0*/
    errorFromDevice = (((uint32_t)(Diag_reg.DIAG.R << 16U)) | (Diag2_reg.DIAG2.R));

    L9907_Diag_Errors.wL9907_ErrorsNow &= L9907_ERROR_CODE_FROM_DEVICE_MASK;
    L9907_Diag_Errors.wL9907_ErrorsNow |= errorFromDevice;
    L9907_Diag_Errors.wL9907_ErrorsOccurred |= L9907_Diag_Errors.wL9907_ErrorsNow;
    *errorNow = L9907_Diag_Errors.wL9907_ErrorsNow;
    *errorOccurred = L9907_Diag_Errors.wL9907_ErrorsOccurred;
  }
  return retVal;
}

/**
  * @brief  It returns MC_DIAG_ERROR or MC_NO_ERROR depending on
  *         external driver Diagnostic values
  * @retval uint16_t Fault code error
  */
uint16_t L9907_CheckFaultState(void)
{
  uint16_t hFault;

  if( (L9907_Diag_Errors.wL9907_ErrorsNow & (~0xC000E180) ) != 0 )
  {
    hFault = MC_DIAG_ERROR;
  }
  else
  {
    hFault = MC_NO_ERROR;
  }
  return hFault;
}

/**
  * @brief  Clears the fault state of L9907 device.
  * @retval none.
  */
void L9907_FaultAck(void)
{
  L9907_Diag_Errors.wL9907_ErrorsOccurred = 0U;
}

/**
  * @brief  Schedule function to be called with periodicity.
  * @retval none.
  */ 
void L9907_Schedule(void)
{
  if (sch_checkerror)
  {
    /* Check errors */
    faultL9907 = L9907_CheckErrors(&L9907_errorsNow,&L9907_errorsOccurred);
    if (faultL9907)
    {
      if ((L9907_errorsNow != 0U)||(L9907_errorsOccurred != 0U))
      {
        /* Spi communication OK, DIAG and DIAG2 reports errors */
        sch_state = SCH_TEST_FAULT;
        /* LedOn(); */
      }
    }
    else
    {
      /* SPI communication KO */
      sch_state = SCH_TEST_FAULT;
    }
    /* Clear scheduler variable: one time read */
    sch_checkerror = 0;
  }
}

/*===========================================================================*/
/* IRQ Handlers                                                              */
/*===========================================================================*/

/**
 * @brief   IRQ handler function declaration.
 * @details This function hides the details of the L9907 ISR function's body.
 *
 * @param[in] id        a vector name as defined in @p vectors.s
 */

void L9907_irqfaultRecovery(void) {


    gpio_clear_pin(E1_GPIO);
    gpio_clear_pin(E2_GPIO);

    L9907_faultRecovery();

}
