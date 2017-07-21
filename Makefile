# ------------------------------------------------------------------------------------------------

PROJECT_DIR = cl
BUILD_DIR = _build

PROJECT_HEADERS = $(PROJECT_DIR)/include/*.h
PROJECT_SOURCES = $(PROJECT_DIR)/src/devices/*.c

# ------------------------------------------------------------------------------------------------
# compiler

CC_OPTS = -std=c99 -O3

# ------------------------------------------------------------------------------------------------
# 

init:
	mkdir $(BUILD_DIR)

install:
	sudo apt-get install opencl-headers

compile:
	gcc $(CC_OPTS) $(PROJECT_DIR)/main.c $(PROJECT_SOURCES) -I $(PROJECT_HEADERS) -o $(BUILD_DIR)/main -lOpenCL
	./$(BUILD_DIR)/main
	
