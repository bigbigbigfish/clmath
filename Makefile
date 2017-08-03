# ------------------------------------------------------------------------------------------------

PROJECT_DIR = cl
BUILD_DIR = _build

# main folders
HEADERS_DIR = $(PROJECT_DIR)/inc
SOURCES_DIR = $(PROJECT_DIR)/src
TESTS_DIR = $(PROJECT_DIR)/tests

# build
BUILD_SOURCES_DIR = $(BUILD_DIR)/src
BUILD_TESTS_DIR = $(BUILD_DIR)/tests

# modules
MODULE_DEVICES = devices
MODULE_HOSTS = hosts

# ------------------------------------------------------------------------------------------------
# compiler

CC = gcc
CC_OPTS = -std=c99 -O3 -g -Wall -Wstrict-prototypes -Wmissing-prototypes -Wshadow -Wconversion
CC_DFLAGS = -lOpenCL
CL_CPU = -I/opt/intel/intel-opencl-1.2-6.3.0.1904/opencl-1.2-sdk-6.3.0.1904/include
CL_GPU = -I/usr/local/cuda-8.0/include

CC_FLAGS = $(CC_OPTS) $(CL_GPU) -I$(HEADERS_DIR) $(CC_DFLAGS)

# ------------------------------------------------------------------------------------------------
# console

RED = \033[1;31m
GREEN = \033[1;32m
BLUE = \033[1;34m
YELLOW = \033[1;33m
NC = \033[1;0m


# ------------------------------------------------------------------------------------------------
# includes

include makefiles/devices.make
include makefiles/hosts.make
include makefiles/tests.make

libs: $(BUILD_DEVICES_DIR)/$(LIB_DEVICES)  

# ------------------------------------------------------------------------------------------------
# 

build:
	mkdir $(BUILD_DIR)
	mkdir $(BUILD_SOURCES_DIR)
	mkdir $(BUILD_DEVICES_DIR)
	mkdir $(BUILD_HOSTS_DIR)
	mkdir $(BUILD_TESTS_DIR)
	mkdir $(BUILD_TESTS_DEVICES_DIR)
	mkdir $(BUILD_TESTS_HOSTS_DIR)

all: clean build libs tests


tests: run_tests_devices run_tests_hosts


clean:
	rm -rf $(BUILD_DIR)	
