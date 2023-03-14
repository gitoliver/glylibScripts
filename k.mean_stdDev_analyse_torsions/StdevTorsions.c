#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>

main( int argc, char *argv[] )  {
double sum=0;
double avr=0;
double var=0;
double Svar=0;
double stdev=0;
double num=0;
int count=0;

if( argc == 3 ){
    double buffer[50];
    FILE *fp = fopen(argv[1], "r");
    if (fp != NULL) {
        while (fscanf(fp, "%lf", &num) !=EOF)
        //while (fgets(num, sizeof(num), fp))
        {
        //printf("num=%lf\n",num);
        count++;
	if ( (strcmp(argv[2],"fix")==0) && (num < 0) ) { num=(num + 360);}
        sum = (sum + num);
        avr = (sum / count);
        var = (num - avr) * (num - avr);
        Svar = (Svar + var);
        }
    }
    stdev=sqrt(Svar / count);
    printf("%.0f\n",stdev);
}


else { printf("Usage: program dihedral.txt fix/norm\n");}
return 0;
}
