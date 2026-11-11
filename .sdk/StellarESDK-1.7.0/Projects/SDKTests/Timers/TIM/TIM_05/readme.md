# StellarE SDK TIM_05

## TIM one pulse mode test

### Description: 
	TIM_05 shows how to configure and use the TIM one pulse mode. In particular,
	TIM1 and TIM2 are configured with the same frequency. The TIM2 channel 1 is
	configured as PWM active high with a duty cycle of 25%, while channel 1 of
	TIM1 is configured as PWM active low with a duty cycle of 20% in one pulse
	mode. Moreover, TIM1 is configured to be reset and triggered by the TIM2.
	So, at the beginnig of each period the channel 1 of TIM2 goes high for a
	duration of 25% and will trigger a one pulse on the channel 1 of TIM1.
	Since the channel 1 of TIM1 is configured as active low, the pulse (with
	a duration of 20%) will be generated at the end of each period.
### Supported Platforms:
-----------------------------------------------------------
#### EVBE7000P
	Requirements:
	1. Connect TP51 (LED_0) with GPIOE[13] (USER_LED_A)
	2. Use GPIOC[15] as channel 1 output of TIM1
	3. Use GPIOH[2]  as channel 1 output of TIM2
-----------------------------------------------------------