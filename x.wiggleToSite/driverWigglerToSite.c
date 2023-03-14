#include <glylib.h>

int main (int argc, char *argv[]){
if (argc<6) {
    printf("Usage: wigglerToSite.exe glycan.pdb target.pdb residueToMove targetResidue coreresidue1 coreresidue2 coreresidueN...\n");
}
if (argc>5) { // If started correctly
   
    int core_ri[50]; // gets added to during wiggling.
    int i=0;
    int ri=0;
    int ready=0;
    int residToMove=0;
    int targetResid=0;

    printf("Run command used:\n");
    for (i=0;i<argc;i++){ 
        printf("%s ",argv[i]); // Keep record of run command
    }
    printf("\n");    

    core_ri[0]=(argc-5);
    for (i=5;i<(argc);i++){
        core_ri[(i-4)]=atoi(argv[i]);
    }
    for (i=0;i<=core_ri[0];i++){
        printf("core_resnums[%d]=%d\n",i,core_ri[i]);
    }
    // Load pdb into assembly 
    assembly A,B;
    load_pdb(argv[1], &A);
    for (ri=0;ri<A.m[0][0].nr;ri++){
        if (A.m[0][0].r[ri].na>1){
            set_smallest_rings_from_residue_atom_nodes(&A.m[0][0].r[ri]);
        }
        for (i=1;i<=core_ri[0];i++){
            if (A.m[0][0].r[ri].n==core_ri[i]){
                core_ri[i]=ri; //Replace the residue numbers with residue indices.
            }
        }
    }
    for (i=0;i<=core_ri[0];i++){
        printf("core_ri[%d]=%d\n",i,core_ri[i]);
    }
    load_pdb(argv[2], &B);
    int tmp1=atoi(argv[3]);
    int tmp2=atoi(argv[4]);
    printf("residToMove=%d,targetResid=%d\n",tmp1,tmp2);
    //printf("%s\n",A.m[0][0].r[1].a[ai].N);
    for(ri=0;ri<A.m[0][0].nr;ri++){
        if (A.m[0][0].r[ri].n==tmp1){residToMove=ri;printf("Found residToMove=%d\n",ri);ready++;}
    }
    for(ri=0;ri<B.m[0][0].nr;ri++){
        if (B.m[0][0].r[ri].n==tmp2){targetResid=ri;printf("Found targetResid=%d\n",ri);ready++;}
    }

    if (ready==2) {
        printf("Ready Eddie!\n");
        wigglerToSite(&A,&B,core_ri,30,residToMove,targetResid);
    }
}
return 0;
}

