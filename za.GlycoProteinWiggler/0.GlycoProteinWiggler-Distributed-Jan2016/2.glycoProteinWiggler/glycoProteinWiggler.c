#include <glylib.h>

int main (int argc, char *argv[]){
if (argc!=2) {
    printf("Usage: glycoProteinWiggler.exe input.txt\n");
}
if (argc==2) { // If started correctly

int i=0,j=0,k=0,l=0,m=0,n=0;
assembly A;
assembly B;

char *r_file;
r_file=(char*)calloc(500,sizeof(char));
char *g_dir;
g_dir=(char*)calloc(500,sizeof(char));
char *r_resid;
r_resid=(char*)calloc(10,sizeof(char)); // residue numbers
char *g_resid;
g_resid=(char*)calloc(100,sizeof(char)); // glycan file name
char **r_resids;
r_resids = (char**)calloc(100,sizeof(char*));
char **g_resids;
g_resids = (char**)calloc(100,sizeof(char*));

fileset Pset;
fileslurp Pslurp;

/* opening and reading all the input files*/
if(argc==2){
Pset.N=strdup(argv[1]);//input file
Pset.F=myfopen(Pset.N,"r");
Pslurp = slurp_file(Pset);
fclose(Pset.F);
}

while(i<Pslurp.n){
        if(strstr(Pslurp.L[i],"Protein:") != NULL){
                i++;
                sscanf(Pslurp.L[i],"%s",r_file);
        }
        if(strstr(Pslurp.L[i],"Glycans:") != NULL){
                i++;
                sscanf(Pslurp.L[i],"%s",g_dir);
        }
        if(strstr(Pslurp.L[i],"Protein Residue list:") != NULL){
                i++;
                while(strstr(Pslurp.L[i],"END") == NULL){
                        sscanf(Pslurp.L[i],"%s",r_resid);
                        //names[j]=(char*)calloc(strlen(atom),sizeof(char));
                        r_resids[j]=strdup(r_resid);
                        printf("1.  r_resids[%d] are >>>%s<<<\n",j,r_resids[j]);
                        i++;
                        j++;
                }
        }
        if(strstr(Pslurp.L[i],"Glycan id list:") != NULL){
                i++;
                while(strstr(Pslurp.L[i],"END") == NULL){
                        sscanf(Pslurp.L[i],"%s",g_resid);
                        //names[j]=(char*)calloc(strlen(atom),sizeof(char));
                        g_resids[k]=strdup(g_resid);
                        printf("1.  g_resids[%d] are >>>%s<<<\n",k,g_resids[k]);
                        i++;
                        k++;
                }
        }
        i++;
}
int num_residues=j;
printf("num_residues=%d\n",j);
/********Read glycan directory ************/
/*
DIR *mydir = opendir(g_dir);
struct dirent *entry = NULL;
int num_glycans;
char (*d_list)[80];
d_list = malloc(80 * sizeof *d_list);
while((entry = readdir(mydir))){ // If we get EOF, the expression is 0 and the loop stops.
    if ( (strcmp(entry->d_name,".")!=0) && (strcmp(entry->d_name,"..")!=0) ){ // skip . and ..
        printf("Glycan in direcory is %s\n", entry->d_name);
        //sprintf(d_list[l], "./%s%s",g_dir,entry->d_name);
        sprintf(d_list[l], "%s", entry->d_name);
        l++;
    }
}
closedir(mydir);
num_glycans=l;
*/
// Yes this is weird. I could have just used namelist instead of d_list. No reason for this.
struct dirent **namelist;
char (*d_list)[80]; // why is this hardcoded?
l = scandir (g_dir, &namelist, NULL, alphasort);
d_list = malloc(l * sizeof *d_list);
int num_glycans=l;

if (l < 0) {
    perror("scandir");
}
else {
    while (l--) {
        printf("%s\n", namelist[l]->d_name);
        sprintf(d_list[l], "%s", namelist[l]->d_name);
        free(namelist[l]);
    }
    free(namelist);
}

printf("num_glycans=%d\n",num_glycans);
for (l=0;l<(num_glycans);l++){
        printf("d_list[%d]=%s\n",l,d_list[l]);
}
/*****************************************/
int mi=0,ri=0,len=0;
char path[100];
load_pdb(r_file, &B);
printf("done\n");
//temp load single glycan
//sprintf(path, "./%s%s",g_dir,d_list[0]);
//load_pdb(path, &A);
//set_nbonds_for_atoms_in_assembly(&A);
//for (ri=0; ri<A.m[0][0].nr ;ri++){ if (A.m[0][0].r[ri].na > 1){ set_smallest_rings_from_residue_atom_nodes(&A.m[0][0].r[ri]);}}
//printf("done\n");
// temp over
char a1[3],a3[3],a4[3];
char out[100],best[100];
residue *R;
R=(residue *)calloc(1, sizeof(residue));
int temp=0;
int core_ri[50];
double overlap=0.0,lowerlap=1000.0;
int lowerlapRes=0;
sprintf(a1,"N");
sprintf(a3,"CA");
sprintf(a4,"CB");

// For each protein residue to be glycosylated
for (m=0; m<num_residues; m++){
    // Get protein residue
    for(mi=0; mi<B.nm; mi++){
        for (ri=0; ri<B.m[mi][0].nr; ri++){
            //printf("B.m[%d][0].r[%d].n=%d,r_resids[%d]=%s\n",mi,ri,B.m[mi][0].r[ri].n,m,r_resids[0]);
            temp = atoi (r_resids[m]);
            if (B.m[mi][0].r[ri].n == temp){
                R = &B.m[mi][0].r[ri];
                //strcpy(R->N,"NLN");
                printf("\n----------------\nm=%d,temp=%d, Residue ID is %d or %d and should equal %d\n",m,temp,R->n,(*R).n,B.m[mi][0].r[ri].n);          
            }
        }
    }
    // Get corresponding glycan
    len = strlen(g_resids[m]);
    for (n=0; n<num_glycans ; n++){
        //printf ("Checking for Glycan %s in dlist[%d]=%s\n",g_resids[m],n,dlist[n]);
        if (strncmp (g_resids[m], d_list[n], len) == 0){
            printf ("Found glycan %s\n",d_list[n]);
            sprintf(path, "./%s%s",g_dir,d_list[n]);
            load_pdb(path, &A); printf("done\n");
            // Set ring atoms
            for (ri=0; ri<A.m[0][0].nr ;ri++){
                if (A.m[0][0].r[ri].na > 1){ set_smallest_rings_from_residue_atom_nodes(&A.m[0][0].r[ri]); }
            }
            // Superimpose
            superimpose3atoms(&A, R, a1, a3, a4);
            // Check for and Resolve Glycan-Protein Clashes
            core_ri[0]=1; core_ri[1]=0;
            overlap = wigglerGP(&A, &B, core_ri, 30);
            if ( overlap < lowerlap ) {
                lowerlap=overlap; 
                lowerlapRes=n;
                printf("New lowerlap=%.1f\n",lowerlap);
                sprintf(best, "res-%s-out.pdb", r_resids[m]);
                printf("r_resids[%d]=%s\n",m,r_resids[m]);
                replaceAsnResidue(&A, R);
                outputMolPDB_NoRenameResid(&A.m[0][0], best);
                //outputAsmblPDB(&A, best);
            }
            printf("%s overlap=%.2f\n",d_list[n],overlap);
        }
        if ( lowerlap < 2.0 ) {
            printf("Found a structure that fits\n");
            n=num_glycans; //Should stop going through the other glycans
        }
    }
    printf("%s had lowest overlap(%.1f) for resid %s\n",d_list[lowerlapRes],lowerlap,r_resids[m]);
    if ( lowerlap < 2.0 ) {
        load_pdb(best, &A); printf("done\n");
        replaceAsnResidue(&A, R);
        mergeAsmblyGP(&B, &B, &A);
    }
    else {
        printf("\n**********************\nCould not find solution for %s\n\n",r_resids[m]);
    }
    
    lowerlap=1000.0; //reset
}

printf("Done...Writing glycosylated.pdb\n");
sprintf(out, "glycosylated.pdb");
//outputAsmblPDB(&B, out);
outputMolPDB_NoRenameResid(&B.m[0][0], out);
//if (j!=k){printf("Input requires a glycan for each protien residue to be glycosylated\n");}
}
return 0;
}

