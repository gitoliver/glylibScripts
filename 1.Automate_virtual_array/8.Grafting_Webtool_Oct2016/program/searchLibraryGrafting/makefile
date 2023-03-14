#GLYHOME = ../Glylib_Jan11
GLYHOME = /programs/site_deps/Grafting/Glylib_May2012-OG1
CC = gcc
FLAGS = -I$(GLYHOME)/inc -L$(GLYHOME)/lib -lglylib -Wall -lm -g
SOURCE = searchLibraryGrafting.c
FUNCTIONS =
OUTEXE = searchLibraryGrafting.exe

default:
	$(CC) -g -o0 $(SOURCE) $(FUNCTIONS) $(FLAGS) -o $(OUTEXE)

gdb:
	$(CC) $(SOURCE) $(FUNCTIONS) $(FLAGS) -g -o $(OUTEXE)

clean:
	rm $(OUTEXE)
