// Function written by Oliver Grant, 2011
/*
 * Purpose: Built to determine if an atom was above or below a plane. 
 * Returns a double which is the distance between the point and the plane.
 * Distance can be negative (below plane) or positive (above).
 * An absolute value can be returned if you pass in absl==1.
 * Testing: Minimal as of 21/Apr/2011
#include <glylib.h>
//#include <mylib.h>
//#include <molecules.h>
/************** get_distance_from_point_to_plane() ******************/
double get_distance_from_point_to_plane(plane p, coord_3D pt, int absl){

double sum, sqroot, d;

sum=(p.A)*(pt.i) + (p.B)*(pt.j) + (p.C)*(pt.k);
sqroot=sqrt(p.A*p.A + p.B*p.B + p.C*p.C);
d=sum/sqroot;

//get absolute value
if (absl==1){
	if (d<0) {d=d*(-1);}
	}
return d;
}
