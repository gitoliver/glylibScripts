/* Script for generating the various permutations of ambiguous glycan sequences. For use with glycome builder. Sequences like 
35811	LFucpa1-6[DGlcpNAcb1-2DManpa1-6[DGalpb1-3/4[LFucpa1-3/4]DGlcpNAcb1-2DManpa1-3]DManpb1-4DGlcpNAcb1-4]DGlcNAc-ol?1-OH 
Will have each of the possibilties generated. */ // OG


#include <string.h>
#include <stdio.h>
#include <limits.h>
#include <math.h>
int main ( int argc, char *argv[] ){

struct backslash {
   int line_position;
   char char_b4;
   char char_after;
};

struct backslash bs[20]; // bs[max number of / per sequence] set by me arbitrarily

int line_info[20][1][2]; // slash number, postion in line, charb4 charafter

int line_length, num_lines, num_positions, offset;
int x, y, i, j, k, cnt=0;

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
			num_positions=0;
			//printf("%s",line);
			line_length=strlen(line);
			//printf("strlen=%d\n",line_length);

			// count number of \ in line 
			for (i=0;i<line_length;i++){
				if (line[i]=='/'){
			//		printf("line[%d]=%c\n",i,line[i]);
					bs[num_positions].line_position=i;
			//		bs[num_positions].char_b4=line[i-1]; 
					bs[num_positions].char_after=line[i+1];
					num_positions++;
				}
			}

			//printf("num_positions=%d\n",num_positions);
			num_lines=pow(2,num_positions);
			//printf("num_lines=%d\n", num_lines);


			// CREATE CORRECT NUMER OF LINES AND STORE IN BUFFER. REMOVE ANY / AND REMOVE CHAR AFTER.
			// THIS TEMPLATE IS THEN EDITED LATER
			char buffer[num_lines][(line_length-(2*num_positions))];
			for (i=0;i<(num_lines);i++){
				k=0;
				for (j=0;j<line_length;j++){
					if (line[j]=='/'){ j=j+2;}
					buffer[i][k]=line[j];
					//printf("%c",line[j]);
					k++;
				}
			}


			// PRINT CONTENTS OF BUFFER
		/*	for (i=0;i<(num_lines);i++){
                                for (j=0;j<(line_length-(2*num_positions));j++){
					printf("%c",buffer[i][j]);
				}
				printf("\n");
			}
		*/
			// ALTER THE STRING IN BUFFER CREATING PERMUTATIONS
			
			//for each position
			for (i=0;i<(num_positions);i++){
				x=0;
				// y is how often to switch between keeping original or changing it to char after /
				y=num_lines/(pow(2,(i+1)));
			//	printf("y=%d\n",y);
				// for each line
				for (j=0;j<(num_lines);j++){
					cnt++;
					// change item at position i -1 + -1*(2*position_num) // offset due to / and char being removed
					offset=(-2*(i+1)+1);
			//		printf("offset=%d\n",offset);
					if (x==1) { 
						buffer[j][bs[i].line_position+offset]=bs[i].char_after; 
					}
					if (cnt==y) { // switch x
						cnt=0;
						if      (x==0) {x=1;}
						else if (x==1) {x=0;}
					}
				}
			}

			// PRINT CONTENTS OF BUFFER
                        for (i=0;i<(num_lines);i++){
                                for (j=0;j<(line_length-(2*num_positions));j++){
					// number each permutation differently
					if (buffer[i][j]=='\t'){
						printf("_p%d",(i+1));
					} 
                                        printf("%c",buffer[i][j]);
                                }
                //              printf("\n");
                        }
		//	printf("\n\n");
		}
		fclose( file );
	}
}
}
