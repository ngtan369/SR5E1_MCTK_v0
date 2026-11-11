################################################################################
# Project makefile.
################################################################################

# 'Stellar SDK' project values
PROJECTNAME := SR5E1_MCTK_TTM01V1_Demo
SDKID := StellarESDK-1.7.0
MCTKID := SR5E1_MCTK-1.3.0
STELLAR_E_SDK_RELATIVE_PATH := .sdk/$(SDKID)
STELLAR_E_MCTK_RELATIVE_PATH := .sdk/$(MCTKID)
PROJECT_COMMON_DIR := $(STELLAR_E_SDK_RELATIVE_PATH)/Projects/SDKTests/CommonBuild

CONFIG_BOARD = evbe7000s
CONFIG_TARGET_MEMORY = flash
TARGET_BUILD = Debug
#TOOLCHAIN=IAR
#TOOLCHAIN=HIGHTEC
include $(PROJECT_COMMON_DIR)/make/test_defs.mk

################################################################################
# Project builds
################################################################################
BUILD_OS_OSAL                  := 1
BUILD_DRIVERS_SPI              := 1
BUILD_DRIVERS_DWT              := 0
BUILD_DRIVERS_DMA              := 1
BUILD_ADDONS_LL                := 0

BUILD_L9907                    := 0
BUILD_L9502                    := 0
BUILD_STGAP4S                  := 0
################################################################################
# Add project files
################################################################################

# Application name
APP_NAME := $(PROJECTNAME)

# C sources
C_SRCS += \
	src/main.c \
	src/SRxxx_L9907_cfg.c \
	src/anti_rollback.c
# C includes
C_INCS += \
	include \
	src-gen \
	src-gen/sr5e1	
# C includes
# C_INCS += \
# 	src-gen/ \
# 	src-gen/<device-name>

################################################################################
# Include 'Stellar SDK' top level makefile
################################################################################
include $(STELLAR_E_MCTK_RELATIVE_PATH)/mk/StellarEMCTK.mk
ifeq ($(BUILD_L9502), 1)
include $(STELLAR_E_MCTK_RELATIVE_PATH)/mk/StellarEMCTK_L9502.mk
endif
ifeq ($(BUILD_L9907), 1)
include $(STELLAR_E_MCTK_RELATIVE_PATH)/mk/StellarEMCTK_L9907.mk
endif
include $(STELLAR_E_SDK_BUILD_SYSTEM_DIR)/StellarESDK.mk

################################################################################
# Define 'Stellar E SDK' compiling target
################################################################################
all: default-all
