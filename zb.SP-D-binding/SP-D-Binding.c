#include <glylib.h>

int main (int argc, char *argv[]){
if (argc!=2) {
 printf("Usage: spd-binding.exe snapshots/\n");
}
if (argc==2) { // If started correctly
 int i=0,j=0,l=0;
 assembly A; // Code expects file will contain only C1 atoms from residue 0MA. Grep is your friend.

 // Expects a directory of 0MA@C1-containing files from a simulation
 char *snapshot_dir; // Directory containing snapshots of C1 atoms from a simulation.
 snapshot_dir=(char*)calloc(500,sizeof(char));
 sprintf(snapshot_dir, "%s", argv[1]);
 struct dirent **namelist;
 char (*d_list)[80];
 l = scandir (snapshot_dir, &namelist, NULL, alphasort);
 d_list = malloc(l * sizeof *d_list);
 int num_snapshots=l;
 char path[1000]; 

 if (l < 0) {
     perror("scandir"); // error checking; to be done infrequently and inconsistently.
 }
 else {
     while (l--) {
         printf("%s\n", namelist[l]->d_name);
         sprintf(d_list[l], "%s", namelist[l]->d_name);
         free(namelist[l]);
     }
     free(namelist);
 }
 
// printf("num_snapshots=%d\n",num_snapshots);
 // Now we have a list of files to analyze, on to the actual program
 /*****************************************/
 int ri=0,rii=0;
 double x=0.0, y=0.0, z=0.0;
 double distance=0.0;
 int list[1000];
 list[0]=0;
 for (l=2; l < (num_snapshots); l++){
     sprintf(path, "./%s/%s" , snapshot_dir, d_list[l]);
 
     load_pdb(path, &A);
     atom *atom1, *atom2;
     atom1 = (atom *)calloc(1, sizeof(atom));
     atom2 = (atom *)calloc(1, sizeof(atom));
     // For each 0MA residue
     printf("\n\nChecking %s\n", path);
     for (ri=0; ri<A.m[0][0].nr; ri++){ // assuming only one "molecule" is read in!
//         printf("Checking residue %d against:\n",A.m[0][0].r[ri].n);
         atom1 = &A.m[0][0].r[ri].a[0];
         for (rii=(ri+1); rii<A.m[0][0].nr; rii++){ // ri+1 as don't need to check same pair twice
//             printf("    residue %d:",A.m[0][0].r[rii].n);
             atom2 = &A.m[0][0].r[rii].a[0];
             x = ( (*atom1).x.i - (*atom2).x.i );
             y = ( (*atom1).x.j - (*atom2).x.j );
             z = ( (*atom1).x.k - (*atom2).x.k );
             distance = sqrt((x*x)+(y*y)+(z*z));
//           printf("distance=%.1f\n",distance);
             if ( distance >= 43 && distance <= 47 ){
//                 printf("distance=%.1f,success\n",distance);
                 list[0]+=1;
                 list[list[0]] = rii;
             }
//             else {printf("distance=%.1f,nope\n",distance);}
         }   
         // Now check if any members of list are ~45 from each other
         if ( list[0] > 0 ) {
             for (i=1; i<list[0]; i++) {
//                 printf("    For residue %d, checking residue %d against:\n",A.m[0][0].r[ri].n ,A.m[0][0].r[ list[i] ].n);
                 for (j=(i+1); j<=list[0]; j++){
//                     printf("        residue %d, ",A.m[0][0].r[ list[j] ].n);
                     x = (A.m[0][0].r[ list[i] ].a[0].x.i - A.m[0][0].r[ list[j] ].a[0].x.i);
                     y = (A.m[0][0].r[ list[i] ].a[0].x.j - A.m[0][0].r[ list[j] ].a[0].x.j);
                     z = (A.m[0][0].r[ list[i] ].a[0].x.k - A.m[0][0].r[ list[j] ].a[0].x.k);                
                     distance = sqrt((x*x)+(y*y)+(z*z));
                     if ( distance >= 43 && distance <= 47 ){
                         //ric = A.m[0][0].r[j].a[0].mb[0].s.r;
                         printf("Residues (%d,%d,%d) are ~45 Angstrom apart\n",A.m[0][0].r[ri].n, A.m[0][0].r[ list[i]  ].n, A.m[0][0].r[ list[j] ].n);
                     }
//                     else {printf("distance=%.1f,no joy\n",distance);}
                 }
             }    
         }
     list[0]=0; // Reset list for each iteration of the ri loop. I.e. each residue builds up a unique list of partners
     }
 printf("\n\n");
 }
}
return 0;
}
