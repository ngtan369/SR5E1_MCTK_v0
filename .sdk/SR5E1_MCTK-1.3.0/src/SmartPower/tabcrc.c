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
*   @file    tabcrc.c
*   @version M4_VERSION
*   @brief   This file provides firmware functions that implement the features
*            of the CRC component of the Stellar Motor Control.
*          
*   @details This file provides firmware functions that implement the features
*            of the CRC component of the Stellar Motor Control.
 */
#include "typedefs.h"
#include "tabcrc.h"

#define SWAP_END_32(value) \
{ \
    int32_t swappedvalue; \
    swappedvalue = ((value>>24)&0xff) | ((value<<8)&0xff0000) |((value>>8)&0xff00) | ((value<<24)&0xff000000); \
    value = swappedvalue; \
}

// ****************************************************************************
// Name:        tabcrcInit
// Parameters:
//              tabcrcConfigSet_t * config - pointer to configuration the
//              configuration have folowed fields which must be configured
//              before the init function is used:
//                uint16_t frame_lengh;        //frame length number of active data bits [8..2040]
//                uint8_t crc_length;         //length of CRC in bits [1..7]
//                uint8_t polynom;            //CRC polynom [1..255]
//                uint8_t seed_value;         //CRC seed value [0..255]
//                uint8_t frame_position;     //highest (first) bit position in input streem [0..7]
//              
// Returns:     none
// Description:
//              CRC driver initialization. The function calculates
//              constants used during CRC calculation
// ****************************************************************************
void tabcrcInit(tabcrcConfigSet_t * config){
    uint16_t input_frame;
    uint16_t bit_mask;
    uint16_t temp_data;
    uint16_t polynom;
    uint16_t counter;
    uint8_t polyshift = 15 -config->crc_length;   //number of active bits in polynom

    config->polynom |= (1<<config->crc_length);
    for(input_frame = 0; input_frame < 256; input_frame++){
        bit_mask = 0x8000;
        temp_data = input_frame<<8;
        polynom = config->polynom << polyshift;
        counter = 8;
        while (counter > 0){
            counter--;
            if (bit_mask & temp_data) {
                temp_data = temp_data ^ polynom;
            }
            bit_mask = bit_mask >> 1;
            polynom = polynom >> 1;
        }
        config->crc_tab_p[input_frame] = temp_data;
    }

    config->frame_last_index = (config->frame_position+config->frame_lengh) / 8;
    config->seed_value_aligned = config->seed_value << (8 - config->crc_length);
    config->rest_bits = config->frame_lengh & 0x7;
    config->result_shift = 8-config->crc_length-config->rest_bits;
    config->polynom_shifted = config->polynom<<(7 - config->crc_length);
}

// ****************************************************************************
// Name:        CrcCalc8bitLookupTab
// Parameters:
//             tabcrcConfigSet_t  config - pointer to configuration
//             uint8_t * frame - pointer to highest byte of frame
// Returns:
//               CRC
// Description:
//              CRC calculation based on precalculated 8bits table
//              tabcrcInit must be executed before driver using
//              The not used bits in frame must be clear during CRC calculation
// ****************************************************************************
uint8_t CrcCalc8bitLookupTab (tabcrcConfigSet_t * config, uint8_t * frame_inv){

    uint8_t dataout;
    uint8_t bitmask,bitnumber,poly;
    uint8_t lastIndex = config->frame_last_index;
    uint8_t frameShiftl = config->frame_position;
    uint32_t value = *(uint32_t*)frame_inv;
    SWAP_END_32(value);
    uint8_t* frame = (uint8_t*)&value+1;
    
    if(0 == frameShiftl){
        uint8_t tabPointer = frame[0] ^ config->seed_value_aligned;
        uint8_t frameIndex = 1;
        dataout =  config->crc_tab_p[tabPointer];
        while(frameIndex < lastIndex){
            tabPointer = frame[frameIndex] ^dataout;
            dataout =  config->crc_tab_p[tabPointer];
            frameIndex++;
        }
        dataout = frame[frameIndex] ^dataout;
    }
    else{
        uint8_t frameShiftr = 8 - frameShiftl;
        uint8_t tabPointer = frame[0] << frameShiftl;
        uint8_t frameIndex = 1;
        tabPointer |= frame[1] >>frameShiftr;
        tabPointer ^=  config->seed_value_aligned;
        dataout =  config->crc_tab_p[tabPointer];
        while(frameIndex < lastIndex){
            tabPointer = frame[frameIndex] << frameShiftl;
            frameIndex++;
            tabPointer |= frame[frameIndex] >> frameShiftr;
            tabPointer ^= dataout;
            dataout =  config->crc_tab_p[tabPointer];
        }
        tabPointer = frame[frameIndex] << frameShiftl;
        frameIndex++;
        tabPointer |= frame[frameIndex] >> frameShiftr;
        dataout ^= tabPointer;
    }
    //Process rest of bits if CRC is lower than 8bits
    bitmask = 0x80;
    bitnumber = config->rest_bits;
    poly = config->polynom_shifted;
    while(bitnumber > 0){
        if(bitmask & dataout ){                     //last bit 7, so the rest is bit 0..6
            dataout = dataout ^ poly;
        }
        bitnumber--;
        bitmask = bitmask>>1;
        poly = poly>>1;
    }
    dataout = dataout >> (config->result_shift);
    return dataout;
}
