################################################################################
# StellarESDK GHS toolchain makefile. Cortex-M7 oriented
################################################################################
ifeq ($(TOOLCHAIN), GHS)

TOOLCHAIN_ID := GHS Cross C Compiler (M7)

# Currently STELLAR_E_SDK_TOOLCHAIN_PATH is not used since GHS compiler
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

ifeq (, $(shell which ccarm 2> /dev/null))
$(error "No GHS compiler in PATH, please add it.")
endif

################################################################################
# Tools and tools specific options
################################################################################

# Compiler and linker tool
# (cxarm is required for projects with at least some C++ code)
ifeq ($(CPP_SRCS),)
CC := ccarm
else
CC := cxarm
endif

# C++ Compiler
CCC := cxarm

# Binary image tool
GENBIN := gmemfile -w
BIN_OPTS :=
BIN_DSEC := -start ".data" -end ".data"
BIN_CSEC := -skip ".data"

# Intel Hexadecimal image tool
GENHEX := gsrec
HEX_OPTS := -hex386

# Motorola S-Record image tool
GENSREC := gsrec
SREC_OPTS :=

OUT_OPTS := -o

# Size tool
SIZE := gsize
SIZE_OPTS := -gcompare -totals

################################################################################
# Optimization flags
################################################################################

ifeq ($(TARGET_BUILD),Release)
COMMON_OPTS += -Ospeed
else
ifeq ($(TARGET_BUILD),Debug)
COMMON_OPTS += -Odebug
else
$(error Wrong target build '$(TARGET_BUILD)')
endif
endif

################################################################################
# FPU options
################################################################################

ifeq ($(CONFIG_FPU),hard)
COMMON_OPTS += \
	-fhard \
	-fpu=vfpv5_d16

A_OPTS += \
	-DUSE_FPU
else
ifeq ($(CONFIG_FPU),softfp)
COMMON_OPTS += \
	-fsoft \
	-fpu=vfpv5_d16

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
	-thumb \
	-littleendian \
	-cpu=cortexm7 \
	--signed_chars \
	--gnu_asm \
	--warnings \
	-g \
	-dwarf2 \
	--commons

C_OPTS += \
	$(COMMON_OPTS)

CPP_OPTS += \
	$(COMMON_OPTS) \
	--restrict \
	-w

A_OPTS += \
	-preprocess_assembly_files \
	--gnu_asm

################################################################################
# Linker options
################################################################################

L_OPTS += \
	-nostartfiles \
	-map="$(TARGET_BUILD_DIR)/$(APP_NAME).map" \
	"$(TARGET_BUILD_DIR)/$(LINKER_SCRIPT_NAME)" \
	-overlap -w

LST_OPTS += -list="$(@:%.o=%.lst)"

LANG_OPTS +=

endif