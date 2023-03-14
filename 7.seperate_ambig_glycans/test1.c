#include <stdio.h>
 
int main (void){
char str[200];
FILE *fp;
 
fp = fopen("test.txt", "r");
	if(!fp) return 1; // bail out if file not found
	while(fgets(str,sizeof(str),fp) != NULL){
		// strip trailing '\n' if it exists
      		int len = strlen(str)-1;
      		if(str[len] == '\n') 
         	str[len] = 0;
     		printf("\n%s", str);
   	}
   	fclose(fp);
}

