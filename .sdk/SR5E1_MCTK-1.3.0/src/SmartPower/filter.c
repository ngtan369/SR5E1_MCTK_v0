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
*   @file    filter.c
*   @version 1.2.0
*   @brief   This file contains filtering functionality
*
*   @details This file contains filtering functionality
*
 *
 */

#include "filter.h"

void FilterInit(Filter_t* handle)
{
  handle->buff_index = 0u;
  handle->buff_samples = 0u;
  for (uint16_t i = 0; i < SAMPLE_BUFF_LENGHT; i++ ) {
    handle->buff[i] = 0.0f;
  }
}

float FilterNewSample(Filter_t *handle, float newSample) {
  handle->buff[handle->buff_index] = newSample;

  if (handle->buff_index < SAMPLE_BUFF_LENGHT) {
    handle->buff_index++;
  } else {
    handle->buff_index = 0;
  }

  if (handle->buff_samples < SAMPLE_BUFF_LENGHT) {
    handle->buff_samples++;
  } else {
    handle->buff_samples = SAMPLE_BUFF_LENGHT;
  }

  float buff_sum = 0.0f;
  for (uint16_t i = 0; i < handle->buff_samples; i++) {
    buff_sum += handle->buff[i];
  }

  float filtered = buff_sum / handle->buff_samples;

  return filtered;
}
