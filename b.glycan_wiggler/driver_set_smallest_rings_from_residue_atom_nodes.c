#include <glylib.h> // required for load_pdb function
#include <mylib.h>
#include <molecules.h>

int main (void){

char filename[50];

assembly B;
strcpy(filename, "receptor.pdb");
B=*load_pdb(filename);

assembly A;
strcpy(filename, "ligand.pdb");
A=*load_pdb(filename);
//set_nbonds_for_atoms_in_assembly(&A);
//set_nmb_for_atoms_in_assembly(&A);

int core_ri[5];
core_ri[0]=1; // number of residues in core is stored in 0.
core_ri[1]=7;
core_ri[2]=2;
core_ri[3]=3;
core_ri[4]=4;

int ai,ri,mi;
/*
for (mi=0;mi<A.nm;mi++){
	for (ri=0;ri<A.m[mi][0].nr;ri++){
		for (ai=0;ai<A.m[mi][0].r[ri].na;ai++){
			printf("mi=%d,ri=%d,ai=%d\n",mi,ri,ai);
		}
	}
}
*/
printf("A.m[0][0].r[0].aT[0].isorigin=%c\n",A.m[0][0].r[0].aT[0].isorigin);

//printf("r=%d\n",A.m[0][1].r[1].n);

for (ri=0;ri<A.m[0][0].nr;ri++){
	if (A.m[0][0].r[ri].na>1){
		set_smallest_rings_from_residue_atom_nodes(&A.m[0][0].r[ri]);
	}
}

for (mi=0;mi<A.nm;mi++){
        for (ri=0;ri<A.m[mi][0].nr;ri++){
                for (ai=0;ai<A.m[mi][0].r[ri].na;ai++){
			if (A.m[mi][0].r[ri].a[ai].R=='Y'){
                        	printf("atom %s in residue %d is in a ring\n",A.m[mi][0].r[ri].a[ai].N,A.m[mi][0].r[ri].n);
			}
		}
	}
}
return 0;
}
