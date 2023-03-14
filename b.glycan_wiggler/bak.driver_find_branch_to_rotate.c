#include <glylib.h>
#include <mylib.h>
#include <molecules.h>

int main (void){

branch *brnch;

assembly A;
char filename[50];
strcpy(filename, "ligand.pdb");
A=*load_pdb(filename);

set_nmb_for_atoms_in_assembly(&A);

char rot_branch[1];

int linkage_resid;
linkage_resid=2;

int core_ri[4];

core_ri[0]=1;
core_ri[1]=1;
core_ri[2]=0;
core_ri[3]=0;



find_branch_to_rotate(&A, rot_branch, linkage_resid, core_ri, brnch);

return 0;
}
