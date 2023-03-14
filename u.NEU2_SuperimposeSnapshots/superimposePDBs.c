#include <glylib.h>
#define CSA 36.31681103


main( int argc, char *argv[] )  {
//int main(void){

assembly A,B,C;
/********************List the contents of RESULTS_library_search**************************/
DIR *mydir = opendir(argv[1]);
struct dirent *entry = NULL;
int numfiles;
int i=0,j=0;
int mi=0,ri=0,ai=0;
char out[1000];
double min=50000,max=0,overlap=0,totalOverlap=0,ovrlap_eqv=0;


// same as char d_list[2000][300]; but can eat more memory :)
char (*d_list)[200];
d_list = malloc(65903 * sizeof *d_list);

//char d_list[2000][300];// dynamically set/expand this? 1024 seems to be the limit anyway
if( argc == 2 ){

while((entry = readdir(mydir))){ /* If we get EOF, the expression is 0 and the loop stops. */
        if ( (strcmp(entry->d_name,".")!=0) && (strcmp(entry->d_name,"..")!=0) ){ // skip . and ..
                printf("%s\n", entry->d_name);
                //strcpy(d_list[i], "./pdb_Files/");
		strcpy(d_list[i], argv[1]);
                strcat(d_list[i], entry->d_name);
                i++; 
        }
}
closedir(mydir);
numfiles=i;
/****************END*List the contents of RESULTS_library_search*END**********************/
printf("numfiles=%d\n",numfiles);

for (i=0;i<(numfiles);i++){
    load_pdb(d_list[i],&A);
    for(mi=0;mi<A.nm;mi++){ // for each molecule
        for(ri=0;ri<A.m[mi][0].nr;ri++){ // for each residue in molecule
            if(strcmp(A.m[mi][0].r[ri].N,"0SA")==0){
                for(ai=0;ai<A.m[mi][0].r[ri].na;ai++){ 
                    if (strcmp(A.m[mi][0].r[ri].a[ai].N,"C1")==0){strcpy(A.m[mi][0].r[ri].a[ai].N,"a1");}
                    if (strcmp(A.m[mi][0].r[ri].a[ai].N,"C2")==0){strcpy(A.m[mi][0].r[ri].a[ai].N,"a2");}
                    if (strcmp(A.m[mi][0].r[ri].a[ai].N,"C3")==0){strcpy(A.m[mi][0].r[ri].a[ai].N,"a3");}
                    if (strcmp(A.m[mi][0].r[ri].a[ai].N,"O6")==0){strcpy(A.m[mi][0].r[ri].a[ai].N,"a4");}
                }
            }
        }
    }
    load_pdb("ligand.pdb",&B);
    superimpose4atoms(&A,&B);

    strcpy(out,d_list[i]);
    outputAsmblPDB(&A, out);
    
    load_pdb("receptor.pdb",&C);
    overlap=find_vdw_clashes_pairwize_between_Assemblies(&A,&C,"blah",&j,j);
    ovrlap_eqv=(overlap / CSA);
    totalOverlap+=ovrlap_eqv;
    printf("\n\novrlap_eqv=%.2f, filesLeft=%d\n\n",ovrlap_eqv,(numfiles - i));
    if (ovrlap_eqv < min) { min=ovrlap_eqv; }
    if (ovrlap_eqv > max) { max=ovrlap_eqv; }
}
double avrOvrlp=(totalOverlap / numfiles);
printf("avrOvrlp=%2f\nmin=%2f\nmax=%2f\n",avrOvrlp,min,max);

} //end of if statement

else { printf("Usage ./scriptName path2files/\nWarning this program overwrites files!"); }
return 0;
}
