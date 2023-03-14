/*This program attaches a glycam to protein back bone.
  the input is 2 pdb files, atom numbers for the protein
  and glycam and distances, bond angles, and torsions.
  the output is a third pdb file
*/
/*argv[] = protein_pdb, glycan_pdb, output_pdb, (PROT) CG2, 
(PROT) CB, (PROT) OG1, (GLYC) C1, (GLYC) C2, (GLYC) C3,
d_C1_OG1, a_CB_OG1_C1, a_OG1_C1_C2, w_CG2_CB_OG1_C1, 
w_CB_OG1_C1_C2, w_OG1_C1_C2_C3 */


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#include "Allocate.h"
#include "vector_functions.h"


#define Pi	3.141594
#define EPS .001



#define DEBUG_FILE "DEBUG_FILE"

void Read_Pdb_File (int *n_atoms, int **atom_num, char ***atom_name, 
	char ***res_name, int **res_num, double ***R_2D, double **A, 
	double **B, int **terminal, char *filename, FILE *fp_debug, int DEBUG);

void Print_Combined_Pdb (int prot_n_atoms, int *prot_atom_num, char **prot_atom_name,
		char **prot_res_name, int *prot_res_num, double **prot_r2d, 
		double *prot_A, double *prot_B, int *prot_terminal, int glyc_n_atoms, 
		int *glyc_atom_num, char **glyc_atom_name, char **glyc_res_name, int *glyc_res_num, 
		double **glyc_r2d, double *glyc_A, double *glyc_B, int *glyc_terminal, char *filename);
void Get_Rotation_Matrix (double n[3], double angle, double Rot[3][3]);

int main(int argc, char *argv[])
/*argv[] = protein_pdb, glycan_pdb, output_pdb, (PROT) CG2, 
(PROT) CB, (PROT) OG1, (GLYC) C1, (GLYC) C2, (GLYC) C3,
d_C1_OG1, a_CB_OG1_C1, a_OG1_C1_C2, w_CG2_CB_OG1_C1, 
w_CB_OG1_C1_C2, w_OG1_C1_C2_C3 */
{
	char PROT_filename[500];
	char GLYC_filename[500];
	char Output_PDB[500];

	int PROT_n_atoms;
	int *PROT_atom_num;
	char **PROT_atom_name;
	char **PROT_res_name;
	int *PROT_res_num;
	double **PROT_r2d;
	double *PROT_A;
	double *PROT_B;
	int *PROT_terminal;

	int GLYC_n_atoms;
	int *GLYC_atom_num;
	char **GLYC_atom_name;
	char **GLYC_res_name;
	int *GLYC_res_num;
	double **GLYC_r2d;
	double *GLYC_A;
	double *GLYC_B;
	int *GLYC_terminal;

/*Connect Data*/
	int i, p, q;
	double trans[3];
	int atom1, atom2, atom3, atom4, atom5, atom6;
	double d;
	double u1[3], u2[3], n[3], delta_r[3], new_delta_r[3];
	double theta, theta2;
	double omega;
	double Rot[3][3];

/*DEBUG*/
	int DEBUG;
	FILE *fp_debug;

	int CG2, CB, OG1, C1, C2, C3;
	double d_C1_OG1;
	double a_CB_OG1_C1, a_OG1_C1_C2;
	double w_CG2_CB_OG1_C1, w_CB_OG1_C1_C2, w_OG1_C1_C2_C3;


	if (argc != 16)
	{
		printf("argv[] = protein_pdb, glycan_pdb, output_pdb, (PROT) CG2,\n");
		printf("\t(PROT) CB, (PROT) OG1, (GLYC) C1, (GLYC) C2, (GLYC) C3,\n");
		printf("\td_C1_OG1, a_CB_OG1_C1, a_OG1_C1_C2, w_CG2_CB_OG1_C1, \n");
		printf("\tw_CB_OG1_C1_C2, w_OG1_C1_C2_C3 \n");
		exit(0);
	}

	DEBUG = 1;

	if (DEBUG)
	{
		fp_debug = fopen( DEBUG_FILE, "w");
    	if (fp_debug == NULL) 
		{
			printf("Cant open %s\n", DEBUG_FILE);
			exit(EXIT_FAILURE);
		}
	}

	strcpy(PROT_filename, argv[1]);
	strcpy(GLYC_filename, argv[2]);
	strcpy(Output_PDB, argv[3]);



	CG2 = atoi(argv[4]);
	CB  = atoi(argv[5]);
	OG1 = atoi(argv[6]);
	C1  = atoi(argv[7]);
	C2  = atoi(argv[8]);
	C3  = atoi(argv[9]);

	d_C1_OG1		= atof(argv[10]);
	a_CB_OG1_C1		= atof(argv[11]);
	a_OG1_C1_C2		= atof(argv[12]);
	w_CG2_CB_OG1_C1	= atof(argv[13]);
	w_CB_OG1_C1_C2	= atof(argv[14]);
	w_OG1_C1_C2_C3	= atof(argv[15]);	

	if (DEBUG)
	{
		fprintf(fp_debug, "PROT_filename = %s\n", PROT_filename);
		fprintf(fp_debug, "GLYC_filename = %s\n", GLYC_filename);
		fprintf(fp_debug, "Output_PDB = %s\n\n", Output_PDB);

		fprintf(fp_debug, "CG2 = %d\n", CG2);
		fprintf(fp_debug, "CB  = %d\n", CB);
		fprintf(fp_debug, "OG1 = %d\n", OG1);
		fprintf(fp_debug, "C1  = %d\n", C1);
		fprintf(fp_debug, "C2  = %d\n", C2);
		fprintf(fp_debug, "C3  = %d\n", C3);

		fprintf(fp_debug, "d_C1_OG1			= %f\n", d_C1_OG1);
		fprintf(fp_debug, "a_CB_OG1_C1		= %f\n", a_CB_OG1_C1);
		fprintf(fp_debug, "a_OG1_C1_C2		= %f\n", a_OG1_C1_C2);
		fprintf(fp_debug, "w_CG2_CB_OG1_C1 	= %f\n", w_CG2_CB_OG1_C1);
		fprintf(fp_debug, "w_CB_OG1_C1_C2 	= %f\n", w_CB_OG1_C1_C2);
		fprintf(fp_debug, "w_OG1_C1_C2_C3	= %f\n", w_OG1_C1_C2_C3);
	}
		





		if (DEBUG)	printf("Reading %s\n", PROT_filename);
	Read_Pdb_File (&PROT_n_atoms, &PROT_atom_num, &PROT_atom_name, &PROT_res_name,
		&PROT_res_num, &PROT_r2d, &PROT_A, &PROT_B, &PROT_terminal, PROT_filename,
		fp_debug, DEBUG);

		if (DEBUG)	printf("Reading %s\n", GLYC_filename);
	Read_Pdb_File (&GLYC_n_atoms, &GLYC_atom_num, &GLYC_atom_name, &GLYC_res_name,
		&GLYC_res_num, &GLYC_r2d, &GLYC_A, &GLYC_B, &GLYC_terminal, GLYC_filename,
		fp_debug, DEBUG);

/*Find atom1 = CG2, atom2 = CB, atom3 = OG1, atom4 = C1, atom5 = C2, atom6 = C3*/
	atom1 = -1;
	atom2 = -1;
	atom3 = -1;
	atom4 = -1;
	atom5 = -1;
	atom6 = -1;

	for (i = 0; i < PROT_n_atoms; i++)
	{
		if (PROT_atom_num[i] == CG2)
			atom1 = i;
		if (PROT_atom_num[i] == CB)
			atom2 = i;
		if (PROT_atom_num[i] == OG1)
			atom3 = i;
	}
	for (i = 0; i < GLYC_n_atoms; i++)
	{
		if (GLYC_atom_num[i] == C1)
			atom4 = i;
		if (GLYC_atom_num[i] == C2)
			atom5 = i;
		if (GLYC_atom_num[i] == C3)
			atom6 = i;
	}

	if (atom1 == -1)
	{
		printf("Could not find CB = %d in %s\n", CB, PROT_filename);
		exit(0);
	}
	if (atom2 == -1)
	{
		printf("Could not find CG2 = %d in %s\n", CG2, PROT_filename);
		exit(0);
	}
	if (atom3 == -1)
	{
		printf("Could not find OG1 = %d in %s\n", OG1, PROT_filename);
		exit(0);
	}
	if (atom4 == -1)
	{
		printf("Could not find C1 = %d in %s\n", C1, GLYC_filename);
		exit(0);
	}
	if (atom5 == -1)
	{
		printf("Could not find C2 = %d in %s\n", C2, GLYC_filename);
		exit(0);
	}
	if (atom6 == -1)
	{
		printf("Could not find C3 = %d in %s\n", C3, GLYC_filename);
		exit(0);
	}

	if (DEBUG)
	{
		fprintf(fp_debug, "\natom1 = %d, %s\n", atom1, PROT_atom_name[atom1]);
		fprintf(fp_debug, "atom2 = %d, %s\n", atom2, PROT_atom_name[atom2]);
		fprintf(fp_debug, "atom3 = %d, %s\n", atom3, PROT_atom_name[atom3]);
		fprintf(fp_debug, "atom4 = %d, %s\n", atom4, GLYC_atom_name[atom4]);
		fprintf(fp_debug, "atom5 = %d, %s\n", atom5, GLYC_atom_name[atom5]);
		fprintf(fp_debug, "atom6 = %d, %s\n", atom6, GLYC_atom_name[atom6]);
	}


/*Translate atom2 of Glycam to atom1 of the protein*/
    for (p = 0; p < 3; p++)
        trans[p] = PROT_r2d[atom3][p] - GLYC_r2d[atom4][p];

	for (i = 0; i < GLYC_n_atoms; i++)
		for (p = 0; p < 3; p++)
			GLYC_r2d[i][p] += trans[p];

	d = sqrt(dot_prod_vf (trans, trans) );
	if (d != 0.0)
		for (p = 0; p < 3; p++)
        	trans[p] *= -1.0*d_C1_OG1/d;
	else
	{
		trans[0] = -d_C1_OG1; 
		trans[1] = 0.0;
		trans[2] = 0.0;
	}

	printf("trans = %f %f %f\n", trans[0], trans[1], trans[2]);

	for (i = 0; i < GLYC_n_atoms; i++)
		for (p = 0; p < 3; p++)
			GLYC_r2d[i][p] += trans[p];


/*Rotate glycam by C1-OG1-CG2 angle*/
	for (p = 0; p < 3; p++)
	{
		u1[p] = GLYC_r2d[atom4][p] - PROT_r2d[atom3][p];
		u2[p] = PROT_r2d[atom2][p] - PROT_r2d[atom3][p];
	}

	d = sqrt(dot_prod_vf (u1, u1) );
	for (p = 0; p < 3; p++)
		u1[p] /= d;

	d = sqrt(dot_prod_vf (u2, u2) );
	for (p = 0; p < 3; p++)
		u2[p] /= d;

	theta = acos( dot_prod_vf (u1, u2) );

	/*if u1 and u2 are parallel, pick another vector that is
		not parallel to both.
	*/
	if ( (fabs(theta) < EPS) || (fabs(theta - Pi) < EPS) )
	{
		u2[0] = 1.0; u2[1] = 0.0; u2[2] = 0.0; 
		theta2 = acos( dot_prod_vf (u1, u2) );
		if ( (fabs(theta2) < EPS) || (fabs(theta2 - Pi) < EPS) )
		{
			u2[0] = 0.0; u2[1] = 1.0; u2[2] = 0.0; 
		}
	}

	cross_prod_vf( u1, u2, n);

	d = sqrt(dot_prod_vf (n, n) );
	for (p = 0; p < 3; p++)
		n[p] /= d;

	Get_Rotation_Matrix (n, -(a_CB_OG1_C1*Pi/180.0 - theta), Rot);

	for (i = 0; i < GLYC_n_atoms; i++)
	{
		for (p = 0; p < 3; p++)
			delta_r[p] = GLYC_r2d[i][p] - PROT_r2d[atom3][p];
		for (p = 0; p < 3; p++)
		{
			new_delta_r[p] = 0;
			for (q = 0; q < 3; q++)
				new_delta_r[p] += Rot[p][q]*delta_r[q];
		}
		for (p = 0; p < 3; p++)
			GLYC_r2d[i][p] = new_delta_r[p] + PROT_r2d[atom3][p];
	}


	if (DEBUG)
	{
		fprintf(fp_debug, "\n\nRotate glycam by C1-OG1-CG2 angle\n");
		fprintf(fp_debug, "u1 = %f %f %f\n", u1[0], u1[1], u1[2]);
		fprintf(fp_debug, "u2 = %f %f %f\n", u2[0], u2[1], u2[2]);
		fprintf(fp_debug, "n  = %f %f %f\n", n[0],   n[1],  n[2]);
		fprintf(fp_debug, "theta = %f, a_CB_OG1_C1 = %f\n", theta*180.0/Pi, a_CB_OG1_C1);

	for (p = 0; p < 3; p++)
	{
		u1[p] = GLYC_r2d[atom4][p] - PROT_r2d[atom3][p];
		u2[p] = PROT_r2d[atom2][p] - PROT_r2d[atom3][p];
	}

	d = sqrt(dot_prod_vf (u1, u1) );
	for (p = 0; p < 3; p++)
		u1[p] /= d;

	d = sqrt(dot_prod_vf (u2, u2) );
	for (p = 0; p < 3; p++)
		u2[p] /= d;

	theta = acos( dot_prod_vf (u1, u2) );

		fprintf(fp_debug, "\nu1 = %f %f %f\n", u1[0], u1[1], u1[2]);
		fprintf(fp_debug, "u2 = %f %f %f\n", u2[0], u2[1], u2[2]);
		fprintf(fp_debug, "new theta = %f\n", theta*180.0/Pi);

	}



/*Rotate glycam by  OG1-C1-C2 angle*/
	for (p = 0; p < 3; p++)
	{
		u1[p] = GLYC_r2d[atom5][p] - GLYC_r2d[atom4][p];
		u2[p] = PROT_r2d[atom3][p] - GLYC_r2d[atom4][p];
	}

	d = sqrt(dot_prod_vf (u1, u1) );
	for (p = 0; p < 3; p++)
		u1[p] /= d;

	d = sqrt(dot_prod_vf (u2, u2) );
	for (p = 0; p < 3; p++)
		u2[p] /= d;

	theta = acos( dot_prod_vf (u1, u2) );

	/*if u1 and u2 are parallel, pick another vector that is
		not parallel to both.
	*/
	if ( (fabs(theta) < EPS) || (fabs(theta - Pi) < EPS) )
	{
		if (DEBUG)	
			fprintf(fp_debug, "\n***theta = %f, have to use cartesian vectors for u2\n", theta);
	
		u2[0] = 1.0; u2[1] = 0.0; u2[2] = 0.0; 
		theta2 = acos( dot_prod_vf (u1, u2) );
		if ( (fabs(theta2) < EPS) || (fabs(theta2 - Pi) < EPS) )
		{
			u2[0] = 0.0; u2[1] = 1.0; u2[2] = 0.0; 
		}
	}

	cross_prod_vf( u1, u2, n);

	d = sqrt(dot_prod_vf (n, n) );
	for (p = 0; p < 3; p++)
		n[p] /= d;

	Get_Rotation_Matrix (n, -(a_OG1_C1_C2*Pi/180.0 - theta), Rot);

	for (i = 0; i < GLYC_n_atoms; i++)
	{
		for (p = 0; p < 3; p++)
			delta_r[p] = GLYC_r2d[i][p] - GLYC_r2d[atom4][p];
		for (p = 0; p < 3; p++)
		{
			new_delta_r[p] = 0;
			for (q = 0; q < 3; q++)
				new_delta_r[p] += Rot[p][q]*delta_r[q];
		}
		for (p = 0; p < 3; p++)
			GLYC_r2d[i][p] = new_delta_r[p] + GLYC_r2d[atom4][p];
	}


	if (DEBUG)
	{
		fprintf(fp_debug, "\n\nRotate glycam by C1-OG1-CG2 angle\n");
		fprintf(fp_debug, "u1 = %f %f %f\n", u1[0], u1[1], u1[2]);
		fprintf(fp_debug, "u2 = %f %f %f\n", u2[0], u2[1], u2[2]);
		fprintf(fp_debug, "n  = %f %f %f\n", n[0],   n[1],  n[2]);
		fprintf(fp_debug, "theta = %f, a_OG1_C1_C2 = %f\n", theta*180.0/Pi, a_OG1_C1_C2);

	for (p = 0; p < 3; p++)
	{
		u1[p] = GLYC_r2d[atom5][p] - GLYC_r2d[atom4][p];
		u2[p] = PROT_r2d[atom3][p] - GLYC_r2d[atom4][p];
	}

	d = sqrt(dot_prod_vf (u1, u1) );
	for (p = 0; p < 3; p++)
		u1[p] /= d;

	d = sqrt(dot_prod_vf (u2, u2) );
	for (p = 0; p < 3; p++)
		u2[p] /= d;

	theta = acos( dot_prod_vf (u1, u2) );

		fprintf(fp_debug, "\nu1 = %f %f %f\n", u1[0], u1[1], u1[2]);
		fprintf(fp_debug, "u2 = %f %f %f\n", u2[0], u2[1], u2[2]);
		fprintf(fp_debug, "new theta = %f\n", theta*180.0/Pi);

	}


/*rotate about w_CG2_CB_OG1_C1 angle*/
	Get_Torsional_Angle_vf (PROT_r2d[atom1], PROT_r2d[atom2], PROT_r2d[atom3], GLYC_r2d[atom4], 
		&omega, fp_debug, 0);

	if (DEBUG)
	{
		fprintf(fp_debug, "\n\nRotating About w_CG2_CB_OG1_C1\n");
		fprintf(fp_debug, "omega = %f\n", omega*180.0/Pi);
		fprintf(fp_debug, "w_CG2_CB_OG1_C1 = %f\n", w_CG2_CB_OG1_C1);
	}

/*rotation unit vector n is along CB_OG1 direction*/
	for (p = 0; p < 3; p++)
		n[p] = PROT_r2d[atom3][p] - PROT_r2d[atom2][p];
	d = sqrt(dot_prod_vf (n, n) );
	for (p = 0; p < 3; p++)
		n[p] /= d;

	Get_Rotation_Matrix (n, (w_CG2_CB_OG1_C1*Pi/180.0 - omega), Rot);

	for (i = 0; i < GLYC_n_atoms; i++)
	{
		for (p = 0; p < 3; p++)
			delta_r[p] = GLYC_r2d[i][p] - PROT_r2d[atom3][p];
		for (p = 0; p < 3; p++)
		{
			new_delta_r[p] = 0;
			for (q = 0; q < 3; q++)
				new_delta_r[p] += Rot[p][q]*delta_r[q];
		}
		for (p = 0; p < 3; p++)
			GLYC_r2d[i][p] = new_delta_r[p] + PROT_r2d[atom3][p];
	}

	if (DEBUG)
	{
		fprintf(fp_debug, "n  = %f %f %f\n", n[0],   n[1],  n[2]);
		Get_Torsional_Angle_vf (PROT_r2d[atom1], PROT_r2d[atom2], PROT_r2d[atom3], GLYC_r2d[atom4], 
			&omega, fp_debug, 0);
		fprintf(fp_debug, "omega = %f\n", omega*180.0/Pi);
		fprintf(fp_debug, "w_CG2_CB_OG1_C1 = %f\n", w_CG2_CB_OG1_C1);
	}


/*rotate about w_CB_OG1_C1_C2*/
	Get_Torsional_Angle_vf (PROT_r2d[atom2], PROT_r2d[atom3], GLYC_r2d[atom4], GLYC_r2d[atom5], 
		&omega, fp_debug, 0);

	if (DEBUG)
	{
		fprintf(fp_debug, "\n\nRotating About w_CB_OG1_C1_C2\n");
		fprintf(fp_debug, "omega = %f\n", omega*180.0/Pi);
		fprintf(fp_debug, "w_CB_OG1_C1_C2 = %f\n", w_CB_OG1_C1_C2);
	}

/*rotation unit vector n is along CB_OG1 direction*/
	for (p = 0; p < 3; p++)
		n[p] = GLYC_r2d[atom4][p] - PROT_r2d[atom3][p];
	d = sqrt(dot_prod_vf (n, n) );
	for (p = 0; p < 3; p++)
		n[p] /= d;

	Get_Rotation_Matrix (n, (w_CB_OG1_C1_C2*Pi/180.0 - omega), Rot);

	for (i = 0; i < GLYC_n_atoms; i++)
	{
		for (p = 0; p < 3; p++)
			delta_r[p] = GLYC_r2d[i][p] - GLYC_r2d[atom4][p];
		for (p = 0; p < 3; p++)
		{
			new_delta_r[p] = 0;
			for (q = 0; q < 3; q++)
				new_delta_r[p] += Rot[p][q]*delta_r[q];
		}
		for (p = 0; p < 3; p++)
			GLYC_r2d[i][p] = new_delta_r[p] + GLYC_r2d[atom4][p];
	}

	if (DEBUG)
	{
		fprintf(fp_debug, "n  = %f %f %f\n", n[0],   n[1],  n[2]);
		Get_Torsional_Angle_vf (PROT_r2d[atom2], PROT_r2d[atom3], GLYC_r2d[atom4], GLYC_r2d[atom5], 
			&omega, fp_debug, 0);
		fprintf(fp_debug, "omega = %f\n", omega*180.0/Pi);
		fprintf(fp_debug, "w_CB_OG1_C1_C2 = %f\n", w_CB_OG1_C1_C2);
	}


/*rotate about w_OG1_C1_C2_C3*/
	Get_Torsional_Angle_vf (PROT_r2d[atom3], GLYC_r2d[atom4], GLYC_r2d[atom5], GLYC_r2d[atom6], 
		&omega, fp_debug, 0);

	if (DEBUG)
	{
		fprintf(fp_debug, "\n\nRotating About w_OG1_C1_C2_C3\n");
		fprintf(fp_debug, "omega = %f\n", omega*180.0/Pi);
		fprintf(fp_debug, "w_OG1_C1_C2_C3 = %f\n", w_OG1_C1_C2_C3);
	}

/*rotation unit vector n is along C2_C1 direction*/
	for (p = 0; p < 3; p++)
		n[p] = GLYC_r2d[atom5][p] - GLYC_r2d[atom4][p];
	d = sqrt(dot_prod_vf (n, n) );
	for (p = 0; p < 3; p++)
		n[p] /= d;

	Get_Rotation_Matrix (n, (w_OG1_C1_C2_C3*Pi/180.0 - omega), Rot);

	for (i = 0; i < GLYC_n_atoms; i++)
	{
		for (p = 0; p < 3; p++)
			delta_r[p] = GLYC_r2d[i][p] - GLYC_r2d[atom5][p];
		for (p = 0; p < 3; p++)
		{
			new_delta_r[p] = 0;
			for (q = 0; q < 3; q++)
				new_delta_r[p] += Rot[p][q]*delta_r[q];
		}
		for (p = 0; p < 3; p++)
			GLYC_r2d[i][p] = new_delta_r[p] + GLYC_r2d[atom5][p];
	}

	if (DEBUG)
	{
		fprintf(fp_debug, "n  = %f %f %f\n", n[0],   n[1],  n[2]);
		Get_Torsional_Angle_vf (PROT_r2d[atom3], GLYC_r2d[atom4], GLYC_r2d[atom5], GLYC_r2d[atom6], 
			&omega, fp_debug, 0);
		fprintf(fp_debug, "omega = %f\n", omega*180.0/Pi);
		fprintf(fp_debug, "w_OG1_C1_C2_C3 = %f\n", w_OG1_C1_C2_C3);
	}





	Print_Combined_Pdb (PROT_n_atoms, PROT_atom_num, PROT_atom_name,
		PROT_res_name, PROT_res_num, PROT_r2d, PROT_A, PROT_B, PROT_terminal,
		GLYC_n_atoms, GLYC_atom_num, GLYC_atom_name, GLYC_res_name, 
		GLYC_res_num, GLYC_r2d, GLYC_A, GLYC_B, GLYC_terminal, Output_PDB);

	if (DEBUG)
		fclose(fp_debug);
}



void Read_Pdb_File (int *n_atoms, int **atom_num, char ***atom_name, 
	char ***res_name, int **res_num, double ***R_2D, double **A, 
	double **B, int **terminal, char *filename, FILE *fp_debug, int DEBUG)
/*
ATOM      1  H1  OME     1       5.322  16.919  10.213
ATOM      2  CH3 OME     1       5.853  15.985  10.410
*/
{
	int i;
	FILE *fp;
	char str[1000];
	char s_temp[50];

	fp = fopen( filename, "r");
    if (fp == NULL) 
	{
		printf("Cant open %s\n", filename);
		exit(EXIT_FAILURE);
	}

/*Find N_atoms*/
	(*n_atoms) = 0;
	for (;;)
	{
		if (fgets(str, sizeof(str), fp) != NULL)
		{
			sscanf(str, "%s ", s_temp);
			if (strcmp(s_temp, "ATOM") == 0)
				(*n_atoms)++;
		}
		else
			break;
	}

	if (DEBUG)
		fprintf(fp_debug, "n_atoms = %d\n", *n_atoms);
	rewind(fp);

    (*atom_num)     = I_Allocate_1D_Matrix (*n_atoms);
	(*atom_name)	= C_Allocate_2D_Matrix (*n_atoms, 10);
	(*res_name) 	= C_Allocate_2D_Matrix (*n_atoms, 10);
	(*res_num)		= I_Allocate_1D_Matrix (*n_atoms);
	(*R_2D)			= D_Allocate_2D_Matrix (*n_atoms, 3);
	(*A)			= D_Allocate_1D_Matrix (*n_atoms);
	(*B)			= D_Allocate_1D_Matrix (*n_atoms);
	(*terminal)		= I_Allocate_1D_Matrix (*n_atoms);


	i = 0;
	for (;;)
	{
		if (fgets(str, sizeof(str), fp) != NULL)
		{
			sscanf(str, "%s ", s_temp);
			if (strcmp(s_temp, "ATOM") == 0)
			{
				sscanf(str, "%*s %d %s %s %d %lf %lf %lf %lf %lf", &(*atom_num)[i],
					(*atom_name)[i], (*res_name)[i], &(*res_num)[i], &(*R_2D)[i][0], 
					&(*R_2D)[i][1], &(*R_2D)[i][2], &(*A)[i], &(*B)[i] );
				(*terminal)[i] = 0;
				i++;
			}
			else if (strcmp(s_temp, "TER") == 0)
				(*terminal)[i-1] = 1;
		}
		else
			break;
	}

	fclose(fp);
	
	if (DEBUG)
	{
		fprintf(fp_debug, "\nReading %s\n", filename);
		fprintf(fp_debug, "N_atoms = %d\n", *n_atoms);
		for (i = 0; i < (*n_atoms); i++)
		{
			fprintf(fp_debug, "\t%4d %4s %4s %4d %10.5f %10.5f %10.5f %6.2f %6.2f\n", 
                (*atom_num)[i], (*atom_name)[i], (*res_name)[i], (*res_num)[i],
                (*R_2D)[i][0], (*R_2D)[i][1], (*R_2D)[i][2],
				(*A)[i], (*B)[i]);
			if ((*terminal)[i])
				fprintf(fp_debug, "TER\n");
		}
	}
}

void Print_Combined_Pdb (int prot_n_atoms, int *prot_atom_num, char **prot_atom_name,
		char **prot_res_name, int *prot_res_num, double **prot_r2d, 
		double *prot_A, double *prot_B, int *prot_terminal, int glyc_n_atoms, 
		int *glyc_atom_num, char **glyc_atom_name, char **glyc_res_name, int *glyc_res_num, 
		double **glyc_r2d, double *glyc_A, double *glyc_B, int *glyc_terminal, char *filename)
{
	int i;
	FILE *fp;


	fp = fopen( filename, "w");
    if (fp == NULL) 
	{
		printf("Cant open %s\n", filename);
		exit(EXIT_FAILURE);
	}

/*Print Out Protein First*/
	for (i = 0; i < prot_n_atoms; i++)
	{
		fprintf(fp, "ATOM  %5d  %-3s %3s %5d    %8.3f %7.3f %7.3f %5.2f %5.2f\n", 
		    prot_atom_num[i], prot_atom_name[i], prot_res_name[i], 
			prot_res_num[i], prot_r2d[i][0], prot_r2d[i][1], prot_r2d[i][2],
			prot_A[i], prot_B[i]);
		if (prot_terminal[i])
			fprintf(fp, "TER\n");
	}

/*Print Out Glycam Portion*/
	for (i = 0; i < glyc_n_atoms; i++)
	{
		fprintf(fp, "ATOM  %5d  %-3s %3s %5d    %8.3f %7.3f %7.3f %5.2f %5.2f\n", 
		    glyc_atom_num[i] + prot_atom_num[prot_n_atoms - 1], 
			glyc_atom_name[i], glyc_res_name[i], 
			glyc_res_num[i] + prot_res_num[prot_n_atoms - 1], 
			glyc_r2d[i][0], glyc_r2d[i][1], glyc_r2d[i][2],
			glyc_A[i], glyc_B[i]);
		if (glyc_terminal[i])
			fprintf(fp, "TER\n");
	}


	fclose(fp);
}


void Get_Rotation_Matrix (double n[3], double angle, double Rot[3][3])
/*Finds a rotation matrix R which rotates about axis of rotation 
  unit vector n by angle.  The formulae for the rotation matrix 
  can be found in Goldstein's Mechanics book, Chapter 4
*/
{
	int i,j;
	
	double sn, cs, cs_1;
	double D[3][3], S[3][3], A[3][3];

	sn = -sin(angle);
	cs = cos(angle);
	cs_1 = 1 - cs;
	
	/*D is diagonal matrix*/
	for (i = 0; i <3; i++)
		for (j = i; j < 3; j++)
		{
			if (i == j)
				D[i][j] = cs;
			else
			{
				D[i][j] = 0.0;
				D[j][i] = 0.0;
			}
		}
	
	/*S is symmetric matrix*/
	for (i = 0; i <3; i++)
		for (j = i; j < 3; j++)
		{
			S[i][j] = cs_1*n[i]*n[j];
			S[j][i] = S[i][j];
		}

	/*A is antisymmetric matrix*/
	A[0][0] = 0.0;		A[0][1] = sn*n[2];	A[0][2] = -sn*n[1];
	A[1][0] = -A[0][1];	A[1][1] = 0.0;		A[1][2] = sn*n[0];
	A[2][0] = -A[0][2];	A[2][1] = -A[1][2];	A[2][2] = 0;
	
	for (i = 0; i <3; i++)
		for (j = 0; j < 3; j++)
			Rot[i][j] = D[i][j] + S[i][j] + A[i][j];	
	
}






