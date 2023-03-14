#include <molecules.h>
#include <mylib.h>
int line_num(char *filename);

int main (void){

//char table[20][7][5]; // 30 resids max

char filename[50];
FILE * pFile;
int i,j,numLines; // counters


strcpy(filename, "LINK.txt");
numLines = line_num(filename);
char table[numLines][7][5]; // 


pFile = fopen (filename,"r");
/*
for (i=0;i<20;i++){
	for (j=0;j<80;j++){
		x= fgetc (pFile);
		printf("%c\n",x);
		table[i][j]=x;
	}
}
*/

for (i=1;i<numLines;i++){ 
	for (j=0;j<7;j++){
		fscanf (pFile, "%s", table[i][j]);
		printf("table[%d][%d] is %s\n",i,j,table[i][j]);
	}
	printf("Residue name is %c%c%c\n",table[i][5][0],table[i][5][1],table[i][5][2]);
}


fclose (pFile);
return 0;
}


int line_num(char *filename)
{
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

if(c != '\n')
lines++;

return lines;
}

