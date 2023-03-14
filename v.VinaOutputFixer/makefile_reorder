GLYLIB = /home/oliver/work/Glylib_May2012/lib
GLYINC = /home/oliver/work/Glylib_May2012/inc
#GLYLIB = /home/spandana/GLYLIB_pre20090806/lib
#GLYINC = /home/spandana/GLYLIB_pre20090806/inc
FLAGS = -I$(GLYINC) -L$(GLYLIB) -lglylib -Wall -lm
SOURCE = reorderpdb.c
OUTEXE = reorder

default:
	 gcc $(SOURCE) $(FLAGS) -g -o $(OUTEXE)

gdb:
	gcc $(SOURCE) $(FLAGS) -g -o $(OUTEXE)

eventually:
	gcc test_final.c outputpdb_fromslurp.c $(FLAGS) -g -o $(OUTEXE)

#test:
#       gcc $(TESTIN) $(FLAGS) -g -o testexe
