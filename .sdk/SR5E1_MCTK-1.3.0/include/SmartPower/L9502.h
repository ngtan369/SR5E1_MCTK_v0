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
*   @file    L9502.h
*   @version M4_VERSION
*   @brief   This file provides firmware functions that implement the features
*            of the L9502 component of the Stellar Motor Control.
*          
*   @details This file provides firmware functions that implement the features
*            of the L9502 component of the Stellar Motor Control.
*
 */
#ifndef __L9502_H
#define __L9502_H
#include "MC_type.h"
#include "L9502_cfg.h"

/*
 * @addtogroup STELLAR_L9502_Library
 * @{
 */
 
 /** @addtogroup STELLAR_L9502_Library
  * @{
  */

/** @defgroup L9502 Gate Driver
  *
  * @brief L9502 Gate Driver 
  *
  * @{
  */
 
// ****************************************************************************
// ******************** USER Configuration Constants **************************
// ****************************************************************************
#define L9502_DEVICES_MAX  (6+1)
#define NUMBER_OF_STATUS_REGISTERS     (6)

// ****************************************************************************
// ******************** SYSTEM Configuration Constants **************************
// ****************************************************************************
//Error messages
#define NO_ERROR                    0x00
#define ERR_WRONG_CRC               0x01
#define ERR_COUNTER_ERROR           0x02
#define ERR_SPI_ERROR               0x04
#define ERR_UNEXPECTED_FRAME        0x08
#define ERR_FRAME_ERROR             0x10
#define ERR_TIMEOUT                 0x20
#define ERR_IN_STATUS               0x80

//SPI COMMANDS
//Constants definition
#define L9502_R_W_READ             0
#define L9502_R_W_WRITE            1
#define L9502_VALID_FRAME_MASK   0x00FFFFC0
#define L9502_GLOBAL_LATCH_CLEAR 0x02U
#define L9502_MSB_DATA_SET       0x0200U
#define L9502_MSB_DATA_RESET     0x0U


// ****************************************************************************
// ***************************** Public Constants *****************************
// ****************************************************************************

//MACROS of SPI protocol definitions
#define L9502_FC(value)      ((((uint32_t)value)&0x00000001)<<00)
#define L9502_CRC(value)     ((((uint32_t)value)&0x0000001F)<<01)
#define L9502_DATA(value)    ((((uint32_t)value)&0x000003FF)<<06)
#define L9502_GSW(value)     ((((uint32_t)value)&0x00000003)<<16)
#define L9502_ADDRESS(value) ((((uint32_t)value)&0x0000001F)<<18)
#define L9502_R_W(value)     ((((uint32_t)value)&0x00000001)<<23)

/* CFG_EN_UNLOCK VALUE */
#define CFG_ENTER_1       0x55
#define CFG_ENTER_2       0x33
#define CFG_LEAVE         0xAA

/*L9502 register addresses - LV Side */
#define ADDR_CHIP_ID_LV   0x10
#define ADDR_UNLOCK_KEY   0x11
#define ADDR_CFG_8        0x12
#define ADDR_CFG_9        0x13
#define ADDR_FHM_2        0x14
#define ADDR_FHM_3        0x15

/*L9502 register addresses - HV Side */
#define ADDR_CFG_0        0x0
#define ADDR_CFG_1        0x1
#define ADDR_CFG_2        0x2
#define ADDR_CFG_3        0x3
#define ADDR_CFG_4        0x4
#define ADDR_CFG_5        0x5
#define ADDR_CFG_6        0x6
#define ADDR_CFG_7        0x7
#define ADDR_FHM_0        0x8
#define ADDR_FHM_1        0x9
#define ADDR_CFG_10       0x0B
#define ADDR_STO_CFG      0x0E
#define ADDR_CFG_11       0x0F

/* L9502 register page for CFG_10 and CFG_11 */
#define L9502_PAGE_1    (0x100)
#define L9502_PAGE_2    (0x200)
#define L9502_PAGE_3    (0x300)

/* Wait High Voltage interframe, spec 200ms */
#define L9502_WAIT_HV    200
#define L9502_WAIT_LV     20

/* These define are used in L9502_CheckErrors to mask whatever is not an error */
#ifdef VERSION_AA
#define FHM_2_ERROR_MASK (uint16_t)(0x00F)    /* To exclude op_mode which is not a fault */
#define FHM_3_ERROR_MASK (uint16_t)(0x01F)
#define FHM_3_ERROR_SHR             0
#define FHM_3_ERROR_SHL             4
#define CFG_7_ERROR_MASK (uint16_t)(0x1C0)
#define CFG_7_ERROR_SHR             6
#define CFG_7_ERROR_SHL             9
#define FHM_0_ERROR_MASK (uint16_t)(0x3FF)
#define FHM_0_ERROR_SHL             12
#define FHM_1_ERROR_MASK (uint16_t)(0x3FF)   /* To include only wtd, srr, gate ON fail AND mc */
#define FHM_1_ERROR_SHL             22
#else
#define FHM_2_ERROR_MASK (uint16_t)(0x00F)    /* To exclude ADC_DATA_NEW, OPERATION_MODE and INT_RST_LV */
#define FHM_3_ERROR_MASK (uint16_t)(0x010)    /* To exclude INP, INP_PWM, INM */
#define FHM_3_ERROR_SHR             4
#define FHM_3_ERROR_SHL             4
#define CFG_7_ERROR_MASK (uint16_t)(0x344)    /* To exclude ADC_SOC, OPERATION_MODE */
#define CFG_7_ERROR_SHR             2
#define CFG_7_ERROR_SHL             5
#define FHM_0_ERROR_MASK (uint16_t)(0x1FF)   /* To exclude INT_RST_HV */
#define FHM_0_ERROR_SHL             13
#define FHM_1_ERROR_MASK (uint16_t)(0x33F)   /* To exclude GATE_CMD and MC_COMP */
#define FHM_1_ERROR_SHL             22
#endif

/* CFG Register Masks */
#define CFG_REG_MASK       (uint16_t)(0x3FF)
#ifdef VERSION_AA
#define CFG5_REG_MASK      CFG_REG_MASK
#define CFG7_REG_MASK      (uint16_t)(0x007)    /* only first 3 bit can be configured in CFG_7 */
#else
#define CFG5_REG_MASK      (uint16_t)(0x1FF)    /* do not check bit 10 (ABIST_EN) in CFG_5 */
#define CFG7_REG_MASK      (uint16_t)(0x080)    /* only Start of Conversion in CFG_7 which is not properly a configuration bit  */
#define STO_CFG_REG_MASK   (uint16_t)(0x1FF)    /* Bit 10 GATE_RETRY is RLR */

#endif
#define CRC_DATA_REG_MASK  (uint16_t)(0x3F8)    /* exclude 3 bit of crc for CFG_0 and CFG_1*/

#define ADDR_TM_ENTER_LV  0x1F
#define ADDR_TM_ENTER_HV  0x0F

#ifndef TAKE_N_BITS_FROM
  /* Let b be any byte and p be the index (>= 0) of the bit from which you want to take n bits (>= 1) */
  #define TAKE_N_BITS_FROM(b, p, n) ((b) >> (p)) & ((1 << (n)) - 1)
#endif

/* Only CFG_0, CFG_2 and CFG_8 can be configured by graphical Interface for version AA;
 * the other registers should be modified here
 * Be aware:
 * 1. do not set 3 bit of crc for CFG_1 because it is calculated by the driver
 * 2. only 3 bit can be changed in CFG_7 */
#ifdef VERSION_AA
/* Default configuration for Register CFG_8 and CFG_9 */
#define L9502_VALUE_CFG_8    (0x204U)    /* 0b0010 0000 0100 */
#define L9502_VALUE_CFG_9    (0x015)

/* Safety Relevant HV Side Registers */
#define L9502_VALUE_CFG_0    (0x000U)    /* 0b0000 0000 0000 */
#define L9502_VALUE_CFG_1    (0x080U)    /* crc[2:0] must to be set to 0, they are calculated inside the fw */

/* Default configuration for Register:
 * CFG_0 CFG_1,CFG_2,CFG_3,CFG_4,CFG_5,CFG_6,CFG_7 */
#define L9502_VALUE_CFG_2    (0x230U)    /* 0b0010 0011 0000 */
#define L9502_VALUE_CFG_3    (0x00FU)
#define L9502_VALUE_CFG_4    (0x045U)
#define L9502_VALUE_CFG_5    (0x055U)
#define L9502_VALUE_CFG_6    (0x044U)

/* Default for Normal Register */
#define L9502_VALUE_CFG_7    (0x002U)    /* only ADCOUT_SEL[0:2] are RW bit, others are masked */
#endif
// ****************************************************************************
// ******************************* Public Types *******************************
// ****************************************************************************
#if 0
typedef enum {
    /* SPI status */
    SPI_NO_ERROR,          /* 0 no error */
    SPI_CRC_ERROR,         /* 1 CRC error */
    SPI_FRAME_CNT_ERROR,   /* 2 counter error */
    SPI_CRC_FC_ERROR       /* 3 counter and crc error */
} L9502_status_t;
#endif


/**
 * @brief L9502 operational modes
 */
typedef enum {
  RESET_0 = 0U,
  OTP_READ,
  SAFE_OFF,
  NORMAL,
  CFG
} L9502_opMode_t;

/**
 * @brief   Internal data structure type
 */
typedef struct {
  bool              testMode;           /**< @brief testMode - if test mode is enabled -true */
  uint8_t           frameCounter[L9502_DEVICES_MAX];       /**< @brief frame counter toggle with each sending */
  bool              wdgEnabled;         /**< @brief watchdog enabled */
  bool              wdgRefreshEnable;   /**< @brief watchdog refresh enabled */
  bool              faultFlag;          /**< @brief fault Flag - asserter when FS flag is LOW */
  bool              initStatus;         /**< @brief init status - asserter when driver is successfully initialized */
  L9502_opMode_t    opMode[L9502_DEVICES_MAX];             /**< @brief operation mode */
} L9502_internalData_t;

#if 0
//L9502 SPI Frame Structure
typedef union{
    uint32_t R;
    struct{
        uint32_t res:           8; /* reserved */
        uint32_t R_W:           1;
        uint32_t Address:       5;
        uint32_t GSW:           2;
        uint32_t DATA:         10;
        uint32_t CRC:           5;
        uint32_t FC:            1;
    }B;
    struct{
        uint32_t res:           8; //NA reserved
        uint32_t SPI_ERR:       1;  //
        uint32_t Address:       5;  //
        uint32_t GSW:           2;  //
        uint32_t DATA:         10;  //
        uint32_t CRC:           5;  //
        uint32_t FC:            1;  //
    }RX_B;
}L9502_Frame_t;
#endif

typedef union{
    uint32_t R;
    struct{
        uint32_t FC:            1;        
        uint32_t CRC_F:         5;        
        uint32_t DATA:         10;        
        uint32_t GSW:           2;
        uint32_t Address:       5;
        uint32_t R_W:           1;        
        uint32_t res:           8; /* reserved */
    }B;
    struct{
        uint32_t FC:            1;  //
        uint32_t CRC_F:         5;  //
        uint32_t DATA:         10;  //
        uint32_t GSW:           2;  //
        uint32_t Address:       5;  //
        uint32_t SPI_ERR:       1;  //
        uint32_t res:           8; //NA reserved                
    }RX_B;
}L9502_Frame_t;

#if 0
typedef union{
    uint16_t R;
    struct{
        uint16_t reserved:                    7;  /* [bit9, bit15] reserved */
        uint16_t LAST_FRAME_NOK:              1;  //bit8 An error occurred during last receive
        uint16_t INITIALIZATION_ERROR:        1;  //bit7 Init Function was not passed correctly
        uint16_t REG_READ_ERROR:              1;  //bit6 An error occurred during register read
        uint16_t TIMEOUT_ERROR:               1;  //bit5 timeout error
        uint16_t ERROR_FRAME_RECEIVED:        1;  //bit4 Error frame received
        uint16_t UNEXPECTED_FRAME:            1;  //bit3 Unknown or unexpected frame received
        uint16_t SPI_ERROR:                   1;  //bit2 SPI_ERROR bit in the received frame
        uint16_t COUNTER_ERROR:               1;  //bit1 Wrong COUNTER received
        uint16_t CRC_ERROR:                   1;  //bit0 Wrong CRC received
    }B;
}L9502_ERRstatus_t;
#endif
typedef union{
    uint16_t R;
    struct{
        uint16_t CRC_ERROR:                   1;  //bit0 Wrong CRC received
        uint16_t COUNTER_ERROR:               1;  //bit1 Wrong COUNTER received        
        uint16_t SPI_ERROR:                   1;  //bit2 SPI_ERROR bit in the received frame        
        uint16_t UNEXPECTED_FRAME:            1;  //bit3 Unknown or unexpected frame received        
        uint16_t ERROR_FRAME_RECEIVED:        1;  //bit4 Error frame received        
        uint16_t TIMEOUT_ERROR:               1;  //bit5 timeout error        
        uint16_t REG_READ_ERROR:              1;  //bit6 An error occurred during register read        
        uint16_t INITIALIZATION_ERROR:        1;  //bit7 Init Function was not passed correctly        
        uint16_t LAST_FRAME_NOK:              1;  //bit8 An error occurred during last receive        
        uint16_t reserved:                    7;  /* [bit9, bit15] reserved */        
    }B;
}L9502_ERRstatus_t;


typedef const struct
{
  /* Low Voltage Side */
  uint16_t hCFG_8;  /*!< Configuration value for CFG_8 register. */
  uint16_t hCFG_9;  /*!< Configuration value for CFG_9 register. */
  /* High Voltage Side - Safety Relevant Registers (SRR) */
  uint16_t hCFG_0;  /*!< Configuration value for CFG_0 register. */
  uint16_t hCFG_1;  /*!< Configuration value for CFG_1 register. */
  /* High Voltage Side - Safety Latent Registers (SLR) */
  uint16_t hCFG_2;  /*!< Configuration value for CFG_2 register. */
  uint16_t hCFG_3;  /*!< Configuration value for CFG_3 register. */
  uint16_t hCFG_4;  /*!< Configuration value for CFG_4 register. */
  uint16_t hCFG_5;  /*!< Configuration value for CFG_5 register. */
  uint16_t hCFG_6;  /*!< Configuration value for CFG_6 register. */
  /* High Voltage Side - Non Safety Relevant Registers (NSR) */
  uint16_t hCFG_7;  /*!< Configuration value for CFG_7 register. */
#if defined(VERSION_BA) || defined(VERSION_CA) || defined(VERSION_CD)
  uint16_t hCFG_10_P1;   /*!< Configuration value for CFG_10 register Page 1. */
  uint16_t hCFG_10_P2;   /*!< Configuration value for CFG_10 register Page 2. */
  uint16_t hCFG_10_P3;   /*!< Configuration value for CFG_10 register Page 3. */
  uint16_t hCFG_11_P1;   /*!< Configuration value for CFG_11 register Page 1. */
  uint16_t hCFG_11_P2;   /*!< Configuration value for CFG_11 register Page 2. */
  uint16_t hCFG_11_P3;   /*!< Configuration value for CFG_11 register Page 3. */
  uint16_t hSTO_CFG;  /*!< Configuration value for STO_CFG register. */
#endif
} L9502DeviceParams_t, *pL9502DeviceParams_t;

/**
 * @brief   Driver configuration structure.
 */
typedef struct {
  union
  {
    struct
    {

      /**
         * @brief   FHM_1 Register: Address = 0x9U
         */
      union {
        vuint16_t R;          /* Stands for Register */
        struct {
          vuint16_t MILLER_CLAMP: 1; /**< @brief Miller Clamp fault (Driver not completely off) */
          vuint16_t GATE_ON_FAIL: 1; /**< @brief Gate on fault (Driver not completely on) */          
          vuint16_t SRR_FAIL:     1; /**< @brief SRR Data CRC Check */          
          vuint16_t WTD_FAIL:     1; /**< @brief watch dog for internal communication */          
          vuint16_t VCE_CLAMP:    1; /**< @brief vce clamp comparator */          
          vuint16_t SENSE_OPEN:   1; /**< @brief over temperature on TSEN pad */          
          vuint16_t MC_COMP:      1; /**< @brief Miller Clamp comparator */          
          vuint16_t GATE_CMD:     1; /**< @brief status of pre-driver gate command */          
          vuint16_t BIST_FAIL:    1; /**< @brief BIST fail */          
          vuint16_t OTP_FAIL:     1; /**< @brief OTP fail */
          vuint16_t UNUSED_0:     6; /**< @brief Unused */
        } B;                  /* Stands for Bit */
      } hFHM_1;

      /**
         * @brief   FHM_0 Register: Address = 0x8U
         */
      union {
        vuint16_t R;          /* Stands for Register */
        struct {
          vuint16_t CLK_MON_ERR:  1; /**< @brief oscillator frequency error and diag clock status */
          vuint16_t OTM_WR:       1; /**< @brief Thermal warning */          
          vuint16_t OTM_SD:       1; /**< @brief Thermal shutdown */          
          vuint16_t VL_UV:        1; /**< @brief Under-voltage at VL pin */          
          vuint16_t VL_OV:        1; /**< @brief Over-voltage at VL pin */          
          vuint16_t VH_UV:        1; /**< @brief Under-voltage at VH pin */          
          vuint16_t VH_OV:        1; /**< @brief Over-voltage at VH pin */          
          vuint16_t SENSE:        1; /**< @brief Overcurrent fault through SENSE diagnosis */          
          vuint16_t DESAT:        1; /**< @brief Overcurrent fault through DESAT diagnosis */          
          vuint16_t INT_RST_HV:   1; /**< @brief power on reset event  */          
          vuint16_t UNUSED_0:     6; /**< @brief Unused */          
        } B;                  /* Stands for Bit */
      } hFHM_0;

#if defined(VERSION_BA) || defined(VERSION_CA) || defined(VERSION_CD)
      /**
         * @brief   CFG_7 Register: Address = 0x7U
         */
      union {
        vuint16_t R;          /* Stands for Register */
        struct {
          vuint16_t UNUSED_0:       2; /**< @brief Unused */
          vuint16_t SHOOT_THROUGH_COMP:   1; /**< @brief status of pin brake 5v */          
          vuint16_t OPERATION_MODE:       3; /**< @brief High Voltage Operational Mode */          
          vuint16_t BRAKE:          1; /**< @brief status of pin BRAKE */          
          vuint16_t ADC_SOC:        1; /**< @brief ADC start of conversion */          
          vuint16_t VTREF_CL:       1; /**< @brief vtref regulator current limitation fault */          
          vuint16_t ISEN_OVC:       1; /**< @brief sense overcurrent fault */          
          vuint16_t UNUSED_1:       6; /**< @brief Unused */          
        } B;                  /* Stands for Bit */
      } hCFG_7;

      /**
         * @brief   STO_CFG Register: Address = 0xEU
         */
      union {
        vuint16_t R;          /* Stands for Register */
        struct {
          vuint16_t STO_VTH_CFG:    3; /**< @brief STO threshold */
          vuint16_t STO_T2LTO_CFG:  3; /**< @brief STO T2LTO time */          
          vuint16_t STO_TSTO_CFG:   3; /**< @brief STO TSTO time */          
          vuint16_t GATE_RETRY:     1; /**< @brief gate retry failure */          
          vuint16_t UNUSED_1:       6; /**< @brief Unused */          
        } B;                  /* Stands for Bit */
      } hSTO_CFG;

#else
      /**
         * @brief   CFG_7 Register: Address = 0x7U
         */
      union {
        vuint16_t R;          /* Stands for Register */
        struct {
          vuint16_t ADCOUT_SEL:   3; /**< @brief  */
          vuint16_t OP_MODE:      3; /**< @brief  */          
          vuint16_t ASC:          1; /**< @brief  */          
          vuint16_t FAULT_OTHERS: 1; /**< @brief  */          
          vuint16_t FAULT_DESAT_SENSE:        1; /**< @brief  */
          vuint16_t UNUSED_0:     7; /**< @brief  */         
        } B;                  /* Stands for Bit */
      } hCFG_7;
#endif
      /**
         * @brief   FHM_2 Register: Address = 0x14U
         */
      union {
        vuint16_t R;          /* Stands for Register */
        struct {
          vuint16_t FLY_OVC:      1; /**< @brief Flyback Overcurrent */
          vuint16_t WDG_FAIL:     1; /**< @brief Watchdog for internal communication */          
          vuint16_t SRR_FAIL:     1; /**< @brief SRR Data CRC Check */          
          vuint16_t OTP_FAIL:     1; /**< @brief OTP Fail */          
          vuint16_t INT_RST_LV:   1; /**< @brief power on reset active */          
          vuint16_t OP_MODE:      3; /**< @brief Low Voltage Operational Mode */          
#ifdef VERSION_AA
          vuint16_t UNUSED_0:     8; /**< @brief  Unused */
#else
          vuint16_t ADC_DATA_NEW: 1; /**< @brief new sample received */    
          vuint16_t UNUSED_0:     7; /**< @brief  Unused */
#endif
        } B;                  /* Stands for Bit */
      } hFHM_2;

      /**
         * @brief   FHM_3 Register: Address = 0x15U
         */
      union {
        vuint16_t R;          /* Stands for Register */
        struct {
#ifdef VERSION_AA
          vuint16_t FAULT_SAFE:   1; /**< @brief  */
#else
          vuint16_t UNUSED_1:       1; /**< @brief Unused */
#endif
          vuint16_t INM:          1; /**< @brief status of pin IN- */            
          vuint16_t INP_PWM:      1; /**< @brief status of pin IN PWM */            
          vuint16_t INP:          1; /**< @brief status of pin IN+ */          
          vuint16_t DIS:          1; /**< @brief status of pin DIS */          
#ifdef VERSION_AA
          vuint16_t UNUSED_0:    11; /**< @brief Unused */
#else
          vuint16_t UNUSED:    11; /**< @brief Unused */
#endif

        } B;                  /* Stands for Bit */
      } hFHM_3;

    } REGS;
    vuint16_t StatRegs[NUMBER_OF_STATUS_REGISTERS];
  };
} L9502_statusReg_t;



#if 0
/**
 * @brief   Driver configuration structure.
 */
typedef struct {
  union
  {
    struct
    {

      /**
         * @brief   FHM_1 Register: Address = 0x9U
         */
      union {
        vuint16_t R;          /* Stands for Register */
        struct {
          vuint16_t UNUSED_0:     6; /**< @brief Unused */
          vuint16_t OTP_FAIL:     1; /**< @brief OTP fail */
          vuint16_t BIST_FAIL:    1; /**< @brief BIST fail */
          vuint16_t GATE_CMD:     1; /**< @brief status of pre-driver gate command */
          vuint16_t MC_COMP:      1; /**< @brief Miller Clamp comparator */
          vuint16_t SENSE_OPEN:   1; /**< @brief over temperature on TSEN pad */
          vuint16_t VCE_CLAMP:    1; /**< @brief vce clamp comparator */
          vuint16_t WTD_FAIL:     1; /**< @brief watch dog for internal communication */
          vuint16_t SRR_FAIL:     1; /**< @brief SRR Data CRC Check */
          vuint16_t GATE_ON_FAIL: 1; /**< @brief Gate on fault (Driver not completely on) */
          vuint16_t MILLER_CLAMP: 1; /**< @brief Miller Clamp fault (Driver not completely off) */
        } B;                  /* Stands for Bit */
      } hFHM_1;

      /**
         * @brief   FHM_0 Register: Address = 0x8U
         */
      union {
        vuint16_t R;          /* Stands for Register */
        struct {
          vuint16_t UNUSED_0:     6; /**< @brief Unused */
          vuint16_t INT_RST_HV:   1; /**< @brief power on reset event  */
          vuint16_t DESAT:        1; /**< @brief Overcurrent fault through DESAT diagnosis */
          vuint16_t SENSE:        1; /**< @brief Overcurrent fault through SENSE diagnosis */
          vuint16_t VH_OV:        1; /**< @brief Over-voltage at VH pin */
          vuint16_t VH_UV:        1; /**< @brief Under-voltage at VH pin */
          vuint16_t VL_OV:        1; /**< @brief Over-voltage at VL pin */
          vuint16_t VL_UV:        1; /**< @brief Under-voltage at VL pin */
          vuint16_t OTM_SD:       1; /**< @brief Thermal shutdown */
          vuint16_t OTM_WR:       1; /**< @brief Thermal warning */
          vuint16_t CLK_MON_ERR:  1; /**< @brief oscillator frequency error and diag clock status */
        } B;                  /* Stands for Bit */
      } hFHM_0;

#if defined(VERSION_BA) || defined(VERSION_CA) || defined(VERSION_CD)
      /**
         * @brief   CFG_7 Register: Address = 0x7U
         */
      union {
        vuint16_t R;          /* Stands for Register */
        struct {
          vuint16_t UNUSED_1:       6; /**< @brief Unused */
          vuint16_t ISEN_OVC:       1; /**< @brief sense overcurrent fault */
          vuint16_t VTREF_CL:       1; /**< @brief vtref regulator current limitation fault */
          vuint16_t ADC_SOC:        1; /**< @brief ADC start of conversion */
          vuint16_t BRAKE:          1; /**< @brief status of pin BRAKE */
          vuint16_t OPERATION_MODE:       3; /**< @brief High Voltage Operational Mode */
          vuint16_t SHOOT_THROUGH_COMP:   1; /**< @brief status of pin brake 5v */
          vuint16_t UNUSED_0:       2; /**< @brief Unused */
        } B;                  /* Stands for Bit */
      } hCFG_7;

      /**
         * @brief   STO_CFG Register: Address = 0xEU
         */
      union {
        vuint16_t R;          /* Stands for Register */
        struct {
          vuint16_t UNUSED_1:       6; /**< @brief Unused */
          vuint16_t GATE_RETRY:     1; /**< @brief gate retry failure */
          vuint16_t STO_TSTO_CFG:   3; /**< @brief STO TSTO time */
          vuint16_t STO_T2LTO_CFG:  3; /**< @brief STO T2LTO time */
          vuint16_t STO_VTH_CFG:    3; /**< @brief STO threshold */
        } B;                  /* Stands for Bit */
      } hSTO_CFG;

#else
      /**
         * @brief   CFG_7 Register: Address = 0x7U
         */
      union {
        vuint16_t R;          /* Stands for Register */
        struct {
          vuint16_t UNUSED_0:     7; /**< @brief  */
          vuint16_t FAULT_DESAT_SENSE:        1; /**< @brief  */
          vuint16_t FAULT_OTHERS: 1; /**< @brief  */
          vuint16_t ASC:          1; /**< @brief  */
          vuint16_t OP_MODE:      3; /**< @brief  */
          vuint16_t ADCOUT_SEL:   3; /**< @brief  */
        } B;                  /* Stands for Bit */
      } hCFG_7;
#endif
      /**
         * @brief   FHM_2 Register: Address = 0x14U
         */
      union {
        vuint16_t R;          /* Stands for Register */
        struct {
#ifdef VERSION_AA
          vuint16_t UNUSED_0:     8; /**< @brief  Unused */
#else
          vuint16_t UNUSED_0:     7; /**< @brief  Unused */
          vuint16_t ADC_DATA_NEW: 1; /**< @brief new sample received */
#endif
          vuint16_t OP_MODE:      3; /**< @brief Low Voltage Operational Mode */
          vuint16_t INT_RST_LV:   1; /**< @brief power on reset active */
          vuint16_t OTP_FAIL:     1; /**< @brief OTP Fail */
          vuint16_t SRR_FAIL:     1; /**< @brief SRR Data CRC Check */
          vuint16_t WDG_FAIL:     1; /**< @brief Watchdog for internal communication */
          vuint16_t FLY_OVC:      1; /**< @brief Flyback Overcurrent */
        } B;                  /* Stands for Bit */
      } hFHM_2;

      /**
         * @brief   FHM_3 Register: Address = 0x15U
         */
      union {
        vuint16_t R;          /* Stands for Register */
        struct {
#ifdef VERSION_AA
          vuint16_t UNUSED_0:    11; /**< @brief Unused */
#else
          vuint16_t UNUSED_0:    12; /**< @brief Unused */
#endif
          vuint16_t DIS:          1; /**< @brief status of pin DIS */
          vuint16_t INP:          1; /**< @brief status of pin IN+ */
          vuint16_t INP_PWM:      1; /**< @brief status of pin IN PWM */
          vuint16_t INM:          1; /**< @brief status of pin IN- */
#ifdef VERSION_AA
          vuint16_t FAULT_SAFE:   1; /**< @brief  */
#endif
        } B;                  /* Stands for Bit */
      } hFHM_3;

    } REGS;
    vuint16_t StatRegs[NUMBER_OF_STATUS_REGISTERS];
  };
} L9502_statusReg_t;
#endif

typedef const struct
{

  uint8_t NumberOfDevices; /*!< Number of GAP used in the daisy chain */
  pL9502DeviceParams_t deviceParams[L9502_DEVICES_MAX]; /*!< Device parameters pointers */
} L9502Params_t, *pL9502Params_t;

//************  Public Variables  **************
extern L9502_ERRstatus_t L9502_ERRstatus[];
//extern L9502Params_t L9502Config;

//************  Public Functions **************

uint16_t  L9502_SendFrame(L9502_Frame_t * TxFrame_p, L9502_Frame_t * RxFrame_p);
uint32_t  L9502_EnterTestModeLV(uint8_t device);
uint32_t  L9502_ExitTestModeLV(uint8_t device);
uint32_t  L9502_EnterTestModeHV(uint8_t device);
uint32_t  L9502_ExitTestModeHV(uint8_t device);

void  L9502_EnterConfigMode(uint8_t device);
void  L9502_ExitConfigMode(uint8_t device);

void  L9502_ClearBuffer(void);
uint16_t  L9502_ReadSingleDev(uint16_t device, uint16_t address, L9502_Frame_t *RxFrame);
uint16_t  L9502_ReadPageSingleDev(uint16_t device, uint16_t address, uint16_t page, L9502_Frame_t *RxFrame);
//void  L9502_PutSingleCommand(uint16_t device, uint16_t address, uint16_t data, L9502_Frame_t *RxFrame);
uint16_t  L9502_ReadMultipleDev(uint16_t number, uint16_t address, L9502_Frame_t *RxFrame);
#if  defined(VERSION_CA) || defined(VERSION_CD) 
uint16_t  L9502_ReadMultipleDevRlr(uint16_t number, uint16_t address, L9502_Frame_t *RxFrame, uint8_t rlr);
#endif
uint16_t  L9502_ReadPageMultipleDev(uint16_t number, uint16_t address,uint16_t page, L9502_Frame_t *RxFrame);
uint16_t  L9502_WriteSingleDev(uint16_t device, uint16_t address, uint16_t data, L9502_Frame_t *RxFrame);
//uint16_t  L9502_WriteMultipleDev(uint16_t device, uint16_t address, uint16_t data, L9502_Frame_t *RxFrame);
uint16_t  L9502_Init(void);

uint16_t L9502_GetChipID(uint16_t device);
uint16_t L9502_UpdateOpMode(uint16_t device);
L9502_opMode_t L9502_GetOpMode(uint16_t device);

void L9502_Schedule(void);
L9502_statusReg_t* L9502_GetStatusRegisters(uint16_t device);
uint16_t L9502_CheckFaultState(void);
bool L9502_SetDiagRegisters(uint8_t set);

uint16_t L9502_FaultRecovery(void);

uint32_t  L9502_WriteHVSingleDev(uint8_t device, uint16_t address, uint16_t value);
uint32_t  L9502_WriteLVSingleDev(uint8_t device, uint16_t address, uint16_t value);
uint32_t  L9502_WritePageSingleDev(uint8_t device, uint16_t page, uint16_t address, uint16_t value);
void L9502_ADCStartConversion(uint8_t device);
void L9502_Force_FAULT1(uint8_t enable);
void L9502_irqfaultRecovery(void);
#endif /* __L9502_H */


