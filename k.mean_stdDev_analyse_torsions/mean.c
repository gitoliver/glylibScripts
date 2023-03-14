#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>

int main( int argc, char *argv[])  {
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
        //printf("Running average=%.1f\n",avr);
        //var = (num - avr) * (num - avr);
        //Svar = (Svar + var);
        }
    }
    //stdev=sqrt(Svar / count);
    //if ( avr > 180 ) { avr=(avr - 360);}
    //printf("%s Average=%.1f +/- %.1f\n",argv[1],avr,stdev);
    printf("%.3f\n",avr);
}


else { printf("Usage: \"meanStdev.exe dihedral.txt fix/norm\"   Set fix if -180+180 problem. norm for normal\n");}
return 0;
}
