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
*   @file    sr5e1_macros.h
*   @version M4_VERSION
*   @brief   Low Level Registers macros.
*
*   @details Low Level Registers macros.
*
 */
#ifndef _SR5E1_MACROS_H
#define _SR5E1_MACROS_H

#include <sr5e1_wrapper.h>

/**
 * @name   Exported types.
 * @{
 */
typedef enum {
     RESET = 0,
     SET = !RESET
} FlagStatus, ITStatus;

typedef enum {
    DISABLE = 0,
    ENABLE = !DISABLE
} FunctionalState;

typedef enum {
    SUCCESS = 0,
    ERROR = !SUCCESS
} ErrorStatus;
/** @} */

/**
 * @name   Exported macros.
 * @{
 */
#define IS_FUNCTIONAL_STATE(STATE)               (((STATE) == DISABLE) || ((STATE) == ENABLE))

#define SET_BIT(REG, BIT)                        ((REG) |= (BIT))
#define CLEAR_BIT(REG, BIT)                      ((REG) &= ~(BIT))
#define READ_BIT(REG, BIT)                       ((REG) & (BIT))
#define CLEAR_REG(REG)                           ((REG) = (0x0))
#define WRITE_REG(REG, VAL)                      ((REG) = (VAL))
#define READ_REG(REG)                            ((REG))
#define MODIFY_REG(REG, CLEARMASK, SETMASK)      WRITE_REG((REG), (((READ_REG(REG)) & (~(CLEARMASK))) | (SETMASK)))
#define POSITION_VAL(VAL)                        (__CLZ(__RBIT(VAL)))
/** @} */

/******************************* ADC Instances *******************************/
#define ADC_MULTIMODE_SUPPORT                 /*!< ADC feature available only on specific devices: multimode available on devices with several ADC instances */

#define IS_ADC_ALL_INSTANCE(INSTANCE) (((INSTANCE) == ADC1) || \
                                       ((INSTANCE) == ADC2) || \
                                       ((INSTANCE) == ADC3) || \
                                       ((INSTANCE) == ADC4) || \
                                       ((INSTANCE) == ADC5))

#define IS_ADC_MULTIMODE_MASTER_INSTANCE(INSTANCE) ((((INSTANCE) == ADC1)) || \
                                                     ((INSTANCE) == ADC3))

#define IS_ADC_COMMON_INSTANCE(INSTANCE) (((INSTANCE) == ADC12_COMMON) || \
                                          ((INSTANCE) == ADC345_COMMON))

/******************************* COMP Instances *******************************/
#define IS_COMP_ALL_INSTANCE(INSTANCE) (((INSTANCE) == COMP1) || \
                                        ((INSTANCE) == COMP2) || \
                                        ((INSTANCE) == COMP3) || \
                                        ((INSTANCE) == COMP4) || \
                                        ((INSTANCE) == COMP5) || \
                                        ((INSTANCE) == COMP6) || \
                                        ((INSTANCE) == COMP7) || \
                                        ((INSTANCE) == COMP8))
                                        
/******************************* CORDIC Instances *****************************/
#define IS_CORDIC_ALL_INSTANCE(INSTANCE) ((INSTANCE) == CORDIC)

/******************************* DAC Instances ********************************/
#define IS_DAC_ALL_INSTANCE(INSTANCE) (((INSTANCE) == DAC1)  || \
                                       ((INSTANCE) == BDAC1) || \
                                       ((INSTANCE) == DAC2)  || \
                                       ((INSTANCE) == DAC3)  || \
                                       ((INSTANCE) == DAC4))

/******************************* DMA Instances *******************************/
#define IS_DMA_ALL_INSTANCE(INSTANCE) (((INSTANCE) == DMA1_Stream0) || \
                                       ((INSTANCE) == DMA1_Stream1) || \
                                       ((INSTANCE) == DMA1_Stream2) || \
                                       ((INSTANCE) == DMA1_Stream3) || \
                                       ((INSTANCE) == DMA1_Stream4) || \
                                       ((INSTANCE) == DMA1_Stream5) || \
                                       ((INSTANCE) == DMA1_Stream6) || \
                                       ((INSTANCE) == DMA1_Stream7) || \
                                       ((INSTANCE) == DMA2_Stream0) || \
                                       ((INSTANCE) == DMA2_Stream1) || \
                                       ((INSTANCE) == DMA2_Stream2) || \
                                       ((INSTANCE) == DMA2_Stream3) || \
                                       ((INSTANCE) == DMA2_Stream4) || \
                                       ((INSTANCE) == DMA2_Stream5) || \
                                       ((INSTANCE) == DMA2_Stream6) || \
                                       ((INSTANCE) == DMA2_Stream7))

/******************************* FDCAN Instances *******************************/
#define IS_FDCAN_ALL_INSTANCE(INSTANCE) (((INSTANCE) == FDCAN1) || \
                                         ((INSTANCE) == FDCAN2) || \
                                         ((INSTANCE) == FDCAN3) || \
                                         ((INSTANCE) == FDCAN4))

/******************************* GPIO Instances *******************************/
#define IS_GPIO_ALL_INSTANCE(INSTANCE) (((INSTANCE) == GPIOA) || \
                                        ((INSTANCE) == GPIOB) || \
                                        ((INSTANCE) == GPIOC) || \
                                        ((INSTANCE) == GPIOD) || \
                                        ((INSTANCE) == GPIOE) || \
                                        ((INSTANCE) == GPIOF) || \
                                        ((INSTANCE) == GPIOG) || \
                                        ((INSTANCE) == GPIOH) || \
                                        ((INSTANCE) == GPIOI))

/******************************* GPIO AF Instances ****************************/
#define IS_GPIO_AF_INSTANCE(INSTANCE)   IS_GPIO_ALL_INSTANCE(INSTANCE)

/**************************** GPIO Lock Instances *****************************/
#define IS_GPIO_LOCK_INSTANCE(INSTANCE) IS_GPIO_ALL_INSTANCE(INSTANCE)

/****************************** HRTIM Instances *******************************/
#define IS_HRTIM_ALL_INSTANCE(INSTANCE) (((INSTANCE) == HRTIM1) || \
                                         ((INSTANCE) == HRTIM2))

/******************************** I2S Instances *******************************/
#define SPI_I2S_SUPPORT                       /*!< I2S support */

#define IS_I2S_ALL_INSTANCE(__INSTANCE__)  (((__INSTANCE__) == SPI2) || \
                                            ((__INSTANCE__) == SPI3))

/******************************** SPI Instances *******************************/
#define IS_SPI_ALL_INSTANCE(INSTANCE) (((INSTANCE) == SPI1) || \
                                       ((INSTANCE) == SPI2) || \
                                       ((INSTANCE) == SPI3) || \
                                       ((INSTANCE) == SPI4))

/******************************* TIM Instances *******************************/
#define IS_TIM_INSTANCE(INSTANCE)       (((INSTANCE) == TIM1_PWM) || \
                                         ((INSTANCE) == TIM2)     || \
                                         ((INSTANCE) == TIM3)     || \
                                         ((INSTANCE) == TIM4)     || \
                                         ((INSTANCE) == TIM5)     || \
                                         ((INSTANCE) == TIM6)     || \
                                         ((INSTANCE) == TIM7)     || \
                                         ((INSTANCE) == TIM8_PWM) || \
                                         ((INSTANCE) == TIM15)    || \
                                         ((INSTANCE) == TIM16)    || \
                                         ((INSTANCE) == TIM_TS))

/****************** TIM Instances : supporting encoder interface **************/
#define IS_TIM_ENCODER_INTERFACE_INSTANCE(INSTANCE)  (((INSTANCE) == TIM1_PWM) || \
                                                      ((INSTANCE) == TIM2)     || \
                                                      ((INSTANCE) == TIM3)     || \
                                                      ((INSTANCE) == TIM4)     || \
                                                      ((INSTANCE) == TIM5)     || \
                                                      ((INSTANCE) == TIM8_PWM) || \
                                                      ((INSTANCE) == TIM15)    || \
                                                      ((INSTANCE) == TIM16))

/****************** TIM Instances : supporting Hall sensor interface **********/
#define IS_TIM_HALL_SENSOR_INTERFACE_INSTANCE(INSTANCE) (((INSTANCE) == TIM1_PWM) || \
                                                         ((INSTANCE) == TIM2)     || \
                                                         ((INSTANCE) == TIM3)     || \
                                                         ((INSTANCE) == TIM4)     || \
                                                         ((INSTANCE) == TIM5)     || \
                                                         ((INSTANCE) == TIM8_PWM) || \
                                                         ((INSTANCE) == TIM15)    || \
                                                         ((INSTANCE) == TIM16))

/****************** TIM Instances : supporting 32 bits counter (counter width=32) ****************/
#define IS_TIM_32B_COUNTER_INSTANCE(INSTANCE) (((INSTANCE) == TIM2)   || \
                                               ((INSTANCE) == TIM5)   || \
                                               ((INSTANCE) == TIM_TS))

/****************** TIM Instances : supporting the break function (bk_impl>0) *************/
#define IS_TIM_BREAK_INSTANCE(INSTANCE) (((INSTANCE) == TIM1_PWM) || \
                                         ((INSTANCE) == TIM8_PWM) || \
                                         ((INSTANCE) == TIM15)    || \
                                         ((INSTANCE) == TIM16))

/************** TIM Instances : supporting Break input selection *************/
#define IS_TIM_BREAKSOURCE_INSTANCE(INSTANCE) (((INSTANCE) == TIM1_PWM) || \
                                               ((INSTANCE) == TIM8_PWM) || \
                                               ((INSTANCE) == TIM15)    || \
                                               ((INSTANCE) == TIM16))

/****************** TIM Instances : supporting 2 break inputs (BK2F implemented if nb_of_cc>0 and bk_impl>2) *****************/
#define IS_TIM_BKIN2_INSTANCE(INSTANCE) (((INSTANCE) == TIM1_PWM)   || \
                                         ((INSTANCE) == TIM8_PWM))

/************* TIM Instances : at least 1 capture/compare channel (nb_of_cc>=1) *************/
#define IS_TIM_CC1_INSTANCE(INSTANCE)   (((INSTANCE) == TIM1_PWM) || \
                                         ((INSTANCE) == TIM8_PWM) || \
                                         ((INSTANCE) == TIM2)     || \
                                         ((INSTANCE) == TIM3)     || \
                                         ((INSTANCE) == TIM4)     || \
                                         ((INSTANCE) == TIM5)     || \
                                         ((INSTANCE) == TIM15)    || \
                                         ((INSTANCE) == TIM16))

/************ TIM Instances : at least 2 capture/compare channels (nb_of_cc>=2)*************/
#define IS_TIM_CC2_INSTANCE(INSTANCE)   (((INSTANCE) == TIM1_PWM) || \
                                         ((INSTANCE) == TIM8_PWM) || \
                                         ((INSTANCE) == TIM2)     || \
                                         ((INSTANCE) == TIM3)     || \
                                         ((INSTANCE) == TIM4)     || \
                                         ((INSTANCE) == TIM5)     || \
                                         ((INSTANCE) == TIM15))

/************ TIM Instances : at least 3 capture/compare channels (nb_of_cc>=3)*************/
#define IS_TIM_CC3_INSTANCE(INSTANCE)   (((INSTANCE) == TIM1_PWM) || \
                                         ((INSTANCE) == TIM8_PWM) || \
                                         ((INSTANCE) == TIM2)     || \
                                         ((INSTANCE) == TIM3)     || \
                                         ((INSTANCE) == TIM4)     || \
                                         ((INSTANCE) == TIM5))

/************ TIM Instances : at least 4 capture/compare channels (nb_of_cc>=4)*************/
#define IS_TIM_CC4_INSTANCE(INSTANCE)   (((INSTANCE) == TIM1_PWM) || \
                                         ((INSTANCE) == TIM8_PWM) || \
                                         ((INSTANCE) == TIM2)     || \
                                         ((INSTANCE) == TIM3)     || \
                                         ((INSTANCE) == TIM4)     || \
                                         ((INSTANCE) == TIM5))

/****************** TIM Instances : at least 5 capture/compare channels *******/
#define IS_TIM_CC5_INSTANCE(INSTANCE)   (((INSTANCE) == TIM1_PWM) || \
                                         ((INSTANCE) == TIM8_PWM))

/****************** TIM Instances : at least 6 capture/compare channels *******/
#define IS_TIM_CC6_INSTANCE(INSTANCE)   (((INSTANCE) == TIM1_PWM)  || \
                                         ((INSTANCE) == TIM8_PWM))

/****************** TIM Instances : DMA requests generation (TIMx_DIER.UDE) (dma_impl=1)***/
#define IS_TIM_DMA_INSTANCE(INSTANCE)      (((INSTANCE) == TIM1_PWM) || \
                                            ((INSTANCE) == TIM8_PWM) || \
                                            ((INSTANCE) == TIM2)     || \
                                            ((INSTANCE) == TIM3)     || \
                                            ((INSTANCE) == TIM4)     || \
                                            ((INSTANCE) == TIM5)     || \
                                            ((INSTANCE) == TIM6)     || \
                                            ((INSTANCE) == TIM7)     || \
                                            ((INSTANCE) == TIM15)    || \
                                            ((INSTANCE) == TIM16)    || \
                                            ((INSTANCE) == TIM_TS))

/************ TIM Instances : DMA requests generation (TIMx_DIER.CCxDE) (implemented if (nb_of_cc>(x-1) and dma_impl>0))*******/
#define IS_TIM_DMA_CC_INSTANCE(INSTANCE)   (((INSTANCE) == TIM1_PWM) || \
                                            ((INSTANCE) == TIM8_PWM) || \
                                            ((INSTANCE) == TIM2)     || \
                                            ((INSTANCE) == TIM3)     || \
                                            ((INSTANCE) == TIM4)     || \
                                            ((INSTANCE) == TIM5)     || \
                                            ((INSTANCE) == TIM15)    || \
                                            ((INSTANCE) == TIM16))

/******************** TIM Instances : DMA burst feature (TIMx_DMAR.DMAB) (dma_impl=2)***********************/
#define IS_TIM_DMABURST_INSTANCE(INSTANCE) (((INSTANCE) == TIM1_PWM) || \
                                            ((INSTANCE) == TIM8_PWM) || \
                                            ((INSTANCE) == TIM2)     || \
                                            ((INSTANCE) == TIM3)     || \
                                            ((INSTANCE) == TIM4)     || \
                                            ((INSTANCE) == TIM5)     || \
                                            ((INSTANCE) == TIM15)    || \
                                            ((INSTANCE) == TIM16))

/******************* TIM Instances : Timer input selection (TISEL not managed as only 0 is acceptable in fields) ********************/
#define IS_TIM_TISEL_INSTANCE(INSTANCE)  (((INSTANCE) == TIM1_PWM)  || \
                                          ((INSTANCE) == TIM8_PWM)  || \
                                          ((INSTANCE) == TIM2)      || \
                                          ((INSTANCE) == TIM3)      || \
                                          ((INSTANCE) == TIM4)      || \
                                          ((INSTANCE) == TIM5)      || \
                                          ((INSTANCE) == TIM15)     || \
                                          ((INSTANCE) == TIM16))

/******************* TIM Instances : output(s) available (nb_of_cc) **********************/
#define IS_TIM_CCX_INSTANCE(INSTANCE, CHANNEL)  \
        ((((INSTANCE) == TIM1_PWM) &&           \
           (((CHANNEL) == TIM_CHANNEL_1)   ||   \
            ((CHANNEL) == TIM_CHANNEL_2)   ||   \
            ((CHANNEL) == TIM_CHANNEL_3)   ||   \
            ((CHANNEL) == TIM_CHANNEL_4)   ||   \
            ((CHANNEL) == TIM_CHANNEL_5)   ||   \
            ((CHANNEL) == TIM_CHANNEL_6))) ||   \
         (((INSTANCE) == TIM8_PWM) &&           \
           (((CHANNEL) == TIM_CHANNEL_1)   ||   \
            ((CHANNEL) == TIM_CHANNEL_2)   ||   \
            ((CHANNEL) == TIM_CHANNEL_3)   ||   \
            ((CHANNEL) == TIM_CHANNEL_4)   ||   \
            ((CHANNEL) == TIM_CHANNEL_5)   ||   \
            ((CHANNEL) == TIM_CHANNEL_6))) ||   \
         (((INSTANCE) == TIM2) &&               \
           (((CHANNEL) == TIM_CHANNEL_1)   ||   \
            ((CHANNEL) == TIM_CHANNEL_2)   ||   \
            ((CHANNEL) == TIM_CHANNEL_3)   ||   \
            ((CHANNEL) == TIM_CHANNEL_4))) ||   \
         (((INSTANCE) == TIM3) &&               \
           (((CHANNEL) == TIM_CHANNEL_1)   ||   \
            ((CHANNEL) == TIM_CHANNEL_2)   ||   \
            ((CHANNEL) == TIM_CHANNEL_3)   ||   \
            ((CHANNEL) == TIM_CHANNEL_4))) ||   \
         (((INSTANCE) == TIM4) &&               \
           (((CHANNEL) == TIM_CHANNEL_1)   ||   \
            ((CHANNEL) == TIM_CHANNEL_2)   ||   \
            ((CHANNEL) == TIM_CHANNEL_3)   ||   \
            ((CHANNEL) == TIM_CHANNEL_4))) ||   \
         (((INSTANCE) == TIM5) &&               \
           (((CHANNEL) == TIM_CHANNEL_1)   ||   \
            ((CHANNEL) == TIM_CHANNEL_2)   ||   \
            ((CHANNEL) == TIM_CHANNEL_3)   ||   \
            ((CHANNEL) == TIM_CHANNEL_4))) ||   \
         (((INSTANCE) == TIM16) &&              \
           (((CHANNEL) == TIM_CHANNEL_1))) ||   \
         (((INSTANCE) == TIM15) &&              \
           (((CHANNEL) == TIM_CHANNEL_1)   ||   \
            ((CHANNEL) == TIM_CHANNEL_2))))

/****************** TIM Instances : supporting complementary output(s) (CCxNP, CCxP, CCxE implemented if nb_of_cc>(x-1)) ********/
#define IS_TIM_CCXN_INSTANCE(INSTANCE, CHANNEL) \
   ((((INSTANCE) == TIM1_PWM) &&                \
     (((CHANNEL) == TIM_CHANNEL_1)   ||         \
      ((CHANNEL) == TIM_CHANNEL_2)   ||         \
      ((CHANNEL) == TIM_CHANNEL_3)   ||         \
      ((CHANNEL) == TIM_CHANNEL_4)   ||         \
      ((CHANNEL) == TIM_CHANNEL_5)   ||         \
      ((CHANNEL) == TIM_CHANNEL_6))) ||         \
    (((INSTANCE) == TIM8_PWM) &&                \
     (((CHANNEL) == TIM_CHANNEL_1)   ||         \
      ((CHANNEL) == TIM_CHANNEL_2)   ||         \
      ((CHANNEL) == TIM_CHANNEL_3)   ||         \
      ((CHANNEL) == TIM_CHANNEL_4)   ||         \
      ((CHANNEL) == TIM_CHANNEL_5)   ||         \
      ((CHANNEL) == TIM_CHANNEL_6))) ||         \
    (((INSTANCE) == TIM2) &&                    \
     (((CHANNEL) == TIM_CHANNEL_1)   ||         \
      ((CHANNEL) == TIM_CHANNEL_2)   ||         \
      ((CHANNEL) == TIM_CHANNEL_3)   ||         \
      ((CHANNEL) == TIM_CHANNEL_4))) ||         \
    (((INSTANCE) == TIM3) &&                    \
     (((CHANNEL) == TIM_CHANNEL_1)   ||         \
      ((CHANNEL) == TIM_CHANNEL_2)   ||         \
      ((CHANNEL) == TIM_CHANNEL_3)   ||         \
      ((CHANNEL) == TIM_CHANNEL_4))) ||         \
    (((INSTANCE) == TIM4) &&                    \
     (((CHANNEL) == TIM_CHANNEL_1)   ||         \
      ((CHANNEL) == TIM_CHANNEL_2)   ||         \
      ((CHANNEL) == TIM_CHANNEL_3)   ||         \
      ((CHANNEL) == TIM_CHANNEL_4))) ||         \
    (((INSTANCE) == TIM5) &&                    \
     (((CHANNEL) == TIM_CHANNEL_1)   ||         \
      ((CHANNEL) == TIM_CHANNEL_2)   ||         \
      ((CHANNEL) == TIM_CHANNEL_3)   ||         \
      ((CHANNEL) == TIM_CHANNEL_4))) ||         \
    (((INSTANCE) == TIM15) &&                   \
     (((CHANNEL) == TIM_CHANNEL_1)   ||         \
      ((CHANNEL) == TIM_CHANNEL_2))) ||         \
    (((INSTANCE) == TIM16) &&                   \
     (((CHANNEL) == TIM_CHANNEL_1))))

/****************** TIM Instances : supporting clock division (CKD implemented if nb_of_cc>0 and etr_impl>0) *****************/
//For me shall not be avilable on TIM15 and 16 as etr_impl=0, but evoked in RM ...
#define IS_TIM_CLOCK_DIVISION_INSTANCE(INSTANCE)   (((INSTANCE) == TIM1_PWM)    || \
                                                    ((INSTANCE) == TIM8_PWM)    || \
                                                    ((INSTANCE) == TIM2)        || \
                                                    ((INSTANCE) == TIM3)        || \
                                                    ((INSTANCE) == TIM4)        || \
                                                    ((INSTANCE) == TIM5)        || \
                                                    ((INSTANCE) == TIM15)       || \
                                                    ((INSTANCE) == TIM16))

//TODO:to review
/****** TIM Instances : supporting external clock mode 1 for ETRF input (implemented if etr_impl>0)*******/
#define IS_TIM_CLOCKSOURCE_ETRMODE1_INSTANCE(INSTANCE) (((INSTANCE) == TIM1_PWM)    || \
                                                        ((INSTANCE) == TIM8_PWM)    || \
                                                        ((INSTANCE) == TIM2)        || \
                                                        ((INSTANCE) == TIM3))

/****** TIM Instances : supporting external clock mode 2 for ETRF input (etr_impl>0) *******/
#define IS_TIM_CLOCKSOURCE_ETRMODE2_INSTANCE(INSTANCE) (((INSTANCE) == TIM1_PWM)   || \
                                                        ((INSTANCE) == TIM8_PWM)   || \
                                                        ((INSTANCE) == TIM2)       || \
                                                        ((INSTANCE) == TIM3)       || \
                                                        ((INSTANCE) == TIM4)       || \
                                                        ((INSTANCE) == TIM5))

/****************** TIM Instances : supporting external clock mode 1 for TIX inputs (see TS config in ADD) */
#define IS_TIM_CLOCKSOURCE_TIX_INSTANCE(INSTANCE)      (((INSTANCE) == TIM1_PWM)   || \
                                                        ((INSTANCE) == TIM8_PWM)   || \
                                                        ((INSTANCE) == TIM2)       || \
                                                        ((INSTANCE) == TIM3)       || \
                                                        ((INSTANCE) == TIM4)       || \
                                                        ((INSTANCE) == TIM5)       || \
                                                        ((INSTANCE) == TIM15))

/****************** TIM Instances : supporting internal trigger inputs(ITRX) (see TS config in ADD) *******/
#define IS_TIM_CLOCKSOURCE_ITRX_INSTANCE(INSTANCE)     (((INSTANCE) == TIM1_PWM)   || \
                                                        ((INSTANCE) == TIM8_PWM)   || \
                                                        ((INSTANCE) == TIM2)       || \
                                                        ((INSTANCE) == TIM3)       || \
                                                        ((INSTANCE) == TIM4)       || \
                                                        ((INSTANCE) == TIM5)       || \
                                                        ((INSTANCE) == TIM15))

/****************** TIM Instances : supporting combined 3-phase PWM mode (visibly nb_of_cc>4 as CCR5 must be implemented)******/
#define IS_TIM_COMBINED3PHASEPWM_INSTANCE(INSTANCE) (((INSTANCE) == TIM1_PWM)   || \
                                                      ((INSTANCE) == TIM8_PWM))

/****************** TIM Instances : supporting commutation event generation (CCUS/COMS, CCPC : implemented if (nb_of_cc>0 and nb_of_dt>0))***/
#define IS_TIM_COMMUTATION_EVENT_INSTANCE(INSTANCE) (((INSTANCE) == TIM1_PWM)   || \
                                                     ((INSTANCE) == TIM8_PWM)   || \
                                                     ((INSTANCE) == TIM15)      || \
                                                     ((INSTANCE) == TIM16))

/****************** TIM Instances : supporting counting mode selection (CMS - cnt_dir=2) ********/
#define IS_TIM_COUNTER_MODE_SELECT_INSTANCE(INSTANCE)  (((INSTANCE) == TIM1_PWM) || \
                                                        ((INSTANCE) == TIM8_PWM) || \
                                                        ((INSTANCE) == TIM2)     || \
                                                        ((INSTANCE) == TIM3)     || \
                                                        ((INSTANCE) == TIM4)     || \
                                                        ((INSTANCE) == TIM5))

/************* TIM Instances : supporting ETR source selection (etr_impl>0) ***************/
#define IS_TIM_ETRSEL_INSTANCE(INSTANCE)    (((INSTANCE) == TIM1_PWM) || \
                                             ((INSTANCE) == TIM8_PWM) || \
                                             ((INSTANCE) == TIM2)     || \
                                             ((INSTANCE) == TIM3)     || \
                                             ((INSTANCE) == TIM4)     || \
                                             ((INSTANCE) == TIM5))

/****** TIM Instances : Master mode available (TIMx_CR2.MMS available ) (if nb_of_sm>0) ********/
//For me TS shall not support it, as nb_of_sm=0 in ADD, but field available in RM ...
#define IS_TIM_MASTER_INSTANCE(INSTANCE)   (((INSTANCE) == TIM1_PWM) || \
                                            ((INSTANCE) == TIM8_PWM) || \
                                            ((INSTANCE) == TIM2)     || \
                                            ((INSTANCE) == TIM3)     || \
                                            ((INSTANCE) == TIM4)     || \
                                            ((INSTANCE) == TIM5)     || \
                                            ((INSTANCE) == TIM6)     || \
                                            ((INSTANCE) == TIM7)     || \
                                            ((INSTANCE) == TIM15)    || \
                                            ((INSTANCE) == TIM_TS))

/*********** TIM Instances : Slave mode available (TIMx_SMCR/SMS available if nb_of_sm>0) *******/
//For me TIM6/7 support it, as nb_of_sm=1 in ADD, but field not available in RM ...
#define IS_TIM_SLAVE_INSTANCE(INSTANCE)    (((INSTANCE) == TIM1_PWM) || \
                                            ((INSTANCE) == TIM8_PWM) || \
                                            ((INSTANCE) == TIM2)     || \
                                            ((INSTANCE) == TIM3)     || \
                                            ((INSTANCE) == TIM4)     || \
                                            ((INSTANCE) == TIM5)     || \
                                            ((INSTANCE) == TIM15))

/****************** TIM Instances : supporting OCxREF clear (OCCS : implemented if (nb_of_cc>0 and etr_impl>2))*******************/
#define IS_TIM_OCXREF_CLEAR_INSTANCE(INSTANCE)        (((INSTANCE) == TIM1_PWM) || \
                                                       ((INSTANCE) == TIM8_PWM) || \
                                                       ((INSTANCE) == TIM2)     || \
                                                       ((INSTANCE) == TIM3))

/****************** TIM Instances : supporting repetition counter (rcr_impl>0) *************/
#define IS_TIM_REPETITION_COUNTER_INSTANCE(INSTANCE)   (((INSTANCE) == TIM1_PWM) || \
                                                        ((INSTANCE) == TIM8_PWM) || \
                                                        ((INSTANCE) == TIM15)    || \
                                                        ((INSTANCE) == TIM16))

/****************** TIM Instances : supporting ADC triggering through TRGO2 (MMS2 - nb_of_sm>0 and nb_of_cc>4)***/
#define IS_TIM_TRGO2_INSTANCE(INSTANCE)    (((INSTANCE) == TIM1_PWM) || \
                                            ((INSTANCE) == TIM8_PWM))

/****************** TIM Instances : internal trigger connection ***/
//TODO: to complete
#define IS_TIM_ITR0_WITH_TIM1_INSTANCE(INSTANCE)    (((INSTANCE) == TIM8_PWM))

#define IS_TIM_ITR1_WITH_TIM2_INSTANCE(INSTANCE)    (((INSTANCE) == TIM1_PWM) || \
                                                     ((INSTANCE) == TIM8_PWM))

#define IS_TIM_ITR2_WITH_TIM3_INSTANCE(INSTANCE)    (((INSTANCE) == TIM1_PWM) || \
                                                     ((INSTANCE) == TIM8_PWM))

#define IS_TIM_ITR3_WITH_TIM4_INSTANCE(INSTANCE)    (((INSTANCE) == TIM1_PWM) || \
                                                     ((INSTANCE) == TIM8_PWM))

#define IS_TIM_ITR4_WITH_TIM5_INSTANCE(INSTANCE)    (((INSTANCE) == TIM1_PWM) || \
                                                     ((INSTANCE) == TIM8_PWM))

#define IS_TIM_ITR5_WITH_TIM8_INSTANCE(INSTANCE)    (((INSTANCE) == TIM1_PWM))

#define IS_TIM_ITR6_WITH_TIM15_INSTANCE(INSTANCE)   (((INSTANCE) == TIM1_PWM) || \
                                                     ((INSTANCE) == TIM8_PWM))

/******************* TIM Instances : Timer input XOR function (require at least TI1S, so (nb_of_cc>0 and nb_of_sm>0))*****************/
#define IS_TIM_XOR_INSTANCE(INSTANCE)      (((INSTANCE) == TIM1_PWM) || \
                                            ((INSTANCE) == TIM8_PWM) || \
                                            ((INSTANCE) == TIM2)     || \
                                            ((INSTANCE) == TIM3)     || \
                                            ((INSTANCE) == TIM4)     || \
                                            ((INSTANCE) == TIM5)     || \
                                            ((INSTANCE) == TIM15))

/************ TIM Instances : Advanced timers  ********************************/
//if supports bidirectional bkin
#define IS_TIM_ADVANCED_INSTANCE(INSTANCE) (((INSTANCE) == TIM1_PWM) || \
                                            ((INSTANCE) == TIM8_PWM) || \
                                            ((INSTANCE) == TIM15)    || \
                                            ((INSTANCE) == TIM16))

/******************** UART Instances : Asynchronous mode **********************/
#define IS_UART_INSTANCE(INSTANCE)   (((INSTANCE) == UART1) || \
                                      ((INSTANCE) == UART2) || \
                                      ((INSTANCE) == UART3))
                                      
/******************** UART Instances : Half-Duplex mode **********************/
#define IS_UART_HALFDUPLEX_INSTANCE(INSTANCE)   (((INSTANCE) == UART1) || \
                                                 ((INSTANCE) == UART2) || \
                                                 ((INSTANCE) == UART3))
                                      
/******************** UART Instances : LIN mode **********************/
#define IS_UART_LIN_INSTANCE(INSTANCE)   (((INSTANCE) == UART1) || \
                                          ((INSTANCE) == UART2) || \
                                          ((INSTANCE) == UART3))
                                          
/****************** UART Instances : Hardware Flow control ********************/
#define IS_UART_HWFLOW_INSTANCE(INSTANCE) (((INSTANCE) == UART1) || \
                                           ((INSTANCE) == UART2) || \
                                           ((INSTANCE) == UART3))

/****************** UART Instances : Auto Baud Rate detection *****************/
#define IS_UART_AUTOBAUDRATE_DETECTION_INSTANCE(INSTANCE) (((INSTANCE) == UART1) || \
                                                           ((INSTANCE) == UART2) || \
                                                           ((INSTANCE) == UART3))

/****************** UART Instances : Driver Enable ****************************/
#define IS_UART_DRIVER_ENABLE_INSTANCE(INSTANCE) (((INSTANCE) == UART1) || \
                                                  ((INSTANCE) == UART2) || \
                                                  ((INSTANCE) == UART3))
                                                  
/******************** UART Instances : FIFO mode.******************************/
#define IS_UART_FIFO_INSTANCE(INSTANCE) (((INSTANCE) == UART1) || \
                                         ((INSTANCE) == UART2) || \
                                         ((INSTANCE) == UART3))

/******************** UART Instances : Wake-up from Stop mode **********************/
/* This macro actually referres to wakeup from low-power mode.*/ 
#define IS_UART_WAKEUP_FROMSTOP_INSTANCE(INSTANCE)   (((INSTANCE) == USART1) || \
                                                      ((INSTANCE) == USART2) || \
                                                      ((INSTANCE) == USART3))

#endif /* _SR5E1_MACROS_H */

/** @} */
