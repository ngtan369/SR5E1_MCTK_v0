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
*   @file    STGAP4S.h
*   @version M4_VERSION
*   @brief   This file provides firmware functions that implement the features
*            of the STGAP4S component of the Stellar Motor Control.
*          
*   @details This file provides firmware functions that implement the features
*            of the STGAP4S component of the Stellar Motor Control.
*
*/

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __STGAP4_DRIVER_H__
#define __STGAP4_DRIVER_H__

#include "MC_type.h"
#include "STGAP4_configuration.h"

/*
 * @addtogroup STELLAR_STGAP4S_Library
 * @{
 */
 
 /** @addtogroup STELLAR_STGAP4S_Library
  * @{
  */

/** @defgroup STGAP4S Gate Driver
  *
  * @brief STGAP4S Gate Driver
  *
  * @{
  */

 /* Define  ------------------------------------------------------------------*/
 /* GENERAL CONSTANTS */
 #define MAX_NUMBER_DEVICES  8U
 #define DEFAULT_NUMBER_DEVICES 2U
 #define SPI_DEFAULT_VALUE (uint8_t) 0x00
 #define SPI_TIMEOUT (uint8_t) 1000U
 #define SPI_BUFFER_SIZE  2u
 #define REFERENCE_CLOCK (uint32_t) 72000

  /* STGAP4 PARAMETERS */
 #define READ_LOCAL_DESELECT_TIME 	 (uint16_t) (0U * TIMER_CLOCK_MHZ)
 #define READ_REMOTE_DESELECT_TIME 	 (uint16_t) (30U * TIMER_CLOCK_MHZ)
 #define START_CONFIG_DESELECT_TIME 	 (uint16_t) (1U * TIMER_CLOCK_MHZ)
 #define STOP_CONFIG_DESELECT_TIME 	 (uint16_t) (8U * TIMER_CLOCK_MHZ)
 #define STATUS_RESET_DESELECT_TIME 	 (uint16_t) (8U * TIMER_CLOCK_MHZ)
 #define WRITE_LOCAL_DESELECT_TIME 	 (uint16_t) (0U * TIMER_CLOCK_MHZ)
 #define WRITE_REMOTE_DESELECT_TIME       (uint16_t) (12U * TIMER_CLOCK_MHZ)
 #define DEFAULT_DESELECT_TIME		 (uint16_t) (1U * TIMER_CLOCK_MHZ)

 #define CRC_POLYNOMIAL 		        (uint16_t) 0x07

 /* STGAP4 REGISTERS */
 //STGAP4 Registers address
 #define CFG1_ADDR (uint8_t) 0x00
 #define CFG2_ADDR (uint8_t) 0x11
 #define CFG3_ADDR (uint8_t) 0x12
 #define CFG4_ADDR (uint8_t) 0x13
 #define CFG5_ADDR (uint8_t) 0x14
 #define CFG6_ADDR (uint8_t) 0x01
 #define CFG7_ADDR (uint8_t) 0x15
 #define STATUS1_ADDR (uint8_t) 0x1B
 #define STATUS2_ADDR (uint8_t) 0x1C
 #define STATUS3_ADDR (uint8_t) 0x0D
 #define STATUS4_ADDR (uint8_t) 0x0E
 #define STATUS5_ADDR (uint8_t) 0x1D
 #define TEST1_ADDR (uint8_t) 0x1F
 #define DIAG1A_ADDR (uint8_t) 0x17
 #define DIAG1B_ADDR (uint8_t) 0x18
 #define DIAG2A_ADDR (uint8_t) 0x19
 #define DIAG2B_ADDR (uint8_t) 0x1A

 //STGAP4 Registers mask
 #define CFG1_MASK (uint8_t) 0xFF
 #define CFG2_MASK (uint8_t) 0xFF
 #define CFG3_MASK (uint8_t) 0xFF
 #define CFG4_MASK (uint8_t) 0xFF
 #define CFG5_MASK (uint8_t) 0x0F
 #define CFG6_MASK (uint8_t) 0xC0
 #define CFG7_MASK (uint8_t) 0xFF
 #define STATUS1_MASK (uint8_t) 0xFF
 #define STATUS2_MASK (uint8_t) 0xDF
 #define STATUS3_MASK (uint8_t) 0xDB
 #define STATUS4_MASK (uint8_t) 0x7F
 #define STATUS5_MASK (uint8_t) 0xFF
 #define TEST1_MASK (uint8_t) 0xFF
 #define DIAG1A_MASK (uint8_t) 0xFF
 #define DIAG1B_MASK (uint8_t) 0xFF
 #define DIAG2A_MASK (uint8_t) 0xFF
 #define DIAG2B_MASK (uint8_t) 0xFF

  //STGAP4 Registers mask
 #define CFG1_LOCATION (LocationType) LOCAL
 #define CFG2_LOCATION (LocationType) REMOTE
 #define CFG3_LOCATION (LocationType) REMOTE
 #define CFG4_LOCATION (LocationType) REMOTE
 #define CFG5_LOCATION (LocationType) REMOTE
 #define CFG6_LOCATION (LocationType) LOCAL
 #define CFG7_LOCATION (LocationType) REMOTE
 #define STATUS1_LOCATION (LocationType) REMOTE
 #define STATUS2_LOCATION (LocationType) REMOTE
 #define STATUS3_LOCATION (LocationType) LOCAL
 #define STATUS4_LOCATION (LocationType) LOCAL
 #define STATUS5_LOCATION (LocationType) REMOTE
 #define TEST1_LOCATION (LocationType) REMOTE
 #define DIAG1A_LOCATION (LocationType) REMOTE
 #define DIAG1B_LOCATION (LocationType) LOCAL
 #define DIAG2A_LOCATION (LocationType) REMOTE
 #define DIAG2B_LOCATION (LocationType) LOCAL

 #define NUMBER_CFG_REGISTERS 12U
 #define NUMBER_STATUS_REGISTERS 5U
 #define NUMBER_DIAG_REGISTERS 4U
 #define NUMBER_REGISTERS 16U

 //Registetrs  ADDRESS	MASK	    LOCATION          REQUIRED VALUE      ACTUAL VALUE		   VALUE VALIDITY
 #define GAP4_CFG1 {CFG1_ADDR, CFG1_MASK, CFG1_LOCATION}//, { {SPI_DEFAULT_VALUE}, {SPI_DEFAULT_VALUE}, {NOT_VALID} } }
 #define GAP4_CFG2 {CFG2_ADDR, CFG2_MASK, CFG2_LOCATION}//, { {SPI_DEFAULT_VALUE}, {SPI_DEFAULT_VALUE}, {NOT_VALID} } }
 #define GAP4_CFG3 {CFG3_ADDR, CFG3_MASK, CFG3_LOCATION}//, { {SPI_DEFAULT_VALUE}, {SPI_DEFAULT_VALUE}, {NOT_VALID} } }
 #define GAP4_CFG4 {CFG4_ADDR, CFG4_MASK, CFG4_LOCATION}//, { {SPI_DEFAULT_VALUE}, {SPI_DEFAULT_VALUE}, {NOT_VALID} } }
 #define GAP4_CFG5 {CFG5_ADDR, CFG5_MASK, CFG5_LOCATION}//, { {SPI_DEFAULT_VALUE}, {SPI_DEFAULT_VALUE}, {NOT_VALID} } }
 #define GAP4_CFG6 {CFG6_ADDR, CFG6_MASK, CFG6_LOCATION}//, { {SPI_DEFAULT_VALUE}, {SPI_DEFAULT_VALUE}, {NOT_VALID} } }
 #define GAP4_CFG7 {CFG7_ADDR, CFG7_MASK, CFG7_LOCATION}//, { {SPI_DEFAULT_VALUE}, {SPI_DEFAULT_VALUE}, {NOT_VALID} } }
 #define STATUS1 {STATUS1_ADDR, STATUS1_MASK, STATUS1_LOCATION}//, { {SPI_DEFAULT_VALUE}, {SPI_DEFAULT_VALUE}, {NOT_VALID} } }
 #define STATUS2 {STATUS2_ADDR, STATUS2_MASK, STATUS2_LOCATION}//, { {SPI_DEFAULT_VALUE}, {SPI_DEFAULT_VALUE}, {NOT_VALID} } }
 #define STATUS3 {STATUS3_ADDR, STATUS3_MASK, STATUS3_LOCATION}//, { {SPI_DEFAULT_VALUE}, {SPI_DEFAULT_VALUE}, {NOT_VALID} } }
 #define STATUS4 {STATUS4_ADDR, STATUS4_MASK, STATUS4_LOCATION}//, { {SPI_DEFAULT_VALUE}, {SPI_DEFAULT_VALUE}, {NOT_VALID} } }
 #define STATUS5 {STATUS5_ADDR, STATUS5_MASK, STATUS5_LOCATION}//, { {SPI_DEFAULT_VALUE}, {SPI_DEFAULT_VALUE}, {NOT_VALID} } }
 #define TEST1 {TEST1_ADDR, TEST1_MASK, TEST1_LOCATION}//, { {SPI_DEFAULT_VALUE}, {SPI_DEFAULT_VALUE}, {NOT_VALID} } }
 #define DIAG1A {DIAG1A_ADDR, DIAG1A_MASK, DIAG1A_LOCATION}//, { {SPI_DEFAULT_VALUE}, {SPI_DEFAULT_VALUE}, {NOT_VALID} } }
 #define DIAG1B {DIAG1B_ADDR, DIAG1B_MASK, DIAG1B_LOCATION}//, { {SPI_DEFAULT_VALUE}, {SPI_DEFAULT_VALUE}, {NOT_VALID} } }
 #define DIAG2A {DIAG2A_ADDR, DIAG2A_MASK, DIAG2A_LOCATION}//, { {SPI_DEFAULT_VALUE}, {SPI_DEFAULT_VALUE}, {NOT_VALID} } }
 #define DIAG2B {DIAG2B_ADDR, DIAG2B_MASK, DIAG2B_LOCATION}//, { {SPI_DEFAULT_VALUE}, {SPI_DEFAULT_VALUE}, {NOT_VALID} } }

 /* Macro  ------------------------------------------------------------------*/
 #define IS_VALID_COMMAND(command) \
        ((command == STGAP4_READ) || \
         (command == STGAP4_WRITE) || \
         (command == STGAP4_STATUS_RESET) || \
         (command == STGAP4_SOFT_RESET) || \
         (command == STGAP4_START_CONFIG) ||\
         (command == STGAP4_STOP_CONFIG) ||\
 		(command == STGAP4_NOP))

 #define VALID_POINTER(pointer) (((void*)pointer == NULL)?MISSING_PARAMETER:OK)
 #define ASSERT_PARAM(par) (( par != 0U ) ? OK : MISSING_PARAMETER)
 #define CHECK_RESULT(res) {if(res!=OK) return res;}

 #ifdef USE_STM32_MICRO
 #define CHECK_ERROR(res) {if (res!=OK) Error_Handler();}
 #else
 #define CHECK_ERROR(res) {res;}
 #endif

  /* Type definition ------------------------------------------------------------------*/
 /**
 * @brief STGAP4S LocationType
 */  
 typedef enum _LocationType
  {
    LOCAL,
    REMOTE
  } LocationType;
  
 /**
 * @brief STGAP4S EnableType
 */  
 typedef enum _EnableType
 {
 	DISABLED = 0U,
 	ENABLED
 } EnableType;
 
 /**
 * @brief STGAP4S ValidityType
 */
 typedef enum _ValidityType
 {
 	INVALID,
 	VALID,
         NOT_VERIFIED
 } ValidityType;

/**
 * @brief STGAP4S StateType
 */
 typedef enum _StateType
 {
 	LOW = 0u,
 	HIGH
 } StateType;

/**
 * @brief STGAP4S _RegContentType
 */
 typedef struct _RegContentType
 {
 	uint8_t RequiredValue; /**< @brief Value of the register to be written*/
 	uint8_t ActualValue;  /**< @brief ctual value of the register */
 	ValidityType Validity; /**< @brief Validation flag according to CRC verification */
 } RegContentType;

/**
 * @brief STGAP4S _RegisterType
 */
 typedef struct _RegisterType
 {
   uint8_t Address;  /**< @brief Registers address */
   uint8_t Mask; /**< @brief Register mask */
   LocationType Location; /**< @brief Register location, local or remote */
 } RegisterType;
 
/**
 * @brief STGAP4S Device types
 */
 typedef struct _DeviceType
 {
 	RegContentType Cfg1; /**< @brief Cfg1 register */
 	RegContentType Cfg2; /**< @brief Cfg2 register */
 	RegContentType Cfg3; /**< @brief Cfg3 register */
 	RegContentType Cfg4; /**< @brief Cfg4 register */
 	RegContentType Cfg5; /**< @brief Cfg5 register */
 	RegContentType Cfg6; /**< @brief Cfg6 register */
 	RegContentType Cfg7; /**< @brief Cfg7 register */
 	RegContentType Diag1a; /**< @brief Diag1a register */
 	RegContentType Diag1b; /**< @brief Diag1b register */
 	RegContentType Diag2a; /**< @brief Diag2a register */
 	RegContentType Diag2b; /**< @brief Diag2b register */
 	RegContentType Test1; /**< @brief Test1 register */
 	RegContentType Status1; /**< @brief Status1 register */
 	RegContentType Status2; /**< @brief Status2 register */
 	RegContentType Status3; /**< @brief Status3 register */
 	RegContentType Status4; /**< @brief Status4 register */
 	RegContentType Status5; /**< @brief Status5 register */
 } DeviceType;

/**
 * @brief STGAP4S CommandType
 */
 typedef enum _CommandType
 {
 	STGAP4_READ =  0xA0,
 	STGAP4_WRITE = 0x80,
 	STGAP4_STATUS_RESET = 0xD0,
 	STGAP4_SOFT_RESET = 0xEA,
 	STGAP4_START_CONFIG = 0x2A,
    STGAP4_STOP_CONFIG = 0x3A,
    STGAP4_NOP = 0x00
 } CommandType;
 
 /**
 * @brief STGAP4S ResultType
 */
 typedef enum ResultType
 {
 	OK,
 	SPI_ERROR,
 	GPIO_ERROR,
 	DATA_ERROR,
 	CS_ERROR,
 	MISSING_PARAMETER,
 	INCOHERENT_REGISTER_CFG,
 	INVALID_REGISTER_CFG,
 	INVALID_REGISTER_STATUS,
 	STATUS_ERROR,
 	OPERATION_ERROR

 } ResultType;



/**
 * @brief   STGAP4S Driver configuration structure.
 */
typedef struct {
  union
  {
    struct
    {
      /**
         * @brief   STATUS_1 Register
         */
      union {
        vuint8_t R;          /* Stands for Register */
        struct {
          vuint8_t STATUS1_BUSY: 1; /**< @brief STATUS1_BUSY */
          vuint8_t TSD:          1; /**< @brief TSD */          
          vuint8_t UVLOL:        1; /**< @brief UVLOL */          
          vuint8_t UVLOH:        1; /**< @brief UVLOH */          
          vuint8_t SENSE_OC:     1; /**< @brief SENSE_OC */          
          vuint8_t DESAT_OC:     1; /**< @brief DESAT_OC */          
          vuint8_t OVLOL:        1; /**< @brief OVLOL */          
          vuint8_t OVLOH:        1; /**< @brief OVLOH */          
        } B;                  /* Stands for Bit */
      } STATUS_1;
      /**
         * @brief   STATUS_2 Register
         */
      union {
        vuint8_t R;          /* Stands for Register */
        struct {
          vuint8_t STATUS2_BUSY:   1; /**< @brief STATUS1_BUSY */
          vuint8_t COMERRR:        1; /**< @brief COMERRR */          
          vuint8_t REGERRR:        1; /**< @brief REGERRR */          
          vuint8_t PROGHV:         1; /**< @brief PROGHV */          
          vuint8_t TWN:            1; /**< @brief TWN */          
          vuint8_t UNUSED:         1; /**< @brief UNUSED */          
          vuint8_t HVReset:        1; /**< @brief OVLOL */          
          vuint8_t SAFESTATE:      1; /**< @brief OVLOH */          
        } B;                  /* Stands for Bit */
      } STATUS_2;
      /**
         * @brief   STATUS_3 Register
         */
      union {
        vuint8_t R;          /* Stands for Register */
        struct {
          vuint8_t FLYBACK_FLT:   1; /**< @brief FLYBACK_FLT */
          vuint8_t PROGLV:        1; /**< @brief PROGLV */          
          vuint8_t UNUSED:        1; /**< @brief UNUSED */          
          vuint8_t SPI_ERR:       1; /**< @brief SPI_ERR */          
          vuint8_t DT_ERR:        1; /**< @brief DT_ERR */          
          vuint8_t UNUSED_1:      1; /**< @brief UNUSED_1 */          
          vuint8_t UVLO_VCC:      1; /**< @brief UVLO_VCC */          
          vuint8_t UVLO_3V3IN:    1; /**< @brief UVLO_3V3IN */          
        } B;                  /* Stands for Bit */
      } STATUS_3;
      /**
         * @brief   STATUS_4 Register
         */
      union {
        vuint8_t R;          /* Stands for Register */
        struct {
          vuint8_t SPI_HVbusy:    1; /**< @brief SPI_HVbusy */
          vuint8_t COMERRL:       1; /**< @brief COMERRL */          
          vuint8_t REGERRL:       1; /**< @brief REGERRL */          
          vuint8_t LVReset:       1; /**< @brief LVReset */          
          vuint8_t GATE_LEVEL_LV: 1; /**< @brief GATE_LEVEL_LV */          
          vuint8_t ASC_LEVEL_LV:  1; /**< @brief ASC_LEVEL_LV */          
          vuint8_t PWM_LV:        1; /**< @brief PWM_LV */          
          vuint8_t UNUSED:        1; /**< @brief UNUSED */          
        } B;                  /* Stands for Bit */
      } STATUS_4;
      /**
         * @brief   STATUS_5 Register
         */
      vuint8_t STATUS_5;	 /**< @brief ADC */ 
	  
    } REGS;
    vuint8_t StatRegs[NUMBER_STATUS_REGISTERS];
  };
} STGAP4S_statusReg_t;

/* Includes ------------------------------------------------------------------*/


 /* Prototype declaration -----------------------------------------------------*/
 void STGAP4_init(void);
 void STGAP_mainLoop(void);

ResultType STGAP4_LoadValue(DeviceType *Devices);
ResultType STGAP4_Command(CommandType Command);
ResultType STGAP4_Read(RegisterType **Registers,  uint8_t RegNumber, DeviceType *Devices);
ResultType STGAP4_Write(RegisterType **Registers, DeviceType *Devices, uint8_t RegNumber);
ResultType STGAP4_ConfigurationFlow(void);
ResultType STGAP4_ChkCfgValue(DeviceType *Devices);
ResultType STGAP4_ChkStatusValue(DeviceType *Devices);


ResultType STGAP4_TxRx(uint8_t *TxByte, uint8_t *RxByte, uint8_t BufferSize);
ResultType STGAP4_Tx(uint8_t *TxByte, uint8_t BufferSize);
ResultType STGAP4_ChipSelect(StateType State);
ResultType STGAP4_Shutdown(StateType State);
void STGAP4_Wait(uint32_t Count);
void STGAP4_TimerStart(void);
void STGAP4_TimerStop(void);

void STGAP4S_Schedule(void);
uint16_t STGAP4S_CheckFaultState(void);
STGAP4S_statusReg_t* STGAP4S_GetStatusRegisters(uint16_t device);
bool STGAP4S_SetDiagRegisters(uint8_t set);
void STGAP4S_irqfaultRecovery(void);
uint16_t STGAP4S_FaultAck(void);
float STGAP4_GetADCVoltFromValue(DeviceType dev, uint8_t value);
float STGAP4_GetADCVoltFromStatusReg(DeviceType dev);

/* Exported variables -----------------------------------------------------*/
extern RegisterType Cfg1;
extern RegisterType Cfg2;
extern RegisterType Cfg3;
extern RegisterType Cfg4;
extern RegisterType Cfg5;
extern RegisterType Cfg6;
extern RegisterType Cfg7;
extern RegisterType Test1;
extern RegisterType Diag1a;
extern RegisterType Diag1b;
extern RegisterType Diag2a;
extern RegisterType Diag2b;
extern RegisterType Status1;
extern RegisterType Status2;
extern RegisterType Status3;
extern RegisterType Status4;
extern RegisterType Status5;

extern RegisterType *CfgRegisters[NUMBER_CFG_REGISTERS];
extern RegisterType *StatusRegisters[NUMBER_STATUS_REGISTERS];

extern DeviceType Devices[NUMBER_DEVICES];




#ifdef __cplusplus
}
#endif

#endif /*__STGAP4_DRIVER_H__*/

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/



