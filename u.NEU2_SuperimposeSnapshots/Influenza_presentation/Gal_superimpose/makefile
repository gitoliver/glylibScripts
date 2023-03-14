GLYHOME = /home/oliver/Programs/Glylib_May2012-OG1-GlycamWeb/
CC = gcc
FLAGS = -I$(GLYHOME)/inc -L$(GLYHOME)/lib -lglylib -Wall -lm
SOURCE = superimposePDBs.c
FUNCTIONS =
OUTEXE = superimposePDBs.exe

default:
	$(CC) $(SOURCE) $(FUNCTIONS) $(FLAGS) -o $(OUTEXE)

gdb:
	$(CC) $(SOURCE) $(FUNCTIONS) $(FLAGS) -g -o $(OUTEXE)

clean:
	rm $(OUTEXE)
