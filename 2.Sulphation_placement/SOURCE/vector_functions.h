void cross_prod_vf( double a[3], double b[3], double axb[3]);
double dot_prod_vf (double a[3], double b[3]);
int Get_Bond_Angle_vf (double R1[3], double R2[3], double R3[3], double *theta);
int Get_Torsional_Angle_vf (double R1[3], double R2[3], double R3[3], double R4[3], 
	double *omega, FILE *fp_debug, int DEBUG);

double Calc_Distance_vf (double a[3], double b[3]);
