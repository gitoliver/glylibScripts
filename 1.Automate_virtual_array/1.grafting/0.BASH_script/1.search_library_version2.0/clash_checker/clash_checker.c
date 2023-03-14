//This script will be called from within 2.make_branches for each branch structure. It will rename the atoms to be superimposed as a1 a2 a3 and a4.

// requires core atom name (aN), branch atom name, core resid (rn) and branch resid
// in these files:core_aN.txt, branch_aN.txt, core_rn.txt, branch_rn.txt
// also requires: branch.pdb: the current branch.pdb whose atoms will be renamed

//branch.pdb contains a branch of the library glycan and the core sacc it is connected to. This function renames the appropriate atoms for the superimpose function

#include <glylib.h>
//function prototype
int main (void){
assembly A,B; // glylib structure which holds molecule information
char ligand[25], receptor[25];// for loading file into assemblies. clunky.
int xsA=-1,xsB=-1; //options for which co-ord set to use. set = -1 for main co-ord set.
/**********************************READ IN DATA*************************************************/

strcpy(ligand,"ligand.pdb"); //for loading file into assembly. clunky.
A=*load_pdb(ligand);

strcpy(receptor,"receptor.pdb"); //for loading file into assembly. clunky.
B=*load_pdb(receptor);

find_vdw_clashes_pairwize_between_Assemblies(&A,xsA,&B,xsB);

return 0;
}
