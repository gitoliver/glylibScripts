#include <glylib.h> // required for load_pdb function
#include <mylib.h>
#include <molecules.h>

int main (void){

// Holds pointers to coord of atoms in part of molecule that is to be rotated

// Holds molecule information This will be passed in as part of function call
/*************function call stuff start******************/
assembly A;
char filename[50];
strcpy(filename, "ligand.pdb");
A=*load_pdb(filename);
// Sets the number of bonds an atom has within a residue
set_nbonds_for_atoms_in_assembly(&A);
// Sets the number of bonds an atom has to other residues. number of molecular bonds nmb
set_nmb_for_atoms_in_assembly(&A);

assembly B;
strcpy(filename, "receptor.pdb");
B=*load_pdb(filename);

int x=0;
int clashiest_resid=0;

clashiest_resid=find_vdw_clashes_return_clashiest_resid(&A, &B, x);

printf("Clashiest resid=%d\n",clashiest_resid);

return 0;
}
