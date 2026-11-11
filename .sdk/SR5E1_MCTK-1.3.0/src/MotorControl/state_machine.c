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
*   @file    state_machine.c
*   @version M4_VERSION
*   @brief   This file provides firmware functions that implement the features
*            of the Motor Control State Machine component of the Stellar Motor Control.
*
*   @details This file provides firmware functions that implement the features
*            of the Motor Control State Machine component of the Stellar Motor Control.
*
 */

/* Includes ------------------------------------------------------------------*/
#include "state_machine.h"
#include "mc_config.h"
/** @addtogroup STELLAR_PMSM_MC_FOC_LIBRARY
  * @{
  */

/** @defgroup STATE_MACHINE Motor Control State Machine
  * @brief Motor Control State Machine component of the Stellar Motor Control
  *
  * @todo Document the Motor Control State Machine "module".
  *
  * @{
  */

/**
  * @brief  Initializes all the object variables, usually it has to be called
  *         once right after object creation.
  * @param pHandle pointer on the component instance to initialize.
  * @retval none.
  */
__weak void STM_Init( STM_Handle_t * pHandle )
{

  pHandle->bState = IDLE;
  pHandle->hFaultNow = MC_NO_FAULTS;
  pHandle->hFaultOccurred = MC_NO_FAULTS;
}

/**
  * @brief It submits the request for moving the state machine into the state
  *        specified by bState (FAULT_NOW and FAUL_OVER are not handled by this
  *        method). Accordingly with the current state, the command is really
  *        executed (state machine set to bState) or discarded (no state
  *        changes).
  *        If requested state can't be reached the return value is false and the
  *        MC_SW_ERROR is raised, but if requested state is IDLE_START,
  *        IDLE_ALIGNMENT or ANY_STOP, that corresponds with the user actions:
  *        Start Motor, Encoder Alignemnt and Stop Motor, the MC_SW_ERROR is
  *        not raised.
  * @param pHanlde pointer of type  STM_Handle_t.
  * @param bState New requested state
  * @retval bool It returns true if the state has been really set equal to
  *         bState, false if the requested state can't be reached
  */
__weak bool STM_NextState( STM_Handle_t * pHandle, State_t bState )
{
  bool bChangeState = false;
  State_t bCurrentState = pHandle->bState;
  State_t bNewState = bCurrentState;

  switch ( bCurrentState )
  {
    case ICLWAIT:
      if ( bState == IDLE )
      {
        bNewState = bState;
        bChangeState = true;
      }
      break;
    case IDLE:
      if ( ( bState == IDLE_START ) || ( bState == IDLE_ALIGNMENT )
           || ( bState == ICLWAIT ) )
      {
        bNewState = bState;
        bChangeState = true;
      }
      break;

    case IDLE_ALIGNMENT:
      if ( ( bState == ANY_STOP ) || ( bState == ALIGN_CHARGE_BOOT_CAP )
           || ( bState == ALIGN_OFFSET_CALIB ) )
      {
        bNewState = bState;
        bChangeState = true;
      }
      break;

    case ALIGN_CHARGE_BOOT_CAP:
      if ( ( bState == ALIGN_OFFSET_CALIB ) || ( bState == ANY_STOP ) )
      {
        bNewState = bState;
        bChangeState = true;
      }
      break;

    case ALIGN_OFFSET_CALIB:
      if ( ( bState == ALIGN_CLEAR ) || ( bState == ANY_STOP ) )
      {
        bNewState = bState;
        bChangeState = true;
      }
      break;

    case ALIGN_CLEAR:
      if ( ( bState == ALIGNMENT ) || ( bState == ANY_STOP ) )
      {
        bNewState = bState;
        bChangeState = true;
      }
      break;

    case ALIGNMENT:
      if ( bState == ANY_STOP )
      {
        bNewState = bState;
        bChangeState = true;
      }
      break;

    case IDLE_START:
      if ( ( bState == ANY_STOP ) || ( bState == CHARGE_BOOT_CAP ) ||
           ( bState == START ) ||
           ( bState == OFFSET_CALIB ) || ( bState == IDLE_ALIGNMENT ) )
      {
        bNewState = bState;
        bChangeState = true;
      }
      break;

    case CHARGE_BOOT_CAP:
      if ( ( bState == OFFSET_CALIB ) || ( bState == ANY_STOP ) )
      {
        bNewState = bState;
        bChangeState = true;
      }
      break;

    case OFFSET_CALIB:
      if ( ( bState == CLEAR ) || ( bState == ANY_STOP ) || ( bState == WAIT_STOP_MOTOR ) )
      {
        bNewState = bState;
        bChangeState = true;
      }
      break;

     case WAIT_STOP_MOTOR:
      if ( ( bState == CLEAR ) || ( bState == ANY_STOP ) )
      {
        bNewState = bState;
        bChangeState = true;
      }
      break;

    case CLEAR:
      if ( ( bState == START ) || ( bState == ANY_STOP ) )
      {
        bNewState = bState;
        bChangeState = true;
      }
      break;

    case START:
      if ( ( bState == SWITCH_OVER ) || ( bState == ANY_STOP ) || (bState == START_RUN) )
      {
        bNewState = bState;
        bChangeState = true;
      }
      break;

    case SWITCH_OVER:
      if ( ( bState == START ) || ( bState == ANY_STOP ) || (bState == START_RUN) )
      {
        bNewState = bState;
        bChangeState = true;
      }
      break;

    case START_RUN:
      if ( ( bState == RUN ) || ( bState == ANY_STOP ) )
      {
        bNewState = bState;
        bChangeState = true;
      }
      break;

    case RUN:
      if ( bState == ANY_STOP )
      {
        bNewState = bState;
        bChangeState = true;
      }
      break;

    case ANY_STOP:
      if ( bState == STOP )
      {
        bNewState = bState;
        bChangeState = true;
      }
      break;

    case STOP:
      if ( bState == STOP_IDLE )
      {
        bNewState = bState;
        bChangeState = true;
      }
      break;

    case STOP_IDLE:
      if ( ( bState == IDLE ) || ( bState == ICLWAIT ) )
      {
        bNewState = bState;
        bChangeState = true;
      }
      break;
    default:
      break;
  }

  if ( bChangeState )
  {
    pHandle->bState = bNewState;
  }
  else
  {
    if ( !( ( bState == IDLE_START ) || ( bState == IDLE_ALIGNMENT )
            || ( bState == ANY_STOP ) ) )
    {
      /* If new state is not a user command START/STOP raise a software error */
      STM_FaultProcessing( pHandle, MC_SW_ERROR, 0u );
    }
  }

  return ( bChangeState );
}

/**
  * @brief It clocks both HW and SW faults processing and update the state
  *        machine accordingly with hSetErrors, hResetErrors and present state.
  *        Refer to State_t description for more information about fault states.
  * @param pHanlde pointer of type  STM_Handle_t
  * @param hSetErrors Bit field reporting faults currently present
  * @param hResetErrors Bit field reporting faults to be cleared
  * @retval State_t New state machine state after fault processing
  */
__weak State_t STM_FaultProcessing( STM_Handle_t * pHandle, uint16_t hSetErrors, uint16_t
                             hResetErrors )
{
  State_t LocalState =  pHandle->bState;

  /* Set current errors */
  pHandle->hFaultNow = ( pHandle->hFaultNow | hSetErrors ) & ( ~hResetErrors );
  pHandle->hFaultOccurred |= hSetErrors;

  if ( LocalState == FAULT_NOW )
  {
    if ( pHandle->hFaultNow == MC_NO_FAULTS )
    {
      pHandle->bState = FAULT_OVER;
      LocalState = FAULT_OVER;
    }
  }
  else
  {
    if ( pHandle->hFaultNow != MC_NO_FAULTS )
    {
      pHandle->bState = FAULT_NOW;
      LocalState = FAULT_NOW;
    }
  }

  return ( LocalState );
}

/**
  * @brief  Returns the current state machine state
  * @param  pHanlde pointer of type  STM_Handle_t
  * @retval State_t Current state machine state
  */
#if defined (MCTK_ITCM)
__weak State_t __attribute__((section(".itcm"))) STM_GetState( STM_Handle_t * pHandle )
#else
__weak State_t STM_GetState( STM_Handle_t * pHandle )
#endif
{
  return ( pHandle->bState );
}

/**
  * @brief It reports to the state machine that the fault state has been
  *        acknowledged by the user. If the state machine is in FAULT_OVER state
  *        then it is moved into STOP_IDLE and the bit field variable containing
  *        information about the faults historically occured is cleared.
  *        The method call is discarded if the state machine is not in FAULT_OVER
  * @param pHanlde pointer of type  STM_Handle_t
  * @retval bool true if the state machine has been moved to IDLE, false if the
  *        method call had no effects
  */
__weak bool STM_FaultAcknowledged( STM_Handle_t * pHandle )
{
  bool bToBeReturned = false;

  if ( pHandle->bState == FAULT_OVER )
  {
    pHandle->bState = STOP_IDLE;
    pHandle->hFaultOccurred = MC_NO_FAULTS;
    bToBeReturned = true;
  }

  return ( bToBeReturned );
}


/**
  * @brief It returns two 16 bit fields containing information about both faults
  *        currently present and faults historically occurred since the state
  *        machine has been moved into state
  * @param pHanlde pointer of type  STM_Handle_t.
  * @retval uint32_t  Two 16 bit fields: in the most significant half are stored
  *         the information about currently present faults. In the least
  *         significant half are stored the information about the faults
  *         historically occurred since the state machine has been moved into
  *         FAULT_NOW state
  */
__weak uint32_t STM_GetFaultState( STM_Handle_t * pHandle )
{
  uint32_t LocalFaultState;

  LocalFaultState = ( uint32_t )( pHandle->hFaultOccurred );
  LocalFaultState |= ( uint32_t )( pHandle->hFaultNow ) << 16;

  return LocalFaultState;
}

#ifdef USE_L9907
#include "SRxxx_L9907.h"
#endif

#ifdef USE_L9502
#include "L9502.h"
#endif

#ifdef USE_STGAP4S
#include "STGAP4S.h"
#endif

#ifdef USE_L9907
uint32_t STM_GetExternalFault(uint8_t regId,  uint8_t *buff)
{
  (void) buff; /* unused */
  uint32_t ret_val = 0;

  L9907_Errors* pDiagErrors = L9907_getDiagRegisters();
  //L9907_Errors diag_errors = L9907_Diag_Errors;
  if (regId == 0)
  {
    ret_val = (pDiagErrors->wL9907_ErrorsNow & 0xFFFF0000) >> 16; /* DIAG */
  }
  else if (regId == 1)
  {
    ret_val = (pDiagErrors->wL9907_ErrorsNow & 0x0000FFFF);       /* DIAG2 */
  }
  else
  {
    /* do nothing */
  }

  return ret_val;
}
#endif

#ifdef USE_STGAP1S
uint32_t STM_GetExternalFault(uint8_t regId,  uint8_t *buff)
{
    (void) buff; /* unused */
    uint32_t ret_val = 0;

    uint32_t* pDiagErrors = GAP_GetStatusRegisters();
    ret_val = pDiagErrors[regId];

    return ret_val;
}
#endif

#ifdef USE_L9502
uint32_t STM_GetExternalFault(uint8_t regId, uint8_t *buff)
{
    uint32_t ret_val = 0;

    uint16_t index;
    uint16_t value;
    for (index = 0; index < L9502_DEVICE_NUM; index++)
    {
        /* regId is used to access 4 FHM register + CFG_7 for device */
        L9502_statusReg_t* pDiagErrors = L9502_GetStatusRegisters(index);
        value = (uint32_t)pDiagErrors->StatRegs[regId];

        //SWAP_ENDIAN_32(value);
        buff[2*index]   = (value & 0xFF00)>>8;  /* MSB on even index */
        buff[2*index+1] = (value & 0x00FF);
    }
    ret_val = (index * 2); /* 16 bit per number of device L9502_DEVICE_NUM */

    return ret_val;
}
#endif

#ifdef USE_STGAP4S
uint32_t STM_GetExternalFault(uint8_t regId, uint8_t *buff)
{
    uint32_t ret_val = 0;

    uint16_t index;
    uint16_t value;
    for (index = 0; index < NUMBER_DEVICES; index++)
    {
        STGAP4S_statusReg_t* pDiagErrors = STGAP4S_GetStatusRegisters(index);
        value = (uint32_t)pDiagErrors->StatRegs[regId];

        //SWAP_ENDIAN_32(value);
        buff[2*index]   = (value & 0xFF00)>>8;  /* MSB on even index */
        buff[2*index+1] = (value & 0x00FF);
    }
    ret_val = (index * 2); /* 16 bit per number of device L9502_DEVICE_NUM */
    return ret_val;
}
#endif
/**
  * @}
  */

/**
  * @}
  */

/******************* (C) COPYRIGHT 2023 STMicroelectronics *****END OF FILE****/
