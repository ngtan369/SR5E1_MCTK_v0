/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.h
  * @brief          : Header for main.c file.
  *                   This file contains the common defines of the application.
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2021-2026 STMicroelectronics.
  * All rights reserved.</center></h2>
  *
  * This software component is licensed by ST under Ultimate Liberty license
  * SLA0044, the "License"; You may not use this file except in compliance with
  * the License. You may obtain a copy of the License at:
  *                             www.st.com/SLA0044
  *
  ******************************************************************************
  */ 
/* USER CODE END Header */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __MAIN_H
#define __MAIN_H

#ifdef __cplusplus
extern "C" {
#endif


/************************* DEBUG APPLICATION *************************/
/* Enable debug on DEBUG_PIN_x to measure tasks duration */
#define MCTK_DEBUG_TASK

/* Enable debug variables for current RPM and angle (for aux and main sensors) */
#define MCTK_DEBUG_RPM_ANGLE

/* Enable debug array for ANGLE (for aux and main sensors) */
#define MCTK_DEBUG_ANGLE_SENSOR
#define MCTK_DEBUG_ANGLE_SENSOR_BUFFER_SIZE 4000

/* Select the motor-control application on branch openloop. */
//#define LED_ONLY_TEST
#define OPEN_LOOP_FOC_TEST

/* Enable debug to diff main sensor vs aux sensor (DebugTestEncHallDiff) */
#define MCTK_DEBUG_ANGLE_DIFF


/* Enable debug in current sensig */
#define MCTK_DEBUG_CURRENT
#define MCTK_DEBUG_CURRENT_SIZE 1200

/* Nhat's def */
#define MCTK_DEBUG_CHART
#define IQD_CHART
//#define MCTK_MC_MODE
//#define MCTK_DEBUG_OFF_STDRIVE601
//#define TEST_MCLIVE_VQD
//#define EXTERNAL_THROTTLE
//#define HILL_ASSIST

/* #define REG_CONV_DISABLE */

/* Enable/Disable minimal OPEN LOOP */
#define OPEN_LOOP_FOC                   TRUE
#define OL_VOLT                          2600
//#define OL_ANGLE                        16383
#define OL_ANGLE                        800
#define OL_DPP                             78
//#define OL_DPP                             32
//#define OPEN_LOOP_SPEED_RPM               30
//#define OPEN_LOOP_SPEED_RAMP_DURATION_MS 500

#define stringify(_x_)         #_x_
#define BOARD_FILE(_a_)        stringify(_a_.h)
#include BOARD_FILE(BOARD_NAME)

/* Exported functions prototypes ---------------------------------------------*/
void Error_Handler(void);
#ifdef USE_L9502
void L9502_En_Button(void);
#endif
#ifdef USE_L9907
void L9907_En2_Button(void);
#endif
#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
