/* This program was written by Spandana Makeneni to add header information to the pdb files for the carbohydrate builder and oligosaccharide library.The program takes in topology,coordinate and name of the pdb outputfile as the input and outputs two pdb files one with hydrogens and one without hydrogens(noh.odb).PLease note that the pdb file with no hydrogens is always named as noh.pdb. So avoid running this program twice in the same directory for different cases as it overwrites the exisitng noh.pdb file. I am working on fixing that now.

Modified by BLF on 20110701 to only write the "noh.pdb" file, but using the supplied filename.  
This is the way it is needed for Oliver Grant's work on the glycome. 

*/
#include <glylib.h>
#include <AMBER/amber.h>

#define USAGE "\n\
\tnew_headers  prmtop.top file.crd nameofoutputpdb\n"

int main(int argc,char* argv[]){

/*  fileset variable defintions 
	Pset = for handling topology file
	Cset = for coordinate file
	OUTset = pdb file with no hydrogens
*/

fileslurp pdbslurp;
fileset Pset,Cset,OUTset;
int i,mi,ri,ai,bi,branch=0;
assembly A;
molindex moli;
atom *atname;
char *string,isH;
string = (char*)calloc(3,sizeof(char));
if(argc<3){printf("USAGE: new_headers  prmtop.top file.crd nameofoutputpdb\n");}
//oprning all the input files
Pset.N=strdup(argv[1]);
Pset.F=myfopen(Pset.N,"r");
Cset.N=strdup(argv[2]);
Cset.F=myfopen(Cset.N,"r");
OUTset.N=strdup(argv[3]);
OUTset.F=myfopen(OUTset.N,"w");
//adding the remark cards
fprintf(OUTset.F,"TITLE      STRUCTURE GENERATED USING GLYCAM/WEB http://www.glycam.org\n");
fprintf(OUTset.F,"REMARK\n");
fprintf(OUTset.F,"REMARK     REFINEMENT\n");
fprintf(OUTset.F,"REMARK     PROGRAM     : AMBER SOFTWARE WITH GLYCAM06f FORCEFIELD\n");
fprintf(OUTset.F,"REMARK     AUTHORS     : GROUP OF R.J.WOODS\n");
fprintf(OUTset.F,"REMARK\n");
fprintf(OUTset.F,"REMARK     OTHER REFINEMENT REMARKS:\n");
fprintf(OUTset.F,"REMARK     **** USERS PLEASE NOTE ****\n");
fprintf(OUTset.F,"REMARK     OLIGOSACCHARIDES ARE HIGHLY FLEXIBLE IN SOLUTION.THIS SINGLE\n");
fprintf(OUTset.F,"REMARK     STRUCTURE WILL RARELY BE ADEQUATE TO DESCRIBE SOLUTION BEHAVIOR\n");
fprintf(OUTset.F,"REMARK\n");
fprintf(OUTset.F,"REMARK     COMPLIES WITH FORMAT V.3.20, 01-DEC-08\n");
fprintf(OUTset.F,"REMARK 250\n");
fprintf(OUTset.F,"REMARK 250 EXPERIMENTAL DETAILS\n");
fprintf(OUTset.F,"REMARK 250 EXPERIMENT TYPE :  THEORETICAL STRUCTURE\n");
fprintf(OUTset.F,"REMARK 250 DATE OF DATA COLLECTION : \n");
fprintf(OUTset.F,"REMARK\n");
fprintf(OUTset.F,"REMARK LINEAR SEQUENCE ");

A = load_amber_prmtop(Pset);
add_trajcrds_to_prmtop_assembly(Cset,&A,'r',0);

//loop for adding in the linear sequence and beanch sequence in the header information
for(mi=0;mi<A.nm;mi++){
        //printf("The number of molecules in this file are %d and this is the %d molecule\n",A.nm,mi+1);
        for(ri=0;ri<A.m[0][mi].nr;ri++){
                if(strstr(A.m[0][mi].r[ri].N,"0") != NULL && ri != A.m[0][mi].nr-1){
                        branch = 1;
                        }
                if(branch ==0){
			if(ri == A.m[0][mi].nr-1){
				fprintf(OUTset.F,"%s\n",A.m[0][mi].r[ri].N);
			}
			else{	
                        fprintf(OUTset.F,"%s ",A.m[0][mi].r[ri].N);
			}
                }
                else{
                        if(branch ==1 ){
                                fprintf(OUTset.F,"%s\n",A.m[0][mi].r[ri].N);
                                fprintf(OUTset.F,"REMARK BRANCH SEQUENCE ");
                                branch =0;
                                }
		}
	}
}

//loop for adding in the link cards
for(mi=0;mi<A.nm;mi++){
	//printf("The number of molecules in this file are %d and this is the %d molecule\n",A.nm,mi+1);
	for(ri=0;ri<A.m[0][mi].nr;ri++){
		//printf("The number of residues in this molecule are %d and this is the %d residue\n",A.m[0][mi].nr,ri+1);
		for(ai=0;ai<A.m[0][mi].r[ri].na;ai++){
			//printf("The number of atoms in this residue are %d and this is atom %d\n",A.m[0][mi].r[ri].na,ai+1);
			//printf("The number of bonds for this atom are %d\n",A.m[0][mi].r[ri].a[ai].nmb);
			for(bi=0;bi<A.m[0][mi].r[ri].a[ai].nmb;bi++){
				moli = A.m[0][mi].r[ri].a[ai].mb[bi].t;
				atname = &A.m[0][moli.m].r[moli.r].a[moli.a];
				//printf("the target residue number is %d\n",A.m[0][moli.m].r[moli.r].n);
				//printf("the source residue is %s\n",A.m[0][mi].r[ri].N);
				//printf("the target residue is %s\n",A.m[0][moli.m].r[moli.r].N);
				//printf("the target atom is %s\n",atname[0].N);
				if(A.m[0][moli.m].r[moli.r].n > ri+1 ){
						// printf("the target atom is %s\n",atname[0].N);
						 fprintf(OUTset.F,"LINK       %4s  %3s  %4d %18s  %3s  %4d\n",A.m[0][mi].r[ri].a[ai].N,A.m[0][mi].r[ri].N,A.m[0][mi].r[ri].n,atname[0].N,A.m[0][moli.m].r[moli.r].N,A.m[0][moli.m].r[moli.r].n);
						}
			}
		}
	}	
}

pdbslurp=get_assembly_PDB_ATOM_lines(&A,'y',0,'n',0);
//loop for adding in the pdb lines to the files outputpdbname.pdb and noh.pdb
for(i=0;i<pdbslurp.n;i++){
	isH='n';
	if(pdbslurp.L[i][12]=='H') isH='y';
	else if((isdigit(pdbslurp.L[i][12]!=0)||(pdbslurp.L[i][12]==' '))&&pdbslurp.L[i][13]=='H') isH='y';
	if(isH=='n'){
		fprintf(OUTset.F,"%s",pdbslurp.L[i]);
		} 
	}

fprintf(OUTset.F,"END\n");

fclose(OUTset.F);
return 0;
}
