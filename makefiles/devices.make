# ------------------------------------------------------------------------------------------------
# directories

# project dir
SOURCES_DEVICES_DIR = $(SOURCES_DIR)/$(MODULE_DEVICES)

# build dir
BUILD_DEVICES_DIR = $(BUILD_SOURCES_DIR)/$(MODULE_DEVICES)

# lib
LIB_DEVICES = libcl_devices.a

# ------------------------------------------------------------------------------------------------
# configuration

DEVICES_SOURCES := $(wildcard $(SOURCES_DEVICES_DIR)/*.c)
DEVICES_OBJECTS := $(patsubst %, $(BUILD_DEVICES_DIR)/%, $(notdir $(DEVICES_SOURCES:.c=.o)))

# ------------------------------------------------------------------------------------------------
# complication

$(BUILD_DEVICES_DIR)/$(LIB_DEVICES) : $(DEVICES_OBJECTS) 
	@echo "$(RED)Linking $@ $(NC)"
	$(AR) cr $@ $^ 
	@echo "$(RED)$(LIB_DEVICES) is saved at $(BUILD_DEVICES_DIR)/$(LIB_DEVICES)$(NC)"
	@$(AR) -t $(BUILD_DEVICES_DIR)/$(LIB_DEVICES)

$(BUILD_DEVICES_DIR)/%.o : $(SOURCES_DEVICES_DIR)/%.c 
	@echo "$(RED)Compiling $< $(NC)"
	$(CC) $(CC_FLAGS) -c $< -o $@ 

