/*
Oliver Grant 14Mar11
Finds clashes between atoms in assemblies in a pairwize fashion. 
NOTE: xsA and xsB refer to the co-ord sets to use. As this is a clash checker I'm not sure of the usefulness of checking clashes over a traj.
*/
#include <glylib.h>
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

// nbr is number of molecules in ligand.pdb. So skip checking first nbr molecules in A
double find_vdw_clashes_pairwize_between_Assemblies(assembly *A, assembly *B, char *fileprefix, int *core_ri, int wiggleWindow){ 
int mi=0,mii=0,ri=0,rii=0,ai=0,aii=0;//loop counters for A(i) and B(ii) assemblies
double soap, total_soap=0.0,resid_soap,prev_soap; // Sphere Overlap Area P? (Can't think of a better one)
double rA,rB; // radii
double x,y,z,d; // xyz are holders to make code more readable. d is distance between two atoms
FILE * pFile; // Clash results out file
char filename[200]; // Name of out file
int numAtm=0;
int swtch=0;
double ovrlap_eqv; //Equivalent overlap. So can say overlap is equivalent to X occluded atoms

//compare dist between each atom in A assembly and all atoms in B assembly
printf("Entered the vdw FUNction\n");

sprintf(filename,"%s_overlaps",fileprefix);
//strcpy(filename,"Results.txt");
pFile = fopen (filename,"a+"); // open file and append
fprintf(pFile,"%s\n",fileprefix);
fclose (pFile);


for(mi=0;mi<(*A).nm;mi++){
        for(ri=0;ri<(*A).m[mi][0].nr;ri++){
                for(ai=0;ai<(*A).m[mi][0].r[ri].na;ai++){// ai is now current atom to check against every atom in assembly B
			for(mii=0;mii<(*B).nm;mii++){
				for(rii=0;rii<(*B).m[mii][0].nr;rii++){
					for(aii=0;aii<(*B).m[mii][0].r[rii].na;aii++){//for current atom check dist to every other atom within the assembly B
						//printf("Comparing Values\n rA=%f\n",rA);
                                		x=((*A).m[mi][0].r[ri].a[ai].x.i-(*B).m[mii][0].r[rii].a[aii].x.i);
                                		y=((*A).m[mi][0].r[ri].a[ai].x.j-(*B).m[mii][0].r[rii].a[aii].x.j);
                                		z=((*A).m[mi][0].r[ri].a[ai].x.k-(*B).m[mii][0].r[rii].a[aii].x.k);
                                		d=sqrt((x*x)+(y*y)+(z*z)); //pythagorus square on hyp in 3D
                                		//printf("\ndist is %f\n",d);
						// set radii of the atoms
						// looking back I'm not sure if I confirmed that N[0] would always be the element type...
						if ((*A).m[mi][0].r[ri].a[ai].N[0]=='C'){rA=1.70;} 
						if ((*A).m[mi][0].r[ri].a[ai].N[0]=='O'){rA=1.52;} 
						if ((*A).m[mi][0].r[ri].a[ai].N[0]=='N'){rA=1.55;} 
						if ((*A).m[mi][0].r[ri].a[ai].N[0]=='S'){rA=1.80;} 

						if ((*B).m[mii][0].r[rii].a[aii].N[0]=='C'){rB=1.70;}
                                                if ((*B).m[mii][0].r[rii].a[aii].N[0]=='O'){rB=1.52;}
						if ((*B).m[mii][0].r[rii].a[aii].N[0]=='N'){rB=1.55;}
						if ((*B).m[mii][0].r[rii].a[aii].N[0]=='S'){rB=1.80;}
						
						//printf("Dist is %f, rA is %f, rB is %f \n\n",d,rA,rB);
						// if the sum of the radii is greater than the distance between them.
						if (rA + rB > d + 0.6){ // 0.6 overlap is deemed acceptable. (Copying chimera:) 
							swtch=1;
  							soap=2*PI*rA*(rA-d/2-(((rA*rA)-(rB*rB))/(2*d))); 
							// Eqn 1, Rychkov and Petukhov, J. Comput. Chem., 2006, Joint Neighbours...
							// NOTE this eqn does account for double overlaps. 
							// NOTE	Each atom is counted against each atom. 
							// NOTE so overlap may be double counted and a higher overlap value will result
							// NOTE the math is availabe in that paper for correcting this but 
							// NOTE I want to switch to a LJ type eqn anyway.
							total_soap=total_soap + soap;
							pFile = fopen (filename,"a+"); // open file and append
							fprintf(pFile,"\t\t%s:%s=%5.1f ",(*A).m[mi][0].r[ri].N,(*A).m[mi][0].r[ri].a[ai].N,soap);
							fprintf(pFile,"with %s:%s\n",(*B).m[mii][0].r[rii].N,(*B).m[mii][0].r[rii].a[aii].N);
							fclose (pFile);
						}
					}
				}
                        }
			if (swtch==1){
				numAtm++; // then this atom (should be a sugar atom) ai had some contacts 
			}
			swtch=0; // reset for next atom
		}
		resid_soap=(total_soap - prev_soap);
		pFile = fopen (filename,"a+");
		fprintf(pFile,"\t%s=%5.1f\n",(*A).m[mi][0].r[ri].N,resid_soap);
		fclose (pFile);
		prev_soap=total_soap;
        }
}
ovrlap_eqv=(total_soap/CSA);
//total_soap=(total_soap / numAtm);
total_soap=(total_soap+0.5); // so can round off by truncation

if (ovrlap_eqv<=1.0){
pFile = fopen (filename,"a+");
fprintf(pFile,"TOTAL=%.1f | BINDER\n",ovrlap_eqv);
fclose (pFile);	
}

// Need some distance from core based metric, big clash far == small clash close
if (ovrlap_eqv>1.0){
	pFile = fopen (filename,"a+");
	fprintf(pFile,"TOTAL=%.1f,",ovrlap_eqv);
	fclose (pFile);
	if (wiggleWindow!=0) {
		total_soap=wiggler(A,B,core_ri,wiggleWindow);
		ovrlap_eqv=(total_soap/CSA);
		printf("ovrlap_eqv=%f\n",ovrlap_eqv);
		if (ovrlap_eqv<=1.0){
			pFile = fopen (filename,"a+");
        		fprintf(pFile,"PostWiggle=%.1f | BINDER\n",ovrlap_eqv);
        		fclose (pFile);
		}
		if (ovrlap_eqv>1.0){
        		pFile = fopen (filename,"a+");
        		fprintf(pFile,"PostWiggle=%.1f | CLASHER\n",ovrlap_eqv);
        		fclose (pFile);
		}
	}
}
//printf("total_soap=%f,ovrlap_eqv=%f\n",total_soap,ovrlap_eqv);
return total_soap;
}
