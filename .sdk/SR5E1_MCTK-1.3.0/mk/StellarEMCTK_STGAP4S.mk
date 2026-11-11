################################################################################
# StellarEMCTK top level makefile.
################################################################################

C_SRCS += \
	$(STELLAR_E_MCTK_RELATIVE_PATH)/src/SmartPower/STGAP4S.c \
	$(STELLAR_E_MCTK_RELATIVE_PATH)/src/SmartPower/STGAP4S_busVoltage.c \
	$(STELLAR_E_MCTK_RELATIVE_PATH)/src/SmartPower/STGAP4S_tempSens.c \
	$(STELLAR_E_MCTK_RELATIVE_PATH)/src/SmartPower/filter.c

# C includes
C_INCS += \
	$(STELLAR_E_MCTK_RELATIVE_PATH)/include/SmartPower