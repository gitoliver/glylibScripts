#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>

main( int argc, char *argv[])  {
double num1=0, num2=0;
int count=0;
int swtch=1;// indicates in ground state

if( argc == 3 ){
    FILE *fp1 = fopen(argv[1], "r");
    FILE *fp2 = fopen(argv[2], "r");
    if ( (fp1 != NULL) && (fp2 != NULL) ){
        while ( (fscanf(fp1, "%lf", &num1) !=EOF) && (fscanf(fp2, "%lf", &num2) !=EOF) )
        {
        count++;
        if (num1 < 0){ num1=(num1 * -1);} // transorm any negative values to positive.
        if (num2 < 0){ num2=(num2 * -1);} // transorm any negative values to positive.
        if ( (num1 >= 115) && (num2 >= 115) ){
            if (swtch==1){ swtch=0; printf("trajin md5000frames.nc %d ",count); }
        }
        if ( (num1 < 115) || (num2 < 115) ){
            if (swtch==0){ swtch=1; printf("%d 1\n",count-1); }
        }
        }
    }
    printf("trajout MC.nc netcdf\n");
}
else { printf("Usage: \"getMCstate.exe MCdihedral1.txt MCdihedral2.txt\" \n");}
return 0;
}
