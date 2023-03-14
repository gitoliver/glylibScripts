#include <glylib.h> // required for load_pdb function
#include <mylib.h>
#include <molecules.h>

int main (void){

char filename[50];

assembly B;
strcpy(filename, "receptor.pdb");
B=*load_pdb(filename);

assembly A;
strcpy(filename, "ligand.pdb");
A=*load_pdb(filename);
set_nbonds_for_atoms_in_assembly(&A);
set_nmb_for_atoms_in_assembly(&A);

int core_ri[5];
core_ri[0]=1; // number of residues in core is stored in 0.
core_ri[1]=7;
core_ri[2]=2;
core_ri[3]=3;
core_ri[4]=4;

wiggler(&A,&B,core_ri);

return 0;
}
