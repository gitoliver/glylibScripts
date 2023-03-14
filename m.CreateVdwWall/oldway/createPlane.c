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

vectormag_3D v34, nv34;
v34=get_vector_from_coords(*b,*c);
nv34=normalize_vec(v34);

//dprint_vectormag_3D(&v34);
//dprint_vectormag_3D(&nv34);

//c->i=(c->i+nv34.i*1.0); // set last figure for distance from C atom to plane
//c->j=(c->j+nv34.j*1.0);
//c->k=(c->k+nv34.k*1.0);

double distance=0.0; // distance from center atom to place current atom
long double theta=90.0*(PI/180); // angle between BCD. Set to 90.
double spacing=5.00; // sets spacing between atoms (Angstrom)
double side=50.00; //  radius? of wall.
int iterations=(side/spacing);
//printf("iterations=%d\n",iterations);
coord_3D d;
long double phi=0.0;// Torsion between ABCD. I think it's actually polar coord but no worries.
long double rad=0.0;
long double interval=60.0; //60 degree intervals.
int i=1,j=2; 
int atom=0;
double dist2=0.0,calc_dist2=0.0;
//double opp=0.0,hyp=0.0,adj=0.0;
long double offset=0.0;
double angle=0.0;
int cnt=0;
for (i=1;i<=iterations;i++){
	printf("i=%d\n",i);
	distance=(distance+spacing);
        phi=0.0;
        while (phi<360) {
                //printf("phi=%Lf,interval=%Lf,iterations=%d,i=%d\n",phi,interval,iterations,i);
                atom++;
                rad=(phi*(PI/180)); //Convert to rads.
                d=get_cartesian_point_from_internal_coords( *a, *b, *c, theta, rad, distance);
                printf("TER\nATOM    %3d  O%-3dWAL     1    %8.3f%8.3f%8.3f  1.00  0.00\n",atom,atom,d.i,d.j,d.k);
	//	printf("X  %12.6f%12.6f%12.6f\n",d.i,d.j,d.k);
                phi=(phi+interval);
		//printf("phi=%Lf\n",phi);
        }
	phi=0.0; //reset
	j=2; // reset
	offset=(interval/i);
	cnt=0; // reset
	for(j=2;j<=i;j++){
		//opp=(distance/i);
		//hyp=distance;
		//adj=sqrt( (hyp*hyp) - (opp*opp) );
		//adj=(sin((60/57.2957795))*distance);
		//printf("distance=%3f,opp=%3f,hyp=%3f,adj=%3f\n",distance,opp,hyp,adj);
		cnt++;
                side=(spacing*cnt);
                dist2=sqrt( (side*side) + (distance*distance) - ( cos((60.0*(PI/180)))*(2*side*distance) ) );
		if(cnt>=60/offset) {cnt=0;}
		phi=(offset*cnt);
		while (phi<360) {
			atom++;
			rad=(phi*(PI/180)); //Convert to rads.
			printf("rad=%Lf\n",(rad*(180/PI)));
	                d=get_cartesian_point_from_internal_coords( *a, *b, *c, theta, rad, dist2);
			calc_dist2=sqrt( (c->i - d.i)*(c->i - d.i) + (c->j - d.j)*(c->j - d.j) + (c->k - d.k)*(c->k - d.k) );
			angle=(180/PI)*(acos( ( (side*side)+(distance*distance)-(dist2*dist2))/(2*side*distance)));
			//printf("phi=%Lf,distance=%3f,dist2=%10f,calc_dist2=%10f,angle=%10f,cnt=%d,side=%f,60.0/offset=%3Lf\n",phi,distance,dist2,calc_dist2,angle,cnt,side,(60.0/offset));
        	        printf("TER\nATOM    %3d  O%-3dWAL     1    %8.3f%8.3f%8.3f  1.00  0.00\n",atom,atom,d.i,d.j,d.k);
	//		printf("O  %12.6f%12.6f%12.6f\n",d.i,d.j,d.k);
                	phi=(phi+interval);
		}
	}
}

			
	

}
return 0;
}

