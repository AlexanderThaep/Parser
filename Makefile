# the compiler to use
CC = clang

# compiler flags:
#  -g    adds debugging information to the executable file
#  -Wall turns on most, but not all, compiler warnings
CFLAGS  = -g -Wall
  
#files to link:
INCLUDE = include
SRC = src
  
# the name to use for both the target source file, and the output file:
TARGET = Main
OBJECTS = ($(SRC).c:=.o)
  
all: $(TARGET)

$(TARGET): $(TARGET).c $(SRC)/*.c
	$(CC) $(CFLAGS) -I $(INCLUDE) $(TARGET).c $(SRC)/*.c -o Main

clean:
	rm -rf *.o