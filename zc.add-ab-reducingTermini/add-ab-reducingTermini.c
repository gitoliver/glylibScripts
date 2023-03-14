#include <glylib.h>

int EndsWith(const char *str, const char *suffix);

int main (int argc, char *argv[]){
if (argc!=3) {
    printf("Usage: add-ab-reducingTermini.exe <input_filename> <residue_number>\n");
}

if (argc==3) { // If started correctly
// Pointers to the coords of the three atoms
coord_3D *a,*b,*c;
a=(coord_3D*)calloc(1, sizeof(coord_3D));
b=(coord_3D*)calloc(1, sizeof(coord_3D));
c=(coord_3D*)calloc(1, sizeof(coord_3D));

int ai=0,ri=0;
// Load pdb into assembly 
assembly A;
load_pdb(argv[1], &A);
char atomName[3];
strcpy (atomName, "O1");
int resNum = atoi(argv[2]);
int resid = 0;
char **resNamePtr;

//printf("Program must now print a,b,c or there is something wrong with your input pdb file\n");
//printf("A.m[0][0].nr=%d\n",A.m[0][0].nr);
for( ri=0; ri < A.m[0][0].nr; ri++ ){
  //  printf("Checking %s-%d\n",A.m[0][0].r[ri].N,A.m[0][0].r[ri].n);
    if( A.m[0][0].r[ri].n == resNum ){
        resid = ri; // save resid for renaming later
        resNamePtr = &A.m[0][0].r[ri].N; 
        if ( ( EndsWith( A.m[0][0].r[ri].N, "SX" ) ) || ( EndsWith( A.m[0][0].r[ri].N, "IA" ) ) ) {
            strcpy (atomName, "O2");
            for( ai = 0; ai < A.m[0][0].r[ri].na; ai++ ){
                if( strcmp( A.m[0][0].r[ri].a[ai].N, "C6" ) == 0 ) { a = &A.m[0][0].r[ri].a[ai].x; /*printf( "a, " );*/ }
                if( strcmp( A.m[0][0].r[ri].a[ai].N, "O6" ) == 0 ) { b = &A.m[0][0].r[ri].a[ai].x; /*printf( "b, " );*/ }
                if( strcmp( A.m[0][0].r[ri].a[ai].N, "C2" ) == 0 ) { c = &A.m[0][0].r[ri].a[ai].x; /*printf( "c, " );*/ }
            }
        }
        else {
            for( ai = 0; ai < A.m[0][0].r[ri].na; ai++ ){
	        if( strcmp( A.m[0][0].r[ri].a[ai].N, "C5" ) == 0 ) { a = &A.m[0][0].r[ri].a[ai].x; /*printf( "a\n" );*/ }
                if( strcmp( A.m[0][0].r[ri].a[ai].N, "O5" ) == 0 ) { b = &A.m[0][0].r[ri].a[ai].x; /*printf( "b\n" );*/ }
                if( strcmp( A.m[0][0].r[ri].a[ai].N, "C1" ) == 0 ) { c = &A.m[0][0].r[ri].a[ai].x; /*printf( "c\n" );*/ }        
            }
        }
    }
}
//printf("Mark1\n");
double theta=1.95; // Roughly 112 degrees
double distance=1.47; // bond length to new atom
coord_3D o1;

//pi gives zero. half pi gives 90 as it should but everything else is inverted. So 91 gives 89. Use negative values of pi to get over 180.
//In each case to get value (in degrees) to input into function, first subtract 180 and then change the sign. Convert that value to radians.

//beta
FILE * fp;
double phi=0.0;
o1=get_cartesian_point_from_internal_coords( *a, *b, *c, theta, phi, distance);
fp = fopen ("beta.pdb", "w+");
A.m[0][0].r[resid].N[2]='B'; // rename residue name for beta. So 0SX becomes 0SB.
fprintf(fp, "HETATM    1  %s  %3s   %3d    %8.3f%8.3f%8.3f  1.00  0.00\n\n",atomName,*resNamePtr,(resNum),o1.i,o1.j,o1.k);   
fclose(fp);
//printf("HETATM    1  %s  ROH   %3d    %8.3f%8.3f%8.3f  1.00  0.00\n",atomName,resNum,o1.i,o1.j,o1.k);

//alpha
phi=2.09;
o1=get_cartesian_point_from_internal_coords( *a, *b, *c, theta, phi, distance);
fp = fopen ("alpha.pdb", "w+");
A.m[0][0].r[resid].N[2]='A'; // Rename residue name for alpha. So 0SX become 0SA.
fprintf(fp, "HETATM    1  %s  %3s   %3d    %8.3f%8.3f%8.3f  1.00  0.00\n\n",atomName,*resNamePtr,(resNum),o1.i,o1.j,o1.k); // \n\n as load_pdb can't handle a single line input.
fclose(fp);
// printf("HETATM    1  %s  ROH   %3d    %8.3f%8.3f%8.3f  1.00  0.00\n",atomName,resNum,o1.i,o1.j,o1.k); 

//Now create alpha and beta versions of the input file
int length = strlen(argv[1]);
char *outName;
length += 6; //allow space for alpha- or beta- prefix
//printf ("M2\n");
outName = (char *)malloc(length * sizeof(char));
//printf ("M3\n");
assembly T; // temp assembly

load_pdb( "alpha.pdb", &T );
//printf("Mark3\n");
A.m[0][0].r[resid].N[2]='A'; // Rename residue name for alpha. So 0SX become 0SA.
sprintf( outName, "%s%s", "alpha-", argv[1] );
mergeOutputAsmbly( &T, &A, outName ); // add A onto T


load_pdb( "beta.pdb", &T);
A.m[0][0].r[resid].N[2]='B'; 
sprintf (outName, "%s%s", "beta-", argv[1]);
mergeOutputAsmbly( &T, &A, outName); // add A onto T


}
return 0;
}

int EndsWith(const char *str, const char *suffix)
{
    if (!str || !suffix)
        return 0;
    size_t lenstr = strlen(str);
    size_t lensuffix = strlen(suffix);
    if (lensuffix >  lenstr)
        return 0;
    return strncmp(str + lenstr - lensuffix, suffix, lensuffix) == 0;
}

