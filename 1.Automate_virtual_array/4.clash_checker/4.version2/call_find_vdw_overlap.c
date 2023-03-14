#include <glylib.h>

int main (void){

char filename[50];
char outname[50];
FILE * pFile;
double overlap;


assembly L, R; // Ligand assembly
strcpy(filename, "ligand.pdb");
T=*load_pdb(filename);
strcpy(filename, "receptor.pdb");
R=*load_pdb(filename);

strcpy(outname, "out.pdb");

overlap=find_vdw_clashes_pairwize_between_Assemblies(&L, &R, char *fileprefix);

return;
}




