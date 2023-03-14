GLYHOME = ../Glylib_May2012-OG1/
CC = gcc
FLAGS = -I$(GLYHOME)/inc -L$(GLYHOME)/lib -lglylib -Wall -lm
SOURCE = glycoProteinWiggler.c
FUNCTIONS =
OUTEXE = glycoProteinWiggler.exe

default:
	$(CC) -g -o0 $(SOURCE) $(FUNCTIONS) $(FLAGS) -o $(OUTEXE)

gdb:
	$(CC) $(SOURCE) $(FUNCTIONS) $(FLAGS) -g -o $(OUTEXE)

clean:
	rm $(OUTEXE)
