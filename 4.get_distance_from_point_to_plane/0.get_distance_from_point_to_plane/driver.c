// Function written by Oliver Grant, 2011
#include <glylib.h>
//#include <mylib.h>
//#include <molecules.h>
/************** driver to test get_distance_from_pt_to_plane() ******************/
int main (void){
double dist;
plane p;
coord_3D pt;
int absl;

printf("mark 1!\n");

p.A=1;
p.B=1;
p.C=1;
p.D=1;

pt.i=1;
pt.j=1;
pt.k=1;

absl=1;

dist=get_distance_from_point_to_plane(p, pt, absl);
printf("mark 2!\n");

printf("Distance between pt and plane is %f\n",dist);

return 0;
}
