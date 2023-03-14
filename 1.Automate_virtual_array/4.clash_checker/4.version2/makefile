GLYHOME = ../1.Automate_virtual_array/1.grafting/1.C_script/1.C_grafting0.1/Glylib_Jan11
CC = gcc
FLAGS = -I$(GLYHOME)/inc -L$(GLYHOME)/lib -lglylib -Wall -lm
SOURCE = call_find_vdw_overlap.c
FUNCTIONS =
OUTEXE = call_find_vdw_overlap.exe

default:
	$(CC) -g -o0 $(SOURCE) $(FUNCTIONS) $(FLAGS) -o $(OUTEXE)

gdb:
	$(CC) $(SOURCE) $(FUNCTIONS) $(FLAGS) -g -o $(OUTEXE)

clean:
	rm $(OUTEXE)
