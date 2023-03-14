GLYHOME = /your/path/to/Glylib?
CC = gcc
FLAGS = -I$(GLYHOME)/inc -L$(GLYHOME)/lib -lglylib -Wall -lm
SOURCE = clashChecker.c
FUNCTIONS =
OUTEXE = clashChecker.exe

default:
	$(CC) -g -o0 $(SOURCE) $(FUNCTIONS) $(FLAGS) -o $(OUTEXE)

gdb:
	$(CC) $(SOURCE) $(FUNCTIONS) $(FLAGS) -g -o $(OUTEXE)

clean:
	rm $(OUTEXE)
