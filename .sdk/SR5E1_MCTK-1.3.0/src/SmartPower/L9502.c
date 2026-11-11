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
*   @file    L9502.c
*   @version M4_VERSION
*   @brief   This file provides firmware functions that implement the features
*            of the L9502 component of the Stellar Motor Control.
*          
*   @details This file provides firmware functions that implement the features
*            of the L9502 component of the Stellar Motor Control.
 */
/*
 * @addtogroup STELLAR_L9502_Library
 * @{
 */
#include "main.h"
#include "typedefs.h"
#include "tabcrc.h"
#include "spi.h"
#include "dwt.h"
#include "gpio.h"
#include "dma.h"
#include <osal.h>
#include "L9502.h"

/* #define L9502_SPI_FAST_TRANSFER */

uint16_t L9502_DevicesConfiguration(void);
uint16_t L9502_IsDevicesProgrammed(void);
uint16_t L9502_ClearFaults(void);
#if defined(VERSION_CA) || defined(VERSION_CD)
uint16_t L9502_ReadStatusReg(void);
#endif
void L9502_SetDisPin(bool enable);
void L9502_SetBrakePin(bool enable);
uint16_t L9502_CheckErrors(uint32_t* errorNow,uint32_t* errorOccurred, uint8_t rlr);

extern L9502Params_t L9502Config;

/* L9502 Initializaiont ok/SPI_ERROR */ 
uint8_t L9502_init = NO_ERROR;

// ****************************************************************************
// *************************** Driver Configuration ***************************
// ****************************************************************************
/* pre calculated table of all possible CRC values of bits CRC [0:2].  */
/* Array index is 0 - 127 (value of bits [3:9]). */
uint8_t crcTbl[128] = {
            (uint8_t)0x00, (uint8_t)0x03, (uint8_t)0x06, (uint8_t)0x05, (uint8_t)0x07, (uint8_t)0x04, (uint8_t)0x01, (uint8_t)0x02,
            (uint8_t)0x05, (uint8_t)0x06, (uint8_t)0x03, (uint8_t)0x00, (uint8_t)0x02, (uint8_t)0x01, (uint8_t)0x04, (uint8_t)0x07,
            (uint8_t)0x01, (uint8_t)0x02, (uint8_t)0x07, (uint8_t)0x04, (uint8_t)0x06, (uint8_t)0x05, (uint8_t)0x00, (uint8_t)0x03,
            (uint8_t)0x04, (uint8_t)0x07, (uint8_t)0x02, (uint8_t)0x01, (uint8_t)0x03, (uint8_t)0x00, (uint8_t)0x05, (uint8_t)0x06,
            (uint8_t)0x02, (uint8_t)0x01, (uint8_t)0x04, (uint8_t)0x07, (uint8_t)0x05, (uint8_t)0x06, (uint8_t)0x03, (uint8_t)0x00,
            (uint8_t)0x07, (uint8_t)0x04, (uint8_t)0x01, (uint8_t)0x02, (uint8_t)0x00, (uint8_t)0x03, (uint8_t)0x06, (uint8_t)0x05,
            (uint8_t)0x03, (uint8_t)0x00, (uint8_t)0x05, (uint8_t)0x06, (uint8_t)0x04, (uint8_t)0x07, (uint8_t)0x02, (uint8_t)0x01,
            (uint8_t)0x06, (uint8_t)0x05, (uint8_t)0x00, (uint8_t)0x03, (uint8_t)0x01, (uint8_t)0x02, (uint8_t)0x07, (uint8_t)0x04,
            (uint8_t)0x04, (uint8_t)0x07, (uint8_t)0x02, (uint8_t)0x01, (uint8_t)0x03, (uint8_t)0x00, (uint8_t)0x05, (uint8_t)0x06,
            (uint8_t)0x01, (uint8_t)0x02, (uint8_t)0x07, (uint8_t)0x04, (uint8_t)0x06, (uint8_t)0x05, (uint8_t)0x00, (uint8_t)0x03,
            (uint8_t)0x05, (uint8_t)0x06, (uint8_t)0x03, (uint8_t)0x00, (uint8_t)0x02, (uint8_t)0x01, (uint8_t)0x04, (uint8_t)0x07,
            (uint8_t)0x00, (uint8_t)0x03, (uint8_t)0x06, (uint8_t)0x05, (uint8_t)0x07, (uint8_t)0x04, (uint8_t)0x01, (uint8_t)0x02,
            (uint8_t)0x06, (uint8_t)0x05, (uint8_t)0x00, (uint8_t)0x03, (uint8_t)0x01, (uint8_t)0x02, (uint8_t)0x07, (uint8_t)0x04,
            (uint8_t)0x03, (uint8_t)0x00, (uint8_t)0x05, (uint8_t)0x06, (uint8_t)0x04, (uint8_t)0x07, (uint8_t)0x02, (uint8_t)0x01,
            (uint8_t)0x07, (uint8_t)0x04, (uint8_t)0x01, (uint8_t)0x02, (uint8_t)0x00, (uint8_t)0x03, (uint8_t)0x06, (uint8_t)0x05,
            (uint8_t)0x02, (uint8_t)0x01, (uint8_t)0x04, (uint8_t)0x07, (uint8_t)0x05, (uint8_t)0x06, (uint8_t)0x03, (uint8_t)0x00};

// L9502 CRC configuration 
tabcrcConfigSet_t  L9502crc_Config ={
  (24-6),        //frame_lengh (frame length number of active data bits [0..2040])
  5,             //crc_length (length of CRC in bits [1..7])
  0x25,          //polynom (CRC polynom [1..255])
  0x1F,          //seed_value (CRC seed value [0..255])
  0,             //frame_position highest (first) bit position in input stream  [0..7])

  0,
  0,
  0,
  0,
  0,
  {0}
};

/*===========================================================================*/
/* SPI-related.                                                              */
/*===========================================================================*/
#define SPI_BUFFER_SIZE     3

volatile uint8_t spi1_completed = 0;

uint8_t spi_tx_buffer[SPI_BUFFER_SIZE];
uint8_t spi_rx_buffer[SPI_BUFFER_SIZE];

#if 0
static void spi_master_data_cb(spi_driver_t *spd) {

  (void)spd;

  /* Set spi1_completed to indicate SPI1 data transferring is completed.*/
  spi1_completed = 1;
}

/* This callback is not used, it is here as an example, it can be replaced
   by a NULL in the structure.*/
static void spi_master_err_cb(spi_driver_t *spd) {

  (void)spd;
}
#endif

#ifdef L9502_SPI_FAST_TRANSFER
//DSPI0 Configuration for ST SPI in CTAR0 and CTAR 1 for slow mode
//Expected input clock 100MHz
//Baud rate max 5MHz   Real Baud rate 100MHz/5/8 = 2.5MHz
//Delay after transfer              1/(100MHz/5/8)  = 0.4us
const DSPI_Config_2ctrDef L9502_DSPI_Config ={
//->DSPI_Instance_p
    ( &DSPI_0),                     //0 pointer to selected DSPI periphery
// ->Command1
    (CONTINUOUS_CS | CTAR_0 | PCS0),
// ->Command2
    (CONTINUOUS_CS | CTAR_1 | PCS0),
//FirstCTARnumb
    0,
//-> CTAR0 Config  First 8 bit transfers in 24 bits transfer
    ( (uint32_t)DSPI_CTAR_FMSZ(7)                // Frame size -1
    | (uint32_t)DSPI_CTAR_CPOL(0)                 // Clock polarity: (DSPI_CTAR_CPOL) 0 - inactive state low; 1 - inactive state high
    | (uint32_t)DSPI_CTAR_CPHA(0)                 // Clock phase:    (DSPI_CTAR_CPHA) 0 -data captured on leading edge; 1 - data changed on leading edge
    | (uint32_t)DSPI_CTAR_PBR(BR_PRESCALER_5)     // Baud Rate Prescaler. (Master mode only)
    | (uint32_t)DSPI_CTAR_BR(BR_SCALER_8)        // Baud Rate divider      (BR)
    | (uint32_t)DSPI_CTAR_PCSSCK(Px_PRESCALER_5)  // PCS to SCK Delay Prescaler
    | (uint32_t)DSPI_CTAR_PASC(Px_PRESCALER_5)    // After SCK Delay Prescaler
    | (uint32_t)DSPI_CTAR_PDT(Px_PRESCALER_5)     // Delay after transfer prescaler
    | (uint32_t)DSPI_CTAR_DT(D_SCALER_4096)          // Delay after transfer
    | (uint32_t)DSPI_CTAR_ASC(D_SCALER_8)         // After SCK Delay Scaler
    | (uint32_t)DSPI_CTAR_CSSCK(D_SCALER_8)      // PCS to SCK Delay Scaler.
    ),
//-> CTAR1 fast Config  Second 16 bit frame in 24 bits transfer
    ( (uint32_t)DSPI_CTAR_FMSZ(15)                 // Frame size -1
    | (uint32_t)DSPI_CTAR_CPOL(0)                 // Clock polarity: (DSPI_CTAR_CPOL) 0 - inactive state low; 1 - inactive state high
    | (uint32_t)DSPI_CTAR_CPHA(0)                 // Clock phase:    (DSPI_CTAR_CPHA) 0 -data captured on leading edge; 1 - data changed on leading edge
    | (uint32_t)DSPI_CTAR_PBR(BR_PRESCALER_5)     // Baud Rate Prescaler. (Master mode only)
    | (uint32_t)DSPI_CTAR_BR(BR_SCALER_8)        // Baud Rate divider      (BR)
    | (uint32_t)DSPI_CTAR_PCSSCK(Px_PRESCALER_5)  // PCS to SCK Delay Prescaler
    | (uint32_t)DSPI_CTAR_PASC(Px_PRESCALER_5)    // After SCK Delay Prescaler
    | (uint32_t)DSPI_CTAR_PDT(Px_PRESCALER_5)     // Delay after transfer prescaler
    | (uint32_t)DSPI_CTAR_DT(D_SCALER_4096)          // Delay after transfer
    | (uint32_t)DSPI_CTAR_ASC(D_SCALER_8)        // After SCK Delay Scaler
    | (uint32_t)DSPI_CTAR_CSSCK(D_SCALER_8)      // PCS to SCK Delay Scaler.
    ),
//-> MCR
    ( (uint32_t)DSPI_MCR_MSTR(1)           // Set DSPI2 as a master              (DSPI_MCR_MSTR)
    | (uint32_t)DSPI_MCR_CONT_SCKE(0)      // Enable / Disable continuous clock   (CONT_SCKE)
    | (uint32_t)DSPI_MCR_DCONF(DECONF_SPI) // Set SPI mode
    | (uint32_t)DSPI_MCR_ROOE(0)           //Receive FIFO overflow enable       
    | (uint32_t)DSPI_MCR_PCSIS0(1)         // Inactive state of CS
    | (uint32_t)DSPI_MCR_DIS_TXF(0)        //Enable TX fifo                      (DIS_TXF)
    | (uint32_t)DSPI_MCR_DIS_RXF(0)        //Enable RX fifo                      (DIS_RXF)
    | (uint32_t)DSPI_MCR_HALT(1)
    ),
//-> DSPI_RSER Interrupts Enabling
    0   
};
#else
  
#if 0
//DSPI0 Configuration for ST SPI in CTAR0 and CTAR 1 for slow mode
//Expected input clock 100MHz
//Baud rate max 5MHz   Real Baud rate 100MHz/5/16 = 0.625MHz
//Delay after transfer              1/(100MHz/3/16)  = 0.48us

const DSPI_Config_2ctrDef L9502_DSPI_Config ={
//->DSPI_Instance_p
    ( &DSPI_0),                     //0 pointer to selected DSPI periphery
// ->Command1
    (CONTINUOUS_CS | CTAR_0 | PCS0),
// ->Command2
    (CONTINUOUS_CS | CTAR_1 | PCS0),
//FirstCTARnumb
    0,
//-> CTAR0 Config  First 8 bit transfers in 24 bits transfer
    ( (uint32_t)DSPI_CTAR_FMSZ(7)                 // Frame size -1
    | (uint32_t)DSPI_CTAR_CPOL(0)                 // Clock polarity: (DSPI_CTAR_CPOL) 0 - inactive state low; 1 - inactive state high
    | (uint32_t)DSPI_CTAR_CPHA(0)                 // Clock phase:    (DSPI_CTAR_CPHA) 0 -data captured on leading edge; 1 - data changed on leading edge
    | (uint32_t)DSPI_CTAR_PBR(BR_PRESCALER_5)     // Baud Rate Prescaler. (Master mode only)
    | (uint32_t)DSPI_CTAR_BR(BR_SCALER_32)        // Baud Rate divider      (BR)
    | (uint32_t)DSPI_CTAR_PCSSCK(Px_PRESCALER_3)  // PCS to SCK Delay Prescaler
    | (uint32_t)DSPI_CTAR_PASC(Px_PRESCALER_3)    // After SCK Delay Prescaler
    | (uint32_t)DSPI_CTAR_PDT(Px_PRESCALER_3)     // Delay after transfer prescaler
    | (uint32_t)DSPI_CTAR_DT(D_SCALER_16)         // Delay after transfer
    | (uint32_t)DSPI_CTAR_ASC(D_SCALER_16)        // After SCK Delay Scaler
    | (uint32_t)DSPI_CTAR_CSSCK(D_SCALER_16)      // PCS to SCK Delay Scaler.
    ),
//-> CTAR1 fast Config  Second 16 bit frame in 24 bits transfer
    ( (uint32_t)DSPI_CTAR_FMSZ(15)                // Frame size -1
    | (uint32_t)DSPI_CTAR_CPOL(0)                 // Clock polarity: (DSPI_CTAR_CPOL) 0 - inactive state low; 1 - inactive state high
    | (uint32_t)DSPI_CTAR_CPHA(0)                 // Clock phase:    (DSPI_CTAR_CPHA) 0 -data captured on leading edge; 1 - data changed on leading edge
    | (uint32_t)DSPI_CTAR_PBR(BR_PRESCALER_5)     // Baud Rate Prescaler. (Master mode only)
    | (uint32_t)DSPI_CTAR_BR(BR_SCALER_32)        // Baud Rate divider      (BR)
    | (uint32_t)DSPI_CTAR_PCSSCK(Px_PRESCALER_3)  // PCS to SCK Delay Prescaler
    | (uint32_t)DSPI_CTAR_PASC(Px_PRESCALER_3)    // After SCK Delay Prescaler
    | (uint32_t)DSPI_CTAR_PDT(Px_PRESCALER_3)     // Delay after transfer prescaler
    | (uint32_t)DSPI_CTAR_DT(D_SCALER_16)         // Delay after transfer
    | (uint32_t)DSPI_CTAR_ASC(D_SCALER_16)        // After SCK Delay Scaler
    | (uint32_t)DSPI_CTAR_CSSCK(D_SCALER_16)      // PCS to SCK Delay Scaler.
    ),
//-> MCR
    ( (uint32_t)DSPI_MCR_MSTR(1)           // Set DSPI2 as a master              (DSPI_MCR_MSTR)
    | (uint32_t)DSPI_MCR_CONT_SCKE(0)      // Enable / Disable continuous clock   (CONT_SCKE)
    | (uint32_t)DSPI_MCR_DCONF(DECONF_SPI) // Set SPI mode
    | (uint32_t)DSPI_MCR_ROOE(0)           //Receive FIFO overflow enable
    | (uint32_t)DSPI_MCR_PCSIS0(1)         // Inactive state of CS
    | (uint32_t)DSPI_MCR_DIS_TXF(0)        //Enable TX fifo                      (DIS_TXF)
    | (uint32_t)DSPI_MCR_DIS_RXF(0)        //Enable RX fifo                      (DIS_RXF)
    | (uint32_t)DSPI_MCR_HALT(1)
    ),
//-> DSPI_RSER Interrupts Enabling
    0
};

#endif
#endif

#if 0 /* not used */
const DSPI_Config_2ctrDef L9502_DSPI_Config1 ={
//->DSPI_Instance_p
    ( &DSPI_1),                     //0 pointer to selected DSPI periphery
// ->Command1
    (CONTINUOUS_CS | CTAR_0 | PCS3),
// ->Command2
    (CONTINUOUS_CS | CTAR_1 | PCS3),
//FirstCTARnumb
    0,
//-> CTAR0 Config  First 8 bit transfers in 24 bits transfer
    ( (uint32_t)DSPI_CTAR_FMSZ(7)                // Frame size -1
    | (uint32_t)DSPI_CTAR_CPOL(0)                 // Clock polarity: (DSPI_CTAR_CPOL) 0 - inactive state low; 1 - inactive state high
    | (uint32_t)DSPI_CTAR_CPHA(0)                 // Clock phase:    (DSPI_CTAR_CPHA) 0 -data captured on leading edge; 1 - data changed on leading edge
    | (uint32_t)DSPI_CTAR_PBR(BR_PRESCALER_2)     // Baud Rate Prescaler. (Master mode only)
    | (uint32_t)DSPI_CTAR_BR(BR_SCALER_2)        // Baud Rate divider      (BR)
    | (uint32_t)DSPI_CTAR_PCSSCK(Px_PRESCALER_1)  // PCS to SCK Delay Prescaler
    | (uint32_t)DSPI_CTAR_PASC(Px_PRESCALER_1)    // After SCK Delay Prescaler
    | (uint32_t)DSPI_CTAR_PDT(Px_PRESCALER_1)     // Delay after transfer prescaler
    | (uint32_t)DSPI_CTAR_DT(D_SCALER_2)          // Delay after transfer
    | (uint32_t)DSPI_CTAR_ASC(D_SCALER_2)         // After SCK Delay Scaler
    | (uint32_t)DSPI_CTAR_CSSCK(D_SCALER_2)      // PCS to SCK Delay Scaler.
    ),
//-> CTAR1 fast Config  Second 16 bit frame in 24 bits transfer
    ( (uint32_t)DSPI_CTAR_FMSZ(15)                 // Frame size -1
    | (uint32_t)DSPI_CTAR_CPOL(0)                 // Clock polarity: (DSPI_CTAR_CPOL) 0 - inactive state low; 1 - inactive state high
    | (uint32_t)DSPI_CTAR_CPHA(0)                 // Clock phase:    (DSPI_CTAR_CPHA) 0 -data captured on leading edge; 1 - data changed on leading edge
    | (uint32_t)DSPI_CTAR_PBR(BR_PRESCALER_2)     // Baud Rate Prescaler. (Master mode only)
    | (uint32_t)DSPI_CTAR_BR(BR_SCALER_2)        // Baud Rate divider      (BR)
    | (uint32_t)DSPI_CTAR_PCSSCK(Px_PRESCALER_1)  // PCS to SCK Delay Prescaler
    | (uint32_t)DSPI_CTAR_PASC(Px_PRESCALER_1)    // After SCK Delay Prescaler
    | (uint32_t)DSPI_CTAR_PDT(Px_PRESCALER_1)     // Delay after transfer prescaler
    | (uint32_t)DSPI_CTAR_DT(D_SCALER_2)          // Delay after transfer
    | (uint32_t)DSPI_CTAR_ASC(D_SCALER_2)        // After SCK Delay Scaler
    | (uint32_t)DSPI_CTAR_CSSCK(D_SCALER_2)      // PCS to SCK Delay Scaler.
    ),
//-> MCR
    ( (uint32_t)DSPI_MCR_MSTR(1)           // Set DSPI2 as a master              (DSPI_MCR_MSTR)
    | (uint32_t)DSPI_MCR_CONT_SCKE(0)      // Enable / Disable continuous clock   (CONT_SCKE)
    | (uint32_t)DSPI_MCR_DCONF(DECONF_SPI) // Set SPI mode
    | (uint32_t)DSPI_MCR_ROOE(0)           //Receive FIFO overflow enable       
    | (uint32_t)DSPI_MCR_PCSIS0(1)         // Inactive state of CS
    | (uint32_t)DSPI_MCR_DIS_TXF(0)        //Enable TX fifo                      (DIS_TXF)
    | (uint32_t)DSPI_MCR_DIS_RXF(0)        //Enable RX fifo                      (DIS_RXF)
    | (uint32_t)DSPI_MCR_HALT(1)
    ),
//-> DSPI_RSER Interrupts Enabling
    0   
};
#endif

// ****************************************************************************
// ***************************** Global Variables *****************************
// ****************************************************************************
//uint32_t  capturedRealTime;
L9502_ERRstatus_t L9502_ERRstatus[L9502_DEVICES_MAX];

//Use global variables for debug purposes
//used by init
L9502_Frame_t L9502_frame[L9502_DEVICES_MAX];
L9502_Frame_t L9502_frame_received[L9502_DEVICES_MAX];
//used by  L9502_ReadSingleReg and L9502_WriteSingleReg
L9502_Frame_t L9502_RSRframe[L9502_DEVICES_MAX];
L9502_Frame_t L9502_RSRframeReceived[L9502_DEVICES_MAX];

/**
 * @brief   L9502 internal data instance
 */
static L9502_internalData_t g_internalData;

/**
 * @brief   L9502 status registers instance
 */
static L9502_statusReg_t    g_statusReg[L9502_DEVICE_NUM];

uint32_t L9502_wErrorsNow[L9502_DEVICE_NUM];
uint32_t L9502_wErrorsOccurred[L9502_DEVICE_NUM];
volatile uint8_t sch_checkerror = 0;


// ****************************************************************************************************
//  Function: void spi_send_24(void)
//return 0
// ****************************************************************************************************
uint16_t spi_send_24(uint16_t number, uint32_t* dataTX, uint32_t* dataRX){
   gpio_clear_pin(SPI1_NSS);
    while(number>0){
        number--;
        /* Send data via SPI1.*/
        spi_rx_buffer[0] = 0;
        spi_rx_buffer[1] = 0;
        spi_rx_buffer[2] = 0;
        spi_tx_buffer[0] = (uint8_t)((dataTX[number]) >> 16); 
        spi_tx_buffer[1] = (uint8_t)((dataTX[number] & 0x0000FFFF) >> 8); 
        spi_tx_buffer[2] = (uint8_t)(dataTX[number] & 0x000000FF); 
        spi_exchange(&DRV_SPI1, spi_tx_buffer, spi_rx_buffer,
                 SPI_BUFFER_SIZE);   
#if 0                 
        /* Wait till SPI1 data receiving is completed.*/
        do {
        ;
        } while(spi1_completed == 0);    
        spi1_completed = 0;
#endif        
        dataRX[number] = (uint32_t) ((spi_rx_buffer[0]<<16) | (spi_rx_buffer[1]<<8) | (spi_rx_buffer[2])) ;
    }
    gpio_set_pin(SPI1_NSS);                 

#if 0    
    uint16_t RxdataL;
    uint16_t RxdataH;
    uint32_t TXdataL;
    uint32_t TXdataH;
    DSPI_tag_p dspi_instance_p = Config->DSPI_Instance_p;


    Clear EOQ flag to be sure there it is not set from other communication
    ((DSPI_tag_p)(Config->DSPI_Instance_p))->SR.R = DSPI_SR_TCF |  DSPI_SR_EOQF;
    Clear RFDF flag to be sure there it is not set from other communication
    ((DSPI_tag_p)(Config->DSPI_Instance_p))->SR.R = DSPI_SR_TCF |  DSPI_SR_RFDF;
    while(number>0){
        number--;
        TXdataL = dataTX[number] & 0xFFFF;
        TXdataH = dataTX[number] >> 16;
        TXdataH |=  Config->Command1;
        TXdataL |=  Config->Command2;
        dspi_instance_p->SR.R = DSPI_SR_TCF |  DSPI_SR_TFFF;
        while(dspi_instance_p->SR.B.TFFF == 0);
        dspi_instance_p->PUSHR.R = TXdataH;
        if(number > 0){
            dspi_instance_p->SR.R = DSPI_SR_TCF |  DSPI_SR_TFFF;
            while(dspi_instance_p->SR.B.TFFF == 0);
            dspi_instance_p->PUSHR.R = TXdataL;
        }
        else{
            dspi_instance_p->SR.R = DSPI_SR_TCF |  DSPI_SR_TFFF;
            while(dspi_instance_p->SR.B.TFFF == 0);
            dspi_instance_p->PUSHR.R = TXdataL | EOQ_EN;
        }
        while(dspi_instance_p->SR.B.RFDF == 0);
        RxdataH = dspi_instance_p->POPR.R; // Read received data H
        dspi_instance_p->SR.R = DSPI_SR_TCF |  DSPI_SR_RFDF;
        while(dspi_instance_p->SR.B.RFDF == 0);
        RxdataL = dspi_instance_p->POPR.R; // Read received data L
        dspi_instance_p->SR.R = DSPI_SR_TCF |  DSPI_SR_RFDF;
        dataRX[number] = ((uint32_t)RxdataH)<<16 | RxdataL;
//        dspi_instance_p->SR.R = DSPI_SR_TCF |  DSPI_SR_EOQF;
    }

 //   dspi_instance_p->SR.R = DSPI_SR_TCF |  DSPI_SR_EOQF;

//    while (dspi_instance_p->SR.B.EOQF == 0); // wait until transfer is not complete.
//    RxdataH = dspi_instance_p->POPR.R; // Read received data H
//    RxdataL = dspi_instance_p->POPR.R; // Read received data L
#endif
    return 0;
}

/**
  * @brief This function send L9502 SPI frames (CRC legacy mode calculation)
  * @param L9502_Frame_t * TxFrame_p - pointer to transmitted frame.
  * @param L9502_Frame_t * RxFrame_p - pointer to received frame  
  * @retval Error message
  *         0 NO_ERROR
  *         1 ERR_WRONG_CRC
  *         2 ERR_COUNTER_ERROR
  *         3 ERR_SPI_ERROR
  */
uint16_t  L9502_SendFrame(L9502_Frame_t * TxFrame_p, L9502_Frame_t * RxFrame_p){
    uint8_t crc;
    uint16_t i;
    uint16_t error = NO_ERROR;
    L9502_Frame_t RxFrame;
    
    for(i=0; i< L9502Config.NumberOfDevices; i++){
        //crc = CrcCalc8bitLookupTab(&L9502crc_Config, (uint8_t *)&TxFrame_p[i].R+1); //Point to first valid byte, highest byte is not used
        crc = CrcCalc8bitLookupTab(&L9502crc_Config, (uint8_t *)&TxFrame_p[i].R); //Point to first valid byte, highest byte is not used
        TxFrame_p[i].R |=  L9502_CRC(crc) |  L9502_FC(g_internalData.frameCounter[i]);
    }
    // -------------------
    // add SPI SDK dspi_send_nx32(L9502Config.NumberOfDevices, &L9502_DSPI_Config, (uint32_t *)TxFrame_p, (uint32_t *)RxFrame_p);
    // -------------------    
    spi_send_24(L9502Config.NumberOfDevices, (uint32_t *)TxFrame_p, (uint32_t *)RxFrame_p);
    
    for(i=0; i< L9502Config.NumberOfDevices; i++)
    {
      /*
      if(RxFrame_p[i].RX_B.SPI_ERR == 1){
          L9502_ERRstatus[i].B.SPI_ERROR = 1;
          L9502_ERRstatus[i].B.LAST_FRAME_NOK = 1;
          error |= ERR_SPI_ERROR;
      }*/
      RxFrame.R =  RxFrame_p[i].R & L9502_VALID_FRAME_MASK;           //CRC bit field must be cleared during CRC calculation
      if(RxFrame_p[i].RX_B.CRC_F != CrcCalc8bitLookupTab(&L9502crc_Config, (uint8_t *)&RxFrame)){        //Point to first valid byte, highest byte is not used
        L9502_ERRstatus[i].B.CRC_ERROR = 1;
        L9502_ERRstatus[i].B.LAST_FRAME_NOK = 1;
        error |= ERR_WRONG_CRC;
      }
      if(g_internalData.frameCounter[i] != RxFrame_p[i].RX_B.FC){
        L9502_ERRstatus[i].B.COUNTER_ERROR = 1;
        L9502_ERRstatus[i].B.LAST_FRAME_NOK = 1;
        error |= ERR_COUNTER_ERROR;
      }
      else
      {
        g_internalData.frameCounter[i]++;
        g_internalData.frameCounter[i] &= 1;
      }
    }

    return error;
}

/**
  * @brief This function to enter Test Mode LV
  * @param device
  * @retval RxFrame 
  */
uint32_t  L9502_EnterTestModeLV(uint8_t device){
    uint8_t i;
    device--;
    for(i=0; i< L9502_DEVICES_MAX; i++){
        L9502_RSRframe[i].R = L9502_R_W(L9502_R_W_READ) | L9502_ADDRESS(ADDR_CHIP_ID_LV) | L9502_DATA(0);
    }
    L9502_RSRframe[device].R =  L9502_R_W(L9502_R_W_WRITE) | L9502_ADDRESS(ADDR_TM_ENTER_LV) | L9502_DATA(0xC1);
    L9502_SendFrame(L9502_RSRframe, L9502_RSRframeReceived);
    L9502_RSRframe[device].R =  L9502_R_W(L9502_R_W_WRITE) | L9502_ADDRESS(ADDR_TM_ENTER_LV) | L9502_DATA(0x3E);
    L9502_SendFrame(L9502_RSRframe, L9502_RSRframeReceived);
    L9502_RSRframe[device].R =  L9502_R_W(L9502_R_W_WRITE) | L9502_ADDRESS(ADDR_TM_ENTER_LV) | L9502_DATA(0xF0);
    L9502_SendFrame(L9502_RSRframe, L9502_RSRframeReceived);
    return  L9502_RSRframeReceived[device].R;
}

/**
  * @brief This function to exit Test Mode LV
  * @param device
  * @retval RxFrame 
  */
uint32_t  L9502_ExitTestModeLV(uint8_t device){
    uint8_t i;
    device--;
    for(i=0; i< L9502_DEVICES_MAX; i++){
        L9502_RSRframe[i].R = L9502_R_W(L9502_R_W_READ) | L9502_ADDRESS(ADDR_CHIP_ID_LV) | L9502_DATA(0);
    }
    L9502_RSRframe[device].R =  L9502_R_W(L9502_R_W_WRITE) | L9502_ADDRESS(ADDR_TM_ENTER_LV) | L9502_DATA(0xF1);
    L9502_SendFrame(L9502_RSRframe, L9502_RSRframeReceived);
    return  L9502_RSRframeReceived[device].R;
}

/**
  * @brief This function to enter Test Mode HV
  * @param device
  * @retval RxFrame 
  */
uint32_t  L9502_EnterTestModeHV(uint8_t device){
    uint8_t i;
    device--;
    for(i=0; i< L9502_DEVICES_MAX; i++){
        L9502_RSRframe[i].R = L9502_R_W(L9502_R_W_READ) | L9502_ADDRESS(ADDR_CHIP_ID_LV) | L9502_DATA(0);
    }
    L9502_RSRframe[device].R =  L9502_R_W(L9502_R_W_WRITE) | L9502_ADDRESS(ADDR_TM_ENTER_HV) | L9502_DATA(0xC1);
    L9502_SendFrame(L9502_RSRframe, L9502_RSRframeReceived);
    L9502_RSRframe[device].R =  L9502_R_W(L9502_R_W_WRITE) | L9502_ADDRESS(ADDR_TM_ENTER_HV) | L9502_DATA(0x3E);
    L9502_SendFrame(L9502_RSRframe, L9502_RSRframeReceived);
    L9502_RSRframe[device].R =  L9502_R_W(L9502_R_W_WRITE) | L9502_ADDRESS(ADDR_TM_ENTER_HV) | L9502_DATA(0xF0);
    L9502_SendFrame(L9502_RSRframe, L9502_RSRframeReceived);
    return  L9502_RSRframeReceived[device].R;
}

/**
  * @brief This function to exit Test Mode HV
  * @param device
  * @retval RxFrame 
  */
uint32_t  L9502_ExitTestModeHV(uint8_t device){
    uint8_t i;
    device--;
    for(i=0; i< L9502_DEVICES_MAX; i++){
        L9502_RSRframe[i].R = L9502_R_W(L9502_R_W_READ) | L9502_ADDRESS(ADDR_CHIP_ID_LV) | L9502_DATA(0);
    }
    L9502_RSRframe[device].R =  L9502_R_W(L9502_R_W_WRITE) | L9502_ADDRESS(ADDR_TM_ENTER_HV) | L9502_DATA(0xF1);
    L9502_SendFrame(L9502_RSRframe, L9502_RSRframeReceived);
    return  L9502_RSRframeReceived[device].R;
}

/**
  * @brief This function used to enter CONFIG MODE
  * @param device
  * @retval void 
  */
void  L9502_EnterConfigMode(uint8_t device){

    uint16_t falign; /* flag to align frame counter */
    uint8_t i;

    L9502_ClearBuffer();
    /* Semi Unlock Commands */
    L9502_RSRframe[device].R =  L9502_R_W(L9502_R_W_WRITE) | L9502_ADDRESS(ADDR_UNLOCK_KEY) | L9502_DATA(CFG_ENTER_1);
    falign = L9502_SendFrame(L9502_RSRframe, L9502_RSRframeReceived);
    if((falign & ERR_COUNTER_ERROR) == ERR_COUNTER_ERROR)
    {
      /* SW workaround in case micro and device do not have frame Counter in sync */
      for(i=0; i< L9502_DEVICES_MAX; i++){
          L9502_ERRstatus[i].R = 0;
      }

      L9502_ClearBuffer();
      L9502_RSRframe[device].R =  L9502_R_W(L9502_R_W_WRITE) | L9502_ADDRESS(ADDR_UNLOCK_KEY) | L9502_DATA(CFG_ENTER_1);
      L9502_SendFrame(L9502_RSRframe, L9502_RSRframeReceived);
    }

    /* Update global Buffer to complete Unlock Command*/
    L9502_ClearBuffer();
    L9502_RSRframe[device].R =  L9502_R_W(L9502_R_W_WRITE) | L9502_ADDRESS(ADDR_UNLOCK_KEY) | L9502_DATA(CFG_ENTER_2);
    L9502_SendFrame(L9502_RSRframe, L9502_RSRframeReceived);

}

/**
  * @brief This function used to exit CONFIG MODE
  * @param device
  * @retval void 
  */
void  L9502_ExitConfigMode(uint8_t device){

    L9502_ClearBuffer();
    L9502_RSRframe[device].R =  L9502_R_W(L9502_R_W_WRITE) | L9502_ADDRESS(ADDR_UNLOCK_KEY) | L9502_DATA(CFG_LEAVE);
    L9502_SendFrame(L9502_RSRframe, L9502_RSRframeReceived);
}

/**
  * @brief This function sent first frame if line is idle.
  *        CRC legacy mode calculation
  * @param device
  * @param Register address to read 
  * @param  *RxFrame - pointer to received frame  
  * @retval RxFrame 
  */
uint16_t  L9502_ReadSingleDev(uint16_t device, uint16_t address, L9502_Frame_t *RxFrame)
{
    uint16_t error;
    
    L9502_ClearBuffer();
    L9502_RSRframe[device].R =  L9502_R_W(L9502_R_W_READ) | L9502_ADDRESS(address) | L9502_DATA(0);
    error = L9502_SendFrame(L9502_RSRframe, L9502_RSRframeReceived);

    //osal_delay_microsec(200);
    dwt_reset();
    dwt_delay_ticks(dwt_us2t(200));

    L9502_ClearBuffer();
    L9502_RSRframe[device].R =  L9502_R_W(L9502_R_W_READ) | L9502_ADDRESS(address) | L9502_DATA(0);
    error |= L9502_SendFrame(L9502_RSRframe, L9502_RSRframeReceived);

    *RxFrame = L9502_RSRframeReceived[device];
    
    return error;
}

/**
  * @brief This function sent multiple frames 
  *        CRC legacy mode calculation
  * @param Number of device to read in daisy chain
  * @param Register address to read 
  * @param  *RxFrame - pointer to received frame  
  * @retval RxFrame 
  */
uint16_t  L9502_ReadMultipleDev(uint16_t number, uint16_t address, L9502_Frame_t *RxFrame)
{
    uint8_t i;
    uint16_t error = NO_ERROR;

    L9502_ClearBuffer();

    for(i=0; i< number; i++){
        L9502_RSRframe[i].R = L9502_R_W(L9502_R_W_READ) | L9502_ADDRESS(address) | L9502_DATA(0);
    }
    error = L9502_SendFrame(L9502_RSRframe, L9502_RSRframeReceived);

    //osal_delay_microsec(200);
    dwt_reset();
    dwt_delay_ticks(dwt_us2t(200));

    for(i=0; i< number; i++){
        L9502_RSRframe[i].R = L9502_R_W(L9502_R_W_READ) | L9502_ADDRESS(address) | L9502_DATA(0);
    }
    error |= L9502_SendFrame(L9502_RSRframe, L9502_RSRframeReceived);
    //osal_delay_microsec(200);
    dwt_reset();
    dwt_delay_ticks(dwt_us2t(200));    

    for(i=0; i< number; i++){
      *RxFrame =  L9502_RSRframeReceived[i];
      RxFrame++;
    }
    return  error;
}

#if defined(VERSION_CA) || defined(VERSION_CD)
/**
  * @brief This function Send a Read command to multiple device connected in daisy chain
  *        With rlr = 1 request a Clear on read of the RLR field in the status register   
  *        Called by L9502_CheckErrors once in the schedule and once in L9502_FaultRecovery
  * @param Number of device to read in daisy chain
  * @param Register address to read 
  * @param  *RxFrame - pointer to received frame  
  * @retval RxFrame 
  */
uint16_t  L9502_ReadMultipleDevRlr(uint16_t number, uint16_t address, L9502_Frame_t *RxFrame, uint8_t rlr)
{
    uint8_t i;
    uint16_t error = NO_ERROR;
    uint16_t data = L9502_MSB_DATA_RESET;

    if (rlr == 1)
      data = L9502_MSB_DATA_SET;

    /* This function has effect only on registers with RLR bit */
    if ( (address == ADDR_FHM_3) || (address == ADDR_FHM_2) ||
         (address == ADDR_FHM_0) || (address == ADDR_FHM_1) ||
         (address == ADDR_CFG_7) || (address == ADDR_STO_CFG) )
    {

      L9502_ClearBuffer();

      for(i=0; i< number; i++){
          L9502_RSRframe[i].R = L9502_R_W(L9502_R_W_READ) | L9502_ADDRESS(address) | L9502_DATA(data);
      }
    error = L9502_SendFrame(L9502_RSRframe, L9502_RSRframeReceived);

      //osal_delay_microsec(200);
      dwt_reset();
      dwt_delay_ticks(dwt_us2t(200));

      for(i=0; i< number; i++){
          L9502_RSRframe[i].R = L9502_R_W(L9502_R_W_READ) | L9502_ADDRESS(address) | L9502_DATA(data);
      }
      error |= L9502_SendFrame(L9502_RSRframe, L9502_RSRframeReceived);
      //osal_delay_microsec(200);
      dwt_reset();
      dwt_delay_ticks(dwt_us2t(200));

    for(i=0; i< number; i++){
      *RxFrame =  L9502_RSRframeReceived[i];
      RxFrame++;
    }
    }

    return  error;
}
#endif

/**
  * @brief This function sent first frame if line is idle
  * @param device device number
  * @param address Register address to read
  * @param page Page number in the Page registers (CFG_10 and CFG_11)
  * @param  *RxFrame - pointer to received frame  
  * @retval RxFrame 
  */
uint16_t  L9502_ReadPageSingleDev(uint16_t device, uint16_t address, uint16_t page, L9502_Frame_t *RxFrame)
{
    uint16_t error;

    L9502_ClearBuffer();
    L9502_RSRframe[device].R =  L9502_R_W(L9502_R_W_READ) | L9502_ADDRESS(address) | L9502_DATA(page);
    error = L9502_SendFrame(L9502_RSRframe, L9502_RSRframeReceived);

    /*osal_delay_microsec(200);*/ dwt_reset(); dwt_delay_ticks(dwt_us2t(200));

    L9502_ClearBuffer();
    L9502_RSRframe[device].R =  L9502_R_W(L9502_R_W_READ) | L9502_ADDRESS(address) | L9502_DATA(page);
    error |= L9502_SendFrame(L9502_RSRframe, L9502_RSRframeReceived);

    *RxFrame = L9502_RSRframeReceived[device];

    return error;
}

/**
  * @brief This function sent first frame if line is idle
  * @param number Number of device to read in daisy chain
  * @param address Register address to read
  * @param page Page number in the Page registers (CFG_10 and CFG_11)
  * @param  *RxFrame - pointer to received frame  
  * @retval RxFrame 
  */
uint16_t  L9502_ReadPageMultipleDev(uint16_t number, uint16_t address, uint16_t page, L9502_Frame_t *RxFrame)
{
    uint8_t i;
    uint16_t error;

    L9502_ClearBuffer();

    for(i=0; i< number; i++){
        L9502_RSRframe[i].R = L9502_R_W(L9502_R_W_READ) | L9502_ADDRESS(address) | L9502_DATA(page);
    }
    error = L9502_SendFrame(L9502_RSRframe, L9502_RSRframeReceived);

    //osal_delay_microsec(200);
    dwt_reset();
    dwt_delay_ticks(dwt_us2t(200));
    

    for(i=0; i< number; i++){
        L9502_RSRframe[i].R = L9502_R_W(L9502_R_W_READ) | L9502_ADDRESS(address) | L9502_DATA(page);
    }
    error |= L9502_SendFrame(L9502_RSRframe, L9502_RSRframeReceived);
    //osal_delay_microsec(200);
    dwt_reset();
    dwt_delay_ticks(dwt_us2t(200));    

    for(i=0; i< number; i++){
      *RxFrame =  L9502_RSRframeReceived[i];
      RxFrame++;
    }
    return  error;
}

 /**
  * @brief This function write HV register for single device 
  *        Info: crc for CFG_0 and CFG_1 is calculated inside the function
  *       Fly back should be enabled 
  * @param device device number
  * @param address Register address 
  * @param value
  */
uint32_t  L9502_WriteHVSingleDev(uint8_t device, uint16_t address, uint16_t value)
{
  L9502_Frame_t    rxPacket;
  uint8_t crc; /* crc for Safety Relevant Registers */
  uint16_t wrt_error = NO_ERROR;
  uint16_t rd_error = NO_ERROR;

  if (address == ADDR_CFG_7)
  {
    wrt_error = L9502_WriteSingleDev(device, address, (value), L9502_frame_received);
    //osal_delay_microsec(200);
    dwt_reset();
    dwt_delay_ticks(dwt_us2t(200));    
  }
  else
  {
    /* Start configuring HV Side*/
    /* High Voltage side SRR registers are protected with crc check */

    /* Enter configuration Mode */
    L9502_EnterConfigMode(device);
    //osal_delay_microsec(200);
    dwt_reset();
    dwt_delay_ticks(dwt_us2t(200));
    

    switch(address){
      case ADDR_CFG_0:
        crc = crcTbl[value>>3];
        wrt_error = L9502_WriteSingleDev(device, address, (value | crc), L9502_frame_received);
      break;
      case ADDR_CFG_1:
        crc = crcTbl[value>>3];
        wrt_error = L9502_WriteSingleDev(device, address, (value | crc), L9502_frame_received);
      break;
      case ADDR_CFG_2:
      case ADDR_CFG_3:
      case ADDR_CFG_4:
      case ADDR_CFG_5:
      case ADDR_CFG_6:
      case ADDR_STO_CFG:
        wrt_error = L9502_WriteSingleDev(device, address, (value), L9502_frame_received);
      break;
      default:
        /* nothing to do */
      break;
    }
    //osal_delay_microsec(200);
    dwt_reset();
    dwt_delay_ticks(dwt_us2t(200));    

    /* Exit configuration Mode */
    L9502_ExitConfigMode(device);
  }

  if(wrt_error == NO_ERROR )
  {
    rd_error = L9502_ReadSingleDev(device, address, &rxPacket);

    if( (rd_error == NO_ERROR ) /* && ( (rxPacket.B.DATA & value) == value) */)
        return NO_ERROR;
    else
      return rd_error;
  }
  else
  {
    return wrt_error;
  }
}

 /**
  * @brief This function write LV register for single device 
  *        Precondition: SAFE_SEL and WTD_REACT_CFG in CFG_8 must match the SRR parity
  * @param device device number
  * @param address Register address 
  * @param value
  */
uint32_t  L9502_WriteLVSingleDev(uint8_t device, uint16_t address, uint16_t value)
{
  L9502_Frame_t    rxPacket;
  uint16_t wrt_error = NO_ERROR;
  uint16_t rd_error = NO_ERROR;

  /* Start configuring LV Side*/
  /* High Voltage side SRR registers are protected with crc check */

  /* Enter configuration Mode */
  L9502_EnterConfigMode(device);
  //osal_delay_microsec(200);
  dwt_reset();
  dwt_delay_ticks(dwt_us2t(200));
  

  switch(address){
   case ADDR_CFG_8:
   case ADDR_CFG_9:
      wrt_error = L9502_WriteSingleDev(device, address, (value), L9502_frame_received);
      //osal_delay_microsec(L9502_WAIT_LV);
      dwt_reset();
      dwt_delay_ticks(dwt_us2t(L9502_WAIT_LV));
    break;
    default:
      /* nothing to do */
    break;
  }
  /* Exit configuration Mode */
  L9502_ExitConfigMode(device);
  //osal_delay_microsec(L9502_WAIT_LV);
  dwt_reset();
  dwt_delay_ticks(dwt_us2t(L9502_WAIT_LV));

  if(wrt_error == NO_ERROR )
  {
    rd_error = L9502_ReadSingleDev(device, address, &rxPacket);

    if( (rd_error == NO_ERROR ) /* && ( (rxPacket.B.DATA & value) == value) */)
        return NO_ERROR;
    else
       return rd_error;
  }
  else
  {
      return wrt_error;
  }
}

 /**
  * @brief This function write page register for single device 
  * @param device device number
  * @param page page number
  * @param address Register address 
  * @param value
  */
uint32_t  L9502_WritePageSingleDev(uint8_t device, uint16_t page,  uint16_t address, uint16_t value)
{
  L9502_Frame_t    rxPacket;
  uint16_t wrt_error = NO_ERROR;
  uint16_t rd_error = NO_ERROR;

  if( (address == ADDR_CFG_10) && (page == L9502_PAGE_2) )
  {
      wrt_error = L9502_WriteSingleDev(device, address, ( L9502_PAGE_2 | value ), L9502_frame_received);
      //osal_delay_microsec(L9502_WAIT_HV);
      dwt_reset();
      dwt_delay_ticks(dwt_us2t(L9502_WAIT_HV));      
  }
  else if(address == ADDR_CFG_10)
  {
    /* Enter configuration Mode */
    L9502_EnterConfigMode(device);
    //osal_delay_microsec(L9502_WAIT_HV);
    dwt_reset();
    dwt_delay_ticks(dwt_us2t(L9502_WAIT_HV));          

    switch(page){
      case L9502_PAGE_1:
        wrt_error = L9502_WriteSingleDev(device, address, (L9502_PAGE_1 | value), L9502_frame_received);
      break;
      case L9502_PAGE_3:
        wrt_error = L9502_WriteSingleDev(device, address, (L9502_PAGE_3 | value), L9502_frame_received);
      break;
      default:
        /* nothing to do */
      break;
      //osal_delay_microsec(L9502_WAIT_HV);
      dwt_reset();
      dwt_delay_ticks(dwt_us2t(L9502_WAIT_HV));          

      /* Exit configuration Mode */
      L9502_ExitConfigMode(device);
    }
  }
  else if (address == ADDR_CFG_11)
  {

   /* Enter configuration Mode */
    L9502_EnterConfigMode(device);
    /*osal_delay_microsec(L9502_WAIT_HV); */ dwt_reset(); dwt_delay_ticks(dwt_us2t(L9502_WAIT_HV));

    switch(page){
      case L9502_PAGE_1:
        wrt_error = L9502_WriteSingleDev(device, address, (L9502_PAGE_1 | value ), L9502_frame_received);
      break;
      case L9502_PAGE_2:
        wrt_error = L9502_WriteSingleDev(device, address, (L9502_PAGE_2 | value ), L9502_frame_received);
      break;
      case L9502_PAGE_3:
        wrt_error = L9502_WriteSingleDev(device, address, (L9502_PAGE_3 | value), L9502_frame_received);
      break;
      default:
        /* nothing to do */
      break;
    }
    /*osal_delay_microsec(L9502_WAIT_HV); */ dwt_reset(); dwt_delay_ticks(dwt_us2t(L9502_WAIT_HV));;

    /* Exit configuration Mode */
    L9502_ExitConfigMode(device);

  }
  else
  {
    /* Do nothing wrong address */
    return 0xFF;
  }

  if(wrt_error == NO_ERROR )
  {
    rd_error = L9502_ReadSingleDev(device, address, &rxPacket);

    if( (rd_error == NO_ERROR ) /* && ( (rxPacket.B.DATA & value) == value) */)
        return NO_ERROR;
    else
      return rd_error;
  }
  else
  {
    return wrt_error;
  }
}

 /**
  * @brief Fill Read Status Register (RSR) Sent and received buffer by dummy commands
  * @param void

  */
void  L9502_ClearBuffer(void){
    uint8_t i;

    for(i=0; i< L9502_DEVICES_MAX; i++){
        L9502_RSRframe[i].R = L9502_R_W(L9502_R_W_READ) | L9502_ADDRESS(ADDR_CHIP_ID_LV) | L9502_DATA(0);
    }
    for(i=0; i< L9502_DEVICES_MAX; i++){
        L9502_RSRframeReceived[i].R = L9502_R_W(L9502_R_W_READ) | L9502_ADDRESS(ADDR_CHIP_ID_LV) | L9502_DATA(0);
    }
}

 /**
  * @brief This function write register for single device 
  * @param device device number
  * @param address Register address 
  * @param data data
  * @param RxFrame_p - pointer to received frame
  */
uint16_t  L9502_WriteSingleDev(uint16_t device, uint16_t address, uint16_t data, L9502_Frame_t *RxFrame){
    uint8_t i;
    uint16_t error;

    for(i=0; i< L9502_DEVICES_MAX; i++){
        L9502_RSRframe[i].R = L9502_R_W(L9502_R_W_READ) | L9502_ADDRESS(ADDR_CHIP_ID_LV) | L9502_DATA(0);
    }
    L9502_RSRframe[device].R =  L9502_R_W(L9502_R_W_WRITE) | L9502_ADDRESS(address) | L9502_DATA(data);
    error = L9502_SendFrame(L9502_RSRframe, L9502_RSRframeReceived);
    
    *RxFrame =  L9502_RSRframeReceived[device];
    return  error;
}

#if 0
// ****************************************************************************
// Name:        L9502_PutSingleCommand
// Parameters:
//              L9502_Frame_t * TxFrame - pointer to transmited frame
// Returns:     None
// Affected Global Variables:
//              L9502_RSRframe
// Description:
//              Put command wor single device to buffer
// ****************************************************************************
void L9502_PutSingleCommand(uint16_t device, uint16_t address, uint16_t data, L9502_Frame_t *RxFrame){

    L9502_RSRframe[device].R =  L9502_R_W(L9502_R_W_WRITE) | L9502_ADDRESS(address) | L9502_DATA(data);
    
}
#endif

// ****************************************************************************
// Name:        L9502_WriteMultipleReg
// Parameters:
//              L9502_Frame_t * TxFrame - pointer to transmitted frame
// Returns:     RxFrame
// Affected Global Variables:
//              L9502_ERRstatus - ERROR_FRAME_RECEIVED
//                                CRC_ERR
//                                LAST_FRAME_NOK
//              L9502_frame
//              L9502_frame_received
// Description:
//              Sent first frame if line is idle
//              CRC legacy mode calculation
//              Expected that CRC part o frame is cleared
// ****************************************************************************
#if 0 /* Not needed L9502_SendFrame already does this functionality */
uint16_t  L9502_WriteMultipleDev(uint16_t device, uint16_t address, uint16_t data, L9502_Frame_t *RxFrame){
    uint16_t error;

    L9502_RSRframe[device].R =  L9502_R_W(L9502_R_W_WRITE) | L9502_ADDRESS(address) | L9502_DATA(data);
    error = L9502_SendFrame(L9502_RSRframe, L9502_RSRframeReceived);
    
    *RxFrame =  L9502_RSRframeReceived[device];
    return  error;
}
#endif

 /**
  * @brief This function initialiaze L9502 driver\n
  *        Provide also initialization of SPI\n
  *        Depends on condition, the ERRstatus could differ to returned status message\n
  *        Affected Global Variables:\n
  *             L9502_ERRstatus - ERROR_FRAME_RECEIVED
  *                               CRC_ERR
  *                               LAST_FRAME_NOK
  *                               INITIALIZATION_ERR
  *             L9502_status    - WAKEUP_OK if master communicates
  *                               CELLS_ENABLED if received frames have correct CRC and no error frame is received
  *             L9502_frame
  *             L9502_frame_received
  * @param void
  * @return status message   ERR_INIT_WAKEUP, No correct answer for wakeup frame
  *                          ERR_INIT_BOARD_IDENTIFICATION, Expected number of boards is wrong
  *                          ERR_UNEXPECTED_FRAME unexpected frame received
  *                          ERR_ERROR_FRAME unexpected frame received
  *                          ERR_WRONG_CRC
  *                          ERR_ERR_IN_STATUS
  *                           DONE, WAKEUP_OK Answer of wakeup command is ok
  */
uint16_t L9502_Init(void){
    //uint32_t capturedRealTime;
    uint8_t i;
    uint16_t status = NO_ERROR;
    uint16_t error = NO_ERROR;
    //uint16_t id0 = 0;
    //uint16_t id1 = 0;
    
    //DSPI peripheral Initialization
    // add SPI SDK dspi_init2CTARs(&L9502_DSPI_Config);


/*===========================================================================*/
/* SPI baud rate calculation                                                 */
/*===========================================================================*/
/* CLOCK_SPI1CLK = CLOCK_PCLK2 = CLOCK_SYSCLKDIV / 2 =                       */
/* = [CLOCK_SYSCLK / (CLOCK_CFG_SYSPRE_VALUE + 1)] / 2 =                     */
/* = [300MHz / (0 + 1)] / 2 = 150MHz                                         */
/*                                                                           */
/* SPI_CR1_BR = DIV32 -> Baud Rate = CLOCK_SPI1CLK / 256 = 150MHz / 256 = 0.5859375MHz  */
/*===========================================================================*/
    
    spi_dma_conf_t spi_master_dma_conf;
    
    /* Configure SPI Master DMA.*/
    spi_master_dma_conf.dma_stream_tx_id = DMA1_STREAM0_ID;
    spi_master_dma_conf.dma_stream_rx_id = DMA1_STREAM1_ID;
    spi_master_dma_conf.dma_stream_bus_prio = DMA_PRIORITY_MAX;
    spi_master_dma_conf.dma_stream_irq_prio = IRQ_PRIORITY_2;

    /* Initialize SPI Master driver instance.*/
    (void)spi_init(&DRV_SPI1);

    /* Configure SPI Master driver instance.*/
    //(void)spi_set_drv_mode(&DRV_SPI1, SPI_DRV_MODE_ASYNCHRONOUS);
    (void)spi_set_drv_mode(&DRV_SPI1, SPI_DRV_MODE_DMA_SYNC);
    (void)spi_set_dma_conf(&DRV_SPI1, &spi_master_dma_conf);
    (void)spi_set_mode(&DRV_SPI1, SPI_MODE_MASTER);
    (void)spi_set_baud(&DRV_SPI1, SPI_BAUDRATE_DIV256);
    (void)spi_set_cpha(&DRV_SPI1, SPI_CPHA_0);
    (void)spi_set_cpol(&DRV_SPI1, SPI_CPOL_0);
    (void)spi_set_fformat(&DRV_SPI1, SPI_FRAMEFORMAT_MSB);
    (void)spi_set_fsize(&DRV_SPI1, SPI_FRAMESIZE_8);

    //CRC Init
    tabcrcInit(&L9502crc_Config);

    /* Apply SPI Master driver instance configuration.*/
    spi_start(&DRV_SPI1);    
//    gpio_set_pin(SPI1_NSS);  /* NSS software mode */

//------------------------------------------------------------------------------
//    Global Registers Initialization
//------------------------------------------------------------------------------
    /* clear internal data */
    g_internalData.testMode = false;
    g_internalData.faultFlag = false;
    g_internalData.initStatus = false;
    for(i=0; i< L9502Config.NumberOfDevices; i++){
        g_internalData.opMode[i] = SAFE_OFF;
        g_internalData.frameCounter[i] = 0;
    }
//gpio_set_pin(SPI1_NSS);    
    /* Wait for power up of a traction inverter board,
     * decrease at your convenience if you are using a different board */
    /*osal_delay_microsec(1000); */ dwt_reset(); dwt_delay_ticks(dwt_us2t(1000));
//gpio_clear_pin(SPI1_NSS);    
    /* Configure devices with settled parameters */
    status = L9502_DevicesConfiguration();
    if(NO_ERROR != status) /* Only write error are possible */
    {
      /* Trap */
      // while (1); /* To be changed with Development Error Reporting */
	  L9502_init = ERR_SPI_ERROR;
    };

    /* Verify if device has been programmed */
    status = L9502_IsDevicesProgrammed();
    for(i=0; i< L9502Config.NumberOfDevices; i++){
      if(L9502_ERRstatus[i].R != 0)
        error = 1;
    }
    if( (NO_ERROR != error) || (NO_ERROR != status) ) /* Read / write error are possible */
    {
      /* Trap */
      //while (1); /* To be changed with Development Error Reporting */
	  L9502_init = ERR_SPI_ERROR;
    };

    /* CUT CA:L9502_ClearFaults SPI Clear status registers using GLOBAL_LATCH_CLEAR in CFG_7
     *        L9502_ReadStatusReg to update g_status for LiveMonitor
     *        than DIS falling edge to go in Normal Mode */
    /* CUT BA:L9502_ClearFaults SPI Read to clear all status registers than DIS falling edge to go in Normal Mode*/
    status = L9502_ClearFaults();
#if defined(VERSION_CA) || defined(VERSION_CD)
    status = L9502_ReadStatusReg();
#endif
    for(i=0; i< L9502Config.NumberOfDevices; i++){
      if(L9502_ERRstatus[i].R != 0)
        error = 1;
    }
    if( (NO_ERROR != error) || (NO_ERROR != status) ) /* Read / write error are possible */
    {
      /* Trap */
      //while (1); /* To be changed with Development Error Reporting */
	  L9502_init = ERR_SPI_ERROR;
    };

    /* Get operational mode */
    for(i=0; i< L9502Config.NumberOfDevices; i++){
      status |= L9502_UpdateOpMode(i);
    }
    if(status != 0) {
        return status;
    }
    for(i=0; i< L9502Config.NumberOfDevices; i++){
      if(g_internalData.opMode[i] != NORMAL) {
          return ERR_IN_STATUS;
      }
    }

    /* True when the initialization was correct */
    g_internalData.initStatus = true;

    /*
    do{
      id0 = L9502_GetChipID(0);
      id1 = L9502_GetChipID(1);
    }
    while ( (id0 != CHIP_ID_LV_VALUE) && (id1 != CHIP_ID_LV_VALUE) );
    */

    return status;
}

#ifdef L9502_SOFT_START_SW
 /**
  * @brief This function configure device gate driver
  * @param void
  * @return status message   
  */
uint16_t L9502_DevicesConfiguration(void)
{
  uint16_t status = NO_ERROR; /* No Errors */
  uint8_t device;

#if defined(MC_SW_BATP)
  /* Switch Battery Protection ON */
  pal_lld_setpad(PORT_MC_SW_BATP, MC_SW_BATP);
  /*osal_delay_microsec(200);*/ dwt_reset(); dwt_delay_ticks(dwt_us2t(200));
#endif

  /* Start configuring LV side */
  for(device=0; device< L9502Config.NumberOfDevices; device++){
    /* Configure single device */
    L9502_EnterConfigMode(device);
    /*osal_delay_microsec(20); */ dwt_reset(); dwt_delay_ticks(dwt_us2t(20));
    /* FLY_EN = 1 in CFG_8 (0x12) */
    status = L9502_WriteSingleDev(device, ADDR_CFG_8, L9502_VALUE_CFG_8, L9502_frame_received);
    /*osal_delay_microsec(20); */ dwt_reset(); dwt_delay_ticks(dwt_us2t(20));
    status = L9502_WriteSingleDev(device, ADDR_CFG_9, L9502_VALUE_CFG_9, L9502_frame_received);
    /*osal_delay_microsec(20); */ dwt_reset(); dwt_delay_ticks(dwt_us2t(20));
    L9502_ExitConfigMode(device);
  }

  // *************   HIGH VOLTAGE CONFIGURATION ********** //
  /* Wait 20 ms between LV configuration and HV configuration to be sure FLY BACK is enabled */
  /*osal_delay_microsec(20); */ dwt_reset(); dwt_delay_ticks(dwt_us2t(20));

  /* Start configuring HV Side*/
  /* High Voltage side SRR registers are protected with crc check */
  /* input in the look up table is value of bits [9:3] of SRR register*/
  uint8_t crc0=crcTbl[L9502_VALUE_CFG_0>>3];
  uint8_t crc1=crcTbl[L9502_VALUE_CFG_1>>3];
  for(device=0; device< L9502Config.NumberOfDevices; device++){

    /* Configure HS device */
    L9502_EnterConfigMode(device);
    /* Configure all protections in CFG_0 */
    /*osal_delay_microsec(200);*/ dwt_reset(); dwt_delay_ticks(dwt_us2t(200));
    status = L9502_WriteSingleDev(device, ADDR_CFG_0, (L9502_VALUE_CFG_0 | crc0), L9502_frame_received);
    /*osal_delay_microsec(200);*/ dwt_reset(); dwt_delay_ticks(dwt_us2t(200));
    status = L9502_WriteSingleDev(device, ADDR_CFG_1, (L9502_VALUE_CFG_1 | crc1), L9502_frame_received);
    /*osal_delay_microsec(200);*/ dwt_reset(); dwt_delay_ticks(dwt_us2t(200));

    status = L9502_WriteSingleDev(device, ADDR_CFG_2, (L9502_VALUE_CFG_2), L9502_frame_received);
    /*osal_delay_microsec(200);*/ dwt_reset(); dwt_delay_ticks(dwt_us2t(200));
    status = L9502_WriteSingleDev(device, ADDR_CFG_3, (L9502_VALUE_CFG_3), L9502_frame_received);
    /*osal_delay_microsec(200);*/ dwt_reset(); dwt_delay_ticks(dwt_us2t(200));
    status = L9502_WriteSingleDev(device, ADDR_CFG_4, (L9502_VALUE_CFG_4), L9502_frame_received);
    /*osal_delay_microsec(200);*/ dwt_reset(); dwt_delay_ticks(dwt_us2t(200));
    status = L9502_WriteSingleDev(device, ADDR_CFG_5, (L9502_VALUE_CFG_5), L9502_frame_received);
    /*osal_delay_microsec(200);*/ dwt_reset(); dwt_delay_ticks(dwt_us2t(200));
    status = L9502_WriteSingleDev(device, ADDR_CFG_6, (L9502_VALUE_CFG_6), L9502_frame_received);
    /*osal_delay_microsec(200);*/ dwt_reset(); dwt_delay_ticks(dwt_us2t(200));

#if defined(VERSION_BA) || defined(VERSION_CA) || defined(VERSION_CD)
    status = L9502_WriteSingleDev(device, ADDR_CFG_10, L9502_VALUE_CFG_10_P1, L9502_frame_received);
    /*osal_delay_microsec(200);*/ dwt_reset(); dwt_delay_ticks(dwt_us2t(200));
    status = L9502_WriteSingleDev(device, ADDR_CFG_10, L9502_VALUE_CFG_10_P3, L9502_frame_received);
    /*osal_delay_microsec(200);*/ dwt_reset(); dwt_delay_ticks(dwt_us2t(200));
    status = L9502_WriteSingleDev(device, ADDR_CFG_11, L9502_VALUE_CFG_11_P1, L9502_frame_received);
    /*osal_delay_microsec(200);*/ dwt_reset(); dwt_delay_ticks(dwt_us2t(200));
    status = L9502_WriteSingleDev(device, ADDR_CFG_11, L9502_VALUE_CFG_11_P2, L9502_frame_received);
    /*osal_delay_microsec(200);*/ dwt_reset(); dwt_delay_ticks(dwt_us2t(200));
    status = L9502_WriteSingleDev(device, ADDR_CFG_11, L9502_VALUE_CFG_11_P3, L9502_frame_received);
    /*osal_delay_microsec(200);*/ dwt_reset(); dwt_delay_ticks(dwt_us2t(200));
    status = L9502_WriteSingleDev(device, ADDR_STO_CFG, L9502_VALUE_STO_CFG, L9502_frame_received);
    /*osal_delay_microsec(200);*/ dwt_reset(); dwt_delay_ticks(dwt_us2t(200));
#endif
    L9502_ExitConfigMode(device);
    /*osal_delay_microsec(200);*/ dwt_reset(); dwt_delay_ticks(dwt_us2t(200));

#if defined(VERSION_BA) || defined(VERSION_CA) || defined(VERSION_CD)
    status = L9502_WriteSingleDev(device, ADDR_CFG_10, L9502_VALUE_CFG_10_P2, L9502_frame_received);
    /*osal_delay_microsec(200);*/ dwt_reset(); dwt_delay_ticks(dwt_us2t(200));
#endif
    status = L9502_WriteSingleDev(device, ADDR_CFG_7, (L9502_VALUE_CFG_7), L9502_frame_received);
    /*osal_delay_microsec(200);*/ dwt_reset(); dwt_delay_ticks(dwt_us2t(200));
  }

  return status;
}
#else /* All device configured at once */
uint16_t L9502_DevicesConfiguration(void)
{
  uint16_t errors = NO_ERROR; /* No Errors */
  uint8_t i;

  /* Start configuring HV Side*/
  /* High Voltage side SRR registers are protected with crc check */
  /* input in the look up table is value of bits [9:3] of SRR register*/
  uint8_t crc0=crcTbl[L9502_VALUE_CFG_0>>3];
  uint8_t crc1=crcTbl[L9502_VALUE_CFG_1>>3];

#if defined(MC_SW_BATP)
  /* Switch Battery Protection ON */
  //pal_lld_setpad(PORT_MC_SW_BATP, MC_SW_BATP);
  gpio_set_pin(MC_SW_BATP);
  /*osal_delay_microsec(200);*/ dwt_reset(); dwt_delay_ticks(dwt_us2t(200));
#endif

  /* Semi-unlock */
  for(i=0; i< L9502Config.NumberOfDevices; i++)
  {
      L9502_frame[i].R = L9502_R_W(L9502_R_W_WRITE) | L9502_ADDRESS(ADDR_UNLOCK_KEY) | L9502_DATA(CFG_ENTER_1);
  }
  errors = L9502_SendFrame(L9502_frame, L9502_frame_received);
  /*Only for first SPI transfer, In case of frame counter error, align Frame Counter */
  if((errors & ERR_COUNTER_ERROR) == ERR_COUNTER_ERROR)
  {
    /* Clear Flag as the configuration start now */
    errors = 0;
    //L9502_ERRstatus.B.CRC_ERROR = 0;
    for(i=0; i< L9502Config.NumberOfDevices; i++)
      L9502_ERRstatus[i].R = 0; /* in case of frame counter problems for any devices */

    /*osal_delay_microsec(L9502_WAIT_LV); */ dwt_reset(); dwt_delay_ticks(dwt_us2t(L9502_WAIT_LV));
    errors = L9502_SendFrame(L9502_frame, L9502_frame_received);
  }
  /*osal_delay_microsec(L9502_WAIT_LV); */ dwt_reset(); dwt_delay_ticks(dwt_us2t(L9502_WAIT_LV));

  if (!errors)
  {
    /* Unlock */
    for(i=0; i< L9502Config.NumberOfDevices; i++)
    {
      L9502_frame[i].R = L9502_R_W(L9502_R_W_WRITE) | L9502_ADDRESS(ADDR_UNLOCK_KEY) | L9502_DATA(CFG_ENTER_2);
    }
    errors = L9502_SendFrame(L9502_frame, L9502_frame_received);
  }
  /*osal_delay_microsec(L9502_WAIT_LV); */ dwt_reset(); dwt_delay_ticks(dwt_us2t(L9502_WAIT_LV));;

  if (!errors)
  {
    /* FLY_EN = 1 in CFG_8 (0x12) */
    for(i=0; i< L9502Config.NumberOfDevices; i++)
    {
      L9502_frame[i].R = L9502_R_W(L9502_R_W_WRITE) | L9502_ADDRESS(ADDR_CFG_8) | L9502_DATA(L9502_VALUE_CFG_8);
    }
    errors = L9502_SendFrame(L9502_frame, L9502_frame_received);
  }
  /*osal_delay_microsec(L9502_WAIT_LV); */ dwt_reset(); dwt_delay_ticks(dwt_us2t(L9502_WAIT_LV));;

  if (!errors)
  {
    /* Dead time and redirection in CFG_9 (0x13) */
    for(i=0; i< L9502Config.NumberOfDevices; i++)
    {
      L9502_frame[i].R = L9502_R_W(L9502_R_W_WRITE) | L9502_ADDRESS(ADDR_CFG_9) | L9502_DATA(L9502_VALUE_CFG_9);
    }
    errors = L9502_SendFrame(L9502_frame, L9502_frame_received);
  }
  /*osal_delay_microsec(L9502_WAIT_LV); */ dwt_reset(); dwt_delay_ticks(dwt_us2t(L9502_WAIT_LV));;

  if (!errors)
  {
    /* Lock */
    for(i=0; i< L9502Config.NumberOfDevices; i++)
    {
      L9502_frame[i].R = L9502_R_W(L9502_R_W_WRITE) | L9502_ADDRESS(ADDR_UNLOCK_KEY) | L9502_DATA(CFG_LEAVE);
    }
    errors = L9502_SendFrame(L9502_frame, L9502_frame_received);
  }

  // *************   HIGH VOLTAGE CONFIGURATION ********** //
  /* Wait 20 ms between LV configuration and HV configuration to be sure FLY BACK is enabled */
  /*osal_delay_microsec(20); */ dwt_reset(); dwt_delay_ticks(dwt_us2t(20));

  if (!errors)
  {
    /* Semi-unlock */
    for(i=0; i< L9502Config.NumberOfDevices; i++)
    {
      L9502_frame[i].R = L9502_R_W(L9502_R_W_WRITE) | L9502_ADDRESS(ADDR_UNLOCK_KEY) | L9502_DATA(CFG_ENTER_1);
    }
    errors = L9502_SendFrame(L9502_frame, L9502_frame_received);
  }
  /*osal_delay_microsec(L9502_WAIT_HV); */ dwt_reset(); dwt_delay_ticks(dwt_us2t(L9502_WAIT_HV));;

  if (!errors)
  {
    /* Unlock */
    for(i=0; i< L9502Config.NumberOfDevices; i++)
    {
      L9502_frame[i].R = L9502_R_W(L9502_R_W_WRITE) | L9502_ADDRESS(ADDR_UNLOCK_KEY) | L9502_DATA(CFG_ENTER_2);
    }
    errors = L9502_SendFrame(L9502_frame, L9502_frame_received);
  }
  /*osal_delay_microsec(L9502_WAIT_HV); */ dwt_reset(); dwt_delay_ticks(dwt_us2t(L9502_WAIT_HV));;

  if (!errors)
  {
    /* Disable all protection in CFG_0 (0x0) */
    for(i=0; i< L9502Config.NumberOfDevices; i++)
    {
      L9502_frame[i].R = L9502_R_W(L9502_R_W_WRITE) | L9502_ADDRESS(ADDR_CFG_0) | L9502_DATA(L9502_VALUE_CFG_0 | crc0);
    }
    errors = L9502_SendFrame(L9502_frame, L9502_frame_received);
  }
  /*osal_delay_microsec(L9502_WAIT_HV); */ dwt_reset(); dwt_delay_ticks(dwt_us2t(L9502_WAIT_HV));;

  if (!errors)
  {
    /* Reactions in CFG_1 (0x1) */
    for(i=0; i< L9502Config.NumberOfDevices; i++)
    {
      L9502_frame[i].R = L9502_R_W(L9502_R_W_WRITE) | L9502_ADDRESS(ADDR_CFG_1) | L9502_DATA(L9502_VALUE_CFG_1 | crc1);
    }
    errors = L9502_SendFrame(L9502_frame, L9502_frame_received);
  }
  /*osal_delay_microsec(L9502_WAIT_HV); */ dwt_reset(); dwt_delay_ticks(dwt_us2t(L9502_WAIT_HV));;

  if (!errors)
  {
    /* Threshold in CFG_2 (0x2) */
    for(i=0; i< L9502Config.NumberOfDevices; i++)
    {
      L9502_frame[i].R = L9502_R_W(L9502_R_W_WRITE) | L9502_ADDRESS(ADDR_CFG_2) | L9502_DATA(L9502_VALUE_CFG_2);
    }
    errors = L9502_SendFrame(L9502_frame, L9502_frame_received);
  }
  /*osal_delay_microsec(200);*/ dwt_reset(); dwt_delay_ticks(dwt_us2t(200));

  if (!errors)
  {
    /* Threshold in CFG_3 (0x3) */
    for(i=0; i< L9502Config.NumberOfDevices; i++)
    {
      L9502_frame[i].R = L9502_R_W(L9502_R_W_WRITE) | L9502_ADDRESS(ADDR_CFG_3) | L9502_DATA(L9502_VALUE_CFG_3);
    }
    errors = L9502_SendFrame(L9502_frame, L9502_frame_received);
  }
  /*osal_delay_microsec(L9502_WAIT_HV); */ dwt_reset(); dwt_delay_ticks(dwt_us2t(L9502_WAIT_HV));;

  if (!errors)
  {
    /* Threshold in CFG_4 (0x4) */
    for(i=0; i< L9502Config.NumberOfDevices; i++)
    {
      L9502_frame[i].R = L9502_R_W(L9502_R_W_WRITE) | L9502_ADDRESS(ADDR_CFG_4) | L9502_DATA(L9502_VALUE_CFG_4);
    }
    errors = L9502_SendFrame(L9502_frame, L9502_frame_received);
  }
  /*osal_delay_microsec(L9502_WAIT_HV); */ dwt_reset(); dwt_delay_ticks(dwt_us2t(L9502_WAIT_HV));;

  if (!errors)
  {
    /* Threshold in CFG_5 (0x5) */
    for(i=0; i< L9502Config.NumberOfDevices; i++)
    {
      L9502_frame[i].R = L9502_R_W(L9502_R_W_WRITE) | L9502_ADDRESS(ADDR_CFG_5) | L9502_DATA(L9502_VALUE_CFG_5);
    }
    errors = L9502_SendFrame(L9502_frame, L9502_frame_received);
  }
  /*osal_delay_microsec(L9502_WAIT_HV); */ dwt_reset(); dwt_delay_ticks(dwt_us2t(L9502_WAIT_HV));;

  if (!errors)
  {
    /* Threshold in CFG_6 (0x6) */
    for(i=0; i< L9502Config.NumberOfDevices; i++)
    {
      L9502_frame[i].R = L9502_R_W(L9502_R_W_WRITE) | L9502_ADDRESS(ADDR_CFG_6) | L9502_DATA(L9502_VALUE_CFG_6);
    }
    errors = L9502_SendFrame(L9502_frame, L9502_frame_received);
  }
  /*osal_delay_microsec(200);*/ dwt_reset(); dwt_delay_ticks(dwt_us2t(200));

#if defined(VERSION_BA) || defined(VERSION_CA) || defined(VERSION_CD)

  if (!errors)
  {
    /* Threshold in CFG_10 (0x0B) Page 1*/
    for(i=0; i< L9502Config.NumberOfDevices; i++)
    {
      L9502_frame[i].R = L9502_R_W(L9502_R_W_WRITE) | L9502_ADDRESS(ADDR_CFG_10) | L9502_DATA(L9502_VALUE_CFG_10_P1);
    }
    errors = L9502_SendFrame(L9502_frame, L9502_frame_received);
  }
  /*osal_delay_microsec(L9502_WAIT_HV); */ dwt_reset(); dwt_delay_ticks(dwt_us2t(L9502_WAIT_HV));;

  if (!errors)
  {
    /* Threshold in CFG_10 (0x0B) Page 3*/
    for(i=0; i< L9502Config.NumberOfDevices; i++)
    {
      L9502_frame[i].R = L9502_R_W(L9502_R_W_WRITE) | L9502_ADDRESS(ADDR_CFG_10) | L9502_DATA(L9502_VALUE_CFG_10_P3);
    }
    errors = L9502_SendFrame(L9502_frame, L9502_frame_received);
  }
  /*osal_delay_microsec(L9502_WAIT_HV); */ dwt_reset(); dwt_delay_ticks(dwt_us2t(L9502_WAIT_HV));;

  if (!errors)
  {
    /* Threshold in CFG_11 (0x0F) Page 1*/
    for(i=0; i< L9502Config.NumberOfDevices; i++)
    {
      L9502_frame[i].R = L9502_R_W(L9502_R_W_WRITE) | L9502_ADDRESS(ADDR_CFG_11) | L9502_DATA(L9502_VALUE_CFG_11_P1);
    }
    errors = L9502_SendFrame(L9502_frame, L9502_frame_received);
  }
  /*osal_delay_microsec(L9502_WAIT_HV); */ dwt_reset(); dwt_delay_ticks(dwt_us2t(L9502_WAIT_HV));;

  if (!errors)
  {
    /* Threshold in CFG_11 (0x0F) Page 2*/
    for(i=0; i< L9502Config.NumberOfDevices; i++)
    {
      L9502_frame[i].R = L9502_R_W(L9502_R_W_WRITE) | L9502_ADDRESS(ADDR_CFG_11) | L9502_DATA(L9502_VALUE_CFG_11_P2);
    }
    errors = L9502_SendFrame(L9502_frame, L9502_frame_received);
  }
  /*osal_delay_microsec(L9502_WAIT_HV); */ dwt_reset(); dwt_delay_ticks(dwt_us2t(L9502_WAIT_HV));;

  if (!errors)
  {
    /* Threshold in CFG_11 (0x0F) Page 3*/
    for(i=0; i< L9502Config.NumberOfDevices; i++)
    {
      L9502_frame[i].R = L9502_R_W(L9502_R_W_WRITE) | L9502_ADDRESS(ADDR_CFG_11) | L9502_DATA(L9502_VALUE_CFG_11_P3);
    }
    errors = L9502_SendFrame(L9502_frame, L9502_frame_received);
  }
  /*osal_delay_microsec(L9502_WAIT_HV); */ dwt_reset(); dwt_delay_ticks(dwt_us2t(L9502_WAIT_HV));;

  if (!errors)
  {
    /* Threshold in STO_CFG (0x0E) */
    for(i=0; i< L9502Config.NumberOfDevices; i++)
    {
      L9502_frame[i].R = L9502_R_W(L9502_R_W_WRITE) | L9502_ADDRESS(ADDR_STO_CFG) | L9502_DATA(L9502_VALUE_STO_CFG);
    }
    errors = L9502_SendFrame(L9502_frame, L9502_frame_received);
  }
  /*osal_delay_microsec(L9502_WAIT_HV); */ dwt_reset(); dwt_delay_ticks(dwt_us2t(L9502_WAIT_HV));;

#endif


  if (!errors)
  {
    /* Lock */
    for(i=0; i< L9502Config.NumberOfDevices; i++)
    {
      L9502_frame[i].R = L9502_R_W(L9502_R_W_WRITE) | L9502_ADDRESS(ADDR_UNLOCK_KEY) | L9502_DATA(CFG_LEAVE);
    }
    errors = L9502_SendFrame(L9502_frame, L9502_frame_received);
  }
  /*osal_delay_microsec(L9502_WAIT_HV); */ dwt_reset(); dwt_delay_ticks(dwt_us2t(L9502_WAIT_HV));;

  /* Non safety relevant Registers, do not need to Unlock the registers */
  if (!errors)
  {
    /* Threshold in CFG_7 (0x7) */
    for(i=0; i< L9502Config.NumberOfDevices; i++)
    {
      L9502_frame[i].R = L9502_R_W(L9502_R_W_WRITE) | L9502_ADDRESS(ADDR_CFG_7) | L9502_DATA(L9502_VALUE_CFG_7);
    }
    errors = L9502_SendFrame(L9502_frame, L9502_frame_received);
  }
  /*osal_delay_microsec(L9502_WAIT_HV); */ dwt_reset(); dwt_delay_ticks(dwt_us2t(L9502_WAIT_HV));;

#if defined(VERSION_BA) || defined(VERSION_CA) || defined(VERSION_CD)

  if (!errors)
  {
    /* Threshold in CFG_10 (0x0B) Page 2*/
    for(i=0; i< L9502Config.NumberOfDevices; i++)
    {
      L9502_frame[i].R = L9502_R_W(L9502_R_W_WRITE) | L9502_ADDRESS(ADDR_CFG_10) | L9502_DATA(L9502_VALUE_CFG_10_P2);
    }
    errors = L9502_SendFrame(L9502_frame, L9502_frame_received);
}
  /*osal_delay_microsec(L9502_WAIT_HV); */ dwt_reset(); dwt_delay_ticks(dwt_us2t(L9502_WAIT_HV));;
#endif

  return errors;
}
#endif

 /**
  * @brief This function exec a multiple read on all configurable registers.
  *        In case the read device do not match with masked configuration
  *        set global variable L9502_ERRstatus
  *        Affected Global Variables:\n
  *        L9502_ERRstatus - SPI_ERROR
  *                          CRC_ERROR
  *                          LAST_FRAME_NOK
  *                          REG_READ_ERROR
  *                          ERROR_FRAME_RECEIVED
  * @param void
  * @return Programming Status
  *             error message (same as L9502_SendFrame):
  *             0 NO_ERROR
  *             1 ERR_WRONG_CRC
  *             2 ERR_COUNTER_ERROR
  *             3 ERR_SPI_ERROR
  */
uint16_t L9502_IsDevicesProgrammed(void)
{
  uint8_t dev_num;
  uint16_t status_lv = NO_ERROR;
  uint16_t status_hv = NO_ERROR;
  L9502_Frame_t readRegValues[L9502_DEVICES_MAX];

  /* Read Low Voltage Register to verify the configuration has been applied */
  status_lv = L9502_ReadMultipleDev(L9502Config.NumberOfDevices, ADDR_CFG_8, readRegValues);
  for (dev_num = 0; dev_num < L9502Config.NumberOfDevices; dev_num++)
  {
    if ((L9502Config.deviceParams[dev_num]->hCFG_8 & CFG_REG_MASK) != readRegValues[dev_num].B.DATA )
    {
      L9502_ERRstatus[dev_num].B.REG_READ_ERROR = 1;
      L9502_ERRstatus[dev_num].B.ERROR_FRAME_RECEIVED = 1;
      break;
    }
  }
  /*osal_delay_microsec(L9502_WAIT_LV); */ dwt_reset(); dwt_delay_ticks(dwt_us2t(L9502_WAIT_LV));;
  status_lv = L9502_ReadMultipleDev(L9502Config.NumberOfDevices, ADDR_CFG_9, readRegValues);
  for (dev_num = 0; dev_num < L9502Config.NumberOfDevices; dev_num++)
  {
    if ((L9502Config.deviceParams[dev_num]->hCFG_9 & CFG_REG_MASK) != readRegValues[dev_num].B.DATA )
    {
      L9502_ERRstatus[dev_num].B.REG_READ_ERROR = 1;
      L9502_ERRstatus[dev_num].B.ERROR_FRAME_RECEIVED = 1;
      break;
    }
  }
  /*osal_delay_microsec(L9502_WAIT_LV); */ dwt_reset(); dwt_delay_ticks(dwt_us2t(L9502_WAIT_LV));;

  /* Only if low voltage has been well configured check High voltage registers */
  if (!status_lv)
  {
    /* Read High Voltage Register to verify the configuration has been applied */
    status_hv = L9502_ReadMultipleDev(L9502Config.NumberOfDevices, ADDR_CFG_0, readRegValues);
    for (dev_num = 0; dev_num < L9502Config.NumberOfDevices; dev_num++)
    {
      if ((L9502Config.deviceParams[dev_num]->hCFG_0 & CFG_REG_MASK) != (readRegValues[dev_num].B.DATA & CRC_DATA_REG_MASK) )
      {
        L9502_ERRstatus[dev_num].B.REG_READ_ERROR = 1;
        L9502_ERRstatus[dev_num].B.ERROR_FRAME_RECEIVED = 1;
        break;
      }
    } // Number of devices
    /*osal_delay_microsec(L9502_WAIT_HV); */ dwt_reset(); dwt_delay_ticks(dwt_us2t(L9502_WAIT_HV));;

    if (!status_hv)
    {
      /* Read High Voltage Register to verify the configuration has been applied */
      status_hv = L9502_ReadMultipleDev(L9502Config.NumberOfDevices, ADDR_CFG_1, readRegValues);
      for (dev_num = 0; dev_num < L9502Config.NumberOfDevices; dev_num++)
      {
        if ((L9502Config.deviceParams[dev_num]->hCFG_1 & CFG_REG_MASK) != (readRegValues[dev_num].B.DATA & CRC_DATA_REG_MASK) )
        {
          L9502_ERRstatus[dev_num].B.REG_READ_ERROR = 1;
          L9502_ERRstatus[dev_num].B.ERROR_FRAME_RECEIVED = 1;
          break;
        }
      } // Number of devices
      /*osal_delay_microsec(L9502_WAIT_HV); */ dwt_reset(); dwt_delay_ticks(dwt_us2t(L9502_WAIT_HV));;
    }

    if (!status_hv)
    {
      /* Read High Voltage Register to verify the configuration has been applied */
      status_hv = L9502_ReadMultipleDev(L9502Config.NumberOfDevices, ADDR_CFG_2, readRegValues);
      for (dev_num = 0; dev_num < L9502Config.NumberOfDevices; dev_num++)
      {
        if ((L9502Config.deviceParams[dev_num]->hCFG_2 & CFG_REG_MASK) != (readRegValues[dev_num].B.DATA) )
        {
          L9502_ERRstatus[dev_num].B.REG_READ_ERROR = 1;
          L9502_ERRstatus[dev_num].B.ERROR_FRAME_RECEIVED = 1;
          break;
        }
      } // Number of devices
      /*osal_delay_microsec(L9502_WAIT_HV); */ dwt_reset(); dwt_delay_ticks(dwt_us2t(L9502_WAIT_HV));;
    }

    if (!status_hv)
    {
      /* Read High Voltage Register to verify the configuration has been applied */
      status_hv = L9502_ReadMultipleDev(L9502Config.NumberOfDevices, ADDR_CFG_3, readRegValues);
      for (dev_num = 0; dev_num < L9502Config.NumberOfDevices; dev_num++)
      {
        if ((L9502Config.deviceParams[dev_num]->hCFG_3 & CFG_REG_MASK) != (readRegValues[dev_num].B.DATA) )
        {
          L9502_ERRstatus[dev_num].B.REG_READ_ERROR = 1;
          L9502_ERRstatus[dev_num].B.ERROR_FRAME_RECEIVED = 1;
          break;
        }
      } // Number of devices
      /*osal_delay_microsec(L9502_WAIT_HV); */ dwt_reset(); dwt_delay_ticks(dwt_us2t(L9502_WAIT_HV));;
    }

    if (!status_hv)
    {
      /* Read High Voltage Register to verify the configuration has been applied */
      status_hv = L9502_ReadMultipleDev(L9502Config.NumberOfDevices, ADDR_CFG_4, readRegValues);
      for (dev_num = 0; dev_num < L9502Config.NumberOfDevices; dev_num++)
      {
        if ((L9502Config.deviceParams[dev_num]->hCFG_4 & CFG_REG_MASK) != (readRegValues[dev_num].B.DATA) )
        {
          L9502_ERRstatus[dev_num].B.REG_READ_ERROR = 1;
          L9502_ERRstatus[dev_num].B.ERROR_FRAME_RECEIVED = 1;
          break;
        }
      } // Number of devices
      /*osal_delay_microsec(L9502_WAIT_HV); */ dwt_reset(); dwt_delay_ticks(dwt_us2t(L9502_WAIT_HV));;
    }

    if (!status_hv)
    {
      /* Read High Voltage Register to verify the configuration has been applied */
      status_hv = L9502_ReadMultipleDev(L9502Config.NumberOfDevices, ADDR_CFG_5, readRegValues);
      for (dev_num = 0; dev_num < L9502Config.NumberOfDevices; dev_num++)
      {
        if ((L9502Config.deviceParams[dev_num]->hCFG_5 & CFG5_REG_MASK) != (readRegValues[dev_num].B.DATA & CFG5_REG_MASK) )
        {
          L9502_ERRstatus[dev_num].B.REG_READ_ERROR = 1;
          L9502_ERRstatus[dev_num].B.ERROR_FRAME_RECEIVED = 1;
          break;
        }
      } // Number of devices
      /*osal_delay_microsec(L9502_WAIT_HV); */ dwt_reset(); dwt_delay_ticks(dwt_us2t(L9502_WAIT_HV));;
    }

    if (!status_hv)
    {
      /* Read High Voltage Register to verify the configuration has been applied */
      status_hv = L9502_ReadMultipleDev(L9502Config.NumberOfDevices, ADDR_CFG_6, readRegValues);
      for (dev_num = 0; dev_num < L9502Config.NumberOfDevices; dev_num++)
      {
        if ((L9502Config.deviceParams[dev_num]->hCFG_6 & CFG_REG_MASK) != (readRegValues[dev_num].B.DATA) )
        {
          L9502_ERRstatus[dev_num].B.REG_READ_ERROR = 1;
          L9502_ERRstatus[dev_num].B.ERROR_FRAME_RECEIVED = 1;
          break;
        }
      } // Number of devices
      /*osal_delay_microsec(L9502_WAIT_HV); */ dwt_reset(); dwt_delay_ticks(dwt_us2t(L9502_WAIT_HV));;
    }

#ifdef VERSION_AA
    /* on BA version 1 bit is write only, cannot be read */
    if (!status_hv)
    {
      /* Read High Voltage Register to verify the configuration has been applied */
      status_hv = L9502_ReadMultipleDev(L9502Config.NumberOfDevices, ADDR_CFG_7, readRegValues);
      for (dev_num = 0; dev_num < L9502Config.NumberOfDevices; dev_num++)
      {
        if ((L9502Config.deviceParams[dev_num]->hCFG_7 & CFG_REG_MASK) != (readRegValues[dev_num].B.DATA & CFG7_REG_MASK) )
        {
          L9502_ERRstatus[dev_num].B.REG_READ_ERROR = 1;
          L9502_ERRstatus[dev_num].B.ERROR_FRAME_RECEIVED = 1;
          break;
        }
      } // Number of devices
      /*osal_delay_microsec(L9502_WAIT_HV); */ dwt_reset(); dwt_delay_ticks(dwt_us2t(L9502_WAIT_HV));;
    }
#endif

#if defined(VERSION_BA) || defined(VERSION_CA) || defined(VERSION_CD)
    if (!status_hv)
    {
      /* Read High Voltage Register to verify the configuration has been applied */
      status_hv = L9502_ReadPageMultipleDev(L9502Config.NumberOfDevices, ADDR_CFG_10, L9502_PAGE_1, readRegValues);
      for (dev_num = 0; dev_num < L9502Config.NumberOfDevices; dev_num++)
      {
        if ((L9502Config.deviceParams[dev_num]->hCFG_10_P1 & CFG_REG_MASK) != readRegValues[dev_num].B.DATA )
        {
          L9502_ERRstatus[dev_num].B.REG_READ_ERROR = 1;
          L9502_ERRstatus[dev_num].B.ERROR_FRAME_RECEIVED = 1;
          break;
        }
      } // Number of devices
      /*osal_delay_microsec(L9502_WAIT_HV); */ dwt_reset(); dwt_delay_ticks(dwt_us2t(L9502_WAIT_HV));;
    }
    if (!status_hv)
    {
      /* Read High Voltage Register to verify the configuration has been applied */
      status_hv = L9502_ReadPageMultipleDev(L9502Config.NumberOfDevices, ADDR_CFG_10, L9502_PAGE_2, readRegValues);
      for (dev_num = 0; dev_num < L9502Config.NumberOfDevices; dev_num++)
      {
        if ((L9502Config.deviceParams[dev_num]->hCFG_10_P2 & CFG_REG_MASK) != readRegValues[dev_num].B.DATA )
        {
          L9502_ERRstatus[dev_num].B.REG_READ_ERROR = 1;
          L9502_ERRstatus[dev_num].B.ERROR_FRAME_RECEIVED = 1;
          break;
        }
      } // Number of devices
      /*osal_delay_microsec(L9502_WAIT_HV); */ dwt_reset(); dwt_delay_ticks(dwt_us2t(L9502_WAIT_HV));;
    }
    if (!status_hv)
    {
      /* Read High Voltage Register to verify the configuration has been applied */
      status_hv = L9502_ReadPageMultipleDev(L9502Config.NumberOfDevices, ADDR_CFG_10, L9502_PAGE_3, readRegValues);
      for (dev_num = 0; dev_num < L9502Config.NumberOfDevices; dev_num++)
      {
        if ((L9502Config.deviceParams[dev_num]->hCFG_10_P3 & CFG_REG_MASK) != readRegValues[dev_num].B.DATA )
        {
          L9502_ERRstatus[dev_num].B.REG_READ_ERROR = 1;
          L9502_ERRstatus[dev_num].B.ERROR_FRAME_RECEIVED = 1;
          break;
        }
      } // Number of devices
      /*osal_delay_microsec(L9502_WAIT_HV); */ dwt_reset(); dwt_delay_ticks(dwt_us2t(L9502_WAIT_HV));;
    }

    if (!status_hv)
    {
      /* Read High Voltage Register to verify the configuration has been applied */
      status_hv = L9502_ReadPageMultipleDev(L9502Config.NumberOfDevices, ADDR_CFG_11, L9502_PAGE_1, readRegValues);
      for (dev_num = 0; dev_num < L9502Config.NumberOfDevices; dev_num++)
      {
        if ((L9502Config.deviceParams[dev_num]->hCFG_11_P1 & CFG_REG_MASK) != readRegValues[dev_num].B.DATA )
        {
          L9502_ERRstatus[dev_num].B.REG_READ_ERROR = 1;
          L9502_ERRstatus[dev_num].B.ERROR_FRAME_RECEIVED = 1;
          break;
        }
      } // Number of devices
      /*osal_delay_microsec(L9502_WAIT_HV); */ dwt_reset(); dwt_delay_ticks(dwt_us2t(L9502_WAIT_HV));;
    }
    if (!status_hv)
    {
      /* Read High Voltage Register to verify the configuration has been applied */
      status_hv = L9502_ReadPageMultipleDev(L9502Config.NumberOfDevices, ADDR_CFG_11, L9502_PAGE_2, readRegValues);
      for (dev_num = 0; dev_num < L9502Config.NumberOfDevices; dev_num++)
      {
        if ((L9502Config.deviceParams[dev_num]->hCFG_11_P2 & CFG_REG_MASK) != readRegValues[dev_num].B.DATA )
        {
          L9502_ERRstatus[dev_num].B.REG_READ_ERROR = 1;
          L9502_ERRstatus[dev_num].B.ERROR_FRAME_RECEIVED = 1;
          break;
        }
      } // Number of devices
      /*osal_delay_microsec(L9502_WAIT_HV); */ dwt_reset(); dwt_delay_ticks(dwt_us2t(L9502_WAIT_HV));;
    }
    if (!status_hv)
    {
      /* Read High Voltage Register to verify the configuration has been applied */
      status_hv = L9502_ReadPageMultipleDev(L9502Config.NumberOfDevices, ADDR_CFG_11, L9502_PAGE_3, readRegValues);
      for (dev_num = 0; dev_num < L9502Config.NumberOfDevices; dev_num++)
      {
        if ((L9502Config.deviceParams[dev_num]->hCFG_11_P3 & CFG_REG_MASK) != readRegValues[dev_num].B.DATA )
        {
          L9502_ERRstatus[dev_num].B.REG_READ_ERROR = 1;
          L9502_ERRstatus[dev_num].B.ERROR_FRAME_RECEIVED = 1;
          break;
        }
      } // Number of devices
      /*osal_delay_microsec(L9502_WAIT_HV); */ dwt_reset(); dwt_delay_ticks(dwt_us2t(L9502_WAIT_HV));;
    }

    if (!status_hv)
    {
      /* Read High Voltage Register to verify the configuration has been applied */
      status_hv = L9502_ReadMultipleDev(L9502Config.NumberOfDevices, ADDR_STO_CFG, readRegValues);
      for (dev_num = 0; dev_num < L9502Config.NumberOfDevices; dev_num++)
      {
        if ((L9502Config.deviceParams[dev_num]->hSTO_CFG & CFG_REG_MASK) != (readRegValues[dev_num].B.DATA & STO_CFG_REG_MASK) )
        {
          L9502_ERRstatus[dev_num].B.REG_READ_ERROR = 1;
          L9502_ERRstatus[dev_num].B.ERROR_FRAME_RECEIVED = 1;
          break;
        }
      } // Number of devices
      /*osal_delay_microsec(L9502_WAIT_HV); */ dwt_reset(); dwt_delay_ticks(dwt_us2t(L9502_WAIT_HV));;
    }
#endif

  }
  else
    return status_hv;

  return (status_lv);
}

#if defined(VERSION_CA) || defined(VERSION_CD)
 /**
  * @brief This function Write GLOBAL_LATCH_CLEAR in CFG_7 to clear all RLR field.
  *        New feature implemented in CUT CA
  *        To be used only after Flyback has been
  *        Affected Global Variables:\n
  *        L9502_ERRstatus - ERROR_FRAME_RECEIVED
  *                          CRC_ERR
  *                          LAST_FRAME_NOK
  * @param void
  * @return     error message (same as L9502_SendFrame):
  *             0 NO_ERROR
  *             1 ERR_WRONG_CRC
  *             2 ERR_COUNTER_ERROR
  *             3 ERR_SPI_ERROR
  */
uint16_t L9502_ClearFaults(void)
{
    uint16_t errors = NO_ERROR;
    uint8_t i;
    /* Non safety relevant Registers, do not need to Unlock the registers */
    /* Set GLOBAL_LATCH_CLEAR in CFG_7 to clear all RLR Field */
    for(i=0; i< L9502Config.NumberOfDevices; i++)
    {
      L9502_frame[i].R = L9502_R_W(L9502_R_W_WRITE) | L9502_ADDRESS(ADDR_CFG_7) | L9502_DATA(L9502_GLOBAL_LATCH_CLEAR);
    }
    errors = L9502_SendFrame(L9502_frame, L9502_frame_received);

    return errors;

}

uint16_t L9502_ReadStatusReg(void)
#else
uint16_t L9502_ClearFaults(void)
#endif
{
  uint16_t status;
  L9502_Frame_t readFault[L9502_DEVICES_MAX];
  uint8_t i;

  /* Read status Register to clear Fault */
  /* Read register FHM_2 */
  for(i=0; i< L9502_DEVICES_MAX; i++){
    readFault[i].R = L9502_R_W(L9502_R_W_READ) | L9502_ADDRESS(ADDR_CHIP_ID_LV) | L9502_DATA(0);
  }
  status = L9502_ReadMultipleDev(L9502Config.NumberOfDevices, ADDR_FHM_2, readFault);
  if(status)
  {
    for (i = 0; i < L9502Config.NumberOfDevices; i++)
    {
      L9502_ERRstatus[i].B.REG_READ_ERROR = 1;
      L9502_ERRstatus[i].B.ERROR_FRAME_RECEIVED = 1;
    }
  }
  else
  {
    for (i = 0; i < L9502Config.NumberOfDevices; i++)
    {
      g_statusReg[i].REGS.hFHM_2.R = (readFault[i].B.DATA);
    }
  }

  /*osal_delay_microsec(L9502_WAIT_LV); */ dwt_reset(); dwt_delay_ticks(dwt_us2t(L9502_WAIT_LV));;

  /* Read register FHM_3 */
  for(i=0; i< L9502_DEVICES_MAX; i++){
    readFault[i].R = L9502_R_W(L9502_R_W_READ) | L9502_ADDRESS(ADDR_CHIP_ID_LV) | L9502_DATA(0);
  }
  status = L9502_ReadMultipleDev(L9502Config.NumberOfDevices, ADDR_FHM_3, readFault);
  if(status)
  {
    for (i = 0; i < L9502Config.NumberOfDevices; i++)
    {
      L9502_ERRstatus[i].B.REG_READ_ERROR = 1;
      L9502_ERRstatus[i].B.ERROR_FRAME_RECEIVED = 1;
    }
  }
  else
  {
    for (i = 0; i < L9502Config.NumberOfDevices; i++)
    {
      g_statusReg[i].REGS.hFHM_3.R = (readFault[i].B.DATA);
    }
  }

  /*osal_delay_microsec(L9502_WAIT_LV); */ dwt_reset(); dwt_delay_ticks(dwt_us2t(L9502_WAIT_LV));;

  /* Read register CFG_7: DESAT, OTHERS, ASC, OP_MODE */
  for(i=0; i< L9502_DEVICES_MAX; i++){
    readFault[i].R = L9502_R_W(L9502_R_W_READ) | L9502_ADDRESS(ADDR_CHIP_ID_LV) | L9502_DATA(0);
  }
  status = L9502_ReadMultipleDev(L9502Config.NumberOfDevices, ADDR_CFG_7, readFault);
  if(status)
  {
    for (i = 0; i < L9502Config.NumberOfDevices; i++)
    {
      L9502_ERRstatus[i].B.REG_READ_ERROR = 1;
      L9502_ERRstatus[i].B.ERROR_FRAME_RECEIVED = 1;
    }
  }
  else
  {
    for (i = 0; i < L9502Config.NumberOfDevices; i++)
    {
      g_statusReg[i].REGS.hCFG_7.R = (readFault[i].B.DATA);
    }
  }
  /*osal_delay_microsec(L9502_WAIT_HV); */ dwt_reset(); dwt_delay_ticks(dwt_us2t(L9502_WAIT_HV));;

  /* Read register FHM_0 */
  for(i=0; i< L9502_DEVICES_MAX; i++){
    readFault[i].R = L9502_R_W(L9502_R_W_READ) | L9502_ADDRESS(ADDR_CHIP_ID_LV) | L9502_DATA(0);
  }
  status = L9502_ReadMultipleDev(L9502Config.NumberOfDevices, ADDR_FHM_0, readFault);
  if(status)
  {
    for (i = 0; i < L9502Config.NumberOfDevices; i++)
    {
      L9502_ERRstatus[i].B.REG_READ_ERROR = 1;
      L9502_ERRstatus[i].B.ERROR_FRAME_RECEIVED = 1;
    }
  }
  else
  {
    for (i = 0; i < L9502Config.NumberOfDevices; i++)
    {
      g_statusReg[i].REGS.hFHM_0.R = (readFault[i].B.DATA);
    }
  }
  /*osal_delay_microsec(L9502_WAIT_HV); */ dwt_reset(); dwt_delay_ticks(dwt_us2t(L9502_WAIT_HV));;

  /* Read register FHM_1 */
  for(i=0; i< L9502_DEVICES_MAX; i++){
    readFault[i].R = L9502_R_W(L9502_R_W_READ) | L9502_ADDRESS(ADDR_CHIP_ID_LV) | L9502_DATA(0);
  }
  status = L9502_ReadMultipleDev(L9502Config.NumberOfDevices, ADDR_FHM_1, readFault);
  if(status)
  {
    for (i = 0; i < L9502Config.NumberOfDevices; i++)
    {
      L9502_ERRstatus[i].B.REG_READ_ERROR = 1;
      L9502_ERRstatus[i].B.ERROR_FRAME_RECEIVED = 1;
    }
  }
  else
  {
    for (i = 0; i < L9502Config.NumberOfDevices; i++)
    {
      g_statusReg[i].REGS.hFHM_1.R = (readFault[i].B.DATA);
    }
  }
  /*osal_delay_microsec(L9502_WAIT_HV); */ dwt_reset(); dwt_delay_ticks(dwt_us2t(L9502_WAIT_HV));;

#if defined(VERSION_BA) || defined(VERSION_CA) || defined(VERSION_CD)
  /* Read register STO_CFG (to clear RLR bit GATE_RETRY) */
  for(i=0; i< L9502_DEVICES_MAX; i++){
    readFault[i].R = L9502_R_W(L9502_R_W_READ) | L9502_ADDRESS(ADDR_CHIP_ID_LV) | L9502_DATA(0);
  }
  status = L9502_ReadMultipleDev(L9502Config.NumberOfDevices, ADDR_STO_CFG, readFault);
  if(status)
  {
    for (i = 0; i < L9502Config.NumberOfDevices; i++)
    {
      L9502_ERRstatus[i].B.REG_READ_ERROR = 1;
      L9502_ERRstatus[i].B.ERROR_FRAME_RECEIVED = 1;
    }
  }
  else
  {
    for (i = 0; i < L9502Config.NumberOfDevices; i++)
    {
      g_statusReg[i].REGS.hSTO_CFG.R = (readFault[i].B.DATA);
    }
  }
  //osal_delay_microsec(L9502_WAIT_HV);
    dwt_reset();
    dwt_delay_ticks(dwt_us2t(L9502_WAIT_HV));    
  
#endif
  /* Write Disable pin: high */
  L9502_SetDisPin(true);
  //L9502_SetBrakePin(true);

  /* wait at least 5 us for communication between LV and HV */
  //osal_delay_microsec(10);
  dwt_reset();
  dwt_delay_ticks(dwt_us2t(10));    
  

  /* Write Disable pin: low */
  L9502_SetDisPin(false);
  //L9502_SetBrakePin(false);
  //osal_delay_microsec(10);
  dwt_reset();
  dwt_delay_ticks(dwt_us2t(10));    
  

  return status;
}

 /**
  * @brief This function Writes '1' or '0' to DIS pin
  * @param enable      true or false to be written
  */
void L9502_SetDisPin(bool enable)
{
  /*Write Disable pin: high*/
  //pal_lld_writepad(PORT_L9502_DIS, L9502_DIS, enable);    /* LS Leg A */
  gpio_write_pin(L9502_DIS,enable);  /* LS Leg A */
#if defined(L9502_DIS2)
  //pal_lld_writepad(PORT_L9502_DIS2, L9502_DIS2, enable);  /* HS Leg A */
  gpio_write_pin(L9502_DIS2,enable);  /* HS Leg A */
#endif
#if defined(L9502_DIS3)
  //pal_lld_writepad(PORT_L9502_DIS3, L9502_DIS3, enable);  /* LS Leg B */
  gpio_write_pin(L9502_DIS3, enable);  /* LS Leg B */
#endif
#if defined(L9502_DIS4)
  //pal_lld_writepad(PORT_L9502_DIS4, L9502_DIS4, enable);  /* HS Leg B */
  gpio_write_pin(L9502_DIS4, enable);  /* HS Leg B */
#endif
#if defined(L9502_DIS5)
  //pal_lld_writepad(PORT_L9502_DIS5, L9502_DIS5, enable);  /* LS Leg C */
  gpio_write_pin(L9502_DIS5, enable);  /* LS Leg C */
#endif
#if defined(L9502_DIS6)
  //pal_lld_writepad(PORT_L9502_DIS6, L9502_DIS6, enable);  /* HS Leg C*/
  gpio_write_pin(L9502_DIS6, enable);  /* HS Leg C*/
#endif
}
#if 0
/**
 * @brief                      Writes '1' or '0' to DIS pin
 * @param[in]      enable      true or false to be written
 * @notapi
 */
void L9502_SetBrakePin(bool enable)
{
  /*Write Disable pin: high*/
  pal_lld_writepad(PORT_L9502_BRAKE, L9502_BRAKE, enable);    /* LS Leg A */
#if defined(L9502_BRAKE2)
  pal_lld_writepad(PORT_L9502_BRAKE2, L9502_BRAKE2, enable);  /* HS Leg A */
#endif
#if defined(L9502_BRAKE3)
  pal_lld_writepad(PORT_L9502_BRAKE3, L9502_BRAKE3, enable);  /* LS Leg B */
#endif
#if defined(L9502_BRAKE4)
  pal_lld_writepad(PORT_L9502_BRAKE4, L9502_BRAKE4, enable);  /* HS Leg B */
#endif
#if defined(L9502_BRAKE5)
  pal_lld_writepad(PORT_L9502_BRAKE5, L9502_BRAKE5, enable);  /* LS Leg C */
#endif
#if defined(L9502_BRAKE6)
  pal_lld_writepad(PORT_L9502_BRAKE6, L9502_BRAKE6, enable);  /* HS Leg C*/
#endif
}
#endif
/**
 * @brief      return status of DIS pin
 * @notapi
 */
bool L9502_GetDisPin(void)
{
  uint8_t ret1;
  uint8_t ret2 = 0;
  //ret1 = pal_lld_readpad(PORT_L9502_DIS, L9502_DIS);
  ret1 = gpio_read_pin(L9502_DIS);
  
#if defined(L9502_DIS2)
  //ret2 = pal_lld_readpad(PORT_L9502_DIS2, L9502_DIS2);  /* HS Leg A */
  ret2 = gpio_read_pin(L9502_DIS2);  /* HS Leg A */
#endif

  /* return true if any dis bit is High */
  return ( ((ret1|ret2) == 1) ? true : false);
}

/**
 * @brief         Read Chip ID from ADDR_CFG_10 Page 0x0
 * @details       Reads Chip ID from Register CFG_10 Page 0x0 :
 *                bits <3,0>; 4 bit chip identifier
 * @return        status value (SPI status)
 * @notapi
 */
uint16_t L9502_GetChipID(uint16_t device)
{
  uint16_t   chipID = 0;
  uint16_t   status;
  L9502_Frame_t    rxPacket;

  /* Register ADDR_CHIP_ID_LV  */
  status = L9502_ReadSingleDev(device, ADDR_CHIP_ID_LV, &rxPacket);

  if(status == NO_ERROR)
  {
     /* Extract from received data */
     chipID = rxPacket.B.DATA;
  }

  return chipID;
}

/**
 * @brief         Updates operation mode
 * @details       Reads L9502 driver operation mode (according to internal state machine) from
 *                Register FMH_2 : bits <5,3>; Set internal operation mode variable
 * @return        status value (SPI status)
 * @notapi
 */
uint16_t L9502_UpdateOpMode(uint16_t device)
{
  uint16_t   status = NO_ERROR;  /* SUCCESS */
  L9502_Frame_t    rxPacket;
  const uint8_t    bit_postion_lsb = 5U;
  const uint8_t    counts_of_bits  = 3U;

  /* Register FMH_2 : bits <5,3> (bit offset = 5, bit width = 3) */
  status = L9502_ReadSingleDev(device, ADDR_FHM_2, &rxPacket);

  if(status == NO_ERROR)
  {
    /* Extract from received data rxPacket.B.READ_DATA bits 5,4,3 */
    g_internalData.opMode[device] = (L9502_opMode_t)(TAKE_N_BITS_FROM(
                                          rxPacket.B.DATA,
                                          bit_postion_lsb,
                                          counts_of_bits));
  }
  return status;
}

/**
 * @brief       returns operation mode
 * @details     returns internal data operation mode, L9502_UpdateOpMode should be called before
 * @return      status value (SPI status)
 * @notapi
 */
L9502_opMode_t L9502_GetOpMode(uint16_t device)
{
  return g_internalData.opMode[device];
}


/**
 * @brief       Reads FS Flag
 * @details     Reads FS FLAG status on driver pin, if FS flag LOW sets internal error
 * @isr            The function is an ISR and must be documented as such in the integration manual.
 *                 Note that this is not a Doxygen tag but a customization.
 *                 Must be used only for module ISR functions
 *                 Delete this text when using (only "@isr" must appear on a new line)
 * @notapi
 */
bool L9502_GetFsFlag(void)
{
    //return (gpio_read_pin(L9502_FS_FLAG) == 1) ? true : false;
    return (gpio_read_pin(TIM1_BKIN) == 1) ? true : false;  
}

/**
  * @brief  Schedule function to be called with periodicity.
  * @retval none.
  */
void L9502_Schedule(void)
{
  uint8_t i;
  uint16_t fault;

  if (sch_checkerror)
  {
    /* Check errors: FAULTN1 should be enabled and used as diagnostic
                     READ ONLY WHEN THE FLAG IS LOW OR THE DIAGNOSTIC WILL IMPACT FOC */
    fault = L9502_CheckErrors(L9502_wErrorsNow,L9502_wErrorsOccurred, 0);
    if (!fault)
    {
       for (i = 0; i < L9502Config.NumberOfDevices; i++)
       {
         if ((L9502_wErrorsNow[i] != 0)||((L9502_wErrorsOccurred[i] != 0)))
         {
          /* Status Registers report errors, SPI communication is OK */
          g_internalData.faultFlag = true;
          /* Fault Led On if any; */
        }
      }
    }
    else
    {
       /* SPI Communication KO */
       g_internalData.faultFlag = false;
    }
    //osal_delay_microsec(200);
    dwt_reset();
    dwt_delay_ticks(dwt_us2t(200));

    /* Clear scheduler variable: one time read */
    sch_checkerror = 0;
  }
}

/**
  * @brief  Check errors of L9502 devices
  * @param  errorNow buffer of returned bitfields containing error flags
  *         coming from L9502 device that are currently active.\n
  *         The buffer have to be provided from the caller.
  * @param  errorOccurred buffer of returned bitfields containing error flags
  *         coming from L9502 device that are over.\n
  *         The buffer have to be provided from the caller.
  * @param  Read command executed with MSB = 1 in field data if rlr = 1
  * @retval uint16_t It returns status for ReadMultipleDevice.
  */
uint16_t L9502_CheckErrors(uint32_t* errorNow,uint32_t* errorOccurred, uint8_t rlr)
{
  uint32_t errorFromDevices[L9502_DEVICES_MAX] = {0};
  L9502_Frame_t readRegValues[L9502_DEVICES_MAX];
  uint16_t status = NO_ERROR;
  uint8_t i,l;
#if defined(VERSION_BA)
  (void) rlr; /* parameter not used in version BA */
#endif
  /* if Buffers provided from the caller are available */
  if ((errorNow)&&(errorOccurred))
  {

    l = L9502Config.NumberOfDevices;

    /* Read High Voltage Status Register */
    for (i = 0; i < L9502_DEVICES_MAX; i++)
    {
      readRegValues[i].R = 0;
    }
    /*********            READ FMH_1          **********/
#if defined(VERSION_CA) || defined(VERSION_CD)
    status = L9502_ReadMultipleDevRlr(l, ADDR_FHM_1, readRegValues, rlr);
#else
    status = L9502_ReadMultipleDev(l, ADDR_FHM_1, readRegValues);
#endif
    for (i = 0; i < l; i++)
    {
      errorFromDevices[i] = ( (readRegValues[i].B.DATA & FHM_1_ERROR_MASK) << FHM_1_ERROR_SHL);
      g_statusReg[i].REGS.hFHM_1.R = (readRegValues[i].B.DATA);
    }
    //osal_delay_microsec(L9502_WAIT_HV);
    dwt_reset();
    dwt_delay_ticks(dwt_us2t(L9502_WAIT_HV));

    /* Read High Voltage Status Register */
    for (i = 0; i < L9502_DEVICES_MAX; i++)
    {
      readRegValues[i].R = 0;
    }
    /*********            READ FMH_0          **********/
#if defined(VERSION_CA) || defined(VERSION_CD)
    status = L9502_ReadMultipleDevRlr(l, ADDR_FHM_0, readRegValues, rlr);
#else
    status = L9502_ReadMultipleDev(l, ADDR_FHM_0, readRegValues);
#endif
    for (i = 0; i < l; i++)
    {
      errorFromDevices[i] |= ( (readRegValues[i].B.DATA & FHM_0_ERROR_MASK) << FHM_0_ERROR_SHL);
      g_statusReg[i].REGS.hFHM_0.R = (readRegValues[i].B.DATA);
    }
    //osal_delay_microsec(L9502_WAIT_HV);
    dwt_reset();
    dwt_delay_ticks(dwt_us2t(L9502_WAIT_HV));    
    

    /* Read High Voltage Status Register */
    for (i = 0; i < L9502_DEVICES_MAX; i++)
    {
      readRegValues[i].R = 0;
    }
    /*********            READ CFG_7          **********/
#if defined(VERSION_CA) || defined(VERSION_CD)
    status = L9502_ReadMultipleDevRlr(l, ADDR_CFG_7, readRegValues, rlr);
#else
    status = L9502_ReadMultipleDev(l, ADDR_CFG_7, readRegValues);
#endif
    for (i = 0; i < l; i++)
    {
      errorFromDevices[i] |= ( ( ((readRegValues[i].B.DATA & CFG_7_ERROR_MASK) >> CFG_7_ERROR_SHR) ) << CFG_7_ERROR_SHL);
      g_statusReg[i].REGS.hCFG_7.R = (readRegValues[i].B.DATA);
    }
    //osal_delay_microsec(L9502_WAIT_HV);
    dwt_reset();
    dwt_delay_ticks(dwt_us2t(L9502_WAIT_HV));    
   

#if defined(VERSION_BA) || defined(VERSION_CA) || defined(VERSION_CD)
    /* Read High Voltage Status Register */
    for (i = 0; i < L9502_DEVICES_MAX; i++)
    {
      readRegValues[i].R = 0;
    }
    /*********            READ STO_CFG          **********/
#if defined(VERSION_CA) || defined(VERSION_CD)
    status = L9502_ReadMultipleDevRlr(l, ADDR_STO_CFG, readRegValues, rlr);
#else
    status = L9502_ReadMultipleDev(l, ADDR_STO_CFG, readRegValues);
#endif
    for (i = 0; i < l; i++)
    {
      //errorFromDevices[i] |= ( ( ((readRegValues[i].B.DATA & CFG_7_ERROR_MASK) >> CFG_7_ERROR_SHR) ) << CFG_7_ERROR_SHL);
      g_statusReg[i].REGS.hSTO_CFG.R = (readRegValues[i].B.DATA);
    }
    //osal_delay_microsec(L9502_WAIT_HV);
    dwt_reset();
    dwt_delay_ticks(dwt_us2t(L9502_WAIT_HV));    
#endif

    /* Read Low Voltage Status Register */
    for (i = 0; i < L9502_DEVICES_MAX; i++)
    {
      readRegValues[i].R = 0;
    }
    /*********            READ FMH_3          **********/
#if defined(VERSION_CA) || defined(VERSION_CD)
    status = L9502_ReadMultipleDevRlr(l, ADDR_FHM_3, readRegValues, rlr);
#else
    status = L9502_ReadMultipleDev(l, ADDR_FHM_3, readRegValues);
#endif
    for (i = 0; i < l; i++)
    {
      errorFromDevices[i] |= ( ((readRegValues[i].B.DATA & FHM_3_ERROR_MASK) >> CFG_7_ERROR_SHR) << FHM_3_ERROR_SHL);
      g_statusReg[i].REGS.hFHM_3.R = (readRegValues[i].B.DATA);
    }
    //osal_delay_microsec(L9502_WAIT_LV);
    dwt_reset();
    dwt_delay_ticks(dwt_us2t(L9502_WAIT_LV));    

    /* Read Low Voltage Status Register */
    for (i = 0; i < L9502_DEVICES_MAX; i++)
    {
      readRegValues[i].R = 0;
    }
    /*********            READ FMH_2          **********/
#if defined(VERSION_CA) || defined(VERSION_CD)
    status = L9502_ReadMultipleDevRlr(l, ADDR_FHM_2, readRegValues, rlr);
#else
    status = L9502_ReadMultipleDev(l, ADDR_FHM_2, readRegValues);
#endif
    for (i = 0; i < l; i++)
    {
      errorFromDevices[i] |= (readRegValues[i].B.DATA & FHM_2_ERROR_MASK);
      g_statusReg[i].REGS.hFHM_2.R = (readRegValues[i].B.DATA);
    }
    //osal_delay_microsec(L9502_WAIT_LV);
     dwt_reset();
     dwt_delay_ticks(dwt_us2t(L9502_WAIT_LV));
    
    /* errorFromDevices will be formatted as below : */
    /* CA: errorFromDevices Not used */
    /* BA:|31  FMH_1  22|21  FMH_0  13|12  CFG_7  5|4  FMH_3  4|3  FMH_2  0|*/
    /* AA:| | | | | |f|m|h|_|1| | | | | |f|m|h|_|0|c|_|7|f|m|h|_|3|f|m|h|2|*/
    /* Any bit set to 1, if not masked will stop PWM */

    for (i = 0; i < l; i++)
    {
      L9502_wErrorsNow[i] = errorFromDevices[i];
      L9502_wErrorsOccurred[i] |= L9502_wErrorsNow[i];
      errorNow[i] = L9502_wErrorsNow[i];
      errorOccurred[i] = L9502_wErrorsOccurred[i];
    }
  }
  return status;
}

/**
 * @brief           Recovers driver from faults
 *
 * @details         This function should be called if error occurs (FAULT1/FAULT2 flag is going to LOW)
 *                  Safe state is left when the following re-engagement procedure is executed:
 *                  - Operation fault is removed;
 *                  - Drive the DIS pin generating a High -> Low transition.
 *                    For correctly recognizing the High -> Low transition,
 *                    the DIS pin must have been held high for at least TDIS_CLEAR (5.4 us).
 *                  - Operation fault flag in status register is Cleared on Read.
 *                  Recover procedure :
 *                  Reads all status registers to analyze which faults occurs.
 *                  Reads operation mode.
 *                  If op mode is in SAFE_OFF mode (shutdown faults), function toggles with DIS pin,
 *                  otherwise (auto-retry and reduced op modes) function toggles affected HBs (according HBn_EN_ECHO).
 *
 *                  Note: to ensure a safe output re-engagement, the IN+ PWM shall
 *                  be kept low before generating the DIS High -> Low transition.
 *
 * @return          status value (SPI status)
 *
 * @notapi
 */
uint16_t L9502_FaultRecovery(void)
{
    uint16_t status;
    uint8_t i;

    /* Execute SPI Clear on Read on Status Registers */
    status = L9502_CheckErrors(L9502_wErrorsNow,L9502_wErrorsOccurred, 1);

    for(i=0; i< L9502Config.NumberOfDevices; i++)
    {
        if (MC_NO_ERROR == L9502_CheckFaultState())
        {
            g_internalData.faultFlag = false;
            status |= L9502_UpdateOpMode(i);
        }
    }

    /* DIS falling edge procedure: SAFE_MODE -> NORMAL_MODE */
    /* Write Disable pin: high */
    L9502_SetDisPin(true);
    //L9502_SetBrakePin(true);

    /* wait at least TDIS_CLEAR (5.4 us) for communication between LV and HV */
    //osal_delay_microsec(10);
    dwt_reset();
    dwt_delay_ticks(dwt_us2t(10));
   

    /* Write Disable pin: low */
    L9502_SetDisPin(false);
    //L9502_SetBrakePin(false);
    //osal_delay_microsec(10);
    dwt_reset();
    dwt_delay_ticks(dwt_us2t(10));
    
    /* Enable TIM brt*/
	LL_TIM_EnableIT_BRK(TIM1);

  return status;
}

/**
  * @brief  It returns MC_DIAG_ERROR or MC_NO_ERROR depending on
  *         external driver Diagnostic values
  * @retval uint16_t Fault code error
  */
uint16_t L9502_CheckFaultState(void)
{
  uint16_t hFault;
  uint8_t i;

  hFault = MC_NO_ERROR;
  if(L9502_init != NO_ERROR)
  {
    hFault = MC_DIAG_ERROR;
  }
  else  
  {
  for (i = 0; i < L9502Config.NumberOfDevices; i++)
  {
    if (L9502_wErrorsNow[i] != 0)
    {
      hFault = MC_DIAG_ERROR;
      break;
    }
  }
  }
  return hFault;
}

/**
 * @brief       Returns the value of the status registers for a single device
 *              Updated during L9502_ClearFault (init) and L9502_CheckErrors (schedule)
 *              Status Registers are:
 *              - LV: FMH_2, FMH_3
 *              - HV: CFG_7, FHM_0, FHM_1
 *
 * @return      L9502_statusReg_t Status registers pointer
 *
 * @notapi
 */
L9502_statusReg_t* L9502_GetStatusRegisters(uint16_t device)
{
    return &(g_statusReg[device]);
}

/**
 * @brief       Set Global variable sch_checkerror used in the schedule
 *              Called by user to enable SPI communication to read Status registers
 *
 * @return      true if the global variable is set
 *
 * @notapi
 */
bool L9502_SetDiagRegisters(uint8_t set)
{
  if(set != 1u)
    return false;
  sch_checkerror = set;
  return true;
}

/**
 * @brief     ADC start of conversion
 * @details	  Trigger ADC start conversion (ADC-SOC bit of CFG_7 HV register)
 * @param     device device number
 */
void L9502_ADCStartConversion(uint8_t device)
{
   uint16_t address;
   uint16_t value;
   address = ADDR_CFG_7;
   value = 0x80U;
   L9502_WriteHVSingleDev(device, address, value);
}

/**
  * @brief  Send SPI command to force FAULT1 (for debug scope).
  * @param  enable (1 force FAULTN1 low / 0 FAULTN1 idle)
  * @retval None
  */
void L9502_Force_FAULT1(uint8_t enable)
{
   if(enable)
   {
      L9502_WriteLVSingleDev(1, ADDR_CFG_8, (L9502_VALUE_CFG_8 | 0x0008));
   }
   else
   {
      L9502_WriteLVSingleDev(1, ADDR_CFG_8, (L9502_VALUE_CFG_8));
   }
}

/*===========================================================================*/
/* IRQ Handlers                                                              */
/*===========================================================================*/

/**
 * @brief   IRQ handler function implementation. Called by external interrupt
 *          when FAULT1/FAULT2 are de-asserted
 */

void L9502_irqfaultRecovery(void)
{
  /* L9502_SetDisPin(true); */
  sch_checkerror = 1u;
}
