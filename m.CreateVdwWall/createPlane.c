#include <glylib.h>

/*
 A
  \
   \
    B
    |
    | 90 degrees, D is created atom.
    C----D    


**********************************************
Creates hexagonal surface by making two layers of squares made out of atoms D and E. We are looking down the B--C atom bond from above:
                    D
         E      E       E 

              D     C     D

         E      E       E
                    D

Note: Using this squares method requires "trimming" of the edges. So the code is a little uglier than it could be.
Note: With radii larger than I require, the x,y,z values can become too large (too many digits). Once they touch each other like the y and z: -93.272 112.261-149.392 you have a problem
*/

int main (int argc, char *argv[]){
if (argc!=6) {
    printf("Usage: surfacePlane input_filename, atomAname, atomBname, atomCname, radius\n");
}
if (argc==6) { // If started correctly
// Pointers to the coords of the three atoms
coord_3D *a,*b,*c; 
a=(coord_3D*)calloc(1, sizeof(coord_3D));
b=(coord_3D*)calloc(1, sizeof(coord_3D));
c=(coord_3D*)calloc(1, sizeof(coord_3D));

int ai=0;
// Load pdb into assembly 
assembly A;
load_pdb(argv[1], &A);

// Fill in pointers to coords of the three atoms. Requires atom names to be unique.
for(ai=0;ai<A.na;ai++){
	if (strcmp(A.a[ai][0].N,argv[2])==0){a=&A.a[ai][0].x;}
	if (strcmp(A.a[ai][0].N,argv[3])==0){b=&A.a[ai][0].x;}
	if (strcmp(A.a[ai][0].N,argv[4])==0){c=&A.a[ai][0].x;}
}		

double distance=0.0; // distance from center atom to place current atom
long double theta=90.0*(PI/180); // angle between atoms BCD. Set to 90.
double spacing=5.00; // sets spacing between atoms (Angstrom)
double side=atof(argv[5]); //  radius? of wall.
int jiterations=0,iterations=(side/spacing); // ITERATIONS MUST BE EVEN! 
coord_3D d0,d60,d90;
int i=1,j=1; 
int atom=0, cnt=0;
double h=0.0;
vectormag_3D vhors, vvert;
double x=0,y=0,z=0,xu=0,yu=0,zu=0,xd=0,yd=0,zd=0; // x-up, x-down

distance=distance+spacing;
h=(2*distance)*(sqrt(3)/2);

d0=get_cartesian_point_from_internal_coords( *a, *b, *c, theta, 0, distance); // first point for first layer
d60=get_cartesian_point_from_internal_coords( *a, *b, *c, theta, (60*(PI/180)), distance); // starting point for second layer
d90=get_cartesian_point_from_internal_coords( *a, *b, *c, theta, (90*(PI/180)), h); // how to move horizontally

vvert=get_vector_from_coords(d0,*c); // describes how to get from center to d0.
vhors=get_vector_from_coords(d90,*c); // describes how to get from center to d90.

//printf("nvvert=%f %f %f\n",nvvert.i,nvvert.j,nvvert.k);

// Layer One
// Positive Direction
cnt=0; // Used to create hexagon shape rather than square that would naturally form.
for (i=0;i<=iterations;i++){
    x=(c->i + (vvert.i*i) );
    y=(c->j + (vvert.j*i) );
    z=(c->k + (vvert.k*i) );
    if (i>0){ // Don't print the initial middle one
        atom++;
        printf("TER\nATOM  %6d W   WAL%6d    %8.3f%8.3f%8.3f  1.00  0.00\n",atom,atom,x,y,z);
    }
    if (i>(iterations/2)) cnt++;
    jiterations=((iterations/2)-cnt);
    for (j=1;j<=jiterations;j++){
        xu=(x + (vhors.i*j) );
        yu=(y + (vhors.j*j) );
        zu=(z + (vhors.k*j) );
        xd=(x - (vhors.i*j) );
        yd=(y - (vhors.j*j) );
        zd=(z - (vhors.k*j) );
        atom++;
        printf("TER\nATOM  %6d W   WAL%6d    %8.3f%8.3f%8.3f  1.00  0.00\n",atom,atom,xu,yu,zu);
        atom++;
        printf("TER\nATOM  %6d W   WAL%6d    %8.3f%8.3f%8.3f  1.00  0.00\n",atom,atom,xd,yd,zd);
    }
}
// Negative Direction
cnt=0;
for (i=-1;i>=(iterations*-1);i--){
    x=(c->i + (vvert.i*i) );
    y=(c->j + (vvert.j*i) );
    z=(c->k + (vvert.k*i) );
    atom++;
    printf("TER\nATOM  %6d W   WAL%6d    %8.3f%8.3f%8.3f  1.00  0.00\n",atom,atom,x,y,z);    
    if (i<(iterations/-2)) cnt--;
    jiterations=((iterations/2*-1)-cnt);
    for (j=-1;j>=jiterations;j--){
        xu=(x + (vhors.i*j) );
        yu=(y + (vhors.j*j) );
        zu=(z + (vhors.k*j) );
        xd=(x - (vhors.i*j) );
        yd=(y - (vhors.j*j) );
        zd=(z - (vhors.k*j) );
        atom++;
        printf("TER\nATOM  %6d W   WAL%6d    %8.3f%8.3f%8.3f  1.00  0.00\n",atom,atom,xu,yu,zu);
        atom++;
        printf("TER\nATOM  %6d W   WAL%6d    %8.3f%8.3f%8.3f  1.00  0.00\n",atom,atom,xd,yd,zd);
    }
}

// Layer Two
// The second layer isn't symetrical as it starts from the point d60 instead of the center
// Thus it has +1 or -1 corrections to adding layers that will look confusing. 
// The more symetrical and easy to understand layer is layer one.
cnt=0;
for (i=0;i<=iterations;i++){
    x=(d60.i + (vvert.i*i) );
    y=(d60.j + (vvert.j*i) );
    z=(d60.k + (vvert.k*i) );
    atom++;
    printf("TER\nATOM  %6d W   WAL%6d    %8.3f%8.3f%8.3f  1.00  0.00\n",atom,atom,x,y,z);
    if (i>((iterations/2)+1)) cnt++;
    jiterations=((iterations/2)-cnt);
    for (j=1;j<=jiterations;j++){
        xu=(x + (vhors.i*j) );
        yu=(y + (vhors.j*j) );
        zu=(z + (vhors.k*j) );
        atom++;
        printf("TER\nATOM  %6d W   WAL%6d    %8.3f%8.3f%8.3f  1.00  0.00\n",atom,atom,xu,yu,zu);
    }
    jiterations=((iterations/2)-cnt-1);
    for (j=1;j<=jiterations;j++){
        xd=(x - (vhors.i*j) );
        yd=(y - (vhors.j*j) );
        zd=(z - (vhors.k*j) );
        atom++;
        printf("TER\nATOM  %6d W   WAL%6d    %8.3f%8.3f%8.3f  1.00  0.00\n",atom,atom,xd,yd,zd);
    }
}

cnt=0;
for (i=-1;i>=((iterations*-1)+1);i--){
    x=(d60.i + (vvert.i*i) );
    y=(d60.j + (vvert.j*i) );
    z=(d60.k + (vvert.k*i) );
    atom++;
    printf("TER\nATOM  %6d W   WAL%6d    %8.3f%8.3f%8.3f  1.00  0.00\n",atom,atom,x,y,z);
    if (i<(iterations/-2)) cnt--;
    jiterations=((iterations/2*-1)-cnt+1);
    for (j=-1;j>=jiterations;j--){
        xu=(x + (vhors.i*j) );
        yu=(y + (vhors.j*j) );
        zu=(z + (vhors.k*j) );
        atom++;
        printf("TER\nATOM  %6d W   WAL%6d    %8.3f%8.3f%8.3f  1.00  0.00\n",atom,atom,xu,yu,zu);
    }
    jiterations=((iterations/2*-1)-cnt);
    for (j=-1;j>=jiterations;j--){
        xd=(x - (vhors.i*j) );
        yd=(y - (vhors.j*j) );
        zd=(z - (vhors.k*j) );
        atom++;
        printf("TER\nATOM  %6d W   WAL%6d    %8.3f%8.3f%8.3f  1.00  0.00\n",atom,atom,xd,yd,zd);
    }
}

	
}
return 0;
}

