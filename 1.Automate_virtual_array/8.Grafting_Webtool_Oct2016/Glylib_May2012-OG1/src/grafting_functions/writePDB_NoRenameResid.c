/* Author: Michael Tessier*/
#include <molecules.h>
#include <mylib.h>
//#include "../inc/molecules.h"
//#include "../inc/mylib.h"
//
#define TRUE  1
#define FALSE 0
// Edited by OG 15June2012
char* spacingOly(int strLen, int totLen);

int outputMolPDB_NoRenameResid(molecule* mol, char* file_name)
{
 //Tells the user what's going on
// printf("Writting file: %s...\n",file_name);
 FILE* file;
 char current_line[80] = ""; char temporary[40] = "";
 char* curLine = current_line; char* temp = temporary;
 char* resName; char* atmName;
 char olytemporary[40] = ""; char* olyTemp = olytemporary;
 int i; int j; int atmTot; int resNum; int resTot = (*mol).nr;
 atom curAtm;
 file = myfopen(file_name,"w");
 // OG first get sorted list by resid .n
 for(i = 0; i < resTot; i++)
 {
  resName = (*((*mol).r+i)).N;
  resNum = (*((*mol).r+i)).n;
  atmTot = (*((*mol).r+i)).na;
  //printf("atmTot=%d,resName=%s,resNum=%d\n",atmTot,resName,resNum);
  for(j = 0; j < atmTot; j++)
  {
   curAtm = (*((*((*mol).r+i)).a+j));
   atmName = curAtm.N;
   strcpy(curLine,"");
   strcat(curLine,"ATOM");
   sprintf(temp,"%d", curAtm.n);
   //printf("curLine=%s,curAtm=%d\n",curLine,curAtm.n);
   //Based on how large the atom # is, depends on the # of spaces
   olyTemp=spacing(strlen(temp),7);
   sprintf(curLine,"%s%s",curLine,olyTemp); // Was getting occasional, weird out of bounds shit on the first ATOM with the strcat.
   //strcat(curLine,spacing(strlen(temp),7));
   //"ATOM atom# atom_name"
   sprintf(temp,"%d  %s", curAtm.n, atmName);
   strcat(curLine,temp);
   
   //Based on how many char are in the residue name, "   " spaces 
   strcat(curLine,spacing(strlen(atmName),4));
   //"ATOM atom# atom_name res_name"
   strcat(curLine,resName);
   
   sprintf(temp,"%d",resNum);
   //Based on how large the residue # is, "              " spaces
   strcat(curLine,spacing(strlen(temp),6));
   //"ATOM atom# atom_name res_name res#"
   strcat(curLine,temp);
   
   sprintf(temp,"%.3lf",curAtm.x.i);
   //Based on how large the x cordinate is, "            " spaces
   strcat(curLine,spacing(strlen(temp),12));
   //"ATOM atom# atom_name res_name x"
   strcat(curLine,temp);

   sprintf(temp,"%.3lf",curAtm.x.j);
   //Based on how large the y cordinate is, "            " spaces
   strcat(curLine,spacing(strlen(temp),8));
   //"ATOM atom# atom_name res_name x y"
   strcat(curLine,temp);

   sprintf(temp,"%.3lf",curAtm.x.k);
   //Based on how large the z cordinate is, "            " spaces
   strcat(curLine,spacing(strlen(temp),8));
   //"ATOM atom# atom_name res_name x y z 1.00 0.00"
   sprintf(temp,"%.3lf  1.00  0.00",curAtm.x.k);
   strcat(curLine,temp);

   //Output the line to the new .pbd file
   fprintf(file,"%s\n",curLine);
  }
 }
 fclose(file);
 return 0;
}
int outputAsmblPDB_NoRenameResid(assembly* asmbl, char* file_name)
{
 //Tells the user what's going on
 //printf("Writting file: %s...\n",file_name);
 FILE* file;
 char current_line[80] = ""; char temporary[40] = "";
 char* curLine = current_line; char* temp = temporary;
 char* resName; char* atmName;
 char olytemporary[40] = ""; char* olyTemp = olytemporary;
 int i,j,k,atmTot,resNum,resTot;
 atom curAtm; molecule* mol;
 file = myfopen(file_name,"w");
 for(k = 0; k < (*asmbl).nm; k++)
 {
  mol = ((*asmbl).m+k)[0]; // changed by BLF on 20080622 -- might need revisiting
  resTot = (*mol).nr;
  for(i = 0; i < resTot; i++)
  {
   resName = (*((*mol).r+i)).N;
   resNum = (*((*mol).r+i)).n;
   atmTot = (*((*mol).r+i)).na;
   //printf("atmTot=%d,resName=%s,resNum=%d\n",atmTot,resName,resNum);
   for(j = 0; j < atmTot; j++)
   {
    curAtm = (*((*((*mol).r+i)).a+j));
    atmName = curAtm.N;
    strcpy(curLine,"");
    strcat(curLine,"ATOM"); 
    sprintf(temp,"%d", curAtm.n);
    //Based on how large the atom # is, depends on the # of spaces
    olyTemp=spacing(strlen(temp),7);
    sprintf(curLine,"%s%s",curLine,olyTemp); // Was getting occasional, weird out of bounds shit on the first ATOM with the strcat.
    //strcat(curLine,spacing(strlen(temp),7); // commented out and replaced with sprintf line and olyTemp line directly above.
    //"ATOM atom# atom_name"
    sprintf(temp,"%d  %s", curAtm.n, atmName);
    strcat(curLine,temp);
    //Based on how many char are in the residue name, "   " spaces 
    strcat(curLine,spacing(strlen(atmName),4));
    //"ATOM atom# atom_name res_name"
    strcat(curLine,resName);
    sprintf(temp,"%d",resNum);
    //Based on how large the residue # is, "              " spaces
    strcat(curLine,spacing(strlen(temp),6));
    //"ATOM atom# atom_name res_name res#"
    strcat(curLine,temp);
    
    sprintf(temp,"%.3lf",curAtm.x.i);
    //Based on how large the x cordinate is, "            " spaces
    strcat(curLine,spacing(strlen(temp),12));
    //"ATOM atom# atom_name res_name x"
    strcat(curLine,temp);
 
    sprintf(temp,"%.3lf",curAtm.x.j);
    //Based on how large the y cordinate is, "            " spaces
    strcat(curLine,spacing(strlen(temp),8));
    //"ATOM atom# atom_name res_name x y"
    strcat(curLine,temp);

    sprintf(temp,"%.3lf",curAtm.x.k);
    //Based on how large the z cordinate is, "            " spaces
    strcat(curLine,spacing(strlen(temp),8));
    //"ATOM atom# atom_name res_name x y z 1.00 0.00"
    sprintf(temp,"%.3lf  1.00  0.00",curAtm.x.k);
    strcat(curLine,temp);

    //Output the line to the new .pbd file
    fprintf(file,"%s\n",curLine);
    //printf("curLine=%s\n",curLine);
   }
  // fprintf(file,"TER   \n");
  }
  fprintf(file,"TER   \n");
 }
 rewind(file);
 fclose(file);
 return 0;
}

int outputAsmblPDB_NoRenameResidTER(assembly* asmbl, char* file_name)
{
 //Tells the user what's going on
 //printf("Writting file: %s...\n",file_name);
 FILE* file;
 char current_line[80] = ""; char temporary[40] = "";
 char* curLine = current_line; char* temp = temporary;
 char* resName; char* atmName;
 int s,h,i,j,k,atmTot,resNum,resTot;
 atom curAtm; molecule* mol;
 file = myfopen(file_name,"w");
 for(k = 0; k < (*asmbl).nm; k++)
 {
  mol = ((*asmbl).m+k)[0]; // changed by BLF on 20080622 -- might need revisiting
  resTot = (*mol).nr;
  int sorted[resTot];
  // OG first get sorted list by resid .n
  for(h = 0; h < resTot; h++)
  {
   resNum = (*((*mol).r+h)).n;
   sorted[resNum] = h;
  }
  for(i = 1; i <= resTot; i++)
  {
   s = sorted[i]; //s is first the index of resid 1, then resid 2 and so on 
   resName = (*((*mol).r+s)).N;
   resNum = (*((*mol).r+s)).n;
   atmTot = (*((*mol).r+s)).na;
   for(j = 0; j < atmTot; j++)
   {
    curAtm = (*((*((*mol).r+s)).a+j));
    atmName = curAtm.N;
    strcpy(curLine,"");
    strcat(curLine,"ATOM");

    sprintf(temp,"%d", curAtm.n);
    //Based on how large the atom # is, depends on the # of spaces
    strcat(curLine,spacing(strlen(temp),7));
    //"ATOM atom# atom_name"
    sprintf(temp,"%d  %s", curAtm.n, atmName);
    strcat(curLine,temp);

    //Based on how many char are in the residue name, "   " spaces 
    strcat(curLine,spacing(strlen(atmName),4));
    //"ATOM atom# atom_name res_name"
    strcat(curLine,resName);

    sprintf(temp,"%d",resNum);
    //Based on how large the residue # is, "              " spaces
    strcat(curLine,spacing(strlen(temp),6));
    //"ATOM atom# atom_name res_name res#"
    strcat(curLine,temp);

    sprintf(temp,"%.3lf",curAtm.x.i);
    //Based on how large the x cordinate is, "            " spaces
    strcat(curLine,spacing(strlen(temp),12));
    //"ATOM atom# atom_name res_name x"
    strcat(curLine,temp);

    sprintf(temp,"%.3lf",curAtm.x.j);
    //Based on how large the y cordinate is, "            " spaces
    strcat(curLine,spacing(strlen(temp),8));
    //"ATOM atom# atom_name res_name x y"
    strcat(curLine,temp);

    sprintf(temp,"%.3lf",curAtm.x.k);
    //Based on how large the z cordinate is, "            " spaces
    strcat(curLine,spacing(strlen(temp),8));
    //"ATOM atom# atom_name res_name x y z 1.00 0.00"
    sprintf(temp,"%.3lf  1.00  0.00",curAtm.x.k);
    strcat(curLine,temp);

    //Output the line to the new .pbd file
    fprintf(file,"%s\n",curLine);
   }
   fprintf(file,"TER   \n");
  }
  //fprintf(file,"TER   \n");
 }
 rewind(file);
 fclose(file);
 return 0;
}
/*char* spacing(int strLen, int totLen)
{
 char returning[15] = ""; char* ret = returning;
 int i;
 for(i = 0; i < totLen - strLen; i++)
  strcat(ret," ");
// printf("ret=\"%s\"\n",ret);
 return ret;
}*/
