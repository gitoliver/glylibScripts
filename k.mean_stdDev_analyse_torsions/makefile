CC = gcc
FLAGS = -Wall -lm -g -o0
#SOURCE=driver_self_clash_finder.c
#SOURCE= driver_set_smallest_rings_from_residue_atom_nodes.c
#SOURCE = driver_find_vdw_clashes_return_clashiest_resid.c
SOURCE = meanStdDev.c
#SOURCE = driver_find_linkage_torsions.c
#SOURCE = driver_find_branch_to_rotate.c
FUNCTIONS =
#OUTEXE=driver_self_clash_finder.exe
#OUTEXE = driver_set_smallest_rings_from_residue_atom_nodes.exe
#OUTEXE = driver_find_vdw_clashes_return_clashiest_resid.exe
OUTEXE = meanStdDev.exe
#OUTEXE = driver_find_linkage_torsions.exe
#OUTEXE = driver_find_branch_to_rotate.exe

default:
	$(CC) -g -o0 $(SOURCE) $(FUNCTIONS) $(FLAGS) -o $(OUTEXE)

gdb:
	$(CC) $(SOURCE) $(FUNCTIONS) $(FLAGS) -g -o $(OUTEXE)

clean:
	rm $(OUTEXE)
