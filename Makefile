# ------------------------------------------------------------------------------------------------

PROJECT_DIR = cl
BUILD_DIR = _build

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
	gcc $(CC_OPTS) $(PROJECT_DIR)/main.c -o $(BUILD_DIR)/main -lOpenCL
	./$(BUILD_DIR)/main
	
