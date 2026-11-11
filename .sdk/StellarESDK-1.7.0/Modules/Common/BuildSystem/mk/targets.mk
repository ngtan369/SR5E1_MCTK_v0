################################################################################
# StellarESDK targets makefile.
################################################################################

PARALLEL_BUILD := 8

################################################################################
# Commands
################################################################################

ECHO    := echo
CP      := cp -f
CPDIR   := cp -rf
RM      := rm -f
RMDIR   := rm -rf
MKDIR   := mkdir -p
DIRNAME := dirname
GREP    := grep
GAWK    := gawk
TOUCH   := touch
CUT     := cut
SED     := sed

################################################################################
# Image file formats
################################################################################

ifeq ($(GENERATE_BIN_IMAGE), 0)
BIN_IMAGE :=
else
BIN_IMAGE := $(TARGET_BUILD_DIR)/$(APP_NAME).bin
endif

ifeq ($(GENERATE_HEX_IMAGE), 0)
HEX_IMAGE :=
else
HEX_IMAGE := $(TARGET_BUILD_DIR)/$(APP_NAME).hex
endif

ifeq ($(GENERATE_SREC_IMAGE), 0)
SREC_IMAGE :=
else
SREC_IMAGE := $(TARGET_BUILD_DIR)/$(APP_NAME).srec
endif

ifneq ($(SIZE),)
# Size information always printed
SIZE_IMAGE := $(TARGET_BUILD_DIR)/$(APP_NAME).size
endif

################################################################################
# Targets
################################################################################

VPATH = $(STELLAR_E_SDK_ROOT_DIR)

.DEFAULT_GOAL := all

uniq = $(eval _uniq := $1)$(strip $(foreach _,$(_uniq),$(if $(filter $_,$(_uniq)),$(eval _uniq := $(filter-out $_,$(_uniq)))$_)))

default-all:
	@$(ECHO) "Build time :  $(shell date +'%Y-%b-%d @ %H:%M:%S')"
	@$(ECHO) "Toolchain  :  $(TOOLCHAIN_ID)"
	@$(ECHO) "Device     :  $(CONFIG_DEVICE)"
	@$(ECHO) "Board      :  $(CONFIG_BOARD)"
	@$(ECHO) "Core       :  $(CONFIG_TARGET_CORE)"
	@$(ECHO) "Build      :  $(TARGET_BUILD) ($(CONFIG_TARGET_MEMORY))"
	$(AT)$(MKDIR) $(TARGET_BUILD_DIR)
	$(if $(C_OBJS), $(shell $(MKDIR) $(call uniq,$(dir $(C_OBJS)))),)
	$(if $(A_OBJS), $(shell $(MKDIR) $(call uniq,$(dir $(A_OBJS)))),)
	$(AT)"$(MAKE)" -j $(PARALLEL_BUILD) --no-print-directory pre-build
	$(AT)"$(MAKE)" -j $(PARALLEL_BUILD) --no-print-directory main-build


$(TARGET_BUILD_DIR)/$(LINKER_SCRIPT_NAME):
	@$(ECHO) Generating linker script file '$(TARGET_BUILD_DIR)/$(LINKER_SCRIPT_NAME)'
ifeq ($(TOOLCHAIN), IAR)
	$(AT)$(CC) $(TARGET_OPTS) $(LANG_OPTS) $(LINKER_SCRIPT_INCS:%=-I%) $(LINKER_SCRIPT_DEFS:%=-D%) "$(LINKER_SCRIPT_SOURCE)" --preprocess=n $(TARGET_BUILD_DIR)/$(LINKER_SCRIPT_NAME).tmp
else
	$(AT)$(CC) $(TARGET_OPTS) -E $(LANG_OPTS) $(LINKER_SCRIPT_INCS:%=-I%) $(LINKER_SCRIPT_DEFS:%=-D%) "$(LINKER_SCRIPT_SOURCE)" > $(TARGET_BUILD_DIR)/$(LINKER_SCRIPT_NAME).tmp
endif
	$(AT)$(GREP) -v "^#" $(TARGET_BUILD_DIR)/$(LINKER_SCRIPT_NAME).tmp > "$(TARGET_BUILD_DIR)/$(LINKER_SCRIPT_NAME)"
	-$(AT)$(RM) $(TARGET_BUILD_DIR)/$(LINKER_SCRIPT_NAME).tmp

main-build: $(TARGET_BUILD_DIR)/$(APP_NAME).elf secondary-outputs
	@$(ECHO) Done.


$(TARGET_BUILD_DIR)/$(APP_NAME).bin: $(TARGET_BUILD_DIR)/$(APP_NAME).elf
ifeq ($(CONFIG_TARGET_MEMORY), ram)
	@$(ECHO) Creating BINARY ram image
else
	@$(ECHO) Creating BINARY nvm image
endif
	$(AT)$(GENBIN) $(BIN_OPTS) "$(TARGET_BUILD_DIR)/$(APP_NAME).elf" $(OUT_OPTS) "$(TARGET_BUILD_DIR)/$(APP_NAME).bin"
ifneq ($(TOOLCHAIN), ARMCLANG)
	$(AT)$(GENBIN) $(BIN_OPTS) $(BIN_CSEC) "$(TARGET_BUILD_DIR)/$(APP_NAME).elf" $(OUT_OPTS) "$(TARGET_BUILD_DIR)/$(APP_NAME).code.bin"
	$(AT)$(GENBIN) $(BIN_OPTS) $(BIN_DSEC) "$(TARGET_BUILD_DIR)/$(APP_NAME).elf" $(OUT_OPTS) "$(TARGET_BUILD_DIR)/$(APP_NAME).data.bin"
endif


$(TARGET_BUILD_DIR)/$(APP_NAME).hex: $(TARGET_BUILD_DIR)/$(APP_NAME).elf
ifeq ($(CONFIG_TARGET_MEMORY), ram)
	@$(ECHO) Creating HEX ram image
else
	@$(ECHO) Creating HEX nvm image
endif
	$(AT)$(GENHEX) $(HEX_OPTS) "$(TARGET_BUILD_DIR)/$(APP_NAME).elf" $(OUT_OPTS) "$(TARGET_BUILD_DIR)/$(APP_NAME).hex"


$(TARGET_BUILD_DIR)/$(APP_NAME).srec: $(TARGET_BUILD_DIR)/$(APP_NAME).elf
ifeq ($(CONFIG_TARGET_MEMORY), ram)
	@$(ECHO) Creating SREC ram image
else
	@$(ECHO) Creating SREC nvm image
endif
	$(AT)$(GENSREC) $(SREC_OPTS) "$(TARGET_BUILD_DIR)/$(APP_NAME).elf" $(OUT_OPTS) "$(TARGET_BUILD_DIR)/$(APP_NAME).srec"


ifneq ($(SIZE),)
$(TARGET_BUILD_DIR)/$(APP_NAME).size: $(TARGET_BUILD_DIR)/$(APP_NAME).elf
	@$(ECHO) 'Printing Size'
	$(AT)$(SIZE) $(SIZE_OPTS) "$(TARGET_BUILD_DIR)/$(APP_NAME).elf"
	$(AT)$(TOUCH) "$(TARGET_BUILD_DIR)/$(APP_NAME).size"
endif

	
$(TARGET_BUILD_DIR)/$(APP_NAME).elf: $(TARGET_BUILD_DIR)/$(LINKER_SCRIPT_NAME) $(C_OBJS) $(A_OBJS)
	@$(ECHO) "Linking '$@'"
	$(file >c_objs.lst,$(C_OBJS) $(A_OBJS))
# GCC/Hightec/GHS linking
ifeq ($(TOOLCHAIN), $(filter $(TOOLCHAIN), ARM HIGHTEC GHS))
	$(AT)$(CC) $(C_OPTS) $(L_OPTS) -o "$(@)" @c_objs.lst $(LIBS)
else
# IAR/ARMCLANG linking
	$(shell $(MKDIR) $(TEMP_OBJ_DIR))
	$(foreach file, $(C_OBJS), $(shell $(CP) $(file) $(TEMP_OBJ_DIR)))
	$(foreach file, $(A_OBJS), $(shell $(CP) $(file) $(TEMP_OBJ_DIR)))
	$(AT)$(eval TEMP_OBJS = $(shell find $(TEMP_OBJ_DIR) -type f -name "*.o"))
ifeq ($(TOOLCHAIN), IAR)
	$(AT)$(LD) $(L_OPTS) -o "$(@)" $(TEMP_OBJS) $(LIBS);
else
	$(AT)$(LD) $(L_OPTS) --output "$(@)" $(TEMP_OBJS) $(LIBS);
endif
	$(AT)$(RMDIR) $(TEMP_OBJ_DIR)
endif

$(TARGET_BUILD_DIR)/%.o: %.S
	@$(ECHO) Building file: '$(<F)'
ifeq ($(TOOLCHAIN), IAR)
	$(AT)$(AS) $(A_OPTS) $(C_INCS:%=-I"%") $(A_INCS:%=-I"%") $(A_DEFS:%=-D%) $(LST_OPTS) $(DEP_OPTS) -o "$(@)" "$(<)"
else
	$(AT)$(CC) -c $(C_OPTS) $(A_OPTS) $(C_INCS:%=-I"%") $(A_INCS:%=-I"%") $(A_DEFS:%=-D%) $(LST_OPTS) $(DEP_OPTS) -o "$(@)" "$(<)"
endif

$(TARGET_BUILD_DIR)/%.o: %.s
	@$(ECHO) Building file: '$(<F)'
ifeq ($(TOOLCHAIN), IAR)
	$(AT)$(AS) $(A_OPTS) $(C_INCS:%=-I"%") $(A_INCS:%=-I"%") $(A_DEFS:%=-D%) $(LST_OPTS) $(DEP_OPTS) -o "$(@)" "$(<)"
else
	$(AT)$(CC) -c $(C_OPTS) $(A_OPTS) $(C_INCS:%=-I"%") $(A_INCS:%=-I"%") $(A_DEFS:%=-D%) $(LST_OPTS) $(DEP_OPTS) -o "$(@)" "$(<)"
endif


$(TARGET_BUILD_DIR)/%.o: %.c
	@$(ECHO) Building file: '$(<F)'
	$(AT)$(CC) -c $(C_OPTS) $(C_INCS:%=-I"%") $(C_DEFS:%=-D%) $(LST_OPTS) $(DEP_OPTS) -o "$(@)" "$(<)"


$(TARGET_BUILD_DIR)/%.o: %.cpp
	@$(ECHO) Building file: '$(<F)'
	$(AT)$(CCC) -c $(CPP_OPTS) $(C_INCS:%=-I"%") $(CPP_INCS:%=-I"%") $(C_DEFS:%=-D%) $(LST_OPTS) $(DEP_OPTS) -o "$(@)" "$(<)"


clean:
	@$(ECHO) Removing object files
	$(file >c_objs.lst,$(C_OBJS) $(A_OBJS) $(A_DEPS) $(C_DEPS) $(A_OBJS:%.o=%.lst) $(C_OBJS:%.o=%.lst))
	-$(AT)$(RM) @c_objs.lst
	-$(AT)$(RM) $(TARGET_BUILD_DIR)/$(APP_NAME).elf $(TARGET_BUILD_DIR)/$(APP_NAME).map
	-$(AT)$(RM) $(TARGET_BUILD_DIR)/$(APP_NAME).bin $(TARGET_BUILD_DIR)/$(APP_NAME).hex $(TARGET_BUILD_DIR)/$(APP_NAME).srec
ifneq ($(BIN_CSEC),)
	-$(AT)$(RM) $(TARGET_BUILD_DIR)/$(APP_NAME).code.bin
endif
ifneq ($(BIN_DSEC),)
	-$(AT)$(RM) $(TARGET_BUILD_DIR)/$(APP_NAME).data.bin
endif
ifneq ($(SIZE),)
	-$(AT)$(RM) $(TARGET_BUILD_DIR)/$(APP_NAME).size
endif
	-$(AT)$(RM) $(TARGET_BUILD_DIR)/$(LINKER_SCRIPT_NAME)
	-$(AT)$(RMDIR) $(TEMP_OBJ_DIR)
	-$(AT)$(RM) c_objs.lst


purge:
	@$(ECHO) Removing build directory for '$(CONFIG_DEVICE) - $(CONFIG_BOARD)'
	-$(AT)$(RMDIR) $(TARGET_BUILD_DIR)
	-$(AT)$(RM) c_objs.lst


purge_all:
	@$(ECHO) Removing build directory
	-$(AT)$(RMDIR) $(firstword $(subst /, ,$(TARGET_BUILD_DIR)))
	-$(AT)$(RM) c_objs.lst

ifneq ($(SIZE),)
secondary-outputs: $(BIN_IMAGE) $(HEX_IMAGE) $(SREC_IMAGE) $(SIZE_IMAGE)
else
secondary-outputs: $(BIN_IMAGE) $(HEX_IMAGE) $(SREC_IMAGE)
endif


.PHONY: all clean purge purge_all main-build pre-build

################################################################################
# Automatic generated dependency files
################################################################################

-include $(C_DEPS) $(A_DEPS)
