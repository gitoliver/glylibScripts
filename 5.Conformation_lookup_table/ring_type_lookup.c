#include <glylib.h>

//conformation_lookup_table (const char * name, arraywithupdowncode, int x ) // if x=0 then letter coming in, code going out. x=1, vice versa

//void get_ringtype(){
int main (void){

typedef struct {
char names[50][20]; // 50 sugar types with 20 name length
char table[50][8][2]; // 8 descriptors with 50 corresponding sugars
int n; // number of entries
} table_struct;

char filename[50];
FILE * pFile;
table_struct table;
int i,j; // counters

// need to have a make opposite function for L isomerisation

//read in file, put the first string in names, and the rest in table until an end of line...
strcpy(filename, "lookuptable.txt");
pFile = fopen (filename,"r");
table.n=36;

for (i=1;i<table.n;i++){
	fscanf (pFile, "%s", table.names[i]);
	printf("table.names[%d][1] is %c\n",i,table.names[i][1]);
	for (j=0;j<8;j++){
		fscanf (pFile, "%s", table.table[i][j]);
		printf("table[%d][%d] is %c\n",i,j,table.table[i][j][0]);
	}
}
fclose (pFile);
return 0;
}
