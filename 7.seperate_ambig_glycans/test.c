#include <string.h>
#include <stdio.h>
#include <limits.h>
int main ( int argc, char *argv[] ){

struct backslash {
   int line_position;
   char char_b4;
   char char_after;
};

struct backslash bs[20]; // bs[max number of / per line]

int x,i,j,k, cnt=0;
if ( argc != 2 ) {/* argc should be 2 for correct execution */
    
       	/* We print argv[0] assuming it is the program name */
       	printf( "usage: %s filename\n", argv[0] );
}
else {
       	// We assume argv[1] is a filename to open
        FILE *file = fopen( argv[1], "r" );

        /* fopen returns 0, the NULL pointer, on failure */
        if ( file == 0 ){
		printf( "Could not open file\n" );
        }
        else {
		char line[LINE_MAX];
		while (fgets(line, LINE_MAX, file) != NULL) {
			cnt=0;
			printf("%s",line);
			x=strlen(line);
			printf("strlen=%d\n",x);
			// count number of \ in line
			for (i=0;i<x;i++){
				if (line[i]=='/'){
			//		printf("line[%d]=%c\n",i,line[i]);
					bs[cnt].line_position=i;
					bs[cnt].char_b4=line[i-1]; 
					bs[cnt].char_after=line[i+1];
					cnt++;
				}
			}
			printf("cnt=%d\n",cnt);

			for (i=0;i<(cnt/2)*cnt;i++){
				for (j=0;j<bs[i].line_position;j++){printf("%c",line[j]);} // print everything up to /
				j=j+2; // move past the / + char
				while (j<x){ // print the rest of the line
       		 		       	printf("%c",line[j]);
					j++;
               		      	}
				for (j=0;(j<bs[i].line_position-1);j++){ printf("%c",line[j]); }
				j=j+2; // move past the char + /
				while (j<x){ // print the rest of the line
               		        	printf("%c",line[j]);
               		        	j++;
				}
			}
			printf("\n\n");
		}
		fclose( file );
	}
}
}
