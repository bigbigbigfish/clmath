# ------------------------------------------------------------------------------------------------
# directories



# ------------------------------------------------------------------------------------------------
# compilcation


TESTS_CFLAGS = $(CC_OPTS) $(CL_GPU) 
TESTS_LFLAGS = $(CC_DFLAGS)

# ------------------------------------------------------------------------------------------------
# sources

# devices
TESTS_DEVICES_SOURCES := $(wildcard $(TESTS_DIR)/$(MODULE_DEVICES)/*.c)
TESTS_DEVICES_OBJECTS := $(patsubst %, $(BUILD_TESTS_DEVICES_DIR)/%, $(notdir $(TESTS_DEVICES_SOURCES:.c=.o)))
TESTS_DEVICES_TARGETS := $(patsubst %, $(BUILD_TESTS_DEVICES_DIR)/%, $(notdir $(TESTS_DEVICES_OBJECTS:.o=)))

# hosts
TESTS_HOSTS_SOURCES := $(wildcard $(TESTS_DIR)/$(MODULE_HOSTS)/*.c)
TESTS_HOSTS_OBJECTS := $(patsubst %, $(BUILD_TESTS_HOSTS_DIR)/%, $(notdir $(TESTS_HOSTS_SOURCES:.c=.o)))
TESTS_HOSTS_TARGETS := $(patsubst %, $(BUILD_TESTS_HOSTS_DIR)/%, $(notdir $(TESTS_HOSTS_OBJECTS:.o=)))


# ------------------------------------------------------------------------------------------------
# complication: tests_devices

tests_devices: $(TESTS_DEVICES_TARGETS)

# devices
$(BUILD_TESTS_DEVICES_DIR)/% : $(BUILD_TESTS_DEVICES_DIR)/%.o
	@echo "$(RED)Linking $@ $(NC)"
	$(CC) -o $@ $^ $(TESTS_LFLAGS)

$(BUILD_TESTS_DEVICES_DIR)/%.o : $(TESTS_DEVICES_DIR)/%.c
	@echo "$(RED)Compiling $< $(NC)"
	$(CC) -c $< -o $@ $(TESTS_CFLAGS)

# ------------------------------------------------------------------------------------------------
# complication: tests_hosts

tests_hosts: $(TESTS_HOSTS_TARGETS)

# hosts
$(BUILD_TESTS_HOSTS_DIR)/% : $(BUILD_TESTS_HOSTS_DIR)/%.o
	@echo "$(RED)Linking $@ $(NC)"
	$(CC) -o $@ $^ $(TESTS_LFLAGS)

$(BUILD_TESTS_HOSTS_DIR)/%.o : $(TESTS_HOSTS_DIR)/%.c
	@echo "$(RED)Compiling $< $(NC)"
	$(CC) -c $< -o $@ $(TESTS_CFLAGS)


# ------------------------------------------------------------------------------------------------
# tests

run_tests_devices: tests_devices
	@echo "$(RED) run tests_devices:$(NC)"
	$(foreach test, $(TESTS_DEVICES_TARGETS), \
          $(test) | grep "FAIL"; \
          echo "$(GREEN) TEST $(test)$(NC)";)	

run_tests_hosts: tests_hosts
	@echo "$(RED) run tests_hosts:$(NC)"
	$(foreach test, $(TESTS_HOSTS_TARGETS), \
          $(test) | grep "FAIL"; \
          echo "$(GREEN) TEST $(test)$(NC)";)	
