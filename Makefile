# build an executable named bidCalculator from bidCalculator.cpp

# Compiler
CC = g++

# Flags
CFLAGS = -std=c++11 -Wall

# Target
TARGET = bidCalculator

all: $(TARGET).cpp
			$(CC) $(CFLAGS) -o $(TARGET) $(TARGET).cpp

run: $(TARGET)
			./$(TARGET)

clean:
			$(RM) $(TARGET)
      
