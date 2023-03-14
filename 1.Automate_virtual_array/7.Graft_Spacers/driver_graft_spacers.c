#include <glylib.h> // required for load_pdb function
#include <mylib.h>
#include <molecules.h>

int main (void){

char filename[50];

assembly *B = (assembly*) calloc (1 , sizeof(assembly));
strcpy(filename, "receptor.pdb");
load_pdb(filename,B);


assembly *A = (assembly*) calloc (1 , sizeof(assembly));
strcpy(filename, "ligand.pdb");
load_pdb(filename,A);
int ri;
for (ri=0;ri<(*A).m[0][0].nr;ri++){
        if ((*A).m[0][0].r[ri].na>1){
                set_smallest_rings_from_residue_atom_nodes(&(*A).m[0][0].r[ri]);
        }
}


//set_nbonds_for_atoms_in_assembly(&A);
//set_nmb_for_atoms_in_assembly(&A);

strcpy(filename, "4-test.pdb");

graft_spacer(A,B,filename);

return 0;
}
