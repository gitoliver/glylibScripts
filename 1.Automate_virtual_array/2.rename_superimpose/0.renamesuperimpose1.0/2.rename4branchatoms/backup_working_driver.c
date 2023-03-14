//This script will be called from within 2.make_branches for each branch structure. It will rename the atoms.

// requires core atom name (aN), branch atom name, core resid (rn) and branch resid
// in these files:Smotif_core_aN_1.txt, Smotif_branch_aN_1.txt, Smotif_core_rn_1.txt, Smotif_branch_rn_1.txt
// the 1 in the file name is redundant now

#include <glylib.h>
//function prototype
int main (void){
assembly A;
char branch[25];
char aN[3];

strcpy(branch,"branch.pdb"); //either take this in as a variable or write a pdb called branch each time.
//load pdb into assembly A
A=*load_pdb(branch);

int nmot; //number of motifs, don't think I need this
FILE * pFile;

  pFile = fopen ("9.MOTIFS/Snmotif.txt","r");
  fscanf (pFile, "%d", &nmot);
  fclose (pFile);
  printf ("Number of motifs are: %d \n",nmot);

int i;
char filename[33]; //however long you want, this is bad, will use malloc when it all works
int core_rn,branch_rn; //residue numbers in the motif
char core_aN[4], branch_aN[4]; // atom names in the motif, again use malloc when it all works
char buffer[3]; // 3 digit buffer, and again malloc

// get atom names and residue numbers
for (i=1; i<=nmot; i++) // again not sure why I did this. Objectives have changed.
	{
	//convert int i into a string. to put into file names to be opened.
	sprintf(buffer, "%d", i);

	strcpy(filename, "9.MOTIFS/Smotif_core_aN_");
	strcat(filename, buffer);
	strcat(filename, ".txt");	
	pFile = fopen (filename,"r");//open the file need check for NULL
        fscanf (pFile, "%s", core_aN);// store contents of file
        fclose (pFile);

	strcpy(filename, "9.MOTIFS/Smotif_branch_aN_");
        strcat(filename, buffer);
        strcat(filename, ".txt");
        pFile = fopen (filename,"r");
        fscanf (pFile, "%s", branch_aN);
        fclose (pFile);
	
	strcpy(filename, "9.MOTIFS/Smotif_core_rn_");
        strcat(filename, buffer);
        strcat(filename, ".txt");
        pFile = fopen (filename,"r");
        fscanf (pFile, "%d", &core_rn);
        fclose (pFile);

	strcpy(filename, "9.MOTIFS/Smotif_branch_rn_");
        strcat(filename, buffer);
        strcat(filename, ".txt");
        pFile = fopen (filename,"r");
        fscanf (pFile, "%d", &branch_rn);
        fclose (pFile);

}


/*
	for (i=0;i<33;++i){
        	printf("%c",filename[i]);
	}
 	pFile = fopen (filename,"r");
	fscanf (pFile, "%s", core_aN);
	fclose (pFile);

for (i=0;i<3;i++){
	printf("\n%c",core_aN[i]);
*/

set_nbonds_for_atoms_in_assembly(A); 
int mi,ri,ai,nmb;

//count number of bonds branch atom has
for(mi=0;mi<A.nm;mi++){
        for(ri=0;ri<A.m[mi][0].nr;ri++){
                if(A.m[mi][0].r[ri].n==branch_rn){// look only at resid1
                        for(ai=0;ai<A.m[mi][0].r[ri].na;ai++){
                                if(strcmp(A.m[mi][0].r[ri].a[ai].N,branch_aN)==0){
                                        nmb=A.m[mi][0].r[ri].a[ai].nmb;}
                        }
                }
        }
}

if (nmb < 3)
printf("\n\nNumber of bonds=%d\n\n",nmb);
else 
printf("\n\nWe have our a1 atom! With Number of Bonds=%d\n\n",nmb);



//if A.m[mi][0].r[ri].a[ai].nmb 
	

return 0;
}

