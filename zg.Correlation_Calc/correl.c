#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include <sys/stat.h>
    
int main( int argc, char *argv[])  
{
    if( argc != 3 )
    {
        printf("Usage: \"%s column1.txt column2.txt\"\n",argv[0]);
    }
    else
    {
        /* Calculate number of lines */
        int length=0;
        char ch;
        FILE *fp = fopen(argv[1], "r");
        while(!feof(fp))
        {
            ch = fgetc(fp);
            if(ch == '\n')
            {
                ++length;
            }
        }
        fclose(fp);

        /* Read inputs from each line */
        float num1, num2;
        FILE *fp1 = fopen(argv[1], "r");
        FILE *fp2 = fopen(argv[2], "r");
        float x[length], y[length];
        int j = 0;
        if ( (fp1 != NULL) && (fp2 != NULL) ) 
        {
            while ( (fscanf(fp1, "%f", &num1) !=EOF) && (fscanf(fp2, "%f", &num2) !=EOF) )
            {
                x[j] = num1;
                y[j] = num2;
                ++j;
            }
        }
    
        float r,xx[length],yy[length],nr=0,dr_1=0,dr_2=0,dr_3=0,dr=0;
        float sum_y=0,sum_yy=0,sum_xy=0,sum_x=0,sum_xx=0;
        int i,n=length;
        for(i=0;i<n;i++)
        {
         xx[i]=x[i]*x[i];
         yy[i]=y[i]*y[i];
        }
        for(i=0;i<n;i++)
        {
         sum_x+=x[i];
         sum_y+=y[i];
         sum_xx+= xx[i];
         sum_yy+=yy[i];
         sum_xy+= x[i]*y[i];     
        }
        nr=(n*sum_xy)-(sum_x*sum_y);
        float sum_x2=sum_x*sum_x;
        float sum_y2=sum_y*sum_y;
        dr_1=(n*sum_xx)-sum_x2;
        dr_2=(n*sum_yy)-sum_y2;
        dr_3=dr_1*dr_2;
        dr=sqrt(dr_3);
        r=(nr/dr);
        //printf("Total Numbers:%d\nCorrelation Coefficient:%.2f\n",n,r);
        printf("r=%.2f\n",r);
    }
return 0;
}
    
    
    
