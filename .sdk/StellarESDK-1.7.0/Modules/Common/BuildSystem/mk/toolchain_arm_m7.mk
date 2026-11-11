################################################################################
# StellarESDK ARM toolchain makefile. Cortex-M7 oriented
################################################################################
ifeq ($(TOOLCHAIN), ARM)

TOOLCHAIN_ID := GNU ARM Cross C Compiler (M7)

ifeq ($(OS),Windows_NT)
STELLAR_E_SDK_TOOLCHAIN_PATH := $(STELLAR_E_SDK_PREFIX_PATH)/Tools/Toolchain/ARM/MINGW64_NT/10.3-2021.10/bin
else
UNAME_S := $(shell uname -s)
ifeq ($(UNAME_S),Linux)
OS := Linux
STELLAR_E_SDK_TOOLCHAIN_PATH := $(STELLAR_E_SDK_PREFIX_PATH)/Tools/Toolchain/ARM/x86_64-linux/10.3-2021.10/bin
else
ifeq ($(UNAME_S),Darwin)
OS := MAC
STELLAR_E_SDK_TOOLCHAIN_PATH := $(STELLAR_E_SDK_PREFIX_PATH)/Tools/Toolchain/ARM/MAC/10.3-2021.10/bin
else
$(error $(UNAME_S) not supported yet)
endif
endif
endif

################################################################################
# Tools and tools specific options
################################################################################

# Compiler and linker tool
CC := arm-none-eabi-gcc

# C++ Compiler
CCC := arm-none-eabi-g++

# Binary image tool
GENBIN := arm-none-eabi-objcopy
BIN_OPTS := -O binary
BIN_DSEC := -j ".data"
BIN_CSEC := -R ".data"

# Intel Hexadecimal image tool
GENHEX := arm-none-eabi-objcopy
HEX_OPTS := -O ihex

# Motorola S-Record image tool
GENSREC := arm-none-eabi-objcopy
SREC_OPTS := -O srec

OUT_OPTS :=

# Size tool
SIZE := arm-none-eabi-size
SIZE_OPTS := --format=berkeley

################################################################################
# Optimization flags
################################################################################

ifeq ($(TARGET_BUILD),Release)
COMMON_OPTS := -O3
else
ifeq ($(TARGET_BUILD),Debug)
COMMON_OPTS := -O0
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

TARGET_OPTS :=

COMMON_OPTS += \
	-mthumb \
	-mcpu=cortex-m7 \
	-fmessage-length=0 \
	-fsigned-char \
	-ffunction-sections \
	-fdata-sections \
	-fomit-frame-pointer \
	-falign-functions=16 \
	-Wall \
	-Wextra \
	-Wundef \
	-g3 \
	-gdwarf-3

C_OPTS += \
	$(COMMON_OPTS) \
	-std=gnu99 \
	-Wstrict-prototypes

CPP_OPTS += \
	$(COMMON_OPTS)

A_OPTS += \
	-x assembler-with-cpp

################################################################################
# Linker options
################################################################################

L_OPTS += \
	-mcpu=cortex-m7 \
	-nostartfiles \
	-Xlinker \
	--gc-sections \
	-Wl,-Map,"$(TARGET_BUILD_DIR)/$(APP_NAME).map" \
	-Xlinker \
	--cref \
	-Wl,--no-warn-mismatch,--script="$(TARGET_BUILD_DIR)/$(LINKER_SCRIPT_NAME)" \
	--specs=rdimon.specs

LST_OPTS += -Wa,-adhlns="$(@:%.o=%.lst)"

LANG_OPTS += -x c

DEP_OPTS += -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)"
endif