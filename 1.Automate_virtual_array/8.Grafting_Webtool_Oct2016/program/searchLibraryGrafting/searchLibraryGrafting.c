#include <glylib.h>
//FUNCTION DECLARATIONS
int line_num(char *filename);
//int is_A_in_B(int A_rn, char A_table[][7][8], int B_rn, char B_table[][7][8], int *A_rnlist, int *B_rnlist, int *B_rns);
int is_A_in_B(int *pn0match, int A_ln, char A_table[][40][8], int An0lines, int B_ln, char B_table[][40][8], int Bn0lines, int *pB_neighbr, int fTtoLG[][40], int nbc);

// nbc = n0_budcores

//int main (void){
int main (int argc, char **argv){
if ( argc != 3 ) {
    printf("Usage: searchLibraryGrafting.exe $library $folderName\n");
}
else
{

char filename[50];
FILE * pFile;

//char wiggle='Y';
//argv[1] - '0'; // convert to integer
//char tmpwiggle=argv[1];
char library[200];
sprintf (library, "%s", argv[1]);

/*********************************READ TEMPLATE INFO**************************************/
int i=0,j=0,k=0,TnumRes; // counters
strcpy(filename, "LINK.txt");
TnumRes = line_num(filename);
char Ttable[40][40][8]; // LINK table for the template. 40 residues max, 6 entries LINK card (so why 40?), max 5 long (so why 8?)

pFile = fopen (filename,"r");
//printf("TnumRes=%d\n",TnumRes);

for (i=1;i<=TnumRes;i++){ 
    for (j=0;j<7;j++){
        fscanf (pFile, "%s", Ttable[i][j]);
       // printf("table[%d][%d] is %s\n",i,j,Ttable[i][j]);
    }
}
fclose (pFile);

/************************GET_LIST_OF_LINENUMBERS_OF_RESIDUES*********************************/

/*int T_rnlist[TnumRes];
// Now get an organized list of line numbers of the residue numbers in Ttable[i][6][0]. So T_rnlist[1]=linenumberthat residue1 is on RHS.
for (i=2;i<=(TnumRes+1);i++){
    for (j=2;j<=(TnumRes+1);j++){
        if (atoi(Ttable[j][6])==i){ 
            T_rnlist[i]=j;
    //        printf("T_rnlist[%d]=%d\n",i,j); //residue number i is on line number j
        }
    }
}*/
        
// load in the template
assembly T; // Template assembly
strcpy(filename, "ligand.pdb");
load_pdb(filename,&T);

// wiggling mod
int mi=0,ri=0;
//int core_ri[TnumRes];
int core_ri[50]; // gets added to during wiggling.
core_ri[0]=0;
for(mi=0;mi<T.nm;mi++){
    for(ri=0;ri<T.m[mi][0].nr;ri++){
        core_ri[0]=((core_ri[0])+1);
        core_ri[core_ri[0]]=T.m[mi][0].r[ri].n; //wtf! I am using .n instead of the ri. should be called core_id 
        //printf("core_ri[%d]=%d\n",core_ri[0],core_ri[core_ri[0]]);
    }
}


/************************* COUNT NUMBER OF BRANCHES IN LIGAND********************************/
// Used for vdw overlap so can exclude ligand from count
int nbr;
nbr=T.nm;
/*********************END*COUNT NUMBER OF BRANCHES IN LIGAND*END*****************************/

/*****************************END*READ TEMPLATE INFO*END**********************************/

/********************List the contents of RESULTS_library_search**************************/
DIR *mydir = opendir(library);
struct dirent *entry = NULL;
int numfiles;
i=0;

// same as char d_list[2000][300]; but can eat more memory :)
char (*d_list)[200];
//d_list = malloc(65903 * sizeof *d_list);
d_list = malloc(62000 * sizeof *d_list); //70000 as human glycome contains ~60000 files
if (!d_list) {
fputs("Nothing in directory.\n", stderr);
exit(EXIT_FAILURE);
}

//char d_list[2000][300];// dynamically set/expand this? 1024 seems to be the limit anyway
while((entry = readdir(mydir))){ /* If we get EOF, the expression is 0 and the loop stops. */
    if ( (strcmp(entry->d_name,".")!=0) && (strcmp(entry->d_name,"..")!=0) ){ // skip . and ..
        //printf("%s\n", entry->d_name);
        //strcpy(d_list[i], library);
        sprintf(d_list[i], "%s/%s", library, entry->d_name);
        //strcat(d_list[i], entry->d_name);
        i++;
    }
}
closedir(mydir);
numfiles=i; 
for (j=0;j<(numfiles);j++){
    //printf("d_list[%d]=%s\n",j,d_list[j]);
}
/****************END*List the contents of RESULTS_library_search*END**********************/
//printf("numfiles=%d\n",numfiles);


//assembly LG; // Library Glycan, CORE2ROH , Branch , Grafted assembly
//char buffer[60]; // stores info from library glycan file, we select out LINK info
char LGtable[40][40][8]; // more than 40 resids in a glycan? 40 residues max, 6 entries LINK card (so why 40?), max 5 long (so why 8?)
int n0LGres; // number of Library Glycan residues
int x,z;// counters

int LG_neighbr[30]; // list of neighbours to LG residue
int n0_budcores=0; // number of budcores (sequences which match template) in a Library Glycan
int budcore[30][30]; // budcore[budcoreNumber][residuenumber]
int n0match; // number of residues which match template. when=TnumRes we have a budcore

int TtoLG[40][40]; // TtoLG[TResidueNumber][budcorenumber]=LGResidueNumber that matches

FILE *idfile;
char searchResultsFile[300];
sprintf(searchResultsFile, "/scratch1/webdev/grafting/runs/%s/0.grafting/searchLibraryResults.txt", argv[2]);

/*************************Read in Library Glycan and store LINK information **********************************/

for (i=0;i<(numfiles);i++){
    //printf("\n");
    //deallocateAssembly(&LG);
    //load_pdb(d_list[i],&LG);
    //set_nbonds_for_atoms_in_assembly(&LG);
    //printf("LG num atoms=%d\n",LG.na);
    //printf("filesleft=%d\n",(numfiles-i));
    // read LINK information into a Table so can compare against template
    FILE * pFile;
    printf("d_list[%d]=%s\n",i,d_list[i]);
    pFile = fopen (d_list[i],"r");
    n0LGres=0;
    j=0;// to match Tstring which starts on 1 as uses TnumRes as top of counter.
    //strcpy(buffer,"reset_string");
    char buffer[60];
    sprintf (buffer, "%s", "reset_string");
    while (strcmp(buffer,"HETATM")!=0 && strcmp(buffer,"ATOM")!=0){
        fscanf (pFile, "%s",buffer);
        if (strcmp(buffer,"LINK")==0){
            j++; // j will be the number of residues in LG
            strcpy(LGtable[j][0],buffer);
            for (k=1;k<7;k++){ // for the next 6 entries
                fscanf (pFile, "%s", LGtable[j][k]);
                //printf("LGtable[%d][%d]=%s\n",j,k,LGtable[j][k]);
            }
        }
    }
    fclose(pFile);
    n0LGres=j;
    //printf("Number of residues in glycan library file, n0LGres=%d\n",n0LGres);

    /*int LG_rnlist[n0LGres];
    // Now get an organized list of line numbers of the residue numbers in table[i][6][0]. So rnlist[1]=linenumberthat residue1 is on RHS.
    for (i=2;i<=(n0LGres+1);i++){
                for (j=2;j<=(n0LGres+1);j++){
                    if (atoi(LGtable[j][6])==i){
                            LG_rnlist[i]=j;
                            printf("LG_rnlist[%d]=%d\n",i,j); //residue number i is on line number j
                    }
            }
    }*/
    // read LINK info END

    /******************************* FIND IF LIBARY GLYCAN CONTAINS TEMPLATE *******************************/

    x=0;
    z=0; 
    //FIND resids of residues in LGtable that match Ttable
    // Starting from resid 2 in Ttable. Find all matches in lG. E.g. looking to match a-Gal(1- for each match:
    n0_budcores=0;
    n0match=0;
    for (j=1;j<=n0LGres;j++){
        if (Ttable[1][5][1]==LGtable[j][5][1] && Ttable[1][5][2]==LGtable[j][5][2] && strcmp(Ttable[1][4],LGtable[j][4])==0){ // for each match
            //printf("matched the 2nd residue in Ttable [1][5]X to the one on row %d of LGtable\n",j);
            LG_neighbr[0]=atoi(LGtable[j][6]);
            n0match=1;
            TtoLG[atoi(Ttable[1][6])][n0_budcores]=atoi(LGtable[j][6]);
            //printf("Checking LGtable[%d][6]=%d\n",j,LG_neighbr[0]);
//            printf("TnumRes=%d j=%d\n",TnumRes,j);
            x=is_A_in_B(&n0match, 1, Ttable, TnumRes, j, LGtable, n0LGres, LG_neighbr, TtoLG, n0_budcores);
            //printf("x=%d\n",x);
            if (x==TnumRes){ // if success and this is a budcore
                //printf("LGtable[%d] is a budcore\n\n",j);
                TtoLG[1][n0_budcores]=atoi(LGtable[j][3]); // know how to convert ROH in template
                n0_budcores++;
                printf("BUDCORE!\n");
                for (k=0;k<TnumRes;k++){
                    budcore[n0_budcores][k]=LG_neighbr[k];
                    //printf("budcore[%d][%d]=%d\n",n0_budcores,k,budcore[n0_budcores][k]);
                }
            }
            //else {printf("No Match!\nTnumRes=%d,n0match=%d\n\n",TnumRes,n0match);
    //        for (k=0;k<n0match;k++){
    //            printf("LG_neighbr[%d]==%d\n",k,LG_neighbr[k]);
    //        }
        }
    }
    if(n0_budcores>0){ // Record glycan for grafting
        idfile = fopen(searchResultsFile, "a+");
        fprintf(idfile,"%s\n",d_list[i]);
        fclose(idfile);
    }

}
printf("Finished all\n");
}
return 0;
}

//FUNCTIONS

int line_num(char *filename){
FILE *f;
char c;
int lines = 0;
f = fopen(filename, "r");
if(f == NULL)
return 0;
while((c = fgetc(f)) != EOF)
if(c == '\n')
lines++;
//fclose(f);
//if(c != '\n')
//lines++;
return lines;
}

int is_A_in_B(int *pn0match, int A_ln, char A_table[][40][8], int An0lines, int B_ln, char B_table[][40][8], int Bn0lines, int *pB_neighbr, int fTtoLG[][40], int nbc){// return residue numbers of B that match A in B_rns
int i,j,x=0;
//printf("pn0match=%d, A_ln=%d, Resid to be checked=%c%c, An0lines=%d, B_ln=%d \n",*pn0match,A_ln,A_table[A_ln][5][1],A_table[A_ln][5][2],An0lines,B_ln);
for (i=1;i<=An0lines;i++){
//    printf("A_table[%d][3]=%s,A_table[A_ln][6]=%s\n",i,A_table[i][3],A_table[A_ln][6]);
    if (strcmp(A_table[i][3],A_table[A_ln][6])==0){ // then residue on line A_ln is connected to some other resid on row i
    //    printf("A rn=%s rN=%s:",A_table[i][6],A_table[i][5]);
        for (j=1;j<=Bn0lines;j++){ // Let's see if B has the same neighbour...
            if (strcmp(B_table[j][3],B_table[B_ln][6])==0){ //if B_rn has neighbourd test them
  //              printf("\nB rn=%s rN=%s ",B_table[j][6],B_table[j][5]); // this is the neighbour that will be tested
            //    if (A_table[i][2][1]==B_table[j][2][1] && A_table[i][2][2]==B_table[j][2][2] && strcmp(A_table[i][1],B_table[j][1])==0){
            //    if LG residue also has a neighbour is it the same residue type LB for example and connected with the same atom?
                if (A_table[i][5][1]==B_table[j][5][1] && A_table[i][5][2]==B_table[j][5][2] && strcmp(A_table[i][1],B_table[j][1])==0){
                    fTtoLG[atoi(A_table[i][6])][nbc]=atoi(B_table[j][6]);
                    pB_neighbr[*pn0match]=atoi(B_table[j][6]); //RHS resid is resid of neighbour
                    (*pn0match)++; //
//                    printf("...MATCH! number=%d\n",*pn0match);
                    /***********RECURSIVE CALL********************************/
                    // is_A_in_B(&n0match, TnumRes, Ttable, j, LGtable, LG_neighbr);
                    x=is_A_in_B(pn0match, i, A_table, An0lines, j, B_table, Bn0lines, pB_neighbr, fTtoLG, nbc);
                }
            }
        }
    }
}
x=*pn0match;
return x;
}
