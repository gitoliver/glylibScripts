/*written by Oliver Grant 04/Jan/2011

written for automation of carbohydrate threading

CONTROLLER FUNCTION FOR CARBOHYDRATE THREADING

(will accept input from another function that calculates the minimal motifs and searches the library for matches to the minimal motifs)

will figure out which atoms are involved in branch and then call a function that will superimpose four atoms from one structure onto another

workflow:
read in pdb/top file with the crystal structure minimal binding motifs and protein structure
read in pdb/top file of the first library glycan

Based on minimal motif in library glycan, work out which atoms in motif are involved in branching to rest of glycan. 
Remove all atoms in library glycan minimal motif exept those 4 involved in the branch
Superimpose those 4 onto the equivalent atoms in the cyrstal structure bringing the rest of glcan along too

output the combined structure

*/
#include <glylib.h>

int main(){

//call lacheles function which returns all possible minimal motifs that would fit into the binding site

//call matts CASA function which returns all possible branch points on the motifs

//call function that searches the virtual library for matches to minimal motifs with branch points
//puts all of these structures in a subdirectory
int 


//for each structure in subdirectory call function that superimposes the four atoms from the branch onto the minimal motif
//checks for clashes and returns 0 (no clash), 1 (minor clash) or 2 (Major clash) OR value of clash for ranking
//prints name of glycan along with clash value into a results file
//puts structure in a subdirectory in pdb format

//call function that sorts results into seperate files by id number,clash rank,think of others

//each glycan in library will have a unique id for calling and for displaying results
//results will look like glycan array screening results :)
return 0;
}


int nc=6,mi=0,ri=0,ai=0;
assembly A,B;
coord_3D *x_list;
fileset IF, IC;



IF.N=strdup("motif_prmtop.top");
IF.F=myfopen(IF.N,"r");

IC.N=strdup("input_crd.crd");
IC.F=myfopen(IC.N,"r");

// load prmtop file that is in the directory
A=load_amber_prmtop(IF);
// load trajcrds into assembly
add_trajcrds_to_prmtop_assembly(IC,&A,'c',0);

x_list=(coord_3D*)calloc(nc, sizeof(coord_3D)); //creates a list to store co-ord


//read in pdb/top file with the crystal structure minimal binding motifs and protein structure
//read in pdb/top file of the first library glycan

//for res1 get coords and store in x_list
for(mi=0;mi<A.nm;mi++){
	for(ri=0;ri<A.m[mi][0].nr;ri++){
		if(A.m[mi][0].r[ri].n==res1){
			if (opt==1 || opt==2){//when res1 is a sugar residue
				for(ai=0;ai<A.m[mi][0].r[ri].na;ai++){
					if(strcmp(A.m[mi][0].r[ri].a[ai].N,"C1")==0){
						x_list[i]=A.m[mi][0].r[ri].a[ai].xa[0];}
                                        if(strcmp(A.m[mi][0].r[ri].a[ai].N,"C2")==0){
                                       		x_list[1]=A.m[mi][0].r[ri].a[ai].xa[0];}
  		                        if(strcmp(A.m[mi][0].r[ri].a[ai].N,"C3")==0){
                		                x_list[2]=A.m[mi][0].r[ri].a[ai].xa[0];}
                                	if(strcmp(A.m[mi][0].r[ri].a[ai].N,"C4")==0){
                                       		x_list[3]=A.m[mi][0].r[ri].a[ai].xa[0];}
 		                        if(strcmp(A.m[mi][0].r[ri].a[ai].N,"C5")==0){
                		                x_list[4]=A.m[mi][0].r[ri].a[ai].xa[0];}
                                	if(strcmp(A.m[mi][0].r[ri].a[ai].N,"O5")==0){
                                       		x_list[5]=A.m[mi][0].r[ri].a[ai].xa[0];}
				}
			}
			else {//when opt==3 then res1 will be a protein residue
				for (i=0;i<3;i++){
                                        for(ai=0;ai<A.m[mi][0].r[ri].na;ai++){
                                                if(strcmp(A.m[mi][0].r[ri].a[ai].N,atm_name)==0){
                                                         x_list[i]=A.m[mi][0].r[ri].a[ai].xa[0];


                                                        }
                                                }
                                        }
                                }
			}
		}
	}
if (opt==3) { //when opt==3 then res1 will be a protein residue
	P1=get_plane(x_list[0],x_list[1],x_list[2]);
}
else { //when res1 is a sugar residue
	P1=get_plane_for_ring(6,x_list);
}
//for res2
for(mi=0;mi<A.nm;mi++){
        for(ri=0;ri<A.m[mi][0].nr;ri++){
                if(A.m[mi][0].r[ri].n==res2){
                        if (opt==1){ //when res2 is a sugar molecule
                                for(ai=0;ai<A.m[mi][0].r[ri].na;ai++){
                                        if(strcmp(A.m[mi][0].r[ri].a[ai].N,"C1")==0){
                                                x_list[i]=A.m[mi][0].r[ri].a[ai].xa[0];}
                                        if(strcmp(A.m[mi][0].r[ri].a[ai].N,"C2")==0){
                                                x_list[1]=A.m[mi][0].r[ri].a[ai].xa[0];}
                                        if(strcmp(A.m[mi][0].r[ri].a[ai].N,"C3")==0){
                                                x_list[2]=A.m[mi][0].r[ri].a[ai].xa[0];}
                                        if(strcmp(A.m[mi][0].r[ri].a[ai].N,"C4")==0){
                                                x_list[3]=A.m[mi][0].r[ri].a[ai].xa[0];}
                                        if(strcmp(A.m[mi][0].r[ri].a[ai].N,"C5")==0){
                                                x_list[4]=A.m[mi][0].r[ri].a[ai].xa[0];}
                                        if(strcmp(A.m[mi][0].r[ri].a[ai].N,"O5")==0){
                                                x_list[5]=A.m[mi][0].r[ri].a[ai].xa[0];}
                                }
                        }
                        else {//when res2 is a protein
                                for (i=0;i<3;i++){
                                        for(ai=0;ai<A.m[mi][0].r[ri].na;ai++){
                                                if(strcmp(A.m[mi][0].r[ri].a[ai].N,atm_name2)==0){
                                                         x_list[i]=A.m[mi][0].r[ri].a[ai].xa[0];
                                                        }
                                                }
                                        }
                                }
                        }
                }
        }

return 0;
}
