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
*   @file    gap_gate_driver_ctrl.c
*   @version M4_VERSION
*   @brief   This file provides firmware functions that implement the features
*            of the GAP_GATE_DRIVER_CTRL component of the Stellar Motor Control.
*          
*   @details This file provides firmware functions that implement the features
*            of the GAP_GATE_DRIVER_CTRL component of the Stellar Motor Control.
 */
/* Includes ------------------------------------------------------------------*/
#include "STGAP1S_cfg.h"
#include "gap_gate_driver_ctrl.h"
#include "main.h"
#include "typedefs.h"
#include "mc_type.h"
#include "spi.h"
#include "gpio.h"
#include "dma.h"
/** @addtogroup STELLAR_PMSM_MC_FOC_LIBRARY
  * @{
  */

/** @defgroup GAP_GATE_DRIVER_CTRL <GAP_GATE_DRIVER_CTRL>
  * @brief GAP_GATE_DRIVER_CTRL component of Stellar Motor Control SDK
  *
  * <Detailed documentation for the component. This detailed description documents the
  * component: it is its users guide. it explains the feratures it provides, how to use
  * it and what conditions are required fdor its usage.
  *
  * There shall be only one such section per component. If the implementation of a
  * component is split among several .c files, only one of them will have this section.
  * The others will use an @addtogroup GAP_GATE_DRIVER_CTRL instead.
  * >
  *
  * @{
  */

/* Private defines -----------------------------------------------------------*/
/**
  * @brief Brief description same rules as for structs
  *
  *  Detailed description if needed.
  *
  * As above: be direct, avoid the obvious, tell the hidden.
  */

#define GAP_STARTCONFIG                 0x2A
#define GAP_STOPCONFIG                  0x3A
#define GAP_WRITEREG                    0x80
#define GAP_READREG                     0xA0
#define GAP_RESETREG                    0xC0
#define GAP_RESETSTATUS                 0xD0
#define GAP_GLOBALRESET                 0xEA
#define GAP_SLPEEP                      0xF5
#define GAP_NOP                         0x00


#define GAP_ERROR_CODE_FROM_DEVICE_MASK (uint32_t)(0xFF000000)

#define WAITTIME                        5000 /* 860u */
#define WAITTRCHK                       50   /* 8.6u */
#define WAITTSENSECHK                   50   /* 8.6u */
#define WAITTGCHK                       50   /* 8.6u */
#define WAITTDESATCHK                   50   /* 8.6u */

#define SPI_BUFFER_SIZE     1
uint8_t spi_tx_buffer[SPI_BUFFER_SIZE*2];
uint8_t spi_rx_buffer[SPI_BUFFER_SIZE*2];

volatile uint8_t sch_checkerror = 0;
typedef enum {SCH_TEST_IDLE,SCH_TEST_FAULT,SCH_TEST_RUN} state_t;
volatile state_t sch_state = SCH_TEST_IDLE;
volatile bool faultGap = (bool)false;
uint32_t GAP_errorsNow[GAP_DEVICE_NUM];
uint32_t GAP_errorsOccurred[GAP_DEVICE_NUM];

/* Global variables ----------------------------------------------------------*/

static void GAP_SD_Deactivate( GAP_Handle_t * pHandle_t );
static void GAP_SD_Activate( GAP_Handle_t * pHandle_t );
static void GAP_CS_Deactivate( GAP_Handle_t * pHandle_t );
static void GAP_CS_Activate( GAP_Handle_t * pHandle_t );
static uint16_t GAP_SPI_Send( GAP_Handle_t * pHandle_t, uint16_t value );

/**
  * @brief  Check errors of GAP devices
  * @param  pHandle related object of class CGAP_GDC
  * @param  errorNow buffer of returned bitfields containing error flags
  *         coming from GAP device that are currently active.\n
  *         \link GAP_class_error_code Code errors\endlink.\n
  *         The buffer have to be provided from the caller.
  * @param  errorOccurred buffer of returned bitfields containing error flags
  *         coming from GAP device that are over.\n
  *         \link GAP_class_error_code Code errors\endlink.\n
  *         The buffer have to be provided from the caller.
  * @retval bool It returns false if an error occurs, otherwise return true.
  */
__weak bool GAP_CheckErrors( GAP_Handle_t * pHandle, uint32_t * error_now, uint32_t * error_occurred )
{
  bool ret_val = false;
  uint32_t errorFromDevices[MAX_DEVICES_NUMBER];

  if ( ( error_now ) && ( error_occurred ) )
  {
    uint8_t index1, index2;
    uint8_t ret_read[MAX_DEVICES_NUMBER];

//    /* If current error is device not programmable try to re-configure before
//       read the status registers */
//    if ((pDVars->wGAP_ErrorsNow[0] & GAP_ERROR_CODE_DEVICES_NOT_PROGRAMMABLE) ||
//        (pDVars->wGAP_ErrorsNow[0] & GAP_ERROR_CODE_SPI_CRC))
//    {
//      if (GAP_Configuration(pHandle))
//      {
//        pDVars->wGAP_ErrorsNow[0] &= ~GAP_ERROR_CODE_DEVICES_NOT_PROGRAMMABLE;
//        pDVars->wGAP_ErrorsNow[0] &= ~GAP_ERROR_CODE_SPI_CRC;
//      }
//    }


    index2 = pHandle->DeviceNum;
    ret_val = GAP_ReadRegs( pHandle, ret_read, STATUS1 );
    for ( index1 = 0; index1 < index2; index1 ++ )
    {
      errorFromDevices[index1] = ( ret_read[index1] << 16 );
    }
    ret_val = GAP_ReadRegs( pHandle, ret_read, STATUS2 );
    for ( index1 = 0; index1 < index2; index1 ++ )
    {
      /* Clear GATE bit from STATUS2 - no error if 1 */
      ret_read[index1] &= 0xFE;
      errorFromDevices[index1] |= ( ret_read[index1] << 8 );
    }
    ret_val = GAP_ReadRegs( pHandle, ret_read, STATUS3 );
    for ( index1 = 0; index1 < index2; index1 ++ )
    {
      errorFromDevices[index1] |= ret_read[index1];
    }

    for ( index1 = 0; index1 < index2; index1 ++ )
    {
      pHandle->GAP_ErrorsNow[index1] &= GAP_ERROR_CODE_FROM_DEVICE_MASK;
      pHandle->GAP_ErrorsNow[index1] |= errorFromDevices[index1];
      pHandle->GAP_ErrorsOccurred[index1] |= pHandle->GAP_ErrorsNow[index1];
      error_now[index1] = pHandle->GAP_ErrorsNow[index1];
      error_occurred[index1] = pHandle->GAP_ErrorsOccurred[index1];
    }
  }
  return ret_val;
}

/**
  * @brief  Clears the fault state of GAP devices.
  * @param  pHandle related object of class CGDC.
  * @retval none.
  */
__weak void GAP_FaultAck( GAP_Handle_t * pHandle )
{
  uint8_t index1, index2;
  uint16_t value;

  GAP_SD_Activate( pHandle );
  value = GAP_CRCCalculate( GAP_RESETSTATUS, 0xFF );
  GAP_CS_Activate( pHandle );
  index2 = pHandle->DeviceNum;

  for ( index1 = 0; index1 < index2; index1 ++ )
  {
    GAP_SPI_Send( pHandle, value );
  }
  GAP_CS_Deactivate( pHandle );
  GAP_SD_Deactivate( pHandle );
  wait( WAITTIME );

  for ( index1 = 0; index1 < index2; index1 ++ )
  {
    pHandle->GAP_ErrorsOccurred[index1] = GAP_ERROR_CLEAR;
  }
}

/**
  * @brief  Programs the GAP devices with the settled parameters.
  * @param  pHandle related object of class CGDC.
  * @retval bool It returns false if at least one device is not programmable,
  *         otherwise return true.
  */
__weak bool GAP_Configuration( GAP_Handle_t * pHandle )
{
  bool ret_val;

  /*===========================================================================*/
  /* SPI baud rate calculation                                                 */
  /*===========================================================================*/
  /* CLOCK_SPI1CLK = CLOCK_PCLK2 = CLOCK_SYSCLKDIV / 2 =                       */
  /* = [CLOCK_SYSCLK / (CLOCK_CFG_SYSPRE_VALUE + 1)] / 2 =                     */
  /* = [300MHz / (0 + 1)] / 2 = 150MHz                                         */
  /*                                                                           */
  /* SPI_CR1_BR = DIV32 -> Baud Rate = CLOCK_SPI1CLK / 64 = 150MHz / 64 = 2.34375MHz  */
  /*===========================================================================*/
    
  spi_dma_conf_t spi_master_dma_conf;
    
  /* Configure SPI Master DMA.*/
  spi_master_dma_conf.dma_stream_tx_id = DMA1_STREAM0_ID;
  spi_master_dma_conf.dma_stream_rx_id = DMA1_STREAM1_ID;
  spi_master_dma_conf.dma_stream_bus_prio = DMA_PRIORITY_MAX;
  spi_master_dma_conf.dma_stream_irq_prio = IRQ_PRIORITY_2;

  /* Initialize SPI Master driver instance.*/
  (void)spi_init(pHandle->Gap_Spix);

  /* Configure SPI Master driver instance.*/
  (void)spi_set_drv_mode(pHandle->Gap_Spix, SPI_DRV_MODE_INT_SYNC);
  (void)spi_set_dma_conf(pHandle->Gap_Spix, &spi_master_dma_conf);
  (void)spi_set_mode(pHandle->Gap_Spix, SPI_MODE_MASTER);
  (void)spi_set_baud(pHandle->Gap_Spix, SPI_BAUDRATE_DIV64);
  (void)spi_set_cpha(pHandle->Gap_Spix, SPI_CPHA_1);
  (void)spi_set_cpol(pHandle->Gap_Spix, SPI_CPOL_0);
  (void)spi_set_fformat(pHandle->Gap_Spix, SPI_FRAMEFORMAT_MSB);
  (void)spi_set_fsize(pHandle->Gap_Spix, SPI_FRAMESIZE_16);

  /* Apply SPI Master driver instance configuration.*/
  spi_start(pHandle->Gap_Spix);
  
  /* Configure devices with settled parameters */
  GAP_DevicesConfiguration( pHandle );

  /* Verify if device has been programmed */
  ret_val = GAP_IsDevicesProgrammed( pHandle );

  if ( !ret_val )
  {
    /* At least one device is not programmable */
    pHandle->GAP_ErrorsNow[0] |= GAP_ERROR_CODE_DEVICES_NOT_PROGRAMMABLE;
    pHandle->GAP_ErrorsOccurred[0] |= GAP_ERROR_CODE_DEVICES_NOT_PROGRAMMABLE;
  }
  return ret_val;
}

/**
  * @brief  Checks if the GAP devices are programmed with the settled parameters.
  * @param  pHandle related object of class CGDC.
  * @retval True if the GAP devices are already programmed with the settled
  *         parameters.
  */
__weak bool GAP_IsDevicesProgrammed( GAP_Handle_t * pHandle )
{
  bool ret_val = true;
  uint8_t index1, index2 = pHandle->DeviceNum;
  uint8_t read_reg_values[MAX_DEVICES_NUMBER];

  GAP_ReadRegs( pHandle, read_reg_values, CFG1 );

  for ( index1 = 0; index1 < index2; index1 ++ )
  {
    if ( ( pHandle->DeviceParams[index1].CFG1 & CFG1_REG_MASK ) != read_reg_values[index1] )
    {
      ret_val = false;
      break;
    }
  }

  if ( ret_val )
  {
    GAP_ReadRegs( pHandle, read_reg_values, CFG2 );
    for ( index1 = 0; index1 < index2; index1 ++ )
    {
      if ( ( pHandle->DeviceParams[index1].CFG2 & CFG2_REG_MASK ) != read_reg_values[index1] )
      {
        ret_val = false;
        break;
      }
    }
  }
  if ( ret_val )
  {
    GAP_ReadRegs( pHandle, read_reg_values, CFG3 );
    for ( index1 = 0; index1 < index2; index1 ++ )
    {
      if ( ( pHandle->DeviceParams[index1].CFG3 & CFG3_REG_MASK ) != read_reg_values[index1] )
      {
        ret_val = false;
        break;
      }
    }
  }
  if ( ret_val )
  {
    GAP_ReadRegs( pHandle, read_reg_values, CFG4 );
    for ( index1 = 0; index1 < index2; index1 ++ )
    {
      if ( ( pHandle->DeviceParams[index1].CFG4 & CFG4_REG_MASK ) != read_reg_values[index1] )
      {
        ret_val = false;
        break;
      }
    }
  }
  if ( ret_val )
  {
    GAP_ReadRegs( pHandle, read_reg_values, CFG5 );
    for ( index1 = 0; index1 < index2; index1 ++ )
    {
      if ( ( pHandle->DeviceParams[index1].CFG5 & CFG5_REG_MASK ) != read_reg_values[index1] )
      {
        ret_val = false;
        break;
      }
    }
  }
  if ( ret_val )
  {
    GAP_ReadRegs( pHandle, read_reg_values, DIAG1 );
    for ( index1 = 0; index1 < index2; index1 ++ )
    {
      if ( ( pHandle->DeviceParams[index1].DIAG1 & DIAG1_REG_MASK )  != read_reg_values[index1] )
      {
        ret_val = false;
        break;
      }
    }
  }
  if ( ret_val )
  {
    GAP_ReadRegs( pHandle, read_reg_values, DIAG2 );
    for ( index1 = 0; index1 < index2; index1 ++ )
    {
      if ( ( pHandle->DeviceParams[index1].DIAG2 & DIAG2_REG_MASK ) != read_reg_values[index1] )
      {
        ret_val = false;
        break;
      }
    }
  }
  return ret_val;
}

/**
  * @brief  Programs the GAP devices with the settled parameters.
  * @param  pHandle related object of class CGDC.
  * @retval bool It returns false if an error occurs, otherwise return true.
  */
__weak bool GAP_DevicesConfiguration( GAP_Handle_t * pHandle )
{
  bool ret_val = true;
  uint8_t index1, DeviceNum = pHandle->DeviceNum;
  uint8_t write_reg_values[MAX_DEVICES_NUMBER];

  GAP_StartConfig( pHandle );

  /* Global Reset before programming */
  GAP_GlobalReset( pHandle );

  for ( index1 = 0; index1 < DeviceNum; index1 ++ )
  {
    write_reg_values[index1] = pHandle->DeviceParams[index1].CFG1;
  }
  ret_val = GAP_WriteRegs( pHandle, write_reg_values, CFG1 );

  if ( ret_val )
  {
    for ( index1 = 0; index1 < DeviceNum; index1 ++ )
    {
      write_reg_values[index1] = pHandle->DeviceParams[index1].CFG2;
    }
    ret_val = GAP_WriteRegs( pHandle, write_reg_values, CFG2 );
  }

  if ( ret_val )
  {
    for ( index1 = 0; index1 < DeviceNum; index1 ++ )
    {
      write_reg_values[index1] = pHandle->DeviceParams[index1].CFG3;
    }
    ret_val = GAP_WriteRegs( pHandle, write_reg_values, CFG3 );
  }
  if ( ret_val )
  {
    for ( index1 = 0; index1 < DeviceNum; index1 ++ )
    {
      write_reg_values[index1] = pHandle->DeviceParams[index1].CFG4;
    }
    ret_val = GAP_WriteRegs( pHandle, write_reg_values, CFG4 );
  }
  if ( ret_val )
  {
    for ( index1 = 0; index1 < DeviceNum; index1 ++ )
    {
      write_reg_values[index1] = pHandle->DeviceParams[index1].CFG5;
    }
    ret_val = GAP_WriteRegs( pHandle, write_reg_values, CFG5 );
  }
  if ( ret_val )
  {
    for ( index1 = 0; index1 < DeviceNum; index1 ++ )
    {
      write_reg_values[index1] = pHandle->DeviceParams[index1].DIAG1;
    }
    ret_val = GAP_WriteRegs( pHandle, write_reg_values, DIAG1 );
  }
  if ( ret_val )
  {
    for ( index1 = 0; index1 < DeviceNum; index1 ++ )
    {
      write_reg_values[index1] = pHandle->DeviceParams[index1].DIAG2;
    }
    ret_val = GAP_WriteRegs( pHandle, write_reg_values, DIAG2 );
  }

  GAP_StopConfig( pHandle );

  /* Fault reset */
  GAP_FaultAck( pHandle );

  return ret_val;
}

/**
  * @brief  Calculate CRC from data and create 16bit value with data as MSB and
  *         CRC as LSB.
  * @param  data 8bit value used to calculate CRC.
  * @retval uint16_t It returns the 16bit value with data as MSB and
  *         CRC as LSB.
  */
__weak uint16_t GAP_CRCCalculate( uint8_t data, uint8_t crc_initial_value )
{
  uint8_t crc = crc_initial_value;
  uint8_t poly = 0x07;
  uint8_t crc_temp;
  uint8_t crctab[8];
  uint8_t index1, index2;

  uint16_t value;
  value = data;
  value <<= 8;

  for ( index2 = 0; index2 < 8; index2 ++ )
  {
    crctab[index2] = ( crc >> index2 ) & 0x1;
  }

  for ( index2 = 0; index2 < 8; index2 ++ )
  {
    crc_temp = ( crctab[7] << 7 ) + ( crctab[6] << 6 ) + ( crctab[5] << 5 ) + ( crctab[4] << 4 ) + ( crctab[3] << 3 ) +
               ( crctab[2] << 2 ) + ( crctab[1] << 1 ) + ( crctab[0] );
    crctab[0] = ( ( data >> ( 7 - index2 ) ) & 0x1 ) ^ crctab[7];

    for ( index1 = 1; index1 < 8; index1 ++ )
    {
      crctab[index1] = ( crctab[0] & ( ( poly >> index1 ) & 0x1 ) ) ^ ( ( crc_temp >> ( index1 - 1 ) ) & 0x1 );
    }
  }

  crc = ( crctab[7] << 7 ) + ( crctab[6] << 6 ) + ( crctab[5] << 5 ) + ( crctab[4] << 4 ) + ( crctab[3] << 3 ) +
        ( crctab[2] << 2 ) + ( crctab[1] << 1 ) + ( crctab[0] << 0 );
  crc ^= 0xFF;

  value |= crc;
  return value;
}

/**
  * @brief  Verify the CRC from dataIn and extract the 8bit data value (out).
  * @param  out Reference for the extracted 8bit data value.
  * @param  dataIn 16bit value with data as MSB and CRC as LSB.
  * @retval bool It returns true if CRC is correct, false otherwise.
  */
__weak bool GAP_CRCCheck( uint8_t * out, uint16_t data_in )
{
  bool ret_val = false;
  uint8_t data = ( uint8_t )( data_in >> 8 );
  uint8_t received_crc = ( uint8_t )( data_in );
  uint8_t crc = ( uint8_t )( GAP_CRCCalculate( data, 0xFF ) ) ^ 0xFF;

  if ( crc == received_crc )
  {
    *out = data;
    ret_val = true;
  }
  return ret_val;
}

/**
  * @brief  Wait a time interval proportional to count.
  * @param  count Number of count to be waited
  * @retval none
  */
__weak void wait( uint16_t count )
{  
  volatile uint16_t wait_cnt;

  for ( wait_cnt = 0; wait_cnt < count; wait_cnt ++ )
  {
  }
}

/**
  * @brief  Return the register mask starting from it address.
  * @param  reg Register address.
  * @retval uint8_t Mask to be and-ed bit wise to data to filter it.
  */
__weak uint8_t GAP_RegMask( GAP_Registers_Handle_t reg )
{
  uint8_t ret_val;
  switch ( reg )
  {
    case CFG1:
    {
      ret_val = CFG1_REG_MASK;
    }
    break;
    case CFG2:
    {
      ret_val = CFG2_REG_MASK;
    }
    break;
    case CFG3:
    {
      ret_val = CFG3_REG_MASK;
    }
    break;
    case CFG4:
    {
      ret_val = CFG4_REG_MASK;
    }
    break;
    case CFG5:
    {
      ret_val = CFG5_REG_MASK;
    }
    break;
    case STATUS1:
    {
      ret_val = STATUS1_REG_MASK;
    }
    break;
    case STATUS2:
    {
      ret_val = STATUS2_REG_MASK;
    }
    break;
    case STATUS3:
    {
      ret_val = STATUS3_REG_MASK;
    }
    break;
    case TEST1:
    {
      ret_val = TEST1_REG_MASK;
    }
    break;
    case DIAG1:
    {
      ret_val = DIAG1_REG_MASK;
    }
    break;
    case DIAG2:
    {
      ret_val = DIAG2_REG_MASK;
    }
    break;
    default:
    {
      ret_val = 0x00;
    }
    break;
  }
  return ret_val;
}

/**
  * @brief  Read all data in the daisy chain related to register reg.
  * @param  pHandle related object of class CGAP_GDC
  * @param  pDataRead Pointer to the buffer in which will be stored the readed
  *         data. The buffer have to be provided from the caller.
  * @param  reg Register to be read. It must be one of the register defined in
  *         \link GAP_class_private_enum GAP_Registers_t\endlink.
  * @retval bool It returns false if an error occurs, otherwise return true.
  */
__weak bool GAP_ReadRegs( GAP_Handle_t * pHandle, uint8_t * pDataRead, GAP_Registers_Handle_t reg )
{
  bool ret_val = false;
  uint8_t index1;
  uint8_t device;
  uint8_t data;
  uint16_t value;

  if ( pDataRead )
  {
    value = GAP_CRCCalculate( GAP_READREG | reg, 0xFF );
    GAP_CS_Activate( pHandle );

    for ( index1 = 0; index1 < pHandle->DeviceNum; index1 ++ )
    {
      GAP_SPI_Send( pHandle, value );
    }

    GAP_CS_Deactivate( pHandle );
    wait( WAITTIME );
    value = GAP_CRCCalculate( GAP_NOP, 0xFF );
    GAP_CS_Activate( pHandle );

    ret_val = true;

    for ( index1 = 0; index1 < pHandle->DeviceNum; index1 ++ )
    {
      device = pHandle->DeviceNum - index1 - 1;

      if ( pHandle->DeviceParams[device].CFG1 & GAP_CFG1_CRC_SPI )
      {
        if ( GAP_CRCCheck( &data, GAP_SPI_Send( pHandle, value ) ) )
        {
          pDataRead[device] = data & GAP_RegMask( reg );
        }
        else
        {
          pDataRead[device] = 0x00;
          pHandle->GAP_ErrorsNow[0] |= GAP_ERROR_CODE_SPI_CRC;
          pHandle->GAP_ErrorsOccurred[0] |= GAP_ERROR_CODE_SPI_CRC;
          ret_val = false;
        }
      }
      else
      {
        pDataRead[device] = ( uint8_t )( GAP_SPI_Send( pHandle, value ) >> 8 ) & GAP_RegMask( reg );
      }
    }
    GAP_CS_Deactivate( pHandle );
  }
  return ret_val;
}

__weak void GAP_StartConfig( GAP_Handle_t * pHandle )
{
  uint8_t index;
  uint16_t value;

  GAP_SD_Activate( pHandle );

  value = GAP_CRCCalculate( GAP_STARTCONFIG, 0xFF );
  GAP_CS_Activate( pHandle );

  for ( index = 0; index < pHandle->DeviceNum; index ++ )
  {
    GAP_SPI_Send( pHandle, value );
  }
  GAP_CS_Deactivate( pHandle );
  wait( WAITTIME );
}

__weak void GAP_StopConfig( GAP_Handle_t * pHandle )
{
  uint8_t index;
  uint16_t value;

  value = GAP_CRCCalculate( GAP_STOPCONFIG, 0xFF );
  GAP_CS_Activate( pHandle );

  for ( index = 0; index < pHandle->DeviceNum; index ++ )
  {
    GAP_SPI_Send( pHandle, value );
  }
  GAP_CS_Deactivate( pHandle );
  wait( WAITTIME );
  GAP_SD_Deactivate( pHandle );
}

/**
  * @brief  Write data in the daisy chain in to the register reg.
  * @param  pHandle related object of class CGAP_GDC
  * @param  pDataWrite Pointer to the buffer in which are stored the data
  *         to be written in the register reg. The buffer have to be provided
  *         from the caller.
  * @param  reg Register to be write. It must be one of the register defined in
  *         \link GAP_class_private_enum GAP_Registers_Handle_t\endlink.
  * @retval bool It returns false if an error occurs, otherwise return true.
  */
__weak bool GAP_WriteRegs( GAP_Handle_t * pHandle, uint8_t * pDataWrite, GAP_Registers_Handle_t reg )
{
  bool ret_val = false;

  if ( pDataWrite )
  {
    uint8_t crc;
    uint8_t index;
    uint16_t value;

    value = GAP_CRCCalculate( GAP_WRITEREG | reg, 0xFF );
    crc = ( uint8_t )( value );
    GAP_CS_Activate( pHandle );

    for ( index = 0; index < pHandle->DeviceNum; index ++ )
    {
      GAP_SPI_Send( pHandle, value );
    }
    GAP_CS_Deactivate( pHandle );
    wait( WAITTIME );

    GAP_CS_Activate( pHandle );
    ret_val = true;

    for ( index = 0; index < pHandle->DeviceNum; index ++ )
    {
      value = GAP_CRCCalculate( pDataWrite[index], crc ^ 0xFF );
      GAP_SPI_Send( pHandle, value );
    }

    GAP_CS_Deactivate( pHandle );
    wait( WAITTIME );
  }
  return ret_val;
}

/**
  * @brief  Reset all the registers to the default and releases all the failure flag.
  * @param  pHandle related object of class CGDC.
  * @retval none.
  */
__weak void GAP_GlobalReset( GAP_Handle_t * pHandle )
{
  uint8_t index;
  uint16_t value;

  value = GAP_CRCCalculate( GAP_GLOBALRESET, 0xFF );
  GAP_CS_Activate( pHandle );

  for ( index = 0; index < pHandle->DeviceNum; index ++ )
  {
    GAP_SPI_Send( pHandle, value );
  }
  GAP_CS_Deactivate( pHandle );
  wait( WAITTIME );
}

/**
  * @brief  Reset selected status register.
  * @param  pHandle related object of class CGAP_GDC
  * @param  reg Register to be reset. It must be one of the STATUS register
   *        defined in \link GAP_class_private_enum GAP_Registers_Handle_t\endlink.
  * @retval bool It returns false if an error occurs, otherwise return true.
  */
__weak bool GAP_ResetStatus( GAP_Handle_t * pHandle, GAP_Registers_Handle_t reg )
{
  bool ret_val = false;
  uint8_t index;
  uint16_t value;
  GAP_SD_Activate( pHandle );
  value = GAP_CRCCalculate( GAP_RESETREG | reg, 0xFF );
  GAP_CS_Activate( pHandle );
  for ( index = 0; index < pHandle->DeviceNum; index ++ )
  {
    GAP_SPI_Send( pHandle, value );
  }
  GAP_CS_Deactivate( pHandle );
  GAP_SD_Deactivate( pHandle );
  return ret_val;
}

/**
  * @brief  ** pHandle function should be called just in IDLE state. **\n
  *         It performs the selected test on each GAP devices.
  * @param  pHandle related object of class CGDC
  * @param  testMode Test mode to be executed. It shall be one of the
  *         test modes present in the
  *         \link GAP_class_testModes GAP class test modes\endlink.
  * @retval bool It returns true if an error occurs, otherwise return false.
  */
__weak bool GAP_Test( GAP_Handle_t * pHandle, GAP_TestMode_t testMode )
{
  bool ret_val = false;
  bool invertResult = false;

  uint8_t testModeData = (uint8_t)0;
  uint8_t clr[MAX_DEVICES_NUMBER];
  uint8_t data[MAX_DEVICES_NUMBER];
  uint8_t index1, index2 = pHandle->DeviceNum;
  uint8_t statusMask = (uint8_t)0;
  uint16_t wait_cnt;

  switch ( testMode )
  {
    case GOFF_CHK:
    {
      testModeData = GAP_TEST1_GOFFCHK;
      wait_cnt = WAITTGCHK;
      statusMask = GAP_STATUS1_DESAT;
    }
    break;
    case GON_CHK:
    {
      testModeData = GAP_TEST1_GONCHK;
      wait_cnt = WAITTGCHK;
      statusMask = GAP_STATUS1_TSD;
    }
    break;
    case GAP_TEST1_DESCHK:
    {
      testModeData = DESAT_CHK;
      wait_cnt = WAITTDESATCHK;
      statusMask = GAP_STATUS1_DESAT;
      invertResult = true;
    }
    break;
    case SENSE_RESISTOR_CHK:
    {
      testModeData = GAP_TEST1_RCHK;
      wait_cnt = WAITTRCHK;
      statusMask = GAP_STATUS1_SENSE;
    }
    break;
    case SENSE_COMPARATOR_CHK:
    {
      testModeData = GAP_TEST1_SNSCHK;
      wait_cnt = WAITTSENSECHK;
      statusMask = GAP_STATUS1_SENSE;
      invertResult = true;
    }
    break;
    default:
    {
      ret_val = true;
    }
    break;
  }

  for ( index1 = 0; index1 < index2; index1 ++ )
  {
    data[index1] = testModeData;
    clr[index1] = 0x00;
  }
  GAP_WriteRegs( pHandle, data, TEST1 );
  wait( wait_cnt );
  GAP_ReadRegs( pHandle, data, STATUS1 );
  /* Clear TEST1 regs */
  GAP_WriteRegs( pHandle, clr, TEST1 );
  /* Clear STATUS1 regs */
  GAP_ResetStatus( pHandle, STATUS1 );

  for ( index1 = 0; index1 < index2; index1 ++ )
  {
    if ( invertResult )
    {
      if ( ( data[index1] & statusMask ) == 0 )
      {
        ret_val = true;
      }
    }
    else
    {
      if ( ( data[index1] & statusMask ) != 0 )
      {
        ret_val = true;
      }
    }
  }

  return ret_val;
}

/**
* @brief  This function sends a 16bit value through the configured SPI and
*         returns the 16-bit value received during communication.
* @param  pHandle_t: related object of class CGDC
* @param  value: Value to be sent through SPI
* @retval uint16_t Received 16bit value
*/
uint16_t GAP_SPI_Send( GAP_Handle_t * pHandle_t, uint16_t value )
{
  spi_tx_buffer[0] = (uint8_t)value;
  spi_tx_buffer[1] = (uint8_t)(value>>8);;
  spi_rx_buffer[0] = 0;
  spi_rx_buffer[1] = 0;
  spi_exchange(pHandle_t->Gap_Spix, spi_tx_buffer, spi_rx_buffer,
              SPI_BUFFER_SIZE);   
  return (spi_rx_buffer[1] << 8 | spi_rx_buffer[0]);
}

/**
* @brief  Deactivate SD pin
* @param  pHandle_t: related object of class CGDC
* @retval none
*/
void GAP_SD_Deactivate( GAP_Handle_t * pHandle_t )
{
  gpio_io_t sd_gap_io;
  sd_gap_io = gpio_iopack(pHandle_t->NCSPport, pHandle_t->NCSPin);
  gpio_set_pin(sd_gap_io); /* SD_GAP */
}

/**
* @brief  Activate SD pin
* @param  pHandle_t: related object of class CGDC
* @retval none
*/
void GAP_SD_Activate( GAP_Handle_t * pHandle_t )
{
  gpio_io_t sd_gap_io;
  sd_gap_io = gpio_iopack(pHandle_t->NCSPport, pHandle_t->NCSPin);
  gpio_clear_pin(sd_gap_io); /* SD_GAP */
}

/**
* @brief  Deactivate CS pin
* @param  pHandle_t: related object of class CGDC
* @retval none
*/
void GAP_CS_Deactivate( GAP_Handle_t * pHandle_t )
{
  gpio_io_t sd_gap_io;
  sd_gap_io = gpio_iopack(pHandle_t->NSDPport, pHandle_t->NSDPin);
  gpio_set_pin(sd_gap_io); /* SPIx_NSS */
}

/**
* @brief  Activate CS pin
* @param  pHandle_t: related object of class CGDC
* @retval none
*/
void GAP_CS_Activate( GAP_Handle_t * pHandle_t )
{
  gpio_io_t sd_gap_io;
  sd_gap_io = gpio_iopack(pHandle_t->NSDPport, pHandle_t->NSDPin);
  gpio_clear_pin(sd_gap_io); /* SPIx_NSS */
}

/**
  * @brief  Schedule function to be called with periodicity.
  * @retval none.
  */ 
void GAPSchedule(GAP_Handle_t * pHandle)
{
  uint8_t i;
  
  if (sch_checkerror)
  {
    /* Check errors: DIAG2 should be enabled and used as diagnostic
                     DIAG1 should be used as FAULT PIN, to STOP Immediatly */
    if (gpio_read_pin(PIN_GAP_DIAG2) == 0)
    {
        faultGap = GAP_CheckErrors(pHandle,GAP_errorsNow,GAP_errorsOccurred);
        if (faultGap)
        {
          for (i = 0; i < GAP_DEVICE_NUM; i++)
          {
            if ((GAP_errorsNow[i] != 0)||((GAP_errorsOccurred[i] != 0)))
            {
              sch_state = SCH_TEST_FAULT;
              /* Led1On(); */
            }
          }
        }
        else
        {
          sch_state = SCH_TEST_FAULT;
        }
    }
    sch_checkerror =0;
  }
}

/**
  * @brief  It returns MC_DIAG_ERROR or MC_NO_ERROR depending on
  *         external driver Diagnostic values
  * @retval uint16_t Fault code error
  */
uint16_t GAP_CheckFaultState(void)
{
  uint16_t hFault;
  uint8_t i;

  hFault = MC_NO_ERROR;
  for (i = 0; i < GAP_DEVICE_NUM; i++)
  {
    if (GAP_errorsNow[i] != 0)
    {
      hFault = MC_DIAG_ERROR;
    }
  }

  return hFault;
}

/**
 * @brief       Set Global variable to enable SPI communication to read Status registers
 *
 * @return      true if the global variable is set
 *
 * @notapi
 */
bool GAP_SetDiagRegisters(uint8_t set)
{
  if(set != 1u)
    return false;
  sch_checkerror = set;
  return true;
}

/**
 * @brief       Returns Status1, Status2 and Status3 registers struct pointer
 *
 * @return      Status registers pointer
 *
 * @notapi
 */
uint32_t* GAP_GetStatusRegisters(void)
{
    return GAP_errorsNow;
}

/*===========================================================================*/
/* IRQ Handlers                                                              */
/*===========================================================================*/

/**
 * @brief   IRQ handler function declaration.
 */

void GAP_irqfaultRecovery(void) {

    sch_checkerror = 1u;

}
/**
  * @}
  */

/**
  * @}
  */

/************************ (C) COPYRIGHT 2023 STMicroelectronics *****END OF FILE****/

