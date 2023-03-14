#include <glylib.h> // required for load_pdb function
#include <mylib.h>
#include <molecules.h>

int main (void){

// Holds pointers to coords of atoms that define linkage torsions
// For a 1-4 linkage it's 5 atoms. For a 1-6 it's 6. For a 2-8 I can't handle it.
dihedral_coord_set *DCS;
DCS=(dihedral_coord_set*)calloc(1, sizeof(dihedral_coord_set)); // why am I doing this when I then do a realloc?
DCS=(dihedral_coord_set*)realloc(DCS, 7*sizeof(dihedral_coord_set)); // space for up to 6 atoms

/*
// Holds pointers to coord of atoms in part of molecule that is to be rotated
branch *brnch;
brnch=(branch*)calloc(1, sizeof(branch)); // initialize
brnch=(branch*)realloc(brnch, 300*sizeof(branch)); // max size should be all atoms in the ligand.
//brnch[0].X=(coord_3D**)calloc(300, sizeof(coord_3D*));
*/

// Holds pointers to coord of atoms in part of molecule that is to be rotated
int nab=0; //Number of Atoms in Branch
coord_3D **brnch;
brnch=(coord_3D**)calloc(300, sizeof(coord_3D*)); // list of pointers to coord in branch

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
int linkage_resid=4; // residue with linkage that we are interested in
int lna; // linkage number of atoms that define the torsions. 
int core_ri[4];
core_ri[0]=2; // number of residues in core is stored in 0.
core_ri[1]=1;
core_ri[2]=2;
core_ri[3]=0;

/*************function call stuff end*******************/
printf("Here!\n");

// Find coords of atoms that are define torsions in linkage
lna=find_linkage_atom_coord(linkage_resid, DCS, &A);
printf("Found %d atoms involved in linkage dihedral\n",lna);

// now change torsion
double delta_phi;
delta_phi=0.26;
int tors=2; // torsion type. 2 would be psi.

find_branch_to_rotate(&A, linkage_resid, core_ri, brnch, &nab);

//brnch[0].X[1].i=0.555;
//printf("brnch[1].i=%f\n",(*brnch[1]).i);

//delta_phi=(size_limit+(window_size*$i*$k)); // 1st loop k will be 1, then -1, then +1 and so on

change_torsion_by(delta_phi, DCS, lna, tors, brnch, &nab);

char outname[15];
strcpy(outname, "output.pdb");
outputAsmblPDB(&A,outname);

return 0;
}
