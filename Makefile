# ------------------------------------------------------------------------------------------------

PROJECT_DIR = clmath
BUILD_DIR = _build

# main folders
HEADERS_DIR = $(PROJECT_DIR)/inc
SOURCES_DIR = $(PROJECT_DIR)/src/clmath
TESTS_DIR = $(PROJECT_DIR)/src/tests

# build
BUILD_SOURCES_DIR = $(BUILD_DIR)/src/clmath
BUILD_TESTS_DIR = $(BUILD_DIR)/src/tests

# modules
MODULE_DEVICES = devices
MODULE_HOSTS = hosts
MODULE_UTILS = utils

# ------------------------------------------------------------------------------------------------
# compiler

CC = gcc
CL_CPU_INC = /opt/intel/intel-opencl-1.2-6.3.0.1904/opencl-1.2-sdk-6.3.0.1904/include
CL_GPU_INC = /usr/local/cuda-8.0/include
CL_GPU_LIB = /usr/local/cuda-8.0/lib64
CC_OPTS = -std=c99 -O3 -g -Wall -Wstrict-prototypes -Wmissing-prototypes -Wshadow -Wconversion
CC_OPTS_SHARED = -fPIC -rdynamic -shared -Lstatic


CC_CFLAGS = $(CC_OPTS) -I$(CL_GPU_INC) -I$(HEADERS_DIR)
CC_LDFLAGS = -L$(CL_GPU_LIB) -lOpenCL

# ------------------------------------------------------------------------------------------------
# console

RED = \033[1;31m
GREEN = \033[1;32m
BLUE = \033[1;34m
YELLOW = \033[1;33m
NC = \033[1;0m


# ------------------------------------------------------------------------------------------------
# includes

include makefiles/clmath.make
include makefiles/tests.make

libs: clmath_objs $(BUILD_SOURCES_DIR)/$(LIB_CL)

# ------------------------------------------------------------------------------------------------
# 

build:
	mkdir -p $(BUILD_DIR)
	mkdir -p $(BUILD_SOURCES_DIR)
	mkdir -p $(BUILD_UTILS_DIR)
	mkdir -p $(BUILD_DEVICES_DIR)
	mkdir -p $(BUILD_HOSTS_DIR)
	mkdir -p $(BUILD_TESTS_DIR)
	mkdir -p $(BUILD_TESTS_UTILS_DIR)
	mkdir -p $(BUILD_TESTS_DEVICES_DIR)
	mkdir -p $(BUILD_TESTS_HOSTS_DIR)

all: clean build libs tests


tests: run_tests_utils run_tests_devices run_tests_hosts


clean:
	rm -rf $(BUILD_DIR)	
