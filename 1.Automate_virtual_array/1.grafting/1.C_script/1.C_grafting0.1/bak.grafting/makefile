GLYHOME = ../Glylib_Jan11
CC = gcc
FLAGS = -I$(GLYHOME)/inc -L$(GLYHOME)/lib -lglylib -Wall -lm
SOURCE = grafting.c
FUNCTIONS =
OUTEXE = 2.grafting.exe

default:
	$(CC) -g -o0 $(SOURCE) $(FUNCTIONS) $(FLAGS) -o $(OUTEXE)

gdb:
	$(CC) $(SOURCE) $(FUNCTIONS) $(FLAGS) -g -o $(OUTEXE)

clean:
	rm $(OUTEXE)
