#include <glylib.h>
//function prototype
void get_names_for_plane(int*opt,int*atm_num,int*atm_num2,int*res1,int*res2,char*atm_name);

//function body
void get_names_for_plane(int*opt,int*atm_num,int*atm_num2,int*res1,int*res2,char*atm_name)
{

int i=0;
//get user to pick 1.sugar-sugar, 2.sugar-protein or 3.protein-protein
/*while (*opt<1 || *opt>3) {
    printf("\nCalculates angle between two planes\nOptions are \n1.sugar-sugar\n2.sugar-protein\n3.protein-protein");
    printf("\nWhich option would you like to do? Enter 1,2 or 3: ");
    scanf("%d",opt);
    }*/
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
    printf("\nEnter number of atoms that will define the plane in your protein residue (min3): ");
    scanf("%d",atm_num);
    for (i=0;i<atm_num;i++){
        printf("\nEnter the name of plane atom number %d: ",i+1);
        scanf("%s",atm_name[i]);
        }
    }
if (opt==3){
    printf("\nEnter the number of your 1st protein residue: ");
    scanf("%d",res1);
    printf("\nEnter number of atoms that will define the plane(min3): ");
    scanf("%d",&atm_num);
    for (i=0;i<atm_num;i++){
        printf("\nEnter the name of plane atom number %d: ",i+1);
        scanf("%s",atm_name[i]);
        }

    printf("\nEnter the number of your 2nd protein residue: ");
    scanf("%d",res2);
    printf("\nEnter number of atoms that will define the plane(min3): ");
    scanf("%d",&atm_num2);
    for (i=0;i<atm_num;i++){
        printf("\nEnter the name of plane atom number %d: ",i+1);
        scanf("%s",atm_name[i]);
        }
    }
}
