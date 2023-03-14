/*
Oliver Grant 14Mar11
Finds clashes between atoms in assemblies in a pairwize fashion. 
NOTE: xsA and xsB refer to the co-ord sets to use. As this is a clash checker I'm not sure of the usefulness of checking clashes over a traj.
*/
#include <mylib.h> 
#include <molecules.h>
// Bondi definitions of VDW in Angstrom HERE AS A REFERENCE, SET IN CODE BELOW
//#define H 	1.09 // Rowland and Taylor modification
//#define C	1.70
//#define N 	1.55
//#define O	1.52
//#define F 	1.47
//#define P	1.80
//#define S	1.80
//#define Cl	1.75

// Carbon Surface Area for normalizing results to the area of a carbon atom. Can ask the question: How many C atom equivalents are inside the protein? 
#define CSA 36.31681103 

double find_vdw_clashes_return_totalToSite(assembly *A, assembly *B, int *residToIgnore){ // xs is co-ord set to use. -1 for main co-ord set
int mi=0,mii=0,ri=0,rii=0,ai=0,aii=0;//loop counters for A(i) and B(ii) assemblies
double soap=0.0, total_soap=0.0; // Sphere Overlap Area P? (Can't think of a better one)
double rA,rB; // radii
double x,y,z,d; // xyz are holders to make code more readable. d is distance between two atoms
int i=0,ignore=0;

for(mi=0;mi<(*A).nm;mi++){
    for(ri=0;ri<(*A).m[mi][0].nr;ri++){
        for(ai=0;ai<(*A).m[mi][0].r[ri].na;ai++){// ai is now current atom to check against every atom in assembly B
            for(mii=0;mii<(*B).nm;mii++){
	        for(rii=0;rii<(*B).m[mii][0].nr;rii++){
                    ignore=0; //reset for each rii
                    for(i=1;i<=residToIgnore[0];i++){
                        if (residToIgnore[i]==rii){ignore=1;}
                    }
                    if (ignore==0){//if not ignore this rii
		        for(aii=0;aii<(*B).m[mii][0].r[rii].na;aii++){//for current atom check dist to every other atom within the assembly B
			    //printf("Comparing Values\n rA=%f\n",rA);
                            x=((*A).m[mi][0].r[ri].a[ai].x.i-(*B).m[mii][0].r[rii].a[aii].x.i);
                            y=((*A).m[mi][0].r[ri].a[ai].x.j-(*B).m[mii][0].r[rii].a[aii].x.j);
                            z=((*A).m[mi][0].r[ri].a[ai].x.k-(*B).m[mii][0].r[rii].a[aii].x.k);
                            d=sqrt((x*x)+(y*y)+(z*z)); //pythagorus square on hyp in 3D
                            //printf("\ndist is %f\n",d);
			    if ((*A).m[mi][0].r[ri].a[ai].N[0]=='C'){rA=1.70;} 
			    if ((*A).m[mi][0].r[ri].a[ai].N[0]=='O'){rA=1.52;} 
			    if ((*A).m[mi][0].r[ri].a[ai].N[0]=='N'){rA=1.55;} 
			    if ((*A).m[mi][0].r[ri].a[ai].N[0]=='S'){rA=1.80;} 
			    if ((*B).m[mii][0].r[rii].a[aii].N[0]=='C'){rB=1.70;}
                            if ((*B).m[mii][0].r[rii].a[aii].N[0]=='O'){rB=1.52;}
			    if ((*B).m[mii][0].r[rii].a[aii].N[0]=='N'){rB=1.55;}
			    if ((*B).m[mii][0].r[rii].a[aii].N[0]=='S'){rB=1.80;}
			    //printf("Dist is %f, rA is %f, rB is %f \n\n",d,rA,rB);
                            if ( (rA + rB > d) && (d>0.0) ){ // 2014/12/18 OGedit. Too much overlap when wiggleToSite.
			   // if ( (rA + rB > d + 0.6) && (d>0.0) ){ // 0.6 overlap is deemed acceptable. (Copying chimera:) d will be 0.0 when atom is superimposed onto another
  				soap=2*PI*rA*(rA-d/2-(((rA*rA)-(rB*rB))/(2*d)));
				//printf("Soap=%f,rA=%f,rB=%f,d=%f\n",soap,rA,rB,d); 
				total_soap=total_soap + soap;
			    }
			}
		    }
                }
	    }
        }
    }
}
//printf("total_soap=%.2f\n",total_soap);
return total_soap; // 
}
