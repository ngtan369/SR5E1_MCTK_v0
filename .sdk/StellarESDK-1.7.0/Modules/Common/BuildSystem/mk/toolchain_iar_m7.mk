################################################################################
# StellarESDK IAR toolchain makefile. Cortex-M7 oriented
################################################################################
ifeq ($(TOOLCHAIN), IAR)

TOOLCHAIN_ID := IAR C/C++ Compiler (M7)

# Currently STELLAR_E_SDK_TOOLCHAIN_PATH is not used since IAR compiler
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

ifeq (, $(shell which iccarm 2> /dev/null))
$(error "No IAR compiler in PATH, please add it.")
endif

################################################################################
# Tools and tools specific options
################################################################################

# Assembler tool
AS := iasmarm

# Compiler tool
CC := iccarm

# C++ Compiler tool
CCC := iccarm

# Linker tool
LD := ilinkarm

# Binary image tool
GENBIN := ielftool
BIN_OPTS := --bin

# Intel Hexadecimal image tool
GENHEX := ielftool
HEX_OPTS := --ihex

# Motorola S-Record image tool
GENSREC := ielftool
SREC_OPTS := --srec

OUT_OPTS :=

# Size tool
SIZE :=
SIZE_OPTS :=

################################################################################
# Optimization flags
################################################################################

ifeq ($(TARGET_BUILD),Release)
COMMON_OPTS += -Ohs
else
ifeq ($(TARGET_BUILD),Debug)
COMMON_OPTS += -On
else
$(error Wrong target build '$(TARGET_BUILD)')
endif
endif

################################################################################
# FPU options
################################################################################

ifeq ($(CONFIG_FPU),hard)
COMMON_OPTS += \
	--fpu=VFPv5_d16

A_OPTS += \
	--fpu VFPv5_d16
else
ifeq ($(CONFIG_FPU),softfp)
$(error Option softfp not supported)
else
ifeq ($(CONFIG_FPU),no)
COMMON_OPTS += \
	--fpu=None

A_OPTS += \
	--fpu None
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
	--thumb \
	--cpu=Cortex-M7 \
	-e \
	--debug \
	--no_size_constraints

C_OPTS += \
	$(COMMON_OPTS) \
	--use_c++_inline

CPP_OPTS += \
	$(COMMON_OPTS) \
	--c++

A_OPTS += \
	--cpu Cortex-M7

################################################################################
# Linker options
################################################################################

L_OPTS += \
	--map "$(TARGET_BUILD_DIR)/$(APP_NAME).map" \
	--config "$(TARGET_BUILD_DIR)/$(LINKER_SCRIPT_NAME)" \
	--entry __reset_handler \
	--vfe \
	--text_out locale

LST_OPTS +=

LANG_OPTS +=

DEP_OPTS +=
endif