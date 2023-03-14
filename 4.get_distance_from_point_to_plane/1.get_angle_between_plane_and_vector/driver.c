// Function written by Oliver Grant, 2011
#include <mylib.h>
#include <molecules.h>
/************** driver for get_angle_between_plane_and_vector() ******************/
int main (void){

double angle;
plane p;
coord_3D pt1,pt2;

p.A=1; 
p.B=1;
p.C=1; 
p.D=1;

pt1.i=1;
pt1.j=1;
pt1.k=1;

pt2.i=2;
pt2.j=2;
pt2.k=2;

angle=get_angle_between_plane_and_vector(p, pt1, pt2);

printf("Angle between plane and vector = %f\n",angle);

return 0;
}
