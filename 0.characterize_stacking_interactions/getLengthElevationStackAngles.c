/*written by Oliver Grant 08/Dec/2010

Function that characterizes stacking interactions between sugars(not Neu5Ac or any sugars with ring atoms that are not C1-C5 and O5) (Sigh) and aromatic residues.
If protein residue is trp like use cd2 as origin. Ce2 as x-axis and ct3 as y axis.
Finds r, θ, ϕ. r is distance between centroid and cd2. Also use r as a vector. θ is angle between r and Z axis. ϕ is angle between r and X axis. 

Proteins: Structure, Function, and Bioinformatics Volume 55, Issue 1 17 feb 2004 55:44-65 Identification of Common Structural Features of Binding Sites in Galactose-Specific Proteins

*/
#include <glylib.h>
//function prototype
void dXprint_coord_3D(coord_3D *c);
double get_angle_two_vectors(vectormag_3D a, vectormag_3D b);
double get_distance_AB_points(coord_3D a, coord_3D b);

main( int argc, char *argv[])  {
int mi=0,ri=0,ai=0;
assembly A;
coord_3D *x_list_s, ol2, cl5, centroid_sug, normalpt;

if( argc == 3 ){
char filename[100];
strcpy(filename, argv[1]);
load_pdb(filename,&A);

//get name of sugar residue and the 3 atoms that make up the plane in the protein residue
int sug=atoi(argv[2]);
//int i=0;//counter 
double r;

x_list_s=(coord_3D*)calloc(5, sizeof(coord_3D)); //creates a list to store co-ord
//x_list=(coord_3D**)calloc(6, sizeof(coord_3D*)); //pointer to address of coord. coord value can change and pointer will"update" by still pointing to address

//for res1 get coords and store in x_list
//printf("sug=%d\n",sug);
for(mi=0;mi<A.nm;mi++){
	for(ri=0;ri<A.m[mi][0].nr;ri++){
		if(A.m[mi][0].r[ri].n==sug){
			for(ai=0;ai<A.m[mi][0].r[ri].na;ai++){
				if(strcmp(A.m[mi][0].r[ri].a[ai].N,"C1")==0){
					x_list_s[0]=A.m[mi][0].r[ri].a[ai].x;}
                                if(strcmp(A.m[mi][0].r[ri].a[ai].N,"C3")==0){
                              		x_list_s[1]=A.m[mi][0].r[ri].a[ai].x;}
  		                if(strcmp(A.m[mi][0].r[ri].a[ai].N,"C5")==0){
                		        x_list_s[2]=A.m[mi][0].r[ri].a[ai].x;}
                               	if(strcmp(A.m[mi][0].r[ri].a[ai].N,"C4")==0){
                              		x_list_s[3]=A.m[mi][0].r[ri].a[ai].x;}
 		                if(strcmp(A.m[mi][0].r[ri].a[ai].N,"O5")==0){
                	                x_list_s[4]=A.m[mi][0].r[ri].a[ai].x;}
                               	if(strcmp(A.m[mi][0].r[ri].a[ai].N,"C2")==0){
                                        x_list_s[5]=A.m[mi][0].r[ri].a[ai].x;}
			}
		}
	}
}
// NEED TO GET CENTROID RATHER THAN PLANE


//P1=get_plane_for_ring(6,x_list);
centroid_sug=get_geometric_center(x_list_s,3);

for(mi=0;mi<A.nm;mi++){
        for(ri=0;ri<A.m[mi][0].nr;ri++){
                if(strcmp(A.m[mi][0].r[ri].N,"LNK")==0){ // if residue number matches
                        for(ai=0;ai<A.m[mi][0].r[ri].na;ai++){
                                if(strcmp(A.m[mi][0].r[ri].a[ai].N,"OL2")==0){
                                        ol2=A.m[mi][0].r[ri].a[ai].x;
                                //        printf("OL2\n");
                                //        dXprint_coord_3D(&ol2);
				}
                                if(strcmp(A.m[mi][0].r[ri].a[ai].N,"CL5")==0){
					cl5=A.m[mi][0].r[ri].a[ai].x;
                                //        printf("CL5\n");
                                //        dXprint_coord_3D(&cl5);
				}
                        }
                }
        }
}

//Calculate "effective length" of spacer as distance from OL2 to C1 of glycan
r=get_distance_AB_points(ol2,x_list_s[0]);
printf("length=%.3f\n",r);


/*  Cn                  Cn=Centroid of sugar ring
  / | \                 PBC=Point Below Centroid, on the array surface
 n  |  \                n=point that is normal to the sugar ring plane
 opp|   \hyp            pt=Point we need to get vpo vector, so we can get to PBC
    |    \
  PBC----OL2----pt
     adj  |  <--vpo
         CL5
*/

// Need to get a vector which is at a right angle to the a1----a3 (v13) bond. Get the point pt on the line defined by the v13 vector such 
// that the vector between it and the a4 atom will be perpendicular to the v13 bond. See the image above.

// Calculate position of pt in above figure.
double t=1.57079633; // angle between CL5, OL2, and pt. Set to 90.
double distance=1; // set 1 so vpo vector will be normal.
coord_3D pt,pbc;

double p=0;// Torsion between Cn, CL5, OL2, and pt. I think it's actually polar coord but no worries.
pt=get_cartesian_point_from_internal_coords( centroid_sug, cl5, ol2, t, p, distance);

vectormag_3D v1,v2,v3,hyp,vpo,normal;
//Get normal from plane of C1,C3,C5 atoms
v1=get_vector_from_coords(x_list_s[0],x_list_s[1]);// C1 to C3
v2=get_vector_from_coords(x_list_s[0],x_list_s[2]);// C1 to C5
v3=get_crossprod(v2,v1);
normal=normalize_vec(v3);
normalpt.i=centroid_sug.i+(normal.i*2);normalpt.j=centroid_sug.j+(normal.j*2);normalpt.k=centroid_sug.k+(normal.k*2);

//Get get angle between sugar plane and array surface (same as angle between normal and opp in figure)
vpo=get_vector_from_coords(ol2,pt);
hyp=get_vector_from_coords(ol2,centroid_sug);

double ele=(get_angle_two_vectors(hyp,vpo));
ele=(PI - ele);
printf("ele=%.3f\n",(ele*57.2957795));
double adj=((cos(ele)) * hyp.d);
//printf("adj=%.3f\n",adj);
pbc.i=(ol2.i - (vpo.i * adj));
pbc.j=(ol2.j - (vpo.j * adj));
pbc.k=(ol2.k - (vpo.k * adj));
//*
printf("ATOM      1  N   pt      0    %8.3f%8.3f%8.3f  1.00  0.00\n",pt.i,pt.j,pt.k);
printf("ATOM      1  N   PBC     0    %8.3f%8.3f%8.3f  1.00  0.00\nTER\n",pbc.i,pbc.j,pbc.k);
printf("ATOM      1  N   CEN     0    %8.3f%8.3f%8.3f  1.00  0.00\nTER\n",centroid_sug.i,centroid_sug.j,centroid_sug.k);
printf("ATOM      2  N   NRM     0    %8.3f%8.3f%8.3f  1.00  0.00\nTER\n",normalpt.i,normalpt.j,normalpt.k);
//*/
vectormag_3D vopp,vnrm;
vopp=get_vector_from_coords(centroid_sug,pbc);
vnrm=get_vector_from_coords(centroid_sug,normalpt);
double stack=get_angle_two_vectors(vopp,vnrm);
printf("stack=%.1f\n",(stack*57.2957795));
printf("height=%.1f\n",vopp.d);

/*
// TORSION // gimbal locks so no point :(
vectormag_3D n1,n2,m1,b2;
double x,y,tors;
// v4 is plane normal vector:b3
// v6 is CL4-OL2:b1
// v3 is OL2-centroid:b2
n1=get_crossprod(v6,v3);
n1=normalize_vec(n1);
n2=get_crossprod(v3,v4);
n2=normalize_vec(n2);

b2.i=(v3.i/v3.d);b2.j=(v3.j/v3.d);b2.k=(v3.k/v3.d);b2.d=1; // unit vector of v3 

m1=get_crossprod(n1,b2);

x=get_dotprod(n1,n2);
y=get_dotprod(m1,n2);

tors=(atan2(y,x)*57.2957795);


printf("tors=%f\ntheta=%f\nphi=%f\nr=%f\n",tors,theta,phi,r);
//d is the distance from the plane of the array to the ring
//r is the distance from OL2 to the ring
*/
}
else { printf("Usage: \"program.exe pdbFile.pdb sugarResid\"\n");}
return 0;
}

// not used in this code:
void rotate_vector_to_X_list(coord_3D* XS,int n,vectormag_3D v){
int ra=0;
vectormag_3D cX,cY,cZ; // direction cosines for X Y and Z
coord_3D RC; // temporary coords
 
// If the requested direction already points along x
if((v.k==0)&&(v.j==0)){ return; }
 
// get direction cosines for unit vector along new X axis (the rotation axis)
cX=normalize_vec(v);
if(cX.d==0){mywhine("zero-length vector passed to rotate_vector_to_X (can't rotate to that)");}

if(cX.j!=0){ // if the j component of the target direction isn't zero
        // set abitrary point in new XY plane for positive Y position
        cY.i=cX.k; // set xY=iZ
        cY.i=cX.i; // set zY=kZ
        cY.j=-(cX.i*cX.i+cX.k*cX.k)/cX.j;  ; // solve for yY in X-Y plane
        // turn that into a unit vector (make into direction cosines)
        cY=normalize_vec(cY); 
        // get direction cosines for unit vector along new X axis
        // (this is the X-prod of the new Y and Z axes)
        cZ=get_crossprod(cY,cX); // note ordering looks backwards...
        }
else{ // the i component isn't zero (or we would have exited)
        // set abitrary point in new XY plane for positive X position
        cZ.j=cX.j; // set yX=jZ, which is zero or we wouldn't be here...
        cZ.i=cX.i; // set zX=kZ
        cZ.k=-(cX.i*cX.i)/cX.k;  ; // solve for yY in X-Y plane
        // turn that into a unit vector (make into direction cosines)
        cZ=normalize_vec(cZ); 
        // get direction cosines for unit vector along new X axis
        // (this is the X-prod of the new Y and Z axes)
        cY=get_crossprod(cX,cZ);
        }

// rotate molecule 
for(ra=0;ra<n;ra++){ // for each residue
         RC.i=cX.i*XS[ra].i + cX.j*XS[ra].j + cX.k*XS[ra].k;
         RC.j=cY.i*XS[ra].i + cY.j*XS[ra].j + cY.k*XS[ra].k;
         RC.k=cZ.i*XS[ra].i + cZ.j*XS[ra].j + cZ.k*XS[ra].k;
         XS[ra]=RC;
        }

return;
}

double get_angle_two_vectors(vectormag_3D a, vectormag_3D b)
{
       return acos(get_dotprod(normalize_vec(a),normalize_vec(b)));
}


double get_distance_AB_points(coord_3D a, coord_3D b)
{ 
	return sqrt(pow(b.i-a.i,2) + pow(b.j-a.j,2) + pow(b.k-a.k,2)); 
}


void dXprint_coord_3D(coord_3D *c){
     printf("coord i j k \t%20.12f \t%20.12f \t%20.12f\n",c[0].i,c[0].j,c[0].k);
return;
}



