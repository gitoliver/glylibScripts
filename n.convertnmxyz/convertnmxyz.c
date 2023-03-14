#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>

main( int argc, char *argv[])  {

double num=0.0,num1=0.0, num2=0.0, num3=0.0;
int cnt=0;

if( argc == 2 ){
//    double buffer[50];
    FILE *fp = fopen(argv[1], "r");
    if (fp != NULL) {
        while (fscanf(fp, "%lf", &num) !=EOF){
	    cnt++;
	    if (cnt==1){num1=num*10;}
	    if (cnt==2){num2=num*10;}
	    if (cnt==3){
	        num3=num*10;  
	        printf("%12.6lf%12.6lf%12.6lf\n",num1,num2,num3); 
	        cnt=0;
	    }
        }
    }
}


else { printf("Usage: \"convertnmxyz.exe input.dat\"");}
return 0;
}
