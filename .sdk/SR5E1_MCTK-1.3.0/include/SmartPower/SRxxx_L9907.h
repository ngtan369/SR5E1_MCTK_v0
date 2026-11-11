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
*   @file    SRxxx_L9907.h
*   @version M4_VERSION
*   @brief   This file provides firmware functions that implement the features
*            of the L9907 component of the Stellar Motor Control.
*          
*   @details This file provides firmware functions that implement the features
*            of the L9907 component of the Stellar Motor Control.
 */
#ifndef __SRxxx_L9907_H__
#define __SRxxx_L9907_H__

#include <stdbool.h>
#include "mc_type.h"
#include "SRxxx_L9907_cfg.h"
#include <test_env.h>

/** @addtogroup STELLAR_L9907_Library
  * @{
  */


/** @defgroup L9907_class_exported_types L9907 class exported types
* @{
*/

/*===========================================================================*/
/* Driver macros.                                                            */
/*===========================================================================*/

/*===========================================================================*/
/* Driver register addresses                                                 */
/*===========================================================================*/

/*===========================================================================*/
/* Driver pre-compile register settings.                                     */
/*===========================================================================*/

/**
 * @brief   L9907 Register ID Addresses for SPI communication
 */
#define L9907_CMD0_ADDRESS           0x0U
#define L9907_CMD1_ADDRESS           0x1U
#define L9907_CMD2_ADDRESS           0x2U
#define L9907_CMD3_ADDRESS           0x3U
#define L9907_CMD4_ADDRESS           0x4U
#define L9907_DIAG_ADDRESS           0x6U
#define L9907_DIAG2_ADDRESS          0x7U

/**
 * @brief   Generic identifiers and corresponding port of the ENABLER pin of the gate drivers
 */
#define E1_GPIO                      L9907_EN1
#define E2_GPIO                      L9907_EN2

/**
 * @brief   Gate drivers current gain identifiers
 */
#define L9907_25_PER_CENT           0x0U
#define L9907_50_PER_CENT           0x1U
#define L9907_75_PER_CENT           0x2U
#define L9907_100_PER_CENT          0x3U

/**
 * @brief   Dead Time identifiers
 */
#define L9907_100_200_NS            0x0U
#define L9907_300_500_NS            0x1U
#define L9907_700_1000_NS           0x2U
#define L9907_1000_1500_NS          0x3U

/**
 * @brief   Current sense amplifier gain
 */
#define L9907_10                    0x0U
#define L9907_30                    0x1U
#define L9907_50                    0x2U
#define L9907_100                   0x3U

/**
 * @brief   Power Mos short circuit detection threshold
 */
#define L9907_900_1100_MV           0x0U
#define L9907_1170_1430_MV          0x1U
#define L9907_1440_1760_MV          0x2U
#define L9907_1710_2090_MV          0x3U

/**
 * @brief   Over-voltage battery application
 */
#define L9907_18_25_V               0x2U
#define L9907_35_42_V               0x1U

/**
 * @brief   Vcc over-voltage threshold
 */
#define L9907_3300_MV               0x2U
#define L9907_5000_MV               0x1U

/**
 * @brief   Default Register words
 */
/* DO NOT CHANGE THE DEFAULT WORDS. */
#define L9907_CMD0_DEFAULT           0x0400U    /* 0b0000010000000000 */
#define L9907_CMD1_DEFAULT           0x3706U    /* 0b0011011100000110 */
#define L9907_CMD2_DEFAULT           0x5E00U    /* 0b0101111000000000 */
#define L9907_CMD3_DEFAULT           0x6E00U    /* 0b0110111000000000 */
#define L9907_CMD4_DEFAULT           0x8FFFU    /* 0b1000111111111111 */
#define L9907_DIAG_DEFAULT           0xCC01U    /* 0b1100110000000001 */
#define L9907_DIAG2_DEFAULT          0xEC00U    /* 0b1110110000000000 */


#define L9907_ERROR_CODE_FROM_DEVICE_MASK (uint32_t)(0x00000000)

/*===========================================================================*/
/* Driver data structures and types.                                         */
/*===========================================================================*/

/**
 * @brief   Gain, time, and threshold profile
 */
typedef enum {
    PROFILE_1, PROFILE_2, PROFILE_3, PROFILE_4
} L9907_profile;

/**
 * @brief   Register ID name
 */
typedef enum {
    CMD0, CMD1, CMD2, CMD3, CMD4, DIAG, DIAG2
} L9907_regID;

/**
 * @brief   Communication and setting errors
 */
typedef enum {
    SPI_L9907_ERROR_NO_SUCH_REGISTER,
    SPI_L9907_SUCCESS,
    SPI_L9907_NO_WRITABLE_REGISTER,
    SPI_L9907_INVALID_SETTING,
    SPI_L9907_GENERAL_ERROR
} L9907_spiState;

/**
 * @brief   Fault effect types
 */
typedef enum {
    THSD,
    VBOV,
    VBUV,
    VCCOV,
    VCCUV,
    UV_HS,
    UV_LS,
    VSCHS1,
    VSCHS2,
    VSCHS3,
    VSCLS1,
    VSCLS2,
    VSCLS3
} L9907_faultEffect;

/**
 * @brief   Gate drivers' enablers
 */
typedef enum {
    EN1, EN2
} L9907_enabler;

/**
 * @brief   Driver configuration structure.
 */
typedef struct {

      /**
       * @brief   CMDO Register - Driver settings: ADD = 000
       */
    union {
        volatile uint16_t R;          /* Stands for Register */
        struct {
            volatile uint16_t OF1: 1; /* CSA1 offset */
            volatile uint16_t G1:  2; /* CSA2 gain (G20-G21) */
            volatile uint16_t OF2: 1; /* CSA2 offset */
            volatile uint16_t G2:  2; /* CSA2 gain (G20-G21) */
            volatile uint16_t IG:  2; /* Gate driver current gain for whatever reference (IG_0-IG_1) */
            volatile uint16_t DT:  2; /* Dead Time (DT0-DT1) */
            volatile uint16_t WE:  1; /* Write enable (B10) */
            volatile uint16_t PAR: 1; /* Parity bit (B11) */
            volatile uint16_t ULB: 1; /* Useless bit (B12) */
            volatile uint16_t ADD: 3; /* Address (B13-B15) */
        } B;                  /* Stands for Bit */
    } CMD0;

      /**
       * @brief   CMD1 Register - Diagnostic settings: ADD = 001
       */
    union{
        volatile uint16_t R;              /* Stands for Register */
        struct {
            volatile uint16_t VCCOV:   2; /* Over voltage detect. threshold (B1-B0) */
            volatile uint16_t VBOV:    2; /* Over voltage detect. threshold (B3-B2) */
            volatile uint16_t SCHS1:   2; /* Short circuit detect. threshold (SC_HS0-SC_HS1) */
            volatile uint16_t SCLS1:   2; /* Short circuit detect. threshold (SC_LS0-SC_LS1) */
            volatile uint16_t E_VBUV:  1; /* Enable VBuv protect. (B8) */
            volatile uint16_t E_VBOV:  1; /* Enable VBov protect. (B9) */
            volatile uint16_t WE:      1; /* Write enable (B10) */
            volatile uint16_t PAR:     1; /* Parity bit (B11) */
            volatile uint16_t E_THSD:  1; /* Enable over temperature protection (B12) */
            volatile uint16_t ADD:     3; /* Address (B13-B15) */
        } B;                      /* Stands for Bit */
    } CMD1;

     /**
      * @brief   CMD2 Register - Test Mode Selections: ADD = 010
      */
    union{
        volatile uint16_t R;              /* Stands for Register */
        struct {
            volatile uint16_t BUL:     5; /* Useless bits (B4-B0) */
            volatile uint16_t SHFET:   1; /* Over voltage detect. threshold (B5) */
            volatile uint16_t VOVTST:  1; /* Activates test function for for VCC over voltage level (B6) */
            volatile uint16_t VSCTST:  1; /* Activates test function for short circuit level (B7) */
            volatile uint16_t SHT_PH:  1; /* Inhibits only the shot through phase (B8) */
            volatile uint16_t E_BST:   1; /* Switch off BST_CLK if BST_DIS is high (B9) */
            volatile uint16_t WE:      1; /* Write enable (B10) */
            volatile uint16_t PAR:     1; /* Parity bit (B11) */
            volatile uint16_t GCR:     1; /* Select internal gate driver's current reference (B12) */
            volatile uint16_t ADD:     3; /* Address (B13-B15) */
        } B;                      /* Stands for Bit */
    } CMD2;

    /**
     * @brief   CMD3 Register - Current Sense amplifier offset calibration: ADD = 011
     */
    union{
        volatile uint16_t R;              /* Stands for Register */
        struct {
            volatile uint16_t TRIM1:   4; /* CSA1 calibration offset (B5-B0) */
            volatile uint16_t SIGN1:   1; /* Sign of the offset 1 */
            volatile uint16_t TRIM2:   4; /* CSA2 calibration offset (B9-B5) */
            volatile uint16_t SIGN2:   1; /* Sign of the offset 2 */
            volatile uint16_t WE:      1; /* Write enable (B10) */
            volatile uint16_t PAR:     1; /* Parity bit (B11) */
            volatile uint16_t D_BSTOV: 1; /* Disable the over voltage protection on BST_C (B12) */
            volatile uint16_t ADD:     3; /* Address (B13-B15) */
        } B;                      /* Stands for Bit */
    } CMD3;

    /**
     * @brief   CMD4 Register - Fault effect enabling: ADD = 100
     */
        union{
            volatile uint16_t R;               /* Stands for Register */
            struct {
                volatile uint16_t EN_VSCL3: 1; /* Enables VSCL3 fault effect in case of the relative fault detect. (B0) */
                volatile uint16_t EN_VSCL2: 1; /* Enables VSCL2 fault effect in case of the relative fault detect. (B1) */
                volatile uint16_t EN_VSCL1: 1; /* Enables VSCL1 fault effect in case of the relative fault detect. (B2) */
                volatile uint16_t EN_VSCH3: 1; /* Enables VSCH3 fault effect in case of the relative fault detect. (B3) */
                volatile uint16_t EN_VSCH2: 1; /* Enables VSCH2 fault effect in case of the relative fault detect. (B4) */
                volatile uint16_t EN_VSCH1: 1; /* Enables VSCH1 fault effect in case of the relative fault detect. (B5) */
                volatile uint16_t EN_UV_LS: 1; /* Enables UV_LS fault effect in case of the relative fault detect. (B6) */
                volatile uint16_t EN_UV_HS: 1; /* Enables UV_HS fault effect in case of the relative fault detect. (B7) */
                volatile uint16_t EN_VCCUV: 1; /* Enables VCCUV fault effect in case of the relative fault detect. (B8) */
                volatile uint16_t EN_VCCOV: 1; /* Enables VCCOV fault effect in case of the relative fault detect. (B9) */
                volatile uint16_t WE:       1; /* Write enable (B10) */
                volatile uint16_t PAR:      1; /* Parity bit (B11) */
                volatile uint16_t E_REGOFF: 1; /* Disable the over voltage protection on BST_C (B12) */
                volatile uint16_t ADD:      3; /* Address (B13-B15) */
            } B;                       /* Stands for Bit */
        } CMD4;

    /**
     * @brief   DIAG Register - Diagnostic register number 1: ADD = 110
     */
        union{
            volatile uint16_t R;               /* Stands for Register */
            struct {
              volatile uint16_t VSCL3:    1; /* Short Circuit low side 3 (B0) */
              volatile uint16_t VSCL2:    1; /* Short Circuit low side 2 (B1) */
              volatile uint16_t VSCL1:    1; /* Short Circuit low side 1 (B2) */
              volatile uint16_t VSCH3:    1; /* Short Circuit high side 3 (B3) */
              volatile uint16_t VSCH2:    1; /* Short Circuit high side 2 (B4) */
              volatile uint16_t VSCH1:    1; /* Short Circuit high side 1 (B5) */
              volatile uint16_t UV_LS:    1; /* Under voltage low side (B6) */
              volatile uint16_t UV_HS:    1; /* Under Voltage high side (B7) */
              volatile uint16_t VCCUV:    1; /* Vcc under voltage (B8) */
              volatile uint16_t VCCOV:    1; /* Vcc over voltage (B9) */
              volatile uint16_t VBUV:     1; /* VB under voltage (B10) */
              volatile uint16_t VBOV:     1; /* VB over-voltage (B11) */
              volatile uint16_t THSD:     1; /* Thermal shut down (B12) */
              volatile uint16_t ADD:      3; /* Address (B13-B15) */
            } B;                       /* Stands for Bit */
        } DIAG;

    /**
     * @brief    DIAG2 Register - Diagnostic register number 2: ADD = 111
     */
        union{
            volatile uint16_t R;               /* Stands for Register */
            struct {
              volatile uint16_t SHT1:             1; /* Info about BST_DIS pin status (B0) */
              volatile uint16_t SHT2:             1; /* Info about BST_DIS pin status (B1) */
              volatile uint16_t SHT3:             1; /* Info about BST_DIS pin status (B2) */
              volatile uint16_t GCR_STG:          1; /* Info about BST_DIS pin status (B3) */
              volatile uint16_t GCR_OL:           1; /* Info about GCR_OL pin status (B4) */
              volatile uint16_t BST_C_OV:         1; /* Info about BST_OV over-voltage level status (B5) */
              volatile uint16_t REGOFF_RB:        1; /* Info about REG_OFF procedure status (B6) */
              volatile uint16_t AND_E1_E2_RB:     1; /* Info about AND(E1,E2) status (B7) */
              volatile uint16_t EN1_RB:           1; /* Info about EN1 pin status (B8) */
              volatile uint16_t BSTDS_RB:         1; /* Info about BST_DIS pin status (B9) */
              volatile uint16_t ULB3:             1; /* Don't care (B10) */
              volatile uint16_t ULB2:             1; /* Don't care (B11) */
              volatile uint16_t ULB1:             1; /* Don't care (B12) */
              volatile uint16_t ADD:              3; /* Address (B13-B15) */
            } B;                       /* Stands for Bit */
        } DIAG2;

} L9907_regBody;

/**
 * @brief   L9907 functionality states
 */
typedef enum {
    OFF, ON, UNREACHABLE
} L9907_functionalityState;

/**
 * @brief  DIAG Register - Diagnostic register number 1: ADD = 110
 */
typedef struct {
    union {
      volatile uint16_t R;                  /* Stands for Register */
       struct {
        volatile uint16_t VSCL3:    1; /* Short Circuit low side 3 (B0) */
        volatile uint16_t VSCL2:    1; /* Short Circuit low side 2 (B1) */
        volatile uint16_t VSCL1:    1; /* Short Circuit low side 1 (B2) */
        volatile uint16_t VSCH3:    1; /* Short Circuit high side 3 (B3) */
        volatile uint16_t VSCH2:    1; /* Short Circuit high side 2 (B4) */
        volatile uint16_t VSCH1:    1; /* Short Circuit high side 1 (B5) */
        volatile uint16_t UV_LS:    1; /* Under voltage low side (B6) */
        volatile uint16_t UV_HS:    1; /* Under Voltage high side (B7) */
        volatile uint16_t VCCUV:    1; /* Vcc under voltage (B8) */
        volatile uint16_t VCCOV:    1; /* Vcc over voltage (B9) */
        volatile uint16_t VBUV:     1; /* VB under voltage (B10) */
        volatile uint16_t VBOV:     1; /* VB over-voltage (B11) */
        volatile uint16_t THSD:     1; /* Thermal shut down (B12) */
        volatile uint16_t ADD:      3; /* Address (B13-B15) */
        } B;                        /* Stands for Bit */
      } DIAG;
  } L9907_Diag;

/**
 * @brief  DIAG2 Register - Diagnostic register number 2: ADD = 111
 */
typedef struct {
    union {
      volatile uint16_t R;                  /* Stands for Register */
      struct {
        volatile uint16_t SHT1:             1; /* Info about BST_DIS pin status (B0) */
        volatile uint16_t SHT2:             1; /* Info about BST_DIS pin status (B1) */
        volatile uint16_t SHT3:             1; /* Info about BST_DIS pin status (B2) */
        volatile uint16_t GCR_STG:          1; /* Info about BST_DIS pin status (B3) */
        volatile uint16_t GCR_OL:           1; /* Info about GCR_OL pin status (B4) */
        volatile uint16_t BST_C_OV:         1; /* Info about BST_OV over-voltage level status (B5) */
        volatile uint16_t REGOFF_RB:        1; /* Info about REG_OFF procedure status (B6) */
        volatile uint16_t AND_E1_E2_RB:     1; /* Info about AND(E1,E2) status (B7) */
        volatile uint16_t EN1_RB:           1; /* Info about EN1 pin status (B8) */
        volatile uint16_t BSTDS_RB:         1; /* Info about BST_DIS pin status (B9) */
        volatile uint16_t ULB3:             1; /* Don't care (B10) */
        volatile uint16_t ULB2:             1; /* Don't care (B11) */
        volatile uint16_t ULB1:             1; /* Don't care (B12) */
        volatile uint16_t ADD:              3; /* Address (B13-B15) */
      } B;                          /* Stands for Bit */
    } DIAG2;
} L9907_Diag2;

/** 
 * @brief  L9907 Diagnostic errors structure definition 
 */
typedef struct
{
  uint32_t wL9907_ErrorsNow;
                       /*!< Bitfield with L9907 error codes that is currently
                            active. */
  uint32_t wL9907_ErrorsOccurred;
                       /*!< Bitfield with L9907 error codes that occurs and is
                            over. */
} L9907_Errors;

/**
* @}
*/

/*===========================================================================*/
/* Function declarations.                                                    */
/*===========================================================================*/
/** @defgroup L9907_class_exported_methods L9907 class exported methods
  * @{
  */

int L9907_init(void);
void L9907_stop(void);
int L9907_setEnabler(L9907_enabler);
int L9907_clearEnabler(L9907_enabler);
L9907_spiState L9907_writeRegister(L9907_regID, L9907_regBody *);
L9907_spiState L9907_readRegister(L9907_regID, L9907_regBody *);
L9907_spiState L9907_setCurrentGain(L9907_profile gain, bool ground);
L9907_spiState L9907_setDeadTime(L9907_profile deadTime);
L9907_spiState L9907_setCSA2Gain(L9907_profile csa2Gain, bool ground);
L9907_spiState L9907_setCSA1Gain(L9907_profile csa1Gain, bool ground);
L9907_spiState L9907_setLowSideTh(L9907_profile lowSideScTh);
L9907_spiState L9907_setHighSideTh(L9907_profile highSideScTh);
L9907_spiState L9907_setOverVbTh(L9907_profile batOverVoltTh);
L9907_spiState L9907_setOverVccTh(L9907_profile overVccTh);
L9907_spiState L9907_setCSA1OffsetCalibration(int Offset);
L9907_spiState L9907_setCSA2OffsetCalibration(int Offset);
L9907_functionalityState L9907_faultEffectManager(L9907_faultEffect faultEffect, L9907_functionalityState faultState);
L9907_functionalityState L9907_boostDisabler(L9907_functionalityState faultState);
L9907_functionalityState L9907_shootThroughPhase(L9907_functionalityState shootThroughState);
L9907_functionalityState L9907_testShort(L9907_functionalityState testShortState);
L9907_functionalityState L9907_testOverVcc(L9907_functionalityState overVccState);
L9907_functionalityState L9907_shortPhase(L9907_functionalityState shortPhaseState);
L9907_functionalityState L9907_boostOverVolt(L9907_functionalityState boostOverVoltState);
L9907_functionalityState L9907_regOffProcedure(L9907_functionalityState regOffState);
bool L9907_fault_detection(L9907_Diag*, L9907_Diag2*);
uint16_t L9907_CheckFaultState(void);
bool L9907_setDiagRegisters(uint8_t set);
L9907_Errors* L9907_getDiagRegisters(void);
bool L9907_CheckErrors(uint32_t*, uint32_t*);
void L9907_FaultAck(void);
void L9907_Schedule(void);
void L9907_irqfaultRecovery(void);

/**
  * @}
  */
  
/**
  * @}
  */


#endif  /* __SRxxx_L9907_H__ */
