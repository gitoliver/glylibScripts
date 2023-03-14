#include <stdio.h>
#include <stdlib.h>



/********************DOUBLE *****************************/
double *D_Allocate_1D_Matrix (int N)
{
	double *A;
	A = (double *) malloc(N*sizeof(double) );
	return A;
}

double **D_Allocate_2D_Matrix (int N, int M)
{
	int i;
	double **A;

	A = (double **) malloc(N*sizeof(double*) );
	for (i = 0; i < N; i++)
		A[i] = (double *) malloc(M*sizeof(double) );
	return A;
}

double ***D_Allocate_3D_Matrix (int N, int M, int L)
{
	int i, j;
	double ***A;

	A = (double ***) malloc(N*sizeof(double**) );
	for (i = 0; i < N; i++)
	{
		A[i] = (double **) malloc(M*sizeof(double*) );
		for (j = 0; j < M; j++)
			A[i][j] = (double *) malloc(L*sizeof(double) );
	}
	return A;
}

void D_free_2D (double **A, int N)
{
	int i;
	for (i = 0; i < N; i++)
		free(A[i]);
	free(A);
}

void D_free_3D (double ***A, int N, int M)
{
	int i, j;

	for (i = 0; i < N; i++)
	{
		for (j = 0; j < M; j++)
			free(A[i][j]);
		free(A[i]);
	}
	free(A);
}



/********************INT *****************************/
int *I_Allocate_1D_Matrix (int N)
{
	int *A;
	A = (int *) malloc(N*sizeof(int) );
	return A;
}

int **I_Allocate_2D_Matrix (int N, int M)
{
	int i;
	int **A;

	A = (int **) malloc(N*sizeof(int*) );
	for (i = 0; i < N; i++)
		A[i] = (int *) malloc(M*sizeof(int) );
	return A;
}

int ***I_Allocate_3D_Matrix (int N, int M, int L)
{
	int i, j;
	int ***A;

	A = (int ***) malloc(N*sizeof(int**) );
	for (i = 0; i < N; i++)
	{
		A[i] = (int **) malloc(M*sizeof(int*) );
		for (j = 0; j < M; j++)
			A[i][j] = (int *) malloc(L*sizeof(int) );
	}
	return A;
}

void I_free_2D (int **A, int N)
{
	int i;
	for (i = 0; i < N; i++)
		free(A[i]);
	free(A);
}

void I_free_3D (int ***A, int N, int M)
{
	int i, j;

	for (i = 0; i < N; i++)
	{
		for (j = 0; j < M; j++)
			free(A[i][j]);
		free(A[i]);
	}
	free(A);
}





/********************CHAR *****************************/
char *C_Allocate_1D_Matrix (int N)
{
	char *A;
	A = (char *) malloc(N*sizeof(char) );
	return A;
}

char **C_Allocate_2D_Matrix (int N, int M)
{
	int i;
	char **A;

	A = (char **) malloc(N*sizeof(char*) );
	for (i = 0; i < N; i++)
		A[i] = (char *) malloc(M*sizeof(char) );
	return A;
}

char ***C_Allocate_3D_Matrix (int N, int M, int L)
{
	int i, j;
	char ***A;

	A = (char ***) malloc(N*sizeof(char**) );
	for (i = 0; i < N; i++)
	{
		A[i] = (char **) malloc(M*sizeof(char*) );
		for (j = 0; j < M; j++)
			A[i][j] = (char *) malloc(L*sizeof(char) );
	}
	return A;
}


void C_free_2D (char **A, int N)
{
	int i;
	for (i = 0; i < N; i++)
		free(A[i]);
	free(A);
}

void C_free_3D (char ***A, int N, int M)
{
	int i, j;

	for (i = 0; i < N; i++)
	{
		for (j = 0; j < M; j++)
			free(A[i][j]);
		free(A[i]);
	}
	free(A);
}

