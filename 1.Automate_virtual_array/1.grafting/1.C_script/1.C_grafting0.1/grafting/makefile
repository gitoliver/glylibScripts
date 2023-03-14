#GLYHOME = ../Glylib_Jan11
GLYHOME = /home/oliver/work/Glylib_May2012
CC = gcc
FLAGS = -I$(GLYHOME)/inc -L$(GLYHOME)/lib -lglylib -Wall -lm -g
SOURCE = grafting.c
FUNCTIONS =
OUTEXE = 2.grafting_20.exe

default:
	$(CC) -g -o0 $(SOURCE) $(FUNCTIONS) $(FLAGS) -o $(OUTEXE)

gdb:
	$(CC) $(SOURCE) $(FUNCTIONS) $(FLAGS) -g -o $(OUTEXE)

clean:
	rm $(OUTEXE)
