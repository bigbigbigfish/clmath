# ------------------------------------------------------------------------------------------------

PROJECT_DIR = cl
BUILD_DIR = _build

PROJECT_HEADERS = $(PROJECT_DIR)/include/*.h
PROJECT_SOURCES = $(PROJECT_DIR)/src/devices/*.c

# ------------------------------------------------------------------------------------------------
# compiler

CC_OPTS = -std=c99 -O3 
CL_CPU = -I/opt/intel/intel-opencl-1.2-6.3.0.1904/opencl-1.2-sdk-6.3.0.1904/include
CL_GPU = 

# ------------------------------------------------------------------------------------------------
# 

init:
	mkdir $(BUILD_DIR)

compile:
	gcc $(CC_OPTS) $(PROJECT_DIR)/main.c $(PROJECT_SOURCES) -I $(PROJECT_HEADERS) -o $(BUILD_DIR)/main -lOpenCL
	./$(BUILD_DIR)/main


platforms:
	g++ -o $(BUILD_DIR)/clDeviceQuery -I/opt/intel/intel-opencl-1.2-6.3.0.1904/opencl-1.2-sdk-6.3.0.1904/include scripts/clDeviceQuery.cpp -lOpenCL
	./$(BUILD_DIR)/clDeviceQuery	
