#include <glylib.h>

int main (int argc, char *argv[]){
if (argc!=3) {
    printf("Usage: centreOfMass.exe input_filename resid\n");
}
if (argc==3) { // If started correctly
// Pointers to the coords of the three atoms
coord_3D *a,*b,*c,*d,*e,*f;
a=(coord_3D*)calloc(1, sizeof(coord_3D));
b=(coord_3D*)calloc(1, sizeof(coord_3D));
c=(coord_3D*)calloc(1, sizeof(coord_3D));
d=(coord_3D*)calloc(1, sizeof(coord_3D));
e=(coord_3D*)calloc(1, sizeof(coord_3D));
f=(coord_3D*)calloc(1, sizeof(coord_3D));

int ai=0,ri=0;
// Load pdb into assembly 
assembly A;
load_pdb(argv[1], &A);
int resid=(*argv[2]-'0');
/*
// Fill in pointers to coords of the three atoms
for(ai=0;ai<A.na;ai++){
	if (strcmp(A.a[ai][0].N,argv[2])==0){a=&A.a[ai][0].x;}
	if (strcmp(A.a[ai][0].N,argv[3])==0){b=&A.a[ai][0].x;}
	if (strcmp(A.a[ai][0].N,argv[4])==0){c=&A.a[ai][0].x;}
}
*/
//printf("%s\n",A.m[0][0].r[1].a[ai].N);
printf("resid=%d\nna=A.m[0][0].r[%d].na=%d\n",resid,resid,A.m[0][0].r[resid].na);
    for(ai=0;ai<A.m[0][0].r[resid].na;ai++){
    printf("%s\n",A.m[0][0].r[resid].a[ai].N);
        if (strcmp(A.m[0][0].r[resid].a[ai].N,"C1")==0){a=&A.m[0][0].r[resid].a[ai].x;}
        if (strcmp(A.m[0][0].r[resid].a[ai].N,"C2")==0){b=&A.m[0][0].r[resid].a[ai].x;}
        if (strcmp(A.m[0][0].r[resid].a[ai].N,"C3")==0){c=&A.m[0][0].r[resid].a[ai].x;}
        if (strcmp(A.m[0][0].r[resid].a[ai].N,"C4")==0){d=&A.m[0][0].r[resid].a[ai].x;}
        if (strcmp(A.m[0][0].r[resid].a[ai].N,"C5")==0){e=&A.m[0][0].r[resid].a[ai].x;}
        if (strcmp(A.m[0][0].r[resid].a[ai].N,"O5")==0){f=&A.m[0][0].r[resid].a[ai].x;}
    }

double avrx=((*a).i + (*b).i + (*c).i + (*d).i + (*e).i + (*f).i)/6;
double avry=((*a).j + (*b).j + (*c).j + (*d).j + (*e).j + (*f).j)/6;
double avrz=((*a).k + (*b).k + (*c).k + (*d).k + (*e).k + (*f).k)/6;

printf("%f\n,%f\n,%f\n,%f\n,%f\n,%f\n",(*a).i , (*b).i , (*c).i , (*d).i , (*e).i , (*f).i);


//vectormag_3D v34, nv34;
//v34=get_vector_from_coords(*b,*c);
//nv34=normalize_vec(v34);

//dprint_vectormag_3D(&v34);
//dprint_vectormag_3D(&nv34);

//c->i=(c->i+nv34.i*1.0); // set last figure for distance from C atom to plane
//c->j=(c->j+nv34.j*1.0);
//c->k=(c->k+nv34.k*1.0);

//double theta=1.57079633; // angle between BCD. Set to 90.
//double phi=0;// Torsion between ABCD. I think it's actually polar coord but no worries.
printf("HETATM    1  CB  OME     1    %8.3f%8.3f%8.3f  1.00  0.00\n",avrx,avry,avrz);

//phi=0;
//d=get_cartesian_point_from_internal_coords( *a, *b, *c, theta, phi, distance);
//printf("ATOM      2  H   XXX     0    %8.3f%8.3f%8.3f  1.00  0.00\n",d.i,d.j,d.k);

//phi=4.1887902;
//d=get_cartesian_point_from_internal_coords( *a, *b, *c, theta, phi, distance);
//printf("ATOM      3  H   XXX     0    %8.3f%8.3f%8.3f  1.00  0.00\n",d.i,d.j,d.k);

}
return 0;
}

