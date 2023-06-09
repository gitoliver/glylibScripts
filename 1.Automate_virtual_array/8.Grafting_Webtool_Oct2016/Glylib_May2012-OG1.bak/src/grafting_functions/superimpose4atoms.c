/*
Oliver Grant 21Jan11
This function was written as part of Computational Carbohydrate Threading.

The four atoms in the branch make a Y shape. The Center atom is called a1, the bottom atom which, depending on the linkage, is a step closer to or is the branch glycan atom is a2, the top left atom is a3 and the top right atom is a4. Each pdb file of the search results will have the minimal motif removed except the 3 atoms involved in the branch. The atoms will be named a1-a4 in the incoming pdb.

The molecule is translated so the a1 atom lies on the origin. This is a perfect translation
The a1-a3 vector of the branch is then aligned to the equivalent atoms in the minimal binding determinant. This is a perfect alignment
The a1-a4 vector of the branch is then aligned to the equivalent atoms in the minimal binding determinant. This depends on ring geometry match of xtal and glycam
In the future the a1-a3 and a1-a4 vectors will be best fitted to the corresponding vectors. This is trivial when gemetries are similar.
a2 isn't used in this (latest) version as part of the alignment.

				    a3	    a4        eg:       C2	O5	
				      \	   /			  \    /
				       \  /	                   \  /
					a1			    C1
				        |                           |
					|			    |	
					a2			    O1
*/
/*****************************************************************************************/

#include <glylib.h>
#include <mylib.h> 
#include <molecules.h>
void superimpose4atoms(assembly *A, assembly *B){ // A gets moved onto B
int naa=0,nba=0,mi=0,ri=0,ai=0;
coord_3D **x_listax,**x_listbx,**px_lista,**px_listb;
char out[20]; // name of output file

//printf("Entered superimpose4atoms function\n");

/***********************************GATHER INFORMATION ABOUT ASSEMBLIES*****************************************/


//count the number of atoms in the assembly to know how many coord_3D points we need
for(mi=0;mi<(*A).nm;mi++){
	for (ri=0;ri<(*A).m[mi][0].nr;ri++){
		for(ai=0;ai<(*A).m[mi][0].r[ri].na;ai++){
			naa++;
		}
	}
}
for(mi=0;mi<(*B).nm;mi++){
	for (ri=0;ri<(*B).m[mi][0].nr;ri++){
		for(ai=0;ai<(*B).m[mi][0].r[ri].na;ai++){
        		nba++;
		}
        }
}

//printf("MARK1\n");

//creates list to store co-ord of the atoms
x_listax=(coord_3D**)calloc(5, sizeof(coord_3D*)); // stores co-ord of a1-a4 atoms
x_listbx=(coord_3D**)calloc(5, sizeof(coord_3D*));
px_lista=(coord_3D**)calloc(naa, sizeof(coord_3D*));// ptrs to co-ord of all atoms
px_listb=(coord_3D**)calloc(nba, sizeof(coord_3D*));

//printf("MARK2\n");

//get coord_3D of atoms in A
int y=0; // counter for number of atoms in the assembly
for(mi=0;mi<(*A).nm;mi++){
	for (ri=0;ri<(*A).m[mi][0].nr;ri++){
		for(ai=0;ai<(*A).m[mi][0].r[ri].na;ai++){
			px_lista[y]=&(*A).m[mi][0].r[ri].a[ai].x;
			y++; // use this as counter as ai resets for each ri
			// also find a1-a4 while we are here :)
			if (strcmp((*A).m[mi][0].r[ri].a[ai].N,"a1")==0){
                        	x_listax[1]=&((*A).m[mi][0].r[ri].a[ai].x);}
			if (strcmp((*A).m[mi][0].r[ri].a[ai].N,"a2")==0){
                                x_listax[2]=&((*A).m[mi][0].r[ri].a[ai].x);}
			if (strcmp((*A).m[mi][0].r[ri].a[ai].N,"a3")==0){
                                x_listax[3]=&((*A).m[mi][0].r[ri].a[ai].x);}
//				printf("x_listax[3].i=%f,x_listax[3].j=%f,x_listax[3].k=%f\n",(*x_listax[3]).i,(*x_listax[3]).j,(*x_listax[3]).k);}
			if (strcmp((*A).m[mi][0].r[ri].a[ai].N,"a4")==0){
                                x_listax[4]=&((*A).m[mi][0].r[ri].a[ai].x);}
//				printf("x_listax[4].i=%f,x_listax[4].j=%f,x_listax[4].k=%f\n",(*x_listax[4]).i,(*x_listax[4]).j,(*x_listax[4]).k);}
		}
	}
}
//printf("MARK3\n");

y=0; // reset counter for new assembly
for(mi=0;mi<(*B).nm;mi++){
	for (ri=0;ri<(*B).m[mi][0].nr;ri++){
        	for(ai=0;ai<(*B).m[mi][0].r[ri].na;ai++){
			px_listb[y]=&(*B).m[mi][0].r[ri].a[ai].x;
			y++;
			// also find a1-a4 while we are here :)
			if (strcmp((*B).m[mi][0].r[ri].a[ai].N,"a1")==0){
                        	x_listbx[1]=&((*B).m[mi][0].r[ri].a[ai].x);}
			if (strcmp((*B).m[mi][0].r[ri].a[ai].N,"a2")==0){
                                x_listbx[2]=&((*B).m[mi][0].r[ri].a[ai].x);}
			if (strcmp((*B).m[mi][0].r[ri].a[ai].N,"a3")==0){
                                x_listbx[3]=&((*B).m[mi][0].r[ri].a[ai].x);}
//				printf("x_listbx[3].i=%f,x_listbx[3].j=%f,x_listbx[3].k=%f\n",(*x_listbx[3]).i,(*x_listbx[3]).j,(*x_listbx[3]).k);}
			if (strcmp((*B).m[mi][0].r[ri].a[ai].N,"a4")==0){
                               x_listbx[4]=&((*B).m[mi][0].r[ri].a[ai].x);}
		}
        }
}

/***********************************GATHER INFORMATION ABOUT ASSEMBLIES*****************************************/
/************************************************ END **********************************************************/

//printf("MARK4\n");

//**************translate all atoms in assembly A by a1 atoms in B to align**************************************/

double i,j,k;
i=((*x_listax[1]).i)-((*x_listbx[1]).i); // Get the difference between A and B a1 atoms
j=((*x_listax[1]).j)-((*x_listbx[1]).j);
k=((*x_listax[1]).k)-((*x_listbx[1]).k);
//printf("Difference between A and B a1 atoms: i=%f,j=%f,k=%f\n",i,j,k); 
// translate A to B
for(mi=0;mi<(*A).nm;mi++){
	for (ri=0;ri<(*A).m[mi][0].nr;ri++){
                for(ai=0;ai<(*A).m[mi][0].r[ri].na;ai++){
                        (*A).m[mi][0].r[ri].a[ai].x.i=(((*A).m[mi][0].r[ri].a[ai].x.i) - i);
                        (*A).m[mi][0].r[ri].a[ai].x.j=(((*A).m[mi][0].r[ri].a[ai].x.j) - j);
                        (*A).m[mi][0].r[ri].a[ai].x.k=(((*A).m[mi][0].r[ri].a[ai].x.k) - k);
                }
        }
}

/*
strcpy(out,"A1out.pdb");
outputAsmblPDB(A, out);
strcpy(out,"B1out.pdb");
outputAsmblPDB(B, out);
*/
//printf("AFTER TRANSLATE\n");
//printf("x_listax[3].i=%f,x_listax[3].j=%f,x_listax[3].k=%f\n",(*x_listax[3]).i,(*x_listax[3]).j,(*x_listax[3]).k);
//printf("x_listax[4].i=%f,x_listax[4].j=%f,x_listax[4].k=%f\n",(*x_listax[4]).i,(*x_listax[4]).j,(*x_listax[4]).k);
//printf("x_listbx[3].i=%f,x_listbx[3].j=%f,x_listbx[3].k=%f\n",(*x_listbx[3]).i,(*x_listbx[3]).j,(*x_listbx[3]).k);
//printf("MARK5\n");

/*****************************ROTATE a3 atom of A so it lies on a3 atom of B******************************************/
vectormag_3D v13A,v13B,direction;
double theta3, theta4; // OG16jan12
v13A = coord2_to_vec(*x_listax[1],*x_listax[3]); // vector of the bond that will be rotated
v13B = coord2_to_vec(*x_listbx[1],*x_listbx[3]); // vector of the bond that will be rotated to.

direction=get_crossprod(v13A,v13B); // google cross product. Rotate around this vector which is vectical to the plane that they are on.

theta3=get_angle_between_vectors(v13A,v13B); // The size of angle between them
//printf("MARK6\n");
rotate_coords_about_axis_dp_list (px_lista, naa, *x_listbx[1], direction, theta3);

//printf("MARK7\n");
/*
strcpy(out,"A2out.pdb");
outputAsmblPDB(A, out);
strcpy(out,"B2out.pdb");
outputAsmblPDB(B, out);
*/

/*****************************ROTATE a4 atom of A so it lies on a4 atom of B******************************************/
/*****************************Also try to best fit A a2 onto B a2 at the same time******************************************/
// NOW ROTATE THE a1-a4 OF "A" BOND SO IT LIES ON THE a1-a4 BOND OF "B"
// rotate around the a1-a3 bond so it does not move. pass the V13B vector into rotate_coords function as direction
// need vectors at right angles to the a1-a3 bond in order to rotate correctly.

double Ang,tmp,adj,hyp,dist;
coord_3D pt;
vectormag_3D v31A,v31B,v14A,norm31,vpt_a4A,vpt_a4B;
//vectormag_3D v14B, v12A,v12B,vpt_a2A; // OG16jan12

//v12A = coord2_to_vec(*x_listax[1],*x_listax[2]); // OG16jan12
//v12B = coord2_to_vec(*x_listbx[1],*x_listbx[2]); // OG16jan12
v14A = coord2_to_vec(*x_listax[1],*x_listax[4]); 
//v14B = coord2_to_vec(*x_listbx[1],*x_listbx[4]); 
v31A = coord2_to_vec(*x_listax[3],*x_listax[1]);
v31B = coord2_to_vec(*x_listbx[3],*x_listbx[1]);


//printf("AFTER a4 ROTATION\n");
//printf("x_listax[3].i=%f,x_listax[3].j=%f,x_listax[3].k=%f\n",(*x_listax[3]).i,(*x_listax[3]).j,(*x_listax[3]).k);
//printf("x_listax[4].i=%f,x_listax[4].j=%f,x_listax[4].k=%f\n",(*x_listax[4]).i,(*x_listax[4]).j,(*x_listax[4]).k);
//printf("x_listbx[3].i=%f,x_listbx[3].j=%f,x_listbx[3].k=%f\n",(*x_listbx[3]).i,(*x_listbx[3]).j,(*x_listbx[3]).k);

/* a4
   | \
   |  \ hyp, V14
   |   \
   pt---a1----a3
     adj  V13
*/

// Need to get a vector which is at a right angle to the a1----a3 (v13) bond. Get the point pt on the line defined by the v13 vector such 
// that the vector between it and the a4 atom will be perpendicular to the v13 bond. See the image above.
// We could assume that this point would be ok for the a1--a2 bond too in the case of perfect geometry. I'll check this 
// and if this comment is still here it worked fine. It will be bad for bad geometries.

tmp=get_angle_between_vectors(v14A,v13A);
Ang=PI-tmp;
hyp=v14A.d;
adj=cos(Ang)*hyp; // math is handy...
norm31=normalize_vec(v31A);
dist=v31A.d+adj;
pt.i=(*x_listax[3]).i+(norm31.i*dist); 
pt.j=(*x_listax[3]).j+(norm31.j*dist);
pt.k=(*x_listax[3]).k+(norm31.k*dist);

//printf("A\ntmp=%f\nAng=%f\nhyp=%f\nadj=%f\nnorm21A.d=%f\ndist=%f\npt.ijk=%.3f %.3f %.3f\n",tmp,Ang,hyp,adj,norm21.d,dist,pt.i,pt.j,pt.k);

vpt_a4A = coord2_to_vec(pt,*x_listax[4]);
//vpt_a2A = coord2_to_vec(pt,*x_listax[2]); // OG16jan12

// Next paragraph/block of code can be commented out but need to think about which is better. 
// Deviation of pt in B from pt in A will depend on extent to which sugar ring configuration matches

//printf("MARK8\n");
/*
tmp=get_angle_between_vectors(v13B,v12B);
Ang=PI-tmp;
hyp=v13B.d;
adj=cos(Ang)*hyp;
norm21=normalize_vec(v21B);
dist=v21B.d+adj;
pt.i=(*x_listbx[2]).i+(norm21.i*dist);
pt.j=(*x_listbx[2]).j+(norm21.j*dist);
pt.k=(*x_listbx[2]).k+(norm21.k*dist);

printf("B\ntmp=%f\nAng=%f\nhyp=%f\nadj=%f\nnorm21B.d=%f\ndist=%f\npt=%.3f %.3f %.3f\n",tmp,Ang,hyp,adj,norm21.d,dist,pt.i,pt.j,pt.k);
*/
//printf("MARK9\n");

//vpt_a4B = coord2_to_vec(pt,*x_listbx[2]);

//printf("JUST BEFORE a3 ROTATION\n");
//printf("x_listax[3].i=%f,x_listax[3].j=%f,x_listax[3].k=%f\n",(*x_listax[3]).i,(*x_listax[3]).j,(*x_listax[3]).k);
//printf("x_listax[4].i=%f,x_listax[4].j=%f,x_listax[4].k=%f\n",(*x_listax[4]).i,(*x_listax[4]).j,(*x_listax[4]).k);
//printf("x_listbx[3].i=%f,x_listbx[3].j=%f,x_listbx[3].k=%f\n",(*x_listbx[3]).i,(*x_listbx[3]).j,(*x_listbx[3]).k);
//direction=v12B //Rotate around this vector which is vectical to the plane vectors v3pt* are on. just pass in v12B instead of direction

theta4=get_angle_between_vectors(vpt_a4B,vpt_a4A); // The size of the angle between them, 
/*
theta2=get_angle_between_vectors(vpt_a2B,vpt_a2A); // OG16jan12
//printf("THETA BA=%f\n",theta);
//theta=get_angle_between_vectors(v3ptA,v3ptB); // The size of angle between them
//printf("THETA AB=%f\n",theta); // this is the same

//theta=theta*-1;
rotate_coords_about_axis_dp_list (px_lista, naa, *x_listbx[1], v13B, theta2); // OG16jan12
vpt_a2A = coord2_to_vec(pt,*x_listax[2]); // OG16jan12
vpt_a2B = coord2_to_vec(pt,*x_listbx[2]); // OG16jan12
theta2=get_angle_between_vectors(vpt_a2A,vpt_a2B); // OG16jan12


while (theta2>(3/57.2957795)){ // convert X degrees to radians, be within that of a3. X can be changed according to crystal structure geometry match to glycam
        rotate_coords_about_axis_dp_list (px_lista, naa, *x_listbx[1], v13B, theta2); // OG16jan12
        vpt_a2A = coord2_to_vec(pt,*x_listax[2]); // OG16jan12
        vpt_a2B = coord2_to_vec(pt,*x_listbx[2]); // OG16jan12
        theta2=get_angle_between_vectors(vpt_a2A,vpt_a2B); // OG16jan12
        //theta=(theta/2); // do a half step towards (or away if on wrong side) target
        printf("New theta2=%f\n",theta2*57.2957795);
}


strcpy(out,"A22out.pdb");
outputAsmblPDB(A, out);
*/

vpt_a4A = coord2_to_vec(pt,*x_listax[4]);
vpt_a4B = coord2_to_vec(pt,*x_listbx[4]);
theta4=get_angle_between_vectors(vpt_a4A,vpt_a4B);

//printf("Theta=%f\n",theta*57.2957795);

int count=0;
double prev_theta4=theta4;

/*******Rotates the v13B vector until v14B lies on atom a4********/
int limit=6;
while ((theta4>(limit/57.2957795))&&(count < 20)){ // convert X degrees to radians, be within that of a4. X can be changed according to crystal structure geometry match to glycam
	// Allows moving over and back desired alignment rather than going around and around if overshoot the mark.
	// Function takes radians and rotation direction is determined by direction of vector given
	if(theta4>prev_theta4){
                theta4=(theta4*1.2); // go back(using v31B vector instead of v13B) past (using 1.2) previous position
//		printf("Rotating backwards by %.1f degrees\n",theta4*57.2957795);
                rotate_coords_about_axis_dp_list (px_lista, naa, *x_listbx[1], v31B, theta4);
        }
	else {
//		printf("Rotating by %.1f degrees\n",theta4*57.2957795);
		rotate_coords_about_axis_dp_list (px_lista, naa, *x_listbx[1], v13B, theta4);
	}
	vpt_a4A = coord2_to_vec(pt,*x_listax[4]);
	vpt_a4B = coord2_to_vec(pt,*x_listbx[4]);
	prev_theta4=theta4;
	theta4=get_angle_between_vectors(vpt_a4A,vpt_a4B); // does not know if on wrong side. rotate rotates in one direction only.
	count++;
}
if (count==20){
	// Reverse
	rotate_coords_about_axis_dp_list (px_lista, naa, *x_listbx[1], v31B, theta4);
	vpt_a4A = coord2_to_vec(pt,*x_listax[4]);
        vpt_a4B = coord2_to_vec(pt,*x_listbx[4]);
	theta4=get_angle_between_vectors(vpt_a4A,vpt_a4B);
	strcpy(out,"CheckMeA.pdb");
	outputAsmblPDB(A, out);
	strcpy(out,"CheckMeB.pdb");
	outputAsmblPDB(B, out);
	printf("CHECK CheckMe files! Probably rings are not same shape.\nMay cause stack smash and SEGFAULT\n Limit was %d but continuing with theta4=%.1f\n",limit,theta4*57.2957795);
}

//printf("AFTER a3 ROTATION\n");
//printf("x_listax[3].i=%f,x_listax[3].j=%f,x_listax[3].k=%f\n",(*x_listax[3]).i,(*x_listax[3]).j,(*x_listax[3]).k);
//printf("x_listax[4].i=%f,x_listax[4].j=%f,x_listax[4].k=%f\n",(*x_listax[4]).i,(*x_listax[4]).j,(*x_listax[4]).k);
//printf("x_listbx[3].i=%f,x_listbx[3].j=%f,x_listbx[3].k=%f\n",(*x_listbx[3]).i,(*x_listbx[3]).j,(*x_listbx[3]).k);


//printf("MARK10\n");

/*
strcpy(out,"A3out.pdb");
outputAsmblPDB(A, out);
strcpy(out,"B3out.pdb");
outputAsmblPDB(B, out);
*/
return;
}
