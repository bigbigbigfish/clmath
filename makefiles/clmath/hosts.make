# ------------------------------------------------------------------------------------------------
# directories

# project dir
HOSTS_SOURCES_DIR = $(SOURCES_DIR)/$(MODULE_HOSTS)

# build dir
BUILD_HOSTS_DIR = $(BUILD_SOURCES_DIR)/$(MODULE_HOSTS)

# lib
LIB_HOSTS = libcl_hosts.a


# ------------------------------------------------------------------------------------------------
# configuration

HOSTS_SOURCES := $(wildcard $(HOSTS_SOURCES_DIR)/*.c)
HOSTS_OBJECTS := $(patsubst %, $(BUILD_HOSTS_DIR)/%, $(notdir $(HOSTS_SOURCES:.c=.o)))

# ------------------------------------------------------------------------------------------------
# complication


#$(BUILD_HOSTS_DIR)/$(LIB_HOSTS) : $(HOSTS_OBJECTS) 
#	@echo "$(RED)Linking $@ $(NC)"
#	$(AR) cr $@ $^ 
#	@echo "$(RED)$(LIB_HOSTS) is saved at $(BUILD_HOSTS_DIR)/$(LIB_HOSTS)$(NC)"
#	@$(AR) -t $(BUILD_HOSTS_DIR)/$(LIB_HOSTS) 

hosts: $(HOSTS_OBJECTS)

$(BUILD_HOSTS_DIR)/%.o : $(HOSTS_SOURCES_DIR)/%.c 
	@echo "$(RED)Compiling $< $(NC)"
	$(CC) $(CC_CFLAGS) $(CC_LDFLAGS) -c $< -o $@  

