#include <molecules.h>
#include <mylib.h>
#include <glylib.h>

#define STEPSIZE 5
#define DEG2RAD 57.2957795

/**************************************************************************************************************************************
 This code will generate all the possible permutations of torsions angles for a linkage with STEPSIZE degrees to a limit or window_size
 If the window_size=20,STEPSIZE=5. I will generate 8 structures per torsion.
 With two torsions, i.e a phi and a psi there will be 8x8=64 structures.
 For each Phi I generate each Psi and for each Psi I would create each Omg and so on when applicable.
 What I want are torsions of +5,-5,+10,-10,+15,-15,+20 and -20 in this order.
 So sequentially I'll change the torsion by +5,-10,+15,-20,+25,-30,+35,-40 to get those.
 Oliver Grant 03May2012. Written as part of glycan wiggler code.
****************************************************************************************************************************************/

void generate_torsion_window_permutationsGP(int window_size, assembly *A, assembly *B, int tors, int *total_struct, double *lowest_clash, int linkage_resid, dihedral_coord_set *DCS, int lna, int linked2){

/***********Find which atoms need to moved for this torsion************/
int nab=0; //number of atoms in branch
coord_3D **brnch;
brnch=(coord_3D**)calloc((*A).na, sizeof(coord_3D*)); // list of pointers to coord in branch

find_branch_to_rotate(A, tors, brnch, &nab, DCS, lna, linked2);

/**********Now generate the different torsions*************************/
double clash=0;
double delta=0;
int loops=(2*(window_size/STEPSIZE));
int j=0;
int k=1; // switchs between -1 and +1
char outname[25];
int internal_clash=0;

//printf("Will now create the torsions\n");
while (j<=(loops)){
	if (j>0) { // allows us to move down to last torsion without outputting structures.
		delta=((STEPSIZE*j*k)/DEG2RAD); // 1st loop k will be 1, then -1, then +1 and so on
//               	printf("delta=%f, tors=%d, j=%d\n",(delta*DEG2RAD),tors,j);
//		printf("Will now alter torsion by %2f\n",(delta*DEG2RAD));
               	change_torsion_by(delta, DCS, lna, tors, brnch, &nab);
//*OUT*/		sprintf(outname, "output_tors%d_%d.pdb",(*total_struct),tors); // outputs a file for each change
//*OUT*/		printf("outputfile=%s\n",outname);
//*OUT*/		outputAsmblPDB_NoRenameResid(A,outname);

		//check clash, if clash<1, set i=loops+1, save phi
        	clash=find_vdw_clashes_return_resid_totalGP(A,B,&linkage_resid);
        	internal_clash=0; //reset
        	internal_clash=find_new_bonds_meaning_clash(A);
        	// if clash is better and not internal clash is happening then output a new step.
		if ( (clash<*lowest_clash) && (internal_clash==0) ){
                	*lowest_clash=clash;
                	//printf("Clash for resid is now %2f\n",clash);
                	sprintf(outname, "%s", "best_structure.pdb"); // for best structure only
               		outputAsmblPDB_NoRenameResid(A,outname);
			// saveDelta? Can't cause assembly get's changed at each linkage when I load in beststructure. Balls.
//*OUT*/			sprintf(outname, "%s%d%s", "best_structure",(*total_struct),".pdb"); // to see best structure evolving.
//*OUT*/			printf("output %s\n",outname);
//*OUT*/ 		outputAsmblPDB_NoRenameResid(A,outname);
        	}

	}
	//printf("j=%d,tors=%d,k=%d,loops=%d\n",j,tors,k,loops);
	*total_struct=((*total_struct)+1); // just a unique id to output .pdb files with. starts at 10000 so loads into vmd in order.
	k=(k*(-1));
	j++;
	// for each Psi, generate every phi. For each omg generate every,psi and every phi.
	if (tors>1) { //tors is torsion id. Phi is 1, Psi is 2, Omg is 3, etc
//		printf("calling generate torsions again with tor=%d\n",tors-1);
	        generate_torsion_window_permutationsGP(window_size,A,B,(tors-1),total_struct,lowest_clash,linkage_resid,DCS,lna,linked2);
	}
}

// need to reset here or torsion we just wiggled will be at the extreme
//printf("Loop finished, resetting angle to original\n");
delta=((delta*(-1))/2); // seems I like brackets... This is to take the Phi angle back to it's starting position for next Psi.
change_torsion_by(delta, DCS, lna, tors, brnch, &nab); // resets

free(brnch);

return;
}
