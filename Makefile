###############################################################################
# Makefile
# Contains instruction to perform build of software for microcontroller
###############################################################################

# Whether to build debug version or release
BUILD_TYPE ?= Debug

# Name of target executable
PROJECT := tiva-freertos

# 
# Directories options
# 

CONFIG_DIR := $(CURDIR)/config

SRC_DIR := $(CURDIR)/src

INCLUDE_DIR := $(CURDIR)/include

BUILD_DIR_BASE := $(CURDIR)/build

BUILD_DIR := $(BUILD_DIR_BASE)/$(BUILD_TYPE)

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
CXXFLAGS += --specs=nano.specs

# We don't like everything from C++...
CXXFLAGS += -fno-exceptions -fno-rtti
CXXFLAGS += -fno-unwind-tables -fno-asynchronous-unwind-tables
CXXFLAGS += -fno-threadsafe-statics -fno-use-cxa-atexit

# Trim as much garbage as possible
CXXFLAGS += -ffunction-sections -fdata-sections  

# We will provide our custom startup files
CXXFLAGS += -nostartfiles

# 
# Linker options
# 

LDFLAGS += -Wl,-T $(SRC_DIR)/tm4c123gh6pm.ld 
LDFLAGS += -Wl,--gc-sections

# 
# Source files
# 

# Include directories
CXXFLAGS += \
	-I$(CONFIG_DIR) \
	-I$(INCLUDE_DIR) \
	-I$(INCLUDE_DIR)/freertos \
	-I$(SRC_DIR)/freertos/portable/GCC/CM4F \

# Headers used across the project
INCLUDES += \
	$(SRC_DIR)/freertos/portable/GCC/CM4F/portmacro.h \
	$(INCLUDE_DIR)/freertos/atomic.h \
	$(INCLUDE_DIR)/freertos/croutine.h \
	$(INCLUDE_DIR)/freertos/deprecated_definitions.h \
	$(INCLUDE_DIR)/freertos/event_groups.h \
	$(INCLUDE_DIR)/freertos/FreeRTOS.h \
	$(INCLUDE_DIR)/freertos/list.h \
	$(INCLUDE_DIR)/freertos/message_buffer.h \
	$(INCLUDE_DIR)/freertos/mpu_prototypes.h \
	$(INCLUDE_DIR)/freertos/mpu_wrappers.h \
	$(INCLUDE_DIR)/freertos/portable.h \
	$(INCLUDE_DIR)/freertos/projdefs.h \
	$(INCLUDE_DIR)/freertos/queue.h \
	$(INCLUDE_DIR)/freertos/semphr.h \
	$(INCLUDE_DIR)/freertos/stack_macros.h \
	$(INCLUDE_DIR)/freertos/StackMacros.h \
	$(INCLUDE_DIR)/freertos/stream_buffer.h \
	$(INCLUDE_DIR)/freertos/task.h \
	$(INCLUDE_DIR)/freertos/timers.h \
	$(INCLUDE_DIR)/hw_sysctl.h \
	$(INCLUDE_DIR)/hw_gpio.h \
	$(INCLUDE_DIR)/hw_uart.h \
	$(INCLUDE_DIR)/hw_hibernate.h \

# Application modules
SOURCES += \
	$(SRC_DIR)/freertos/portable/GCC/CM4F/port.c \
	$(SRC_DIR)/freertos/portable/MemMang/heap_1.c \
	$(SRC_DIR)/freertos/croutine.c \
	$(SRC_DIR)/freertos/event_group.c \
	$(SRC_DIR)/freertos/list.c \
	$(SRC_DIR)/freertos/queue.c \
	$(SRC_DIR)/freertos/stream_buffer.c \
	$(SRC_DIR)/freertos/tasks.c \
	$(SRC_DIR)/freertos/timers.c \
    $(SRC_DIR)/main.cpp \

# Modules configurations
CONFIGS += \
	$(CONFIG_DIR)/FreeRTOSConfig.h \

# All dependencies (even not compiled directly)
DEPS += \
	$(SOURCES) \
	$(INCLUDES) \
	$(CONFIGS) \
	$(SRC_DIR)/cli.cpp \
    $(SRC_DIR)/gpio.cpp \
    $(SRC_DIR)/handlers.cpp \
    $(SRC_DIR)/hibernate.cpp \
    $(SRC_DIR)/leds.cpp \
    $(SRC_DIR)/nvic.cpp \
    $(SRC_DIR)/reset.cpp \
    $(SRC_DIR)/stack.cpp \
    $(SRC_DIR)/sysctl.cpp \
    $(SRC_DIR)/tm4c123gh6pm.ld \
    $(SRC_DIR)/types.cpp \
    $(SRC_DIR)/uart.cpp \
    $(SRC_DIR)/utils.cpp \
    Makefile \

# 
# Outputs
# 

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
$(PROJECT_ELF): $(DEPS) | $(BUILD_DIR)
	$(CXX) $(SOURCES) $(CXXFLAGS) $(LDFLAGS) -o $(PROJECT_ELF)

# Gett binary version (ready to write) of the application
$(PROJECT_BIN): $(PROJECT_ELF)
	$(OBJCOPY) -O binary $(PROJECT_ELF) $(PROJECT_BIN)

# Obtain size of generated ELF file
size: $(PROJECT_ELF)
	$(SIZE) $(PROJECT_ELF)

# Get object dump with source instructions
dump: $(PROJECT_ELF)
	$(OBJDUMP) -S -d $(PROJECT_ELF) > dump

# Write (flashing) binary file into MCU
flash: $(PROJECT_BIN)
	$(LM4FLASH) $(PROJECT_BIN) -E -v

# Start debug session with MCU
debug: $(PROJECT_ELF)
	$(GDB) $(PROJECT_ELF)

# Clean everything from current build directory
clean:
	rm -rf $(BUILD_DIR)/*

# Clean everything from all build directories
distclean:
	rm -rf $(BUILD_DIR_BASE)/*
