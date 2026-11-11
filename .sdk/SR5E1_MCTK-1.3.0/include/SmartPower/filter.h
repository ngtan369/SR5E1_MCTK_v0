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
*   @file    filter.h
*   @version 1.2.0
*   @brief   This file contains filtering functionality
*
*   @details This file contains filtering functionality
*
 *
 */

#ifndef INCLUDE_FILTER_H_
#define INCLUDE_FILTER_H_

#include <stdint.h>

#define SAMPLE_BUFF_LENGHT 500

typedef struct
{
  float buff[SAMPLE_BUFF_LENGHT];
  uint16_t buff_index;
  uint16_t buff_samples;
} Filter_t;

void FilterInit(Filter_t* handle);
float FilterNewSample(Filter_t* handle, float newSample);

#endif /* INCLUDE_FILTER_H_ */
