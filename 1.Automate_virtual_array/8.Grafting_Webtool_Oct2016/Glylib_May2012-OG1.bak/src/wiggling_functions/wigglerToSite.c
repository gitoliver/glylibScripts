#include <glylib.h> // required for load_pdb function
#include <mylib.h>
#include <molecules.h>

//int main (void){
double wigglerToSite(assembly *A, assembly *B, int *core_ri, int wiggleWindow, int current_resid, int target_resid){

int window_size=wiggleWindow; //What the fuck? Same thing? 
int branch[(*A).nr]; // stores which residues are in the branch
branch[0]=0; // stores number of residues in branch
int linkage_resid=0,linked2=0; // linkage is resid being wiggled.
int struct_total=10000; // id for structures generated so far. loads into vmd in order when starting at 10000
int i=1; // 
int y=0;
int lna;
int tors=0;// dummy for the first call, set in generate_torsion...
int stop=0;
dihedral_coord_set *DCS;
DCS=(dihedral_coord_set*)calloc(8, sizeof(dihedral_coord_set));
//DCS=(dihedral_coord_set*)realloc(DCS, 8*sizeof(dihedral_coord_set)); // space for 8
double dist=1000,prev_dist=1000;
double lowest_clash=0.0; 
int residsToIgnore[3]; 
residsToIgnore[0]=2;
residsToIgnore[1]=target_resid;
residsToIgnore[2]=(target_resid-1);
lowest_clash=find_vdw_clashes_return_totalToSite(A,B,residsToIgnore);



while ( (dist>3) && (branch[0]>=0) && (struct_total<15000) && (stop==0) ) {
    dist=get_avr_dist_btwn_res(A,B,current_resid,target_resid);
//    printf("dist=%2f\n",dist);
    //for(y=2;y<=core_ri[0];y++){ //The converted ROH residue will be in core_ri[1]. Any residue attached there needs to be wiggled.
    //    printf("core_ri[%d]=%d\n",y,core_ri[y]);
	   // if(current_resid==core_ri[y]){
	//	stop=1;
        //}
    //} // if current_resid has already become a core then stop
    find_path_to_coreToSite(A, core_ri, current_resid, branch);
    printf("coreri=%d,wiggledRi=%d\n",branch[1],branch[2]);
    printf("Starting wiggling\n");
    //  printf("branch[%d]=%d,branch[%d+1]=%d\n",i,branch[i],i+1,branch[i+1]);
    linkage_resid=branch[2]; // this will be rotated branch
    linked2=branch[1]; // this is the core
    printf("Will find which atoms are involved in linkage\n");
    lna=find_connection_atomsToSite(linkage_resid, linked2, DCS, A);
    tors=(lna-3);
    printf("lna=%d,tors=%d,branch[%d]=%d\n",lna,tors,i,branch[i]);

    // if tors>0 so won't try to wiggle OME or SO3. Can't atm. 
    if (tors>0){
        printf("Will generate torsions\n");
        prev_dist=dist;
        generate_torsion_window_permutationsToSite(window_size,A,B,tors,&struct_total,&dist,linkage_resid,DCS,lna,linked2,current_resid,target_resid,&lowest_clash);
        printf("If previous dist(%f) is greater than dist(%f) then keep new structure\n",prev_dist,dist);
        if (prev_dist>(dist+0.1)){
            keep_best_structureToSite(A,B,&dist,current_resid,target_resid);    
        }
    }
    i++;
    // Add wiggled bits to core. They won't be wiggled again.
    core_ri[0]=((core_ri[0])+1);
    core_ri[core_ri[0]]=linkage_resid;
    printf("current_resid=%d\n",current_resid);
    for (y=1;y<core_ri[0];y++){
        printf("core_ri[%d]=%d\n",y,core_ri[y]);
        if(current_resid==core_ri[y]){
            printf("Current_resid:%d has become part of core_ri[%d]:%d\n",current_resid,y,core_ri[y]);
            stop=1;
        }
    }
    printf("clash=%3f,dist=%3f,i=%d,branch[0]=%d,struct_total=%d,stop=%d\n",lowest_clash,dist,i,branch[0],struct_total,stop);
}
return dist;
}
