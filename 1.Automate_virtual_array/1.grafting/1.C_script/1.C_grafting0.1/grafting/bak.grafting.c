#include <glylib.h>
//FUNCTION DECLARATIONS
int line_num(char *filename);
//int is_A_in_B(int A_rn, char A_table[][7][8], int B_rn, char B_table[][7][8], int *A_rnlist, int *B_rnlist, int *B_rns);
int is_A_in_B(int *pn0match, int A_ln, char A_table[][7][8], int An0lines, int B_ln, char B_table[][7][8], int Bn0lines, int *pB_neighbr, int fTtoLG[][40], int nbc);

// nbc = n0_budcores

int main (void){
char filename[50];
FILE * pFile;

/*********************************READ TEMPLATE INFO**************************************/
int i=0,j=0,k=0,l=0,m=0,TnumRes; // counters
strcpy(filename, "LINK.txt");
TnumRes = line_num(filename);
char Ttable[40][7][8]; // LINK table for the template
int n0LGcores;


pFile = fopen (filename,"r");
printf("TnumRes=%d\n",TnumRes);

for (i=1;i<=TnumRes;i++){ 
	for (j=0;j<7;j++){
		fscanf (pFile, "%s", Ttable[i][j]);
	//	printf("table[%d][%d] is %s\n",i,j,Ttable[i][j]);
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
	//		printf("T_rnlist[%d]=%d\n",i,j); //residue number i is on line number j
		}
	}
}*/
		
// load in the template
assembly T; // Template assembly
strcpy(filename, "ligand.pdb");
T=*load_pdb(filename);

/************************* LOAD IN RECEPTOR FOR CLASH EVALUATION LATER **********************/
assembly R; // Receptor (for clash evaluation)
strcpy(filename, "receptor.pdb");
R=*load_pdb(filename);
/********************************************************************************************/

/*****************************END*READ TEMPLATE INFO*END**********************************/

/********************List the contents of RESULTS_library_search**************************/
DIR *mydir = opendir("./RESULTS_library_search");
struct dirent *entry = NULL;
int numfiles;
i=0;
char d_list[300][300];// dynamically set/expand this?
while((entry = readdir(mydir))){ /* If we get EOF, the expression is 0 and the loop stops. */
	if ( (strcmp(entry->d_name,".")!=0) && (strcmp(entry->d_name,"..")!=0) ){ // skip . and ..
		printf("%s\n", entry->d_name);
		strcpy(d_list[i], "./RESULTS_library_search/");
		strcat(d_list[i], entry->d_name);
		i++;
	}
}
closedir(mydir);
numfiles=i; 
for (j=0;j<(numfiles);j++){
	printf("d_list[%d]=%s\n",j,d_list[j]);
}
/****************END*List the contents of RESULTS_library_search*END**********************/
printf("numfiles=%d\n",numfiles);


assembly LG, C2R, BR, GR; // Library Glycan, CORE2ROH , Branch , Grafted assembly
char buffer[60]; // stores info from library glycan file, we select out LINK info
char LGtable[40][7][8]; // more than 40 resids in a glycan? 7 LINK entries, none more than 8
int n0LGres; // number of Library Glycan residues
int x,z;// counters

int LG_neighbr[30]; // list of neighbours to LG residue
int n0_budcores=0; // number of budcores (sequences which match template) in a Library Glycan
int budcore[30][30]; // budcore[budcoreNumber][residuenumber]
int n0match; // number of residues which match template. when=TnumRes we have a budcore

int n0branches=0, write=1; // number of branches in a LG 
int branches[40]; // 

int TtoLG[40][40]; // TtoLG[TResidueNumber][budcorenumber]=LGResidueNumber that matches

char outname[300]; // stores name of file to be outputed
//char branchoutname[200];
char vdwoutname[300];

int lowest_core_rn=50; // for finding the lowest residue number in the LG core. Go from there to resid 1
int CORE2ROH_rn[40]; // residue numbers from lowest core to 1
int n0CORE2ROH=0; // number of theses residues
int current_rn=0; // current residue number we are checking

int proceed=1; // keep going until this equals 0
int merge,brc1,brc2; // whether to merge or not and the branch numbers to be merged
int bn=0; // branch number we are on
int add2CORE2ROH=0; // switch to add branch to core2roh or not
int branch_rn[40][40]; // stores branches and their residue numbers
int n0rnInBranch=0; // counter for number of residues in the branch
int branch_n0[40]; // funky but branch_n0[branch number] = number of residues in that branch

int tmpbranch[40];
int LGcore_rn,LGbranch_rn,Tcore_rn,Tbranch_rn;
char LGbranch_aN[3],LGcore_aN[3];

//double overlap, total_overlap;
double total_overlap;

/*************************Read in Library Glycan and store LINK information **********************************/

for (i=0;i<(numfiles);i++){
	printf("\n");
	LG=*load_pdb(d_list[i]);
	printf("numfiles=%d\nfilesleft=%d\n",numfiles,(numfiles-i));

	// read LINK information into a Table so can compare against template
	pFile = fopen (d_list[i],"r");
	n0LGres=0;
	j=0;// to match Tstring which starts on 1 as uses TnumRes as top of counter.
	strcpy(buffer,"reset_string");
	while (strcmp(buffer,"HETATM")!=0 && strcmp(buffer,"ATOM")!=0){
		fscanf (pFile, "%s",buffer);
		if (strcmp(buffer,"LINK")==0){
			j++; // j will be the number of residues in LG
			strcpy(LGtable[j][0],buffer);
			for (k=1;k<7;k++){ // for the next 6 entries
				fscanf (pFile, "%s", LGtable[j][k]);
	//			printf("LGtable[%d][%d]=%s\n",j,k,LGtable[j][k]);
			}
		}
	}
	n0LGres=j;
	printf("Number of residues in glycan library file, n0LGres=%d\n",n0LGres);

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
			//printf("matched the 2nd residue in Ttable to the one on the %d row of LGtable\n",j);
			LG_neighbr[0]=atoi(LGtable[j][6]);
			n0match=1;
			TtoLG[atoi(Ttable[1][6])][n0_budcores]=atoi(LGtable[j][6]);
	//		printf("Checking LGtable[%d][6]=%d\n",j,LG_neighbr[0]);
	//		printf("TnumRes=%d j=%d\n",TnumRes,j);
			x=is_A_in_B(&n0match, 1, Ttable, TnumRes, j, LGtable, n0LGres, LG_neighbr, TtoLG, n0_budcores);
	//		printf("x=%d\n",x);
			if (x==TnumRes){ // if success and this is a budcore
		//		printf("LGtable[%d] is a budcore\n\n",j);
				n0_budcores++;
	//			printf("BUDCORE!\n");
				for (k=0;k<TnumRes;k++){
					budcore[n0_budcores][k]=LG_neighbr[k];
					//printf("\n");
				}
			}
			//else {printf("No Match!\nTnumRes=%d,n0match=%d\n\n",TnumRes,n0match);
		//	for (k=0;k<n0match;k++){
		//		printf("LG_neighbr[%d]==%d\n",k,LG_neighbr[k]);
		//	}
		}
	}
	n0LGcores=n0match; // what is this for?

	/********************************** FOR EACH BUDCORE IN LIBRARY GLYCAN*************************************************/
	printf("n0_budcores in library glycan=%d\n",n0_budcores);
	for (z=1;z<=n0_budcores;z++){
		
		// Each budcore will have it's own complete output structure GR
		// GR=T may have worked too... but byte for byte does not like strings and assemblies are complicated.
		strcpy(filename, "ligand.pdb");
		GR=*load_pdb(filename);
		// rename_GR(assembly *GR,assembly LG,TtoLG[][n0_budcores]
		//rename_GR(&GR,LG,TtoLG[][n0_budcores]); // rename Tcore resids in Gr so they match LG naming

		//for (j=1;j<TnumRes,j++){
		total_overlap=0; // reset this.	

		//Find the number and resids of 0residues that aren't in the LGcore_rn list. This will be the number of branches.
		n0branches=0;
		write=1;
		for (j=2;j<=n0LGres;j++){
			write=1;
			if (LGtable[j][5][0]=='0'){ // if it has an 0 here it is a terminal branch
				for (k=0;k<TnumRes;k++){
					x=atoi(LGtable[j][6]);
					//printf("x=%d\n",x);
					//printf("budcore[%d]=%d\n",k,budcore[z][k]);
					if (x==budcore[z][k]){
						write=0; // resid is a budcore and we don't want to waste time starting there.
						//printf("resid %d is a core, ignore\n",x);
					}
				}
				if (write==1){ // if resid wasn't a budcore residue
					branches[n0branches]=atoi(LGtable[j][6]);
					printf("residue number %d is an 0resid\n",branches[n0branches]);
					n0branches++;
				}
			}
		}
		// for each branch
	
		/******************************** first add the CORE2ROH residues ***********************************/
		lowest_core_rn=50;
		n0CORE2ROH=0;
		current_rn=0;
	
		for (k=0;k<TnumRes;k++){
			if (lowest_core_rn>budcore[z][k]){
				lowest_core_rn=budcore[z][k];
			}
		}
		printf("lowest_core_rn=%d\n",lowest_core_rn);
		if (lowest_core_rn>2){
			current_rn=lowest_core_rn;
			while (current_rn!=1){ // until we get to resid 1
				CORE2ROH_rn[n0CORE2ROH]=current_rn;
				printf("CORE2ROH_rn[%d]=%d\n",n0CORE2ROH,CORE2ROH_rn[n0CORE2ROH]);
				n0CORE2ROH++;
				for (l=0;l<=n0LGres;l++){ // find next residue that current_rn is connected to (going down towards resid 1)
       		         		if (atoi(LGtable[l][6])==current_rn){
        	                		current_rn=atoi(LGtable[l][3]);
					}
				}
				//printf("current_rn=%d\n",current_rn);
			}
			CORE2ROH_rn[n0CORE2ROH]=current_rn; // add resid 1
			n0CORE2ROH++;
			printf("n0CORE2ROH=%d/n",n0CORE2ROH);
		}
		/****************************END* first add the CORE2ROH residues *END*******************************/
		/******************************** Create the 0resid starting branches *******************************/
		proceed=1;
		bn=0;
		add2CORE2ROH=0;
		n0rnInBranch=0;
		//	printf("n0 branches=%d\n\n\n",n0branches);
		for (k=0;k<n0branches;k++){
			// compare resid with LGcore resids and 1
			current_rn=branches[k];
			n0rnInBranch=0;
			branch_n0[bn]=n0rnInBranch; // reset to zero
			proceed=1;
			merge=0;
			
			while (proceed==1){ // Starting at an 0resid, create a branch by moving to the residue it is connected to going down the LINK table
				// it could meet a core and thus the list of resids visted become a branch
				// it could meet a residue that is in another branch and will be merged with it
				// it could meet a residue that is in the CORE2ROH and thus be merged with CORE2ROH
				
				// add rn to list in branch_rn and find what it is connected to.
				//printf("current_rn=%d\n",current_rn);
				branch_rn[bn][n0rnInBranch]=current_rn;
				n0rnInBranch++;
				// Move down the branch towards residue 1.
				for (l=0;l<=n0LGres;l++){
					if (atoi(LGtable[l][6])==current_rn){ 
						current_rn=atoi(LGtable[l][3]); // go down the branch
						printf("current_rn is now %d\n",(atoi(LGtable[l][3])));
					}
				}
				// if we hit a LGcore create new branch
				for (l=0;l<=n0LGres;l++){ 
					if(current_rn==budcore[z][l]){
						branch_rn[bn][n0rnInBranch]=current_rn; // save current_rn to branch
       		                 		n0rnInBranch++;
						proceed=0;
						printf("Stopping, reached a core! Bn=%d current_rn=%d\n",bn,current_rn);
					}
				}
				// if rn is already in branch list merge into that branch
				if (proceed==1){ // i.e this isn't a core residue, otherwize would join branches that end in same core 
					for (l=0;l<=bn;l++){ // for each residue
						for (m=0;m<branch_n0[l];m++){ // for each residue in each branch
					//		printf("branch_rn[%d][%d]=%d\n",l,m,branch_rn[l][m]);
       	                	         		if(current_rn==branch_rn[l][m]){ // have we seen it before in any of the branches?
								merge=1;
								brc1=l;
								brc2=bn;
                                        			proceed=0;
                             		    //      		printf("Stopping, this is part of branch already! bn=%d branch_rn=%d\n",bn,branch_rn[bn][l]);
							}
                        	      		}
                        		}
				}
				// if we hit a CORE2ROH merge with it
				if (lowest_core_rn>2){
					for (l=0;l<n0CORE2ROH;l++){ // is it part of the CORE2ROH?
						if(current_rn==CORE2ROH_rn[l]){
							//branch_rn[bn][n0rnInBranch]=current_rn; // save current_rn to branch
                       	                		 //n0rnInBranch++;
							printf("n0CORE2ROH=%d\n",n0CORE2ROH);
							add2CORE2ROH=1;
							brc1=bn;
							proceed=0;
                       		        	 	printf("Stopping, this is part of CORE2ROH! CORE2ROH_rn[%d] matches  branch_rn=%d\n",l,CORE2ROH_rn[l]);
						}
                       	       		  }
                       	 	}
			} // end while proceed
			branch_n0[bn]=n0rnInBranch;
			printf("branch_n0[%d]=%d\n",bn,branch_n0[bn]);

			for (l=0;l<branch_n0[bn];l++){
				printf("branch_rn[%d][%d]=%d\n",bn,l,branch_rn[bn][l]);
			}

			bn++;
			// if we ended up hitting the CORE2ROH branch, merge with it
			if (add2CORE2ROH==1){
				m=0;
				for (l=n0CORE2ROH;l<(n0CORE2ROH+branch_n0[brc1]);l++){
					CORE2ROH_rn[l]=branch_rn[brc1][m];
					m++;
				}
				bn--;
			//	printf("added branch_rn[%d] to CORE2ROH\n",brc1);
				n0CORE2ROH=(n0CORE2ROH+branch_n0[brc1]);
				printf("n0CORE2ROH=%d\n",n0CORE2ROH);
				add2CORE2ROH=0; // reset
			}
			// if we ended up hitting a previous branch, merge with it
			if (merge==1){ // this is awkward as brc1 must be at the end so the branch and core are last 2 resids
				printf("MERGE!\n");
				for (l=0;l<branch_n0[brc1];l++){
					tmpbranch[l]=branch_rn[brc1][l]; // store brc1 in a tmp
				}
				m=0;
               		 	for (l=0;l<branch_n0[brc2];l++){ 
                       		 	branch_rn[brc1][l]=branch_rn[brc2][m]; // put brc2 into brc1 (overwriting)
			//		printf("making  branch_rn[%d][%d]=branch_rn[%d][%d]==%d\n",brc1,l,brc2,m,branch_rn[brc2][m]);
					m++;
               		        }
				m=0;
				for (l=branch_n0[brc2];l<(branch_n0[brc1]+branch_n0[brc2]);l++){
                                        branch_rn[brc1][l]=tmpbranch[m]; // put brc1 stored in tmp back into brc1 after brc2
                        //              printf("making  branch_rn[%d][%d]=branch_rn[%d][%d]==%d\n",brc1,l,brc2,m,branch_rn[brc2][m]);
                                        m++;
                                }
     	          	        branch_n0[brc1]=(branch_n0[brc1]+branch_n0[brc2]);
			//	printf("n0 in merged branch=%d\n",branch_n0[brc1]);
       		                bn--; // one less branch as merged two together
               		// 	printf("MERGED\n");
				merge=0; //reset
         	       }
		}// end each 0resid branch
		// now we have merged all the relevent branches

		/*********************************GRAFTING*******************************************/
		// GRAFT the CORE2ROH
		//int grn=CORE2ROH_rn[0]; // grn is residue number to be grafted

		/****************************CREATE BRANCH PDB FILE**********************************/
		strcpy(outname,"branch.pdb");
//		printf("n0CORE2ROH=%d\n",n0CORE2ROH);
//		for (j=0;j<n0CORE2ROH;j++){
//			printf("CORE2ROH_rn[%d]=%d\n",j,CORE2ROH_rn[j]);
//		}
		if (n0CORE2ROH>0){
			printf("Dealing with CORE2ROH, n0CORE2ROH=%d\n",n0CORE2ROH);
			outputResPDB(&LG, outname, CORE2ROH_rn, n0CORE2ROH); // only outputs resids in CORE2ROH
			C2R=*load_pdb(outname); // load it back in again.
			// set branch and core info for renamesuperimpose function
			LGcore_rn=CORE2ROH_rn[0];
			LGbranch_rn=CORE2ROH_rn[1];
			Tbranch_rn=1; // DODGEY!
			for (l=1;l<=n0LGres;l++){
                        	if (atoi(LGtable[l][6])==LGcore_rn){
                                	strcpy(LGcore_aN,LGtable[l][4]);
					strcpy(LGbranch_aN,LGtable[l][1]);
				}
				if (TtoLG[l][z-1]==LGcore_rn){
					Tcore_rn=l;
				}
				if (TtoLG[l][z-1]==LGbranch_rn){
                                        Tbranch_rn=l;
                                }
			}
			printf("LGcore_rn=%d, LGbranch_rn=%d, LGcore_aN=%s,LGbranch_aN=%s,Tcore_rn=%d,Tbranch_rn=%d\n",LGcore_rn,LGbranch_rn,LGcore_aN,LGbranch_aN,Tcore_rn,Tbranch_rn);
			rename_superimpose(&C2R, LGcore_aN, LGbranch_aN, LGcore_rn, LGbranch_rn, Tcore_rn, Tbranch_rn);
			CORE2ROH_rn[0]=CORE2ROH_rn[n0CORE2ROH-1]; // overwrite the first (core) resid with the last
			printf("CORE2ROH_rn[0]=%d\n",CORE2ROH_rn[0]);
			outputResPDB(&C2R, outname, CORE2ROH_rn, (n0CORE2ROH-1)); // output everything except the last one (now duplicate)
			C2R=*load_pdb(outname); // load it back in
			
			// OVERLAP ANALYSIS
                        //sprintf(branchoutname, "%s%s%d%s%d%s", d_list[i],"_budcore_",budcore[z][0],"_branch_", 0, ".pdb");
			//overlap=find_vdw_clashes_pairwize_between_Assemblies(&C2R,&R,branchoutname);
			//total_overlap=total_overlap + overlap;

			// rename Tcore by LGcore
			mergeAsmbly(&GR, &GR, &C2R); // add C2R onto Gr
			//outputAsmblPDB(&GR,branchoutname);			

		}
			
		for (j=0;j<bn;j++){
			printf("Dealing with branch number %d\n",j);
			outputResPDB(&LG, outname, branch_rn[j], branch_n0[j]);
			BR=*load_pdb(outname);

			// set branch and core info for renamesuperimpose function
			printf("branch_rn[%d][%d]=%d\n",j,branch_n0[j]-1,branch_rn[j][(branch_n0[j]-1)]);
			LGcore_rn=branch_rn[j][(branch_n0[j]-1)]; // set core to last resid in branch
                        LGbranch_rn=branch_rn[j][(branch_n0[j]-2)]; // set branch to one before
			Tbranch_rn=1; // DODGEY!
                        for (l=1;l<=n0LGres;l++){
                                if ( (atoi(LGtable[l][3])==LGcore_rn) && (atoi(LGtable[l][6])==LGbranch_rn) ){
                                        strcpy(LGcore_aN,LGtable[l][1]);
					strcpy(LGbranch_aN,LGtable[l][4]);
                                }
                                if (TtoLG[l][z-1]==LGcore_rn){ // find equivalent resid in T
                                        Tcore_rn=l;
                                }
                                if (TtoLG[l][z-1]==LGbranch_rn){ // is this correct for scenario 1? ... having an ROH O1 instead of the branch atom breaks it
                                        Tbranch_rn=l;
                                }
                        }
			//void rename_superimpose(char *core_aN, char *branch_aN, int core_rn, int branch_rn, int Tcore_rn, int Tbranch_rn);
			printf("LGcore_rn=%d, LGbranch_rn=%d, LGcore_aN=%s,LGbranch_aN=%s,Tcore_rn=%d,Tbranch_rn=%d\n",LGcore_rn,LGbranch_rn,LGcore_aN,LGbranch_aN,Tcore_rn,Tbranch_rn);
			rename_superimpose(&BR,LGcore_aN, LGbranch_aN, LGcore_rn, LGbranch_rn, Tcore_rn, Tbranch_rn);
			outputResPDB(&BR, outname, branch_rn[j], (branch_n0[j]-1)); // do not output the last (core) residue
			BR=*load_pdb(outname); // load it back in

			// OVERLAP ANALYSIS
			//sprintf(branchoutname, "%s%s%d%s%d%s", d_list[i],"_budcore_",budcore[z][0],"_branch_", (j+1), ".pdb");
			//overlap=find_vdw_clashes_pairwize_between_Assemblies(&BR,&R,branchoutname); // need name to match results after
			//total_overlap=total_overlap + overlap;
                        mergeAsmbly(&GR, &GR, &BR); // add BR onto Gr
			
		}

		// OUTPUT MERGED ASSEMBLY THIS IS AWESOME :)
		sprintf(outname, "%s%s%d%s", d_list[i],"_budcore_",budcore[z][0], ".pdb");
		outputAsmblPDB(&GR,outname);

		// OVERLAP ANALYSIS 
		//sprintf(vdwoutname, "%s%s%d%s", d_list[i],"_budcore_",budcore[z][0], "_Clash_results_total.txt");
                //pFile = fopen (branchoutname,"w");
                //fprintf(pFile,"Clash Area For _budcore No._%d_ is _ %5.1f\n",budcore[z][0],total_overlap);
                //fclose (pFile);

		sprintf(vdwoutname, "%s%s%d%s", d_list[i],"_budcore_",budcore[z][0],".pdb");
		total_overlap=find_vdw_clashes_pairwize_between_Assemblies(&GR,&R,vdwoutname); // includ all branches and core
		printf("Finished budcore, total_overlap=%f\n\n",total_overlap);
		//pFile = fopen (branchoutname,"w");
                //fprintf(pFile,"All Clash Area For _budcore No._%d_ is _ %5.1f\n",budcore[z][0],total_overlap);
                //fclose (pFile);

		//printf("LGcore_rn=%d, LGbranch_rn=%d, LGcore_aN=%s,LGbranch_aN=%s,Tcore_rn=%d,Tbranch_rn=%d\n",LGcore_rn,LGbranch_rn,LGcore_aN,LGbranch_aN,Tcore_rn,Tbranch_rn);
		
		//for (j=2;j<(TnumRes+2);j++){
		//	printf("TtoLG[%d][%d]=%d\n",j,(z-1),TtoLG[j][z-1]);
		//}
	}
printf("Finished all\n");
}
printf("Finished all\n");
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
fclose(f);
//if(c != '\n')
//lines++;
return lines;
}

int is_A_in_B(int *pn0match, int A_ln, char A_table[][7][8], int An0lines, int B_ln, char B_table[][7][8], int Bn0lines, int *pB_neighbr, int fTtoLG[][40], int nbc){// return residue numbers of B that match A in B_rns
int i,j,x=0;
//printf("pn0match=%d, A_ln=%d, Resid to be checked=%c%c, An0lines=%d, B_ln=%d \n",*pn0match,A_ln,A_table[A_ln][5][1],A_table[A_ln][5][2],An0lines,B_ln);
for (i=1;i<=An0lines;i++){
//	printf("A_table[%d][3]=%s,A_table[A_ln][6]=%s\n",i,A_table[i][3],A_table[A_ln][6]);
	if (strcmp(A_table[i][3],A_table[A_ln][6])==0){ // then residue on line A_ln is connected to some other resid on row i
//		printf("\n\nA rn=%s rN=%s:",A_table[i][6],A_table[i][5]);
		for (j=1;j<=Bn0lines;j++){ // Let's see if B has the same neighbour...
			if (strcmp(B_table[j][3],B_table[B_ln][6])==0){ //if B_rn has more than one neighbour
//				printf("\nB rn=%s rN=%s ",B_table[j][6],B_table[j][5]);
				if (A_table[i][2][1]==B_table[j][2][1] && A_table[i][2][2]==B_table[j][2][2] && strcmp(A_table[i][1],B_table[j][1])==0){
					fTtoLG[atoi(A_table[i][6])][nbc]=atoi(B_table[j][6]);
					pB_neighbr[*pn0match]=atoi(B_table[j][6]); //RHS resid is resid of neighbour
					(*pn0match)++; //
//					printf("...MATCH! number=%d\n",*pn0match);
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
