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
*   @file    mc_tasks.c
*   @version M4_VERSION
*   @brief   This file implements tasks definition.
*
*   @details This file implements tasks definition.
*
 */
  
/** @addtogroup STELLAR_PMSM_MC_APPLICATION
  * @{
  */

/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "mc_type.h"
#include "mc_math.h"
#include "motorcontrol.h"
#include "regular_conversion_manager.h"
#include "mc_interface.h"
#include "mc_tuning.h"
#include "digital_output.h"
#include "state_machine.h"
#include "pwm_common.h"

#include "mc_tasks.h"
#include "parameters_conversion.h"

#ifdef USE_L9907
#include "SRxxx_L9907.h"
#endif

#ifdef USE_L9502
#include "L9502.h"
#endif

#ifdef USE_STGAP1S
#include "gap_gate_driver_ctrl.h"
#endif

#ifdef USE_STGAP4S
#include "STGAP4S.h"
#endif

#ifdef USE_L9502
/* DEBUG options */
/* #define DEBUG_L9502_FORCE_FAULT1 */
#ifdef DEBUG_L9502_FORCE_FAULT1
   void L9502_Force_FAULT1(uint8_t enable);
   uint8_t L9502_force_fault1 = 0;
#endif /* #ifdef DEBUG_L9502_FORCE_FAULT1 */
#endif /* #ifdef USE_L9502 */

/* DEBUG variables----------------------------------------------------------*/
#if defined(MCTK_DEBUG_RPM_ANGLE)
int16_t DebugRPM = 0;
int16_t DebugAngle;

#if defined(MCTK_DEBUG_RPM_ARRAY)
  int16_t DebugRPMArray[MCTK_DEBUG_RPM_SENSOR_BUFFER_SIZE];
  uint16_t DebugRPMArrayIndex = 0;
  uint8_t mctk_debug_rpm_array_main_aux = 0;
#endif
#if (defined(AUX_STATE_OBSERVER_PLL) || defined(AUX_ENCODER) || defined(AUX_RESOLVER) || defined(AUX_HALL_SENSORS) )
  int16_t DebugRPMAux;
  int16_t DebugAngleAux;
#endif

  int16_t DebugTestEncHallDiff = 0;
  uint8_t DebugflagAngle = 0;
#endif
#if defined(MCTK_DEBUG_ANGLE_SENSOR)
int16_t DebugAngleMainArray[MCTK_DEBUG_ANGLE_SENSOR_BUFFER_SIZE];
#if (defined(AUX_STATE_OBSERVER_PLL) || defined(AUX_ENCODER) || defined(AUX_RESOLVER) || defined(AUX_HALL_SENSORS) )
int16_t DebugAngleAuxArray[MCTK_DEBUG_ANGLE_SENSOR_BUFFER_SIZE];
#endif
uint16_t DebugAngleArrayIndex = 0;
#endif

#if defined(MCTK_DEBUG_CURRENT)
int16_t DebugCurrentIa[MCTK_DEBUG_CURRENT_SIZE]; //__attribute__((section(".dtcm")));
int16_t DebugCurrentIb[MCTK_DEBUG_CURRENT_SIZE]; //__attribute__((section(".dtcm")));
uint16_t DebugCurrentIabIndex = 0;
#endif

#if (OPEN_LOOP_FOC == TRUE)
int16_t OL_Volt =  OL_VOLT;
int16_t OL_angle = OL_ANGLE;
int16_t OL_dpp = OL_DPP;
#endif

#ifdef USE_CHECK_MAX_SPEED

#define NB_SPEED_ELEMENT 16u
uint8_t index_speed_buffer = 0;
uint8_t first = 0;
uint32_t wAVRMECspeed = 0;
uint16_t check_speed_buffer[NB_SPEED_ELEMENT];

/* #define DEBUG_MAX_SPEED_BUFFER */
#if defined(DEBUG_MAX_SPEED_BUFFER)
/* average speed buffer (debug) */
uint16_t DebugAvrgSpeed[NB_SPEED_ELEMENT];
#endif
static uint16_t MC_MaxSpeed_CheckFaultState(void);
static void MC_ClearSpeedBuffer(void);
static int16_t MaxSpeedBuffer(int16_t speed);
#endif


/* USER CODE BEGIN Includes */

/* USER CODE END Includes */

/* USER CODE BEGIN Private define */
/* Private define ------------------------------------------------------------*/

#define CHARGE_BOOT_CAP_MS  10
#define CHARGE_BOOT_CAP_MS2 10
#define OFFCALIBRWAIT_MS     0
#define OFFCALIBRWAIT_MS2    0
#define STOPPERMANENCY_MS  400
#define STOPPERMANENCY_MS2 400
#define CHARGE_BOOT_CAP_TICKS  (uint16_t)((SYS_TICK_FREQUENCY * CHARGE_BOOT_CAP_MS)/ 1000)
#define CHARGE_BOOT_CAP_TICKS2 (uint16_t)((SYS_TICK_FREQUENCY * CHARGE_BOOT_CAP_MS2)/ 1000)
#define OFFCALIBRWAITTICKS     (uint16_t)((SYS_TICK_FREQUENCY * OFFCALIBRWAIT_MS)/ 1000)
#define OFFCALIBRWAITTICKS2    (uint16_t)((SYS_TICK_FREQUENCY * OFFCALIBRWAIT_MS2)/ 1000)
#define STOPPERMANENCY_TICKS   (uint16_t)((SYS_TICK_FREQUENCY * STOPPERMANENCY_MS)/ 1000)
#define STOPPERMANENCY_TICKS2  (uint16_t)((SYS_TICK_FREQUENCY * STOPPERMANENCY_MS2)/ 1000)

/* Un-Comment this macro define in order to activate the smooth
   braking action on over voltage */
/* #define  MC.SMOOTH_BRAKING_ACTION_ON_OVERVOLTAGE */

/* Private variables----------------------------------------------------------*/
FOCVars_t FOCVars[NBR_OF_MOTORS];
MCI_Handle_t Mci[NBR_OF_MOTORS];
MCI_Handle_t * oMCInterface[NBR_OF_MOTORS];
MCT_Handle_t MCT[NBR_OF_MOTORS];
STM_Handle_t STM[NBR_OF_MOTORS];
SpeednTorqCtrl_Handle_t *pSTC[NBR_OF_MOTORS];
PID_Handle_t *pPIDSpeed[NBR_OF_MOTORS];
PID_Handle_t *pPIDIq[NBR_OF_MOTORS];
PID_Handle_t *pPIDId[NBR_OF_MOTORS];
#if (defined ENCODER) || (defined AUX_ENCODER)
EncAlign_Handle_t *pEAC[NBR_OF_MOTORS];
#endif
#if (VBUS_VOLTAGE_ENABLE == TRUE)
RDivider_Handle_t *pBusSensorM1;
#else
VirtualBusVoltageSensor_Handle_t *pBusSensorM1;
#endif
#if (TEMP_VOLTAGE_ENABLE == TRUE)
NTC_Handle_t *pTemperatureSensor[NBR_OF_MOTORS];
#endif
PWMC_Handle_t * pwmcHandle[NBR_OF_MOTORS];
DOUT_handle_t *pR_Brake[NBR_OF_MOTORS];
DOUT_handle_t *pOCPDisabling[NBR_OF_MOTORS];
PQD_MotorPowMeas_Handle_t *pMPM[NBR_OF_MOTORS];
CircleLimitation_Handle_t *pCLM[NBR_OF_MOTORS];
RampExtMngr_Handle_t *pREMNG[NBR_OF_MOTORS];   /*!< Ramp manager used to modify the Iq ref
                                                    during the start-up switch over.*/

static volatile uint16_t hMFTaskCounterM1 = 0;
static volatile uint16_t hBootCapDelayCounterM1 = 0;
static volatile uint16_t hStopPermanencyCounterM1 = 0;

uint8_t bMCBootCompleted = 0;

/* USER CODE BEGIN Private Variables */

/* USER CODE END Private Variables */

/* Private functions ---------------------------------------------------------*/
void TSK_MediumFrequencyTaskM1(void);
void FOC_Clear(uint8_t bMotor);
void FOC_InitAdditionalMethods(uint8_t bMotor);
void FOC_CalcCurrRef(uint8_t bMotor);
static uint16_t FOC_CurrControllerM1(void);
void TSK_SetChargeBootCapDelayM1(uint16_t hTickCount);
bool TSK_ChargeBootCapDelayHasElapsedM1(void);
void TSK_SetStopPermanencyTimeM1(uint16_t hTickCount);
bool TSK_StopPermanencyTimeHasElapsedM1(void);
void TSK_SafetyTask_PWMOFF(uint8_t motor);
void UI_Scheduler(void);

/* USER CODE BEGIN Private Functions */

/* USER CODE END Private Functions */
/**
  * @brief  It initializes the whole MC core according to user defined
  *         parameters.
  * @param  pMCIList pointer to the vector of MCInterface objects that will be
  *         created and initialized. The vector must have length equal to the
  *         number of motor drives.
  * @param  pMCTList pointer to the vector of MCTuning objects that will be
  *         created and initialized. The vector must have length equal to the
  *         number of motor drives.
  * @retval None
  */
__weak void MCboot( MCI_Handle_t* pMCIList[],MCT_Handle_t* pMCTList[] )
{
  /* USER CODE BEGIN MCboot 0 */

  /* USER CODE END MCboot 0 */

  /**************************************/
  /*    State machine initialization    */
  /**************************************/
  STM_Init(&STM[M1]);

  bMCBootCompleted = 0;
  pCLM[M1] = &CircleLimitationM1;

  /**********************************************************/
  /*    PWM and current sensing component initialization    */
  /**********************************************************/
  pwmcHandle[M1] = &PWM_Handle_M1._Super;
  ICS_Init(&PWM_Handle_M1);
  /* USER CODE BEGIN MCboot 1 */

  /* USER CODE END MCboot 1 */

  /**************************************/
  /*    Start timers synchronously      */
  /**************************************/
  startTimers();

  /******************************************************/
  /*   PID component initialization: speed regulation   */
  /******************************************************/
  PID_HandleInit(&PIDSpeedHandle_M1);
  pPIDSpeed[M1] = &PIDSpeedHandle_M1;

  /******************************************************/
  /*   Main speed sensor component initialization       */
  /******************************************************/
  pSTC[M1] = &SpeednTorqCtrlM1;
#if (defined STATE_OBSERVER_PLL)
  STO_PLL_Init (&STO_PLL_M1);
#elif (defined ENCODER)
  ENC_Init (&ENCODER_M1);
#elif (defined RESOLVER)
  RES_Init(&Resolver_M1);
#elif (defined HALL_SENSORS)  
  HALL_Init (&HALL_M1);
#endif  

#if (defined ENCODER)
  /******************************************************/
  /*   Main encoder alignment component initialization  */
  /******************************************************/
  EAC_Init(&EncAlignCtrlM1,pSTC[M1],&VirtualSpeedSensorM1,&ENCODER_M1);
  pEAC[M1] = &EncAlignCtrlM1;
#endif
  /******************************************************/
  /*   Speed & torque component initialization          */
  /******************************************************/
#if (defined STATE_OBSERVER_PLL)
  STC_Init(pSTC[M1],pPIDSpeed[M1], &STO_PLL_M1._Super);
#elif (defined ENCODER)
  STC_Init(pSTC[M1],pPIDSpeed[M1], &ENCODER_M1._Super);
#elif (defined RESOLVER)
  STC_Init(pSTC[M1],pPIDSpeed[M1], &Resolver_M1._Super);
#elif (defined HALL_SENSORS)
  STC_Init(pSTC[M1],pPIDSpeed[M1], &HALL_M1._Super);
#endif

  /******************************************************/
  /*   Auxiliary speed sensor component initialization  */
  /******************************************************/
#if (defined AUX_STATE_OBSERVER_PLL)
  STO_PLL_Init (&STO_PLL_M1);
#endif
#if (defined AUX_ENCODER)
  /******************************************************/
  /*   Auxiliary speed sensor component initialization  */
  /******************************************************/
  ENC_Init (&ENCODER_M1);

  /***********************************************************/
  /*   Auxiliary encoder alignment component initialization  */
  /***********************************************************/
  EAC_Init(&EncAlignCtrlM1,pSTC[M1],&VirtualSpeedSensorM1,&ENCODER_M1);
  pEAC[M1] = &EncAlignCtrlM1;
#elif (defined AUX_RESOLVER)
  RES_Init(&Resolver_M1);
#elif (defined AUX_HALL_SENSORS)  
  HALL_Init (&HALL_M1);
#endif
#if (defined OBSERVER_PLL || defined AUX_STATE_OBSERVER_PLL || defined ENCODER || defined AUX_ENCODER)
  /****************************************************/
  /*   Virtual speed sensor component initialization  */
  /****************************************************/
  VSS_Init (&VirtualSpeedSensorM1);
#endif
#if (defined STATE_OBSERVER_PLL)
  /**************************************/
  /*   Rev-up component initialization  */
  /**************************************/
  RUC_Init(&RevUpControlM1,pSTC[M1],&VirtualSpeedSensorM1, &STO_M1, pwmcHandle[M1]);
#endif
  /********************************************************/
  /*   PID component initialization: current regulation   */
  /********************************************************/
  PID_HandleInit(&PIDIqHandle_M1);
  PID_HandleInit(&PIDIdHandle_M1);
  pPIDIq[M1] = &PIDIqHandle_M1;
  pPIDId[M1] = &PIDIdHandle_M1;
#if (VBUS_VOLTAGE_ENABLE == TRUE)
  /********************************************************/
  /*   Bus voltage sensor component initialization        */
  /********************************************************/
  pBusSensorM1 = &RealBusVoltageSensorParamsM1;
  RVBS_Init(pBusSensorM1);
#ifdef USE_VBUS_EXT_ADC_OUT
  ExtADC_Init(&VBUS_EXT_ADCOUT_M1);
#endif
#else
  /**********************************************************/
  /*   Virtual bus voltage sensor component initialization  */
  /**********************************************************/
  pBusSensorM1 = &VirtualBusVoltageSensorParamsM1;
  VVBS_Init(pBusSensorM1);
#endif
  /*************************************************/
  /*   Power measurement component initialization  */
  /*************************************************/
  pMPM[M1] = &PQD_MotorPowMeasM1;
  pMPM[M1]->pVBS = &(pBusSensorM1->_Super);
  pMPM[M1]->pFOCVars = &FOCVars[M1];
  
#if (TEMP_VOLTAGE_ENABLE == TRUE)
  /*******************************************************/
  /*   Temperature measurement component initialization  */
  /*******************************************************/
  NTC_Init(&TempSensorParamsM1);
#ifdef USE_TEMP_EXT_ADC_OUT
  ExtADC_Init(&TEMP_EXT_ADCOUT_M1);
#endif
  pTemperatureSensor[M1] = &TempSensorParamsM1;
#endif

  pREMNG[M1] = &RampExtMngrHFParamsM1;
  REMNG_Init(pREMNG[M1]);

  FOC_Clear(M1);
  FOCVars[M1].bDriveInput = EXTERNAL;
  FOCVars[M1].Iqdref = STC_GetDefaultIqdref(pSTC[M1]);
  FOCVars[M1].UserIdref = STC_GetDefaultIqdref(pSTC[M1]).d;
  oMCInterface[M1] = & Mci[M1];
  MCI_Init(oMCInterface[M1], &STM[M1], pSTC[M1], &FOCVars[M1] );
  MCI_ExecSpeedRamp(oMCInterface[M1],
  STC_GetMecSpeedRefUnitDefault(pSTC[M1]),0); /*First command to STC*/
  pMCIList[M1] = oMCInterface[M1];
  MCT[M1].pPIDSpeed = pPIDSpeed[M1];
  MCT[M1].pPIDIq = pPIDIq[M1];
  MCT[M1].pPIDId = pPIDId[M1];
  MCT[M1].pPIDFluxWeakening = MC_NULL; /* if M1 doesn't has FW */
  MCT[M1].pPWMnCurrFdbk = pwmcHandle[M1];
#if (defined STATE_OBSERVER_PLL)
  MCT[M1].pRevupCtrl = &RevUpControlM1;              /* only if M1 is sensorless*/
  MCT[M1].pSpeedSensorMain = (SpeednPosFdbk_Handle_t *) &STO_PLL_M1;
  MCT[M1].pSpeedSensorVirtual = &VirtualSpeedSensorM1;  /* only if M1 is sensorless*/
#elif (defined ENCODER)
  MCT[M1].pRevupCtrl = MC_NULL;              /* only if M1 is not sensorless*/
  MCT[M1].pSpeedSensorMain = (SpeednPosFdbk_Handle_t *) &ENCODER_M1;
  MCT[M1].pSpeedSensorVirtual = MC_NULL;
#elif (defined RESOLVER)
  MCT[M1].pRevupCtrl = MC_NULL;              /* only if M1 is not sensorless*/
  MCT[M1].pSpeedSensorMain = (SpeednPosFdbk_Handle_t *) &Resolver_M1;
  MCT[M1].pSpeedSensorVirtual = MC_NULL;  
#elif (defined HALL_SENSORS)
  MCT[M1].pRevupCtrl = MC_NULL;              /* only if M1 is not sensorless*/
  MCT[M1].pSpeedSensorMain = (SpeednPosFdbk_Handle_t *) &HALL_M1;  
  MCT[M1].pSpeedSensorVirtual = MC_NULL;
#endif
#if (defined AUX_STATE_OBSERVER_PLL)
  MCT[M1].pSpeedSensorAux = (SpeednPosFdbk_Handle_t *) &STO_PLL_M1;
#elif (defined AUX_ENCODER)  
  MCT[M1].pSpeedSensorAux = (SpeednPosFdbk_Handle_t *) &ENCODER_M1;
#elif (defined AUX_RESOLVER)  
  MCT[M1].pSpeedSensorAux = (SpeednPosFdbk_Handle_t *) &Resolver_M1;
#elif (defined AUX_HALL_SENSORS)    
  MCT[M1].pSpeedSensorAux = (SpeednPosFdbk_Handle_t *) &HALL_M1;
#endif
  MCT[M1].pSpeednTorqueCtrl = pSTC[M1];
  MCT[M1].pStateMachine = &STM[M1];
#if (TEMP_VOLTAGE_ENABLE == TRUE)  
  MCT[M1].pTemperatureSensor = (NTC_Handle_t *) pTemperatureSensor[M1];
#endif
  MCT[M1].pBusVoltageSensor = &(pBusSensorM1->_Super);
  MCT[M1].pBrakeDigitalOutput = MC_NULL;   /* brake is defined, oBrakeM1*/
  MCT[M1].pNTCRelay = MC_NULL;             /* relay is defined, oRelayM1*/
  MCT[M1].pMPM =  (MotorPowMeas_Handle_t*)pMPM[M1];
  MCT[M1].pFW = MC_NULL;
  MCT[M1].pFF = MC_NULL;

  MCT[M1].pPosCtrl = MC_NULL;

  MCT[M1].pSCC = MC_NULL;
  MCT[M1].pOTT = MC_NULL;
  pMCTList[M1] = &MCT[M1];

  /* USER CODE BEGIN MCboot 2 */

  /* USER CODE END MCboot 2 */
#if (defined RESOLVER) || (defined AUX_RESOLVER)
  RES_BootComplete(&Resolver_M1);
#endif

  bMCBootCompleted = 1;
}

/**
 * @brief Runs all the Tasks of the Motor Control cockpit
 *
 * This function is to be called periodically at least at the Medium Frequency task
 * rate (It is typically called on the Systick interrupt). Exact invokation rate is
 * the Speed regulator execution rate set in the Motor Contorl Workbench.
 *
 * The following tasks are executed in this order:
 *
 * - Medium Frequency Tasks of each motors
 * - Safety Task
 * - Power Factor Correction Task (if enabled)
 * - User Interface task.
 */
__weak void MC_RunMotorControlTasks(void)
{
  if ( bMCBootCompleted ) {

#ifdef USE_L9907
    L9907_Schedule();
#endif
#ifdef USE_L9502
   #if (VBUS_VOLTAGE_ENABLE == TRUE)
   #ifdef USE_VBUS_EXT_ADC_OUT
    /* trigger L9502 ADC conversion (used when PWM signals are off) */
    L9502_ADCStartConversion(VBUS_EXT_ADC_DEVICE_NB);
   #endif /* #ifdef USE_VBUS_EXT_ADC_OUT */
   #endif /* #if (VBUS_VOLTAGE_ENABLE == TRUE) */

   #if (TEMP_VOLTAGE_ENABLE == TRUE)
   #ifdef USE_TEMP_EXT_ADC_OUT
   /* trigger L9502 ADC conversion (used when PWM signals are off) */
   L9502_ADCStartConversion(TEMP_EXT_ADC_DEVICE_NB);
   #endif /* #ifdef USE_TEMP_EXT_ADC_OUT */
   #endif /* #if (TEMP_VOLTAGE_ENABLE == TRUE) */

    L9502_Schedule();
#endif
#ifdef USE_STGAP1S
  GAPSchedule(&STGAP_M1);
#endif
#ifdef USE_STGAP4S
  STGAP4S_Schedule();
#endif
    /* ** Medium Frequency Tasks ** */
    MC_Scheduler();

    /* Safety task is run after Medium Frequency task so that
     * it can overcome actions they initiated if needed. */
    TSK_SafetyTask();

    /* ** User Interface Task ** */
    UI_Scheduler();
#ifdef USE_L9502
#ifdef DEBUG_L9502_FORCE_FAULT1
    if (L9502_force_fault1 == 1)
    {
       L9502_Force_FAULT1(1);
       L9502_force_fault1 = 0;
    }
    if (L9502_force_fault1 == 2)
    {
       L9502_Force_FAULT1(0);
       L9502_force_fault1 = 0;
    }
#endif
#endif
  }
}

/**
 * @brief  Executes the Medium Frequency Task functions for each drive instance.
 *
 * It is to be clocked at the Systick frequency.
 */
__weak void MC_Scheduler(void)
{
/* USER CODE BEGIN MC_Scheduler 0 */

/* USER CODE END MC_Scheduler 0 */

  if (bMCBootCompleted == 1)
  {
    if(hMFTaskCounterM1 > 0u)
    {
      hMFTaskCounterM1--;
    }
    else
    {
#if(0)
  static uint8_t flag = 0;
  if (flag ==0)
  {
     gpio_set_pin(GPIOE, GPIOE_PIN_13);
     flag = 1;
  }
  else
  {
    gpio_clear_pin(GPIOE, GPIOE_PIN_13);
    flag = 0;
  }
#endif

      TSK_MediumFrequencyTaskM1();
      /* USER CODE BEGIN MC_Scheduler 1 */

      /* USER CODE END MC_Scheduler 1 */
      hMFTaskCounterM1 = MF_TASK_OCCURENCE_TICKS;
    }
    if(hBootCapDelayCounterM1 > 0u)
    {
      hBootCapDelayCounterM1--;
    }
    if(hStopPermanencyCounterM1 > 0u)
    {
      hStopPermanencyCounterM1--;
    }
  }
  else
  {
  }
  /* USER CODE BEGIN MC_Scheduler 2 */

  /* USER CODE END MC_Scheduler 2 */
}

/**
  * @brief Executes medium frequency periodic Motor Control tasks
  *
  * This function performs some of the control duties on Motor 1 according to the
  * present state of its state machine. In particular, duties requiring a periodic
  * execution at a medium frequency rate (such as the speed controller for instance)
  * are executed here.
  */
__weak void TSK_MediumFrequencyTaskM1(void)
{
  /* USER CODE BEGIN MediumFrequencyTask M1 0 */

  /* USER CODE END MediumFrequencyTask M1 0 */

  State_t StateM1;
  int16_t wAux = 0;
#if (defined(AUX_STATE_OBSERVER_PLL) || defined(AUX_ENCODER) || defined(AUX_RESOLVER) || defined(AUX_HALL_SENSORS) )
  int16_t wAux2 = 0;
#endif
#if (defined STATE_OBSERVER_PLL)
  (void) STO_PLL_CalcAvrgMecSpeedUnit( &STO_PLL_M1, &wAux );
#elif (defined ENCODER)
   (void) ENC_CalcAvrgMecSpeedUnit( &ENCODER_M1, &wAux );
#elif (defined RESOLVER)
   bool IsSpeedReliable = RES_GetAvrgMecSpeed01Hz( &Resolver_M1, &wAux);
#elif (defined HALL_SENSORS)
   bool IsSpeedReliable = HALL_CalcAvrgMecSpeedUnit( &HALL_M1, &wAux );
#endif

#if (defined AUX_STATE_OBSERVER_PLL)
   (void) STO_PLL_CalcAvrgMecSpeedUnit( &STO_PLL_M1, &wAux2 );
#elif (defined AUX_ENCODER)
   (void) ENC_CalcAvrgMecSpeedUnit( &ENCODER_M1, &wAux2 );
#elif (defined AUX_RESOLVER)
    RES_GetAvrgMecSpeed01Hz( &Resolver_M1, &wAux2);
#elif (defined AUX_HALL_SENSORS)    
    (void) HALL_CalcAvrgMecSpeedUnit( &HALL_M1, &wAux2 );
#endif 


#if defined(MCTK_DEBUG_RPM_ANGLE)
  DebugRPM = wAux * 6;
#if (defined(AUX_STATE_OBSERVER_PLL) || defined(AUX_ENCODER) || defined(AUX_RESOLVER) || defined(AUX_HALL_SENSORS) )
  DebugRPMAux = wAux2 * 6;
#endif
#if defined(MCTK_DEBUG_RPM_ARRAY)
  if(DebugRPMArrayIndex < MCTK_DEBUG_RPM_SENSOR_BUFFER_SIZE)
  {
      if(mctk_debug_rpm_array_main_aux == 0)
      {
         DebugRPMArray[DebugRPMArrayIndex] = DebugRPM;
      }
#if (defined( defined (AUX_STATE_OBSERVER_PLL) ||defined(AUX_ENCODER) || defined(AUX_RESOLVER) || defined(AUX_HALL_SENSORS) )
      else
      {
          DebugRPMArray[DebugRPMArrayIndex] = DebugRPMAux;
      }
#endif
    DebugRPMArrayIndex++;
  }
  if(DebugRPMArrayIndex == MCTK_DEBUG_RPM_SENSOR_BUFFER_SIZE)
  {
     DebugRPMArrayIndex = 0;
  }
#endif
#endif
  PQD_CalcElMotorPower( pMPM[M1] );

  StateM1 = STM_GetState( &STM[M1] );

  switch ( StateM1 )
  {
#if (defined DYNAMO_EN)
    case IDLE:
       gpio_clear_pin(DYNO_PIN);
     break;
#endif
#if (defined ENCODER) || (defined AUX_ENCODER)
  case IDLE:

    if ( EAC_GetRestartState( &EncAlignCtrlM1 ) )
    {
      /* The Encoder Restart State is true: the IDLE state has been entered
       * after Encoder alignment was performed. The motor can now be started. */
      EAC_SetRestartState( &EncAlignCtrlM1,false );

      /* USER CODE BEGIN MediumFrequencyTask M1 Encoder Restart */

      /* USER CODE END MediumFrequencyTask M1 Encoder Restart */

      STM_NextState( &STM[M1], IDLE_START );
    }
    break;
#endif
  case IDLE_START:
#if (defined STATE_OBSERVER_PLL)
    RUC_Clear( &RevUpControlM1, MCI_GetImposedMotorDirection( oMCInterface[M1] ) );
#endif
#if (defined ENCODER) || (defined AUX_ENCODER)
    if ( EAC_IsAligned( &EncAlignCtrlM1 ) == false )
    {
      /* The encoder is not aligned. It needs to be and the alignment procedure will make
       * the state machine go back to IDLE. Setting the Restart State to true ensures that
       * the start up procedure will carry on after alignment. */
      EAC_SetRestartState( &EncAlignCtrlM1, true );

      STM_NextState( &STM[M1], IDLE_ALIGNMENT );
      break;
    }
#endif
#if ((defined RESOLVER) ||(defined VIEW_RESOLVER_FEEDBACK))
    if(!IsSpeedReliable)
    {
      STM_FaultProcessing(&STM[M1], MC_SPEED_FDBK, 0);
      break;
    }
#endif 
    ICS_TurnOnLowSides( pwmcHandle[M1] );
    TSK_SetChargeBootCapDelayM1( CHARGE_BOOT_CAP_TICKS );
    STM_NextState( &STM[M1], CHARGE_BOOT_CAP );
    break;

  case CHARGE_BOOT_CAP:
    if ( TSK_ChargeBootCapDelayHasElapsedM1() )
    {
      PWMC_CurrentReadingCalibr( pwmcHandle[M1], CRC_START );

      /* USER CODE BEGIN MediumFrequencyTask M1 Charge BootCap elapsed */

      /* USER CODE END MediumFrequencyTask M1 Charge BootCap elapsed */

      STM_NextState(&STM[M1],OFFSET_CALIB);
    }
    break;

  case OFFSET_CALIB:
    if ( PWMC_CurrentReadingCalibr( pwmcHandle[M1], CRC_EXEC ) )
    {
      STM_NextState( &STM[M1], CLEAR );
    }
    break;

  case CLEAR:
#ifdef USE_CHECK_MAX_SPEED
     MC_ClearSpeedBuffer();
#endif
#if (defined STATE_OBSERVER_PLL)
    /* In a sensorless configuration. Initiate the Revup procedure */
    FOCVars[M1].bDriveInput = EXTERNAL;
    STC_SetSpeedSensor( pSTC[M1], &VirtualSpeedSensorM1._Super );
    STO_PLL_Clear( &STO_PLL_M1 );
#endif
#if (defined ENCODER) || (defined AUX_ENCODER)
    ENC_Clear( &ENCODER_M1 );
#endif
#if (defined RESOLVER) || (defined AUX_RESOLVER)
    RES_Clear( &Resolver_M1 );
#endif
#if (defined HALL_SENSORS) || (defined AUX_HALL_SENSORS)
    HALL_Clear( &HALL_M1 );
#endif

    if ( STM_NextState( &STM[M1], START ) == true )
    {
      FOC_Clear( M1 );

      ICS_SwitchOnPWM( pwmcHandle[M1] );
    }
    break;
#if (defined ENCODER) || (defined AUX_ENCODER)
  case IDLE_ALIGNMENT:
    ICS_TurnOnLowSides( pwmcHandle[M1] );
    TSK_SetChargeBootCapDelayM1( CHARGE_BOOT_CAP_TICKS );
    STM_NextState( &STM[M1], ALIGN_CHARGE_BOOT_CAP );
    break;

  case ALIGN_CHARGE_BOOT_CAP:
    if ( TSK_ChargeBootCapDelayHasElapsedM1() )
    {
      PWMC_CurrentReadingCalibr( pwmcHandle[M1], CRC_START );

      /* USER CODE BEGIN MediumFrequencyTask M1 Align Charge BootCap elapsed */

      /* USER CODE END MediumFrequencyTask M1 Align Charge BootCap elapsed */

      STM_NextState(&STM[M1],ALIGN_OFFSET_CALIB);
    }
    break;

  case ALIGN_OFFSET_CALIB:
    if ( PWMC_CurrentReadingCalibr( pwmcHandle[M1], CRC_EXEC ) )
    {
      STM_NextState( &STM[M1], ALIGN_CLEAR );
    }
    break;

  case ALIGN_CLEAR:
    FOCVars[M1].bDriveInput = EXTERNAL;
    STC_SetSpeedSensor( pSTC[M1], &VirtualSpeedSensorM1._Super );
    EAC_StartAlignment( &EncAlignCtrlM1 );

    if ( STM_NextState( &STM[M1], ALIGNMENT ) == true )
    {
      FOC_Clear( M1 );
      ICS_SwitchOnPWM( pwmcHandle[M1] );
    }
    break;
#endif
  case START:
    {
#if (defined STATE_OBSERVER_PLL)

      /* Mechanical speed as imposed by the Virtual Speed Sensor during the Rev Up phase. */
      int16_t hForcedMecSpeedUnit;
      qd_t IqdRef;
      bool ObserverConverged = false;

      /* Execute the Rev Up procedure */
      if( ! RUC_Exec( &RevUpControlM1 ) )
      {
#if (OPEN_LOOP_FOC == FALSE)          
        /* The time allowed for the startup sequence has expired */
        STM_FaultProcessing( &STM[M1], MC_START_UP, 0 );
#endif        
      }
      else
      {
        /* Execute the torque open loop current start-up ramp:
         * Compute the Iq reference current as configured in the Rev Up sequence */
        IqdRef.q = STC_CalcTorqueReference( pSTC[M1] );
        IqdRef.d = FOCVars[M1].UserIdref;
        /* Iqd reference current used by the High Frequency Loop to generate the PWM output */
        FOCVars[M1].Iqdref = IqdRef;
      }

      (void) VSS_CalcAvrgMecSpeedUnit( &VirtualSpeedSensorM1, &hForcedMecSpeedUnit );

      /* check that startup stage where the observer has to be used has been reached */
      if (RUC_FirstAccelerationStageReached(&RevUpControlM1) == true)
      {
        ObserverConverged = STO_PLL_IsObserverConverged( &STO_PLL_M1,hForcedMecSpeedUnit );
        STO_SetDirection(&STO_PLL_M1, MCI_GetImposedMotorDirection( &Mci[M1]));
        (void) VSS_SetStartTransition( &VirtualSpeedSensorM1, ObserverConverged );
      }

      if ( ObserverConverged )
      {
        qd_t StatorCurrent = MCM_Park( FOCVars[M1].Ialphabeta, SPD_GetElAngle( &STO_PLL_M1._Super ) );

        /* Start switch over ramp. This ramp will transition from the revup to the closed loop FOC. */
        REMNG_Init( pREMNG[M1] );
        REMNG_ExecRamp( pREMNG[M1], FOCVars[M1].Iqdref.q, 0 );
        REMNG_ExecRamp( pREMNG[M1], StatorCurrent.q, TRANSITION_DURATION );

        STM_NextState( &STM[M1], SWITCH_OVER );
      }
    }
    break;

  case SWITCH_OVER:
    {
      bool LoopClosed;
      int16_t hForcedMecSpeedUnit;

      if( ! RUC_Exec( &RevUpControlM1 ) )
      {
          /* The time allowed for the startup sequence has expired */
          STM_FaultProcessing( &STM[M1], MC_START_UP, 0 );
      }
      else
      {
        /* Compute the virtual speed and positions of the rotor.
           The function returns true if the virtual speed is in the reliability range */
        LoopClosed = VSS_CalcAvrgMecSpeedUnit(&VirtualSpeedSensorM1,&hForcedMecSpeedUnit);
        /* Check if the transition ramp has completed. */
        LoopClosed |= VSS_TransitionEnded( &VirtualSpeedSensorM1 );

        /* If any of the above conditions is true, the loop is considered closed.
           The state machine transitions to the START_RUN state. */
        if ( LoopClosed == true )
        {
          #if ( PID_SPEED_INTEGRAL_INIT_DIV == 0 )
          PID_SetIntegralTerm( pPIDSpeed[M1], 0 );
          #else
          PID_SetIntegralTerm( pPIDSpeed[M1],
                               (int32_t) ( FOCVars[M1].Iqdref.q * PID_GetKIDivisor(pPIDSpeed[M1]) /
                               PID_SPEED_INTEGRAL_INIT_DIV ) );
          #endif

          STM_NextState( &STM[M1], START_RUN );
        }
      }
    }
#else
        STM_NextState( &STM[M1], START_RUN ); /* only for sensored*/
    }
#endif
    break;
#if (defined ENCODER) || (defined AUX_ENCODER)
  case ALIGNMENT:
    if ( !EAC_Exec( &EncAlignCtrlM1 ) )
    {
      qd_t IqdRef;
      IqdRef.q = 0;
      IqdRef.d = STC_CalcTorqueReference( pSTC[M1] );
      FOCVars[M1].Iqdref = IqdRef;
    }
    else
    {
      ICS_SwitchOffPWM( pwmcHandle[M1] );
      STC_SetControlMode( pSTC[M1], STC_SPEED_MODE );
      STC_SetSpeedSensor( pSTC[M1], &ENCODER_M1._Super );

      /* USER CODE BEGIN MediumFrequencyTask M1 EndOfEncAlignment */

      /* USER CODE END MediumFrequencyTask M1 EndOfEncAlignment */

      STM_NextState( &STM[M1], ANY_STOP );
    }
    break;
#endif
  case START_RUN:
#if (defined STATE_OBSERVER_PLL)
 /* only for sensor-less control */
    STC_SetSpeedSensor(pSTC[M1], &STO_PLL_M1._Super); /*Observer has converged*/
#endif
    {
      /* USER CODE BEGIN MediumFrequencyTask M1 1 */

      /* USER CODE END MediumFrequencyTask M1 1 */
	  FOC_InitAdditionalMethods(M1);
      FOC_CalcCurrRef( M1 );
      STM_NextState( &STM[M1], RUN );
    }
    STC_ForceSpeedReferenceToCurrentSpeed( pSTC[M1] ); /* Init the reference speed to current speed */
    MCI_ExecBufferedCommands( oMCInterface[M1] ); /* Exec the speed ramp after changing of the speed sensor */

    break;

  case RUN:
    /* USER CODE BEGIN MediumFrequencyTask M1 2 */

    /* USER CODE END MediumFrequencyTask M1 2 */

    MCI_ExecBufferedCommands( oMCInterface[M1] );
    FOC_CalcCurrRef( M1 );
#if ((defined RESOLVER) ||(defined VIEW_RESOLVER_FEEDBACK) || (defined HALL_SENSORS) )
    if(!IsSpeedReliable)
    {
     STM_FaultProcessing(&STM[M1], MC_SPEED_FDBK, 0);
     break;
    }
#endif 
    /* USER CODE BEGIN MediumFrequencyTask M1 3 */

    /* USER CODE END MediumFrequencyTask M1 3 */
    break;

  case ANY_STOP:
    ICS_SwitchOffPWM( pwmcHandle[M1] );
    FOC_Clear( M1 );
    MPM_Clear( (MotorPowMeas_Handle_t*) pMPM[M1] );
    TSK_SetStopPermanencyTimeM1( STOPPERMANENCY_TICKS );

    /* USER CODE BEGIN MediumFrequencyTask M1 4 */

    /* USER CODE END MediumFrequencyTask M1 4 */

    STM_NextState( &STM[M1], STOP );
    break;

  case STOP:
    if ( TSK_StopPermanencyTimeHasElapsedM1() )
    {
      STM_NextState( &STM[M1], STOP_IDLE );
    }
    break;

  case STOP_IDLE:
#if (defined STATE_OBSERVER_PLL)
    STC_SetSpeedSensor( pSTC[M1],&VirtualSpeedSensorM1._Super );  	/*  sensor-less */
    VSS_Clear( &VirtualSpeedSensorM1 ); /* Reset measured speed in IDLE */
#endif
    /* USER CODE BEGIN MediumFrequencyTask M1 5 */

    /* USER CODE END MediumFrequencyTask M1 5 */
    STM_NextState( &STM[M1], IDLE );
    break;

  default:
    break;
  }

  /* USER CODE BEGIN MediumFrequencyTask M1 6 */

  /* USER CODE END MediumFrequencyTask M1 6 */
}

/**
  * @brief  It re-initializes the current and voltage variables. Moreover
  *         it clears qd currents PI controllers, voltage sensor and SpeednTorque
  *         controller. It must be called before each motor restart.
  *         It does not clear speed sensor.
  * @param  bMotor related motor it can be M1 or M2
  * @retval none
  */
__weak void FOC_Clear(uint8_t bMotor)
{
  /* USER CODE BEGIN FOC_Clear 0 */

  /* USER CODE END FOC_Clear 0 */
  ab_t NULL_ab = {(int16_t)0, (int16_t)0};
  qd_t NULL_qd = {(int16_t)0, (int16_t)0};
  alphabeta_t NULL_alphabeta = {(int16_t)0, (int16_t)0};

  FOCVars[bMotor].Iab = NULL_ab;
  FOCVars[bMotor].Ialphabeta = NULL_alphabeta;
  FOCVars[bMotor].Iqd = NULL_qd;
  FOCVars[bMotor].Iqdref = NULL_qd;
  FOCVars[bMotor].hTeref = (int16_t)0;
  FOCVars[bMotor].Vqd = NULL_qd;
  FOCVars[bMotor].Valphabeta = NULL_alphabeta;
  FOCVars[bMotor].hElAngle = (int16_t)0;

  PID_SetIntegralTerm(pPIDIq[bMotor], (int32_t)0);
  PID_SetIntegralTerm(pPIDId[bMotor], (int32_t)0);

  STC_Clear(pSTC[bMotor]);

  PWMC_SwitchOffPWM(pwmcHandle[bMotor]);

  /* USER CODE BEGIN FOC_Clear 1 */

  /* USER CODE END FOC_Clear 1 */
}

/**
  * @brief  Use this method to initialize additional methods (if any) in
  *         START_TO_RUN state
  * @param  bMotor related motor it can be M1 or M2
  * @retval none
  */
__weak void FOC_InitAdditionalMethods(uint8_t bMotor)
{
  (void)(bMotor); /*  added to remove warning */
  /* USER CODE BEGIN FOC_InitAdditionalMethods 0 */

  /* USER CODE END FOC_InitAdditionalMethods 0 */
}

/**
  * @brief  It computes the new values of Iqdref (current references on qd
  *         reference frame) based on the required electrical torque information
  *         provided by oTSC object (internally clocked).
  *         If implemented in the derived class it executes flux weakening and/or
  *         MTPA algorithm(s). It must be called with the periodicity specified
  *         in oTSC parameters
  * @param  bMotor related motor it can be M1 or M2
  * @retval none
  */
__weak void FOC_CalcCurrRef(uint8_t bMotor)
{

  /* USER CODE BEGIN FOC_CalcCurrRef 0 */

  /* USER CODE END FOC_CalcCurrRef 0 */
  if(FOCVars[bMotor].bDriveInput == INTERNAL)
  {
    FOCVars[bMotor].hTeref = STC_CalcTorqueReference(pSTC[bMotor]);
    FOCVars[bMotor].Iqdref.q = FOCVars[bMotor].hTeref;
    FOCVars[bMotor].Iqdref.d = STC_CalcFluxReference(pSTC[bMotor]);
  }
  /* USER CODE BEGIN FOC_CalcCurrRef 1 */

  /* USER CODE END FOC_CalcCurrRef 1 */
}

/**
  * @brief  It set a counter intended to be used for counting the delay required
  *         for drivers boot capacitors charging of motor 1
  * @param  hTickCount number of ticks to be counted
  * @retval void
  */
__weak void TSK_SetChargeBootCapDelayM1(uint16_t hTickCount)
{
   hBootCapDelayCounterM1 = hTickCount;
}

/**
  * @brief  Use this function to know whether the time required to charge boot
  *         capacitors of motor 1 has elapsed
  * @param  none
  * @retval bool true if time has elapsed, false otherwise
  */
__weak bool TSK_ChargeBootCapDelayHasElapsedM1(void)
{
  bool retVal = false;
  if (hBootCapDelayCounterM1 == 0)
  {
    retVal = true;
  }
  return (retVal);
}

/**
  * @brief  It set a counter intended to be used for counting the permanency
  *         time in STOP state of motor 1
  * @param  hTickCount number of ticks to be counted
  * @retval void
  */
__weak void TSK_SetStopPermanencyTimeM1(uint16_t hTickCount)
{
  hStopPermanencyCounterM1 = hTickCount;
}

/**
  * @brief  Use this function to know whether the permanency time in STOP state
  *         of motor 1 has elapsed
  * @param  none
  * @retval bool true if time is elapsed, false otherwise
  */
__weak bool TSK_StopPermanencyTimeHasElapsedM1(void)
{
  bool retVal = false;
  if (hStopPermanencyCounterM1 == 0)
  {
    retVal = true;
  }
  return (retVal);
}

/**
  * @brief  Executes the Motor Control duties that require a high frequency rate and a precise timing
  *
  *  This is mainly the FOC current control loop. It is executed depending on the state of the Motor Control
  * subsystem (see the state machine(s)).
  *
  * @retval Number of the  motor instance which FOC loop was executed.
  */
#if defined (MCTK_ITCM)
__weak uint8_t __attribute__((section(".itcm"))) TSK_HighFrequencyTask(void)
#else
__weak uint8_t TSK_HighFrequencyTask(void)
#endif
{
  /* USER CODE BEGIN HighFrequencyTask 0 */

  /* USER CODE END HighFrequencyTask 0 */

  uint8_t bMotorNbr = 0;
  uint16_t hFOCreturn;
#if (defined STATE_OBSERVER_PLL)
  uint16_t hState;  /*  only if sensorless main*/
  Observer_Inputs_t STO_Inputs; /*  only if sensorless main*/
#elif (defined ENCODER) 
  ENC_CalcAngle(&ENCODER_M1);   /* if not sensorless then 2nd parameter is MC_NULL*/
#elif (defined HALL_SENSORS) 
  HALL_CalcElAngle (&HALL_M1);  
#endif
#if (defined AUX_STATE_OBSERVER_PLL)
  Observer_Inputs_t STO_aux_Inputs; /*  only if sensorless aux*/
  STO_aux_Inputs.Valfa_beta = FOCVars[M1].Valphabeta;  /* only if sensorless*/
#endif
#if (defined STATE_OBSERVER_PLL)
  STO_Inputs.Valfa_beta = FOCVars[M1].Valphabeta;  /* only if sensorless*/
  if ( STM[M1].bState == SWITCH_OVER )
  {
    if (!REMNG_RampCompleted(pREMNG[M1]))
    {
      FOCVars[M1].Iqdref.q = REMNG_Calc(pREMNG[M1]);
    }
  }
#endif
#if (defined AUX_ENCODER)
   DebugAngleAux = ENC_CalcAngle(&ENCODER_M1);
#elif (defined AUX_HALL_SENSORS) 
   DebugAngleAux = HALL_CalcElAngle (&HALL_M1);  
#elif (defined AUX_RESOLVER)
   DebugAngleAux = RES_GetElAngle(&Resolver_M1);
#endif

  /* USER CODE BEGIN HighFrequencyTask SINGLEDRIVE_1 */

  /* USER CODE END HighFrequencyTask SINGLEDRIVE_1 */
  hFOCreturn = FOC_CurrControllerM1();
  /* USER CODE BEGIN HighFrequencyTask SINGLEDRIVE_2 */

  /* USER CODE END HighFrequencyTask SINGLEDRIVE_2 */
  if(hFOCreturn == MC_FOC_DURATION)
  {
    STM_FaultProcessing(&STM[M1], MC_FOC_DURATION, 0);
  }
  else
  {
#if (defined STATE_OBSERVER_PLL)
    bool IsAccelerationStageReached = RUC_FirstAccelerationStageReached(&RevUpControlM1);
    STO_Inputs.Ialfa_beta = FOCVars[M1].Ialphabeta; /*  only if sensorless*/
    STO_Inputs.Vbus = VBS_GetAvBusVoltage_d(&(pBusSensorM1->_Super)); /*  only for sensorless*/
    STO_PLL_CalcElAngle (&STO_PLL_M1, &STO_Inputs);
    STO_PLL_CalcAvrgElSpeedDpp (&STO_PLL_M1); /*  Only in case of Sensor-less */
	 if (IsAccelerationStageReached == false)
    {
      STO_ResetPLL(&STO_PLL_M1);
    }
    hState = STM_GetState(&STM[M1]);
    if((hState == START) || (hState == SWITCH_OVER) || (hState == START_RUN)) /*  only for sensor-less*/
    {
      int16_t hObsAngle = SPD_GetElAngle(&STO_PLL_M1._Super);
      VSS_CalcElAngle(&VirtualSpeedSensorM1,&hObsAngle);
    }
#elif (defined AUX_STATE_OBSERVER_PLL) 
    STO_aux_Inputs.Ialfa_beta = FOCVars[M1].Ialphabeta; /*  only if sensorless*/
    STO_aux_Inputs.Vbus = VBS_GetAvBusVoltage_d(&(pBusSensorM1->_Super)); /*  only for sensorless*/
    DebugAngleAux = STO_PLL_CalcElAngle (&STO_PLL_M1, &STO_aux_Inputs);
	STO_PLL_CalcAvrgElSpeedDpp (&STO_PLL_M1);
#endif
    /* USER CODE BEGIN HighFrequencyTask SINGLEDRIVE_3 */

    /* USER CODE END HighFrequencyTask SINGLEDRIVE_3 */
  }
  /* USER CODE BEGIN HighFrequencyTask 1 */

  /* USER CODE END HighFrequencyTask 1 */

  return bMotorNbr;
}

/**
  * @brief It executes the core of FOC drive that is the controllers for Iqd
  *        currents regulation. Reference frame transformations are carried out
  *        accordingly to the active speed sensor. It must be called periodically
  *        when new motor currents have been converted
  * @param this related object of class CFOC.
  * @retval int16_t It returns MC_NO_FAULTS if the FOC has been ended before
  *         next PWM Update event, MC_FOC_DURATION otherwise
  */
#if defined (MCTK_ITCM)
inline uint16_t __attribute__((section(".itcm"))) FOC_CurrControllerM1(void)
#else
inline uint16_t FOC_CurrControllerM1(void)
#endif
{
  qd_t Iqd, Vqd;
  ab_t Iab;
  alphabeta_t Ialphabeta, Valphabeta;

  int16_t hElAngle;
  uint16_t hCodeError;
  SpeednPosFdbk_Handle_t *speedHandle;

  speedHandle = STC_GetSpeedSensor(pSTC[M1]);
  hElAngle = SPD_GetElAngle(speedHandle);
  /* hElAngle += SPD_GetInstElSpeedDpp(speedHandle)*PARK_ANGLE_COMPENSATION_FACTOR; */
#if defined(MCTK_DEBUG_RPM_ANGLE)
  DebugAngle = hElAngle;
#endif /* #if defined(MCTK_DEBUG_RPM_ANGLE) */

#if defined(MCTK_DEBUG_ANGLE_SENSOR) && defined(MCTK_DEBUG_RPM_ANGLE)
  if(DebugAngleArrayIndex < MCTK_DEBUG_ANGLE_SENSOR_BUFFER_SIZE)
  {
    DebugAngleMainArray[DebugAngleArrayIndex] = DebugAngle;
#if (defined(AUX_STATE_OBSERVER_PLL) ||defined(AUX_ENCODER) || defined(AUX_RESOLVER) || defined(AUX_HALL_SENSORS) )
    DebugAngleAuxArray[DebugAngleArrayIndex] = DebugAngleAux;
#endif
    DebugAngleArrayIndex++;
  }
  if(DebugAngleArrayIndex == MCTK_DEBUG_ANGLE_SENSOR_BUFFER_SIZE)
  {
     DebugAngleArrayIndex = 0;
  }
#endif /* defined(MCTK_DEBUG_ANGLE_SENSOR) */

#if defined(MCTK_DEBUG_ANGLE_DIFF) && defined(MCTK_DEBUG_RPM_ANGLE)
#if ((defined(AUX_STATE_OBSERVER_PLL) || defined(AUX_ENCODER) || defined(AUX_RESOLVER) || defined(AUX_HALL_SENSORS) ))
  if(DebugflagAngle)
  {
     hElAngle = DebugAngleAux;
  }
  DebugTestEncHallDiff = DebugAngle - DebugAngleAux;
#endif
#endif /* #if defined(MCTK_DEBUG_ANGLE_DIFF) */

  PWMC_GetPhaseCurrents(pwmcHandle[M1], &Iab);
#if defined(MCTK_DEBUG_CURRENT)
  if(DebugCurrentIabIndex < MCTK_DEBUG_CURRENT_SIZE)
  {
     DebugCurrentIa[DebugCurrentIabIndex] = Iab.a;
     DebugCurrentIb[DebugCurrentIabIndex] = Iab.b;
     DebugCurrentIabIndex++;
     if(DebugCurrentIabIndex == MCTK_DEBUG_CURRENT_SIZE)
         DebugCurrentIabIndex= 0;
  }
#endif
#ifndef REG_CONV_DISABLE
  RCM_ReadOngoingConv();
  RCM_ExecNextConv();
#endif  
  Ialphabeta = MCM_Clarke(Iab);
  Iqd = MCM_Park(Ialphabeta, hElAngle);
  Vqd.q = PI_Controller(pPIDIq[M1],
            (int32_t)(FOCVars[M1].Iqdref.q) - Iqd.q);

  Vqd.d = PI_Controller(pPIDId[M1],
            (int32_t)(FOCVars[M1].Iqdref.d) - Iqd.d);

  Vqd = Circle_Limitation(pCLM[M1], Vqd);
  /* hElAngle += SPD_GetInstElSpeedDpp(speedHandle)*REV_PARK_ANGLE_COMPENSATION_FACTOR; */
#if (OPEN_LOOP_FOC == TRUE)
  Vqd.d = 0;
  Vqd.q = OL_Volt;
  hElAngle = OL_angle;
  OL_angle += OL_dpp;
#endif
  Valphabeta = MCM_Rev_Park(Vqd, hElAngle);
  hCodeError = PWMC_SetPhaseVoltage(pwmcHandle[M1], Valphabeta);
  FOCVars[M1].Vqd = Vqd;
  FOCVars[M1].Iab = Iab;
  FOCVars[M1].Ialphabeta = Ialphabeta;
  FOCVars[M1].Iqd = Iqd;
  FOCVars[M1].Valphabeta = Valphabeta;
  FOCVars[M1].hElAngle = hElAngle;
  return(hCodeError);
}

/**
  * @brief  Executes safety checks (e.g. bus voltage and temperature) for all drive instances.
  *
  * Faults flags are updated here.
  */
__weak void TSK_SafetyTask(void)
{
  /* USER CODE BEGIN TSK_SafetyTask 0 */

  /* USER CODE END TSK_SafetyTask 0 */
  if (bMCBootCompleted == 1)
  {
    TSK_SafetyTask_PWMOFF(M1);
    /* User conversion execution */
    RCM_ExecUserConv ();
  /* USER CODE BEGIN TSK_SafetyTask 1 */

  /* USER CODE END TSK_SafetyTask 1 */
  }
}

/**
  * @brief  Safety task implementation if  MC.ON_OVER_VOLTAGE == TURN_OFF_PWM
  * @param  bMotor Motor reference number defined
  *         \link Motors_reference_number here \endlink
  * @retval None
  */
__weak void TSK_SafetyTask_PWMOFF(uint8_t bMotor)
{
  /* USER CODE BEGIN TSK_SafetyTask_PWMOFF 0 */

  /* USER CODE END TSK_SafetyTask_PWMOFF 0 */

  uint16_t CodeReturn = MC_NO_ERROR;
#if (TEMP_VOLTAGE_ENABLE == TRUE)
  CodeReturn |= NTC_CalcAvTemp(pTemperatureSensor[bMotor]); /* check for fault if FW protection is activated. It returns MC_OVER_TEMP or MC_NO_ERROR */
#endif  
  CodeReturn |= PWMC_CheckOverCurrent(pwmcHandle[bMotor]);                    /* check for fault. It return MC_BREAK_IN or MC_NO_FAULTS */
#ifdef USE_L9907
  CodeReturn |= L9907_CheckFaultState(); /* Clock the External Driver Diagnostic and check for fault. It returns MC_DIAG_ERROR or MC_NO_ERROR */
#endif
#ifdef USE_L9502
  CodeReturn |= L9502_CheckFaultState(); /* Clock the External Driver Diagnostic and check for fault. It returns MC_DIAG_ERROR or MC_NO_ERROR */
#endif
#ifdef USE_STGAP1S
  CodeReturn |= GAP_CheckFaultState(); /* Clock the External Driver Diagnostic and check for fault. It returns MC_DIAG_ERROR or MC_NO_ERROR */
#endif
#ifdef USE_STGAP4S
  CodeReturn |= STGAP4S_CheckFaultState(); /* Clock the External Driver Diagnostic and check for fault. It returns MC_DIAG_ERROR or MC_NO_ERROR */
#endif

#if (VBUS_VOLTAGE_ENABLE == TRUE)
  if(bMotor == M1)
  {
    CodeReturn |=  RVBS_CalcAvVbus(pBusSensorM1);
  }
#endif
#ifdef USE_CHECK_MAX_SPEED
  CodeReturn |= MC_MaxSpeed_CheckFaultState(); /* Verify the max speed and check for fault. It returns MC_MAX_SPEED_ERROR or MC_NO_ERROR */  
#endif
  STM_FaultProcessing(&STM[bMotor], CodeReturn, ~CodeReturn); /* Update the STM according error code */
  switch (STM_GetState(&STM[bMotor])) /* Acts on PWM outputs in case of faults */
  {
  case FAULT_NOW:
#if (defined ENCODER) || (defined AUX_ENCODER)
    /* reset Encoder state */
    if (pEAC[bMotor] != MC_NULL)
    {
      EAC_SetRestartState( pEAC[bMotor], false );
    }
#endif
    PWMC_SwitchOffPWM(pwmcHandle[bMotor]);
    FOC_Clear(bMotor);
    MPM_Clear((MotorPowMeas_Handle_t*)pMPM[bMotor]);
#if (defined DYNAMO_EN)
    gpio_set_pin(DYNO_PIN);
#endif
    break;
  case FAULT_OVER:
    PWMC_SwitchOffPWM(pwmcHandle[bMotor]);
#if (defined DYNAMO_EN)
    gpio_set_pin(DYNO_PIN);
#endif
    break;
  default:
    break;
  }
  /* USER CODE BEGIN TSK_SafetyTask_PWMOFF 3 */

  /* USER CODE END TSK_SafetyTask_PWMOFF 3 */
}

/**
  * @brief  This function returns the reference of the MCInterface relative to
  *         the selected drive.
  * @param  bMotor Motor reference number defined
  *         \link Motors_reference_number here \endlink
  * @retval MCI_Handle_t * Reference to MCInterface relative to the selected drive.
  *         Note: it can be MC_NULL if MCInterface of selected drive is not
  *         allocated.
  */
__weak MCI_Handle_t * GetMCI(uint8_t bMotor)
{
  MCI_Handle_t * retVal = MC_NULL;
  if (bMotor < NBR_OF_MOTORS)
  {
    retVal = oMCInterface[bMotor];
  }
  return retVal;
}

/**
  * @brief  This function returns the reference of the MCTuning relative to
  *         the selected drive.
  * @param  bMotor Motor reference number defined
  *         \link Motors_reference_number here \endlink
  * @retval MCT_Handle_t motor control tuning handler for the selected drive.
  *         Note: it can be MC_NULL if MCInterface of selected drive is not
  *         allocated.
  */
__weak MCT_Handle_t* GetMCT(uint8_t bMotor)
{
  MCT_Handle_t* retVal = MC_NULL;
  if (bMotor < NBR_OF_MOTORS)
  {
    retVal = &MCT[bMotor];
  }
  return retVal;
}

/**
  * @brief  Puts the Motor Control subsystem in in safety conditions on a Hard Fault
  *
  *  This function is to be executed when a general hardware failure has been detected
  * by the microcontroller and is used to put the system in safety condition.
  */
__weak void TSK_HardwareFaultTask(void)
{
  /* USER CODE BEGIN TSK_HardwareFaultTask 0 */

  /* USER CODE END TSK_HardwareFaultTask 0 */

  ICS_SwitchOffPWM(pwmcHandle[M1]);
  STM_FaultProcessing(&STM[M1], MC_SW_ERROR, 0);
  /* USER CODE BEGIN TSK_HardwareFaultTask 1 */

  /* USER CODE END TSK_HardwareFaultTask 1 */
}

#ifdef USE_CHECK_MAX_SPEED
/**
  * @brief  Clear speed buffer 
  *         
*           
  * @retval None
  */
static void MC_ClearSpeedBuffer(void)
{
  uint8_t bIndex= 0;
  index_speed_buffer = 0;
  first = 0;
  wAVRMECspeed = 0;
  for(bIndex = 0; bIndex < NB_SPEED_ELEMENT; bIndex++)
  { 
     check_speed_buffer[bIndex] = 0; 
  }   
}

/**
  * @brief  It returns the speed average. The speed average is recalculated 
  *         each time by removing the oldest acquired value from the instantaneous 
*           speed buffer and inserting a new value in its place.
  * @retval uint16_t Fault code error
  */
static int16_t MaxSpeedBuffer(int16_t speed)
{

  if(index_speed_buffer>=NB_SPEED_ELEMENT) 
     index_speed_buffer = 0;
  wAVRMECspeed = wAVRMECspeed - check_speed_buffer[index_speed_buffer] + speed;
  check_speed_buffer[index_speed_buffer]=speed;
#if defined(DEBUG_MAX_SPEED_BUFFER)
  /* debug */
  DebugAvrgSpeed[index_speed_buffer] = (uint16_t)(wAVRMECspeed/NB_SPEED_ELEMENT);
#endif

  index_speed_buffer++;
  return ((uint16_t)(wAVRMECspeed/NB_SPEED_ELEMENT));
}


/**
  * @brief  It returns MC_MAX_SPEED_ERROR or MC_NO_ERROR depending on
  *         average current speed 
  * @retval uint16_t Fault code error
  */
static uint16_t MC_MaxSpeed_CheckFaultState(void)
{
  uint16_t hFault = MC_NO_ERROR;
  int16_t AVRMECspeed;
  SpeednPosFdbk_Handle_t *speedHandle;

  speedHandle = STC_GetSpeedSensor(pSTC[M1]);
  
  if(STM_GetState(&STM[M1]) == RUN)
  {
    AVRMECspeed = SPD_GetAvrgMecSpeedUnit(speedHandle);
    AVRMECspeed = (AVRMECspeed < 0 ? (-AVRMECspeed) : (AVRMECspeed));    
    
    if (first == 0)
    {
       if (index_speed_buffer < NB_SPEED_ELEMENT)
       {
          AVRMECspeed = MaxSpeedBuffer(AVRMECspeed); 
       }
       else 
       {
          first = 1;
       }
    }
    else
    {
      AVRMECspeed = MaxSpeedBuffer(AVRMECspeed); 
      if(AVRMECspeed >=(STC_GetMaxAppPositiveMecSpeedUnit(pSTC[M1])))
      {
         hFault = MC_MAX_SPEED_ERROR;
      }
    }    
  }
  return hFault;
}
#endif


/******************* (C) COPYRIGHT 2023 STMicroelectronics *****END OF FILE****/
