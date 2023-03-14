GLYHOME = /home/oliver/storage/Dropbox/Glylib_Jan11
CC = gcc
FLAGS = -I$(GLYHOME)/inc -L$(GLYHOME)/lib -lglylib -Wall -lm
SOURCE = ring_type_lookup.c
FUNCTIONS =
OUTEXE = ring_type_lookup.exe

default:
	$(CC) $(SOURCE) $(FUNCTIONS) $(FLAGS) -o $(OUTEXE)

gdb:
	$(CC) $(SOURCE) $(FUNCTIONS) $(FLAGS) -g -o $(OUTEXE)

clean:
	rm $(OUTEXE)
