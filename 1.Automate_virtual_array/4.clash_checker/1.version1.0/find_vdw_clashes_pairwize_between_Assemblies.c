/*
Oliver Grant 14Mar11
Finds clashes between atoms in assemblies in a pairwize fashion.
*/
#include <glylib.h>
// Bondi definitions of VDW in Angstrom
//#define H 	1.09 // Rowland and Taylor modification
//#define C	1.70
//#define N 	1.55
//#define O	1.52
//#define F 	1.47
//#define P	1.80
//#define S	1.80
//#define Cl	1.75
//#define PI 3.1415926535897932384626433832795028841971693993751058209749445923078164062862089986280348253421170679 //lols?

find_vdw_clashes_pairwize_between_Assemblies(assembly *A, int xsA, assembly *B, int xsB){ // xs is co-ord set to use. -1 for main co-ord set
int mi=0,mii=0,ri=0,rii=0,ai=0,aii=0;//loop counters for A(i) and B(ii) assemblies
double soap; // Sphere Overlap Area P?
double rA,rB; // radii
double x,y,z,d; // xyz are holders to make code more readable. d is distance between two atoms
//compare dist between each atom and all other atoms in that residue
for(mi=0;mi<(*A).nm;mi++){
        for(ri=0;ri<(*A).m[mi][0].nr;ri++){
                for(ai=0;ai<(*A).m[mi][0].r[ri].na;ai++){// ai is now current atom to check against every atom in assembly B
			for(mii=0;mii<(*B).nm;mii++){
				for(rii=0;rii<(*B).m[mii][0].nr;rii++){
					for(aii=0;aii<(*B).m[mii][0].r[rii].na;aii++){//for current atom check dist to every other atom within the assembly B
                                		x=((*A).m[mi][0].r[ri].a[ai].x.i-(*B).m[mii][0].r[rii].a[aii].x.i);
                                		y=((*A).m[mi][0].r[ri].a[ai].x.j-(*B).m[mii][0].r[rii].a[aii].x.j);
                                		z=((*A).m[mi][0].r[ri].a[ai].x.k-(*B).m[mii][0].r[rii].a[aii].x.k);
                                		d=sqrt((x*x)+(y*y)+(z*z)); //pythagorus square on hyp in 3D
                                		//printf("\ndist is %f\n",d);
						// find radii of the atoms
						rA=1.4; //(*A).m[mi][0].r[ri].a[ai].N[0]; // or better .T? is this set?
						rB=1.4; //(*B).m[mii][0].r[rii].a[aii].N[0];
						if (strcmp((*A).m[mi][0].r[ri].a[ai].T,"C")==0){rA=1.70;}
						if (strcmp((*A).m[mi][0].r[ri].a[ai].T,"N")==0){rA=1.55;}
						if (strcmp((*A).m[mi][0].r[ri].a[ai].T,"O")==0){rA=1.52;}
						
						if (strcmp((*B).m[mi][0].r[ri].a[ai].T,"C")==0){rB=1.70;}
                                                if (strcmp((*B).m[mi][0].r[ri].a[ai].T,"N")==0){rB=1.55;}
                                                if (strcmp((*B).m[mi][0].r[ri].a[ai].T,"O")==0){rB=1.52;}

						if (rA + rB > d){ // if the sum of the radii is greater than the distance between them.
  							soap=2*PI*rA*(rA-d/2-(((rA*rA)-(rB*rB))/(2*d))); // Eqn 1, Rychkov and Petukhov, J. Comput. Chem., 2006, Joint Neighbours... google it.
							printf("soap of A.m[%d][0].r[%d].a[%d].%c=%f\n",mi,ri,ai,(*A).m[mi][0].r[ri].a[ai].T,soap); 
						}
					}
				}
                        }
                }
        }
}
return 0;
}
