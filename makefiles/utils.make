# ------------------------------------------------------------------------------------------------
# directories

# project dir
SOURCES_UTILS_DIR = $(SOURCES_DIR)/$(MODULE_UTILS)

# build dir
BUILD_UTILS_DIR = $(BUILD_SOURCES_DIR)/$(MODULE_UTILS)

# lib
LIB_UTILS = libcl_utils.a

# ------------------------------------------------------------------------------------------------
# configuration

UTILS_SOURCES := $(wildcard $(SOURCES_UTILS_DIR)/*.c)
UTILS_OBJECTS := $(patsubst %, $(BUILD_UTILS_DIR)/%, $(notdir $(UTILS_SOURCES:.c=.o)))

# ------------------------------------------------------------------------------------------------
# complication

$(BUILD_UTILS_DIR)/$(LIB_UTILS) : $(UTILS_OBJECTS) 
	@echo "$(RED)Linking $@ $(NC)"
	$(AR) cr $@ $^ 
	@echo "$(RED)$(LIB_UTILS) is saved at $(BUILD_UTILS_DIR)/$(LIB_UTILS)$(NC)"
	@$(AR) -t $(BUILD_UTILS_DIR)/$(LIB_UTILS)

$(BUILD_UTILS_DIR)/%.o : $(SOURCES_UTILS_DIR)/%.c 
	@echo "$(RED)Compiling $< $(NC)"
	$(CC) $(CC_FLAGS) -c $< -o $@ 

