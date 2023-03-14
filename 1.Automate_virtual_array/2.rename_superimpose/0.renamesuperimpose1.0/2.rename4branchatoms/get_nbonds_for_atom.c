/*
Oliver Grant 7Feb11
This function was written as part of Computational Carbohydrate Threading.

Finds the number of bonds an atom in a structure has based on distance

*/
#include <glylib.h>
get_nbonds_for_atom(assembly A, char *aN, int resid, int *nbp ){
//aN is name of atom to get number of bonds, resid is it's residue number, nbp is number of bonds worked out here
int mi=0,ri=0,ai=0;//loop counters

*nbp=-1;//as it will include itself as within 1.6 when searching atoms
double x,y,z,d; // xyz are holders to make code more readable. d is distance between two atoms
coord_3D x_list; //coord of bonding info atom

// get co_ord of atom of interest and store in x_list
for(mi=0;mi<A.nm;mi++){
        for(ri=0;ri<A.m[mi][0].nr;ri++){
		if(A.m[mi][0].r[ri].n==resid){
                	for(ai=0;ai<A.m[mi][0].r[ri].na;ai++){
                        	if(strcmp(A.m[mi][0].r[ri].a[ai].N,aN)==0){
					x_list=A.m[mi][0].r[ri].a[ai].x;}

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
			printf("\ndist is %f\n",d);// check that one of these is 0.000 (itself)
			if (d<=1.6){ *nbp=*nbp+1;}
		}
	}
}
printf("\n\nNumber of bonds=%d\n\n",*nbp);//print within the function and outside to check
return;
}

