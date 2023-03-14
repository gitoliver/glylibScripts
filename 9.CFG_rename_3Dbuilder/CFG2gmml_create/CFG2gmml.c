#include <glylib.h>
// This program moves sulfates to after ring type declaration and before reducing anomeric declaration. It stack smashes on the last line and I cannot for the life of me find out why. I is be bad programming man.


//FUNCTION DECLARATIONS
int line_num(char *filename);
// MAIN
int main (int argc, char *argv[]){
// CHECK IF STARTED INCORRECTLY
if (argc!=2) {
    printf("Usage: CFG2gmml input_filename\n");
}
if (argc==2) {
// IF STARTED CORRECTLY
char inputname[50];
strcpy(inputname, argv[1]);
// printf("Beginning...\n");

int n0lines=0,i=0,j=0,k=0,l=0,len=0,Spt=0,stop=0;
FILE * pFile;
char buffer[500];
    
// now deal with moving sulfates
pFile = fopen (inputname,"r");
n0lines= line_num(inputname);
//printf("n0lines in file=%d\n",n0lines);

char txtfile[(n0lines*2)+4][500]; // It's gonna read in string by string so ID's are in odd numbered places

for (i=1;i<=n0lines*2;i++){
    fscanf (pFile, "%s", txtfile[i]);
//    printf("line%d=%s\n",i,txtfile[i]);
}
fclose (pFile);
    // as id will be in the odd numbered places do i+2
for (i=2;i<=n0lines*2;i=i+2){
    // strcpy(buffer[i],txtfile[i]);
    //get length of glycan sequence
    len=strlen(txtfile[i]);
//    printf("len=%d",len);
//  printf("%s\n",txtfile[i]);
    // go through each character in sequence
    for(j=0;j<=len;j++){
        //printf("%c",txtfile[i][j]);
        //put char into buffer
        buffer[j]=txtfile[i][j];
        // if character ahead is an S
        if (txtfile[i][j+2]=='S') {
            stop=0; // don't stop :)
            Spt=j; // Save Sulfate start point
            //printf("\nSpt=%d\n",Spt);      
            k=j+4; // skip over the sulfate
            while (stop!=1){ // keep going until find next a/bX- where x is linkage. Can also reach end without a or b thus k+4==len.
                if ( ( ((txtfile[i][k]=='a') || (txtfile[i][k]=='b')) && (txtfile[i][k+2]=='-') ) || ( ((k+4)==len) ) ){
                    stop=1;
                    //write sulfate into buffer
                    for (l=Spt;l<Spt+4;l++){
                        //printf("Writing %c into buffer %d\n",txtfile[i][l],j);
                        buffer[j]=txtfile[i][l];
                        j++;
                    }
                    buffer[j]=txtfile[i][k];
              //    printf("j=%d,k=%d\n",j,k);
                }
                else {
                     buffer[j]=txtfile[i][k]; // write in what was after [XS] where x is an integer 2,3,4,5 or 6
                     k++;
              //       printf("k=%d, ",k);
                     j++;
              //       printf("j=%d\n",j);
//                   printf("%s\n",buffer);
                }    
            }
        }
    }
    strcat(buffer,"\0");
    //printf("strlen=%d\n",len);
    printf("%s\t",txtfile[i-1]); // print the id
    printf("%s\n",buffer); // print the buffer contents
}
}    
//printf("Finished\n");
//printf("strlen=%d\n",len);
return 0;
}

//FUNCTIONS
int line_num(char *filename){
FILE *f;
char c;
int lines = 0;
f = fopen(filename, "r");
if(f == NULL)
return 0;
while((c = fgetc(f)) != EOF)
if(c == '\n')
lines++;
fclose(f);
//if(c != '\n')
//lines++;
return lines;
}




