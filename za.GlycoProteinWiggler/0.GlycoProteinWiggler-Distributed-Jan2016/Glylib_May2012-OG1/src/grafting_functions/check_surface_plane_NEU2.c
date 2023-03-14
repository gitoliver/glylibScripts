// Function adapted by Oliver Grant Jan 2013
/* Function checks whether a receptor (assembly B) is below the plane at right angles to vector defined by LNK:CL5 and LNK:OL2 atoms. 
 * Requires LNK as end of aglycon.  
 * pt1 is CL5, pt2 is OL1 and pt3 becomes every CA atom in receptor in turn 
 * checks angle between pt1-->pt2 and pt1-->pt3. If >90 degrees then pt3 is below the plane (Surface clash and can't bind).
 * Illustration of an example:
 *
 *                    (OL2)pt1 
 *                          |
 *                          |         (CA)pt3 (above plane)                
 *                          |                               (CA)pt3 (above plane)
 *                          |
 * -------------------(CL5)pt2------------------------------------------------------------- 90 degree plane
 *                                                 (CA)pt3 (below plane)
 *            (CA)pt3 (below plane)
 *
 * A faster way could be to align the bond to the Z axis and check the sign of the Z component.
 */
#include <mylib.h>
#include <molecules.h>
int check_surface_plane_NEU2(assembly *A, assembly *B){ // A is Spacer. B is receptor
int is_below=0; // Number of residues below the plane.
int ai=0,ri=0,mi=0; // Counters
vectormag_3D v12, v23; //v02 means vector from pt1 to pt2 
double angle; 
double sumx1=0.0, sumy1=0.0, sumz1=0.0;
double sumx2=0.0, sumy2=0.0, sumz2=0.0;
coord_3D pt1,pt2;
coord_3D *pt3; pt3=(coord_3D*)calloc(1, sizeof(coord_3D));
int numRes=0;
// Find pt1 and pt2
for(mi=0;mi<(*A).nm;mi++){ // for each molecule
        for(ri=0;ri<(*A).m[mi][0].nr;ri++){ // ***************************** YO REALLY SPECIFIC TO NEU2 HERE!!!!!! Bilayer. Only want average of top layer. First 29 residues.
                if( (strcmp((*A).m[mi][0].r[ri].N,"DMP")==0) && ((*A).m[mi][0].r[ri].n<30) ) {
                        for(ai=0;ai<(*A).m[mi][0].r[ri].na;ai++){ // f
				if(strcmp((*A).m[mi][0].r[ri].a[ai].N,"N50")==0){sumx1+=(*A).m[mi][0].r[ri].a[ai].x.i; sumy1+=(*A).m[mi][0].r[ri].a[ai].x.j; sumz1+=(*A).m[mi][0].r[ri].a[ai].x.k;}
				if(strcmp((*A).m[mi][0].r[ri].a[ai].N,"P43")==0){sumx2+=(*A).m[mi][0].r[ri].a[ai].x.i; sumy2+=(*A).m[mi][0].r[ri].a[ai].x.j; sumz2+=(*A).m[mi][0].r[ri].a[ai].x.k;
                                    numRes+=1;}
			}
		}
	}
}
pt1.i=(sumx1/numRes); pt1.j=(sumy1/numRes); pt1.k=(sumz1/numRes);
pt2.i=(sumx2/numRes); pt2.j=(sumy2/numRes); pt2.k=(sumz2/numRes);

printf("%.3f  %.3f  %.3f\n",pt1.i,pt1.j,pt1.k);
printf("%.3f  %.3f  %.3f\n",pt2.i,pt2.j,pt2.k);


// Find pt0
v12=get_vector_from_coords(pt1,pt2);

// Check angle of v12 against each v13 in receptor
for(mi=0;mi<(*B).nm;mi++){ // for each molecule
	for(ri=0;ri<(*B).m[mi][0].nr;ri++){ // for each residue in molecule
        	for(ai=0;ai<(*B).m[mi][0].r[ri].na;ai++){
			if (strcmp((*B).m[mi][0].r[ri].a[ai].N,"CA")==0){
				pt3=&(*B).m[mi][0].r[ri].a[ai].x;
				v23=get_vector_from_coords(pt2,*pt3);
				angle=get_angle_between_vectors(v12,v23);
                               // printf("Residue:%s Angle:%.1f\n",(*B).m[mi][0].r[ri].N,angle);
				if (angle<1.57){ // 90/57.2957795=1.57 90 degrees in radians...
					//printf("Residue %d, atom %s, angle is %.2f\n",(*B).m[mi][0].r[ri].n,(*B).m[mi][0].r[ri].a[ai].N,(angle*57.2957795));
					is_below++;
				}
			}
		}
	}		
}
printf("is_below=%d\n",is_below);
return is_below;
}
