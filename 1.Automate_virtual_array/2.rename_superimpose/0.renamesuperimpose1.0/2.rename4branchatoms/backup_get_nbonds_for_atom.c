/*
Oliver Grant 7Feb11
This function was written as part of Computational Carbohydrate Threading.

Finds the number of bonds an atom in a structure has based on distance

*/

#include <glylib.h>
int main (void){

int mi=0,ri=0,ai=0;
assembly A;

char pdbName[25];
strcpy(pdbName,"pdbName.pdb");
//load pdb into assembly A
A=*load_pdb(pdbName);
int nb=-1;//as it will include itself as within 1.6 when searching atoms
double x,y,z,d; // xyz are holders to make code more readable. d is distance between two atoms
coord_3D x_list; //coord of bonding info atom

// get co_ord of atom of interest and store in x_list
for(mi=0;mi<A.nm;mi++){
        for(ri=0;ri<A.m[mi][0].nr;ri++){
		if(A.m[mi][0].r[ri].n==3){
                	for(ai=0;ai<A.m[mi][0].r[ri].na;ai++){
                        	if(strcmp(A.m[mi][0].r[ri].a[ai].N,"O6")==0){
					x_list=A.m[mi][0].r[ri].a[ai].x;
			}
		}
	}
}

//compare dist of atom in x_list with all other atoms
for(mi=0;mi<A.nm;mi++){
        for(ri=0;ri<A.m[mi][0].nr;ri++){
                for(ai=0;ai<A.m[mi][0].r[ri].na;ai++){
			x=(x_list.i-A.m[mi][0].r[ri].a[ai].x.i);
			y=(x_list.j-A.m[mi][0].r[ri].a[ai].x.j);
			z=(x_list.k-A.m[mi][0].r[ri].a[ai].x.k);
			d=sqrt((x*x)+(y*y)+(z*z)); //pythagorus square on hyp in 3D
			printf("\ndist is %f\n",d);
			if (d<=1.6){ nb++;}
		}
	}
}
printf("\n\nNumber of bonds=%d\n\n",nb);
return 0;
}
