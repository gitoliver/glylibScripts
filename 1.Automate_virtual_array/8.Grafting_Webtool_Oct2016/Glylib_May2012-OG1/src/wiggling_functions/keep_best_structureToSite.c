#include <molecules.h>
#include <glylib.h>
#include <mylib.h>

/* Load in best_structure and if it clashes less than current one keep it */

void keep_best_structureToSite(assembly *A, assembly *B, double *total_clash, int current_resid, int target_resid){
assembly C;
char filename[20];
strcpy(filename, "best_structure.pdb");
load_pdb(filename,&C);
int ri;
load_pdb(filename,A); // replace A with it and continue
for (ri=0;ri<(*A).m[0][0].nr;ri++){
    if ((*A).m[0][0].r[ri].na>1){
        set_smallest_rings_from_residue_atom_nodes(&(*A).m[0][0].r[ri]);
    }
}
	//set_nbonds_for_atoms_in_assembly(A);
	//set_nmb_for_atoms_in_assembly(A);
return;
}
