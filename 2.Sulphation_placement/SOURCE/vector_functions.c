#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#include "vector_functions.h"

#define Pi 3.141592654
#define eps .000001

double dot_prod_vf (double a[3], double b[3]);
void cross_prod_vf( double a[3], double b[3], double axb[3]);

double Calc_Distance_vf (double a[3], double b[3])
{
	int p;
	double delta_r[3];

	for (p =0 ; p < 3; p++)
		delta_r[p] = a[p] - b[p];
	return sqrt(dot_prod_vf(delta_r, delta_r) );
}
	


int Get_Torsional_Angle_vf (double R1[3], double R2[3], double R3[3], double R4[3], 
	double *omega, FILE *fp_debug, int DEBUG)
{
	int i;
	double theta1, theta2;
	double R12[3], R43[3], R32[3];
	double d12, d43, d32_2, d32, cos_omega;

	double a1[3], a2[3];
	double a1xa2[3];

/*Filter out cases of bond angles or bond distances going to 0*/
	

	if (Get_Bond_Angle_vf (R1, R2, R3, &theta1) == -1)
		return -1;

	if (Get_Bond_Angle_vf (R2, R3, R4, &theta2) == -1)
		return -1;

	if ( (fabs(theta1) < eps ) || (fabs(theta2) < eps ) )
		return -1;

	if ( (fabs(theta1 - Pi) < eps) || (fabs(theta2 - Pi) < eps) ) 
		return -1;

	for (i = 0; i < 3; i++)
	{
		R12[i] = R1[i] - R2[i];
		R43[i] = R4[i] - R3[i];
		R32[i] = R3[i] - R2[i];
	}
	
		
	d12 = sqrt (dot_prod_vf(R12, R12));
	d43 = sqrt (dot_prod_vf(R43, R43));
	d32_2 = dot_prod_vf (R32, R32);
	d32 = sqrt(d32_2);

	if (DEBUG)
	{
		fprintf(fp_debug, "R1    = %f %f %f\n", R1[0], R1[1], R1[2]);
		fprintf(fp_debug, "R2    = %f %f %f\n", R2[0], R2[1], R2[2]);
		fprintf(fp_debug, "R3    = %f %f %f\n", R3[0], R3[1], R3[2]);
		fprintf(fp_debug, "R4    = %f %f %f\n", R4[0], R4[1], R4[2]);

		fprintf(fp_debug, "d12   = %f\n", d12);
		fprintf(fp_debug, "d43   = %f\n", d43);
		fprintf(fp_debug, "d32_2 = %f\n", d32_2);
		fprintf(fp_debug, "d32   = %f\n", d32);
	}

	cos_omega = 1/(d12*d43*sin(theta1)*sin(theta2) )*(dot_prod_vf(R12, R43) - dot_prod_vf(R12, R32)*dot_prod_vf(R32, R43)/d32_2 );


	if ( fabs(cos_omega-1) < eps)
		*omega = 0.0;
	else if ( fabs(cos_omega+1) < eps)
		*omega = Pi;
	else
		*omega = acos(cos_omega);

	if (DEBUG)
	{
		fprintf(fp_debug, "cos_omega = %f\n", cos_omega);
		fprintf(fp_debug, "omega     = %f\n", (*omega) * 180/Pi);
	}

/*Now check if omega > or < 0*/
	for (i = 0; i < 3; i++)
	{
		a1[i] = R12[i] - cos(theta1)*d12/d32*R32[i];
		a2[i] = R43[i] + cos(theta2)*d43/d32*R32[i];
	}
	
	cross_prod_vf(a1, a2, a1xa2);



/*omega > 0 if a1xa2 is parallel to R23 or antiparallel to R32*/
	if (fabs(a1xa2[0]) > eps)
	{
		if (R32[0]/a1xa2[0] < 0)
			*omega *= -1;
	}
	else if (fabs(a1xa2[1]) > eps)
	{
		if (R32[1]/a1xa2[1] < 0)
			*omega *= -1;
	}
	else if (fabs(a1xa2[2]) > eps)
	{
		if (R32[2]/a1xa2[2] < 0)
			*omega *= -1;
	}
/*
printf("omega = %f\n", *omega);	
*/
	if (DEBUG)
	{
		fprintf(fp_debug, "cos_omega = %f\n", cos_omega);
		fprintf(fp_debug, "omega     = %f\n", (*omega) * 180/Pi);
	}

/*a1xa2 is 0 if omega = 0 or Pi*/
	return 0;	
}

void cross_prod_vf( double a[3], double b[3], double axb[3])
{
	axb[0] = a[1]*b[2] - a[2]*b[1];
	axb[1] = a[2]*b[0] - a[0]*b[2];
	axb[2] = a[0]*b[1] - a[1]*b[0];
}
double dot_prod_vf (double a[3], double b[3])
{	
	return (a[0]*b[0] + a[1]*b[1] + a[2]*b[2]);
}


int Get_Bond_Angle_vf (double R1[3], double R2[3], double R3[3], double *theta)
/*returns bond angle in radians*/
{
	int i;
	double R12[3], R32[3];
	double d12, d32;
	double cos_theta;

	for (i = 0; i < 3; i++)
	{
		R12[i] = R1[i] - R2[i];
		R32[i] = R3[i] - R2[i];
	}

	d12 = sqrt(dot_prod_vf(R12, R12) );
	d32 = sqrt(dot_prod_vf(R32, R32) );
		
	if ( (d12 == 0) || (d32 == 0) )
		return -1;

	cos_theta = dot_prod_vf(R12, R32)/(d12*d32);

	if (fabs(cos_theta-1.0) < eps)
		*theta = 0.0;
	else if (fabs(cos_theta+1.0) < eps)
		*theta = Pi;
	else
		*theta = acos(cos_theta);

	return 0;
}

