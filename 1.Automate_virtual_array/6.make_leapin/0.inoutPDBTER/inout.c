#include <glylib.h>
//function prototype
int main (void){
assembly A; // glylib structure which holds molecule information
char branch[25];// for loading file into assemblies. clunky.
/**********************************READ IN DATA*************************************************/

strcpy(branch,"in.pdb"); //for loading file into assembly. clunky.
//load pdb into assembly A
A=*load_pdb(branch);

//set_nbonds_for_atoms_in_assembly(&A); // send the address of A

char out[25];
//strcpy(out,"out.pdb");
//outputAsmblPDB(&A, out);

strcpy(out,"outTER.pdb");
outputAsmblPDBTER(&A, out);

return 0;
}
