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
*	@file	 STGAP4S.c
*	@version M4_VERSION
*	@brief	 This file provides firmware functions that implement the features
*			 of the STGAP4S component of the Stellar Motor Control.
*		   
*	@details This file provides firmware functions that implement the features
*			 of the STGAP4S component of the Stellar Motor Control.
 */
/*
 * @addtogroup STELLAR_STGAP4S_Library
 * @{
 */

/* Includes ------------------------------------------------------------------*/
#include <stdio.h>
#include <string.h>
#include "clock.h"
#include "gpio.h"
#include "spi.h"

#include "STGAP4S.h"
#include <test_env.h>
#include "r_divider_bus_voltage_sensor.h"

/* Private define ------------------------------------------------------------*/
#define BUFFER_SIZE 2u
#define CRC_INITIAL_VALUE 0xFF
#define CRC_POLYMOMIAL 0x07
#define STGAP4_DEFAULT_BYTE 0x00
#define STGAP4_NOTVALIDATED_BYTE 0xFE
#define BYTE_LENGTH 8U
#define STATUS_OK 0x00
#define WAIT_TIMEBASE (uint16_t) (4U*SYSTEM_CLOCK/REFERENCE_CLOCK)
#define MAX_RETRY 5u

#ifndef NUMBER_DEVICES
#define NUMBER_DEVICES DEFAULT_NUMBER_DEVICES
#elif (NUMBER_DEVICES > MAX_NUMBER_DEVICES)
#error "Error: number of declared devices is over the limit"
#endif


/* Private macros  -----------------------------------------------------------*/
#define CRC_CHECK(data, crc_rec)({uint8_t crc_calc; crc_calc=(uint8_t)(STGAP4_CRC(data, CRC_INITIAL_VALUE) ^ 0xFF); \
				 (crc_calc == (uint8_t) crc_rec) ? (ValidityType) VALID : (ValidityType) INVALID ;})
#define EXTRACT_iTH_BIT(val, i) ((val >> i) & 0x01)
#define CRC_FIRST_BIT(crc, data, index) (EXTRACT_iTH_BIT(crc,7) ^ EXTRACT_iTH_BIT(data, index))
#define DESELECTION_TIME(location, local_time, remote_time) ((location == REMOTE)?remote_time:local_time)
#define CHECK_FLOW(fun) ({result=fun;if(result != OK) goto EXIT;})

/* Function prototypes -----------------------------------------------------------*/
static uint8_t STGAP4_CRC(uint8_t Byte, uint8_t InitValue);
static void STGAP4_SelectDeviceRegister(RegisterType **Registers, uint8_t NumberRegisters, DeviceType *Devices, RegContentType *PRegContent[NUMBER_REGISTERS][NUMBER_DEVICES]);

/* Variables -----------------------------------------------------*/
RegisterType Cfg1 = GAP4_CFG1;
RegisterType Cfg2 = GAP4_CFG2;
RegisterType Cfg3 = GAP4_CFG3;
RegisterType Cfg4 = GAP4_CFG4;
RegisterType Cfg5 = GAP4_CFG5;
RegisterType Cfg6 = GAP4_CFG6;
RegisterType Cfg7 = GAP4_CFG7;
RegisterType Test1 = TEST1;
RegisterType Diag1a = DIAG1A;
RegisterType Diag2a = DIAG2A;
RegisterType Diag1b = DIAG1B;
RegisterType Diag2b = DIAG2B;
RegisterType Status1 = STATUS1;
RegisterType Status2 = STATUS2;
RegisterType Status3 = STATUS3;
RegisterType Status4 = STATUS4;
RegisterType Status5 = STATUS5;

RegisterType *CfgRegisters[NUMBER_CFG_REGISTERS] = {&Cfg1, &Cfg2, &Cfg3, &Cfg4, &Cfg5, &Cfg6, &Cfg7, &Test1, &Diag1a, &Diag1b, &Diag2a, &Diag2b};
RegisterType *StatusRegisters[NUMBER_STATUS_REGISTERS] = {&Status1, &Status2, &Status3, &Status4, &Status5};

DeviceType Devices[NUMBER_DEVICES];

static void STGAP4_SPI_Init(void);
static void STGAP4_TIM_Init(void);

uint32_t notBlockingFaultCount = 0;
volatile uint8_t sch_checkerror = 0;
uint32_t STGAP4S_wErrorsNow[NUMBER_DEVICES];
uint32_t STGAP4S_wErrorsOccurred[NUMBER_DEVICES];


/* Analog measurement function */
#define ADC_HIGHth_mask 0b11100000
#define ADC_LOWth_mask	0b00011100

#define ADC_HIGHth_bit 5
#define ADC_LOWth_bit  2

#define LOW_TH(x)  ((x & ADC_LOWth_mask)  >> ADC_LOWth_bit )
#define HIGH_TH(x) ((x & ADC_HIGHth_mask) >> ADC_HIGHth_bit)

const float V_lowth_Values[] = {0.0f, 0.215f, 0.430f, 0.645f, 0.855f, 1.070f, 1.285f, 1.5f};
const float V_highth_Values[] = {1.50f , 1.715f, 1.930f, 2.145f, 2.360f, 2.570f, 2.785f, 3.00f};


uint32_t STGAP4S_wErrorsNow[NUMBER_DEVICES];
uint32_t STGAP4S_wErrorsOccurred[NUMBER_DEVICES];
typedef enum {SCH_TEST_IDLE,SCH_TEST_FAULT,SCH_TEST_RUN} state_t;
volatile state_t sch_state = SCH_TEST_IDLE;

/**
 * @brief	STGAP4S status registers instance
 */
static STGAP4S_statusReg_t	  g_statusReg[NUMBER_DEVICES];

/* STGAP4S Initializaiont ok/SPI_ERROR */ 
uint8_t STGAP4S_init = OK;

 /**
  * @brief	This function initialiaze STGAP4S driver\n
  *			Provide also initialization of SPI\n
  * @param	void
  * @return void
  */
void STGAP4_init(void) {
	
  ResultType initResult = OK;

  STGAP4_SPI_Init();
  STGAP4_TIM_Init();

  initResult = STGAP4_ConfigurationFlow();
  if (initResult != OK) {
    //while(1); // Trap
    STGAP4S_init = SPI_ERROR;
  }
}

#if 0
void STGAP_mainLoop(void) {
  static uint32_t lastTick = 0;
  static uint32_t delayTk = (2 * 10); // 10ms
  //uint32_t currentTick = getTick();
  uint32_t currentTick = osal_get_tick();
  if (currentTick > lastTick + delayTk) {
	STGAP4_Read(StatusRegisters,
		sizeof(StatusRegisters) / sizeof(*StatusRegisters), Devices);
	if (STGAP4_ChkStatusValue(Devices) != OK) {
	  //STGAP4_Shutdown(LOW);
	  //led1_on();
	  notBlockingFaultCount++;
	}
	lastTick = currentTick;
  }
}
#endif

/* SPI Functions ---------------------------------------------------------*/

 /**
  * @brief	This function initialiaze SPI driver to communicato with STGAP4S device\n
  *	  
  * @param	void
  * @return void
  */
static void STGAP4_SPI_Init(void)
{

  (void)spi_init(&DRV_SPI1);

  (void)spi_set_drv_mode(&DRV_SPI1, SPI_DRV_MODE_INT_SYNC);	   /**< Synchronous interrupt mode	*/
  (void)spi_set_mode(&DRV_SPI1, SPI_MODE_MASTER);			   /**< Master mode	 */
  (void)spi_set_cmode(&DRV_SPI1, SPI_CMODE_FULL_DUPLEX);	   /**< Full duplex communication */
  (void)spi_set_baud(&DRV_SPI1, SPI_BAUDRATE_DIV32);		   /**< 40MHz/32 = 1.25MHz */
  (void)spi_set_cpha(&DRV_SPI1, SPI_CPHA_1);				   /**< The second clock transition is the first data capture edge */
  (void)spi_set_cpol(&DRV_SPI1, SPI_CPOL_0);				   /**< Clock to 0 when idle */
  (void)spi_set_fformat(&DRV_SPI1, SPI_FRAMEFORMAT_MSB);	   /**< Data is transmitted/received with the MSB first	 */
  (void)spi_set_fsize(&DRV_SPI1, SPI_FRAMESIZE_8);			   /**< Frame size = 8bit  */
  (void)spi_set_nssp_mode(&DRV_SPI1, SPI_NSS_PULSE_MODE_OFF);  /**< No NSS pulse		*/
  (void)spi_set_nss_mode(&DRV_SPI1, SPI_NSS_MODE_HARDWARE);	   /**< Slave select mode = software */

  (void)spi_set_prio(&DRV_SPI1, 3); /** < It must be highr that the Systick priority */

  /* Apply SPI Master driver instance configuration.*/
  spi_start(&DRV_SPI1);
}

/* End SPI Functions ---------------------------------------------------------*/


/* Functions ---------------------------------------------------------*/
/**
  * @brief	STGAP4_Read
  *			Read a list of STGAP4 registers
  *			through the SPI interface. Registers reading
  *			is performed for every device in SPI chain.
  *
  * @param	in Registers: List of registers to be read.
  * @param	in RegNumber: Number of registers listed in registers list.
  * @param	out Devices: List of devices values to be updated.
  * @retval Result: OK or error.
 */
ResultType STGAP4_Read(RegisterType **Registers, uint8_t RegNumber, DeviceType *Devices)
{
  uint8_t reg, dev = 0;
  uint8_t TxByte[NUMBER_REGISTERS][NUMBER_DEVICES*BUFFER_SIZE], RxByte[NUMBER_REGISTERS][NUMBER_DEVICES*BUFFER_SIZE];
  uint16_t WaitTime[NUMBER_REGISTERS];
  RegContentType *preg_content[NUMBER_REGISTERS][NUMBER_DEVICES];
  ResultType result = OK;

  /********* CHECK THE INPUT PARAMETER **********/
  CHECK_RESULT(VALID_POINTER(Registers));
  CHECK_RESULT(ASSERT_PARAM(RegNumber));
  CHECK_RESULT(VALID_POINTER(Devices));
  
  /********* PREPARE BYTE TO BE TRANSMITTED **********/
  //for every register to be read
  for (reg = 0; reg <= RegNumber; reg++)
  {
	//For every device in SPI chain
	for (dev=0; dev < NUMBER_DEVICES*BUFFER_SIZE; dev+=BUFFER_SIZE)
	{

	  if (reg == RegNumber)
	  {
		//if last register to be read, byte to be transmitted: NOP
		TxByte[reg][dev+0] = STGAP4_NOP;

		WaitTime[reg] = 0;
	  }
	  else
	  {
		  //Otherwise, byte to be transmitted:	STGAP4 Read Command + Address
		  TxByte[reg][dev+0] = STGAP4_READ + Registers[reg]->Address;

		  //Set wait time according to register location
		  WaitTime[reg] = DESELECTION_TIME(Registers[reg]->Location,
														   READ_LOCAL_DESELECT_TIME,
														   READ_REMOTE_DESELECT_TIME);
	  }

	//Calculate CRC to be transmitted if required
	TxByte[reg][dev+1]	= ( CRC_FLAG ? STGAP4_CRC(TxByte[reg][dev+0], CRC_INITIAL_VALUE) : STGAP4_DEFAULT_BYTE);
	
	}//end for every device in SPI chain
	
  }//end for every register to be read
  /***************************************************/

  //Start the timer for waiting time
  STGAP4_TimerStart();

  /**** SEND AND RECEIVE BYTE OVER SPI INTERFACE ****/
  //for every register to be read
  for (reg = 0; reg <= RegNumber; reg++)
  {
	//Chip select signal low
	STGAP4_ChipSelect(LOW);

	//Recall SPI driver function to transmit and receive data over SPI interface
	result = STGAP4_TxRx(TxByte[reg], RxByte[reg], NUMBER_DEVICES*BUFFER_SIZE);

	//Chip select signal high
	STGAP4_ChipSelect(HIGH);

	//Wait for deselection time
	STGAP4_Wait(WaitTime[reg]);

  }//end for every register to be read
  /*************************************************/

  //Stop the timer for waiting time
  STGAP4_TimerStop();

  /************** STORE RECEIVED DATA **************/
  if  (result == OK)
  {
	//Select the register pointer
	STGAP4_SelectDeviceRegister(Registers, RegNumber, Devices, preg_content);

	//for every register to be read
	for (reg = 0; reg < RegNumber; reg++)
	{
	  //For every device in SPI chain
	  for (dev=0; dev < NUMBER_DEVICES; dev++)
	  {
		//Store the register value
		preg_content[reg][dev]->ActualValue = RxByte[reg+1][2*dev];

		//Perform data validation or not according to CRC enabled flag
		if(CRC_FLAG)
		   preg_content[reg][dev]->Validity = CRC_CHECK(RxByte[reg+1][2*dev], RxByte[reg+1][2*dev+1]);
		else
		   preg_content[reg][dev]->Validity = NOT_VERIFIED;

	  }//end for every device in SPI chain

	}//end	for every register to be read
  }
  /*************************************************/

  return result;

}

/**
  * @brief	STGAP4_Write
  *			Write the required content to the listed
  *			STGAP4 registers through the SPI interface.
  *			Registers writing is performed for every
  *			device in SPI chain.
  *
  * @param	in Registers: List of registers to be written.
  * @param	in Devices: List of values to be written for every device.
  * @param	in RegNumber: Number of registers listed in registers list.
  * @retval Result: OK or error.
 */
ResultType STGAP4_Write(RegisterType **Registers, DeviceType *Devices, uint8_t RegNumber)
{
   uint8_t reg, dev = 0;
   uint8_t TxCommand[NUMBER_REGISTERS][NUMBER_DEVICES*BUFFER_SIZE];
   uint8_t TxData[NUMBER_REGISTERS][NUMBER_DEVICES*BUFFER_SIZE];
   uint16_t WaitTime[NUMBER_REGISTERS];
   RegContentType *preg_content[NUMBER_REGISTERS][NUMBER_DEVICES];
   ResultType result = OK;
   
   /********* CHECK THE INPUT PARAMETER **********/
   CHECK_RESULT(VALID_POINTER(Registers));
   CHECK_RESULT(VALID_POINTER(Devices));
   CHECK_RESULT(ASSERT_PARAM(RegNumber));

   /********** PREPARE BYTE TO BE TRANSMITTED *********/
   //Get the register content pointer
   STGAP4_SelectDeviceRegister(Registers, RegNumber, Devices, preg_content);

   //for every register to be read
   for (reg = 0; reg < RegNumber; reg++)
   {
	 //For every device in SPI chain
	 for (dev=0; dev < NUMBER_DEVICES; dev++)
	 {
		//Command + Address
		TxCommand[reg][BUFFER_SIZE*dev+0] = STGAP4_WRITE + Registers[reg]->Address;

		//Calculate CRC to be transmitted if required
		TxCommand[reg][BUFFER_SIZE*dev+1] = (CRC_FLAG ? STGAP4_CRC(TxCommand[reg][BUFFER_SIZE*dev+0], CRC_INITIAL_VALUE) : STGAP4_DEFAULT_BYTE);

		//Data to be transmitted:  Data	 +	 CRC or 0x00
		TxData[reg][BUFFER_SIZE*dev+0]=preg_content[reg][dev]->RequiredValue;

		//Inverted CRC calculated on the write command will be used to initiate CRC of the data
		TxData[reg][BUFFER_SIZE*dev+1]=(CRC_FLAG ? STGAP4_CRC(TxData[reg][BUFFER_SIZE*dev+0], TxCommand[reg][BUFFER_SIZE*dev+1] ^ 0xFF) : STGAP4_DEFAULT_BYTE);
	 }

	 WaitTime[reg] = DESELECTION_TIME(Registers[reg]->Location,
													 WRITE_LOCAL_DESELECT_TIME,
													 WRITE_REMOTE_DESELECT_TIME);
	}
	/**************************************************/

	/**************** START CONFIG ********************/
	//Send Start Config command.
	STGAP4_Command(STGAP4_START_CONFIG);
	/**************************************************/

	//Start the timer for waiting time
	STGAP4_TimerStart();

	/**** SEND COMMAND AND DATA OVER SPI INTERFACE ****/
	//for every register to be written
	for (reg=0; reg < RegNumber; reg++)
	{
	  //Chip select low
	  STGAP4_ChipSelect(LOW);

	  //Send write command
	  //For every device in SPI chain
	  result = STGAP4_Tx(TxCommand[reg],  NUMBER_DEVICES*BUFFER_SIZE);

	  //Chip select high
	  STGAP4_ChipSelect(HIGH);

	  //Deselect time
	  STGAP4_Wait(WaitTime[reg]);

	  if (result == OK)
	  {
		//Chip select low
		STGAP4_ChipSelect(LOW);

		//Transmit data through SPI
		result = STGAP4_Tx(TxData[reg], NUMBER_DEVICES*BUFFER_SIZE);

		//CS high
		STGAP4_ChipSelect(HIGH);
	  }

	} // end for every register
	/***************************************************/

	//Stop the timer for waiting time
	STGAP4_TimerStop();

	/***************** STOP CONFIG *********************/
	//Send Stop Config command.
	STGAP4_Command(STGAP4_STOP_CONFIG);
	/***************************************************/

  return result;

}

/**
  * @brief	STGAP4_Command
  *			Send a command (Start or Stop Config, Status Reset, Soft reset etc.)
  *			to all the STGAP4 devices in SPI chain.

  * @param in Command: Command to be sent.
  * @retval Result: OK or error.
 */
ResultType STGAP4_Command(CommandType Command)
{

  uint8_t TxByte[BUFFER_SIZE], dev;
  uint16_t DS_time = 0;
  ResultType result =OK;

  /********* CHECK THE INPUT PARAMETER **********/
  CHECK_RESULT(ASSERT_PARAM(IS_VALID_COMMAND(Command)));

  /********** PREPARE BYTE TO BE TRANSMITTED AND SET DESELECTION TIME **********/
  //Store command byte
  TxByte[0]=Command;
  //Calculate and store CRC
  TxByte[1]=(CRC_FLAG ? STGAP4_CRC(TxByte[0], CRC_INITIAL_VALUE) : STGAP4_DEFAULT_BYTE);

  //Select deselect time according to command
  switch (Command)
  {
	case (STGAP4_STATUS_RESET):
	{
	  DS_time = STATUS_RESET_DESELECT_TIME;
	  break;
	}
	case (STGAP4_SOFT_RESET):
	{
	  DS_time = DEFAULT_DESELECT_TIME;
	  break;
	}
	case (STGAP4_START_CONFIG):
	{
	  DS_time = START_CONFIG_DESELECT_TIME;
	  break;
	}
	case (STGAP4_STOP_CONFIG):
	{
	  DS_time = STOP_CONFIG_DESELECT_TIME;
	  break;
	}
	default:
	{
	  result = DATA_ERROR;
	  break;
	}
  }//end switch
 /***************************************************/

  //Start the timer for waiting time
  STGAP4_TimerStart();

  /********** SEND THE BYTE OVER SPI INTERFACE **********/
  if (result == OK)
  {
	//Chip select low
	STGAP4_ChipSelect(LOW);

	for (dev=0; dev<NUMBER_DEVICES; dev++)
	{
	//Transmit data through SPI
	result = STGAP4_Tx(TxByte, SPI_BUFFER_SIZE);

	if (result != OK)
	  break;

	}

	//Chip select signal high
	STGAP4_ChipSelect(HIGH);

	//Wait for deselect time
	STGAP4_Wait(DS_time);

  }//End if
  /*************************************/

  //Stop the timer for waiting time
  STGAP4_TimerStop();

  return result;
}



/**
  * @brief	STGAP4_LoadValue
  *			Load the registers values to be written into
  *			the dedicated structure list.
  *
  * @param	in Devices: Devices values list.
  * @retval Result: OK or error.
 */
ResultType STGAP4_LoadValue(DeviceType *Devices)
{
  uint8_t reg, dev = 0;
  ResultType result = OK;

  uint8_t DevicesValue[MAX_NUMBER_DEVICES][NUMBER_CFG_REGISTERS + NUMBER_DIAG_REGISTERS] =
														{DEVICE1_REGISTERS_VALUES,
														 DEVICE2_REGISTERS_VALUES,
														 DEVICE3_REGISTERS_VALUES,
														 DEVICE4_REGISTERS_VALUES,
														 DEVICE5_REGISTERS_VALUES,
														 DEVICE6_REGISTERS_VALUES,
														 DEVICE7_REGISTERS_VALUES,
														 DEVICE8_REGISTERS_VALUES} ;

  /********* CHECK THE INPUT PARAMETER **********/
  CHECK_RESULT(VALID_POINTER(Devices));

  /********* LOAD THE VALUES **********/
  //for every devices in SPI chain
  for (dev = 0; dev < NUMBER_DEVICES; dev++)
  {
	//for every registers
	for (reg = 0; reg < (NUMBER_CFG_REGISTERS + NUMBER_DIAG_REGISTERS); reg++)
	{
	  switch (reg)
	  {
		//CFG1
		case 0:
		{
		  Devices[dev].Cfg1.RequiredValue = DevicesValue[dev][reg];
		  break;
		}
		//CFG2
		case 1:
		{
		  Devices[dev].Cfg2.RequiredValue = DevicesValue[dev][reg];
		  break;
		}
		//CFG3
		case 2:
		{
		  Devices[dev].Cfg3.RequiredValue = DevicesValue[dev][reg];
		  break;
		}
		//CFG4
		case 3:
		{
		  Devices[dev].Cfg4.RequiredValue = DevicesValue[dev][reg];
		  break;
		}
		//CFG5
		case 4:
		{
		  Devices[dev].Cfg5.RequiredValue = DevicesValue[dev][reg];
		  break;
		}
		//CFG6
		case 5:
		{
		  Devices[dev].Cfg6.RequiredValue = DevicesValue[dev][reg];
		  break;
		}
		//CFG7
		case 6:
		{
		  Devices[dev].Cfg7.RequiredValue = DevicesValue[dev][reg];
		  break;
		}
		//TEST1
		case 7:
		{
		  Devices[dev].Test1.RequiredValue = DevicesValue[dev][reg];
		  break;
		}
		//DIAG1A
		case 8:
		{
		  Devices[dev].Diag1a.RequiredValue = DevicesValue[dev][reg];
		  break;
		}
		//DIAG1B
		case 9:
		{
		  Devices[dev].Diag1b.RequiredValue = DevicesValue[dev][reg];
		  break;
		}
		//DIAG2A
		case 10:
		{
		  Devices[dev].Diag2a.RequiredValue = DevicesValue[dev][reg];
		  break;
		}
		//DIAG2B
		case 11:
		{
		  Devices[dev].Diag2b.RequiredValue = DevicesValue[dev][reg];
		  break;
		}
		default:
		{
		  break;
		}
		
	  } // end switch
	  
	} // end for reg
	
  } // end for dev

  return result;

}


/**
  * @brief	STGAP4_ChkCfgValue
  *			Check the configuration values of STGAP4 device.
  *			Check the content validity according to CRC
  *			and the data coherency between expected and actual value.
  *
  * @param in Devices: Devices values list.
  * @retval Result: OK or error.
 */
ResultType STGAP4_ChkCfgValue(DeviceType *Devices)
{
  uint8_t dev = 0;
  ResultType result = OK;
  
  /********* CHECK THE INPUT PARAMETER **********/
  CHECK_RESULT(VALID_POINTER(Devices));
  
  /****** CHECK THE CONFIGURATION VALUES *******/
  //for every device in SPI chain
  for (dev = 0; dev < NUMBER_DEVICES; dev++)
  {
	if (CRC_FLAG)
	{
	  //Check content validity according to CRC
	  if (
		  (Devices[dev].Cfg1.Validity != VALID) ||
		  (Devices[dev].Cfg2.Validity != VALID) ||
		  (Devices[dev].Cfg3.Validity != VALID) ||
		  (Devices[dev].Cfg4.Validity != VALID) ||
		  (Devices[dev].Cfg5.Validity != VALID) ||
		  (Devices[dev].Cfg6.Validity != VALID) ||
		  (Devices[dev].Cfg7.Validity != VALID) ||
		  (Devices[dev].Test1.Validity != VALID) ||
		  (Devices[dev].Diag1a.Validity != VALID) ||
		  (Devices[dev].Diag1b.Validity != VALID) ||
		  (Devices[dev].Diag2a.Validity != VALID) ||
		  (Devices[dev].Diag2b.Validity != VALID) )
	  {
		result = INVALID_REGISTER_CFG;
		break;
	  } //end if validity 
	  
	}//end if (CRC_FLAG)
	
	//If data is valid, check content coherency 
	else if	 (
		(Devices[dev].Cfg1.RequiredValue != Devices[dev].Cfg1.ActualValue) ||
		(Devices[dev].Cfg2.RequiredValue != Devices[dev].Cfg2.ActualValue) ||
		(Devices[dev].Cfg3.RequiredValue != Devices[dev].Cfg3.ActualValue) ||
		(Devices[dev].Cfg4.RequiredValue != Devices[dev].Cfg4.ActualValue) ||
		(Devices[dev].Cfg5.RequiredValue != Devices[dev].Cfg5.ActualValue) ||
		(Devices[dev].Cfg6.RequiredValue != Devices[dev].Cfg6.ActualValue) ||
		(Devices[dev].Cfg7.RequiredValue != Devices[dev].Cfg7.ActualValue) ||
		(Devices[dev].Test1.RequiredValue != Devices[dev].Test1.ActualValue) ||
		(Devices[dev].Diag1a.RequiredValue != Devices[dev].Diag1a.ActualValue) ||
		(Devices[dev].Diag1b.RequiredValue != Devices[dev].Diag1b.ActualValue) ||
		(Devices[dev].Diag2a.RequiredValue != Devices[dev].Diag2a.ActualValue) ||
		(Devices[dev].Diag2b.RequiredValue != Devices[dev].Diag2b.ActualValue) )
	{
	  result = INCOHERENT_REGISTER_CFG;
	  break;
	} //end if required value != actual value

  }//end for every devices in SPI chain
  
  return result;
}

/**
  * @brief	STGAP4_ChkStatusValue
  *			Check the status values of STGAP4 devices.
  *			Check the content validity according to CRC
  *			and if no error flag are present.
  *
  * @param	in Devices: Devices values list
  * @retval Result: OK or error
 */
ResultType STGAP4_ChkStatusValue(DeviceType *Devices)
{
  uint8_t dev = 0;
  ResultType result = OK;

  /********* CHECK THE INPUT PARAMETER **********/
  CHECK_RESULT(VALID_POINTER(Devices));
	
  //for every devices
  for (dev = 0; dev < NUMBER_DEVICES; dev++)
  {
	//Check content validity according to CRC
	if ((Devices[dev].Status1.Validity != VALID) ||
		(Devices[dev].Status2.Validity != VALID) ||
		(Devices[dev].Status3.Validity != VALID) ||
		(Devices[dev].Status4.Validity != VALID))
	{
	  result = INVALID_REGISTER_STATUS;
	}
	//if content is valid, check if no error are present
	else if	 (
			(Devices[dev].Status1.ActualValue != STATUS_OK) ||
			(Devices[dev].Status2.ActualValue != STATUS_OK) ||
			(Devices[dev].Status3.ActualValue != STATUS_OK) ||
			(Devices[dev].Status4.ActualValue != STATUS_OK))
	{
	  result = STATUS_ERROR;
	}

  }//end for every devices

  return result;
}


/**
  * @brief	STGAP4_ConfigurationFlow
  *			Perform the standard configuration flow
  *			on STGAP4 devices connected in SPI chain:
  *			1. Devices status reset
  *			2. Write configuration values
  *			3. Read and verify configuration values
  *			4. Read and verify status values
  *
  * @param	none
  * @retval Result: OK or error
 */
ResultType STGAP4_ConfigurationFlow(void)
{
  volatile uint8_t retry = 0;
  ResultType result;

  //Load the required configuration values
  STGAP4_LoadValue(Devices);

  //Set shutdown low
  CHECK_FLOW(STGAP4_Shutdown(LOW));

  //Send status reset command
  CHECK_FLOW(STGAP4_Command(STGAP4_STATUS_RESET));

  do
  {
	//Registers configuration
	CHECK_FLOW(STGAP4_Write(CfgRegisters, Devices, sizeof(CfgRegisters)/sizeof(*CfgRegisters)));

	//Read register configuration
	CHECK_FLOW(STGAP4_Read(CfgRegisters, sizeof(CfgRegisters)/sizeof(*CfgRegisters), Devices));

	retry ++;

  }
  //Check register configuration content
  while ((STGAP4_ChkCfgValue(Devices) != OK) && (retry < MAX_RETRY));

  //Read status registers
  CHECK_FLOW(STGAP4_Read(StatusRegisters, sizeof(StatusRegisters)/sizeof(*StatusRegisters), Devices));

  //Check no error status
  CHECK_FLOW(STGAP4_ChkStatusValue(Devices));

 EXIT:
  //Set shutdown high
  STGAP4_Shutdown(HIGH);

  return result;

}

/**
  * @brief	STGAP4_CRC
  *			Calculate the CRC of a provided byte value
  *			Internal use only.
  *
  * @param	in Byte, value on which CRC have to be calculated
  * @param	in InitValue, crc initialization value
  * @retval Calculated CRC
 */
static uint8_t STGAP4_CRC(uint8_t Byte, uint8_t InitValue)
{
  uint8_t crc, i;

  //Initialize CRC
  crc = InitValue;

  //for every bit of data starting from
  //second bit
  for (i = 0; i < BYTE_LENGTH; i++)
  {
	//Apply CRC polynomial calculation algorithm
	crc = ( (crc << 1) ^ ( CRC_FIRST_BIT(crc, Byte, (BYTE_LENGTH-i-1) ) ? (CRC_POLYNOMIAL - 1) : 0x00 ) ) + CRC_FIRST_BIT(crc, Byte, (BYTE_LENGTH-i-1) );
  }

  //XOF 0XFF the calculated crc and return
  return (crc ^= 0xFF);

}
/**
  * @brief	STGAP4_SelectDeviceRegister
  *			Select the registers pointer from devices
  *			value list where data have to be read from or
  *			written to. 
  *			Internal use only.
  *
  * @param in Register: list of registers to be written or read
  * @param in NumberRegisters: number of register to be written or read
  * @param in Devices: list of devices values
  * @param out PRegContent: list of pointer to devices values
  * @retval none
 */
static void STGAP4_SelectDeviceRegister(RegisterType **Registers, uint8_t NumberRegisters, DeviceType *Devices, RegContentType *PRegContent[NUMBER_REGISTERS][NUMBER_DEVICES])
{
  uint8_t i, j;

  //for every registers
  for (i=0; i<NumberRegisters; i++)
  {
	//for every devices
	for (j=0; j<NUMBER_DEVICES; j++)
	{
	  //select register address
	  switch (Registers[i]->Address)
	  {
		case CFG1_ADDR:
		{
		  PRegContent[i][j] = &(Devices[j].Cfg1);
		  break;
		}
		case CFG2_ADDR:
		{
		  PRegContent[i][j] = &(Devices[j].Cfg2);
		  break;
		}
		case CFG3_ADDR:
		{
		  PRegContent[i][j] = &(Devices[j].Cfg3);
		  break;
		}
		case CFG4_ADDR:
		{
		  PRegContent[i][j] = &(Devices[j].Cfg4);
		  break;
		}
		case CFG5_ADDR:
		{
		  PRegContent[i][j] = &(Devices[j].Cfg5);
		  break;
		}
		case CFG6_ADDR:
		{
		  PRegContent[i][j] = &(Devices[j].Cfg6);
		  break;
		}
		case CFG7_ADDR:
		{
		  PRegContent[i][j] = &(Devices[j].Cfg7);
		  break;
		}
		case TEST1_ADDR:
		{
		  PRegContent[i][j] = &(Devices[j].Test1);
		  break;
		}
		case DIAG1A_ADDR:
		{
		  PRegContent[i][j] = &(Devices[j].Diag1a);
		  break;
		}
		case DIAG2A_ADDR:
		{
		  PRegContent[i][j] = &(Devices[j].Diag2a);
		  break;
		}
		case DIAG1B_ADDR:
		{
		  PRegContent[i][j] = &(Devices[j].Diag1b);
		  break;
		}
		case DIAG2B_ADDR:
		{
		  PRegContent[i][j] = &(Devices[j].Diag2b);
		  break;
		}
		case STATUS1_ADDR:
		{
		  PRegContent[i][j] = &(Devices[j].Status1);
		  break;
		}
		case STATUS2_ADDR:
		{
		  PRegContent[i][j] = &(Devices[j].Status2);
		  break;
		}
		case STATUS3_ADDR:
		{
		  PRegContent[i][j] = &(Devices[j].Status3);
		  break;
		}
		case STATUS4_ADDR:
		{
		  PRegContent[i][j] = &(Devices[j].Status4);
		  break;
		}
		case STATUS5_ADDR:
		{
		  PRegContent[i][j] = &(Devices[j].Status5);
		  break;
		}
		default:
		{
		  PRegContent = NULL;
		  break;
		}
	  
	  }//end select register address
	
	}//end for every devices
	
  }//end for every registers

}


/* Functions ---------------------------------------------------------*/
/**
  * @brief	STGAP4_TxRx
  *			Recall SPI driver to transmit and fetch byte over
  *			SPI interface. Received byte are necessary as many 
  *			as transmitted byte.
  *
  * @param	in TxByte: Array of byte to be transmitted.
  * @param	out RxByte: Array of byte to be received.
  * @param	in BufferSize: Number of byte to be transmitted\received.
  * @retval Result: OK or error.
*/
ResultType STGAP4_TxRx(uint8_t *TxByte, uint8_t *RxByte, uint8_t BufferSize)
{
  ResultType result = OK;

  uint32_t framesSent = spi_exchange(&DRV_SPI1, TxByte, RxByte, BufferSize);
  if (framesSent != BufferSize ) {
	result = SPI_ERROR;
  }

  return result;
}

/**
  * @brief	STGAP4_Tx
  *			Recall SPI driver to transmit over
  *			SPI interface. 
  *
  * @param	in TxByte: Array of byte to be transmitted.
  * @param	in BufferSize: Number of byte to be transmitted.
  * @retval Result: OK or error.
*/
ResultType STGAP4_Tx(uint8_t *TxByte, uint8_t BufferSize)
{
  ResultType result = OK;

  uint8_t RxByte[100];
  uint32_t framesSent = spi_exchange(&DRV_SPI1, TxByte, RxByte, BufferSize);
  if (framesSent != BufferSize ) {
	result = SPI_ERROR;
  }
  return result;
}

/**
  * @brief	STGAP4_ChipSelect
  *			Toggle CS signal state.
  *
  * @param	in State: Required state, HIGH or LOW.
  * @retval none.
*/
ResultType STGAP4_ChipSelect(StateType State)
{
  ResultType result = OK;

  if (State == LOW) {
	  gpio_clear_pin(SPI1_NSS);

	}
	if (State == HIGH) {
	  gpio_set_pin(SPI1_NSS);

	}
  return result;
}

/**
  * @brief	STGAP4_Shutdown
  *			Toggle SD signal state.
  *
  * @param	in State: Required state, HIGH or LOW.
  * @retval none.
*/
ResultType STGAP4_Shutdown(StateType State)
{
  ResultType result = OK;

  if (State == LOW) {
	gpio_clear_pin(STGAP4_DIS_HS);
	gpio_clear_pin(STGAP4_DIS_LS);
  }
  if (State == HIGH) {
	gpio_set_pin(STGAP4_DIS_HS);
	gpio_set_pin(STGAP4_DIS_LS);
  }
  return result;
}

static void STGAP4_TIM_Init(void)
{
  LL_TIM_InitTypeDef TIM_InitStruct = {0};
  TIM_TypeDef *TIMx = TIM8;

  //handle->clock_enable(true);
  if(TIMx == TIM1)
  {
	/* Peripheral clock enable */
	clock_enable_TIM1(true);
  }
  if(TIMx == TIM8)
  {
	/* Peripheral clock enable */
	clock_enable_TIM8(true);
  }

  TIM_InitStruct.Prescaler = 0;
  TIM_InitStruct.CounterMode = LL_TIM_COUNTERMODE_UP;
  TIM_InitStruct.Autoreload = 65535;
  TIM_InitStruct.ClockDivision = LL_TIM_CLOCKDIVISION_DIV1;
  LL_TIM_Init(TIMx, &TIM_InitStruct);
  LL_TIM_DisableARRPreload(TIMx);
  LL_TIM_SetClockSource(TIMx, LL_TIM_CLOCKSOURCE_INTERNAL);
  LL_TIM_SetTriggerOutput(TIMx, LL_TIM_TRGO_RESET);
  LL_TIM_DisableMasterSlaveMode(TIMx);
}

/**
  * @brief	STGAP4_Wait
  *			Wait for required time by
  *			monitoring a timer value till i
  *			reaches the indicated count.
  *
  * @param	in Count: timer count .
  * @retval none.
*/
void STGAP4_Wait(uint32_t Count)
{
   //Reset timer count
   TIMER_HANDLE->CNT = 0;
   //Loop until the counter has reached the final value
   while (TIMER_HANDLE->CNT <= Count);
}

/**
  * @brief	STGAP4_TimerStart
  *			Start the timer for the waiting
  *			time monitoring.
  *
  * @param	none
  * @retval none.
 */
void STGAP4_TimerStart(void)
{
  LL_TIM_EnableCounter(TIMER_HANDLE);
}

/**
  * @brief	STGAP4_TimerStop
  *			Start the timer for the waiting
  *			time monitoring.
  *
  * @param	none
  * @retval none.
 */
void STGAP4_TimerStop(void)
{
  LL_TIM_DisableCounter(TIMER_HANDLE);
}

uint16_t STGAP4S_CheckErrors(uint32_t* errorNow,uint32_t* errorOccurred);
/**
  * @brief	Schedule function to be called with periodicity.
  * @retval none.
  */
void STGAP4S_Schedule(void)
{
  uint8_t i;
  uint16_t fault;

  if (sch_checkerror)
  {
	fault = STGAP4S_CheckErrors(STGAP4S_wErrorsNow,STGAP4S_wErrorsOccurred);
	if (!fault)
	{
	   for (i = 0; i < NUMBER_DEVICES; i++)
	   {
		 if ((STGAP4S_wErrorsNow[i] != 0)||((STGAP4S_wErrorsOccurred[i] != 0)))
		 {
		  /* Status Registers report errors, SPI communication is OK */
		  sch_state = SCH_TEST_FAULT;
		  /* Fault Led On if any; */
		}
	  }
	}
	else
	{
	   /* SPI Communication KO */
	   sch_state = SCH_TEST_FAULT;
	}	

	/* Clear scheduler variable: one time read */
	sch_checkerror = 0;
  }
}

/**
  * @brief	Check errors of STGAP4S devices
  * @param	errorNow buffer of returned bitfields containing error flags
  *			coming from STGAP4S device that are currently active.\n
  *			The buffer have to be provided from the caller.
  * @param	errorOccurred buffer of returned bitfields containing error flags
  *			coming from STGAP4S device that are over.\n
  *			The buffer have to be provided from the caller.
  * @retval uint16_t It returns status for ReadMultipleDevice.
  */
uint16_t STGAP4S_CheckErrors(uint32_t* errorNow,uint32_t* errorOccurred)
{
  uint32_t errorFromDevices[NUMBER_DEVICES] = {0};
  ResultType status = OK;
  uint8_t dev = 0;
  
  /* Read status registers	*/
  status = STGAP4_Read(StatusRegisters, sizeof(StatusRegisters) / sizeof(*StatusRegisters), Devices);
		for (dev = 0; dev < NUMBER_DEVICES; dev++)
		{
		  /*********			STATUS_1		  **********/
		  if(Devices[dev].Status1.ActualValue != 0)
		  {
	  status = STATUS_ERROR;
	}
			g_statusReg[dev].REGS.STATUS_1.R = (uint8_t)Devices[dev].Status1.ActualValue;	  
			errorFromDevices[dev] = Devices[dev].Status1.ActualValue;			
	 
		  /*********			STATUS_2		  **********/
		  if(Devices[dev].Status2.ActualValue != 0)
		  {
	  status = STATUS_ERROR;
	}
			g_statusReg[dev].REGS.STATUS_2.R = Devices[dev].Status2.ActualValue; 
			errorFromDevices[dev] |= Devices[dev].Status2.ActualValue;			
	 
		  /*********			STATUS_3		  **********/
		  if(Devices[dev].Status3.ActualValue != 0)
		  {
	  status = STATUS_ERROR;
	}
			g_statusReg[dev].REGS.STATUS_3.R = Devices[dev].Status3.ActualValue;
			errorFromDevices[dev] |= Devices[dev].Status3.ActualValue;			
	 
		  /*********			STATUS_4		  **********/
		  if(Devices[dev].Status4.ActualValue != 0)
		  {
	  status = STATUS_ERROR;
	}  
			g_statusReg[dev].REGS.STATUS_4.R = Devices[dev].Status4.ActualValue;	 
			errorFromDevices[dev] |= Devices[dev].Status4.ActualValue;						
	 
  }
  /* if Buffers provided from the caller are available */
  if ((errorNow)&&(errorOccurred))
  { 
	for (dev = 0; dev < NUMBER_DEVICES; dev++)
	{
	  STGAP4S_wErrorsNow[dev] = errorFromDevices[dev];
	  STGAP4S_wErrorsOccurred[dev] |= STGAP4S_wErrorsNow[dev];
	  errorNow[dev] = STGAP4S_wErrorsNow[dev];
	  errorOccurred[dev] = STGAP4S_wErrorsOccurred[dev];
	}
  }
  return status;
}

/**
  * @brief	It returns MC_DIAG_ERROR or MC_NO_ERROR depending on
  *			external driver Diagnostic values
  * @retval uint16_t Fault code error
  */
uint16_t STGAP4S_CheckFaultState(void)
{
  uint16_t hFault;
  uint8_t i;

  hFault = MC_NO_ERROR;
  if(STGAP4S_init != OK)
  {
    hFault = MC_DIAG_ERROR;
  }
  else 
  {
  for (i = 0; i < NUMBER_DEVICES; i++)
  {
	if (STGAP4S_wErrorsNow[i] != 0)
	{
	  hFault = MC_DIAG_ERROR;
	  break;
	}
  }
  }
  return hFault;
}

/**
 * @brief		Returns the value of the status registers for a single device
 *				Updated during STGPAP4S_CheckErrors (schedule)
 *				Status Registers are:
 *				- STATUS REGISTER 1
 *				- STATUS REGISTER 2
 *				- STATUS REGISTER 3
 *				- STATUS REGISTER 4 
 *
 * @return		STGAP4S_statusReg_t Status registers pointer
 *
 * @notapi
 */
STGAP4S_statusReg_t* STGAP4S_GetStatusRegisters(uint16_t device)
{
	return &(g_statusReg[device]);
}

/**
 * @brief		Set Global variable sch_checkerror used in the schedule
 *				Called by user to enable SPI communication to read Status registers
 *
 * @return		true if the global variable is set
 *
 * @notapi
 */
bool STGAP4S_SetDiagRegisters(uint8_t set)
{
  if(set != 1u)
	return false;
  sch_checkerror = set;
  return true;
}


/**
 * @brief			Recovers driver from faults
 *
 *
 * @return			status value (SPI status)
 *
 * @notapi
 */
uint16_t STGAP4S_FaultAck(void)
{
  ResultType result;
  uint16_t fault;
  
  /* Set shutdown low */
  CHECK_FLOW(STGAP4_Shutdown(LOW));
  
  /* Send Reset status command. */
  STGAP4_Command(STGAP4_STATUS_RESET);
  
  EXIT:
  /* Set shutdown high */  
  STGAP4_Shutdown(HIGH);
  
  fault = STGAP4S_CheckErrors(STGAP4S_wErrorsNow,STGAP4S_wErrorsOccurred);
  
  if (fault==OK)
  {
	LL_TIM_ClearFlag_BRK(TIM1);
	LL_TIM_ClearFlag_BRK2(TIM1);

	/* Enable TIM brt*/
	LL_TIM_EnableIT_BRK(TIM1);
  }
  
  return result;
}


/**
 * @brief	IRQ handler function implementation. Called by external interrupt
 *			
 */
void STGAP4S_irqfaultRecovery(void)
{
  sch_checkerror = 1u;
}

/**
 * @brief	Convert ADC values 
 *			
 */
static float STGAP4_ConvertADC(uint8_t cfg, uint8_t value) {
  float v1 = V_lowth_Values [LOW_TH(cfg)];
  float v2 = V_highth_Values[HIGH_TH(cfg)];
  return v1 + ((value * (v2 - v1)) / 255.0f);
}

/**
 * @brief	Convert ADC values from status register
 *			
 */
float STGAP4_GetADCVoltFromStatusReg(DeviceType dev) {
  uint8_t value = dev.Status5.ActualValue;
  return STGAP4_ConvertADC(dev.Cfg7.ActualValue, value);
}

/**
 * @brief	Get ADC values 
 *			
 */
float STGAP4_GetADCVoltFromValue(DeviceType dev, uint8_t value) {
  return STGAP4_ConvertADC(dev.Cfg7.ActualValue, value);
}
