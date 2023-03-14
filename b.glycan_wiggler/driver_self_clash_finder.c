#include <glylib.h> // required for load_pdb function
#include <mylib.h>
#include <molecules.h>

int main (void){

char filename[50];
int resid;

/*
assembly B;
strcpy(filename, "receptor.pdb");
B=*load_pdb(filename);
*/

assembly A;
strcpy(filename, "ligand.pdb");
A=*load_pdb(filename);
resid=find_vdw_clashes_return_self_clashes(&A);

/*
int ri;
for (ri=0;ri<A.m[0][0].nr;ri++){
        if (A.m[0][0].r[ri].na>1){
                set_smallest_rings_from_residue_atom_nodes(&A.m[0][0].r[ri]);
        }
}
*/

//set_nbonds_for_atoms_in_assembly(&A);
//set_nmb_for_atoms_in_assembly(&A);


int core_ri[2];
core_ri[0]=1; // number of residues in core is stored in 0.
core_ri[1]=6;
core_ri[2]=3;
core_ri[3]=4;
core_ri[4]=5;

//wiggler(&A,&B,core_ri);

return 0;
}
