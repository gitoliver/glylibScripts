GLYHOME = /home/oliver/Glylib_Jan11
CC = gcc
FLAGS = -I$(GLYHOME)/inc -L$(GLYHOME)/lib -lglylib -Wall -lm
SOURCE = rename_superimpose1.1.c
FUNCTIONS =
OUTEXE = rename_superimpose1.1.exe

default:
	$(CC) $(SOURCE) $(FUNCTIONS) $(FLAGS) -o $(OUTEXE)

gdb:
	$(CC) $(SOURCE) $(FUNCTIONS) $(FLAGS) -g -o $(OUTEXE)

clean:
	rm $(OUTEXE)
