################################################################################
# StellarESDK ARMCLANG toolchain makefile. Cortex-M7 oriented
################################################################################
ifeq ($(TOOLCHAIN), ARMCLANG)

TOOLCHAIN_ID := Arm Clang Cross C Compiler (M7)

# Currently STELLAR_E_SDK_TOOLCHAIN_PATH is not used since Arm Clang compiler
# path is stored in the environment variables during compiler installation.
ifeq ($(OS),Windows_NT)
STELLAR_E_SDK_TOOLCHAIN_PATH :=
else
UNAME_S := $(shell uname -s)
ifeq ($(UNAME_S),Linux)
OS := Linux
STELLAR_E_SDK_TOOLCHAIN_PATH :=
else
ifeq ($(UNAME_S),Darwin)
OS := MAC
STELLAR_E_SDK_TOOLCHAIN_PATH :=
else
$(error $(UNAME_S) not supported yet)
endif
endif
endif

ifeq (, $(shell which armclang 2> /dev/null))
$(error "No IAR compiler in PATH, please add it.")
endif

################################################################################
# Tools and tools specific options
################################################################################

# Assembler tool
AS := armasm

# Compiler tool
CC := armclang

# C++ Compiler tool
CCC := armclang

# Linker tool
LD := armlink

# Binary image tool
GENBIN := fromelf
BIN_OPTS := --bincombined

# Intel Hexadecimal image tool
GENHEX := fromelf
HEX_OPTS := --i32combined

# Motorola S-Record image tool
GENSREC := fromelf
SREC_OPTS := --m32combined

OUT_OPTS := --output

# Size tool
SIZE := fromelf
SIZE_OPTS := --text -z

################################################################################
# Optimization flags
################################################################################

ifeq ($(TARGET_BUILD),Release)
COMMON_OPTS += -O3
else
ifeq ($(TARGET_BUILD),Debug)
COMMON_OPTS += -O0
else
$(error Wrong target build '$(TARGET_BUILD)')
endif
endif

################################################################################
# FPU options
################################################################################

ifeq ($(CONFIG_FPU),hard)
COMMON_OPTS += \
	-mfloat-abi=hard \
	-mfpu=fpv5-d16

A_OPTS += \
	-DUSE_FPU
else
ifeq ($(CONFIG_FPU),softfp)
COMMON_OPTS += \
	-mfloat-abi=softfp \
	-mfpu=fpv5-d16

A_OPTS += \
	-DUSE_FPU
else
ifeq ($(CONFIG_FPU),no)
else
$(error Wrong FPU configuration '$(CONFIG_FPU)')
endif
endif
endif

################################################################################
# Compiler options
################################################################################

TARGET_OPTS += --target=arm-arm-none-eabi -mcpu=cortex-m7

COMMON_OPTS += \
	-mthumb \
	-mcpu=cortex-m7 \
	--target=arm-arm-none-eabi \
	-fno-function-sections \
	-Wno-implicit-function-declaration \
	-Wno-implicit-int \
	-fno-common \
	-ffp-mode=fast \
	-g

C_OPTS += \
	$(COMMON_OPTS)

CPP_OPTS += \
	$(COMMON_OPTS)

A_OPTS +=

################################################################################
# Linker options
################################################################################

L_OPTS += \
	--map \
	--list="$(TARGET_BUILD_DIR)/$(APP_NAME).map" \
	--info=sizes,totals \
	--entry=Reset_Handler \
	--scatter "$(TARGET_BUILD_DIR)/$(LINKER_SCRIPT_NAME)" \
	--lto

LST_OPTS +=

LANG_OPTS += -x c

DEP_OPTS +=
endif