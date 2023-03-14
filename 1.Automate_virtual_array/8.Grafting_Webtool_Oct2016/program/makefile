#GLYHOME = ../Glylib_Jan11
GLYHOME = /home/oliver/Dropbox/1.glylib_scripts/1.Automate_virtual_array/8.Grafting_Webtool_Oct2016/Glylib_May2012-OG1
CC = gcc
FLAGS = -I$(GLYHOME)/inc -L$(GLYHOME)/lib -lglylib -Wall -lm -g
SOURCE = grafting.c
FUNCTIONS =
OUTEXE = grafting.exe

default:
	$(CC) -g -o0 $(SOURCE) $(FUNCTIONS) $(FLAGS) -o $(OUTEXE)

gdb:
	$(CC) $(SOURCE) $(FUNCTIONS) $(FLAGS) -g -o $(OUTEXE)

clean:
	rm $(OUTEXE)
