//The four atoms make a Y shape. The Center atom is called a1, the bottom atom is a2, the top left atom is a3 and the top right atom is a4

#include <glylib.h>
int main(void){
int ncA=0,ncB=0,mi=0,ri=0,ai=0;
assembly A,B;
coord_3D *x_listA, *x_listB,*x_lista, *x_listb;//whatever happens to x_list happens to assembly
//fileset IF, IC;
char motifpdb[25];

/*
IF.N=strdup("motif.pdb");
IF.F=myfopen(IF.N,"r");
*/
//load the motif into assembly A
strcpy(motifpdb,"motif.pdb");
//motifpdb[9]='\o';//this is really crap. there must be a better way... 

A=*load_pdb(motifpdb);

strcpy(motifpdb,"1.library.pdb");
B=*load_pdb(motifpdb);
printf("\nmark");
//cp "Search_Results/*" ".";
//for loop to keep going through search directory until all files have been superimposed
/*IC.N=strdup("search_result.pdb");
printf("Working on %s...",IC.N);
IC.F=myfopen(IC.N,"r");
*/
//load the library structure into an assembly

//figure out which four atoms are to be superimposed
//each pdb file of the search results will have the minimal motif removed except the 3 atoms involved in the branch. The resname of these atoms along with the 4th connecting atom will be changed to BRC to make them it identifiable here. this will be done by the search function
//the atoms will be ordered sequentially in BRC from a1-a4 for x_list

//count the number of atoms in each assembly
for(mi=0;mi<A.nm;mi++){
        for(ri=0;ri<A.m[mi][0].nr;ri++){
                for(ai=0;ai<A.m[mi][0].r[ri].na;ai++){
			ncA+=1;}
	}
}

printf("\nmark2");

for(mi=0;mi<B.nm;mi++){
        for(ri=0;ri<B.m[mi][0].nr;ri++){
                for(ai=0;ai<B.m[mi][0].r[ri].na;ai++){
                        ncB+=1;}
        }
}
printf("\nmark3");
printf("\nB contains %d atoms",ncB);
printf("\nA contains %d atoms",ncA);

//creates lists to store co-ord of all atoms
x_listA=(coord_3D*)calloc(ncA, sizeof(coord_3D));
x_listB=(coord_3D*)calloc(ncB, sizeof(coord_3D));
//creates lists to store co-ord of the a1-4 atoms

x_lista=(coord_3D*)calloc(5, sizeof(coord_3D));//only 4 atoms but can refer to a1 as x_lista[1]
x_listb=(coord_3D*)calloc(5, sizeof(coord_3D));

//coord_3D x_lista; //for the translate to origin 
//coord_3D x_listb;
printf("\nmark4");


//get coord_3D of a1-4 atoms in both structures 
for(mi=0;mi<A.nm;mi++){
        for(ri=0;ri<A.m[mi][0].nr;ri++){
                for(ai=0;ai<A.m[mi][0].r[ri].na;ai++){
                        if(strcmp(A.m[mi][0].r[ri].a[ai].N,"a1")==0){
                                x_lista[1]=A.m[mi][0].r[ri].a[ai].x;}
			if(strcmp(A.m[mi][0].r[ri].a[ai].N,"a2")==0){
                                x_lista[2]=A.m[mi][0].r[ri].a[ai].x;}
                        if(strcmp(A.m[mi][0].r[ri].a[ai].N,"a3")==0){
                                x_lista[3]=A.m[mi][0].r[ri].a[ai].x;}
                        if(strcmp(A.m[mi][0].r[ri].a[ai].N,"a4")==0){
                                x_lista[4]=A.m[mi][0].r[ri].a[ai].x;}
                         				
                }
        }
}
for(mi=0;mi<B.nm;mi++){
        for(ri=0;ri<B.m[mi][0].nr;ri++){
		for(ai=0;ai<B.m[mi][0].r[ri].na;ai++){
			if(strcmp(B.m[mi][0].r[ri].a[ai].N,"a1")==0){ 
				x_listb[1]=B.m[mi][0].r[ri].a[ai].x;}
			if(strcmp(B.m[mi][0].r[ri].a[ai].N,"a2")==0){
                        	x_listb[2]=B.m[mi][0].r[ri].a[ai].x;}
                	if(strcmp(B.m[mi][0].r[ri].a[ai].N,"a3")==0){
                        	x_listb[3]=B.m[mi][0].r[ri].a[ai].x;}
			if(strcmp(B.m[mi][0].r[ri].a[ai].N,"a4")==0){
				x_listb[4]=B.m[mi][0].r[ri].a[ai].x;}
		}
	}		
}

//translate both assemblies by a1 atoms co-ord so they lie on the origin

for(mi=0;mi<A.nm;mi++){
        for(ri=0;ri<A.m[mi][0].nr;ri++){
                for(ai=0;ai<A.m[mi][0].r[ri].na;ai++){
                        A.m[mi][0].r[ri].a[ai].x.i=((A.m[mi][0].r[ri].a[ai].x.i) - (x_lista[1].i));
                        A.m[mi][0].r[ri].a[ai].x.j=((A.m[mi][0].r[ri].a[ai].x.j) - (x_lista[1].j));
                        A.m[mi][0].r[ri].a[ai].x.k=((A.m[mi][0].r[ri].a[ai].x.k) - (x_lista[1].k));
                }
        }
}

for(mi=0;mi<B.nm;mi++){
        for(ri=0;ri<B.m[mi][0].nr;ri++){
                for(ai=0;ai<B.m[mi][0].r[ri].na;ai++){
		 	B.m[mi][0].r[ri].a[ai].x.i=((B.m[mi][0].r[ri].a[ai].x.i) - (x_listb[1].i));
			B.m[mi][0].r[ri].a[ai].x.j=((B.m[mi][0].r[ri].a[ai].x.j) - (x_listb[1].j));
                	B.m[mi][0].r[ri].a[ai].x.k=((B.m[mi][0].r[ri].a[ai].x.k) - (x_listb[1].k));
                }
        }
}

//#####################################################################################################
/*
//update x_listA/B with new co-ord
for(mi=0;mi<A.nm;mi++){
       for(ri=0;ri<A.m[mi][0].nr;ri++){
	     	for(ai=0;ai<A.m[mi][0].r[ri].na;ai++){
    	           	x_listA[ai]=A.m[mi][0].r[ri].a[ai].x;}
  	  }
}

for(mi=0;mi<B.nm;mi++){
       for(ri=0;ri<B.m[mi][0].nr;ri++){
                for(ai=0;ai<B.m[mi][0].r[ri].na;ai++){
                        x_listB[ai]=B.m[mi][0].r[ri].a[ai].x;}
          }
}
*/
//have to update coord of a2 atoms in xlists but really should be able to use a pointer...
for(mi=0;mi<A.nm;mi++){
        for(ri=0;ri<A.m[mi][0].nr;ri++){
                for(ai=0;ai<A.m[mi][0].r[ri].na;ai++){
                        if(strcmp(A.m[mi][0].r[ri].a[ai].N,"a2")==0){
                                x_lista[2]=A.m[mi][0].r[ri].a[ai].x;}
                }
        }
}
for(mi=0;mi<B.nm;mi++){
        for(ri=0;ri<B.m[mi][0].nr;ri++){
                for(ai=0;ai<B.m[mi][0].r[ri].na;ai++){
                        if(strcmp(B.m[mi][0].r[ri].a[ai].N,"a2")==0){
                                x_listb[2]=B.m[mi][0].r[ri].a[ai].x;}
                }
	}
}


//Rotate the a1-a2 vector to the Z axis
vectormag_3D v12A; //vector from a1(origin) to a2
vectormag_3D v12B;

v12A = coord_to_vec(x_lista[2]);
v12B = coord_to_vec(x_listb[2]);

int xs=-1,xl=-1,vs=-1,vl=-1; //parameters for rotate_vector_to_Z_M

rotate_vector_to_Z_M(A.m[0],xs,xl,vs,vl,v12A);
rotate_vector_to_Z_M(B.m[0],xs,xl,vs,vl,v12B);


//########################################################################################

//get coord of a3 atoms and store in x_lista/b
for(mi=0;mi<A.nm;mi++){
        for(ri=0;ri<A.m[mi][0].nr;ri++){
                for(ai=0;ai<A.m[mi][0].r[ri].na;ai++){
                        if(strcmp(A.m[mi][0].r[ri].a[ai].N,"a3")==0){
                                x_lista[3]=A.m[mi][0].r[ri].a[ai].x;}
			if(strcmp(A.m[mi][0].r[ri].a[ai].N,"a4")==0){
                                x_lista[4]=A.m[mi][0].r[ri].a[ai].x;}
                }
        }
}
for(mi=0;mi<B.nm;mi++){
        for(ri=0;ri<B.m[mi][0].nr;ri++){
                for(ai=0;ai<B.m[mi][0].r[ri].na;ai++){
                        if(strcmp(B.m[mi][0].r[ri].a[ai].N,"a3")==0){
                                x_listb[3]=B.m[mi][0].r[ri].a[ai].x;}
			if(strcmp(B.m[mi][0].r[ri].a[ai].N,"a4")==0){
                                x_listb[4]=B.m[mi][0].r[ri].a[ai].x;}              
               }
        }
}


//Get angle between the a2-a3 bond and the X axis for both molecules and yaw to x-axis
double angle3a2x,angle3b2x,angle4a2x,angle4b2x,diffa,diffb;
assembly *Ap=&A,*Bp=&B;//yawAssembly requires pointers

angle3a2x=atan(x_lista[3].j/x_lista[3].i);
angle4a2x=atan(x_lista[4].j/x_lista[4].i);
//diffa=(angle3a2x+angle4a2x/2);
yawAssembly(Ap,angle3a2x);

angle3b2x=atan(x_listb[3].j/x_listb[3].i);
angle4b2x=atan(x_listb[4].j/x_listb[4].i);
//diffb=(angle3b2x+angle4b2x/2);
yawAssembly(Bp,angle3b2x);


// Delete the a1-4 atoms of the minimal motif and join the rest into one assembly.

//write out assembly into a pdb in a results subfolder

char out[10];
strcpy(out,"OUTB.pdb");
outputAsmblPDB(Bp, out);


strcpy(out,"OUTA.pdb");
outputAsmblPDB(Ap, out);


return 0;
}
