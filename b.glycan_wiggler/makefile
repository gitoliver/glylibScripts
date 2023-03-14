GLYHOME = /home/oliver/work/Glylib_May2012
#GLYHOME = /home/oliver/Dropbox/1.glylib_scripts/1.Automate_virtual_array/1.grafting/1.C_script/1.C_grafting0.1/Glylib_Jan11
CC = gcc
FLAGS = -I$(GLYHOME)/inc -L$(GLYHOME)/lib -lglylib -Wall -lm -g -o0
#SOURCE=driver_self_clash_finder.c
#SOURCE= driver_set_smallest_rings_from_residue_atom_nodes.c
#SOURCE = driver_find_vdw_clashes_return_clashiest_resid.c
SOURCE = driver_wiggler.c
#SOURCE = driver_find_linkage_torsions.c
#SOURCE = driver_find_branch_to_rotate.c
FUNCTIONS =
#OUTEXE=driver_self_clash_finder.exe
#OUTEXE = driver_set_smallest_rings_from_residue_atom_nodes.exe
#OUTEXE = driver_find_vdw_clashes_return_clashiest_resid.exe
OUTEXE = driver_wiggler.exe
#OUTEXE = driver_find_linkage_torsions.exe
#OUTEXE = driver_find_branch_to_rotate.exe

default:
	$(CC) -g -o0 $(SOURCE) $(FUNCTIONS) $(FLAGS) -o $(OUTEXE)

gdb:
	$(CC) $(SOURCE) $(FUNCTIONS) $(FLAGS) -g -o $(OUTEXE)

clean:
	rm $(OUTEXE)
