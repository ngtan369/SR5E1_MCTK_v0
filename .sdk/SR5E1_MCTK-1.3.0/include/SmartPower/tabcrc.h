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
*   @file    tabcrc.h
*   @version M4_VERSION
*   @brief   This file provides firmware functions that implement the features
*            of the CRC component of the Stellar Motor Control.
*          
*   @details This file provides firmware functions that implement the features
*            of the CRC component of the Stellar Motor Control.
 */
// ****************************************************************************
// ******************************* Public Constants *******************************
// ****************************************************************************
#define CRC_TABLE_SIZE   256
// ****************************************************************************
// ******************************* Public Types *******************************
// ****************************************************************************
//CRC configuration structure
typedef struct
{
  //public configuration
  uint16_t frame_lengh;       //frame length number of active data bits [8..2040]
  uint8_t crc_length;         //length of CRC in bits [1..7]
  uint8_t polynom;            //CRC polynom [1..255]
  uint8_t seed_value;         //CRC seed value [0..255]
  uint8_t frame_position;     //highest (first) bit position in input streem [0..7]
  //local variables
  uint8_t seed_value_aligned; //Internal variable will be calculated during init
  uint8_t frame_last_index;         //Internal variable will be calculated during init
  uint8_t rest_bits;            //Number of rest bits which needs to be process serially
  uint8_t result_shift;         //Results shift
  uint8_t polynom_shifted;      //Polynom shifted shift
  uint8_t crc_tab_p[CRC_TABLE_SIZE]; //CRC table
}tabcrcConfigSet_t;

// ****************************************************************************
// **************************** Functions Prototypes **************************
// ****************************************************************************

// ****************************************************************************
// Name:        tabcrcInit
// Parameters:
//              tabcrcConfigSet_t * config - pointer to configuration the
//              configuration have followed fields which must be configured
//              before the init function is used:
//                uint16_t frame_lengh;       frame length number of active data bits [8..2040]
//                uint8_t crc_length;         length of CRC in bits [1..7]
//                uint8_t polynom;            CRC polynom [1..255]
//                uint8_t seed_value;         CRC seed value [0..255]
//                uint8_t frame_position;     highest (first) bit position in input stream [0..7]
//              
// Returns:     none
// Description:
//              CRC driver initialization. The function calculates
//              constants used during CRC calculation
// ****************************************************************************
void tabcrcInit(tabcrcConfigSet_t * config);

// ****************************************************************************
// Name:        CrcCalc8bitLookupTab
// Parameters:
//             tabcrcConfigSet_t  config - pointer to configuration
//             uint8_t * frame - pointer to highest byte of frame
// Returns:
//               CRC
// Description:
//              CRC calculation based on pre calculated 8bits table
//              tabcrcInit must be executed before driver using
// ****************************************************************************
uint8_t CrcCalc8bitLookupTab (tabcrcConfigSet_t * config, uint8_t * frame);

