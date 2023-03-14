/*
Oliver Grant 21Jan11
This function was written as part of Computational Carbohydrate Threading.

The four atoms in the branch make a Y shape. The Center atom is called a1, the bottom atom which, depending on the linkage, is a step closer to or is the branch glycan atom is a2, the top left atom is a3 and the top right atom is a4. Each pdb file of the search results will have the minimal motif removed except the 3 atoms involved in the branch. The atoms will be named a1-a4 in the incoming pdb.

The molecule is translated so the a1 atom lies on the origin. 
The a1-a2 vector is then aligned to the Z axis.
The a2-a3 vector is then rotated to the X axis.
In the future the a2-a3 and a2-a4 vectors will be best fitted to the corresponding motif vectors.
*/

/* This function is called by a bash script which runs it for each branch and merges the branch with the pdb*/

#include <glylib.h> 
superimpose_four_branch_atoms(assembly *A){ // *Ap is a pointer to the Assembly that is read in.
int na=0,mi=0,ri=0,ai=0;
coord_3D *x_lista;//whatever happens to assembly happens to x_list? no have to update :( pointers?
// I am not treating x_list as a pointer. i.e assigning the address and then deferencing x_list. 

//count the number of atoms in the assembly
for(mi=0;mi<(*A).nm;mi++){
        for(ri=0;ri<(*A).m[mi][0].nr;ri++){
                for(ai=0;ai<(*A).m[mi][0].r[ri].na;ai++){
			na++;}
	}
}
//creates list to store co-ord of the a1-4 atoms
x_lista=(coord_3D*)calloc(5, sizeof(coord_3D));//only 4 atoms but can refer to a1 as x_lista[1]

//get coord_3D of a1-4 atoms 
for(mi=0;mi<(*A).nm;mi++){
        for(ri=0;ri<(*A).m[mi][0].nr;ri++){
                for(ai=0;ai<(*A).m[mi][0].r[ri].na;ai++){
                        if(strcmp((*A).m[mi][0].r[ri].a[ai].N,"a1")==0){
                                x_lista[1]=(*A).m[mi][0].r[ri].a[ai].x;}
//x_list does not get updated when assembly changes. I thought it was a pointer... it is not.
//			if(strcmp((*A).m[mi][0].r[ri].a[ai].N,"a2")==0){
//                                x_lista[2]=(*A).m[mi][0].r[ri].a[ai].x;}
//                      if(strcmp((*A).m[mi][0].r[ri].a[ai].N,"a3")==0){
//                                x_lista[3]=(*A).m[mi][0].r[ri].a[ai].x;}
//                      if(strcmp((*A).m[mi][0].r[ri].a[ai].N,"a4")==0){
//			          x_lista[4]=(*A).m[mi][0].r[ri].a[ai].x;}
                }
        }
}

//translate all atoms in assembly by a1 atoms co-ord so a1 lies on the origin
for(mi=0;mi<(*A).nm;mi++){
        for(ri=0;ri<(*A).m[mi][0].nr;ri++){
                for(ai=0;ai<(*A).m[mi][0].r[ri].na;ai++){
                        (*A).m[mi][0].r[ri].a[ai].x.i=(((*A).m[mi][0].r[ri].a[ai].x.i) - (x_lista[1].i));
                        (*A).m[mi][0].r[ri].a[ai].x.j=(((*A).m[mi][0].r[ri].a[ai].x.j) - (x_lista[1].j));
                        (*A).m[mi][0].r[ri].a[ai].x.k=(((*A).m[mi][0].r[ri].a[ai].x.k) - (x_lista[1].k));
                }
        }
}

//have to update coord of a2 atom. really should be able to use a pointer...
for(mi=0;mi<(*A).nm;mi++){
        for(ri=0;ri<(*A).m[mi][0].nr;ri++){
                for(ai=0;ai<(*A).m[mi][0].r[ri].na;ai++){
                        if(strcmp((*A).m[mi][0].r[ri].a[ai].N,"a2")==0){
                                x_lista[2]=(*A).m[mi][0].r[ri].a[ai].x;}
                }
        }
}

//Rotate the a1(now@origin)-a2 vector to the Z axis
vectormag_3D v12A; 
v12A = coord_to_vec(x_lista[2]);
int xs=-1,xl=-1,vs=-1,vl=-1; //parameters for rotate_vector_to_Z_M
rotate_vector_to_Z_M((*A).m[0],xs,xl,vs,vl,v12A);

//get new coord of a3 and a4 atoms 
for(mi=0;mi<(*A).nm;mi++){
        for(ri=0;ri<(*A).m[mi][0].nr;ri++){
                for(ai=0;ai<(*A).m[mi][0].r[ri].na;ai++){
                        if(strcmp((*A).m[mi][0].r[ri].a[ai].N,"a3")==0){
                                x_lista[3]=(*A).m[mi][0].r[ri].a[ai].x;}
			if(strcmp((*A).m[mi][0].r[ri].a[ai].N,"a4")==0){
                                x_lista[4]=(*A).m[mi][0].r[ri].a[ai].x;}
                }
        }
}

//Get angle between the a2-a3 bond and the X axis for both molecules and yaw to x-axis
double angle3a2x;//angle4a2x,diffa,; had an idea to get difference but need to think

angle3a2x=atan(x_lista[3].j/x_lista[3].i);
//angle4a2x=atan(x_lista[4].j/x_lista[4].i);
//diffa=(angle3a2x+angle4a2x/2);

//when x is negative add 180 degrees (PI in radians) so it rotates to the +ve x axis
if ( x_lista[3].i<0 ) {	angle3a2x=angle3a2x+3.14159265;}

yawAssembly(A,angle3a2x);

//write out assembly into a pdb in a results subfolder
char out[20];
strcpy(out,"superimposed.pdb");
outputAsmblPDB(A, out);
return ;
}
