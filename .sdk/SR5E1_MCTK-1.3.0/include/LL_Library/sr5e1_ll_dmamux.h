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
*   @file    sr5e1_ll_dmamux.h
*   @version M4_VERSION
*   @brief   Header file of DMAMUX LL module.
*
*   @details Header file of DMAMUX LL module.
*
 */
/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef SR5E1_LL_DMAMUX_H
#define SR5E1_LL_DMAMUX_H

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "platform.h"
#include "sr5e1_macros.h"

/** @addtogroup SR5E1_LL_Driver
  * @{
  */

#if defined (DMAMUX1)

/** @defgroup DMAMUX_LL DMAMUX
  * @{
  */

/* Private types -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private constants ---------------------------------------------------------*/
/** @defgroup DMAMUX_LL_Private_Constants DMAMUX Private Constants
  * @{
  */
/* Define used to get DMAMUX CCR register size */
#define DMAMUX_CCR_SIZE                   0x00000004U

/* Define used to get DMAMUX RGCR register size */
#define DMAMUX_RGCR_SIZE                  0x00000004U

/* Define used to get DMAMUX RequestGenerator offset */
#define DMAMUX_REQ_GEN_OFFSET             (DMAMUX1_RequestGenerator0_BASE - DMAMUX1_BASE)
/* Define used to get DMAMUX Channel Status offset */
#define DMAMUX_CH_STATUS_OFFSET           (DMAMUX1_ChannelStatus_BASE - DMAMUX1_BASE)
/* Define used to get DMAMUX RequestGenerator status offset */
#define DMAMUX_REQ_GEN_STATUS_OFFSET      (DMAMUX1_RequestGenStatus_BASE - DMAMUX1_BASE)

/**
  * @}
  */

/* Private macros ------------------------------------------------------------*/
/* Exported types ------------------------------------------------------------*/
/* Exported constants --------------------------------------------------------*/
/** @defgroup DMAMUX_LL_Exported_Constants DMAMUX Exported Constants
  * @{
  */
/** @defgroup DMAMUX_LL_EC_CLEAR_FLAG Clear Flags Defines
  * @brief    Flags defines which can be used with LL_DMAMUX_WriteReg function
  * @{
  */
#define LL_DMAMUX_CFR_CSOF0               DMAMUX_CFR_CSOF0       /*!< Synchronization Event Overrun Flag Channel 0   */
#define LL_DMAMUX_CFR_CSOF1               DMAMUX_CFR_CSOF1       /*!< Synchronization Event Overrun Flag Channel 1   */
#define LL_DMAMUX_CFR_CSOF2               DMAMUX_CFR_CSOF2       /*!< Synchronization Event Overrun Flag Channel 2   */
#define LL_DMAMUX_CFR_CSOF3               DMAMUX_CFR_CSOF3       /*!< Synchronization Event Overrun Flag Channel 3   */
#define LL_DMAMUX_CFR_CSOF4               DMAMUX_CFR_CSOF4       /*!< Synchronization Event Overrun Flag Channel 4   */
#define LL_DMAMUX_CFR_CSOF5               DMAMUX_CFR_CSOF5       /*!< Synchronization Event Overrun Flag Channel 5   */
#define LL_DMAMUX_CFR_CSOF6               DMAMUX_CFR_CSOF6       /*!< Synchronization Event Overrun Flag Channel 6   */
#define LL_DMAMUX_CFR_CSOF7               DMAMUX_CFR_CSOF7       /*!< Synchronization Event Overrun Flag Channel 7   */
#define LL_DMAMUX_CFR_CSOF8               DMAMUX_CFR_CSOF8       /*!< Synchronization Event Overrun Flag Channel 8   */
#define LL_DMAMUX_CFR_CSOF9               DMAMUX_CFR_CSOF9       /*!< Synchronization Event Overrun Flag Channel 9   */
#define LL_DMAMUX_CFR_CSOF10              DMAMUX_CFR_CSOF10      /*!< Synchronization Event Overrun Flag Channel 10  */
#define LL_DMAMUX_CFR_CSOF11              DMAMUX_CFR_CSOF11      /*!< Synchronization Event Overrun Flag Channel 11  */
#define LL_DMAMUX_CFR_CSOF12              DMAMUX_CFR_CSOF12      /*!< Synchronization Event Overrun Flag Channel 12  */
#define LL_DMAMUX_CFR_CSOF13              DMAMUX_CFR_CSOF13      /*!< Synchronization Event Overrun Flag Channel 13  */
#define LL_DMAMUX_CFR_CSOF14              DMAMUX_CFR_CSOF14      /*!< Synchronization Event Overrun Flag Channel 14  */
#define LL_DMAMUX_CFR_CSOF15              DMAMUX_CFR_CSOF15      /*!< Synchronization Event Overrun Flag Channel 15  */
#define LL_DMAMUX_RGCFR_RGCOF0            DMAMUX_RGCFR_COF0      /*!< Request Generator 0 Trigger Event Overrun Flag */
#define LL_DMAMUX_RGCFR_RGCOF1            DMAMUX_RGCFR_COF1      /*!< Request Generator 1 Trigger Event Overrun Flag */
#define LL_DMAMUX_RGCFR_RGCOF2            DMAMUX_RGCFR_COF2      /*!< Request Generator 2 Trigger Event Overrun Flag */
#define LL_DMAMUX_RGCFR_RGCOF3            DMAMUX_RGCFR_COF3      /*!< Request Generator 3 Trigger Event Overrun Flag */
/**
  * @}
  */

/** @defgroup DMAMUX_LL_EC_GET_FLAG Get Flags Defines
  * @brief    Flags defines which can be used with LL_DMAMUX_ReadReg function
  * @{
  */
#define LL_DMAMUX_CSR_SOF0                DMAMUX_CSR_SOF0       /*!< Synchronization Event Overrun Flag Channel 0   */
#define LL_DMAMUX_CSR_SOF1                DMAMUX_CSR_SOF1       /*!< Synchronization Event Overrun Flag Channel 1   */
#define LL_DMAMUX_CSR_SOF2                DMAMUX_CSR_SOF2       /*!< Synchronization Event Overrun Flag Channel 2   */
#define LL_DMAMUX_CSR_SOF3                DMAMUX_CSR_SOF3       /*!< Synchronization Event Overrun Flag Channel 3   */
#define LL_DMAMUX_CSR_SOF4                DMAMUX_CSR_SOF4       /*!< Synchronization Event Overrun Flag Channel 4   */
#define LL_DMAMUX_CSR_SOF5                DMAMUX_CSR_SOF5       /*!< Synchronization Event Overrun Flag Channel 5   */
#define LL_DMAMUX_CSR_SOF6                DMAMUX_CSR_SOF6       /*!< Synchronization Event Overrun Flag Channel 6   */
#define LL_DMAMUX_CSR_SOF7                DMAMUX_CSR_SOF7       /*!< Synchronization Event Overrun Flag Channel 7   */
#define LL_DMAMUX_CSR_SOF8                DMAMUX_CSR_SOF8       /*!< Synchronization Event Overrun Flag Channel 8   */
#define LL_DMAMUX_CSR_SOF9                DMAMUX_CSR_SOF9       /*!< Synchronization Event Overrun Flag Channel 9   */
#define LL_DMAMUX_CSR_SOF10               DMAMUX_CSR_SOF10      /*!< Synchronization Event Overrun Flag Channel 10  */
#define LL_DMAMUX_CSR_SOF11               DMAMUX_CSR_SOF11      /*!< Synchronization Event Overrun Flag Channel 11  */
#define LL_DMAMUX_CSR_SOF12               DMAMUX_CSR_SOF12      /*!< Synchronization Event Overrun Flag Channel 12  */
#define LL_DMAMUX_CSR_SOF13               DMAMUX_CSR_SOF13      /*!< Synchronization Event Overrun Flag Channel 13  */
#define LL_DMAMUX_CSR_SOF14               DMAMUX_CSR_SOF14      /*!< Synchronization Event Overrun Flag Channel 14  */
#define LL_DMAMUX_CSR_SOF15               DMAMUX_CSR_SOF15      /*!< Synchronization Event Overrun Flag Channel 15  */
#define LL_DMAMUX_RGSR_RGOF0              DMAMUX_RGSR_OF0       /*!< Request Generator 0 Trigger Event Overrun Flag */
#define LL_DMAMUX_RGSR_RGOF1              DMAMUX_RGSR_OF1       /*!< Request Generator 1 Trigger Event Overrun Flag */
#define LL_DMAMUX_RGSR_RGOF2              DMAMUX_RGSR_OF2       /*!< Request Generator 2 Trigger Event Overrun Flag */
#define LL_DMAMUX_RGSR_RGOF3              DMAMUX_RGSR_OF3       /*!< Request Generator 3 Trigger Event Overrun Flag */
/**
  * @}
  */

/** @defgroup DMAMUX_LL_EC_IT IT Defines
  * @brief    IT defines which can be used with LL_DMA_ReadReg and  LL_DMAMUX_WriteReg functions
  * @{
  */
#define LL_DMAMUX_CCR_SOIE                DMAMUX_CxCR_SOIE          /*!< Synchronization Event Overrun Interrupt               */
#define LL_DMAMUX_RGCR_RGOIE              DMAMUX_RGxCR_OIE          /*!< Request Generation Trigger Event Overrun Interrupt    */
/**
  * @}
  */

/** @defgroup DMAMUX1_Request_selection DMAMUX1 Request selection
  * @brief    DMAMUX1 Request selection
  * @{
  */
/* DMAMUX1 requests */
#define LL_DMAMUX1_REQ_MEM2MEM              0U   /*!< memory to memory transfer         */
#define LL_DMAMUX1_REQ_GENERATOR0           1U   /*!< DMAMUX1 request generator 0       */
#define LL_DMAMUX1_REQ_GENERATOR1           2U   /*!< DMAMUX1 request generator 1       */
#define LL_DMAMUX1_REQ_GENERATOR2           3U   /*!< DMAMUX1 request generator 2       */
#define LL_DMAMUX1_REQ_GENERATOR3           4U   /*!< DMAMUX1 request generator 3       */
#define LL_DMAMUX1_REQ_ADC1                 5U   /*!< DMAMUX1 ADC1 request              */
#define LL_DMAMUX1_REQ_BDAC1_CH1            6U   /*!< DMAMUX1 BDAC1 Channel 1 request   */
#define LL_DMAMUX1_REQ_BDAC1_CH2            7U   /*!< DMAMUX1 BDAC1 Channel 1 request   */
#define LL_DMAMUX1_REQ_TIM6_UP              8U   /*!< DMAMUX1 TIM6 UP request           */
#define LL_DMAMUX1_REQ_TIM7_UP              9U   /*!< DMAMUX1 TIM7 UP request           */
#define LL_DMAMUX1_REQ_SPI1_RX              10U  /*!< DMAMUX1 SPI1 RX request           */
#define LL_DMAMUX1_REQ_SPI1_TX              11U  /*!< DMAMUX1 SPI1 TX request           */
#define LL_DMAMUX1_REQ_SPI2_RX              12U  /*!< DMAMUX1 SPI2 RX request           */
#define LL_DMAMUX1_REQ_SPI2_TX              13U  /*!< DMAMUX1 SPI2 TX request           */
#define LL_DMAMUX1_REQ_SPI3_RX              14U  /*!< DMAMUX1 SPI3 RX request           */
#define LL_DMAMUX1_REQ_SPI3_TX              15U  /*!< DMAMUX1 SPI3 TX request           */
#define LL_DMAMUX1_REQ_I2C1_RX              16U  /*!< DMAMUX1 I2C1 RX request           */
#define LL_DMAMUX1_REQ_I2C1_TX              17U  /*!< DMAMUX1 I2C1 TX request           */
#define LL_DMAMUX1_REQ_I2C2_RX              18U  /*!< DMAMUX1 I2C2 RX request           */
#define LL_DMAMUX1_REQ_I2C2_TX              19U  /*!< DMAMUX1 I2C2 TX request           */
#define LL_DMAMUX1_REQ_USART1_RX            24U  /*!< DMAMUX1 USART1 RX request         */
#define LL_DMAMUX1_REQ_USART1_TX            25U  /*!< DMAMUX1 USART1 TX request         */
#define LL_DMAMUX1_REQ_USART2_RX            26U  /*!< DMAMUX1 USART2 RX request         */
#define LL_DMAMUX1_REQ_USART2_TX            27U  /*!< DMAMUX1 USART2 TX request         */
#define LL_DMAMUX1_REQ_USART3_RX            28U  /*!< DMAMUX1 USART3 RX request         */
#define LL_DMAMUX1_REQ_USART3_TX            29U  /*!< DMAMUX1 USART3 TX request         */
#define LL_DMAMUX1_REQ_ADC2                 36U  /*!< DMAMUX1 ADC2 request              */
#define LL_DMAMUX1_REQ_ADC3                 37U  /*!< DMAMUX1 ADC3  request             */
#define LL_DMAMUX1_REQ_ADC4                 38U  /*!< DMAMUX1 ADC4  request             */
#define LL_DMAMUX1_REQ_ADC5                 39U  /*!< DMAMUX1 ADC5  request             */
#define LL_DMAMUX1_REQ_TIM1_CH1             42U  /*!< DMAMUX1 TIM1 CH1 request          */
#define LL_DMAMUX1_REQ_TIM1_CH2             43U  /*!< DMAMUX1 TIM1 CH2 request          */
#define LL_DMAMUX1_REQ_TIM1_CH3             44U  /*!< DMAMUX1 TIM1 CH3 request          */
#define LL_DMAMUX1_REQ_TIM1_CH4             45U  /*!< DMAMUX1 TIM1 CH4 request          */
#define LL_DMAMUX1_REQ_TIM1_UP              46U  /*!< DMAMUX1 TIM1 UP request           */
#define LL_DMAMUX1_REQ_TIM1_TRIG            47U  /*!< DMAMUX1 TIM1 TRIG request         */
#define LL_DMAMUX1_REQ_TIM1_COM             48U  /*!< DMAMUX1 TIM1 COM request          */
#define LL_DMAMUX1_REQ_TIM8_CH1             49U  /*!< DMAMUX1 TIM8 CH1 request          */
#define LL_DMAMUX1_REQ_TIM8_CH2             50U  /*!< DMAMUX1 TIM8 CH2 request          */
#define LL_DMAMUX1_REQ_TIM8_CH3             51U  /*!< DMAMUX1 TIM8 CH3 request          */
#define LL_DMAMUX1_REQ_TIM8_CH4             52U  /*!< DMAMUX1 TIM8 CH4 request          */
#define LL_DMAMUX1_REQ_TIM8_UP              53U  /*!< DMAMUX1 TIM8 UP request           */
#define LL_DMAMUX1_REQ_TIM8_TRIG            54U  /*!< DMAMUX1 TIM8 TRIG request         */
#define LL_DMAMUX1_REQ_TIM8_COM             55U  /*!< DMAMUX1 TIM8 COM request          */
#define LL_DMAMUX1_REQ_TIM2_CH1             56U  /*!< DMAMUX1 TIM2 CH1 request          */
#define LL_DMAMUX1_REQ_TIM2_CH2             57U  /*!< DMAMUX1 TIM2 CH2 request          */
#define LL_DMAMUX1_REQ_TIM2_CH3             58U  /*!< DMAMUX1 TIM2 CH3 request          */
#define LL_DMAMUX1_REQ_TIM2_CH4             59U  /*!< DMAMUX1 TIM2 CH4 request          */
#define LL_DMAMUX1_REQ_TIM2_UP              60U  /*!< DMAMUX1 TIM2 UP request           */
#define LL_DMAMUX1_REQ_TIM3_CH1             61U  /*!< DMAMUX1 TIM3 CH1 request          */
#define LL_DMAMUX1_REQ_TIM3_CH2             62U  /*!< DMAMUX1 TIM3 CH2 request          */
#define LL_DMAMUX1_REQ_TIM3_CH3             63U  /*!< DMAMUX1 TIM3 CH3 request          */
#define LL_DMAMUX1_REQ_TIM3_CH4             64U  /*!< DMAMUX1 TIM3 CH4 request          */
#define LL_DMAMUX1_REQ_TIM3_UP              65U  /*!< DMAMUX1 TIM3 UP request           */
#define LL_DMAMUX1_REQ_TIM3_TRIG            66U  /*!< DMAMUX1 TIM3 TRIG request         */
#define LL_DMAMUX1_REQ_TIM4_CH1             67U  /*!< DMAMUX1 TIM4 CH1 request          */
#define LL_DMAMUX1_REQ_TIM4_CH2             68U  /*!< DMAMUX1 TIM4 CH2 request          */
#define LL_DMAMUX1_REQ_TIM4_CH3             69U  /*!< DMAMUX1 TIM4 CH3 request          */
#define LL_DMAMUX1_REQ_TIM4_CH4             70U  /*!< DMAMUX1 TIM4 CH4 request          */
#define LL_DMAMUX1_REQ_TIM4_UP              71U  /*!< DMAMUX1 TIM4 UP request           */
#define LL_DMAMUX1_REQ_TIM5_CH1             72U  /*!< DMAMUX1 TIM5 CH1 request          */
#define LL_DMAMUX1_REQ_TIM5_CH2             73U  /*!< DMAMUX1 TIM5 CH2 request          */
#define LL_DMAMUX1_REQ_TIM5_CH3             74U  /*!< DMAMUX1 TIM5 CH3 request          */
#define LL_DMAMUX1_REQ_TIM5_CH4             75U  /*!< DMAMUX1 TIM5 CH4 request          */
#define LL_DMAMUX1_REQ_TIM5_UP              76U  /*!< DMAMUX1 TIM5 UP request           */
#define LL_DMAMUX1_REQ_TIM5_TRIG            77U  /*!< DMAMUX1 TIM5 TRIG request         */
#define LL_DMAMUX1_REQ_TIM15_CH1            78U  /*!< DMAMUX1 TIM15 CH1 request         */
#define LL_DMAMUX1_REQ_TIM15_UP             79U  /*!< DMAMUX1 TIM15 UP request          */
#define LL_DMAMUX1_REQ_TIM15_TRIG           80U  /*!< DMAMUX1 TIM15 TRIG request        */
#define LL_DMAMUX1_REQ_TIM15_COM            81U  /*!< DMAMUX1 TIM15 COM request         */
#define LL_DMAMUX1_REQ_TIM16_CH1            82U  /*!< DMAMUX1 TIM16 CH1 request         */
#define LL_DMAMUX1_REQ_TIM16_UP             83U  /*!< DMAMUX1 TIM16 UP request          */
#define LL_DMAMUX1_REQ_TIM_TS_UP            84U  /*!< DMAMUX1 TIM TS UP request         */
#define LL_DMAMUX1_REQ_DAC1_CH1             91U  /*!< DMAMUX1 DAC1 Channel 1 request    */
#define LL_DMAMUX1_REQ_DAC1_CH2             92U  /*!< DMAMUX1 DAC1 Channel 2 request    */
#define LL_DMAMUX1_REQ_DAC2_CH1             93U  /*!< DMAMUX1 DAC2 Channel 1 request    */
#define LL_DMAMUX1_REQ_DAC2_CH2             94U  /*!< DMAMUX1 DAC2 Channel 2 request    */
#define LL_DMAMUX1_REQ_HRTIM1_MASTER        95U  /*!< DMAMUX1 HRTIM1 Master request 1   */
#define LL_DMAMUX1_REQ_HRTIM1_TIMER_A       96U  /*!< DMAMUX1 HRTIM1 TimerA request 2   */
#define LL_DMAMUX1_REQ_HRTIM1_TIMER_B       97U  /*!< DMAMUX1 HRTIM1 TimerB request 3   */
#define LL_DMAMUX1_REQ_HRTIM1_TIMER_C       98U  /*!< DMAMUX1 HRTIM1 TimerC request 4   */
#define LL_DMAMUX1_REQ_HRTIM1_TIMER_D       99U  /*!< DMAMUX1 HRTIM1 TimerD request 5   */
#define LL_DMAMUX1_REQ_HRTIM1_TIMER_E      100U  /*!< DMAMUX1 HRTIM1 TimerE request 6   */
#define LL_DMAMUX1_REQ_HRTIM1_TIMER_F      101U  /*!< DMAMUX1 HRTIM1 TimerE request 6   */
#define LL_DMAMUX1_REQ_DAC3_CH1            102U  /*!< DMAMUX1 DAC3 Channel 1 request    */
#define LL_DMAMUX1_REQ_DAC3_CH2            103U  /*!< DMAMUX1 DAC3 Channel 2 request    */
#define LL_DMAMUX1_REQ_DAC4_CH1            104U  /*!< DMAMUX1 DAC4 Channel 1 request    */
#define LL_DMAMUX1_REQ_DAC4_CH2            105U  /*!< DMAMUX1 DAC4 Channel 2 request    */
#define LL_DMAMUX1_REQ_SPI4_RX             106U  /*!< DMAMUX1 SPI4 RX request           */
#define LL_DMAMUX1_REQ_SPI4_TX             107U  /*!< DMAMUX1 SPI4 TX request           */
#define LL_DMAMUX1_REQ_CORDIC_READ         112U  /*!< DMAMUX1 CORDIC Read request       */
#define LL_DMAMUX1_REQ_CORDIC_WRITE        113U  /*!< DMAMUX1 CORDIC Write request      */
#define LL_DMAMUX1_REQ_HRTIM2_MASTER       116U  /*!< DMAMUX1 HRTIM2 Master request 1   */
#define LL_DMAMUX1_REQ_HRTIM2_TIMER_A      117U  /*!< DMAMUX1 HRTIM2 TimerA request 2   */
#define LL_DMAMUX1_REQ_HRTIM2_TIMER_B      118U  /*!< DMAMUX1 HRTIM2 TimerB request 3   */
#define LL_DMAMUX1_REQ_HRTIM2_TIMER_C      119U  /*!< DMAMUX1 HRTIM2 TimerC request 4   */
#define LL_DMAMUX1_REQ_HRTIM2_TIMER_D      120U  /*!< DMAMUX1 HRTIM2 TimerD request 5   */
#define LL_DMAMUX1_REQ_HRTIM2_TIMER_E      121U  /*!< DMAMUX1 HRTIM2 TimerE request 6   */
#define LL_DMAMUX1_REQ_HRTIM2_TIMER_F      122U  /*!< DMAMUX1 HRTIM2 TimerE request 6   */
#define LL_DMAMUX1_REQ_CAN_SUB_1_DMU_1_TX  130U  /*!< DMAMUX1 CAN SUB1 DMU1 TX request  */
#define LL_DMAMUX1_REQ_CAN_SUB_1_DMU_1_TXE 131U  /*!< DMAMUX1 CAN SUB1 DMU1 TXE request */
#define LL_DMAMUX1_REQ_CAN_SUB_1_DMU_1_RX0 132U  /*!< DMAMUX1 CAN SUB1 DMU1 RX0 request */
#define LL_DMAMUX1_REQ_CAN_SUB_1_DMU_1_RX1 133U  /*!< DMAMUX1 CAN SUB1 DMU1 RX1 request */
#define LL_DMAMUX1_REQ_CAN_SUB_1_DMU_2_TX  134U  /*!< DMAMUX1 CAN SUB1 DMU2 TX request  */
#define LL_DMAMUX1_REQ_CAN_SUB_1_DMU_2_TXE 135U  /*!< DMAMUX1 CAN SUB1 DMU2 TXE request */
#define LL_DMAMUX1_REQ_CAN_SUB_1_DMU_2_RX0 136U  /*!< DMAMUX1 CAN SUB1 DMU2 RX0 request */
#define LL_DMAMUX1_REQ_CAN_SUB_1_DMU_2_RX1 137U  /*!< DMAMUX1 CAN SUB1 DMU2 RX1 request */
#define LL_DMAMUX1_REQ_CAN_SUB_1_DMU_3_TX  138U  /*!< DMAMUX1 CAN SUB1 DMU3 TX request  */
#define LL_DMAMUX1_REQ_CAN_SUB_1_DMU_3_TXE 139U  /*!< DMAMUX1 CAN SUB1 DMU3 TXE request */
#define LL_DMAMUX1_REQ_CAN_SUB_1_DMU_3_RX0 140U  /*!< DMAMUX1 CAN SUB1 DMU3 RX0 request */
#define LL_DMAMUX1_REQ_CAN_SUB_1_DMU_3_RX1 141U  /*!< DMAMUX1 CAN SUB1 DMU3 RX1 request */
#define LL_DMAMUX1_REQ_CAN_SUB_1_DMU_4_TX  142U  /*!< DMAMUX1 CAN SUB1 DMU4 TX request  */
#define LL_DMAMUX1_REQ_CAN_SUB_1_DMU_4_TXE 143U  /*!< DMAMUX1 CAN SUB1 DMU4 TXE request */
#define LL_DMAMUX1_REQ_CAN_SUB_1_DMU_4_RX0 144U  /*!< DMAMUX1 CAN SUB1 DMU4 RX0 request */
#define LL_DMAMUX1_REQ_CAN_SUB_1_DMU_4_RX1 145U  /*!< DMAMUX1 CAN SUB1 DMU4 RX1 request */
#define LL_DMAMUX1_REQ_CAN_SUB_1_M_CAN_1   146U  /*!< DMAMUX1 CAN SUB1 MCAN1 request    */
#define LL_DMAMUX1_REQ_CAN_SUB_1_M_CAN_2   147U  /*!< DMAMUX1 CAN SUB1 MCAN2 request    */
#define LL_DMAMUX1_REQ_SD_ADC1             150U  /*!< DMAMUX1 SD-ADC1 request           */
#define LL_DMAMUX1_REQ_SD_ADC2             151U  /*!< DMAMUX1 SD-ADC2 request           */
/**
  * @}
  */

/** @defgroup DMAMUX_LL_EC_CHANNEL DMAMUX Channel
  * @{
  */
#define LL_DMAMUX_CHANNEL_0     0x00000000U  /*!< DMAMUX1 Channel 0  connected to DMA1 Channel 0 */
#define LL_DMAMUX_CHANNEL_1     0x00000001U  /*!< DMAMUX1 Channel 1  connected to DMA1 Channel 1 */
#define LL_DMAMUX_CHANNEL_2     0x00000002U  /*!< DMAMUX1 Channel 2  connected to DMA1 Channel 2 */
#define LL_DMAMUX_CHANNEL_3     0x00000003U  /*!< DMAMUX1 Channel 3  connected to DMA1 Channel 3 */
#define LL_DMAMUX_CHANNEL_4     0x00000004U  /*!< DMAMUX1 Channel 4  connected to DMA1 Channel 4 */
#define LL_DMAMUX_CHANNEL_5     0x00000005U  /*!< DMAMUX1 Channel 5  connected to DMA1 Channel 5 */
#define LL_DMAMUX_CHANNEL_6     0x00000006U  /*!< DMAMUX1 Channel 6  connected to DMA1 Channel 6 */
#define LL_DMAMUX_CHANNEL_7     0x00000007U  /*!< DMAMUX1 Channel 7  connected to DMA1 Channel 7 */
#define LL_DMAMUX_CHANNEL_8     0x00000008U  /*!< DMAMUX1 Channel 8  connected to DMA2 Channel 0 */
#define LL_DMAMUX_CHANNEL_9     0x00000009U  /*!< DMAMUX1 Channel 9  connected to DMA2 Channel 1 */
#define LL_DMAMUX_CHANNEL_10    0x0000000AU  /*!< DMAMUX1 Channel 10 connected to DMA2 Channel 2 */
#define LL_DMAMUX_CHANNEL_11    0x0000000BU  /*!< DMAMUX1 Channel 11 connected to DMA2 Channel 3 */
#define LL_DMAMUX_CHANNEL_12    0x0000000CU  /*!< DMAMUX1 Channel 12 connected to DMA2 Channel 4 */
#define LL_DMAMUX_CHANNEL_13    0x0000000DU  /*!< DMAMUX1 Channel 13 connected to DMA2 Channel 5 */
#define LL_DMAMUX_CHANNEL_14    0x0000000EU  /*!< DMAMUX1 Channel 14 connected to DMA2 Channel 6 */
#define LL_DMAMUX_CHANNEL_15    0x0000000FU  /*!< DMAMUX1 Channel 15 connected to DMA2 Channel 7 */
/**
  * @}
  */

/** @defgroup DMAMUX_LL_EC_SYNC_NO Synchronization Signal Polarity
  * @{
  */
#define LL_DMAMUX_SYNC_NO_EVENT            0x00000000U                               /*!< All requests are blocked                            */
#define LL_DMAMUX_SYNC_POL_RISING          DMAMUX_CxCR_SPOL_0                        /*!< Synchronization on event on rising edge             */
#define LL_DMAMUX_SYNC_POL_FALLING         DMAMUX_CxCR_SPOL_1                        /*!< Synchronization on event on falling edge            */
#define LL_DMAMUX_SYNC_POL_RISING_FALLING  (DMAMUX_CxCR_SPOL_0 | DMAMUX_CxCR_SPOL_1) /*!< Synchronization on event on rising and falling edge */
/**
  * @}
  */

/** @defgroup DMAMUX_LL_EC_SYNC_EVT Synchronization Signal Event
  * @{
  */
#define LL_DMAMUX1_SYNC_DMAMUX1_CH0_EVT   0x00000000U   /*!< DMAMUX1 synchronization Signal is DMAMUX1 Channel0 Event */
#define LL_DMAMUX1_SYNC_DMAMUX1_CH1_EVT   0x01000000U   /*!< DMAMUX1 synchronization Signal is DMAMUX1 Channel1 Event */
#define LL_DMAMUX1_SYNC_DMAMUX1_CH2_EVT   0x02000000U   /*!< DMAMUX1 synchronization Signal is DMAMUX1 Channel2 Event */
#define LL_DMAMUX1_SYNC_DMAMUX1_CH3_EVT   0x02000000U   /*!< DMAMUX1 synchronization Signal is DMAMUX1 Channel3 Event */
/**
  * @}
  */

/** @defgroup DMAMUX_LL_EC_REQUEST_GENERATOR Request Generator Channel
  * @{
  */
#define LL_DMAMUX_REQ_GEN_0           0x00000000U
#define LL_DMAMUX_REQ_GEN_1           0x00000001U
#define LL_DMAMUX_REQ_GEN_2           0x00000002U
#define LL_DMAMUX_REQ_GEN_3           0x00000003U
/**
  * @}
  */

/** @defgroup DMAMUX_LL_EC_REQUEST_GEN_POLARITY External Request Signal Generation Polarity
  * @{
  */
#define LL_DMAMUX_REQ_GEN_NO_EVENT             0x00000000U                                  /*!< No external DMA request  generation                        */
#define LL_DMAMUX_REQ_GEN_POL_RISING           DMAMUX_RGxCR_GPOL_0                          /*!< External DMA request generation on event on rising edge    */
#define LL_DMAMUX_REQ_GEN_POL_FALLING          DMAMUX_RGxCR_GPOL_1                          /*!< External DMA request generation on event on falling edge   */
#define LL_DMAMUX_REQ_GEN_POL_RISING_FALLING   (DMAMUX_RGxCR_GPOL_0 | DMAMUX_RGxCR_GPOL_1)  /*!< External DMA request generation on rising and falling edge */
/**
  * @}
  */

/** @defgroup DMAMUX_LL_EC_REQUEST_GEN External Request Signal Generation
  * @{
  */
#define LL_DMAMUX1_REQ_GEN_DMAMUX1_CH0_EVT   0U   /*!< DMAMUX1 Request generator Signal is DMAMUX1 Channel0 Event        */
#define LL_DMAMUX1_REQ_GEN_DMAMUX1_CH1_EVT   1U   /*!< DMAMUX1 Request generator Signal is DMAMUX1 Channel1 Event        */
#define LL_DMAMUX1_REQ_GEN_DMAMUX1_CH2_EVT   2U   /*!< DMAMUX1 Request generator Signal is DMAMUX1 Channel2 Event        */
#define LL_DMAMUX1_REQ_GEN_DMAMUX1_CH3_EVT   3U   /*!< DMAMUX1 Request generator Signal is DMAMUX1 Channel3 Event        */
/**
  * @}
  */

/**
  * @}
  */

/* Exported macro ------------------------------------------------------------*/
/** @defgroup DMAMUX_LL_Exported_Macros DMAMUX Exported Macros
  * @{
  */

/** @defgroup DMAMUX_LL_EM_WRITE_READ Common Write and read registers macros
  * @{
  */
/**
  * @brief  Write a value in DMAMUX register
  * @param  __INSTANCE__ DMAMUX Instance
  * @param  __REG__ Register to be written
  * @param  __VALUE__ Value to be written in the register
  * @retval None
  */
#define LL_DMAMUX_WriteReg(__INSTANCE__, __REG__, __VALUE__) WRITE_REG(__INSTANCE__->__REG__, (__VALUE__))

/**
  * @brief  Read a value in DMAMUX register
  * @param  __INSTANCE__ DMAMUX Instance
  * @param  __REG__ Register to be read
  * @retval Register value
  */
#define LL_DMAMUX_ReadReg(__INSTANCE__, __REG__) READ_REG(__INSTANCE__->__REG__)
/**
  * @}
  */

/**
  * @}
  */

/* Exported functions --------------------------------------------------------*/
/** @defgroup DMAMUX_LL_Exported_Functions DMAMUX Exported Functions
 * @{
 */

/** @defgroup DMAMUX_LL_EF_Configuration Configuration
  * @{
  */
/**
  * @brief  Set DMAMUX request ID for DMAMUX Channel x.
  * @note   DMAMUX1 channel 0 to 7 are mapped to DMA1 channel 0 to 7.
  *         DMAMUX1 channel 8 to 15 are mapped to DMA2 channel 0 to 7.
  * @rmtoll CxCR         DMAREQ_ID     LL_DMAMUX_SetRequestID
  * @param  DMAMUXx DMAMUXx Instance
  * @param  Channel This parameter can be one of the following values:
  *         @arg @ref LL_DMAMUX_CHANNEL_0
  *         @arg @ref LL_DMAMUX_CHANNEL_1
  *         @arg @ref LL_DMAMUX_CHANNEL_2
  *         @arg @ref LL_DMAMUX_CHANNEL_3
  *         @arg @ref LL_DMAMUX_CHANNEL_4
  *         @arg @ref LL_DMAMUX_CHANNEL_5
  *         @arg @ref LL_DMAMUX_CHANNEL_6
  *         @arg @ref LL_DMAMUX_CHANNEL_7
  *         @arg @ref LL_DMAMUX_CHANNEL_8
  *         @arg @ref LL_DMAMUX_CHANNEL_9
  *         @arg @ref LL_DMAMUX_CHANNEL_10
  *         @arg @ref LL_DMAMUX_CHANNEL_11
  *         @arg @ref LL_DMAMUX_CHANNEL_12
  *         @arg @ref LL_DMAMUX_CHANNEL_13
  *         @arg @ref LL_DMAMUX_CHANNEL_14
  *         @arg @ref LL_DMAMUX_CHANNEL_15
  * @param  Request This parameter can be one of the following values:
  *         @arg @ref LL_DMAMUX1_REQ_MEM2MEM
  *         @arg @ref LL_DMAMUX1_REQ_GENERATOR0
  *         @arg @ref LL_DMAMUX1_REQ_GENERATOR1
  *         @arg @ref LL_DMAMUX1_REQ_GENERATOR2
  *         @arg @ref LL_DMAMUX1_REQ_GENERATOR3
  *         @arg @ref LL_DMAMUX1_REQ_ADC1
  *         @arg @ref LL_DMAMUX1_REQ_BDAC1_CH1
  *         @arg @ref LL_DMAMUX1_REQ_BDAC1_CH2
  *         @arg @ref LL_DMAMUX1_REQ_TIM6_UP
  *         @arg @ref LL_DMAMUX1_REQ_TIM7_UP
  *         @arg @ref LL_DMAMUX1_REQ_SPI1_RX
  *         @arg @ref LL_DMAMUX1_REQ_SPI1_TX
  *         @arg @ref LL_DMAMUX1_REQ_SPI2_RX
  *         @arg @ref LL_DMAMUX1_REQ_SPI2_TX
  *         @arg @ref LL_DMAMUX1_REQ_SPI3_RX
  *         @arg @ref LL_DMAMUX1_REQ_SPI3_TX
  *         @arg @ref LL_DMAMUX1_REQ_I2C1_RX
  *         @arg @ref LL_DMAMUX1_REQ_I2C1_TX
  *         @arg @ref LL_DMAMUX1_REQ_I2C2_RX
  *         @arg @ref LL_DMAMUX1_REQ_I2C2_TX
  *         @arg @ref LL_DMAMUX1_REQ_USART1_RX
  *         @arg @ref LL_DMAMUX1_REQ_USART1_TX
  *         @arg @ref LL_DMAMUX1_REQ_USART2_RX
  *         @arg @ref LL_DMAMUX1_REQ_USART2_TX
  *         @arg @ref LL_DMAMUX1_REQ_USART3_RX
  *         @arg @ref LL_DMAMUX1_REQ_USART3_TX
  *         @arg @ref LL_DMAMUX1_REQ_ADC2
  *         @arg @ref LL_DMAMUX1_REQ_ADC3
  *         @arg @ref LL_DMAMUX1_REQ_ADC4
  *         @arg @ref LL_DMAMUX1_REQ_ADC5
  *         @arg @ref LL_DMAMUX1_REQ_TIM1_CH1
  *         @arg @ref LL_DMAMUX1_REQ_TIM1_CH2
  *         @arg @ref LL_DMAMUX1_REQ_TIM1_CH3
  *         @arg @ref LL_DMAMUX1_REQ_TIM1_CH4
  *         @arg @ref LL_DMAMUX1_REQ_TIM1_UP
  *         @arg @ref LL_DMAMUX1_REQ_TIM1_TRIG
  *         @arg @ref LL_DMAMUX1_REQ_TIM1_COM
  *         @arg @ref LL_DMAMUX1_REQ_TIM8_CH1
  *         @arg @ref LL_DMAMUX1_REQ_TIM8_CH2
  *         @arg @ref LL_DMAMUX1_REQ_TIM8_CH3
  *         @arg @ref LL_DMAMUX1_REQ_TIM8_CH4
  *         @arg @ref LL_DMAMUX1_REQ_TIM8_UP
  *         @arg @ref LL_DMAMUX1_REQ_TIM8_TRIG
  *         @arg @ref LL_DMAMUX1_REQ_TIM8_COM
  *         @arg @ref LL_DMAMUX1_REQ_TIM2_CH1
  *         @arg @ref LL_DMAMUX1_REQ_TIM2_CH2
  *         @arg @ref LL_DMAMUX1_REQ_TIM2_CH3
  *         @arg @ref LL_DMAMUX1_REQ_TIM2_CH4
  *         @arg @ref LL_DMAMUX1_REQ_TIM2_UP
  *         @arg @ref LL_DMAMUX1_REQ_TIM3_CH1
  *         @arg @ref LL_DMAMUX1_REQ_TIM3_CH2
  *         @arg @ref LL_DMAMUX1_REQ_TIM3_CH3
  *         @arg @ref LL_DMAMUX1_REQ_TIM3_CH4
  *         @arg @ref LL_DMAMUX1_REQ_TIM3_UP
  *         @arg @ref LL_DMAMUX1_REQ_TIM3_TRIG
  *         @arg @ref LL_DMAMUX1_REQ_TIM4_CH1
  *         @arg @ref LL_DMAMUX1_REQ_TIM4_CH2
  *         @arg @ref LL_DMAMUX1_REQ_TIM4_CH3
  *         @arg @ref LL_DMAMUX1_REQ_TIM4_CH4
  *         @arg @ref LL_DMAMUX1_REQ_TIM4_UP
  *         @arg @ref LL_DMAMUX1_REQ_TIM5_CH1
  *         @arg @ref LL_DMAMUX1_REQ_TIM5_CH2
  *         @arg @ref LL_DMAMUX1_REQ_TIM5_CH3
  *         @arg @ref LL_DMAMUX1_REQ_TIM5_CH4
  *         @arg @ref LL_DMAMUX1_REQ_TIM5_UP
  *         @arg @ref LL_DMAMUX1_REQ_TIM5_TRIG
  *         @arg @ref LL_DMAMUX1_REQ_TIM15_CH1
  *         @arg @ref LL_DMAMUX1_REQ_TIM15_UP
  *         @arg @ref LL_DMAMUX1_REQ_TIM15_TRIG
  *         @arg @ref LL_DMAMUX1_REQ_TIM15_COM
  *         @arg @ref LL_DMAMUX1_REQ_TIM16_CH1
  *         @arg @ref LL_DMAMUX1_REQ_TIM16_UP
  *         @arg @ref LL_DMAMUX1_REQ_TIM_TS_UP
  *         @arg @ref LL_DMAMUX1_REQ_DAC1_CH1
  *         @arg @ref LL_DMAMUX1_REQ_DAC1_CH2
  *         @arg @ref LL_DMAMUX1_REQ_DAC2_CH1
  *         @arg @ref LL_DMAMUX1_REQ_DAC2_CH1
  *         @arg @ref LL_DMAMUX1_REQ_HRTIM1_MASTER
  *         @arg @ref LL_DMAMUX1_REQ_HRTIM1_TIMER_A
  *         @arg @ref LL_DMAMUX1_REQ_HRTIM1_TIMER_B
  *         @arg @ref LL_DMAMUX1_REQ_HRTIM1_TIMER_C
  *         @arg @ref LL_DMAMUX1_REQ_HRTIM1_TIMER_D
  *         @arg @ref LL_DMAMUX1_REQ_HRTIM1_TIMER_E
  *         @arg @ref LL_DMAMUX1_REQ_HRTIM1_TIMER_F
  *         @arg @ref LL_DMAMUX1_REQ_DAC3_CH1
  *         @arg @ref LL_DMAMUX1_REQ_DAC3_CH2
  *         @arg @ref LL_DMAMUX1_REQ_DAC4_CH1
  *         @arg @ref LL_DMAMUX1_REQ_DAC4_CH2
  *         @arg @ref LL_DMAMUX1_REQ_SPI4_RX
  *         @arg @ref LL_DMAMUX1_REQ_SPI4_TX
  *         @arg @ref LL_DMAMUX1_REQ_CORDIC_READ
  *         @arg @ref LL_DMAMUX1_REQ_CORDIC_WRITE
  *         @arg @ref LL_DMAMUX1_REQ_HRTIM2_MASTER
  *         @arg @ref LL_DMAMUX1_REQ_HRTIM2_TIMER_A
  *         @arg @ref LL_DMAMUX1_REQ_HRTIM2_TIMER_B
  *         @arg @ref LL_DMAMUX1_REQ_HRTIM2_TIMER_C
  *         @arg @ref LL_DMAMUX1_REQ_HRTIM2_TIMER_D
  *         @arg @ref LL_DMAMUX1_REQ_HRTIM2_TIMER_E
  *         @arg @ref LL_DMAMUX1_REQ_HRTIM2_TIMER_F
  *         @arg @ref LL_DMAMUX1_REQ_CAN_SUB_1_DMU_1_TX
  *         @arg @ref LL_DMAMUX1_REQ_CAN_SUB_1_DMU_1_TXE
  *         @arg @ref LL_DMAMUX1_REQ_CAN_SUB_1_DMU_1_RX0
  *         @arg @ref LL_DMAMUX1_REQ_CAN_SUB_1_DMU_1_RX1
  *         @arg @ref LL_DMAMUX1_REQ_CAN_SUB_1_DMU_2_TX
  *         @arg @ref LL_DMAMUX1_REQ_CAN_SUB_1_DMU_2_TXE
  *         @arg @ref LL_DMAMUX1_REQ_CAN_SUB_1_DMU_2_RX0
  *         @arg @ref LL_DMAMUX1_REQ_CAN_SUB_1_DMU_2_RX1
  *         @arg @ref LL_DMAMUX1_REQ_CAN_SUB_1_DMU_3_TX
  *         @arg @ref LL_DMAMUX1_REQ_CAN_SUB_1_DMU_3_TXE
  *         @arg @ref LL_DMAMUX1_REQ_CAN_SUB_1_DMU_3_RX0
  *         @arg @ref LL_DMAMUX1_REQ_CAN_SUB_1_DMU_3_RX1
  *         @arg @ref LL_DMAMUX1_REQ_CAN_SUB_1_DMU_4_TX
  *         @arg @ref LL_DMAMUX1_REQ_CAN_SUB_1_DMU_4_TXE
  *         @arg @ref LL_DMAMUX1_REQ_CAN_SUB_1_DMU_4_RX0
  *         @arg @ref LL_DMAMUX1_REQ_CAN_SUB_1_DMU_4_RX1
  *         @arg @ref LL_DMAMUX1_REQ_CAN_SUB_1_M_CAN_1
  *         @arg @ref LL_DMAMUX1_REQ_CAN_SUB_1_M_CAN_2
  *         @arg @ref LL_DMAMUX1_REQ_SD_ADC1
  *         @arg @ref LL_DMAMUX1_REQ_SD_ADC2
  *
  * @retval None
  */
__STATIC_INLINE void LL_DMAMUX_SetRequestID(DMAMUX_Channel_TypeDef *DMAMUXx, uint32_t Channel, uint32_t Request)
{
  uint32_t dmamux_base_addr = (uint32_t)DMAMUXx;

  MODIFY_REG(((DMAMUX_Channel_TypeDef *)(dmamux_base_addr + (DMAMUX_CCR_SIZE * (Channel))))->CCR, DMAMUX_CxCR_DMAREQ_ID, Request);
}

/**
  * @brief  Get DMAMUX request ID for DMAMUX Channel x.
  * @note   DMAMUX1 channel 0 to 7  are mapped to DMA1 channel 0 to 7.
  *         DMAMUX1 channel 8 to 15 are mapped to DMA2 channel 0 to 7.
  * @rmtoll CxCR         DMAREQ_ID     LL_DMAMUX_GetRequestID
  * @param  DMAMUXx DMAMUXx Instance
  * @param  Channel This parameter can be one of the following values:
  *         @arg @ref LL_DMAMUX_CHANNEL_0
  *         @arg @ref LL_DMAMUX_CHANNEL_1
  *         @arg @ref LL_DMAMUX_CHANNEL_2
  *         @arg @ref LL_DMAMUX_CHANNEL_3
  *         @arg @ref LL_DMAMUX_CHANNEL_4
  *         @arg @ref LL_DMAMUX_CHANNEL_5
  *         @arg @ref LL_DMAMUX_CHANNEL_6
  *         @arg @ref LL_DMAMUX_CHANNEL_7
  *         @arg @ref LL_DMAMUX_CHANNEL_8
  *         @arg @ref LL_DMAMUX_CHANNEL_9
  *         @arg @ref LL_DMAMUX_CHANNEL_10
  *         @arg @ref LL_DMAMUX_CHANNEL_11
  *         @arg @ref LL_DMAMUX_CHANNEL_12
  *         @arg @ref LL_DMAMUX_CHANNEL_13
  *         @arg @ref LL_DMAMUX_CHANNEL_14
  *         @arg @ref LL_DMAMUX_CHANNEL_15
  * @retval Returned value can be one of the following values:
  *         @arg @ref LL_DMAMUX1_REQ_MEM2MEM
  *         @arg @ref LL_DMAMUX1_REQ_GENERATOR0
  *         @arg @ref LL_DMAMUX1_REQ_GENERATOR1
  *         @arg @ref LL_DMAMUX1_REQ_GENERATOR2
  *         @arg @ref LL_DMAMUX1_REQ_GENERATOR3
  *         @arg @ref LL_DMAMUX1_REQ_ADC1
  *         @arg @ref LL_DMAMUX1_REQ_BDAC1_CH1
  *         @arg @ref LL_DMAMUX1_REQ_BDAC1_CH2
  *         @arg @ref LL_DMAMUX1_REQ_TIM6_UP
  *         @arg @ref LL_DMAMUX1_REQ_TIM7_UP
  *         @arg @ref LL_DMAMUX1_REQ_SPI1_RX
  *         @arg @ref LL_DMAMUX1_REQ_SPI1_TX
  *         @arg @ref LL_DMAMUX1_REQ_SPI2_RX
  *         @arg @ref LL_DMAMUX1_REQ_SPI2_TX
  *         @arg @ref LL_DMAMUX1_REQ_SPI3_RX
  *         @arg @ref LL_DMAMUX1_REQ_SPI3_TX
  *         @arg @ref LL_DMAMUX1_REQ_I2C1_RX
  *         @arg @ref LL_DMAMUX1_REQ_I2C1_TX
  *         @arg @ref LL_DMAMUX1_REQ_I2C2_RX
  *         @arg @ref LL_DMAMUX1_REQ_I2C2_TX
  *         @arg @ref LL_DMAMUX1_REQ_USART1_RX
  *         @arg @ref LL_DMAMUX1_REQ_USART1_TX
  *         @arg @ref LL_DMAMUX1_REQ_USART2_RX
  *         @arg @ref LL_DMAMUX1_REQ_USART2_TX
  *         @arg @ref LL_DMAMUX1_REQ_USART3_RX
  *         @arg @ref LL_DMAMUX1_REQ_USART3_TX
  *         @arg @ref LL_DMAMUX1_REQ_ADC2
  *         @arg @ref LL_DMAMUX1_REQ_ADC3
  *         @arg @ref LL_DMAMUX1_REQ_ADC4
  *         @arg @ref LL_DMAMUX1_REQ_ADC5
  *         @arg @ref LL_DMAMUX1_REQ_TIM1_CH1
  *         @arg @ref LL_DMAMUX1_REQ_TIM1_CH2
  *         @arg @ref LL_DMAMUX1_REQ_TIM1_CH3
  *         @arg @ref LL_DMAMUX1_REQ_TIM1_CH4
  *         @arg @ref LL_DMAMUX1_REQ_TIM1_UP
  *         @arg @ref LL_DMAMUX1_REQ_TIM1_TRIG
  *         @arg @ref LL_DMAMUX1_REQ_TIM1_COM
  *         @arg @ref LL_DMAMUX1_REQ_TIM8_CH1
  *         @arg @ref LL_DMAMUX1_REQ_TIM8_CH2
  *         @arg @ref LL_DMAMUX1_REQ_TIM8_CH3
  *         @arg @ref LL_DMAMUX1_REQ_TIM8_CH4
  *         @arg @ref LL_DMAMUX1_REQ_TIM8_UP
  *         @arg @ref LL_DMAMUX1_REQ_TIM8_TRIG
  *         @arg @ref LL_DMAMUX1_REQ_TIM8_COM
  *         @arg @ref LL_DMAMUX1_REQ_TIM2_CH1
  *         @arg @ref LL_DMAMUX1_REQ_TIM2_CH2
  *         @arg @ref LL_DMAMUX1_REQ_TIM2_CH3
  *         @arg @ref LL_DMAMUX1_REQ_TIM2_CH4
  *         @arg @ref LL_DMAMUX1_REQ_TIM2_UP
  *         @arg @ref LL_DMAMUX1_REQ_TIM3_CH1
  *         @arg @ref LL_DMAMUX1_REQ_TIM3_CH2
  *         @arg @ref LL_DMAMUX1_REQ_TIM3_CH3
  *         @arg @ref LL_DMAMUX1_REQ_TIM3_CH4
  *         @arg @ref LL_DMAMUX1_REQ_TIM3_UP
  *         @arg @ref LL_DMAMUX1_REQ_TIM3_TRIG
  *         @arg @ref LL_DMAMUX1_REQ_TIM4_CH1
  *         @arg @ref LL_DMAMUX1_REQ_TIM4_CH2
  *         @arg @ref LL_DMAMUX1_REQ_TIM4_CH3
  *         @arg @ref LL_DMAMUX1_REQ_TIM4_CH4
  *         @arg @ref LL_DMAMUX1_REQ_TIM4_UP
  *         @arg @ref LL_DMAMUX1_REQ_TIM5_CH1
  *         @arg @ref LL_DMAMUX1_REQ_TIM5_CH2
  *         @arg @ref LL_DMAMUX1_REQ_TIM5_CH3
  *         @arg @ref LL_DMAMUX1_REQ_TIM5_CH4
  *         @arg @ref LL_DMAMUX1_REQ_TIM5_UP
  *         @arg @ref LL_DMAMUX1_REQ_TIM5_TRIG
  *         @arg @ref LL_DMAMUX1_REQ_TIM15_CH1
  *         @arg @ref LL_DMAMUX1_REQ_TIM15_UP
  *         @arg @ref LL_DMAMUX1_REQ_TIM15_TRIG
  *         @arg @ref LL_DMAMUX1_REQ_TIM15_COM
  *         @arg @ref LL_DMAMUX1_REQ_TIM16_CH1
  *         @arg @ref LL_DMAMUX1_REQ_TIM16_UP
  *         @arg @ref LL_DMAMUX1_REQ_TIM_TS_UP
  *         @arg @ref LL_DMAMUX1_REQ_DAC1_CH1
  *         @arg @ref LL_DMAMUX1_REQ_DAC1_CH2
  *         @arg @ref LL_DMAMUX1_REQ_DAC2_CH1
  *         @arg @ref LL_DMAMUX1_REQ_DAC2_CH1
  *         @arg @ref LL_DMAMUX1_REQ_HRTIM1_MASTER
  *         @arg @ref LL_DMAMUX1_REQ_HRTIM1_TIMER_A
  *         @arg @ref LL_DMAMUX1_REQ_HRTIM1_TIMER_B
  *         @arg @ref LL_DMAMUX1_REQ_HRTIM1_TIMER_C
  *         @arg @ref LL_DMAMUX1_REQ_HRTIM1_TIMER_D
  *         @arg @ref LL_DMAMUX1_REQ_HRTIM1_TIMER_E
  *         @arg @ref LL_DMAMUX1_REQ_HRTIM1_TIMER_F
  *         @arg @ref LL_DMAMUX1_REQ_DAC3_CH1
  *         @arg @ref LL_DMAMUX1_REQ_DAC3_CH2
  *         @arg @ref LL_DMAMUX1_REQ_DAC4_CH1
  *         @arg @ref LL_DMAMUX1_REQ_DAC4_CH2
  *         @arg @ref LL_DMAMUX1_REQ_SPI4_RX
  *         @arg @ref LL_DMAMUX1_REQ_SPI4_TX
  *         @arg @ref LL_DMAMUX1_REQ_CORDIC_READ
  *         @arg @ref LL_DMAMUX1_REQ_CORDIC_WRITE
  *         @arg @ref LL_DMAMUX1_REQ_HRTIM2_MASTER
  *         @arg @ref LL_DMAMUX1_REQ_HRTIM2_TIMER_A
  *         @arg @ref LL_DMAMUX1_REQ_HRTIM2_TIMER_B
  *         @arg @ref LL_DMAMUX1_REQ_HRTIM2_TIMER_C
  *         @arg @ref LL_DMAMUX1_REQ_HRTIM2_TIMER_D
  *         @arg @ref LL_DMAMUX1_REQ_HRTIM2_TIMER_E
  *         @arg @ref LL_DMAMUX1_REQ_HRTIM2_TIMER_F
  *         @arg @ref LL_DMAMUX1_REQ_CAN_SUB_1_DMU_1_TX
  *         @arg @ref LL_DMAMUX1_REQ_CAN_SUB_1_DMU_1_TXE
  *         @arg @ref LL_DMAMUX1_REQ_CAN_SUB_1_DMU_1_RX0
  *         @arg @ref LL_DMAMUX1_REQ_CAN_SUB_1_DMU_1_RX1
  *         @arg @ref LL_DMAMUX1_REQ_CAN_SUB_1_DMU_2_TX
  *         @arg @ref LL_DMAMUX1_REQ_CAN_SUB_1_DMU_2_TXE
  *         @arg @ref LL_DMAMUX1_REQ_CAN_SUB_1_DMU_2_RX0
  *         @arg @ref LL_DMAMUX1_REQ_CAN_SUB_1_DMU_2_RX1
  *         @arg @ref LL_DMAMUX1_REQ_CAN_SUB_1_DMU_3_TX
  *         @arg @ref LL_DMAMUX1_REQ_CAN_SUB_1_DMU_3_TXE
  *         @arg @ref LL_DMAMUX1_REQ_CAN_SUB_1_DMU_3_RX0
  *         @arg @ref LL_DMAMUX1_REQ_CAN_SUB_1_DMU_3_RX1
  *         @arg @ref LL_DMAMUX1_REQ_CAN_SUB_1_DMU_4_TX
  *         @arg @ref LL_DMAMUX1_REQ_CAN_SUB_1_DMU_4_TXE
  *         @arg @ref LL_DMAMUX1_REQ_CAN_SUB_1_DMU_4_RX0
  *         @arg @ref LL_DMAMUX1_REQ_CAN_SUB_1_DMU_4_RX1
  *         @arg @ref LL_DMAMUX1_REQ_CAN_SUB_1_M_CAN_1
  *         @arg @ref LL_DMAMUX1_REQ_CAN_SUB_1_M_CAN_2
  *         @arg @ref LL_DMAMUX1_REQ_SD_ADC1
  *         @arg @ref LL_DMAMUX1_REQ_SD_ADC2
  *
  * @retval None
  */
__STATIC_INLINE uint32_t LL_DMAMUX_GetRequestID(DMAMUX_Channel_TypeDef *DMAMUXx, uint32_t Channel)
{
  uint32_t dmamux_base_addr = (uint32_t)DMAMUXx;

  return (uint32_t)(READ_BIT(((DMAMUX_Channel_TypeDef *)(dmamux_base_addr + (DMAMUX_CCR_SIZE * (Channel))))->CCR, DMAMUX_CxCR_DMAREQ_ID));
}

/**
  * @brief  Set the number of DMA request that will be autorized after a synchronization event and/or the number of DMA request needed to generate an event.
  * @rmtoll CxCR         NBREQ         LL_DMAMUX_SetSyncRequestNb
  * @param  DMAMUXx DMAMUXx Instance
  * @param  Channel This parameter can be one of the following values:
  *         @arg @ref LL_DMAMUX_CHANNEL_0
  *         @arg @ref LL_DMAMUX_CHANNEL_1
  *         @arg @ref LL_DMAMUX_CHANNEL_2
  *         @arg @ref LL_DMAMUX_CHANNEL_3
  *         @arg @ref LL_DMAMUX_CHANNEL_4
  *         @arg @ref LL_DMAMUX_CHANNEL_5
  *         @arg @ref LL_DMAMUX_CHANNEL_6
  *         @arg @ref LL_DMAMUX_CHANNEL_7
  *         @arg @ref LL_DMAMUX_CHANNEL_8
  *         @arg @ref LL_DMAMUX_CHANNEL_9
  *         @arg @ref LL_DMAMUX_CHANNEL_10
  *         @arg @ref LL_DMAMUX_CHANNEL_11
  *         @arg @ref LL_DMAMUX_CHANNEL_12
  *         @arg @ref LL_DMAMUX_CHANNEL_13
  *         @arg @ref LL_DMAMUX_CHANNEL_14
  *         @arg @ref LL_DMAMUX_CHANNEL_15
  * @param  RequestNb This parameter must be a value between Min_Data = 1 and Max_Data = 32.
  * @retval None
  */
__STATIC_INLINE void LL_DMAMUX_SetSyncRequestNb(DMAMUX_Channel_TypeDef *DMAMUXx, uint32_t Channel, uint32_t RequestNb)
{
  uint32_t dmamux_base_addr = (uint32_t)DMAMUXx;

  MODIFY_REG(((DMAMUX_Channel_TypeDef *)(dmamux_base_addr + (DMAMUX_CCR_SIZE * (Channel))))->CCR, DMAMUX_CxCR_NBREQ, (RequestNb - 1U) << DMAMUX_CxCR_NBREQ_Pos);
}

/**
  * @brief  Get the number of DMA request that will be autorized after a synchronization event and/or the number of DMA request needed to generate an event.
  * @rmtoll CxCR         NBREQ         LL_DMAMUX_GetSyncRequestNb
  * @param  DMAMUXx DMAMUXx Instance
  * @param  Channel This parameter can be one of the following values:
  *         @arg @ref LL_DMAMUX_CHANNEL_0
  *         @arg @ref LL_DMAMUX_CHANNEL_1
  *         @arg @ref LL_DMAMUX_CHANNEL_2
  *         @arg @ref LL_DMAMUX_CHANNEL_3
  *         @arg @ref LL_DMAMUX_CHANNEL_4
  *         @arg @ref LL_DMAMUX_CHANNEL_5
  *         @arg @ref LL_DMAMUX_CHANNEL_6
  *         @arg @ref LL_DMAMUX_CHANNEL_7
  *         @arg @ref LL_DMAMUX_CHANNEL_8
  *         @arg @ref LL_DMAMUX_CHANNEL_9
  *         @arg @ref LL_DMAMUX_CHANNEL_10
  *         @arg @ref LL_DMAMUX_CHANNEL_11
  *         @arg @ref LL_DMAMUX_CHANNEL_12
  *         @arg @ref LL_DMAMUX_CHANNEL_13
  *         @arg @ref LL_DMAMUX_CHANNEL_14
  *         @arg @ref LL_DMAMUX_CHANNEL_15
  * @retval Between Min_Data = 1 and Max_Data = 32
  */
__STATIC_INLINE uint32_t LL_DMAMUX_GetSyncRequestNb(DMAMUX_Channel_TypeDef *DMAMUXx, uint32_t Channel)
{
  uint32_t dmamux_base_addr = (uint32_t)DMAMUXx;

  return (uint32_t)((READ_BIT(((DMAMUX_Channel_TypeDef *)(dmamux_base_addr + (DMAMUX_CCR_SIZE * (Channel))))->CCR, DMAMUX_CxCR_NBREQ) >> DMAMUX_CxCR_NBREQ_Pos) + 1U);
}

/**
  * @brief  Set the polarity of the signal on which the DMA request is synchronized.
  * @rmtoll CxCR         SPOL          LL_DMAMUX_SetSyncPolarity
  * @param  DMAMUXx DMAMUXx Instance
  * @param  Channel This parameter can be one of the following values:
  *         @arg @ref LL_DMAMUX_CHANNEL_0
  *         @arg @ref LL_DMAMUX_CHANNEL_1
  *         @arg @ref LL_DMAMUX_CHANNEL_2
  *         @arg @ref LL_DMAMUX_CHANNEL_3
  *         @arg @ref LL_DMAMUX_CHANNEL_4
  *         @arg @ref LL_DMAMUX_CHANNEL_5
  *         @arg @ref LL_DMAMUX_CHANNEL_6
  *         @arg @ref LL_DMAMUX_CHANNEL_7
  *         @arg @ref LL_DMAMUX_CHANNEL_8
  *         @arg @ref LL_DMAMUX_CHANNEL_9
  *         @arg @ref LL_DMAMUX_CHANNEL_10
  *         @arg @ref LL_DMAMUX_CHANNEL_11
  *         @arg @ref LL_DMAMUX_CHANNEL_12
  *         @arg @ref LL_DMAMUX_CHANNEL_13
  *         @arg @ref LL_DMAMUX_CHANNEL_14
  *         @arg @ref LL_DMAMUX_CHANNEL_15
  * @param  Polarity This parameter can be one of the following values:
  *         @arg @ref LL_DMAMUX_SYNC_NO_EVENT
  *         @arg @ref LL_DMAMUX_SYNC_POL_RISING
  *         @arg @ref LL_DMAMUX_SYNC_POL_FALLING
  *         @arg @ref LL_DMAMUX_SYNC_POL_RISING_FALLING
  * @retval None
  */
__STATIC_INLINE void LL_DMAMUX_SetSyncPolarity(DMAMUX_Channel_TypeDef *DMAMUXx, uint32_t Channel, uint32_t Polarity)
{
  uint32_t dmamux_base_addr = (uint32_t)DMAMUXx;

  MODIFY_REG(((DMAMUX_Channel_TypeDef *)(dmamux_base_addr + (DMAMUX_CCR_SIZE * (Channel))))->CCR, DMAMUX_CxCR_SPOL, Polarity);
}

/**
  * @brief  Get the polarity of the signal on which the DMA request is synchronized.
  * @rmtoll CxCR         SPOL          LL_DMAMUX_GetSyncPolarity
  * @param  DMAMUXx DMAMUXx Instance
  * @param  Channel This parameter can be one of the following values:
  *         @arg @ref LL_DMAMUX_CHANNEL_0
  *         @arg @ref LL_DMAMUX_CHANNEL_1
  *         @arg @ref LL_DMAMUX_CHANNEL_2
  *         @arg @ref LL_DMAMUX_CHANNEL_3
  *         @arg @ref LL_DMAMUX_CHANNEL_4
  *         @arg @ref LL_DMAMUX_CHANNEL_5
  *         @arg @ref LL_DMAMUX_CHANNEL_6
  *         @arg @ref LL_DMAMUX_CHANNEL_7
  *         @arg @ref LL_DMAMUX_CHANNEL_8
  *         @arg @ref LL_DMAMUX_CHANNEL_9
  *         @arg @ref LL_DMAMUX_CHANNEL_10
  *         @arg @ref LL_DMAMUX_CHANNEL_11
  *         @arg @ref LL_DMAMUX_CHANNEL_12
  *         @arg @ref LL_DMAMUX_CHANNEL_13
  *         @arg @ref LL_DMAMUX_CHANNEL_14
  *         @arg @ref LL_DMAMUX_CHANNEL_15
  * @retval Returned value can be one of the following values:
  *         @arg @ref LL_DMAMUX_SYNC_NO_EVENT
  *         @arg @ref LL_DMAMUX_SYNC_POL_RISING
  *         @arg @ref LL_DMAMUX_SYNC_POL_FALLING
  *         @arg @ref LL_DMAMUX_SYNC_POL_RISING_FALLING
  */
__STATIC_INLINE uint32_t LL_DMAMUX_GetSyncPolarity(DMAMUX_Channel_TypeDef *DMAMUXx, uint32_t Channel)
{
  uint32_t dmamux_base_addr = (uint32_t)DMAMUXx;

  return (uint32_t)(READ_BIT(((DMAMUX_Channel_TypeDef *)(dmamux_base_addr + (DMAMUX_CCR_SIZE * (Channel))))->CCR, DMAMUX_CxCR_SPOL));
}

/**
  * @brief  Enable the Event Generation on DMAMUX channel x.
  * @rmtoll CxCR         EGE           LL_DMAMUX_EnableEventGeneration
  * @param  DMAMUXx DMAMUXx Instance
  * @param  Channel This parameter can be one of the following values:
  *         @arg @ref LL_DMAMUX_CHANNEL_0
  *         @arg @ref LL_DMAMUX_CHANNEL_1
  *         @arg @ref LL_DMAMUX_CHANNEL_2
  *         @arg @ref LL_DMAMUX_CHANNEL_3
  *         @arg @ref LL_DMAMUX_CHANNEL_4
  *         @arg @ref LL_DMAMUX_CHANNEL_5
  *         @arg @ref LL_DMAMUX_CHANNEL_6
  *         @arg @ref LL_DMAMUX_CHANNEL_7
  *         @arg @ref LL_DMAMUX_CHANNEL_8
  *         @arg @ref LL_DMAMUX_CHANNEL_9
  *         @arg @ref LL_DMAMUX_CHANNEL_10
  *         @arg @ref LL_DMAMUX_CHANNEL_11
  *         @arg @ref LL_DMAMUX_CHANNEL_12
  *         @arg @ref LL_DMAMUX_CHANNEL_13
  *         @arg @ref LL_DMAMUX_CHANNEL_14
  *         @arg @ref LL_DMAMUX_CHANNEL_15
  * @retval None
  */
__STATIC_INLINE void LL_DMAMUX_EnableEventGeneration(DMAMUX_Channel_TypeDef *DMAMUXx, uint32_t Channel)
{
  uint32_t dmamux_base_addr = (uint32_t)DMAMUXx;

  SET_BIT(((DMAMUX_Channel_TypeDef *)(dmamux_base_addr + (DMAMUX_CCR_SIZE * (Channel))))->CCR, DMAMUX_CxCR_EGE);
}

/**
  * @brief  Disable the Event Generation on DMAMUX channel x.
  * @rmtoll CxCR         EGE           LL_DMAMUX_DisableEventGeneration
  * @param  DMAMUXx DMAMUXx Instance
  * @param  Channel This parameter can be one of the following values:
  *         @arg @ref LL_DMAMUX_CHANNEL_0
  *         @arg @ref LL_DMAMUX_CHANNEL_1
  *         @arg @ref LL_DMAMUX_CHANNEL_2
  *         @arg @ref LL_DMAMUX_CHANNEL_3
  *         @arg @ref LL_DMAMUX_CHANNEL_4
  *         @arg @ref LL_DMAMUX_CHANNEL_5
  *         @arg @ref LL_DMAMUX_CHANNEL_6
  *         @arg @ref LL_DMAMUX_CHANNEL_7
  *         @arg @ref LL_DMAMUX_CHANNEL_8
  *         @arg @ref LL_DMAMUX_CHANNEL_9
  *         @arg @ref LL_DMAMUX_CHANNEL_10
  *         @arg @ref LL_DMAMUX_CHANNEL_11
  *         @arg @ref LL_DMAMUX_CHANNEL_12
  *         @arg @ref LL_DMAMUX_CHANNEL_13
  *         @arg @ref LL_DMAMUX_CHANNEL_14
  *         @arg @ref LL_DMAMUX_CHANNEL_15
  * @retval None
  */
__STATIC_INLINE void LL_DMAMUX_DisableEventGeneration(DMAMUX_Channel_TypeDef *DMAMUXx, uint32_t Channel)
{
  uint32_t dmamux_base_addr = (uint32_t)DMAMUXx;

  CLEAR_BIT(((DMAMUX_Channel_TypeDef *)(dmamux_base_addr + (DMAMUX_CCR_SIZE * (Channel))))->CCR, DMAMUX_CxCR_EGE);
}

/**
  * @brief  Check if the Event Generation on DMAMUX channel x is enabled or disabled.
  * @rmtoll CxCR         EGE           LL_DMAMUX_IsEnabledEventGeneration
  * @param  DMAMUXx DMAMUXx Instance
  * @param  Channel This parameter can be one of the following values:
  *         @arg @ref LL_DMAMUX_CHANNEL_0
  *         @arg @ref LL_DMAMUX_CHANNEL_1
  *         @arg @ref LL_DMAMUX_CHANNEL_2
  *         @arg @ref LL_DMAMUX_CHANNEL_3
  *         @arg @ref LL_DMAMUX_CHANNEL_4
  *         @arg @ref LL_DMAMUX_CHANNEL_5
  *         @arg @ref LL_DMAMUX_CHANNEL_6
  *         @arg @ref LL_DMAMUX_CHANNEL_7
  *         @arg @ref LL_DMAMUX_CHANNEL_8
  *         @arg @ref LL_DMAMUX_CHANNEL_9
  *         @arg @ref LL_DMAMUX_CHANNEL_10
  *         @arg @ref LL_DMAMUX_CHANNEL_11
  *         @arg @ref LL_DMAMUX_CHANNEL_12
  *         @arg @ref LL_DMAMUX_CHANNEL_13
  *         @arg @ref LL_DMAMUX_CHANNEL_14
  *         @arg @ref LL_DMAMUX_CHANNEL_15
  * @retval State of bit (1 or 0).
  */
__STATIC_INLINE uint32_t LL_DMAMUX_IsEnabledEventGeneration(DMAMUX_Channel_TypeDef *DMAMUXx, uint32_t Channel)
{
  uint32_t dmamux_base_addr = (uint32_t)DMAMUXx;

  return ((READ_BIT(((DMAMUX_Channel_TypeDef *)(dmamux_base_addr + (DMAMUX_CCR_SIZE * (Channel))))->CCR, DMAMUX_CxCR_EGE) == (DMAMUX_CxCR_EGE)) ? 1UL : 0UL);
}

/**
  * @brief  Enable the synchronization mode.
  * @rmtoll CxCR         SE            LL_DMAMUX_EnableSync
  * @param  DMAMUXx DMAMUXx Instance
  * @param  Channel This parameter can be one of the following values:
  *         @arg @ref LL_DMAMUX_CHANNEL_0
  *         @arg @ref LL_DMAMUX_CHANNEL_1
  *         @arg @ref LL_DMAMUX_CHANNEL_2
  *         @arg @ref LL_DMAMUX_CHANNEL_3
  *         @arg @ref LL_DMAMUX_CHANNEL_4
  *         @arg @ref LL_DMAMUX_CHANNEL_5
  *         @arg @ref LL_DMAMUX_CHANNEL_6
  *         @arg @ref LL_DMAMUX_CHANNEL_7
  *         @arg @ref LL_DMAMUX_CHANNEL_8
  *         @arg @ref LL_DMAMUX_CHANNEL_9
  *         @arg @ref LL_DMAMUX_CHANNEL_10
  *         @arg @ref LL_DMAMUX_CHANNEL_11
  *         @arg @ref LL_DMAMUX_CHANNEL_12
  *         @arg @ref LL_DMAMUX_CHANNEL_13
  *         @arg @ref LL_DMAMUX_CHANNEL_14
  *         @arg @ref LL_DMAMUX_CHANNEL_15
  * @retval None
  */
__STATIC_INLINE void LL_DMAMUX_EnableSync(DMAMUX_Channel_TypeDef *DMAMUXx, uint32_t Channel)
{
  uint32_t dmamux_base_addr = (uint32_t)DMAMUXx;

  SET_BIT(((DMAMUX_Channel_TypeDef *)(dmamux_base_addr + (DMAMUX_CCR_SIZE * (Channel))))->CCR, DMAMUX_CxCR_SE);
}

/**
  * @brief  Disable the synchronization mode.
  * @rmtoll CxCR         SE            LL_DMAMUX_DisableSync
  * @param  DMAMUXx DMAMUXx Instance
  * @param  Channel This parameter can be one of the following values:
  *         @arg @ref LL_DMAMUX_CHANNEL_0
  *         @arg @ref LL_DMAMUX_CHANNEL_1
  *         @arg @ref LL_DMAMUX_CHANNEL_2
  *         @arg @ref LL_DMAMUX_CHANNEL_3
  *         @arg @ref LL_DMAMUX_CHANNEL_4
  *         @arg @ref LL_DMAMUX_CHANNEL_5
  *         @arg @ref LL_DMAMUX_CHANNEL_6
  *         @arg @ref LL_DMAMUX_CHANNEL_7
  *         @arg @ref LL_DMAMUX_CHANNEL_8
  *         @arg @ref LL_DMAMUX_CHANNEL_9
  *         @arg @ref LL_DMAMUX_CHANNEL_10
  *         @arg @ref LL_DMAMUX_CHANNEL_11
  *         @arg @ref LL_DMAMUX_CHANNEL_12
  *         @arg @ref LL_DMAMUX_CHANNEL_13
  *         @arg @ref LL_DMAMUX_CHANNEL_14
  *         @arg @ref LL_DMAMUX_CHANNEL_15
  * @retval None
  */
__STATIC_INLINE void LL_DMAMUX_DisableSync(DMAMUX_Channel_TypeDef *DMAMUXx, uint32_t Channel)
{
  uint32_t dmamux_base_addr = (uint32_t)DMAMUXx;

  CLEAR_BIT(((DMAMUX_Channel_TypeDef *)(dmamux_base_addr + (DMAMUX_CCR_SIZE * (Channel))))->CCR, DMAMUX_CxCR_SE);
}

/**
  * @brief  Check if the synchronization mode is enabled or disabled.
  * @rmtoll CxCR         SE            LL_DMAMUX_IsEnabledSync
  * @param  DMAMUXx DMAMUXx Instance
  * @param  Channel This parameter can be one of the following values:
  *         @arg @ref LL_DMAMUX_CHANNEL_0
  *         @arg @ref LL_DMAMUX_CHANNEL_1
  *         @arg @ref LL_DMAMUX_CHANNEL_2
  *         @arg @ref LL_DMAMUX_CHANNEL_3
  *         @arg @ref LL_DMAMUX_CHANNEL_4
  *         @arg @ref LL_DMAMUX_CHANNEL_5
  *         @arg @ref LL_DMAMUX_CHANNEL_6
  *         @arg @ref LL_DMAMUX_CHANNEL_7
  *         @arg @ref LL_DMAMUX_CHANNEL_8
  *         @arg @ref LL_DMAMUX_CHANNEL_9
  *         @arg @ref LL_DMAMUX_CHANNEL_10
  *         @arg @ref LL_DMAMUX_CHANNEL_11
  *         @arg @ref LL_DMAMUX_CHANNEL_12
  *         @arg @ref LL_DMAMUX_CHANNEL_13
  *         @arg @ref LL_DMAMUX_CHANNEL_14
  *         @arg @ref LL_DMAMUX_CHANNEL_15
  * @retval State of bit (1 or 0).
  */
__STATIC_INLINE uint32_t LL_DMAMUX_IsEnabledSync(DMAMUX_Channel_TypeDef *DMAMUXx, uint32_t Channel)
{
  uint32_t dmamux_base_addr = (uint32_t)DMAMUXx;

  return ((READ_BIT(((DMAMUX_Channel_TypeDef *)(dmamux_base_addr + (DMAMUX_CCR_SIZE * (Channel))))->CCR, DMAMUX_CxCR_SE) == (DMAMUX_CxCR_SE)) ? 1UL : 0UL);
}

/**
  * @brief  Set DMAMUX synchronization ID  on DMAMUX Channel x.
  * @rmtoll CxCR         SYNC_ID       LL_DMAMUX_SetSyncID
  * @param  DMAMUXx DMAMUXx Instance
  * @param  Channel This parameter can be one of the following values:
  *         @arg @ref LL_DMAMUX_CHANNEL_0
  *         @arg @ref LL_DMAMUX_CHANNEL_1
  *         @arg @ref LL_DMAMUX_CHANNEL_2
  *         @arg @ref LL_DMAMUX_CHANNEL_3
  *         @arg @ref LL_DMAMUX_CHANNEL_4
  *         @arg @ref LL_DMAMUX_CHANNEL_5
  *         @arg @ref LL_DMAMUX_CHANNEL_6
  *         @arg @ref LL_DMAMUX_CHANNEL_7
  *         @arg @ref LL_DMAMUX_CHANNEL_8
  *         @arg @ref LL_DMAMUX_CHANNEL_9
  *         @arg @ref LL_DMAMUX_CHANNEL_10
  *         @arg @ref LL_DMAMUX_CHANNEL_11
  *         @arg @ref LL_DMAMUX_CHANNEL_12
  *         @arg @ref LL_DMAMUX_CHANNEL_13
  *         @arg @ref LL_DMAMUX_CHANNEL_14
  *         @arg @ref LL_DMAMUX_CHANNEL_15
  * @param  SyncID This parameter can be one of the following values:
  *         @arg @ref LL_DMAMUX1_SYNC_DMAMUX1_CH0_EVT
  *         @arg @ref LL_DMAMUX1_SYNC_DMAMUX1_CH1_EVT
  *         @arg @ref LL_DMAMUX1_SYNC_DMAMUX1_CH2_EVT
  *         @arg @ref LL_DMAMUX1_SYNC_DMAMUX1_CH3_EVT
  *
  * @retval None
  */
__STATIC_INLINE void LL_DMAMUX_SetSyncID(DMAMUX_Channel_TypeDef *DMAMUXx, uint32_t Channel, uint32_t SyncID)
{
  uint32_t dmamux_base_addr = (uint32_t)DMAMUXx;

  MODIFY_REG(((DMAMUX_Channel_TypeDef *)(dmamux_base_addr + (DMAMUX_CCR_SIZE * (Channel))))->CCR, DMAMUX_CxCR_SYNC_ID, SyncID);
}

/**
  * @brief  Get DMAMUX synchronization ID  on DMAMUX Channel x.
  * @rmtoll CxCR         SYNC_ID       LL_DMAMUX_GetSyncID
  * @param  DMAMUXx DMAMUXx Instance
  * @param  Channel This parameter can be one of the following values:
  *         @arg @ref LL_DMAMUX_CHANNEL_0
  *         @arg @ref LL_DMAMUX_CHANNEL_1
  *         @arg @ref LL_DMAMUX_CHANNEL_2
  *         @arg @ref LL_DMAMUX_CHANNEL_3
  *         @arg @ref LL_DMAMUX_CHANNEL_4
  *         @arg @ref LL_DMAMUX_CHANNEL_5
  *         @arg @ref LL_DMAMUX_CHANNEL_6
  *         @arg @ref LL_DMAMUX_CHANNEL_7
  *         @arg @ref LL_DMAMUX_CHANNEL_8
  *         @arg @ref LL_DMAMUX_CHANNEL_9
  *         @arg @ref LL_DMAMUX_CHANNEL_10
  *         @arg @ref LL_DMAMUX_CHANNEL_11
  *         @arg @ref LL_DMAMUX_CHANNEL_12
  *         @arg @ref LL_DMAMUX_CHANNEL_13
  *         @arg @ref LL_DMAMUX_CHANNEL_14
  *         @arg @ref LL_DMAMUX_CHANNEL_15
  * @retval Returned value can be one of the following values:
  *         @arg @ref LL_DMAMUX1_SYNC_DMAMUX1_CH0_EVT
  *         @arg @ref LL_DMAMUX1_SYNC_DMAMUX1_CH1_EVT
  *         @arg @ref LL_DMAMUX1_SYNC_DMAMUX1_CH2_EVT
  *         @arg @ref LL_DMAMUX1_SYNC_DMAMUX1_CH3_EVT
  */
__STATIC_INLINE uint32_t LL_DMAMUX_GetSyncID(DMAMUX_Channel_TypeDef *DMAMUXx, uint32_t Channel)
{
  uint32_t dmamux_base_addr = (uint32_t)DMAMUXx;

  return (uint32_t)(READ_BIT(((DMAMUX_Channel_TypeDef *)(dmamux_base_addr + (DMAMUX_CCR_SIZE * (Channel))))->CCR, DMAMUX_CxCR_SYNC_ID));
}

/**
  * @brief  Enable the Request Generator.
  * @rmtoll RGxCR        GE            LL_DMAMUX_EnableRequestGen
  * @param  DMAMUXx DMAMUXx Instance
  * @param  RequestGenChannel This parameter can be one of the following values:
  *         @arg @ref LL_DMAMUX_REQ_GEN_0
  *         @arg @ref LL_DMAMUX_REQ_GEN_1
  *         @arg @ref LL_DMAMUX_REQ_GEN_2
  *         @arg @ref LL_DMAMUX_REQ_GEN_3
  * @retval None
  */
__STATIC_INLINE void LL_DMAMUX_EnableRequestGen(DMAMUX_Channel_TypeDef *DMAMUXx, uint32_t RequestGenChannel)
{
  uint32_t dmamux_base_addr = (uint32_t)DMAMUXx;

  SET_BIT(((DMAMUX_RequestGen_TypeDef *)(dmamux_base_addr + DMAMUX_REQ_GEN_OFFSET + (DMAMUX_RGCR_SIZE * (RequestGenChannel))))->RGCR, DMAMUX_RGxCR_GE);
}

/**
  * @brief  Disable the Request Generator.
  * @rmtoll RGxCR        GE            LL_DMAMUX_DisableRequestGen
  * @param  DMAMUXx DMAMUXx Instance
  * @param  RequestGenChannel This parameter can be one of the following values:
  *         @arg @ref LL_DMAMUX_REQ_GEN_0
  *         @arg @ref LL_DMAMUX_REQ_GEN_1
  *         @arg @ref LL_DMAMUX_REQ_GEN_2
  *         @arg @ref LL_DMAMUX_REQ_GEN_3
  * @retval None
  */
__STATIC_INLINE void LL_DMAMUX_DisableRequestGen(DMAMUX_Channel_TypeDef *DMAMUXx, uint32_t RequestGenChannel)
{
  uint32_t dmamux_base_addr = (uint32_t)DMAMUXx;

  CLEAR_BIT(((DMAMUX_RequestGen_TypeDef *)(dmamux_base_addr + DMAMUX_REQ_GEN_OFFSET + (DMAMUX_RGCR_SIZE * (RequestGenChannel))))->RGCR, DMAMUX_RGxCR_GE);
}

/**
  * @brief  Check if the Request Generator is enabled or disabled.
  * @rmtoll RGxCR        GE            LL_DMAMUX_IsEnabledRequestGen
  * @param  DMAMUXx DMAMUXx Instance
  * @param  RequestGenChannel This parameter can be one of the following values:
  *         @arg @ref LL_DMAMUX_REQ_GEN_0
  *         @arg @ref LL_DMAMUX_REQ_GEN_1
  *         @arg @ref LL_DMAMUX_REQ_GEN_2
  *         @arg @ref LL_DMAMUX_REQ_GEN_3
  * @retval State of bit (1 or 0).
  */
__STATIC_INLINE uint32_t LL_DMAMUX_IsEnabledRequestGen(DMAMUX_Channel_TypeDef *DMAMUXx, uint32_t RequestGenChannel)
{
  uint32_t dmamux_base_addr = (uint32_t)DMAMUXx;

  return ((READ_BIT(((DMAMUX_RequestGen_TypeDef *)(dmamux_base_addr + DMAMUX_REQ_GEN_OFFSET + (DMAMUX_RGCR_SIZE * RequestGenChannel)))->RGCR, DMAMUX_RGxCR_GE) == (DMAMUX_RGxCR_GE)) ? 1UL : 0UL);
}

/**
  * @brief  Set the polarity of the signal on which the DMA request is generated.
  * @rmtoll RGxCR        GPOL          LL_DMAMUX_SetRequestGenPolarity
  * @param  DMAMUXx DMAMUXx Instance
  * @param  RequestGenChannel This parameter can be one of the following values:
  *         @arg @ref LL_DMAMUX_REQ_GEN_0
  *         @arg @ref LL_DMAMUX_REQ_GEN_1
  *         @arg @ref LL_DMAMUX_REQ_GEN_2
  *         @arg @ref LL_DMAMUX_REQ_GEN_3
  * @param  Polarity This parameter can be one of the following values:
  *         @arg @ref LL_DMAMUX_REQ_GEN_NO_EVENT
  *         @arg @ref LL_DMAMUX_REQ_GEN_POL_RISING
  *         @arg @ref LL_DMAMUX_REQ_GEN_POL_FALLING
  *         @arg @ref LL_DMAMUX_REQ_GEN_POL_RISING_FALLING
  * @retval None
  */
__STATIC_INLINE void LL_DMAMUX_SetRequestGenPolarity(DMAMUX_Channel_TypeDef *DMAMUXx, uint32_t RequestGenChannel, uint32_t Polarity)
{
  uint32_t dmamux_base_addr = (uint32_t)DMAMUXx;

  MODIFY_REG(((DMAMUX_RequestGen_TypeDef *)(dmamux_base_addr + DMAMUX_REQ_GEN_OFFSET + (DMAMUX_RGCR_SIZE * RequestGenChannel)))->RGCR, DMAMUX_RGxCR_GPOL, Polarity);
}

/**
  * @brief  Get the polarity of the signal on which the DMA request is generated.
  * @rmtoll RGxCR        GPOL          LL_DMAMUX_GetRequestGenPolarity
  * @param  DMAMUXx DMAMUXx Instance
  * @param  RequestGenChannel This parameter can be one of the following values:
  *         @arg @ref LL_DMAMUX_REQ_GEN_0
  *         @arg @ref LL_DMAMUX_REQ_GEN_1
  *         @arg @ref LL_DMAMUX_REQ_GEN_2
  *         @arg @ref LL_DMAMUX_REQ_GEN_3
  * @retval Returned value can be one of the following values:
  *         @arg @ref LL_DMAMUX_REQ_GEN_NO_EVENT
  *         @arg @ref LL_DMAMUX_REQ_GEN_POL_RISING
  *         @arg @ref LL_DMAMUX_REQ_GEN_POL_FALLING
  *         @arg @ref LL_DMAMUX_REQ_GEN_POL_RISING_FALLING
  */
__STATIC_INLINE uint32_t LL_DMAMUX_GetRequestGenPolarity(DMAMUX_Channel_TypeDef *DMAMUXx, uint32_t RequestGenChannel)
{
  uint32_t dmamux_base_addr = (uint32_t)DMAMUXx;

  return (uint32_t)(READ_BIT(((DMAMUX_RequestGen_TypeDef *)(dmamux_base_addr + DMAMUX_REQ_GEN_OFFSET + (DMAMUX_RGCR_SIZE * RequestGenChannel)))->RGCR, DMAMUX_RGxCR_GPOL));
}

/**
  * @brief  Set the number of DMA request that will be autorized after a generation event.
  * @note   This field can only be written when Generator is disabled.
  * @rmtoll RGxCR        GNBREQ        LL_DMAMUX_SetGenRequestNb
  * @param  DMAMUXx DMAMUXx Instance
  * @param  RequestGenChannel This parameter can be one of the following values:
  *         @arg @ref LL_DMAMUX_REQ_GEN_0
  *         @arg @ref LL_DMAMUX_REQ_GEN_1
  *         @arg @ref LL_DMAMUX_REQ_GEN_2
  *         @arg @ref LL_DMAMUX_REQ_GEN_3
  * @param  RequestNb This parameter must be a value between Min_Data = 1 and Max_Data = 32.
  * @retval None
  */
__STATIC_INLINE void LL_DMAMUX_SetGenRequestNb(DMAMUX_Channel_TypeDef *DMAMUXx, uint32_t RequestGenChannel, uint32_t RequestNb)
{
  uint32_t dmamux_base_addr = (uint32_t)DMAMUXx;

  MODIFY_REG(((DMAMUX_RequestGen_TypeDef *)(dmamux_base_addr + DMAMUX_REQ_GEN_OFFSET + (DMAMUX_RGCR_SIZE * RequestGenChannel)))->RGCR, DMAMUX_RGxCR_GNBREQ, (RequestNb - 1U) << DMAMUX_RGxCR_GNBREQ_Pos);
}

/**
  * @brief  Get the number of DMA request that will be autorized after a generation event.
  * @rmtoll RGxCR        GNBREQ        LL_DMAMUX_GetGenRequestNb
  * @param  DMAMUXx DMAMUXx Instance
  * @param  RequestGenChannel This parameter can be one of the following values:
  *         @arg @ref LL_DMAMUX_REQ_GEN_0
  *         @arg @ref LL_DMAMUX_REQ_GEN_1
  *         @arg @ref LL_DMAMUX_REQ_GEN_2
  *         @arg @ref LL_DMAMUX_REQ_GEN_3
  * @retval Between Min_Data = 1 and Max_Data = 32
  */
__STATIC_INLINE uint32_t LL_DMAMUX_GetGenRequestNb(DMAMUX_Channel_TypeDef *DMAMUXx, uint32_t RequestGenChannel)
{
  uint32_t dmamux_base_addr = (uint32_t)DMAMUXx;

  return (uint32_t)((READ_BIT(((DMAMUX_RequestGen_TypeDef *)(dmamux_base_addr + DMAMUX_REQ_GEN_OFFSET + (DMAMUX_RGCR_SIZE * RequestGenChannel)))->RGCR, DMAMUX_RGxCR_GNBREQ) >> DMAMUX_RGxCR_GNBREQ_Pos) + 1U);
}

/**
  * @brief  Set DMAMUX external Request Signal ID on DMAMUX Request Generation Trigger Event Channel x.
  * @rmtoll RGxCR        SIG_ID        LL_DMAMUX_SetRequestSignalID
  * @param  DMAMUXx DMAMUXx Instance
  * @param  RequestGenChannel This parameter can be one of the following values:
  *         @arg @ref LL_DMAMUX_REQ_GEN_0
  *         @arg @ref LL_DMAMUX_REQ_GEN_1
  *         @arg @ref LL_DMAMUX_REQ_GEN_2
  *         @arg @ref LL_DMAMUX_REQ_GEN_3
  * @param  RequestSignalID This parameter can be one of the following values:
  *         @arg @ref LL_DMAMUX1_REQ_GEN_DMAMUX1_CH0_EVT
  *         @arg @ref LL_DMAMUX1_REQ_GEN_DMAMUX1_CH1_EVT
  *         @arg @ref LL_DMAMUX1_REQ_GEN_DMAMUX1_CH2_EVT
  *         @arg @ref LL_DMAMUX1_REQ_GEN_DMAMUX1_CH3_EVT
  *
  * @retval None
  */
__STATIC_INLINE void LL_DMAMUX_SetRequestSignalID(DMAMUX_Channel_TypeDef *DMAMUXx, uint32_t RequestGenChannel, uint32_t RequestSignalID)
{
  uint32_t dmamux_base_addr = (uint32_t)DMAMUXx;

  MODIFY_REG(((DMAMUX_RequestGen_TypeDef *)(dmamux_base_addr + DMAMUX_REQ_GEN_OFFSET + (DMAMUX_RGCR_SIZE * RequestGenChannel)))->RGCR, DMAMUX_RGxCR_SIG_ID, RequestSignalID);
}

/**
  * @brief  Get DMAMUX external Request Signal ID set on DMAMUX Channel x.
  * @rmtoll RGxCR        SIG_ID        LL_DMAMUX_GetRequestSignalID
  * @param  DMAMUXx DMAMUXx Instance
  * @param  RequestGenChannel This parameter can be one of the following values:
  *         @arg @ref LL_DMAMUX_REQ_GEN_0
  *         @arg @ref LL_DMAMUX_REQ_GEN_1
  *         @arg @ref LL_DMAMUX_REQ_GEN_2
  *         @arg @ref LL_DMAMUX_REQ_GEN_3
  * @retval Returned value can be one of the following values:
  *         @arg @ref LL_DMAMUX1_SYNC_DMAMUX1_CH0_EVT
  *         @arg @ref LL_DMAMUX1_SYNC_DMAMUX1_CH1_EVT
  *         @arg @ref LL_DMAMUX1_SYNC_DMAMUX1_CH2_EVT
  *         @arg @ref LL_DMAMUX1_SYNC_DMAMUX1_CH3_EVT
  */
__STATIC_INLINE uint32_t LL_DMAMUX_GetRequestSignalID(DMAMUX_Channel_TypeDef *DMAMUXx, uint32_t RequestGenChannel)
{
  uint32_t dmamux_base_addr = (uint32_t)DMAMUXx;

  return (uint32_t)(READ_BIT(((DMAMUX_RequestGen_TypeDef *)(dmamux_base_addr + DMAMUX_REQ_GEN_OFFSET + (DMAMUX_RGCR_SIZE * RequestGenChannel)))->RGCR, DMAMUX_RGxCR_SIG_ID));
}

/**
  * @}
  */

/** @defgroup DMAMUX_LL_EF_FLAG_Management FLAG_Management
  * @{
  */

/**
  * @brief  Get Synchronization Event Overrun Flag Channel 0.
  * @rmtoll CSR          SOF0          LL_DMAMUX_IsActiveFlag_SO0
  * @param  DMAMUXx DMAMUXx DMAMUXx Instance
  * @retval State of bit (1 or 0).
  */
__STATIC_INLINE uint32_t LL_DMAMUX_IsActiveFlag_SO0(DMAMUX_Channel_TypeDef *DMAMUXx)
{
  uint32_t dmamux_base_addr = (uint32_t)DMAMUXx;

  return ((READ_BIT(((DMAMUX_ChannelStatus_TypeDef *)(dmamux_base_addr + DMAMUX_CH_STATUS_OFFSET))->CSR, DMAMUX_CSR_SOF0) == (DMAMUX_CSR_SOF0)) ? 1UL : 0UL);
}

/**
  * @brief  Get Synchronization Event Overrun Flag Channel 1.
  * @rmtoll CSR          SOF1          LL_DMAMUX_IsActiveFlag_SO1
  * @param  DMAMUXx DMAMUXx DMAMUXx Instance
  * @retval State of bit (1 or 0).
  */
__STATIC_INLINE uint32_t LL_DMAMUX_IsActiveFlag_SO1(DMAMUX_Channel_TypeDef *DMAMUXx)
{
  uint32_t dmamux_base_addr = (uint32_t)DMAMUXx;

  return ((READ_BIT(((DMAMUX_ChannelStatus_TypeDef *)(dmamux_base_addr + DMAMUX_CH_STATUS_OFFSET))->CSR, DMAMUX_CSR_SOF1) == (DMAMUX_CSR_SOF1)) ? 1UL : 0UL);
}

/**
  * @brief  Get Synchronization Event Overrun Flag Channel 2.
  * @rmtoll CSR          SOF2          LL_DMAMUX_IsActiveFlag_SO2
  * @param  DMAMUXx DMAMUXx DMAMUXx Instance
  * @retval State of bit (1 or 0).
  */
__STATIC_INLINE uint32_t LL_DMAMUX_IsActiveFlag_SO2(DMAMUX_Channel_TypeDef *DMAMUXx)
{
  uint32_t dmamux_base_addr = (uint32_t)DMAMUXx;

  return ((READ_BIT(((DMAMUX_ChannelStatus_TypeDef *)(dmamux_base_addr + DMAMUX_CH_STATUS_OFFSET))->CSR, DMAMUX_CSR_SOF2) == (DMAMUX_CSR_SOF2)) ? 1UL : 0UL);
}

/**
  * @brief  Get Synchronization Event Overrun Flag Channel 3.
  * @rmtoll CSR          SOF3          LL_DMAMUX_IsActiveFlag_SO3
  * @param  DMAMUXx DMAMUXx DMAMUXx Instance
  * @retval State of bit (1 or 0).
  */
__STATIC_INLINE uint32_t LL_DMAMUX_IsActiveFlag_SO3(DMAMUX_Channel_TypeDef *DMAMUXx)
{
  uint32_t dmamux_base_addr = (uint32_t)DMAMUXx;

  return ((READ_BIT(((DMAMUX_ChannelStatus_TypeDef *)(dmamux_base_addr + DMAMUX_CH_STATUS_OFFSET))->CSR, DMAMUX_CSR_SOF3) == (DMAMUX_CSR_SOF3)) ? 1UL : 0UL);
}

/**
  * @brief  Get Synchronization Event Overrun Flag Channel 4.
  * @rmtoll CSR          SOF4          LL_DMAMUX_IsActiveFlag_SO4
  * @param  DMAMUXx DMAMUXx DMAMUXx Instance
  * @retval State of bit (1 or 0).
  */
__STATIC_INLINE uint32_t LL_DMAMUX_IsActiveFlag_SO4(DMAMUX_Channel_TypeDef *DMAMUXx)
{
  uint32_t dmamux_base_addr = (uint32_t)DMAMUXx;

  return ((READ_BIT(((DMAMUX_ChannelStatus_TypeDef *)(dmamux_base_addr + DMAMUX_CH_STATUS_OFFSET))->CSR, DMAMUX_CSR_SOF4) == (DMAMUX_CSR_SOF4)) ? 1UL : 0UL);
}

/**
  * @brief  Get Synchronization Event Overrun Flag Channel 5.
  * @rmtoll CSR          SOF5          LL_DMAMUX_IsActiveFlag_SO5
  * @param  DMAMUXx DMAMUXx DMAMUXx Instance
  * @retval State of bit (1 or 0).
  */
__STATIC_INLINE uint32_t LL_DMAMUX_IsActiveFlag_SO5(DMAMUX_Channel_TypeDef *DMAMUXx)
{
  uint32_t dmamux_base_addr = (uint32_t)DMAMUXx;

  return ((READ_BIT(((DMAMUX_ChannelStatus_TypeDef *)(dmamux_base_addr + DMAMUX_CH_STATUS_OFFSET))->CSR, DMAMUX_CSR_SOF5) == (DMAMUX_CSR_SOF5)) ? 1UL : 0UL);
}

/**
  * @brief  Get Synchronization Event Overrun Flag Channel 6.
  * @rmtoll CSR          SOF6          LL_DMAMUX_IsActiveFlag_SO6
  * @param  DMAMUXx DMAMUXx DMAMUXx Instance
  * @retval State of bit (1 or 0).
  */
__STATIC_INLINE uint32_t LL_DMAMUX_IsActiveFlag_SO6(DMAMUX_Channel_TypeDef *DMAMUXx)
{
  uint32_t dmamux_base_addr = (uint32_t)DMAMUXx;

  return ((READ_BIT(((DMAMUX_ChannelStatus_TypeDef *)(dmamux_base_addr + DMAMUX_CH_STATUS_OFFSET))->CSR, DMAMUX_CSR_SOF6) == (DMAMUX_CSR_SOF6)) ? 1UL : 0UL);
}

/**
  * @brief  Get Synchronization Event Overrun Flag Channel 7.
  * @rmtoll CSR          SOF7          LL_DMAMUX_IsActiveFlag_SO7
  * @param  DMAMUXx DMAMUXx DMAMUXx Instance
  * @retval State of bit (1 or 0).
  */
__STATIC_INLINE uint32_t LL_DMAMUX_IsActiveFlag_SO7(DMAMUX_Channel_TypeDef *DMAMUXx)
{
  uint32_t dmamux_base_addr = (uint32_t)DMAMUXx;

  return ((READ_BIT(((DMAMUX_ChannelStatus_TypeDef *)(dmamux_base_addr + DMAMUX_CH_STATUS_OFFSET))->CSR, DMAMUX_CSR_SOF7) == (DMAMUX_CSR_SOF7)) ? 1UL : 0UL);
}

/**
  * @brief  Get Synchronization Event Overrun Flag Channel 8.
  * @rmtoll CSR          SOF8          LL_DMAMUX_IsActiveFlag_SO8
  * @param  DMAMUXx DMAMUXx DMAMUXx Instance
  * @retval State of bit (1 or 0).
  */
__STATIC_INLINE uint32_t LL_DMAMUX_IsActiveFlag_SO8(DMAMUX_Channel_TypeDef *DMAMUXx)
{
  uint32_t dmamux_base_addr = (uint32_t)DMAMUXx;

  return ((READ_BIT(((DMAMUX_ChannelStatus_TypeDef *)(dmamux_base_addr + DMAMUX_CH_STATUS_OFFSET))->CSR, DMAMUX_CSR_SOF8) == (DMAMUX_CSR_SOF8)) ? 1UL : 0UL);
}

/**
  * @brief  Get Synchronization Event Overrun Flag Channel 9.
  * @rmtoll CSR          SOF9          LL_DMAMUX_IsActiveFlag_SO9
  * @param  DMAMUXx DMAMUXx DMAMUXx Instance
  * @retval State of bit (1 or 0).
  */
__STATIC_INLINE uint32_t LL_DMAMUX_IsActiveFlag_SO9(DMAMUX_Channel_TypeDef *DMAMUXx)
{
  uint32_t dmamux_base_addr = (uint32_t)DMAMUXx;

  return ((READ_BIT(((DMAMUX_ChannelStatus_TypeDef *)(dmamux_base_addr + DMAMUX_CH_STATUS_OFFSET))->CSR, DMAMUX_CSR_SOF9) == (DMAMUX_CSR_SOF9)) ? 1UL : 0UL);
}

/**
  * @brief  Get Synchronization Event Overrun Flag Channel 10.
  * @rmtoll CSR          SOF10         LL_DMAMUX_IsActiveFlag_SO10
  * @param  DMAMUXx DMAMUXx DMAMUXx Instance
  * @retval State of bit (1 or 0).
  */
__STATIC_INLINE uint32_t LL_DMAMUX_IsActiveFlag_SO10(DMAMUX_Channel_TypeDef *DMAMUXx)
{
  uint32_t dmamux_base_addr = (uint32_t)DMAMUXx;

  return ((READ_BIT(((DMAMUX_ChannelStatus_TypeDef *)(dmamux_base_addr + DMAMUX_CH_STATUS_OFFSET))->CSR, DMAMUX_CSR_SOF10) == (DMAMUX_CSR_SOF10)) ? 1UL : 0UL);
}

/**
  * @brief  Get Synchronization Event Overrun Flag Channel 11.
  * @rmtoll CSR          SOF11         LL_DMAMUX_IsActiveFlag_SO11
  * @param  DMAMUXx DMAMUXx DMAMUXx Instance
  * @retval State of bit (1 or 0).
  */
__STATIC_INLINE uint32_t LL_DMAMUX_IsActiveFlag_SO11(DMAMUX_Channel_TypeDef *DMAMUXx)
{
  uint32_t dmamux_base_addr = (uint32_t)DMAMUXx;

  return ((READ_BIT(((DMAMUX_ChannelStatus_TypeDef *)(dmamux_base_addr + DMAMUX_CH_STATUS_OFFSET))->CSR, DMAMUX_CSR_SOF11) == (DMAMUX_CSR_SOF11)) ? 1UL : 0UL);
}

/**
  * @brief  Get Synchronization Event Overrun Flag Channel 12.
  * @rmtoll CSR          SOF12         LL_DMAMUX_IsActiveFlag_SO12
  * @param  DMAMUXx DMAMUXx DMAMUXx Instance
  * @retval State of bit (1 or 0).
  */
__STATIC_INLINE uint32_t LL_DMAMUX_IsActiveFlag_SO12(DMAMUX_Channel_TypeDef *DMAMUXx)
{
  uint32_t dmamux_base_addr = (uint32_t)DMAMUXx;

  return ((READ_BIT(((DMAMUX_ChannelStatus_TypeDef *)(dmamux_base_addr + DMAMUX_CH_STATUS_OFFSET))->CSR, DMAMUX_CSR_SOF12) == (DMAMUX_CSR_SOF12)) ? 1UL : 0UL);
}

/**
  * @brief  Get Synchronization Event Overrun Flag Channel 13.
  * @rmtoll CSR          SOF13         LL_DMAMUX_IsActiveFlag_SO13
  * @param  DMAMUXx DMAMUXx DMAMUXx Instance
  * @retval State of bit (1 or 0).
  */
__STATIC_INLINE uint32_t LL_DMAMUX_IsActiveFlag_SO13(DMAMUX_Channel_TypeDef *DMAMUXx)
{
  uint32_t dmamux_base_addr = (uint32_t)DMAMUXx;

  return ((READ_BIT(((DMAMUX_ChannelStatus_TypeDef *)(dmamux_base_addr + DMAMUX_CH_STATUS_OFFSET))->CSR, DMAMUX_CSR_SOF13) == (DMAMUX_CSR_SOF13)) ? 1UL : 0UL);
}

/**
  * @brief  Get Synchronization Event Overrun Flag Channel 14.
  * @rmtoll CSR          SOF14         LL_DMAMUX_IsActiveFlag_SO14
  * @param  DMAMUXx DMAMUXx DMAMUXx Instance
  * @retval State of bit (1 or 0).
  */
__STATIC_INLINE uint32_t LL_DMAMUX_IsActiveFlag_SO14(DMAMUX_Channel_TypeDef *DMAMUXx)
{
  uint32_t dmamux_base_addr = (uint32_t)DMAMUXx;

  return ((READ_BIT(((DMAMUX_ChannelStatus_TypeDef *)(dmamux_base_addr + DMAMUX_CH_STATUS_OFFSET))->CSR, DMAMUX_CSR_SOF14) == (DMAMUX_CSR_SOF14)) ? 1UL : 0UL);
}

/**
  * @brief  Get Synchronization Event Overrun Flag Channel 15.
  * @rmtoll CSR          SOF15         LL_DMAMUX_IsActiveFlag_SO15
  * @param  DMAMUXx DMAMUXx DMAMUXx Instance
  * @retval State of bit (1 or 0).
  */
__STATIC_INLINE uint32_t LL_DMAMUX_IsActiveFlag_SO15(DMAMUX_Channel_TypeDef *DMAMUXx)
{
  uint32_t dmamux_base_addr = (uint32_t)DMAMUXx;

  return ((READ_BIT(((DMAMUX_ChannelStatus_TypeDef *)(dmamux_base_addr + DMAMUX_CH_STATUS_OFFSET))->CSR, DMAMUX_CSR_SOF15) == (DMAMUX_CSR_SOF15)) ? 1UL : 0UL);
}

/**
  * @brief  Get Request Generator 0 Trigger Event Overrun Flag.
  * @rmtoll RGSR         OF0           LL_DMAMUX_IsActiveFlag_RGO0
  * @param  DMAMUXx DMAMUXx DMAMUXx Instance
  * @retval State of bit (1 or 0).
  */
__STATIC_INLINE uint32_t LL_DMAMUX_IsActiveFlag_RGO0(DMAMUX_Channel_TypeDef *DMAMUXx)
{
  uint32_t dmamux_base_addr = (uint32_t)DMAMUXx;

  return ((READ_BIT(((DMAMUX_RequestGenStatus_TypeDef *)(dmamux_base_addr + DMAMUX_REQ_GEN_STATUS_OFFSET))->RGSR, DMAMUX_RGSR_OF0) == (DMAMUX_RGSR_OF0)) ? 1UL : 0UL);
}

/**
  * @brief  Get Request Generator 1 Trigger Event Overrun Flag.
  * @rmtoll RGSR         OF1           LL_DMAMUX_IsActiveFlag_RGO1
  * @param  DMAMUXx DMAMUXx DMAMUXx Instance
  * @retval State of bit (1 or 0).
  */
__STATIC_INLINE uint32_t LL_DMAMUX_IsActiveFlag_RGO1(DMAMUX_Channel_TypeDef *DMAMUXx)
{
  uint32_t dmamux_base_addr = (uint32_t)DMAMUXx;

  return ((READ_BIT(((DMAMUX_RequestGenStatus_TypeDef *)(dmamux_base_addr + DMAMUX_REQ_GEN_STATUS_OFFSET))->RGSR, DMAMUX_RGSR_OF1) == (DMAMUX_RGSR_OF1)) ? 1UL : 0UL);
}

/**
  * @brief  Get Request Generator 2 Trigger Event Overrun Flag.
  * @rmtoll RGSR         OF2           LL_DMAMUX_IsActiveFlag_RGO2
  * @param  DMAMUXx DMAMUXx DMAMUXx Instance
  * @retval State of bit (1 or 0).
  */
__STATIC_INLINE uint32_t LL_DMAMUX_IsActiveFlag_RGO2(DMAMUX_Channel_TypeDef *DMAMUXx)
{
  uint32_t dmamux_base_addr = (uint32_t)DMAMUXx;

  return ((READ_BIT(((DMAMUX_RequestGenStatus_TypeDef *)(dmamux_base_addr + DMAMUX_REQ_GEN_STATUS_OFFSET))->RGSR, DMAMUX_RGSR_OF2) == (DMAMUX_RGSR_OF2)) ? 1UL : 0UL);
}

/**
  * @brief  Get Request Generator 3 Trigger Event Overrun Flag.
  * @rmtoll RGSR         OF3           LL_DMAMUX_IsActiveFlag_RGO3
  * @param  DMAMUXx DMAMUXx DMAMUXx Instance
  * @retval State of bit (1 or 0).
  */
__STATIC_INLINE uint32_t LL_DMAMUX_IsActiveFlag_RGO3(DMAMUX_Channel_TypeDef *DMAMUXx)
{
  uint32_t dmamux_base_addr = (uint32_t)DMAMUXx;

  return ((READ_BIT(((DMAMUX_RequestGenStatus_TypeDef *)(dmamux_base_addr + DMAMUX_REQ_GEN_STATUS_OFFSET))->RGSR, DMAMUX_RGSR_OF3) == (DMAMUX_RGSR_OF3)) ? 1UL : 0UL);
}

/**
  * @brief  Clear Synchronization Event Overrun Flag Channel 0.
  * @rmtoll CFR          CSOF0         LL_DMAMUX_ClearFlag_SO0
  * @param  DMAMUXx DMAMUXx DMAMUXx Instance
  * @retval None
  */
__STATIC_INLINE void LL_DMAMUX_ClearFlag_SO0(DMAMUX_Channel_TypeDef *DMAMUXx)
{
  uint32_t dmamux_base_addr = (uint32_t)DMAMUXx;

  SET_BIT(((DMAMUX_ChannelStatus_TypeDef *)(dmamux_base_addr + DMAMUX_CH_STATUS_OFFSET))->CFR, DMAMUX_CFR_CSOF0);
}

/**
  * @brief  Clear Synchronization Event Overrun Flag Channel 1.
  * @rmtoll CFR          CSOF1         LL_DMAMUX_ClearFlag_SO1
  * @param  DMAMUXx DMAMUXx DMAMUXx Instance
  * @retval None
  */
__STATIC_INLINE void LL_DMAMUX_ClearFlag_SO1(DMAMUX_Channel_TypeDef *DMAMUXx)
{
  uint32_t dmamux_base_addr = (uint32_t)DMAMUXx;

  SET_BIT(((DMAMUX_ChannelStatus_TypeDef *)(dmamux_base_addr + DMAMUX_CH_STATUS_OFFSET))->CFR, DMAMUX_CFR_CSOF1);
}

/**
  * @brief  Clear Synchronization Event Overrun Flag Channel 2.
  * @rmtoll CFR          CSOF2         LL_DMAMUX_ClearFlag_SO2
  * @param  DMAMUXx DMAMUXx DMAMUXx Instance
  * @retval None
  */
__STATIC_INLINE void LL_DMAMUX_ClearFlag_SO2(DMAMUX_Channel_TypeDef *DMAMUXx)
{
  uint32_t dmamux_base_addr = (uint32_t)DMAMUXx;

  SET_BIT(((DMAMUX_ChannelStatus_TypeDef *)(dmamux_base_addr + DMAMUX_CH_STATUS_OFFSET))->CFR, DMAMUX_CFR_CSOF2);
}

/**
  * @brief  Clear Synchronization Event Overrun Flag Channel 3.
  * @rmtoll CFR          CSOF3         LL_DMAMUX_ClearFlag_SO3
  * @param  DMAMUXx DMAMUXx DMAMUXx Instance
  * @retval None
  */
__STATIC_INLINE void LL_DMAMUX_ClearFlag_SO3(DMAMUX_Channel_TypeDef *DMAMUXx)
{
  uint32_t dmamux_base_addr = (uint32_t)DMAMUXx;

  SET_BIT(((DMAMUX_ChannelStatus_TypeDef *)(dmamux_base_addr + DMAMUX_CH_STATUS_OFFSET))->CFR, DMAMUX_CFR_CSOF3);
}

/**
  * @brief  Clear Synchronization Event Overrun Flag Channel 4.
  * @rmtoll CFR          CSOF4         LL_DMAMUX_ClearFlag_SO4
  * @param  DMAMUXx DMAMUXx DMAMUXx Instance
  * @retval None
  */
__STATIC_INLINE void LL_DMAMUX_ClearFlag_SO4(DMAMUX_Channel_TypeDef *DMAMUXx)
{
  uint32_t dmamux_base_addr = (uint32_t)DMAMUXx;

  SET_BIT(((DMAMUX_ChannelStatus_TypeDef *)(dmamux_base_addr + DMAMUX_CH_STATUS_OFFSET))->CFR, DMAMUX_CFR_CSOF4);
}

/**
  * @brief  Clear Synchronization Event Overrun Flag Channel 5.
  * @rmtoll CFR          CSOF5         LL_DMAMUX_ClearFlag_SO5
  * @param  DMAMUXx DMAMUXx DMAMUXx Instance
  * @retval None
  */
__STATIC_INLINE void LL_DMAMUX_ClearFlag_SO5(DMAMUX_Channel_TypeDef *DMAMUXx)
{
  uint32_t dmamux_base_addr = (uint32_t)DMAMUXx;

  SET_BIT(((DMAMUX_ChannelStatus_TypeDef *)(dmamux_base_addr + DMAMUX_CH_STATUS_OFFSET))->CFR, DMAMUX_CFR_CSOF5);
}

/**
  * @brief  Clear Synchronization Event Overrun Flag Channel 6.
  * @rmtoll CFR          CSOF6         LL_DMAMUX_ClearFlag_SO6
  * @param  DMAMUXx DMAMUXx DMAMUXx Instance
  * @retval None
  */
__STATIC_INLINE void LL_DMAMUX_ClearFlag_SO6(DMAMUX_Channel_TypeDef *DMAMUXx)
{
  uint32_t dmamux_base_addr = (uint32_t)DMAMUXx;

  SET_BIT(((DMAMUX_ChannelStatus_TypeDef *)(dmamux_base_addr + DMAMUX_CH_STATUS_OFFSET))->CFR, DMAMUX_CFR_CSOF6);
}

/**
  * @brief  Clear Synchronization Event Overrun Flag Channel 7.
  * @rmtoll CFR          CSOF7         LL_DMAMUX_ClearFlag_SO7
  * @param  DMAMUXx DMAMUXx DMAMUXx Instance
  * @retval None
  */
__STATIC_INLINE void LL_DMAMUX_ClearFlag_SO7(DMAMUX_Channel_TypeDef *DMAMUXx)
{
  uint32_t dmamux_base_addr = (uint32_t)DMAMUXx;

  SET_BIT(((DMAMUX_ChannelStatus_TypeDef *)(dmamux_base_addr + DMAMUX_CH_STATUS_OFFSET))->CFR, DMAMUX_CFR_CSOF7);
}

/**
  * @brief  Clear Synchronization Event Overrun Flag Channel 8.
  * @rmtoll CFR          CSOF8         LL_DMAMUX_ClearFlag_SO8
  * @param  DMAMUXx DMAMUXx DMAMUXx Instance
  * @retval None
  */
__STATIC_INLINE void LL_DMAMUX_ClearFlag_SO8(DMAMUX_Channel_TypeDef *DMAMUXx)
{
  uint32_t dmamux_base_addr = (uint32_t)DMAMUXx;

  SET_BIT(((DMAMUX_ChannelStatus_TypeDef *)(dmamux_base_addr + DMAMUX_CH_STATUS_OFFSET))->CFR, DMAMUX_CFR_CSOF8);
}

/**
  * @brief  Clear Synchronization Event Overrun Flag Channel 9.
  * @rmtoll CFR          CSOF9         LL_DMAMUX_ClearFlag_SO9
  * @param  DMAMUXx DMAMUXx DMAMUXx Instance
  * @retval None
  */
__STATIC_INLINE void LL_DMAMUX_ClearFlag_SO9(DMAMUX_Channel_TypeDef *DMAMUXx)
{
  uint32_t dmamux_base_addr = (uint32_t)DMAMUXx;

  SET_BIT(((DMAMUX_ChannelStatus_TypeDef *)(dmamux_base_addr + DMAMUX_CH_STATUS_OFFSET))->CFR, DMAMUX_CFR_CSOF9);
}

/**
  * @brief  Clear Synchronization Event Overrun Flag Channel 10.
  * @rmtoll CFR          CSOF10        LL_DMAMUX_ClearFlag_SO10
  * @param  DMAMUXx DMAMUXx DMAMUXx Instance
  * @retval None
  */
__STATIC_INLINE void LL_DMAMUX_ClearFlag_SO10(DMAMUX_Channel_TypeDef *DMAMUXx)
{
  uint32_t dmamux_base_addr = (uint32_t)DMAMUXx;

  SET_BIT(((DMAMUX_ChannelStatus_TypeDef *)(dmamux_base_addr + DMAMUX_CH_STATUS_OFFSET))->CFR, DMAMUX_CFR_CSOF10);
}

/**
  * @brief  Clear Synchronization Event Overrun Flag Channel 11.
  * @rmtoll CFR          CSOF11        LL_DMAMUX_ClearFlag_SO11
  * @param  DMAMUXx DMAMUXx DMAMUXx Instance
  * @retval None
  */
__STATIC_INLINE void LL_DMAMUX_ClearFlag_SO11(DMAMUX_Channel_TypeDef *DMAMUXx)
{
  uint32_t dmamux_base_addr = (uint32_t)DMAMUXx;

  SET_BIT(((DMAMUX_ChannelStatus_TypeDef *)(dmamux_base_addr + DMAMUX_CH_STATUS_OFFSET))->CFR, DMAMUX_CFR_CSOF11);
}

/**
  * @brief  Clear Synchronization Event Overrun Flag Channel 12.
  * @rmtoll CFR          CSOF12        LL_DMAMUX_ClearFlag_SO12
  * @param  DMAMUXx DMAMUXx DMAMUXx Instance
  * @retval None
  */
__STATIC_INLINE void LL_DMAMUX_ClearFlag_SO12(DMAMUX_Channel_TypeDef *DMAMUXx)
{
  uint32_t dmamux_base_addr = (uint32_t)DMAMUXx;

  SET_BIT(((DMAMUX_ChannelStatus_TypeDef *)(dmamux_base_addr + DMAMUX_CH_STATUS_OFFSET))->CFR, DMAMUX_CFR_CSOF12);
}

/**
  * @brief  Clear Synchronization Event Overrun Flag Channel 13.
  * @rmtoll CFR          CSOF13        LL_DMAMUX_ClearFlag_SO13
  * @param  DMAMUXx DMAMUXx DMAMUXx Instance
  * @retval None
  */
__STATIC_INLINE void LL_DMAMUX_ClearFlag_SO13(DMAMUX_Channel_TypeDef *DMAMUXx)
{
  uint32_t dmamux_base_addr = (uint32_t)DMAMUXx;

  SET_BIT(((DMAMUX_ChannelStatus_TypeDef *)(dmamux_base_addr + DMAMUX_CH_STATUS_OFFSET))->CFR, DMAMUX_CFR_CSOF13);
}

/**
  * @brief  Clear Synchronization Event Overrun Flag Channel 14.
  * @rmtoll CFR          CSOF14        LL_DMAMUX_ClearFlag_SO14
  * @param  DMAMUXx DMAMUXx DMAMUXx Instance
  * @retval None
  */
__STATIC_INLINE void LL_DMAMUX_ClearFlag_SO14(DMAMUX_Channel_TypeDef *DMAMUXx)
{
  uint32_t dmamux_base_addr = (uint32_t)DMAMUXx;

  SET_BIT(((DMAMUX_ChannelStatus_TypeDef *)(dmamux_base_addr + DMAMUX_CH_STATUS_OFFSET))->CFR, DMAMUX_CFR_CSOF14);
}

/**
  * @brief  Clear Synchronization Event Overrun Flag Channel 15.
  * @rmtoll CFR          CSOF15        LL_DMAMUX_ClearFlag_SO15
  * @param  DMAMUXx DMAMUXx DMAMUXx Instance
  * @retval None
  */
__STATIC_INLINE void LL_DMAMUX_ClearFlag_SO15(DMAMUX_Channel_TypeDef *DMAMUXx)
{
  uint32_t dmamux_base_addr = (uint32_t)DMAMUXx;

  SET_BIT(((DMAMUX_ChannelStatus_TypeDef *)(dmamux_base_addr + DMAMUX_CH_STATUS_OFFSET))->CFR, DMAMUX_CFR_CSOF15);
}

/**
  * @brief  Clear Request Generator 0 Trigger Event Overrun Flag.
  * @rmtoll RGCFR        COF0          LL_DMAMUX_ClearFlag_RGO0
  * @param  DMAMUXx DMAMUXx DMAMUXx Instance
  * @retval None
  */
__STATIC_INLINE void LL_DMAMUX_ClearFlag_RGO0(DMAMUX_Channel_TypeDef *DMAMUXx)
{
  uint32_t dmamux_base_addr = (uint32_t)DMAMUXx;

  SET_BIT(((DMAMUX_RequestGenStatus_TypeDef *)(dmamux_base_addr + DMAMUX_REQ_GEN_STATUS_OFFSET))->RGCFR, DMAMUX_RGCFR_COF0);
}

/**
  * @brief  Clear Request Generator 1 Trigger Event Overrun Flag.
  * @rmtoll RGCFR        COF1          LL_DMAMUX_ClearFlag_RGO1
  * @param  DMAMUXx DMAMUXx DMAMUXx Instance
  * @retval None
  */
__STATIC_INLINE void LL_DMAMUX_ClearFlag_RGO1(DMAMUX_Channel_TypeDef *DMAMUXx)
{
  uint32_t dmamux_base_addr = (uint32_t)DMAMUXx;

  SET_BIT(((DMAMUX_RequestGenStatus_TypeDef *)(dmamux_base_addr + DMAMUX_REQ_GEN_STATUS_OFFSET))->RGCFR, DMAMUX_RGCFR_COF1);
}

/**
  * @brief  Clear Request Generator 2 Trigger Event Overrun Flag.
  * @rmtoll RGCFR        COF2          LL_DMAMUX_ClearFlag_RGO2
  * @param  DMAMUXx DMAMUXx DMAMUXx Instance
  * @retval None
  */
__STATIC_INLINE void LL_DMAMUX_ClearFlag_RGO2(DMAMUX_Channel_TypeDef *DMAMUXx)
{
  uint32_t dmamux_base_addr = (uint32_t)DMAMUXx;

  SET_BIT(((DMAMUX_RequestGenStatus_TypeDef *)(dmamux_base_addr + DMAMUX_REQ_GEN_STATUS_OFFSET))->RGCFR, DMAMUX_RGCFR_COF2);
}

/**
  * @brief  Clear Request Generator 3 Trigger Event Overrun Flag.
  * @rmtoll RGCFR        COF3          LL_DMAMUX_ClearFlag_RGO3
  * @param  DMAMUXx DMAMUXx DMAMUXx Instance
  * @retval None
  */
__STATIC_INLINE void LL_DMAMUX_ClearFlag_RGO3(DMAMUX_Channel_TypeDef *DMAMUXx)
{
  uint32_t dmamux_base_addr = (uint32_t)DMAMUXx;

  SET_BIT(((DMAMUX_RequestGenStatus_TypeDef *)(dmamux_base_addr + DMAMUX_REQ_GEN_STATUS_OFFSET))->RGCFR, DMAMUX_RGCFR_COF3);
}
/**
  * @}
  */

/** @defgroup DMAMUX_LL_EF_IT_Management IT_Management
  * @{
  */

/**
  * @brief  Enable the Synchronization Event Overrun Interrupt on DMAMUX channel x.
  * @rmtoll CxCR         SOIE          LL_DMAMUX_EnableIT_SO
  * @param  DMAMUXx DMAMUXx Instance
  * @param  Channel This parameter can be one of the following values:
  *         @arg @ref LL_DMAMUX_CHANNEL_0
  *         @arg @ref LL_DMAMUX_CHANNEL_1
  *         @arg @ref LL_DMAMUX_CHANNEL_2
  *         @arg @ref LL_DMAMUX_CHANNEL_3
  *         @arg @ref LL_DMAMUX_CHANNEL_4
  *         @arg @ref LL_DMAMUX_CHANNEL_5
  *         @arg @ref LL_DMAMUX_CHANNEL_6
  *         @arg @ref LL_DMAMUX_CHANNEL_7
  *         @arg @ref LL_DMAMUX_CHANNEL_8
  *         @arg @ref LL_DMAMUX_CHANNEL_9
  *         @arg @ref LL_DMAMUX_CHANNEL_10
  *         @arg @ref LL_DMAMUX_CHANNEL_11
  *         @arg @ref LL_DMAMUX_CHANNEL_12
  *         @arg @ref LL_DMAMUX_CHANNEL_13
  *         @arg @ref LL_DMAMUX_CHANNEL_14
  *         @arg @ref LL_DMAMUX_CHANNEL_15
  * @retval None
  */
__STATIC_INLINE void LL_DMAMUX_EnableIT_SO(DMAMUX_Channel_TypeDef *DMAMUXx, uint32_t Channel)
{
  uint32_t dmamux_base_addr = (uint32_t)DMAMUXx;

  SET_BIT(((DMAMUX_Channel_TypeDef *)((uint32_t)(dmamux_base_addr + (DMAMUX_CCR_SIZE * (Channel)))))->CCR, DMAMUX_CxCR_SOIE);
}

/**
  * @brief  Disable the Synchronization Event Overrun Interrupt on DMAMUX channel x.
  * @rmtoll CxCR         SOIE          LL_DMAMUX_DisableIT_SO
  * @param  DMAMUXx DMAMUXx Instance
  * @param  Channel This parameter can be one of the following values:
  *         @arg @ref LL_DMAMUX_CHANNEL_0
  *         @arg @ref LL_DMAMUX_CHANNEL_1
  *         @arg @ref LL_DMAMUX_CHANNEL_2
  *         @arg @ref LL_DMAMUX_CHANNEL_3
  *         @arg @ref LL_DMAMUX_CHANNEL_4
  *         @arg @ref LL_DMAMUX_CHANNEL_5
  *         @arg @ref LL_DMAMUX_CHANNEL_6
  *         @arg @ref LL_DMAMUX_CHANNEL_7
  *         @arg @ref LL_DMAMUX_CHANNEL_8
  *         @arg @ref LL_DMAMUX_CHANNEL_9
  *         @arg @ref LL_DMAMUX_CHANNEL_10
  *         @arg @ref LL_DMAMUX_CHANNEL_11
  *         @arg @ref LL_DMAMUX_CHANNEL_12
  *         @arg @ref LL_DMAMUX_CHANNEL_13
  *         @arg @ref LL_DMAMUX_CHANNEL_14
  *         @arg @ref LL_DMAMUX_CHANNEL_15
  * @retval None
  */
__STATIC_INLINE void LL_DMAMUX_DisableIT_SO(DMAMUX_Channel_TypeDef *DMAMUXx, uint32_t Channel)
{
  uint32_t dmamux_base_addr = (uint32_t)DMAMUXx;

  CLEAR_BIT(((DMAMUX_Channel_TypeDef *)((uint32_t)(dmamux_base_addr + (DMAMUX_CCR_SIZE * (Channel)))))->CCR, DMAMUX_CxCR_SOIE);
}

/**
  * @brief  Check if the Synchronization Event Overrun Interrupt on DMAMUX channel x is enabled or disabled.
  * @rmtoll CxCR         SOIE          LL_DMAMUX_IsEnabledIT_SO
  * @param  DMAMUXx DMAMUXx Instance
  * @param  Channel This parameter can be one of the following values:
  *         @arg @ref LL_DMAMUX_CHANNEL_0
  *         @arg @ref LL_DMAMUX_CHANNEL_1
  *         @arg @ref LL_DMAMUX_CHANNEL_2
  *         @arg @ref LL_DMAMUX_CHANNEL_3
  *         @arg @ref LL_DMAMUX_CHANNEL_4
  *         @arg @ref LL_DMAMUX_CHANNEL_5
  *         @arg @ref LL_DMAMUX_CHANNEL_6
  *         @arg @ref LL_DMAMUX_CHANNEL_7
  *         @arg @ref LL_DMAMUX_CHANNEL_8
  *         @arg @ref LL_DMAMUX_CHANNEL_9
  *         @arg @ref LL_DMAMUX_CHANNEL_10
  *         @arg @ref LL_DMAMUX_CHANNEL_11
  *         @arg @ref LL_DMAMUX_CHANNEL_12
  *         @arg @ref LL_DMAMUX_CHANNEL_13
  *         @arg @ref LL_DMAMUX_CHANNEL_14
  *         @arg @ref LL_DMAMUX_CHANNEL_15
  * @retval State of bit (1 or 0).
  */
__STATIC_INLINE uint32_t LL_DMAMUX_IsEnabledIT_SO(DMAMUX_Channel_TypeDef *DMAMUXx, uint32_t Channel)
{
  uint32_t dmamux_base_addr = (uint32_t)DMAMUXx;

  return (READ_BIT(((DMAMUX_Channel_TypeDef *)(dmamux_base_addr + (DMAMUX_CCR_SIZE * (Channel))))->CCR, DMAMUX_CxCR_SOIE));
}

/**
  * @brief  Enable the Request Generation Trigger Event Overrun Interrupt on DMAMUX channel x.
  * @rmtoll RGxCR        OIE           LL_DMAMUX_EnableIT_RGO
  * @param  DMAMUXx DMAMUXx Instance
  * @param  RequestGenChannel This parameter can be one of the following values:
  *         @arg @ref LL_DMAMUX_REQ_GEN_0
  *         @arg @ref LL_DMAMUX_REQ_GEN_1
  *         @arg @ref LL_DMAMUX_REQ_GEN_2
  *         @arg @ref LL_DMAMUX_REQ_GEN_3
  * @retval None
  */
__STATIC_INLINE void LL_DMAMUX_EnableIT_RGO(DMAMUX_Channel_TypeDef *DMAMUXx, uint32_t RequestGenChannel)
{
  uint32_t dmamux_base_addr = (uint32_t)DMAMUXx;

  SET_BIT(((DMAMUX_RequestGen_TypeDef *)(dmamux_base_addr + DMAMUX_REQ_GEN_OFFSET + (DMAMUX_RGCR_SIZE * RequestGenChannel)))->RGCR, DMAMUX_RGxCR_OIE);
}

/**
  * @brief  Disable the Request Generation Trigger Event Overrun Interrupt on DMAMUX channel x.
  * @rmtoll RGxCR        OIE           LL_DMAMUX_DisableIT_RGO
  * @param  DMAMUXx DMAMUXx Instance
  * @param  RequestGenChannel This parameter can be one of the following values:
  *         @arg @ref LL_DMAMUX_REQ_GEN_0
  *         @arg @ref LL_DMAMUX_REQ_GEN_1
  *         @arg @ref LL_DMAMUX_REQ_GEN_2
  *         @arg @ref LL_DMAMUX_REQ_GEN_3
  * @retval None
  */
__STATIC_INLINE void LL_DMAMUX_DisableIT_RGO(DMAMUX_Channel_TypeDef *DMAMUXx, uint32_t RequestGenChannel)
{
  uint32_t dmamux_base_addr = (uint32_t)DMAMUXx;

  CLEAR_BIT(((DMAMUX_RequestGen_TypeDef *)(dmamux_base_addr + DMAMUX_REQ_GEN_OFFSET + (DMAMUX_RGCR_SIZE * RequestGenChannel)))->RGCR, DMAMUX_RGxCR_OIE);
}

/**
  * @brief  Check if the Request Generation Trigger Event Overrun Interrupt on DMAMUX channel x is enabled or disabled.
  * @rmtoll RGxCR        OIE           LL_DMAMUX_IsEnabledIT_RGO
  * @param  DMAMUXx DMAMUXx Instance
  * @param  RequestGenChannel This parameter can be one of the following values:
  *         @arg @ref LL_DMAMUX_REQ_GEN_0
  *         @arg @ref LL_DMAMUX_REQ_GEN_1
  *         @arg @ref LL_DMAMUX_REQ_GEN_2
  *         @arg @ref LL_DMAMUX_REQ_GEN_3
  * @retval State of bit (1 or 0).
  */
__STATIC_INLINE uint32_t LL_DMAMUX_IsEnabledIT_RGO(DMAMUX_Channel_TypeDef *DMAMUXx, uint32_t RequestGenChannel)
{
  uint32_t dmamux_base_addr = (uint32_t)DMAMUXx;

  return ((READ_BIT(((DMAMUX_RequestGen_TypeDef *)(dmamux_base_addr + DMAMUX_REQ_GEN_OFFSET + (DMAMUX_RGCR_SIZE * RequestGenChannel)))->RGCR, DMAMUX_RGxCR_OIE) == (DMAMUX_RGxCR_OIE)) ? 1UL : 0UL);
}

/**
  * @}
  */

/**
  * @}
  */

/**
  * @}
  */

#endif /* DMAMUX1 */

/**
  * @}
  */

#ifdef __cplusplus
}
#endif

#endif /* SR5E1_LL_DMAMUX_H */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
