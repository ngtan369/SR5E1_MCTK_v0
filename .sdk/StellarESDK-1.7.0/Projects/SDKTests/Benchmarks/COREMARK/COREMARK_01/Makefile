################################################################################
# Project makefile.
################################################################################

# 'Stellar E SDK' project common dir
STELLAR_E_SDK_RELATIVE_PATH := ../../../../..
PROJECT_COMMON_DIR := $(STELLAR_E_SDK_RELATIVE_PATH)/Projects/SDKTests/CommonBuild

include $(PROJECT_COMMON_DIR)/make/test_defs.mk

################################################################################
# Project builds
################################################################################
BUILD_DRIVERS_DMA              := 1
BUILD_DRIVERS_UART             := 1
BUILD_RUNTIME_IO               := 1

################################################################################
# Add project files
################################################################################

# Application name
APP_NAME := out

# Version
COREMARK_VERSION := 1.01

# Porting layer
COREMARK_PORTING := sdk

# C sources
C_SRCS += \
	coremark-$(COREMARK_VERSION)/core_list_join.c \
	coremark-$(COREMARK_VERSION)/core_matrix.c \
	coremark-$(COREMARK_VERSION)/core_state.c \
	coremark-$(COREMARK_VERSION)/core_util.c \
	coremark-$(COREMARK_VERSION)/core_main.c \
	coremark-$(COREMARK_VERSION)/$(COREMARK_PORTING)/core_portme.c

# C includes
C_INCS += \
	coremark-$(COREMARK_VERSION) \
	coremark-$(COREMARK_VERSION)/$(COREMARK_PORTING)

# C definitions
C_DEFS += \
	CORE_DEBUG=0 \
	COMPILER_REQUIRES_SORT_RETURN=0 \
	FLAGS_STR=\"$(CONFIG_OPT_LEVEL)\" \
	ITERATIONS=20000

# C options (optimazation for performances improvement)
ifeq ($(TOOLCHAIN), ARM)
C_OPTS += \
	-funroll-loops \
	-finline-limit=1000
endif

################################################################################
# Include 'Stellar E SDK' top level makefile
################################################################################
include $(STELLAR_E_SDK_BUILD_SYSTEM_DIR)/StellarESDK.mk

################################################################################
# Define 'Stellar E SDK' compiling target
################################################################################
all: default-all
