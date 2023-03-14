/*written by Oliver Grant 08/Dec/2010

Function that gets the angle between two planes. The planes can be either from sugar or protein residueswhose numbers are entered by the user. For sugars it automatically uses all 6 ring atoms and calls get_plane_for_ring. For protein residues it asks the user to enter 3 atom names that will define the plane. 

Currently it works for 1.sugar-sugar, 2.sugar-protein but not for 3.protein-protein OG 8/Dec/10

Improvements: 
1.Alter get_plane_for_ring and get_plane so they return a pointer to an array of plane values for each snapshot in trajectory so that std_dev can be calculated. OG 13/Dec/10
2.Accept input file names as variables
*/
#include <glylib.h>
//function prototype
void get_names_for_plane(int opt,int*res1,int*res2,char*atm_name,char*atm_name2);

int main(){
int nc=6,mi=0,ri=0,ai=0;
assembly A;
coord_3D *x_list;
fileset IF, IC;
plane P1,P2;

IF.N=strdup("input_prmtop.top");
IF.F=myfopen(IF.N,"r");

IC.N=strdup("input_crd.crd");
IC.F=myfopen(IC.N,"r");

//get name of sugar residue and the 3 atoms that make up the plane in the protein residue
int opt=0,res1=0,res2=0;
int i=0;//counter 
char atm_name[3],atm_name2[3];//asks user to select an option
  while (opt<1 || opt>3){
   printf("Select an option:\n1.Sugar-Sugar\n2.Sugar-Protein\n3.Protein-Protein\nEnter 1,2 or 3: ");
   scanf("%d",&opt);
   }
// load prmtop file that is in the directory
A=load_amber_prmtop(IF);
// load trajcrds into assembly
add_trajcrds_to_prmtop_assembly(IC,&A,'c',0);


// get names of the two residues and atom names(if protein) from user
get_names_for_plane(opt,&res1,&res2,atm_name,atm_name2);

x_list=(coord_3D*)calloc(nc, sizeof(coord_3D)); //creates a list to store co-ord

//for res1 get coords and store in x_list
for(mi=0;mi<A.nm;mi++){
	for(ri=0;ri<A.m[mi][0].nr;ri++){
		if(A.m[mi][0].r[ri].n==res1){
			if (opt==1 || opt==2){//when res1 is a sugar residue
				for(ai=0;ai<A.m[mi][0].r[ri].na;ai++){
					if(strcmp(A.m[mi][0].r[ri].a[ai].N,"C1")==0){
						x_list[i]=A.m[mi][0].r[ri].a[ai].xa[0];}
                                        if(strcmp(A.m[mi][0].r[ri].a[ai].N,"C2")==0){
                                       		x_list[1]=A.m[mi][0].r[ri].a[ai].xa[0];}
  		                        if(strcmp(A.m[mi][0].r[ri].a[ai].N,"C3")==0){
                		                x_list[2]=A.m[mi][0].r[ri].a[ai].xa[0];}
                                	if(strcmp(A.m[mi][0].r[ri].a[ai].N,"C4")==0){
                                       		x_list[3]=A.m[mi][0].r[ri].a[ai].xa[0];}
 		                        if(strcmp(A.m[mi][0].r[ri].a[ai].N,"C5")==0){
                		                x_list[4]=A.m[mi][0].r[ri].a[ai].xa[0];}
                                	if(strcmp(A.m[mi][0].r[ri].a[ai].N,"O5")==0){
                                       		x_list[5]=A.m[mi][0].r[ri].a[ai].xa[0];}
				}
			}
			else {//when opt==3 then res1 will be a protein residue
				for (i=0;i<3;i++){
                                        for(ai=0;ai<A.m[mi][0].r[ri].na;ai++){
                                                if(strcmp(A.m[mi][0].r[ri].a[ai].N,atm_name)==0){
                                                         x_list[i]=A.m[mi][0].r[ri].a[ai].xa[0];


                                                        }
                                                }
                                        }
                                }
			}
		}
	}
if (opt==3) { //when opt==3 then res1 will be a protein residue
	P1=get_plane(x_list[0],x_list[1],x_list[2]);
}
else { //when res1 is a sugar residue
	P1=get_plane_for_ring(6,x_list);
}
//for res2
for(mi=0;mi<A.nm;mi++){
        for(ri=0;ri<A.m[mi][0].nr;ri++){
                if(A.m[mi][0].r[ri].n==res2){
                        if (opt==1){ //when res2 is a sugar molecule
                                for(ai=0;ai<A.m[mi][0].r[ri].na;ai++){
                                        if(strcmp(A.m[mi][0].r[ri].a[ai].N,"C1")==0){
                                                x_list[i]=A.m[mi][0].r[ri].a[ai].xa[0];}
                                        if(strcmp(A.m[mi][0].r[ri].a[ai].N,"C2")==0){
                                                x_list[1]=A.m[mi][0].r[ri].a[ai].xa[0];}
                                        if(strcmp(A.m[mi][0].r[ri].a[ai].N,"C3")==0){
                                                x_list[2]=A.m[mi][0].r[ri].a[ai].xa[0];}
                                        if(strcmp(A.m[mi][0].r[ri].a[ai].N,"C4")==0){
                                                x_list[3]=A.m[mi][0].r[ri].a[ai].xa[0];}
                                        if(strcmp(A.m[mi][0].r[ri].a[ai].N,"C5")==0){
                                                x_list[4]=A.m[mi][0].r[ri].a[ai].xa[0];}
                                        if(strcmp(A.m[mi][0].r[ri].a[ai].N,"O5")==0){
                                                x_list[5]=A.m[mi][0].r[ri].a[ai].xa[0];}
                                }
                        }
                        else {//when res2 is a protein
                                for (i=0;i<3;i++){
                                        for(ai=0;ai<A.m[mi][0].r[ri].na;ai++){
                                                if(strcmp(A.m[mi][0].r[ri].a[ai].N,atm_name2)==0){
                                                         x_list[i]=A.m[mi][0].r[ri].a[ai].xa[0];
                                                        }
                                                }
                                        }
                                }
                        }
                }
        }
if (opt==1) 	{P2=get_plane_for_ring(6,x_list);} //when res2 is a sugar molecule
else 		{P2=get_plane(x_list[0],x_list[1],x_list[2]);} //when res2 is a protein

vectormag_3D v1,v2;
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

return 0;
}
//function body
void get_names_for_plane(int opt,int*res1,int*res2,char*atm_name,char*atm_name2)
{
printf("Entered function ok");
int i=0;
//if 1 get names of each sugar residue
if (opt==1){
    printf("\nEnter the number of your 1st sugar residue: ");
    scanf("%d",res1);
    printf("\nEnter the number of your 2nd sugar residue: ");
    scanf("%d",res2);
    }
//if 2 get name of sugar residue and the name and atoms that define plane in the protein residue
if (opt==2){
    printf("\nEnter the residue number of your sugar residue: ");
    scanf("%d",res1);
    printf("\nEnter the residue number of your protein residue: ");
    scanf("%d",res2);
    printf("\nThe number of atoms that will define the plane in your protein residue is 3: ");
    for (i=0;i<3;i++){
        printf("\nEnter the name of plane atom number %d: ",i+1);
        scanf("%s",atm_name);
        }
    }
if (opt==3){
    printf("\nEnter the number of your 1st protein residue: ");
    scanf("%d",res1);
    printf("\nThe number of atoms that will define the plane in your protein residue is 3: ");
    for (i=0;i<3;i++){
        printf("\nEnter the name of plane atom number %d: ",i+1);
        scanf("%s",atm_name);
        }
   printf("\nEnter the number of your 2nd protein residue: ");
    scanf("%d",res2);
printf("\nThe number of atoms that will define the plane in your protein residue is 3: ");
    for (i=0;i<3;i++){
        printf("\nEnter the name of plane atom number %d: ",i+1);
        scanf("%s",atm_name2);
        }
    }
}
