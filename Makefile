###############################################################################
# Makefile
# Contains instruction to perform build of software for microcontroller
###############################################################################

# Whether to build debug version or release
BUILD_TYPE ?= Debug

# Name of target executable
PROJECT := tiva-freertos

# 
# Compile options
# 

# Toolset
CC := arm-none-eabi-gcc
CXX := arm-none-eabi-g++
OBJCOPY := arm-none-eabi-objcopy
OBJDUMP := arm-none-eabi-objdump
SIZE := arm-none-eabi-size
GDB := gdb-multiarch
LM4FLASH := lm4flash

# Target architecture settings
CXXFLAGS += -march=armv7e-m -mtune=cortex-m4 -mfloat-abi=hard -mthumb -mfpu=fpv4-sp-d16 

# Optimization level depends on selected build type
ifeq ($(BUILD_TYPE),Debug)
CXXFLAGS += -g
else ifeq ($(BUILD_TYPE),MinSizeRel)
CXXFLAGS += -flto -Os -DNDEBUG
else ifeq ($(BUILD_TYPE),RelWithDebInfo)
CXXFLAGS += -flto -O2 -DNDEBUG -g
else ifeq ($(BUILD_TYPE),Release)
CXXFLAGS += -flto -O2 -DNDEBUG -g
endif

# Used language standard
CXXFLAGS += -std=gnu++17

# Warnings options
CXXFLAGS += -Wall
CXXFLAGS += -Wextra
CXXFLAGS += -pedantic

# Linkage options
CXXFLAGS += --specs=nano.specs -Wl,-T $(SRC_DIR)/tm4c123gh6pm.ld

# We don't like everything from C++...
CXXFLAGS += -fno-exceptions -fno-rtti
CXXFLAGS += -fno-unwind-tables -fno-asynchronous-unwind-tables
CXXFLAGS += -fno-threadsafe-statics -fno-use-cxa-atexit

# Trim as much garbage as possible
CXXFLAGS += -ffunction-sections -fdata-sections -Wl,--gc-sections 

# We will provide our custom startup files
CXXFLAGS += -nostartfiles

# 
# Sources
# 

SRC_DIR := $(CURDIR)/src

SOURCES += \
    $(SRC_DIR)/main.cpp \
    $(SRC_DIR)/reset.cpp \
    $(SRC_DIR)/handlers.cpp \
    $(SRC_DIR)/stack.cpp \
    $(SRC_DIR)/types.cpp \
    $(SRC_DIR)/tm4c123gh6pm.ld \
    Makefile \

# 
# Outputs
# 

BUILD_DIR_BASE := $(CURDIR)/build

BUILD_DIR := $(BUILD_DIR_BASE)/$(BUILD_TYPE)

PROJECT_ELF := $(BUILD_DIR)/$(PROJECT).elf

PROJECT_BIN := $(BUILD_DIR)/$(PROJECT).bin

# 
# Build rules
# 

.PHONY: all size clean distclean dump flash debug

# Default target, build everything and get size
all: $(PROJECT_BIN) size

# Before compilation takes place, make sure that build folder exists
$(BUILD_DIR): 
	mkdir -p $(BUILD_DIR)

# Rule for building main application
# NOTE: We are passing here only `main.cpp` assuming therefore,
#       that all other files are #include'd inside it
$(PROJECT_ELF): $(SOURCES) | $(BUILD_DIR)
	$(CXX) $(SRC_DIR)/main.cpp $(CXXFLAGS) -o $(PROJECT_ELF)

# Getting binary version (ready to write) of the application
$(PROJECT_BIN): $(PROJECT_ELF)
	$(OBJCOPY) -O binary $(PROJECT_ELF) $(PROJECT_BIN)

# Obtaining size of generated ELF file
size: $(PROJECT_ELF)
	$(SIZE) $(PROJECT_ELF)

# Getting object dump with source instructions
dump: $(PROJECT_ELF)
	$(OBJDUMP) -S -d $(PROJECT_ELF)

# Writing (flashing) binary file into MCU
flash: $(PROJECT_BIN)
	$(LM4FLASH) $(PROJECT_BIN)

# Starting debug session with MCU
debug: $(PROJECT_ELF)
	$(GDB) $(PROJECT_ELF)

# Cleaning everything from current build directory
clean:
	rm -rf $(BUILD_DIR)/*

# Cleaning everything from all build directories
distclean:
	rm -rf $(BUILD_DIR_BASE)/*
