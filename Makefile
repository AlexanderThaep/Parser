# the compiler to use
CC = clang

ifeq ($(VERBOSE), YES)
	V_CC = $(CC)
else
	V_CC = @$(CC)
endif

# compiler flags:
#  -g    adds debugging information to the executable file
#  -Wall turns on most, but not all, compiler warnings
CFLAGS  = -g -Wall

#files to link:
INCLUDE = include
SRC = src
  
# the name to use for both the target source file, and the output file:
TARGET = Main
BUILD = build

SRCS = $(shell find $(SRC) -name '*.c')
 
all: $(TARGET)

$(TARGET): $(SRCS)
	mkdir -p $(BUILD)
	$(CC) $(CFLAGS) -I $(INCLUDE) $(SRCS) -o $(BUILD)/Main

clean:
	-rm -rf $(BUILD)/*

run: $(TARGET)
	$(BUILD)/$(TARGET)