/*
Oliver Grant 7Feb11
This function was written as to fill in number of bonds info when a pdb was read in.
Finds the number of bonds an atom in a structure has based on distance
Only finds bonds to atoms within the residue ri. This should change for more general applications but it works well as is for carbohydrate grafting

*/
#include <glylib.h>
#define DIST 1.6
set_nbonds_for_atoms_in_assembly(assembly A){

int mi=0,ri=0,ai=0,aii=0,bi=0;//loop counters

double x,y,z,d; // xyz are holders to make code more readable. d is distance between two atoms
//compare dist of atom in x_list with all other atoms
for(mi=0;mi<A.nm;mi++){
        for(ri=0;ri<A.m[mi][0].nr;ri++){
                for(ai=0;ai<A.m[mi][0].r[ri].na;ai++){// ai is now current atom to check every other atom against
			for(aii=0;aii<A.m[mi][0].r[ri].na;aii++){//for current atom check dist to every other atom within the residue ri 
				x=(A.m[mi][0].r[ri].a[aii].x.i-A.m[mi][0].r[ri].a[ai].x.i);
				y=(A.m[mi][0].r[ri].a[aii].x.j-A.m[mi][0].r[ri].a[ai].x.j);
				z=(A.m[mi][0].r[ri].a[aii].x.k-A.m[mi][0].r[ri].a[ai].x.k);
				d=sqrt((x*x)+(y*y)+(z*z)); //pythagorus square on hyp in 3D
				//printf("\ndist is %f\n",d);// check that one of these is 0.000 (itself)
				if (d<=DIST && d>0.0){ 
					//increment number of bonds counter if atom within DIST but not itself (d>0.0)
					A.m[mi][0].r[ri].a[ai].nb=A.m[mi][0].r[ri].a[ai].nb+1; 
					//set source atom info, redundant in this case but hey it's there and I can fill it in	
				//	A.m[mi][0].r[ri].a[ai].b[bi].s.r=ri;
					bi++;
				}
			}
		}
	}
}
return;
}

