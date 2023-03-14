//The four atoms make a Y shape. The Center atom is called a1, the bottom atom is a2, the top left atom is a3 and the top right atom is a4

#include <glylib.h>

int main(){
int ncA=0,ncB=0,mi=0,ri=0,ai=0;
assembly A,B;
coord_3D *x_listA, *x_listB;
fileset IF, IC;
char motifpdb[10];

/*
IF.N=strdup("motif.pdb");
IF.F=myfopen(IF.N,"r");
*/
//load the motif into assembly A
strcpy(motifpdb,"motif.pdb");
//motifpdb[9]='\o';//this is really crap. there must be a better way... 

A=*load_pdb(motifpdb);

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

printf("\nmark");

for(mi=0;mi<B.nm;mi++){
        for(ri=0;ri<B.m[mi][0].nr;ri++){
                for(ai=0;ai<B.m[mi][0].r[ri].na;ai++){
                        ncB+=1;}
        }
}
printf("\nmark3");


//creates a list to store co-ord of all atoms
x_listA=(coord_3D*)calloc(ncA, sizeof(coord_3D));
x_listB=(coord_3D*)calloc(ncB, sizeof(coord_3D));

printf("\nmark4");


//get coord_3D of atom a1 to transform to origin
for(mi=0;mi<A.nm;mi++){
        for(ri=0;ri<A.m[mi][0].nr;ri++){
		if(strcmp(A.m[mi][0].r[ri].N,"BRC")==0){ 
			x_listA[1]=A.m[mi][0].r[ri].a[1].xa[0];}
	}		
}
printf("\nmark5");


for(mi=0;mi<B.nm;mi++){
        for(ri=0;ri<B.m[mi][0].nr;ri++){
		if(strcmp(A.m[mi][0].r[ri].N,"MOT")==0){
                	x_listB[1]=B.m[mi][0].r[ri].a[1].xa[0];}
        }
}

printf("\nmark6");


//translate both a1 atoms so they lie on the origin
for(mi=0;mi<A.nm;mi++){
        for(ri=0;ri<A.m[mi][0].nr;ri++){
		for(ai=0;ai<A.m[mi][0].r[ri].na;ai++){ //for each atoms co_ord
			//take to Co-ord of a1 atom away, prob need a function
			A.m[mi][0].r[ri].a[ai].xa[0].i=(A.m[mi][0].r[ri].a[ai].xa[0].i-x_list[1].i);
			A.m[mi][0].r[ri].a[ai].xa[0].j=(A.m[mi][0].r[ri].a[ai].xa[0].i-x_list[1].j);
			A.m[mi][0].r[ri].a[ai].xa[0].k=(A.m[mi][0].r[ri].a[ai].xa[0].i-x_list[1].k);
		}
	}
}

//repeat for B
printf("\nmark7");

//update x_list with new co-ord
for(mi=0;mi<A.nm;mi++){
        for(ri=0;ri<A.m[mi][0].nr;ri++){
        	for(ai=0;ai<A.m[mi][0].r[ri].na;ai++){
//copy the coord of all the atoms in library struct into x_list
                	x_listA[ai]=A.m[mi][0].r[ri].a[ai].xa[0];}
                }
        }
}

printf("\nmark8");

//repeat for B

//Rotate the a1a2 vector to the Z axis
vectormag_3D v12A; //vector from a1(origin) to a2

v12A = coord_to_vec(x_listA[2]);

rotate_vector_to_Z_list(x_listA,ncA,v12A);

/*
//this is a vector which tells me how to get from a1 to a2.
v1.i=(x_list[1][i]-x_list[2][i]);
v1.j=(x_list[1][j]-x_list[2][j]);
v1.k=(x_list[1][k]-x_list[2][k]);
v1.d=sqrt(v1.i*v1.i + v1.j*v1.j + v1.k*v1.k)
*/



//Get angle between the a2-a3 bond and the X axis 
double angle2x,rad;

angle2x=atan(x_listA[3].j/x_listA[3].i);
rad=(angle2x*0.0174532925);//convert to radians for yawAssembly function

//and rotate both molecules so they lie along the x-axis
assembly *Ap;
yawAssembly(Ap,rad);

// Delete the a1-4 atoms of the minimal motif and join the rest into one assembly.

//write out assembly into a pdb in a results subfolder

char out[10];
strcpy(out,"OUT.pdb");
outputAsmblPDB(Ap, out);


return 0;
}
