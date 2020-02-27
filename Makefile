# the compiler: gcc for C program, define as g++ for C++
CC = gcc

# compiler flags:
#  -g    adds debugging information to the executable file
#  -Wall turns on most, but not all, compiler warnings
CFLAGS  = -g -Wall -lncurses

# the build target executable:
TARGET = battleship

all: $(TARGET)

$(TARGET): $(TARGET).o file.o maps.o power.o game.o  
	$(CC) $(TARGET).o maps.o file.o game.o power.o  -o $(TARGET) $(CFLAGS)

$(TARGET).o: $(TARGET).c
	$(CC) -c $(TARGET).c

maps.o: maps.c
	$(CC) -c maps.c

power.o: power.c
	$(CC) -c power.c

game.o: game.c
	$(CC) -c game.c

file.o: file.c
	$(CC) -c file.c

run: $(TARGET)
	./$(TARGET) input.in input1.in input2.in

clean:
	$(RM) $(TARGET)
	$(RM) *.o