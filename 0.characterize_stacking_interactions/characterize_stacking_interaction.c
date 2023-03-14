/*written by Oliver Grant 08/Dec/2010

Function that characterizes stacking interactions between sugars(not Neu5Ac or any sugars with ring atoms that are not C1-C5 and O5) (Sigh) and aromatic residues.
If protein residue is trp like use cd2 as origin. Ce2 as x-axis and ct3 as y axis.
Finds r, θ, ϕ. r is distance between centroid and cd2. Also use r as a vector. θ is angle between r and Z axis. ϕ is angle between r and X axis. 

Proteins: Structure, Function, and Bioinformatics Volume 55, Issue 1 17 feb 2004 55:44-65 Identification of Common Structural Features of Binding Sites in Galactose-Specific Proteins

*/
#include <glylib.h>
//function prototype
void dXprint_coord_3D(coord_3D *c);
void get_residue_numbers(int*res1, int*res2);
double get_angle_two_vectors(vectormag_3D a, vectormag_3D b);
double get_distance_AB_points(coord_3D a, coord_3D b);

int main(){
int nc=6,mi=0,ri=0,ai=0;
assembly A;
coord_3D *x_list_s, *x_list, centroid, centroid_trp;
fileset IF, IC;

IF.N=strdup("input_prmtop.top");
IF.F=myfopen(IF.N,"r");

IC.N=strdup("input_crd.crd");
IC.F=myfopen(IC.N,"r");

//get name of sugar residue and the 3 atoms that make up the plane in the protein residue
int res1=0,res2=0;
//int i=0;//counter 
double theta, r, phi;

// load prmtop file that is in the directory
A=load_amber_prmtop(IF);
// load trajcrds into assembly
add_trajcrds_to_prmtop_assembly(IC,&A,'r',0);


// get numbers of the two residues and atom names (if protein) from user
get_residue_numbers(&res1,&res2);

x_list_s=(coord_3D*)calloc(nc, sizeof(coord_3D)); //creates a list to store co-ord
//x_list=(coord_3D**)calloc(6, sizeof(coord_3D*)); //pointer to address of coord. coord value can change and pointer will"update" by still pointing to address
x_list=(coord_3D*)calloc(6, sizeof(coord_3D));

//for res1 get coords and store in x_list
for(mi=0;mi<A.nm;mi++){
	for(ri=0;ri<A.m[mi][0].nr;ri++){
		if(A.m[mi][0].r[ri].n==res1){
			for(ai=0;ai<A.m[mi][0].r[ri].na;ai++){
				if(strcmp(A.m[mi][0].r[ri].a[ai].N,"C1")==0){
					x_list_s[0]=A.m[mi][0].r[ri].a[ai].xa[0];}
                                if(strcmp(A.m[mi][0].r[ri].a[ai].N,"C2")==0){
                              		x_list_s[1]=A.m[mi][0].r[ri].a[ai].xa[0];}
  		                if(strcmp(A.m[mi][0].r[ri].a[ai].N,"C3")==0){
                		        x_list_s[2]=A.m[mi][0].r[ri].a[ai].xa[0];}
                               	if(strcmp(A.m[mi][0].r[ri].a[ai].N,"C4")==0){
                              		x_list_s[3]=A.m[mi][0].r[ri].a[ai].xa[0];}
 		                if(strcmp(A.m[mi][0].r[ri].a[ai].N,"C5")==0){
                	                x_list_s[4]=A.m[mi][0].r[ri].a[ai].xa[0];}
                               	if(strcmp(A.m[mi][0].r[ri].a[ai].N,"O5")==0){
                               		x_list_s[5]=A.m[mi][0].r[ri].a[ai].xa[0];}
			}
		}
	}
}
// NEED TO GET CENTROID RATHER THAN PLANE


//P1=get_plane_for_ring(6,x_list);
centroid=get_geometric_center(x_list_s,6);

for(mi=0;mi<A.nm;mi++){
        for(ri=0;ri<A.m[mi][0].nr;ri++){
                if(A.m[mi][0].r[ri].n==res2){ // if residue number matches
                        for(ai=0;ai<A.m[mi][0].r[ri].na;ai++){
                                if(strcmp(A.m[mi][0].r[ri].a[ai].N,"CD2")==0){
                                        x_list[0]=A.m[mi][0].r[ri].a[ai].xa[0];
                                        printf("CD2");
                                        dXprint_coord_3D(&x_list[0]);}
                                if(strcmp(A.m[mi][0].r[ri].a[ai].N,"CE2")==0){
                                        x_list[1]=A.m[mi][0].r[ri].a[ai].xa[0];
                                        printf("CE2");
                                        dXprint_coord_3D(&x_list[1]);}
                                if(strcmp(A.m[mi][0].r[ri].a[ai].N,"CZ3")==0){
                                        x_list[2]=A.m[mi][0].r[ri].a[ai].xa[0];
                                        printf("CZ3");
                                        dXprint_coord_3D(&x_list[2]);}
                                if(strcmp(A.m[mi][0].r[ri].a[ai].N,"CZ2")==0){
                                        x_list[3]=A.m[mi][0].r[ri].a[ai].xa[0];
                                        printf("CZ2");
                                        dXprint_coord_3D(&x_list[3]);}
                                if(strcmp(A.m[mi][0].r[ri].a[ai].N,"CH2")==0){
                                        x_list[4]=A.m[mi][0].r[ri].a[ai].xa[0];
                                        printf("CH2");
                                        dXprint_coord_3D(&x_list[4]);}
                                if(strcmp(A.m[mi][0].r[ri].a[ai].N,"CE3")==0){
                                        x_list[5]=A.m[mi][0].r[ri].a[ai].xa[0];
                                        printf("CE3");
                                        dXprint_coord_3D(&x_list[5]);}
                        }
                }
        }
}




centroid_trp=get_geometric_center(x_list,6);
printf("centroid_trp.x=%f,centroid_trp.y=%f,centroid_trp.z=%f\n",centroid_trp.i,centroid_trp.j,centroid_trp.k);


//P2=get_plane(x_list[0],x_list[1],x_list[2]); //when res2 is a protein

// NEED XLIST TO CONTAIN CD2 AT 0, CE2 AT 1 AND CT3 AT 4


/*
double x,y,z;
x=(*x_list[0]).i;
y=(*x_list[0]).j;
z=(*x_list[0]).k;
printf("CD2 xyz is: i=%f,j=%f,k=%f\n",x,y,z); 


// translate so CD2 is on origin
for(mi=0;mi<A.nm;mi++){
        for (ri=0;ri<A.m[mi][0].nr;ri++){
                for(ai=0;ai<A.m[mi][0].r[ri].na;ai++){
                        A.m[mi][0].r[ri].a[ai].x.i=((A.m[mi][0].r[ri].a[ai].x.i) - x);
                        A.m[mi][0].r[ri].a[ai].x.j=((A.m[mi][0].r[ri].a[ai].x.j) - y);
                        A.m[mi][0].r[ri].a[ai].x.k=((A.m[mi][0].r[ri].a[ai].x.k) - z);
                }
        }
}
*/

// THIS JUST DOESN'T WORK!

/*  Cn                  Cn=Centroid of sugar ring
  / | \                 PBC=Point Below Centroid, on the array surface
 n  |  \                n=point that is normal to the sugar ring plane
 opp|   \hyp            pt0=Point we need to get V13, so we can get to PBC
    |    \
  PBC----OL2----pt0
     adj  |  V13
         CL5
*/

// Need to get a vector which is at a right angle to the a1----a3 (v13) bond. Get the point pt on the line defined by the v13 vector such 
// that the vector between it and the a4 atom will be perpendicular to the v13 bond. See the image above.

tmp=get_angle_between_vectors(v14A,v13A);
Ang=PI-tmp;
hyp=v14A.d;
adj=cos(Ang)*hyp; // math is handy...
norm31=normalize_vec(v31A);
dist=v31A.d+adj;
pt.i=(*x_listax[3]).i+(norm31.i*dist);
pt.j=(*x_listax[3]).j+(norm31.j*dist);
pt.k=(*x_listax[3]).k+(norm31.k*dist);


double theta=1.57079633; // angle between BCD. Set to 90.
double distance=6; // set to any distance
coord_3D d;

double phi=0;// Torsion between ABCD. I think it's actually polar coord but no worries.
d=get_cartesian_point_from_internal_coords( *a, *b, *c, theta, phi, distance);
printf("ATOM      1  H   XXX     0    %8.3f%8.3f%8.3f  1.00  0.00\n",d.i,d.j,d.k);




//plane p,p1;
vectormag_3D v1,v2,v3,v4,normal,rv;


coord_3D norm_point, trp_point;
double d;

v1=get_vector_from_coords(x_list[0],x_list[1]);// center to CE2
v2=get_vector_from_coords(x_list[0],x_list[2]);// center to CZ3
v3=get_vector_from_coords(x_list[0],centroid);
v4=get_crossprod(v2,v1);
normal=normalize_vec(v4);


phi=(get_angle_two_vectors(v1,v3)*57.2957795);
//r=get_distance_from_point_to_plane(p, centroid, 1);
d=get_distance_AB_points(centroid,x_list[0]);
theta=(get_angle_two_vectors(v3,normal));

// gotta love the old right angled triangled math being usefull :)
r=(sin(1.570796325-theta)*d); // radians. 90 degrees is 1.57...


theta=(get_angle_two_vectors(v3,normal)*57.2957795);
phi=(get_angle_two_vectors(v1,v3)*57.2957795);


printf("centroid.x=%f,centroid.y=%f,centroid.z=%f\n",centroid_trp.i,centroid_trp.j,centroid_trp.k);
printf("theta=%f,phi=%f,r=%f,d=%f\n",theta,phi,r,d);

rv.i=(normal.i*r);
rv.j=(normal.j*r);
rv.k=(normal.k*r); 
rv.d=r;

// for visualization of normal from CD2 atom using a false atom with these co-ord
norm_point.i=x_list[0].i+(rv.i);
norm_point.j=x_list[0].j+(rv.j);
norm_point.k=x_list[0].k+(rv.k);
printf("\n\nnewpoint.i,j,k=%.3f  %.3f %.3f\n\n",norm_point.i,norm_point.j,norm_point.k);

// get point that is on the trp plane underneath the centroid of the sugar
trp_point.i=centroid.i-rv.i;
trp_point.j=centroid.j-rv.j;
trp_point.k=centroid.k-rv.k;
printf("\n\ntrp_point=%.3f  %.3f %.3f\n",trp_point.i,trp_point.j,trp_point.k);


/*


double degree,cos,angle;

v1.i=P1.A;
v1.j=P1.B;
v1.k=P1.C;
v2.i=P2.A;
v2.j=P2.B;
v2.k=P2.C;

v1=normalize_vec(v1);
v2=normalize_vec(v2);

cos=get_dotprod(v1,v2);
angle=acos(cos);//angle between two lines, result in radians
degree=angle*(180/PI);

printf("\nThe normal between the two planes is:%.2f\n",degree);
*/
return 0;
}
//function body
// could be in main text just used to get more info. redundant to make function now
void get_residue_numbers(int*res1, int*res2)
{
printf("Entered function ok\n");
printf("\nEnter the residue number of your sugar residue: ");
scanf("%d",res1);
printf("\nEnter the residue number of your protein residue: ");
scanf("%d",res2);
}
/*
00020         *XS = pointer to list of coordinates to rotate
00021           n = number of coordinates in list XS to rotate
00022           v = the vector into which the x axis should be rotated
*/

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



