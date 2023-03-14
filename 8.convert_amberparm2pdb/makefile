GLYLIB = /home/oliver/Glylibs/Glylib_Sep2011/lib
GLYINC = /home/oliver/Glylibs/Glylib_Sep2011//inc
FLAGS = -I$(GLYINC) -L$(GLYLIB) -lglylib -Wall -lm
SOURCE = glycomePDB.c
OUTEXE = makeGlycomePDB

default:
	 gcc $(SOURCE)  $(FLAGS) -g -o $(OUTEXE)

gdb:
	gcc $(SOURCE) $(FLAGS) -g -o $(OUTEXE)

