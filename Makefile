# ------------------------------------------------------------------------------------------------

PROJECT_DIR = cl
BUILD_DIR = _build

PROJECT_HEADERS = $(PROJECT_DIR)/include/*.h
PROJECT_SOURCES = $(PROJECT_DIR)/src/devices/*.c
PROJECT_TESTS = $(PROJECT_DIR)/tests

# ------------------------------------------------------------------------------------------------
# compiler

CC = gcc
CC_OPTS = -std=c99 -O3
CC_DFLAGS = -lOpenCL
CL_CPU = -I/opt/intel/intel-opencl-1.2-6.3.0.1904/opencl-1.2-sdk-6.3.0.1904/include
CL_GPU = -I/usr/local/cuda-8.0/include

# ------------------------------------------------------------------------------------------------
# 

init:
	mkdir $(BUILD_DIR)

vector_add:
	$(CC) -o $(BUILD_DIR)/test_vector_add $(PROJECT_TESTS)/test_vector_add.c
	./$(BUILD_DIR)/test_vector_add

