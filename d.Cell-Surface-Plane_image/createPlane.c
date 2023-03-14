#include <glylib.h>

/* Creates 3 atoms on a plane that is 90 degrees from the B-C atom bond with C on the plane. 
 * For use in chimera to draw planes
*/
int main (int argc, char *argv[]){
if (argc!=5) {
    printf("Usage: surfacePlane input_filename, atomAname, atomBname, atomCname\n");
}
if (argc==5) { // If started correctly
// Pointers to the coords of the three atoms
coord_3D *a,*b,*c;
a=(coord_3D*)calloc(1, sizeof(coord_3D));
b=(coord_3D*)calloc(1, sizeof(coord_3D));
c=(coord_3D*)calloc(1, sizeof(coord_3D));

int ai=0;
// Load pdb into assembly 
assembly A;
load_pdb(argv[1], &A);

// Fill in pointers to coords of the three atoms
for(ai=0;ai<A.na;ai++){
	if (strcmp(A.a[ai][0].N,argv[2])==0){a=&A.a[ai][0].x;}
	if (strcmp(A.a[ai][0].N,argv[3])==0){b=&A.a[ai][0].x;}
	if (strcmp(A.a[ai][0].N,argv[4])==0){c=&A.a[ai][0].x;}
}		

//vectormag_3D v34, nv34;
//v34=get_vector_from_coords(*b,*c);
//nv34=normalize_vec(v34);

//dprint_vectormag_3D(&v34);
//dprint_vectormag_3D(&nv34);

//c->i=(c->i+nv34.i*1.0); // set last figure for distance from C atom to plane
//c->j=(c->j+nv34.j*1.0);
//c->k=(c->k+nv34.k*1.0);

double theta=1.57079633; // angle between BCD. Set to 90.
double distance=1; // set to any distance
coord_3D d;

double phi=0;// Torsion between ABCD. I think it's actually polar coord but no worries.
d=get_cartesian_point_from_internal_coords( *a, *b, *c, theta, phi, distance);
printf("ATOM      1  H   XXX     0    %8.3f%8.3f%8.3f  1.00  0.00\n",d.i,d.j,d.k);

phi=2.0943951;
d=get_cartesian_point_from_internal_coords( *a, *b, *c, theta, phi, distance);
printf("ATOM      2  H   XXX     0    %8.3f%8.3f%8.3f  1.00  0.00\n",d.i,d.j,d.k);

phi=4.1887902;
d=get_cartesian_point_from_internal_coords( *a, *b, *c, theta, phi, distance);
printf("ATOM      3  H   XXX     0    %8.3f%8.3f%8.3f  1.00  0.00\n",d.i,d.j,d.k);

}
return 0;
}

