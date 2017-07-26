# ------------------------------------------------------------------------------------------------

PROJECT_DIR = cl
BUILD_DIR = _build

PROJECT_HEADERS = $(PROJECT_DIR)/include/*.h
PROJECT_SOURCES = $(PROJECT_DIR)/src/devices/*.c

# ------------------------------------------------------------------------------------------------
# compiler

CC = gcc
CC_OPTS = -std=c99 -O3
CC_DFLAGS = -lOpenCL
CL_CPU = /opt/intel/intel-opencl-1.2-6.3.0.1904/opencl-1.2-sdk-6.3.0.1904/include
CL_GPU = /usr/local/cuda-8.0/include

# ------------------------------------------------------------------------------------------------
# 

init:
	mkdir $(BUILD_DIR)

compile_cpu:
	$(CC) $(CC_OPTS) $(PROJECT_DIR)/tests/main.c $(PROJECT_SOURCES) -I $(PROJECT_HEADERS) -I $(CL_CPU) -o $(BUILD_DIR)/main $(CC_DFLAGS)
	./$(BUILD_DIR)/main

compile_gpu:
	$(CC) $(CC_OPTS) $(PROJECT_DIR)/tests/main.c $(PROJECT_SOURCES) -I $(PROJECT_HEADERS) -I $(CL_GPU) -o $(BUILD_DIR)/main $(CC_DFLAGS)
	./$(BUILD_DIR)/main

vector:
	$(CC) $(CC_OPTS) $(PROJECT_DIR)/tests/test_vector_add.c $(PROJECT_SOURCES) -I $(PROJECT_HEADERS) -o $(BUILD_DIR)/test_vector_add $(CC_DFLAGS)
	./$(BUILD_DIR)/test_vector_add 


devices_cpu:
	$(CC) $(CC_OPTS) $(PROJECT_DIR)/tests/test_devices.c $(PROJECT_SOURCES) -I $(PROJECT_HEADERS) -I $(CL_CPU) -o $(BUILD_DIR)/test_devices $(CC_DFLAGS)
	./$(BUILD_DIR)/test_devices

devices_gpu:
	$(CC) $(CC_OPTS) $(PROJECT_DIR)/tests/test_devices.c $(PROJECT_SOURCES) -I $(PROJECT_HEADERS) -I $(CL_GPU) -o $(BUILD_DIR)/test_devices $(CC_DFLAGS)
	./$(BUILD_DIR)/test_devices

platforms:
	g++ -o $(BUILD_DIR)/clDeviceQuery -I/opt/intel/intel-opencl-1.2-6.3.0.1904/opencl-1.2-sdk-6.3.0.1904/include scripts/clDeviceQuery.cpp -lOpenCL
	./$(BUILD_DIR)/clDeviceQuery	
