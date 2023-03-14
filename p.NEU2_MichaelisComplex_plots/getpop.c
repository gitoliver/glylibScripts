#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>

main( int argc, char *argv[])  {
double tsum=0;
double tavr=0;
double tvar=0;
double tSvar=0;
int tstate=0;
double gsum=0;
double gavr=0;
double gvar=0;
double gSvar=0;
int gstate=0;
double pop=0;
double stdev=0;
double num=0;
int count=0;
//int swtch=1;// indicates in ground state

if( argc == 2 ){
    double buffer[50];
    FILE *fp = fopen(argv[1], "r");
    if (fp != NULL) {
        while (fscanf(fp, "%lf", &num) !=EOF)
        //while (fgets(num, sizeof(num), fp))
        {
        //printf("num=%lf\n",num);
        count++;
        //if ( (strcmp(argv[2],"fix")==0) && (num < 0) ) { num=(num + 360);}
        if (num < 0){ num=(num * -1);} // transorm any negative values to positive.
        if (num >= 115){
            tstate++;
            tsum = (tsum + num);
            tavr = (tsum / tstate);
            tvar = (num - tavr) * (num - tavr);
            tSvar = (tSvar + tvar);
            //if (swtch==1){ swtch=0; printf("trajin md5000frames.nc %d ",count); }
        }
        if (num < 115){
            gstate++;
            gsum = (gsum + num);
            gavr = (gsum / gstate);
            gvar = (num - gavr) * (num - gavr);
            gSvar = (gSvar + gvar);
            //if (swtch==0){ swtch=1; printf("%d 1\n",count-1); }
        }
        }
    }
    pop=(((double)tstate/count)*100);
    stdev=sqrt(tSvar / tstate);
    if ( tavr > 180 ) { tavr=(tavr - 360);}
    printf("%s Transistion-State:\t Pop=%.1f Average=%-5.1f +/- %-5.1f\n",argv[1],pop,tavr,stdev);

    pop=(((double)gstate/count)*100);
    stdev=sqrt(gSvar / gstate);
    if ( gavr > 180 ) { gavr=(gavr - 360);}
    printf("%s Ground-State:\t Pop=%.1f Average=%-5.1f +/- %-5.1f\n",argv[1],pop,gavr,stdev);
}


else { printf("Usage: \"getpopn.exe dihedral.txt\" \n");}
return 0;
}
