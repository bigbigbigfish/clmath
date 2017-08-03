# ------------------------------------------------------------------------------------------------

PROJECT_DIR = cl
BUILD_DIR = _build

MODULE_DEVICES = devices
MODULE_HOSTS = hosts

HEADERS_DIR = $(PROJECT_DIR)/inc

SOURCES_DIR = $(PROJECT_DIR)/src

TESTS_DIR = $(PROJECT_DIR)/tests
TESTS_DEVICES_DIR = $(TESTS_DIR)/$(MODULE_DEVICES)
TESTS_HOSTS_DIR = $(TESTS_DIR)/$(MODULE_HOSTS)

BUILD_SOURCES_DIR = $(BUILD_DIR)/src
BUILD_TESTS_DIR = $(BUILD_DIR)/tests
BUILD_TESTS_DEVICES_DIR = $(BUILD_TESTS_DIR)/$(MODULE_DEVICES)
BUILD_TESTS_HOSTS_DIR = $(BUILD_TESTS_DIR)/$(MODULE_HOSTS)


# ------------------------------------------------------------------------------------------------
# compiler

CC = gcc
CC_OPTS = -std=c99 -O3
CC_DFLAGS = -lOpenCL
CL_CPU = -I/opt/intel/intel-opencl-1.2-6.3.0.1904/opencl-1.2-sdk-6.3.0.1904/include
CL_GPU = -I/usr/local/cuda-8.0/include

# ------------------------------------------------------------------------------------------------
# console

RED = \033[1;31m
GREEN = \033[1;32m
BLUE = \033[1;34m
YELLOW = \033[1;33m
NC = \033[1;0m


# ------------------------------------------------------------------------------------------------
# includes

include makefiles/tests.make

# ------------------------------------------------------------------------------------------------
# 

build:
	mkdir $(BUILD_DIR)
	mkdir $(BUILD_SOURCES_DIR)
	mkdir $(BUILD_TESTS_DIR)
	mkdir $(BUILD_TESTS_DEVICES_DIR)
	mkdir $(BUILD_TESTS_HOSTS_DIR)

all:


tests: run_tests_devices run_tests_hosts


clean:
	rm -rf $(BUILD_DIR)	
