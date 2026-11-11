################################################################################
# StellarESDK HighTec toolchain makefile. Cortex-M7 oriented
################################################################################
ifeq ($(TOOLCHAIN), HIGHTEC)

TOOLCHAIN_ID := HighTec Cross C Compiler (M7)

# Currently STELLAR_E_SDK_TOOLCHAIN_PATH is not used since HighTec compiler
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

ifeq (, $(shell which clang 2> /dev/null))
$(error "No HighTec compiler in PATH, please add it.")
endif

################################################################################
# Tools and tools specific options
################################################################################

# Compiler and linker tool
CC := clang

# C++ Compiler
CCC := clang

# Binary image tool
GENBIN := llvm-objcopy
BIN_OPTS := -O binary
BIN_DSEC := -j ".data"
BIN_CSEC := -R ".data"

# Intel Hexadecimal image tool
GENHEX := llvm-objcopy
HEX_OPTS := -O ihex

# Motorola S-Record image tool
GENSREC := aarch32-objcopy
SREC_OPTS := -O srec

OUT_OPTS :=

# Size tool
SIZE := aarch32-size
SIZE_OPTS := --format=berkeley

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

TARGET_OPTS := --target=armv7-m-none-eabi

COMMON_OPTS += \
	-mthumb \
	-march=armv7e-m \
	-mcpu=cortex-m7 \
	-fmessage-length=0 \
	-fsigned-char \
	-ffunction-sections \
	-fdata-sections \
	-fomit-frame-pointer \
	-falign-functions=16 \
	-fno-common \
	-Wall \
	-Wextra \
	-Wundef \
	-Wstrict-prototypes \
	-g3 \
	-gdwarf-3 \
	-fno-exceptions

C_OPTS += \
	$(COMMON_OPTS) \
	-std=gnu99

CPP_OPTS += \
	$(COMMON_OPTS) \
	-std=c++17

A_OPTS += \
	-x assembler-with-cpp

################################################################################
# Linker options
################################################################################

L_OPTS += \
	-Xlinker \
	--gc-sections \
	-Wl,-Map,"$(TARGET_BUILD_DIR)/$(APP_NAME).map" \
	-Xlinker \
	--cref \
	-Wl,--no-warn-mismatch,--script="$(TARGET_BUILD_DIR)/$(LINKER_SCRIPT_NAME)"

LST_OPTS +=

LANG_OPTS += -x c

DEP_OPTS += -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)"
endif