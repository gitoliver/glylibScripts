GLYHOME = /home/oliver/Programs/Glylib_May2012-OG1-GlycamWeb
CC = gcc
FLAGS = -I$(GLYHOME)/inc -L$(GLYHOME)/lib -lglylib -Wall -lm
SOURCE = add-ab-reducingTermini.c
FUNCTIONS =
OUTEXE = add-ab-reducingTermini.exe

default:
	$(CC) -g -o0 $(SOURCE) $(FUNCTIONS) $(FLAGS) -o $(OUTEXE)

gdb:
	$(CC) $(SOURCE) $(FUNCTIONS) $(FLAGS) -g -o $(OUTEXE)

clean:
	rm $(OUTEXE)
