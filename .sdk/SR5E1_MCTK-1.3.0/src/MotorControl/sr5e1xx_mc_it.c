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
*	@file	 sr5e1xx_mc_it.c
*	@version M4_VERSION
*	@brief	 This file provides exceptions handler and peripherals interrupt
*			 service routine related to Stellar Motor Control.
*
*	@details This file provides exceptions handler and peripherals interrupt
*			 service routine related to Stellar Motor Control.
*
 */

/* Includes ------------------------------------------------------------------*/
#include <test_env.h>
#include "mc_type.h"
#include "mc_config.h"
#include "mc_tasks.h"
#include "main.h" 
#include "parameters_conversion.h"
#include "motorcontrol.h"
#include "sr5e1_ll_exti.h"
#include <platform.h>
#include "gpio.h"
#include "irq.h"
#include "irq_numbers.h"
#include "ui_task.h"
#ifdef USE_L9907
#include "SRxxx_L9907.h"
#endif
#ifdef USE_L9502
#include "L9502.h"
#endif
#ifdef USE_STGAP4S
#include "STGAP4S.h"
#endif

#ifdef CAN_COMMUNICATION
#include "sr5e1_hal_conf.h"
#include "sr5e1_hal_fdcan.h"
extern FDCAN_HandleTypeDef hfdcan1;
static volatile uint8_t msg[16];   /* global for DEBUG */
#endif

/* enable this define to test input capture functionality */
#define DYNAMO_DEBUG_INPUT_PIN

/* enable this define to verify TTL input event number */
#define DYNAMO_DEBUG_TTL_NB

#ifdef DYNAMO_EN
#ifdef DYNAMO_DEBUG_INPUT_PIN
uint8_t test_input_capture_dynamo = 0;
#endif
#ifdef DYNAMO_DEBUG_TTL_NB
uint16_t ttl_number = 0;
#endif
#endif

#if defined USE_L9502 || defined USE_STGAP4S
uint8_t Tim_break_isr_flag = 0;
#endif

/** @addtogroup STELLAR_PMSM_MC_FOC_LIBRARY
  * @{
  */

#define SYSTICK_DIVIDER (SYS_TICK_FREQUENCY/1000)

/* Public prototypes of IRQ handlers called from assembly code ---------------*/
void USART_IRQHandler(void);

/**
  * @brief	This function handles ADC1/ADC2 interrupt request.
  * @param	None
  * @retval None
  */
#if defined (MCTK_ITCM)
IRQ_HANDLER_ITCM(ADCx_HF_M1_IRQHandler)
#else
IRQ_HANDLER(ADCx_HF_M1_IRQHandler)
#endif
{
  /* Clear Flags M1 */
  LL_ADC_ClearFlag_JEOS( ADC_0_PERIPH );

#if(0)
  static uint8_t flag = 0;
  if (flag ==0)
  {
	 gpio_set_pin(B_DAC2_OUT); // MC_DEBUG_0
	 flag = 1;
  }
  else
  {
	gpio_clear_pin(B_DAC2_OUT);
	flag = 0;
  }
#endif

#if(0)
  gpio_set_pin(B_DAC2_OUT);
#endif
#if (DAC_FUNCTIONALITY == TRUE)
  UI_DACUpdate(TSK_HighFrequencyTask());
#else
  TSK_HighFrequencyTask();
#endif /* DAC_FUNCTIONALITY */  
#if(0)
  gpio_clear_pin(B_DAC2_OUT);
#endif

}

/**
  * @brief	This function handles first motor TIMx Update interrupt request.
  * @param	None
  * @retval None
  */
#if defined (MCTK_ITCM)
IRQ_HANDLER_ITCM(TIMx_UP_M1_IRQHandler)
#else
IRQ_HANDLER(TIMx_UP_M1_IRQHandler)
#endif 
{
#if(0)
  static uint8_t flag = 0;
  if (flag ==0)
  {
	 gpio_set_pin(MC_DEBUG_0);
	 flag = 1;
  }
  else
  {
	gpio_clear_pin(MC_DEBUG_0);
	flag = 0;
  }
#endif

   LL_TIM_ClearFlag_UPDATE(PWM_MODULE_SELECTION);
   ICS_TIMx_UP_IRQHandler(&PWM_Handle_M1);

}

IRQ_HANDLER(TIMx_BRK_M1_IRQHandler)
{
#if defined USE_L9502 || defined USE_STGAP4S
  Tim_break_isr_flag++;
#endif

  if (LL_TIM_IsActiveFlag_BRK(PWM_MODULE_SELECTION))
  {
   LL_TIM_ClearFlag_BRK(PWM_MODULE_SELECTION);
   ICS_BRK_IRQHandler(&PWM_Handle_M1);
#ifdef USE_L9502  
   if (Tim_break_isr_flag == 1)
   {
	  /* call fault recovery to caputure L9502 diag error (i.e. DESAT) */
	  /* this function is called just once to avoid clearing the L9502 error values */
   L9502_irqfaultRecovery();
   }
#endif	 
#ifdef USE_STGAP4S	
   /* call fault recovery to caputure STGAP4S error	 */
   STGAP4S_irqfaultRecovery();
#endif	 
  }
  if (LL_TIM_IsActiveFlag_BRK2(PWM_MODULE_SELECTION))
  {
	LL_TIM_ClearFlag_BRK2(PWM_MODULE_SELECTION);
	ICS_BRK2_IRQHandler(&PWM_Handle_M1);
#ifdef USE_STGAP4S	
   /* call fault recovery to caputure STGAP4S error	 */
   STGAP4S_irqfaultRecovery();
#endif	
#ifdef USE_L9502
   /* call fault recovery to caputure L9502 error	 */
   L9502_irqfaultRecovery();
#endif
  }
  /* MC_RunMotorControlTasks is not executed due low priority so is necessary to call scheduler here.*/
  MC_RunMotorControlTasks();

  /* Disable BRK ISR */
  LL_TIM_DisableIT_BRK(PWM_MODULE_SELECTION);

}
/**
  * @brief	This function handles TIMx global interrupt request for M1 Speed Sensor.
  * @param	None
  * @retval None
  */
#if (defined ENCODER) || (defined AUX_ENCODER)
IRQ_HANDLER(SPD_ENCODER_IRQHandler)
{

 /* Encoder Timer UPDATE IT is dynamicaly enabled/disabled, checking enable state is required */
  if (LL_TIM_IsEnabledIT_UPDATE (ENCODER_M1.TIMx) && LL_TIM_IsActiveFlag_UPDATE (ENCODER_M1.TIMx))
  {
	LL_TIM_ClearFlag_UPDATE(ENCODER_M1.TIMx);
	ENC_IRQHandler(&ENCODER_M1);
  }
  else
  {
  /* No other IT to manage for encoder config */
  }
}
#endif

/**
  * @brief	This function handles TIMx global interrupt request for M1 Speed Sensor.
  * @param	None
  * @retval None
  */
#if (defined HALL_SENSORS) || (defined AUX_HALL_SENSORS)
IRQ_HANDLER(SPD_HALL_IRQHandler)
{
  /* HALL Timer Update IT always enabled, no need to check enable UPDATE state */
  if (LL_TIM_IsActiveFlag_UPDATE(HALL_M1.TIMx) != 0)
  {
	LL_TIM_ClearFlag_UPDATE(HALL_M1.TIMx);
	HALL_TIMx_UP_IRQHandler(&HALL_M1);
  }
  else
  {
	/* Nothing to do */
  }
  /* HALL Timer CC1 IT always enabled, no need to check enable CC1 state */
  if (LL_TIM_IsActiveFlag_CC1 (HALL_M1.TIMx))
  {

	LL_TIM_ClearFlag_CC1(HALL_M1.TIMx);
	HALL_TIMx_CC_IRQHandler(&HALL_M1);
  }
  else
  {
  /* Nothing to do */
  }
}
#endif

/* This section is present only when serial communication is used */
/**
  * @brief	This function handles USART interrupt request.
  * @param	None
  * @retval None
  */
#ifdef SERIAL_COMMUNICATION
IRQ_HANDLER(USART_IRQHandler)
{

  if (LL_USART_IsActiveFlag_RXNE(pUSART.USARTx)) /* Valid data have been received */
  {
	uint16_t retVal;
	retVal = *(uint16_t*)(UFCP_RX_IRQ_Handler(&pUSART,LL_USART_ReceiveData8(pUSART.USARTx))); /* Flag 0 = RX */
	if (retVal == 1)
	{
	  UI_SerialCommunicationTimeOutStart();
	}
	if (retVal == 2)
	{
	  UI_SerialCommunicationTimeOutStop();
	}

  }

  if (LL_USART_IsActiveFlag_TXE(pUSART.USARTx))
  {
	UFCP_TX_IRQ_Handler(&pUSART);

  }

  if (LL_USART_IsActiveFlag_ORE(pUSART.USARTx)) /* Overrun error occurs */
  {
	/* Send Overrun message */
	UFCP_OVR_IRQ_Handler(&pUSART);
	LL_USART_ClearFlag_ORE(pUSART.USARTx); /* Clear overrun flag */
	UI_SerialCommunicationTimeOutStop();
  }

}
#endif

/**
  * @brief	This function handles Hard Fault exception.
  * @param	None
  * @retval None
  */
IRQ_HANDLER(HardFault_Handler)
{

  TSK_HardwareFaultTask();

  /* Go to infinite loop when Hard Fault exception occurs */
  while (1)
  {

  }
}

void osal_tick_hook(void)
{
#if(0)
  static uint8_t flag = 0;
  if (flag ==0)
  {
	 gpio_set_pin(B_DAC2_OUT); // B_DAC2_OUT
	 flag = 1;
  }
  else
  {
	gpio_clear_pin(B_DAC2_OUT); // MC_DEBUG_0
	flag = 0;
  }
#endif

  MC_RunMotorControlTasks();
#ifdef DYNAMO_EN
#ifdef DYNAMO_DEBUG_INPUT_PIN
  if (test_input_capture_dynamo)
  {
	  gpio_set_pin(DYNO_PIN);
  }
#endif
#endif
}

#ifdef DYNAMO_EN
/**
  * @brief	This function handles Button IRQ on PIN PI0.
  */
IRQ_HANDLER(IRQ_EXTI0_HANDLER)
{

  if ( LL_EXTI_ReadFlag_0_31(LL_EXTI_LINE_0) )
  {
	LL_EXTI_ClearFlag_0_31 (LL_EXTI_LINE_0);
	TTL_DYNO_INPUT_FUNC();
#ifdef DYNAMO_DEBUG_TTL_NB
	ttl_number++;
#endif
  }

}
#endif

#ifdef USE_L9907
/**
  * @brief	This function handles Button IRQ on PIN PI6.
  */
IRQ_HANDLER(IRQ_EXTI9_5_HANDLER)
{
  if ( LL_EXTI_ReadFlag_0_31(LL_EXTI_LINE_6) )
  {
	LL_EXTI_ClearFlag_0_31 (LL_EXTI_LINE_6);
	L9907_En2_Button();
  }
}
#endif

#ifdef USE_L9907
/**
  * @brief	This function handles IRQ on PIN PI2.
  */
IRQ_HANDLER(IRQ_EXTI2_HANDLER)
{
  if ( LL_EXTI_ReadFlag_0_31(LL_EXTI_LINE_2) )
  {
	LL_EXTI_ClearFlag_0_31 (LL_EXTI_LINE_2);
#ifdef USE_L9907	
	L9907_irqfaultRecovery();
#endif
  }
}
#endif

#ifdef USE_STGAP1S
/**
  * @brief	This function handles Button IRQ on PIN PH5.
  */
IRQ_HANDLER(IRQ_EXTI9_5_HANDLER)
{
  if ( LL_EXTI_ReadFlag_0_31(LL_EXTI_LINE_5) )
  {
	LL_EXTI_ClearFlag_0_31 (LL_EXTI_LINE_5);
	GAP_irqfaultRecovery();
  }
  if ( LL_EXTI_ReadFlag_0_31(LL_EXTI_LINE_6) )
  {
	LL_EXTI_ClearFlag_0_31 (LL_EXTI_LINE_6);
	GAP_SD_Button();
  }	 
}
#endif

/**
  * @brief	This function handles ADC3 interrupt request.
  * @param	None
  * @retval None
  */
#if (defined RESOLVER) || (defined AUX_RESOLVER)  
#if defined (MCTK_ITCM)
IRQ_HANDLER_ITCM(ADCx_RES_M1_IRQHandler)
#else
IRQ_HANDLER(ADCx_RES_M1_IRQHandler)
#endif
{

  /* Clear Flags M1 */
  LL_ADC_ClearFlag_JEOS( RES_ADC_MODULE_SIN );


#if(0)
  /* DEBUG PIN ADC TRIG SIN /COS (Reolver) */
  static uint8_t flag = 0;
  if (flag ==0)
  {
	 gpio_set_pin(B_DAC2_OUT); // MC_DEBUG_0
	 flag = 1;
  }
  else
  {
	gpio_clear_pin(B_DAC2_OUT);
	flag = 0;
  }
#endif

  RES_IRQHandler(&Resolver_M1);

}
#endif

#ifdef CAN_COMMUNICATION
/**
  * @brief This function handles FDCAN1 interrupt 0.
  */
IRQ_HANDLER(IRQ_CAN1_LINE0_HANDLER)
{
  uint32_t RxFifo0ITs;
  uint32_t TxEventFifoITs;
  uint16_t retVal;
  FDCAN_HandleTypeDef *hfdcan = &hfdcan1;
  static FDCAN_RxHeaderTypeDef RxHeader;
  uint8_t RxData[12];
  uint8_t count = 0;
  //TxEventFifoITs = hfdcan->Instance->IR & FDCAN_TX_EVENT_FIFO_MASK;
  TxEventFifoITs = hfdcan->Instance->IR & ((FDCAN_IR_TEFL | FDCAN_IR_TEFF | FDCAN_IR_TEFW | FDCAN_IR_TEFN));
  TxEventFifoITs &= hfdcan->Instance->IE;
  
  //RxFifo0ITs = hfdcan->Instance->IR & FDCAN_RX_FIFO0_MASK;
  RxFifo0ITs = hfdcan->Instance->IR & ((FDCAN_IR_RF0L | FDCAN_IR_RF0F | FDCAN_IR_RF0W | FDCAN_IR_RF0N));
  RxFifo0ITs &= hfdcan->Instance->IE;

  /* Rx FIFO 0 interrupts management ******************************************/
  if (RxFifo0ITs != 0U)
  {
	/* Clear the Rx FIFO 0 flags */
	__HAL_FDCAN_CLEAR_FLAG(hfdcan, RxFifo0ITs);


	/* Retrieve message from Rx FIFO 0 */
	HAL_FDCAN_GetRxMessage(hfdcan, FDCAN_RX_FIFO0, &RxHeader, RxData);

	 /* Configured Message */
	  /* 0x01 - Set register frame. */
	  /* 0x02 - Get register frame. */
	  /* 0x03 - Execute command frame. */
	  /* 0x06 - Get board info. */
	  /* 0x07 - Exec ramp.*/
	  /* 0x08 - Get revup data.*/
	  /* 0x09 - Set revup data.*/
	  /* 0x0A - Set current references.*/
	  if ((RxHeader.Identifier == 0x01U) || (RxHeader.Identifier == 0x02U) || (RxHeader.Identifier == 0x03U) ||
		  (RxHeader.Identifier == 0x06U) || (RxHeader.Identifier == 0x07U) || (RxHeader.Identifier == 0x08U) ||
		  (RxHeader.Identifier == 0x09U) || (RxHeader.Identifier == 0x0AU) )
	  {

		msg[0] = RxHeader.Identifier;		/* 1. CMD ID */
		msg[1] = RxData[0];	 /* 2. CMD Length */

		for (count=0; count < (RxData[0]); count++) {
		  msg[2 + count] = RxData[1 + count];  /* CMD Data[length]*/
		}

		msg[2 + count] = RxData[1 + count];	  /* 3. CMD CRC */

		for (count=0; count < (RxData[0] + 3); count++)
		{
			retVal = *(uint16_t*)(CFCP_RX_IRQ_Handler(&pFDCAN,msg[count])); 
			if (retVal == 1)
			{
				UI_SerialCommunicationTimeOutStart();
			}
			if (retVal == 2)
			{
				UI_SerialCommunicationTimeOutStop();
			}
		}
		/* Reset the message ID to avoid calling CAN_IRQHandlerRX with no reason */
		RxHeader.Identifier = 0;

	  } /* rxmsg.SID == Message ID */
  }

  /* Tx event FIFO interrupts management **************************************/
  if (TxEventFifoITs != 0U)
  {
	/* Clear the Tx Event FIFO flags */
	__HAL_FDCAN_CLEAR_FLAG(hfdcan, TxEventFifoITs);

  }
}
#endif

/**
  * @brief This function handles External ADC OUT interrupt
  */
#ifdef USE_VBUS_EXT_ADC_OUT	 
IRQ_HANDLER(VBUS_EXT_ADC_TIM_IRQHandler)
{
  /* If the interrupt is triggered by channel 1 */
  if (LL_TIM_IsActiveFlag_CC1(VBUS_EXT_ADCOUT_M1.TIMx) ) 
  {
	LL_TIM_ClearFlag_CC1(VBUS_EXT_ADCOUT_M1.TIMx);
	ExtADC_IRQHandler(&VBUS_EXT_ADCOUT_M1);
  }
}
#endif

/**
  * @brief This function handles External ADC OUT interrupt
  */
#ifdef USE_TEMP_EXT_ADC_OUT
IRQ_HANDLER(TEMP_EXT_ADC_TIM_IRQHandler)
{
  /* If the interrupt is triggered by channel 1 */
  if (LL_TIM_IsActiveFlag_CC1(TEMP_EXT_ADCOUT_M1.TIMx) )
  {
	LL_TIM_ClearFlag_CC1(TEMP_EXT_ADCOUT_M1.TIMx);
	ExtADC_IRQHandler(&TEMP_EXT_ADCOUT_M1);
  }
}
#endif
/**
  * @}
  */

/**
  * @}
  */
/******************* (C) COPYRIGHT 2023 STMicroelectronics *****END OF FILE****/
