#include <glylib.h>

int main( int argc, char *argv[])  {
assembly A,B;
if( argc == 3 ){
    load_pdb(argv[1],&A);
    load_pdb(argv[2],&B);
    //printf("A.nb=%d,B.nb=%d\n",A.nb,B.nb);
    if (A.nb!=B.nb){
        printf("Internal Bonds: %s\n",argv[2]);
    }
}
else { printf("usage: compare_find_clash.exe structure1.pdb structure2.pdb\n");}
return 0;
}
