
#include <glylib.h> // required for load_pdb function
#include <mylib.h>
#include <molecules.h>

//function prototype
int main void {
assembly B; // glylib structure which holds molecule information
FILE * pFile;
//char branch[25]; 
char filename[25];// for loading file in
char line_buffer[300];
char word_buffer[300];

fscanf (pFile, "%s",buffer);
if (strcmp(buffer,"LINK")==0){
                        j++; // j will be the number of residues in LG
                        strcpy(LGtable[j][0],buffer);
                        for (k=1;k<7;k++){ // for the next 6 entries
                                fscanf (pFile, "%s", LGtable[j][k]);
        //                      printf("LGtable[%d][%d]=%s\n",j,k,LGtable[j][k]);
                        }
                }
        }

char str[200];
FILE *fp;
 
fp = fopen("test.txt", "r");
if(!fp) return 1; // bail out if file not found
while(fgets(str,sizeof(str),fp) != NULL)
	{
	// strip trailing '\n' if it exists
	int len = strlen(str)-1;
	if(str[len] == '\n') 
	str[len] = 0;
	printf("\n%s", str);
	}
fclose(fp);
}



}
