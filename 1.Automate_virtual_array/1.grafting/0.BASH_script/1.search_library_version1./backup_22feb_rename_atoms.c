//This script will be called from within 2.make_branches for each branch structure. It will rename the atoms to be superimposed as a1 a2 a3 and a4.

// requires core atom name (aN), branch atom name, core resid (rn) and branch resid
// in these files:core_aN.txt, branch_aN.txt, core_rn.txt, branch_rn.txt
// also requires: branch.pdb: the current branch.pdb whose atoms will be renamed

//branch.pdb contains a branch of the library glycan and the core sacc it is connected to. This function renames the appropriate atoms for the superimpose function

#include <glylib.h>
//function prototype
int main (void){
assembly A,B; // glylib structure which holds molecule information
FILE * pFile;
char branch[25], cplx[25];// for loading file into assemblies. clunky.
char filename[20]; //hardcoded .txt file names. see below.
char core_aN[3], branch_aN[3], connect_aN_1[3], connect_aN_2[3], connect_aN_3[3],TEMP[3]; // stores atom names of core, branch and connected atoms
char cplx_a1[3], cplx_a2[3], cplx_a3[3], cplx_a4[3], cplx_core_rN[3];
int core_rn,branch_rn; //stores residue numbers of core and branch structures
int mi,ri,ai,nb;
int c_ai,c_ri,c_mi;
int b_ai,b_ri,b_mi;
int connect_ai, next_connect_ai;
int tmpi1=0,tmpi2=0,tmpi3=0,tmpi4=0; /* a dummy index to make some bits more readable */

/**********************************READ IN DATA*************************************************/

strcpy(branch,"branch.pdb"); //for loading file into assembly. clunky.
//load pdb into assembly A
A=*load_pdb(branch);

// get atom names and residue numbers 
strcpy(filename, "core_aN.txt");
pFile = fopen (filename,"r");//open the file need check for NULL
fscanf (pFile, "%s", core_aN);// store contents of file
fclose (pFile);

strcpy(filename, "branch_aN.txt");
pFile = fopen (filename,"r");
fscanf (pFile, "%s", branch_aN);
fclose (pFile);
	
strcpy(filename, "core_rn.txt");
pFile = fopen (filename,"r");
fscanf (pFile, "%d", &core_rn);
fclose (pFile);

strcpy(filename, "branch_rn.txt");
pFile = fopen (filename,"r");
fscanf (pFile, "%d", &branch_rn);
fclose (pFile);
/***********************************************************************************************/

set_nbonds_for_atoms_in_assembly(&A); // send the address of A

/* DEBUGGING
printf("\n\n");
for(mi=0;mi<A.nm;mi++){ // for each molecule
        for(ri=0;ri<A.m[mi][0].nr;ri++){
		for(ai=0;ai<A.m[mi][0].r[ri].na;ai++){
			for (bi=0;bi<A.m[mi][0].r[ri].a[ai].nb;bi++){
				printf("mi=%d ri=%d ai=%d bi=%d t.a=%d\n",mi,ri,ai,bi,A.m[mi][0].r[ri].a[ai].b[bi].t.a);
			}
		}
	}
}
*/

//find ai of branch_aN and set to b_ai
for(mi=0;mi<A.nm;mi++){ // for each molecule
        for(ri=0;ri<A.m[mi][0].nr;ri++){ // for each residue in molecule
                if(A.m[mi][0].r[ri].n==branch_rn){
			for(ai=0;ai<A.m[mi][0].r[ri].na;ai++){ // for each atom in branch_rn residue
                                if(strcmp(A.m[mi][0].r[ri].a[ai].N,branch_aN)==0){
				b_mi=mi;
				b_ri=ri;
				b_ai=ai;
				printf("branch set, b_ai=%d\n",b_ai);
				}
			}
		}
	}
}

printf("A.m[b_mi][0].r[b_ri].a[b_ai].b[0].t.a=%d\n",A.m[b_mi][0].r[b_ri].a[b_ai].b[0].t.a);
printf("A.m[b_mi][0].r[b_ri].a[b_ai].b[1].t.a=%d\n",A.m[b_mi][0].r[b_ri].a[b_ai].b[1].t.a);


//count number of bonds core atom has
for(mi=0;mi<A.nm;mi++){ // for each molecule
        for(ri=0;ri<A.m[mi][0].nr;ri++){ // for each residue in molecule
                if(A.m[mi][0].r[ri].n==core_rn){// look only at core_rn residue
			printf("core residue number=%d\n",A.m[mi][0].r[ri].n);
                        for(ai=0;ai<A.m[mi][0].r[ri].na;ai++){ // for each atom in core_rn residue
                                if(strcmp(A.m[mi][0].r[ri].a[ai].N,core_aN)==0){ // look only at core_aN atom in core_rn residue
					c_mi=mi;
					c_ri=ri;
					c_ai=ai;// I should be able to use a pointer for all these...
					printf("core set, c_ai=%d\n",c_ai);
                                        nb=A.m[mi][0].r[ri].a[ai].nb; // copy details of number of bonds
					printf("A.m[c_mi][0].r[c_ri].a[c_ai].b[0].t.a=%d\n",A.m[c_mi][0].r[c_ri].a[c_ai].b[0].t.a);
				}
                        }
                }
        }
}

printf("core set, c_ai=%d\n",c_ai);
printf("A.m[c_mi][0].r[c_ri].a[c_ai].b[0].t.a=%d\n",A.m[c_mi][0].r[c_ri].a[c_ai].b[0].t.a);
printf("A.m[c_mi][0].r[c_ri].a[c_ai].b[1].t.a=%d\n",A.m[c_mi][0].r[c_ri].a[c_ai].b[1].t.a);

//NOTE atoms on the branch will not be "seen" by core atoms when looking at bonding partners by distance.
// number of bonds may seem strange but it is due to atoms in the branch not being counted as bonding partners to core atoms

/****************************************************SCENARIO 1****************************************************************/
if (nb > 1){ // we are in scenario 1 (see documentation for scenarios 1-3, it is to do with type of glycoside linkage) and need to rename atoms
	printf("We are in scenario 1\n\n");
	 //SAVE ATOM NAME FOR COMPLEX
	strcpy(cplx_a1,A.m[c_mi][0].r[c_ri].a[c_ai].N);
	strcpy(cplx_a2,cplx_a1);
	cplx_a2[0]='O'; // This needs to be checked. Works if a1 atom is a CX and a2 atom is an OX (X is an integer)
	
	// rename core_aN a1, 
	strcpy(A.m[c_mi][0].r[c_ri].a[c_ai].N,"a1");
	// rename branch_aN a2, 
	strcpy(A.m[b_mi][0].r[b_ri].a[b_ai].N,"a2");
	// rename connect_aN_i a3 and a4
	// store names in an array

	tmpi1=A.m[c_mi][0].r[c_ri].a[c_ai].b[0].t.a; //for readability of next line
	strcpy(connect_aN_1, A.m[c_mi][0].r[c_ri].a[tmpi1].N); //tmpi is index of first connected atom
	tmpi2=A.m[c_mi][0].r[c_ri].a[c_ai].b[1].t.a; //for readability of next line
	strcpy(connect_aN_2, A.m[c_mi][0].r[c_ri].a[tmpi2].N); //atom index is index of second connect atom
	printf("tmpi1=%d,tmpi2=%d,c_ai=%d,b_ai=%d",tmpi1,tmpi2,c_ai,b_ai);


	printf("connect_aN_1 is %c%c connect_aN2 is %c%c\n\n", connect_aN_1[0], connect_aN_1[1], connect_aN_2[0],connect_aN_2[1]);
	// call lowest numbered atom name a3 and the higher one a4
	if (connect_aN_1[1] < connect_aN_2[1]){
		
		//SAVE ATOM NAME FOR COMPLEX
        	strcpy(cplx_a3,A.m[c_mi][0].r[c_ri].a[tmpi1].N);
		strcpy(cplx_a4,A.m[c_mi][0].r[c_ri].a[tmpi2].N);

		printf("connect_aN_1[1] < connect_aN_2[1]\n\n");
		strcpy(A.m[c_mi][0].r[c_ri].a[tmpi1].N, "a3");
		strcpy(A.m[c_mi][0].r[c_ri].a[tmpi2].N, "a4");
	}
	else { // swap who gets called a3
		printf("connect_aN_1[1] > connect_aN_2[1]\n\n"); 
		strcpy(A.m[c_mi][0].r[c_ri].a[tmpi1].N, "a4");
		strcpy(A.m[c_mi][0].r[c_ri].a[tmpi2].N, "a3");
	}
}
/****************************************************SCENARIO 1 END************************************************************/

if (nb == 1){ // we are in either scenario 2 or scenario 3 and need to continue down the link.
	connect_ai=A.m[c_mi][0].r[c_ri].a[c_ai].b[0].t.a; // set tempi to the connecting atom ai
	//find nb for connect_ri
	nb=A.m[c_mi][0].r[c_ri].a[connect_ai].nb;

/****************************************************SCENARIO 2****************************************************************/
	//SCENARIO 2 (nb has been reset to connected atom and we can decide whether we are in scenario 2 or 3) 
	if (nb == 3){ // connected atom has 3 bonds. we are in scenario 2 and need to rename atoms
		printf("We are in scenario 2\n\n");
		// get id's of atoms connected to next atom in linkage. one will be the c_ai(core atom index)
		tmpi1=A.m[c_mi][0].r[c_ri].a[connect_ai].b[0].t.a;
		tmpi2=A.m[c_mi][0].r[c_ri].a[connect_ai].b[1].t.a;
		tmpi3=A.m[c_mi][0].r[c_ri].a[connect_ai].b[2].t.a;
		//swap tmpi's so c_ai isn't tmpi1 or tmpi2 (should use pointers for efficiency)
		if (tmpi1==c_ai){tmpi1=tmpi2;tmpi2=tmpi3;}
		if (tmpi2==c_ai){tmpi2=tmpi3;}
		// else tmpi3 is c_ai and all's ok
		// get the name of these atoms
		strcpy(connect_aN_1, A.m[c_mi][0].r[c_ri].a[tmpi1].N);
		strcpy(connect_aN_2, A.m[c_mi][0].r[c_ri].a[tmpi2].N);
	
		// call lowest numbered atom name a3 and the higher one a4
	        if (connect_aN_1[1] < connect_aN_2[1]){
	                printf("connect_aN_1[1] < connect_aN_2[1]\n\n");
			 //SAVE ATOM NAME FOR COMPLEX
                        strcpy(cplx_a3,A.m[c_mi][0].r[c_ri].a[tmpi1].N);
                        strcpy(cplx_a4,A.m[c_mi][0].r[c_ri].a[tmpi2].N);

	                strcpy(A.m[c_mi][0].r[c_ri].a[tmpi1].N, "a3");
	                strcpy(A.m[c_mi][0].r[c_ri].a[tmpi2].N, "a4");
			
	        }
	        else { // swap who gets called a3
	                printf("connect_aN_1[1] > connect_aN_2[1]\n\n");
			//SAVE ATOM NAME FOR COMPLEX
	                strcpy(cplx_a3,A.m[c_mi][0].r[c_ri].a[tmpi2].N);
        	        strcpy(cplx_a4,A.m[c_mi][0].r[c_ri].a[tmpi1].N);

	                strcpy(A.m[c_mi][0].r[c_ri].a[tmpi1].N, "a4");
	                strcpy(A.m[c_mi][0].r[c_ri].a[tmpi2].N, "a3");
		//In scenario 2 c_ai becomes atom a2 and the connected atom becomes a1

		//SAVE ATOM NAME FOR COMPLEX
                strcpy(cplx_a1,A.m[c_mi][0].r[c_ri].a[connect_ai].N);
		strcpy(cplx_a2,A.m[c_mi][0].r[c_ri].a[c_ai].N);

		strcpy(A.m[c_mi][0].r[c_ri].a[connect_ai].N,"a1");
	        strcpy(A.m[c_mi][0].r[c_ri].a[c_ai].N,"a2");
	        }
	}
/****************************************************SCENARIO 2 END************************************************************/

/****************************************************SCENARIO 3****************************************************************/
	//SCENARIO 3
	//need to move down 1 atom from c_ai (core atom index) and rename that to "a2"
	//need to move down 2 atoms from c_ai and rename that to "a1"
	if (nb == 2){ //  means we are in scenario 3 (Neither core_aN or branch_aN will be renamed.so it gets complicated)
		printf("We are in scenario 3\n\n");
		//SAVE ATOM NAME FOR COMPLEX
		strcpy(cplx_a1,A.m[c_mi][0].r[c_ri].a[connect_ai].N);
		//rename connect_aN a2
		strcpy(A.m[c_mi][0].r[c_ri].a[connect_ai].N, "a2");
		//find id of next_connect_ai
		tmpi1=A.m[c_mi][0].r[c_ri].a[connect_ai].b[0].t.a;
		tmpi2=A.m[c_mi][0].r[c_ri].a[connect_ai].b[1].t.a;
		if (tmpi1==c_ai){tmpi1=tmpi2;}
		//else tmpi1 is next atom in link and we are good to go
		next_connect_ai=tmpi1; //readability? If you've followed me this far you don't really need this ;)
		//SAVE ATOM NAME FOR COMPLEX
		strcpy(cplx_a2,A.m[c_mi][0].r[c_ri].a[next_connect_ai].N);
		//rename next_connect_ai "a1"
		strcpy(A.m[c_mi][0].r[c_ri].a[next_connect_ai].N, "a1");
		nb=A.m[c_mi][0].r[c_ri].a[next_connect_ai].nb;

		if (nb==3){
			tmpi1=A.m[c_mi][0].r[c_ri].a[next_connect_ai].b[0].t.a;
			tmpi2=A.m[c_mi][0].r[c_ri].a[next_connect_ai].b[1].t.a;
			tmpi3=A.m[c_mi][0].r[c_ri].a[next_connect_ai].b[2].t.a;
			if (tmpi1==connect_ai) {tmpi1=tmpi2;tmpi2=tmpi3;}
			if (tmpi2==connect_ai) {tmpi2=tmpi3;}
		        // else tmpi3 is connect_ai and all's ok
	       		// get the name of these atoms
		        strcpy(connect_aN_1, A.m[c_mi][0].r[c_ri].a[tmpi1].N);
		        strcpy(connect_aN_2, A.m[c_mi][0].r[c_ri].a[tmpi2].N);
		        // call lowest numbered atom name a3 and the higher one a4
		        if (connect_aN_1[1] < connect_aN_2[1]){
		       	        printf("connect_aN_1[1] < connect_aN_2[1]\n");
				//SAVE ATOM NAME FOR COMPLEX
                		strcpy(cplx_a3,A.m[c_mi][0].r[c_ri].a[tmpi1].N);
				strcpy(cplx_a4,A.m[c_mi][0].r[c_ri].a[tmpi2].N);
		                strcpy(A.m[c_mi][0].r[c_ri].a[tmpi1].N, "a3");
        	       		strcpy(A.m[c_mi][0].r[c_ri].a[tmpi2].N, "a4");
			}
        		else { // swap who gets called a3
               			printf("connect_aN_1[1] > connect_aN_2[1]\n");
				//SAVE ATOM NAME FOR COMPLEX
                                strcpy(cplx_a4,A.m[c_mi][0].r[c_ri].a[tmpi1].N);
                                strcpy(cplx_a3,A.m[c_mi][0].r[c_ri].a[tmpi2].N);
               			strcpy(A.m[c_mi][0].r[c_ri].a[tmpi1].N, "a4");
               			strcpy(A.m[c_mi][0].r[c_ri].a[tmpi2].N, "a3");
			}
	       	}

		// i.e. This is a 2-6 linked sialic acid with a carboxylate C1 that will be included in nb
		if (nb==4){
			//In scenario 3 connect_ai is a2 and next_connect_ai is a1
			//SAVE ATOM NAME FOR COMPLEX
                        strcpy(cplx_a1, A.m[c_mi][0].r[c_ri].a[next_connect_ai].N);
                        strcpy(cplx_a2, A.m[c_mi][0].r[c_ri].a[connect_ai].N);
			strcpy(A.m[c_mi][0].r[c_ri].a[connect_ai].N,"a2");
        	        strcpy(A.m[c_mi][0].r[c_ri].a[next_connect_ai].N,"a1");
	 
			//Get ai of atoms connected to next_connect_ai
			tmpi1=A.m[c_mi][0].r[c_ri].a[next_connect_ai].b[0].t.a;
	      	        tmpi2=A.m[c_mi][0].r[c_ri].a[next_connect_ai].b[1].t.a;
       		        tmpi3=A.m[c_mi][0].r[c_ri].a[next_connect_ai].b[2].t.a;
			tmpi4=A.m[c_mi][0].r[c_ri].a[next_connect_ai].b[3].t.a;
			//Discard connect_ai 
			if (tmpi1==connect_ai) {tmpi1=tmpi2;tmpi2=tmpi3;tmpi3=tmpi4;}
			if (tmpi2==connect_ai) {tmpi2=tmpi3;tmpi3=tmpi4;}
			if (tmpi3==connect_ai) {tmpi3=tmpi4;}
			//else tmpi4==connect_ai and we are good to go. Copy atom names of connected atoms
			strcpy(connect_aN_1, A.m[c_mi][0].r[c_ri].a[tmpi1].N);
			strcpy(connect_aN_2, A.m[c_mi][0].r[c_ri].a[tmpi2].N);
			strcpy(connect_aN_3, A.m[c_mi][0].r[c_ri].a[tmpi3].N);
			// Find the two highest atom numbers and name a3 and a4. 
			// most inefficient bubble sort ever! Need pointers once this works :P
			if (connect_aN_1[1] > connect_aN_2[1]){
				strcpy(TEMP,connect_aN_1);strcpy(connect_aN_1,connect_aN_2);strcpy(connect_aN_2,TEMP);
				if (connect_aN_2[1] > connect_aN_3[1]){
					strcpy(TEMP,connect_aN_2);strcpy(connect_aN_2,connect_aN_3);strcpy(connect_aN_3,TEMP);
				}
			}
			if (connect_aN_1[1] > connect_aN_2[1]){
                                strcpy(TEMP,connect_aN_1);strcpy(connect_aN_1,connect_aN_2);strcpy(connect_aN_2,TEMP);
			}
			// now rename the sorted atoms
			//SAVE ATOM NAME FOR COMPLEX
                        strcpy(cplx_a3, A.m[c_mi][0].r[c_ri].a[tmpi2].N);
                        strcpy(cplx_a4, A.m[c_mi][0].r[c_ri].a[tmpi3].N);

			strcpy(A.m[c_mi][0].r[c_ri].a[tmpi2].N,"a3");
			strcpy(A.m[c_mi][0].r[c_ri].a[tmpi3].N,"a4");

		}
	}
/****************************************************SCENARIO 3 END************************************************************/
}	
	

superimpose_four_branch_atoms(&A);

//write out assembly into a pdb in a results subfolder
char out[25];
strcpy(out,"superimposed_branch.pdb");
outputAsmblPDB(&A, out);

//Align complex to origin
strcpy(cplx,"complex.pdb"); //for loading file into assembly. clunky.
//load pdb into assembly A
B=*load_pdb(cplx);
/*
for(mi=0;mi<A.nm;mi++){ // for each molecule
        for(ri=0;ri<A.m[mi][0].nr;ri++){ // for each residue in molecule
                if(A.m[mi][0].r[ri].n==core_rn){// look only at core_rn residue
		printf("core_rn resnumber=%d\n\n",A.m[mi][0].r[ri].n);
		strcpy(cplx_core_rN,A.m[mi][0].r[ri].N);
		}
	}
}

for(mi=0;mi<B.nm;mi++){ // for each molecule
        for(ri=0;ri<B.m[mi][0].nr;ri++){ // for each residue in molecule
                if(B.m[mi][0].r[ri].N[1]==cplx_core_rN[1] && B.m[mi][0].r[ri].N[2]==cplx_core_rN[2]){//if the resname is the same as in the branch
			printf("We have a match%c%c\n\n",B.m[mi][0].r[ri].N[1],B.m[mi][0].r[ri].N[2]);
                        for(ai=0;ai<B.m[mi][0].r[ri].na;ai++){ // for each atom of that residue
				// if it matches the atom name of branch's core, rename it as the branch's core was renamed
				// so we are aligning to the correct atoms of the complex
				if(strcmp(B.m[mi][0].r[ri].a[ai].N,cplx_a1)==0){ strcpy(B.m[mi][0].r[ri].a[ai].N,"a1");}
				if(strcmp(B.m[mi][0].r[ri].a[ai].N,cplx_a2)==0){ strcpy(B.m[mi][0].r[ri].a[ai].N,"a2");}
				if(strcmp(B.m[mi][0].r[ri].a[ai].N,cplx_a3)==0){ strcpy(B.m[mi][0].r[ri].a[ai].N,"a3");}
				if(strcmp(B.m[mi][0].r[ri].a[ai].N,cplx_a4)==0){ strcpy(B.m[mi][0].r[ri].a[ai].N,"a4");}
                        }
		}
        }
}

//superimpose_four_branch_atoms(&B);
*/
char outB[25];
strcpy(outB,"superimposed_cplx.pdb");
outputAsmblPDB(&B, outB);


//if A.m[mi][0].r[ri].a[ai].nmb 
	
return 0;
}

