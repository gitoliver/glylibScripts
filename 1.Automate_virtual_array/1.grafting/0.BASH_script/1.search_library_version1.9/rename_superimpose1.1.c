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
char core_aN[3], branch_aN[3]; //stores atom names of core and branch atoms
char cplx_a1[3], cplx_a2[3], cplx_a3[3], cplx_a4[3];
int scenario=0;
int core_rn,branch_rn,Tcore_rn,Tbranch_rn; //stores residue numbers of core and branch structures of the bud and the template core and branch.
int mi,ri,ai,nb;
int c_ai,c_ri,c_mi;
int b_ai,b_ri,b_mi;
int connect_ai, next_connect_ai;
int tmpi[4]; //array of dummy indices
int i, a4_ai, a3_ai, prev=0;
int a3=0,a4=0; // for sorting out which atom is a3 and which is a4 in each scenario
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

strcpy(filename, "Tcore_rn.txt");
pFile = fopen (filename,"r");
fscanf (pFile, "%d", &Tcore_rn);
fclose (pFile);

strcpy(filename, "Tbranch_rn.txt");
pFile = fopen (filename,"r");
fscanf (pFile, "%d", &Tbranch_rn);
fclose (pFile);


/***********************************************************************************************/

set_nbonds_for_atoms_in_assembly(&A); // send the address of A

/*
//DEBUGGING
printf("\n\n");
int bi;
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
				//printf("branch set, b_ai=%d\n",b_ai);
				}
			}
		}
	}
}
/* DEBUGGING
printf("A.m[b_mi][0].r[b_ri].a[b_ai].b[0].t.a=%d\n",A.m[b_mi][0].r[b_ri].a[b_ai].b[0].t.a);
printf("A.m[b_mi][0].r[b_ri].a[b_ai].b[1].t.a=%d\n",A.m[b_mi][0].r[b_ri].a[b_ai].b[1].t.a);
*/

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
				//	printf("core set, c_ai=%d\n",c_ai);
                                        nb=A.m[mi][0].r[ri].a[ai].nb; // copy details of number of bonds
				//	printf("A.m[c_mi][0].r[c_ri].a[c_ai].b[0].t.a=%d\n",A.m[c_mi][0].r[c_ri].a[c_ai].b[0].t.a);
				}
                        }
                }
        }
}
printf("number of bonds=%d\n\n",nb);

/* DEBUGGING
printf("core set, c_ai=%d\n",c_ai);
printf("A.m[c_mi][0].r[c_ri].a[c_ai].b[0].t.a=%d\n",A.m[c_mi][0].r[c_ri].a[c_ai].b[0].t.a);
printf("A.m[c_mi][0].r[c_ri].a[c_ai].b[1].t.a=%d\n",A.m[c_mi][0].r[c_ri].a[c_ai].b[1].t.a);
*/
//NOTE atoms on the branch will not be "seen" by core atoms when looking at bonding partners by distance.
// number of bonds may seem strange but it is due to atoms in the branch not being counted as bonding partners to core atoms

/****************************************************SCENARIO 1****************************************************************/
if (nb > 1){ // we are in scenario 1 (see documentation for scenarios 1-3, it is to do with type of glycoside linkage) and need to rename atoms
	scenario=1;
	printf("SCENARIO 1\n\n");
	//SAVE ATOM NAME FOR RENAMING COMPLEX
	strcpy(cplx_a1,A.m[c_mi][0].r[c_ri].a[c_ai].N);
	strcpy(cplx_a2,cplx_a1);
	cplx_a2[0]='O'; // This needs to be checked. Works if a1 atom is a CX and a2 atom is an OX (X is an integer)
	
	// rename core_aN a1 and branch_aN a2 
	strcpy(A.m[c_mi][0].r[c_ri].a[c_ai].N,"a1");
	strcpy(A.m[b_mi][0].r[b_ri].a[b_ai].N,"a2");

	for (i=0;i<A.m[c_mi][0].r[c_ri].a[c_ai].nb;i++){ // for each atom bonded to c_ai
	tmpi[i]=A.m[c_mi][0].r[c_ri].a[c_ai].b[i].t.a; // readability: save ai of bonded atom in array
	if (A.m[c_mi][0].r[c_ri].a[tmpi[i]].N[1]>a4){ // if the atom number is greater than number in a4
              	a3=a4; // push what was a4 down to a3. These variables are just for atom number comparisons.
                a3_ai=prev; 
                a4=A.m[c_mi][0].r[c_ri].a[tmpi[i]].N[1]; // make a4 equal the current atom number
                a4_ai=tmpi[i];
                prev=tmpi[i];
                }
                else if (A.m[c_mi][0].r[c_ri].a[tmpi[i]].N[1]>a3){ // if not greater than a4 check if greater than a3
                a3=A.m[c_mi][0].r[c_ri].a[tmpi[i]].N[1]; // make a3 equal the atom number
                a3_ai=tmpi[i];
                }
        }
        strcpy(cplx_a3, A.m[c_mi][0].r[c_ri].a[a3_ai].N); // SAVE ATOM NAME FOR RENAMING COMPLEX
        strcpy(A.m[c_mi][0].r[c_ri].a[a3_ai].N,"a3"); // rename that atom a3
        strcpy(cplx_a4, A.m[c_mi][0].r[c_ri].a[a4_ai].N); // SAVE ATOM NAME FOR RENAMING COMPLEX
        strcpy(A.m[c_mi][0].r[c_ri].a[a4_ai].N,"a4"); // rename that atom a4
        printf("a1 is %c%c,a2 is %c%c,a3 is %c%c, a4 is %c%c ",cplx_a1[0],cplx_a1[1],cplx_a2[0],cplx_a2[1],cplx_a3[0],cplx_a3[1],cplx_a4[0],cplx_a4[1]);
}
/****************************************************SCENARIO 1 END************************************************************/

if (nb == 1){ // we are in either scenario 2 or scenario 3 and need to continue down the link.
	connect_ai=A.m[c_mi][0].r[c_ri].a[c_ai].b[0].t.a; // set tempi to the connecting atom ai
	//find nb for connect_ri
	nb=A.m[c_mi][0].r[c_ri].a[connect_ai].nb;
	printf("connect_ai number of bonds=%d\n\n",nb);
/****************************************************SCENARIO 2****************************************************************/
	//SCENARIO 2 (nb has been reset to connected atom and we can decide whether we are in scenario 2 or 3) 
	if (nb > 2){ // connected atom has 3 bonds. we are in scenario 2 and need to rename atoms
		scenario=2;
		printf("SCENARIO 2\n\n");

                //SAVE ATOM NAME FOR RENAMING COMPLEX
                strcpy(cplx_a1,A.m[c_mi][0].r[c_ri].a[connect_ai].N);
                strcpy(cplx_a2,A.m[c_mi][0].r[c_ri].a[c_ai].N);

		//In scenario 2 c_ai becomes atom a2 and the connected atom becomes a1
                strcpy(A.m[c_mi][0].r[c_ri].a[connect_ai].N,"a1");
                strcpy(A.m[c_mi][0].r[c_ri].a[c_ai].N,"a2");

		// get id's of atoms connected to next atom in linkage. one will be the c_ai(core atom index)
		for (i=0;i<A.m[c_mi][0].r[c_ri].a[connect_ai].nb;i++){ // for each atom bonded to connect_ai
			if (A.m[c_mi][0].r[c_ri].a[connect_ai].b[i].t.a != c_ai){ // look only at atoms which are not c_ai
				tmpi[i]=A.m[c_mi][0].r[c_ri].a[connect_ai].b[i].t.a; // readability: save ai of bonded atom in array
				if (A.m[c_mi][0].r[c_ri].a[tmpi[i]].N[1]>a4){ // if the atom number is greater than number in a4
                                        a3=a4; // push what was a4 down to a3. These variables are just for atom number comparisons.
                                        a3_ai=prev; 
                                        a4=A.m[c_mi][0].r[c_ri].a[tmpi[i]].N[1]; // make a4 equal the current atom number
                                        a4_ai=tmpi[i];
                                        prev=tmpi[i];
                                }
                                else if (A.m[c_mi][0].r[c_ri].a[tmpi[i]].N[1]>a3){ // if not greater than a4 check if greater than a3
                                        a3=A.m[c_mi][0].r[c_ri].a[tmpi[i]].N[1]; // make a3 equal the atom number
                                        a3_ai=tmpi[i];
                                }
                        }
                }
        strcpy(cplx_a3, A.m[c_mi][0].r[c_ri].a[a3_ai].N); // SAVE ATOM NAME FOR RENAMING COMPLEX
        strcpy(A.m[c_mi][0].r[c_ri].a[a3_ai].N,"a3"); // rename that atom a3
        strcpy(cplx_a4, A.m[c_mi][0].r[c_ri].a[a4_ai].N); // SAVE ATOM NAME FOR RENAMING COMPLEX
        strcpy(A.m[c_mi][0].r[c_ri].a[a4_ai].N,"a4"); // rename that atom a4
        printf("a1 is %c%c,a2 is %c%c,a3 is %c%c, a4 is %c%c ",cplx_a1[0],cplx_a1[1],cplx_a2[0],cplx_a2[1],cplx_a3[0],cplx_a3[1],cplx_a4[0],cplx_a4[1]);
	}
/****************************************************SCENARIO 2 END************************************************************/

/****************************************************SCENARIO 3****************************************************************/
	//SCENARIO 3
	//need to move down 1 atom from c_ai (core atom index) and rename that to "a2"
	//need to move down 2 atoms from c_ai and rename that to "a1"
	if (nb == 2){ // c_ai and next_connect_ai are the neighbours.  means we are in scenario 3 (Neither core_aN or branch_aN will be renamed.so it gets complicated)
		scenario=3;
		printf("SCENARIO 3\n\n");

		//find id of next_connect_ai
		for (i=0;i<A.m[c_mi][0].r[c_ri].a[connect_ai].nb;i++){ // for each atom bonded to connect_ai
                        if (A.m[c_mi][0].r[c_ri].a[connect_ai].b[i].t.a != c_ai){ // look only at atoms which are not c_ai
                                next_connect_ai=A.m[c_mi][0].r[c_ri].a[connect_ai].b[i].t.a; // there will only be one which is not c_ai
			}
		}

		//SAVE ATOM NAME FOR RENAMING COMPLEX
		strcpy(cplx_a1,A.m[c_mi][0].r[c_ri].a[next_connect_ai].N);
		strcpy(cplx_a2,A.m[c_mi][0].r[c_ri].a[connect_ai].N);

		//In scenario 3 connect_ai becomes atom a2 and next_connect_ai becomes a1
		strcpy(A.m[c_mi][0].r[c_ri].a[next_connect_ai].N,"a1");
		strcpy(A.m[c_mi][0].r[c_ri].a[connect_ai].N,"a2");
		nb=A.m[c_mi][0].r[c_ri].a[next_connect_ai].nb;
		//find id of atoms connected to next_connect_ai, which are not connect_ai and assign a3 and a4 to highest atom numbers
                for (i=0;i<nb;i++){ // for each atom bonded to next_connect_ai
                        if (A.m[c_mi][0].r[c_ri].a[next_connect_ai].b[i].t.a != connect_ai){ // look only at atoms which are not connect_ai
                                tmpi[i]=A.m[c_mi][0].r[c_ri].a[next_connect_ai].b[i].t.a; // there will be 2 or 3 which are not connect_ai
				if (A.m[c_mi][0].r[c_ri].a[tmpi[i]].N[1]>a4){ // if the atom number is greater than number in a4
                                        a3=a4; // push what was a4 down to a3. These variables are just for atom number comparisons.
					a3_ai=prev; 
					a4=A.m[c_mi][0].r[c_ri].a[tmpi[i]].N[1]; // make a4 equal the current atom number
					a4_ai=tmpi[i];
					prev=tmpi[i];
                                }
                                else if (A.m[c_mi][0].r[c_ri].a[tmpi[i]].N[1]>a3){ // if not greater than a4 check if greater than a3
					a3=A.m[c_mi][0].r[c_ri].a[tmpi[i]].N[1]; // make a3 equal the atom number
					a3_ai=tmpi[i];
                                }
                        }
              	}
	strcpy(cplx_a3, A.m[c_mi][0].r[c_ri].a[a3_ai].N); // SAVE ATOM NAME FOR RENAMING COMPLEX
	strcpy(A.m[c_mi][0].r[c_ri].a[a3_ai].N,"a3"); // rename that atom a3
	strcpy(cplx_a4, A.m[c_mi][0].r[c_ri].a[a4_ai].N); // SAVE ATOM NAME FOR RENAMING COMPLEX
	strcpy(A.m[c_mi][0].r[c_ri].a[a4_ai].N,"a4"); // rename that atom a4
	printf("a1 is %c%c,a2 is %c%c,a3 is %c%c, a4 is %c%c ",cplx_a1[0],cplx_a1[1],cplx_a2[0],cplx_a2[1],cplx_a3[0],cplx_a3[1],cplx_a4[0],cplx_a4[1]);
        }
/****************************************************SCENARIO 3 END************************************************************/
}	
// Only one scenario will be true at a time. 	


/***************************************************COMPLEX SUPERIMPOSING*****************************************************/
strcpy(cplx,"complex.pdb"); //for loading file into assembly. clunky.
B=*load_pdb(cplx);

// find residue in template-core with the same residue number (.n) as the coreTemplate residue that was hit by this branch.
for(mi=0;mi<B.nm;mi++){ // for each molecule
        for(ri=0;ri<B.m[mi][0].nr;ri++){ // for each residue in molecule
		if(B.m[mi][0].r[ri].n==Tcore_rn){ // if residue number is the templates core residue number
                        for(ai=0;ai<B.m[mi][0].r[ri].na;ai++){ // for each atom in branch_rn residue
                //if(B.m[mi][0].r[ri].N[1]==cplx_core_rN[1] && B.m[mi][0].r[ri].N[2]==cplx_core_rN[2]){//if the resname is the same as in the branch
		// DEBUGGING	printf("We have a match%c%c\n\n",B.m[mi][0].r[ri].N[1],B.m[mi][0].r[ri].N[2]);
		// DEBUGGING	printf("cplx_a1=%c%c%c,cplx_a2=%c%c%c,cplx_a3=%c%c%c,cplx_a4=%c%c%c,\n",cplx_a1[0],cplx_a1[1],cplx_a1[2],cplx_a2[0],cplx_a2[1],cplx_a2[2],cplx_a3[0],cplx_a3[1],cplx_a3[2],cplx_a4[0],cplx_a4[1],cplx_a4[2]);
				// if it matches the atom name of branch's core, rename it as the branch's core was renamed
				// so we are aligning to the correct atoms of the complex
// DEBUGGING printf("ai=%d,ri=%d,.N[]=%c%c%c\n",ai,ri,B.m[mi][0].r[ri].a[ai].N[0],B.m[mi][0].r[ri].a[ai].N[1],B.m[mi][0].r[ri].a[ai].N[2]);
				if(strcmp(B.m[mi][0].r[ri].a[ai].N,cplx_a1)==0){ strcpy(B.m[mi][0].r[ri].a[ai].N,"a1");}
				if(strcmp(B.m[mi][0].r[ri].a[ai].N,cplx_a2)==0){ strcpy(B.m[mi][0].r[ri].a[ai].N,"a2");}
				if(strcmp(B.m[mi][0].r[ri].a[ai].N,cplx_a3)==0){ strcpy(B.m[mi][0].r[ri].a[ai].N,"a3");}
				if(strcmp(B.m[mi][0].r[ri].a[ai].N,cplx_a4)==0){ strcpy(B.m[mi][0].r[ri].a[ai].N,"a4");}
                        }
		}
        }
}

// if scenario==1 we need to rename the complex Tbranch_rn atom to a2
if (scenario==1){
	for(mi=0;mi<B.nm;mi++){ // for each molecule
       		for(ri=0;ri<B.m[mi][0].nr;ri++){ // for each residue in molecule
               		if(B.m[mi][0].r[ri].n==Tbranch_rn){ // if residue number is the templates branch residue number
                       		for(ai=0;ai<B.m[mi][0].r[ri].na;ai++){ // go through and rename a2 atom to a2
					if(strcmp(B.m[mi][0].r[ri].a[ai].N,cplx_a2)==0){ strcpy(B.m[mi][0].r[ri].a[ai].N,"a2");}
				}
			}
		}
	}
}

/***********************************************************************************/
// SUPERIMPOSING
printf("SUPERIMPOSING\n");
superimpose4atoms(&A,&B);

if (scenario==1){
        strcpy(A.m[b_mi][0].r[b_ri].a[b_ai].N,"a9"); // naming it a9 allows the controlling bash script to cut it out
}

char out[25];
strcpy(out,"superimposed_branch.pdb");
outputAsmblPDB(&A, out);


//OUTPUT
char dirtoutB[35];
strcpy(dirtoutB,"superimposed_cplx_dirt.pdb");
outputAsmblPDB(&B, dirtoutB);

//Before output rename atoms back to original names
for(mi=0;mi<B.nm;mi++){ // for each molecule
        for(ri=0;ri<B.m[mi][0].nr;ri++){ // for each residue in molecule
		if(B.m[mi][0].r[ri].n==Tcore_rn){ // if residue number is the templates core residue number
                //if(B.m[mi][0].r[ri].N[1]==cplx_core_rN[1] && B.m[mi][0].r[ri].N[2]==cplx_core_rN[2]){//if the resname is the same as in the branch
                        for(ai=0;ai<B.m[mi][0].r[ri].na;ai++){ // for each atom of that residue
                                if(strcmp(B.m[mi][0].r[ri].a[ai].N,"a1")==0){ strcpy(B.m[mi][0].r[ri].a[ai].N,cplx_a1);}
                                if(strcmp(B.m[mi][0].r[ri].a[ai].N,"a2")==0){ strcpy(B.m[mi][0].r[ri].a[ai].N,cplx_a2);}
                                if(strcmp(B.m[mi][0].r[ri].a[ai].N,"a3")==0){ strcpy(B.m[mi][0].r[ri].a[ai].N,cplx_a3);}
                                if(strcmp(B.m[mi][0].r[ri].a[ai].N,"a4")==0){ strcpy(B.m[mi][0].r[ri].a[ai].N,cplx_a4);}
				if(scenario==3){ // From Matt: swap the coord of the oxygen atom with those of the bud_cores. This is the oxygen we need.
					if(strcmp(B.m[mi][0].r[ri].a[ai].N,core_aN)==0){
						B.m[mi][0].r[ri].a[ai].x.i=A.m[c_mi][0].r[c_ri].a[c_ai].x.i;
						B.m[mi][0].r[ri].a[ai].x.j=A.m[c_mi][0].r[c_ri].a[c_ai].x.j;
						B.m[mi][0].r[ri].a[ai].x.k=A.m[c_mi][0].r[c_ri].a[c_ai].x.k;
					}
				}
                        }
                }
        }
}


//OUTPUT
char outB[25];
strcpy(outB,"superimposed_cplx.pdb");
outputAsmblPDB(&B, outB);
return 0;
}
