# ------------------------------------------------------------------------------------------------
# includes

include makefiles/clmath/utils.make
include makefiles/clmath/devices.make
include makefiles/clmath/hosts.make

# ------------------------------------------------------------------------------------------------
# clmath

LIB_CL = libclmath.a
LIB_CL_LD = libclmath.so

clmath_objs: utils devices hosts

$(BUILD_SOURCES_DIR)/$(LIB_CL) : $(UTILS_OBJECTS) $(DEVICES_OBJECTS) $(HOSTS_OBJECTS)
	@echo "$(RED)Linking $@ $(NC)"
	$(AR) cr $(CC_FLAGS) $@ $^  
	@echo "$(RED)$(LIB_CL) is saved at $(BUILD_SOURCES_DIR)/$(LIB_CL)$(NC)"
	@$(AR) -t $(BUILD_SOURCES_DIR)/$(LIB_CL) 

$(BUILD_SOURCES_DIR)/$(LIB_CL_LD):
	$(CC) $(CC_OPTS_SHARED) $(UTILS_OBJECTS) $(DEVICES_OBJECTS) $(HOSTS_OBJECTS) $(CC_FLAGS) $(CC_LDFLAGS)

clmath_core: $(BUILD_UTILS_DIR)/$(LIB_UTILS) $(BUILD_DEVICES_DIR)/$(LIB_DEVICES) $(BUILD_HOSTS_DIR)/$(LIB_HOSTS)

# ------------------------------------------------------------------------------------------------
