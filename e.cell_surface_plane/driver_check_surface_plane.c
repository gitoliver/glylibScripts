#include <glylib.h> // required for load_pdb function
#include <mylib.h>
#include <molecules.h>

int main (void){

int res_below=0;
assembly A;
char filename[50];
strcpy(filename,"ligand.pdb");
load_pdb(filename, &A);
set_nbonds_for_atoms_in_assembly(&A);

assembly B;
strcpy(filename,"receptor.pdb");
load_pdb(filename, &B);

res_below=check_surface_plane(&A,&B);
printf("Number of residues below surface: %d\n",res_below);

return 0;
}
