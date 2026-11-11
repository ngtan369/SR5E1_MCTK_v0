################################################################################
# StellarEMCTK top level makefile.
################################################################################

C_SRCS += \
	$(STELLAR_E_MCTK_RELATIVE_PATH)/src/SmartPower/L9502.c \
	$(STELLAR_E_MCTK_RELATIVE_PATH)/src/SmartPower/tabcrc.c \
	$(STELLAR_E_MCTK_RELATIVE_PATH)/src/SmartPower/L9502_busVoltage.c \
	$(STELLAR_E_MCTK_RELATIVE_PATH)/src/SmartPower/L9502_tempSens.c \
	$(STELLAR_E_MCTK_RELATIVE_PATH)/src/SmartPower/filter.c

# C includes
C_INCS += \
	$(STELLAR_E_MCTK_RELATIVE_PATH)/include/SmartPower