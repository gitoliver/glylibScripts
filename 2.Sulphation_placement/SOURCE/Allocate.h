double *D_Allocate_1D_Matrix (int N);
double **D_Allocate_2D_Matrix (int N, int M);
double ***D_Allocate_3D_Matrix (int N, int M, int L);
void D_free_2D (double **A, int N);
void D_free_3D (double ***A, int N, int M);


int *I_Allocate_1D_Matrix (int N);
int **I_Allocate_2D_Matrix (int N, int M);
int ***I_Allocate_3D_Matrix (int N, int M, int L);
void I_free_2D (int **A, int N);
void I_free_3D (int ***A, int N, int M);


char *C_Allocate_1D_Matrix (int N);
char **C_Allocate_2D_Matrix (int N, int M);
char ***C_Allocate_3D_Matrix (int N, int M, int L);
void C_free_2D (char **A, int N);
void C_free_3D (char ***A, int N, int M);

