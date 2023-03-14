/**breaks down the pdbqt output file from docking into individual models and reorders the residues into the right order***/
#include <glylib.h>


int main(int argc,char *argv[]){


fileset Fset,Mset,Pset;
fileslurp Fslurp,Pslurp;
molecule m,n;
int i,j=1,k,l;
char *remove;
remove = (char*)calloc(15,sizeof(char));

Fset.N=strdup(argv[1]);
Fset.F=myfopen(Fset.N,"r");
//printf("Fset.N is %s\n",Fset.N);
Fslurp = slurp_file(Fset);

char *filename;
filename=(char*)calloc(13,sizeof(char));


for(i=0;i<Fslurp.n;i++){
        if(strstr(Fslurp.L[i],"MODEL") != NULL){
                sprintf(filename,"model_%d.pdb",j);
		Mset.N=strdup(filename);
		Mset.F=myfopen(Mset.N,"w");
		fprintf(Mset.F,"%s",Fslurp.L[i]);
        }
        if(strstr(Fslurp.L[i],"ATOM") != NULL || strstr(Fslurp.L[i],"HETATM") != NULL){
                //printf("%s\n",Fslurp.L[i]);
                fprintf(Mset.F,"%s",Fslurp.L[i]);

        }
        if(strstr(Fslurp.L[i],"ENDMDL") != NULL){
                fprintf(Mset.F,"%s",Fslurp.L[i]);
		j++;
		fclose(Mset.F);
        }
}

int e=0;

//printf(" j is %d\n",j);
for(i=0;i<j-1;i++){
	sprintf(filename,"model_%d.pdb",i+1);
	Pset.N=strdup(filename);
	Pset.F=myfopen(Pset.N,"r");
	Pslurp=slurp_file(Pset);
	m = load_pdb_from_slurp(Pslurp);

	char *reorder_pdb;
	reorder_pdb=(char*)calloc(11,sizeof(char));
	sprintf(reorder_pdb,"model%d.pdb",i+1);
	n.nr = m.nr;
	//printf("m.nr is %d\n",m.nr);
	n.r=(residue*)calloc(n.nr,sizeof(residue));
	e=0;
	for(k=0;k<n.nr;k++){
		l=m.r[k].n;
		//printf("l is %d\n",l);
		n.r[e].n=m.r[k].n;
		n.r[e].na=m.r[k].na;
		n.r[e].a=(atom*)calloc(n.r[e].na,sizeof(atom));
		n.r[e]=m.r[k];	
		e++;
		//printf("%d\n",m.r[k].n);
	}
	
	//printf("before outputMolPDB\n");
	outputMolPDB(&n,reorder_pdb);
	sprintf(remove,"rm model_%d.pdb",i+1);
	system(remove);

}


fileset Aset;
char *afile;
afile=(char*)calloc(14,sizeof(char));
sprintf(afile,"allmodels.pdb");
Aset.N=strdup(afile);
Aset.F=myfopen(Aset.N,"w");

char *efile;
efile=(char*)calloc(12,sizeof(char));
fileset Eset;
fileslurp Eslurp;
for(i=0;i<j-1;i++){
	sprintf(efile,"model%d.pdb",i+1);
	Eset.N=strdup(efile);
	Eset.F=myfopen(Eset.N,"r");
	Eslurp=slurp_file(Eset);
	for(k=0;k<Eslurp.n;k++){
		fprintf(Aset.F,"%s",Eslurp.L[k]);
		}
	fprintf(Aset.F,"ENDMDL\n");
	fclose(Eset.F);
	free(Eset.N);
}

fclose(Aset.F);

return 0;		
}
