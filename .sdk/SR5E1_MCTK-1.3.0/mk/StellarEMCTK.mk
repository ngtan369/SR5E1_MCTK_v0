################################################################################
# StellarEMCTK top level makefile.
################################################################################

C_SRCS += \
	$(STELLAR_E_MCTK_RELATIVE_PATH)/src/MotorControl/mc_config.c \
	$(STELLAR_E_MCTK_RELATIVE_PATH)/src/MotorControl/circle_limitation.c \
	$(STELLAR_E_MCTK_RELATIVE_PATH)/src/MotorControl/bus_voltage_sensor.c \
	$(STELLAR_E_MCTK_RELATIVE_PATH)/src/MotorControl/encoder_speed_pos_fdbk.c \
	$(STELLAR_E_MCTK_RELATIVE_PATH)/src/MotorControl/enc_align_ctrl.c \
	$(STELLAR_E_MCTK_RELATIVE_PATH)/src/MotorControl/hall_speed_pos_fdbk.c \
	$(STELLAR_E_MCTK_RELATIVE_PATH)/src/MotorControl/virtual_bus_voltage_sensor.c \
	$(STELLAR_E_MCTK_RELATIVE_PATH)/src/MotorControl/ics_SR5E1_pwm_curr_fdbk.c \
	$(STELLAR_E_MCTK_RELATIVE_PATH)/src/MotorControl/inrush_current_limiter.c \
	$(STELLAR_E_MCTK_RELATIVE_PATH)/src/MotorControl/max_torque_per_ampere.c \
	$(STELLAR_E_MCTK_RELATIVE_PATH)/src/MotorControl/motor_power_measurement.c \
	$(STELLAR_E_MCTK_RELATIVE_PATH)/src/MotorControl/ntc_temperature_sensor.c \
	$(STELLAR_E_MCTK_RELATIVE_PATH)/src/MotorControl/open_loop.c \
	$(STELLAR_E_MCTK_RELATIVE_PATH)/src/MotorControl/pid_regulator.c \
	$(STELLAR_E_MCTK_RELATIVE_PATH)/src/MotorControl/pqd_motor_power_measurement.c \
	$(STELLAR_E_MCTK_RELATIVE_PATH)/src/MotorControl/pwm_common.c \
	$(STELLAR_E_MCTK_RELATIVE_PATH)/src/MotorControl/pwm_curr_fdbk.c \
	$(STELLAR_E_MCTK_RELATIVE_PATH)/src/MotorControl/ramp_ext_mngr.c \
	$(STELLAR_E_MCTK_RELATIVE_PATH)/src/MotorControl/revup_ctrl.c \
	$(STELLAR_E_MCTK_RELATIVE_PATH)/src/MotorControl/r_divider_bus_voltage_sensor.c \
	$(STELLAR_E_MCTK_RELATIVE_PATH)/src/MotorControl/speed_pos_fdbk.c \
	$(STELLAR_E_MCTK_RELATIVE_PATH)/src/MotorControl/speed_torq_ctrl.c \
	$(STELLAR_E_MCTK_RELATIVE_PATH)/src/MotorControl/state_machine.c \
	$(STELLAR_E_MCTK_RELATIVE_PATH)/src/MotorControl/sto_cordic_speed_pos_fdbk.c \
	$(STELLAR_E_MCTK_RELATIVE_PATH)/src/MotorControl/sto_pll_speed_pos_fdbk.c \
	$(STELLAR_E_MCTK_RELATIVE_PATH)/src/MotorControl/trajectory_ctrl.c \
	$(STELLAR_E_MCTK_RELATIVE_PATH)/src/MotorControl/virtual_bus_voltage_sensor.c \
	$(STELLAR_E_MCTK_RELATIVE_PATH)/src/MotorControl/virtual_speed_sensor.c \
	$(STELLAR_E_MCTK_RELATIVE_PATH)/src/MotorControl/mc_tasks.c \
	$(STELLAR_E_MCTK_RELATIVE_PATH)/src/MotorControl/motor_control_protocol.c \
	$(STELLAR_E_MCTK_RELATIVE_PATH)/src/MotorControl/motorcontrol.c \
	$(STELLAR_E_MCTK_RELATIVE_PATH)/src/MotorControl/mc_api.c \
	$(STELLAR_E_MCTK_RELATIVE_PATH)/src/MotorControl/mc_interface.c \
	$(STELLAR_E_MCTK_RELATIVE_PATH)/src/MotorControl/mc_math.c \
	$(STELLAR_E_MCTK_RELATIVE_PATH)/src/MotorControl/regular_conversion_manager.c \
	$(STELLAR_E_MCTK_RELATIVE_PATH)/src/MotorControl/sr5e1xx_mc_it.c \
	$(STELLAR_E_MCTK_RELATIVE_PATH)/src/MotorControl/PI.c \
	$(STELLAR_E_MCTK_RELATIVE_PATH)/src/MotorControl/PLL.c \
	$(STELLAR_E_MCTK_RELATIVE_PATH)/src/MotorControl/maximizer.c \
	$(STELLAR_E_MCTK_RELATIVE_PATH)/src/MotorControl/resolver_SR5E1_speed_pos_fdbk.c \
	$(STELLAR_E_MCTK_RELATIVE_PATH)/src/MotorControl/external_adc_out.c \
	$(STELLAR_E_MCTK_RELATIVE_PATH)/src/UILibrary/user_interface.c \
	$(STELLAR_E_MCTK_RELATIVE_PATH)/src/UILibrary/ui_task.c \
	$(STELLAR_E_MCTK_RELATIVE_PATH)/src/UILibrary/dac_common_ui.c \
	$(STELLAR_E_MCTK_RELATIVE_PATH)/src/UILibrary/frame_communication_protocol.c \
	$(STELLAR_E_MCTK_RELATIVE_PATH)/src/UILibrary/ui_irq_handler.c \
	$(STELLAR_E_MCTK_RELATIVE_PATH)/src/UILibrary/unidirectional_fast_com.c \
	$(STELLAR_E_MCTK_RELATIVE_PATH)/src/UILibrary/usart_frame_communication_protocol.c \
	$(STELLAR_E_MCTK_RELATIVE_PATH)/src/UILibrary/can_frame_communication_protocol.c \
	$(STELLAR_E_MCTK_RELATIVE_PATH)/src/UILibrary/dac_ui.c \
	$(STELLAR_E_MCTK_RELATIVE_PATH)/src/UILibrary/dac_rctimer_ui.c \
	$(STELLAR_E_MCTK_RELATIVE_PATH)/src/LL_Library/sr5e1_ll_exti.c \
	$(STELLAR_E_MCTK_RELATIVE_PATH)/src/LL_Library/sr5e1_ll_tim.c \
	$(STELLAR_E_MCTK_RELATIVE_PATH)/src/LL_Library/sr5e1_ll_adc.c \
	$(STELLAR_E_MCTK_RELATIVE_PATH)/src/LL_Library/sr5e1_ll_dac.c \
	$(STELLAR_E_MCTK_RELATIVE_PATH)/src/LL_Library/sr5e1_ll_cordic.c \
	$(STELLAR_E_MCTK_RELATIVE_PATH)/src/LL_Library/sr5e1_ll_usart.c \
	$(STELLAR_E_MCTK_RELATIVE_PATH)/src/LL_Library/sr5e1_ll_dma.c \
	$(STELLAR_E_MCTK_RELATIVE_PATH)/src/LL_Library/sr5e1_hal_fdcan.c	

# C includes
C_INCS += \
	$(STELLAR_E_MCTK_RELATIVE_PATH)/include/MotorControl \
	$(STELLAR_E_MCTK_RELATIVE_PATH)/include/LL_Library \
	$(STELLAR_E_MCTK_RELATIVE_PATH)/include/UILibrary \
	$(STELLAR_E_MCTK_RELATIVE_PATH)/include/SmartPower