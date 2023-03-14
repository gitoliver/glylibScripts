#include <glylib.h>
#include <mylib.h> 
#include <molecules.h>
void replaceAsnResidue(assembly *A, residue *R) {
int mi=0,ri=0,ai=0,aii=0;
//char out[20]; // name of output file

printf("Entered replaceAsnResidue function\n");
for(mi=0;mi<(*A).nm;mi++){
    for (ri=0;ri<(*A).m[mi][0].nr;ri++){
        if (strcmp((*A).m[mi][0].r[ri].N,"NLN")==0){
            strcpy((*A).m[mi][0].r[ri].N,"ZZZ"); // Remove atoms in ZZZ when writing out.
	    for(ai=0;ai<(*A).m[mi][0].r[ri].na;ai++){
                for(aii=0; aii<R->na ;aii++){
                    //printf("Checking %s\n",R->a[aii].N);
		    if (strcmp(R->a[aii].N, (*A).m[mi][0].r[ri].a[ai].N)==0){
                        printf("R->a[aii].x.i = %.2f, R->a[aii].x.j = %.2f, R->a[aii].x.k = %.2f\n",R->a[aii].x.i,R->a[aii].x.j,R->a[aii].x.k);
                        R->a[aii].x.i = A->m[mi][0].r[ri].a[ai].x.i;
                        R->a[aii].x.j = A->m[mi][0].r[ri].a[ai].x.j;
                        R->a[aii].x.k = A->m[mi][0].r[ri].a[ai].x.k;
                        printf("R->a[aii].x.i = %.2f, R->a[aii].x.j = %.2f, R->a[aii].x.k = %.2f\n",R->a[aii].x.i,R->a[aii].x.j,R->a[aii].x.k);
                        printf("Replaced coords of %s with %s, x=%.2f\n",R->a[aii].N, A->m[mi][0].r[ri].a[ai].N, R->a[aii].x.i);
                    }
                }
            }
        }
    }
}
strcpy(R->N,"NLN"); // rename ASN as NLN
printf("JOHN1\n");
/*
strcpy(out,"A1out.pdb");
outputAsmblPDB(A, out);
*/
return;
}
