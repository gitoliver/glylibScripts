#include <glylib.h> // for load_pdb function
#include <molecules.h>
#include <mylib.h>

void mergeAsmblyGP(assembly* merged, assembly* A, assembly *B){
printf("Entered Merge Assembly function...\n");

char file_name[50];
strcpy(file_name,"temp.pdb");

//FIRST RENUMBER GLYCAN TO MATCH END OF PROTIEN
int mi=0,ri=0,rii=0,highResNum=0;

mi = (A->nm - 1); //Check last molecule in protein
ri = (A->m[mi][0].nr -1); //Check last residue in that molecule
highResNum = A->m[mi][0].r[ri].n; // Get the residue number
for (rii=0; rii < B->m[0][0].nr; rii++){
    B->m[0][0].r[rii].n = highResNum;
    printf("Changed resnum of %s to %d\n",B->m[0][0].r[rii].N,B->m[0][0].r[rii].n);    
    highResNum++; // The first residue is ZZZ,it will be removed. I want 4YB to be +1 from end. This works.
}
//outputMolPDB(&(*A).m[0][0], file_name);
outputMolPDB_NoRenameResid(&(*A).m[0][0], file_name); //Glycan should be all in one molecule from now on. outputAsmbl now adds TER cards and needs to to work
//outputAsmblPDB_NoRenameResidTER(A, file_name);
printf("LukeDone\n");
//outputAsmblPDB(A, file_name); // overwrites temp file and put A in there
//outputAsmblPDBNoTER(A, file_name);
appendAsmblPDBGP(B, file_name); // adds B onto the bottom of A. Written by Oliver so no TER cards need to be added for it to work.
load_pdb(file_name,merged); // loads in this pdb file into one assembly
//int ri=0;
//for (ri=0;ri<(*merged).m[0][0].nr;ri++){
//        if ((*merged).m[0][0].r[ri].na>1){
//                set_smallest_rings_from_residue_atom_nodes(&(*merged).m[0][0].r[ri]);
//        }
//}
return;
}
