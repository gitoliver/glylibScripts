#include <glylib.h>

int main (int argc, char *argv[]){
if (argc!=2) {
    printf("Usage: createTorsionAtoms.exe input_filename\n");
}
if (argc==2) { // If started correctly
// Pointers to the coords of the three atoms
coord_3D *a,*b,*c;
a=(coord_3D*)calloc(1, sizeof(coord_3D));
b=(coord_3D*)calloc(1, sizeof(coord_3D));
c=(coord_3D*)calloc(1, sizeof(coord_3D));

int ai=0,ri=0;
int resnum=0;
// Load pdb into assembly 
assembly A;
load_pdb(argv[1], &A);

/*
// Fill in pointers to coords of the three atoms
for(ai=0;ai<A.na;ai++){
	if (strcmp(A.a[ai][0].N,argv[2])==0){a=&A.a[ai][0].x;}
	if (strcmp(A.a[ai][0].N,argv[3])==0){b=&A.a[ai][0].x;}
	if (strcmp(A.a[ai][0].N,argv[4])==0){c=&A.a[ai][0].x;}
}
*/
//printf("%s\n",A.m[0][0].r[1].a[ai].N);
for(ri=0;ri<2;ri++){
    for(ai=0;ai<A.m[0][0].r[ri].na;ai++){
    //printf("%s\n",A.m[0][0].r[1].a[ai].N);
	if (strcmp(A.m[0][0].r[ri].a[ai].N,"O5")==0){a=&A.m[0][0].r[ri].a[ai].x;printf("a\n");}
        if (strcmp(A.m[0][0].r[ri].a[ai].N,"C1")==0){b=&A.m[0][0].r[ri].a[ai].x;printf("b\n");}
        if (strcmp(A.m[0][0].r[ri].a[ai].N,"ND2")==0){
            c=&A.m[0][0].r[ri].a[ai].x;
            resnum=A.m[0][0].r[ri].n;
            printf("c\n");
        }
    }
}

//vectormag_3D v34, nv34;
//v34=get_vector_from_coords(*b,*c);
//nv34=normalize_vec(v34);

//dprint_vectormag_3D(&v34);
//dprint_vectormag_3D(&nv34);

//c->i=(c->i+nv34.i*1.0); // set last figure for distance from C atom to plane
//c->j=(c->j+nv34.j*1.0);
//c->k=(c->k+nv34.k*1.0);

//double theta=1.57079633; // angle between BCD. Set to 90.
double theta=2.03; // Roughly 116.33 degrees
double distance=1.52; // bond length to new atom
coord_3D cg,cb,ca,n,od1;

//double phi=0;// Torsion between ABCD. I think it's actually polar coord but no worries.
// phi does not make sense to me. 0 gives 180degrees. pi gives zero. I think there is a bug in the function.

//Petresuc
//Phi=261.0 deg or 4.5553 radians
//Psi=177.3 deg or 3.0945 radians
//ch2=177.6 deg or 3.0997 radians
//ch1=191.6 deg or 3.3441 radians
//pi gives zero. half pi gives 90 as it should but everything else is inverted. So 91 gives 89. Use negative values of pi to get over 180.
double phi=-1.415;
cg=get_cartesian_point_from_internal_coords( *a, *b, *c, theta, phi, distance);
printf("HETATM    1  CG  ASN   %3d    %8.3f%8.3f%8.3f  1.00  0.00\n",resnum,cg.i,cg.j,cg.k);

phi=0.0471;
cb=get_cartesian_point_from_internal_coords( *b, *c, cg, theta, phi, distance);
printf("HETATM    1  CB  ASN   %3d    %8.3f%8.3f%8.3f  1.00  0.00\n",resnum,cb.i,cb.j,cb.k);

phi=0.0419;
ca=get_cartesian_point_from_internal_coords( *c, cg, cb, theta, phi, distance);
printf("HETATM    1  CA  ASN   %3d    %8.3f%8.3f%8.3f  1.00  0.00\n",resnum,ca.i,ca.j,ca.k);

phi=-0.2025;
n=get_cartesian_point_from_internal_coords( cg, cb, ca, theta, phi, distance);
printf("HETATM    1  N   ASN   %3d    %8.3f%8.3f%8.3f  1.00  0.00\n",resnum,n.i,n.j,n.k);

phi=3.1416;
od1=get_cartesian_point_from_internal_coords( ca, cb, cg, theta, phi, distance);
printf("HETATM    1  OD1 ASN   %3d    %8.3f%8.3f%8.3f  1.00  0.00\n",resnum,od1.i,od1.j,od1.k);

//phi=1.5;
//d=get_cartesian_point_from_internal_coords( *a, *b, *c, theta, phi, distance);
//printf("ATOM      2  CB   XXX     0    %8.3f%8.3f%8.3f  1.00  0.00\n",d.i,d.j,d.k);

//phi=4.1887902;
//d=get_cartesian_point_from_internal_coords( *a, *b, *c, theta, phi, distance);
//printf("ATOM      3  CC   XXX     0    %8.3f%8.3f%8.3f  1.00  0.00\n",d.i,d.j,d.k);

}
return 0;
}

