/* General algoritm: Takes a carbohydrate pdb and puts sulphates in user specified positions
Usage: add_sulphates -i carb.pdb -s sulph.pos -opdb sulfcarb.pdb

example sulph.pos 	1:O3,O6		or if in bio order 	2:O2
			2:O2		(Prob not this)		1:O6,3:O6
			3:O3,O6					3:O3,1:O3
############################################################################################*/
#include <glylib.h>
//Take in pdb from command line
int main(argc,char *argv[]){

char *pos;

if (argc!=4)
	{printf("Try again... \nUsage: add_sulphates -i carb.pdb -s sulph.pos -opdb sulfcarb.pdb"); 
	exit(0);
	}

//Open and read sulph.pos file
pos=fopen(argv[3],"r");

//open the sulphate molecule pdb and load into and assembly
//

//Call Denny's program to translate the sulphate to specified position
translate_sulphate(int bond, int ang, int tors,*A)//*A is a pointer to a sulphate molecule assembly
//Addition may be in biological order or may just do a final minimzation at finish

//After each addition do a sort minimization in the 3 rotamers -180,60,-60 and select lowest energy
//This is a seperate function that must be generic. So give it 4 atoms.

//spit out the modified carb.pdb
