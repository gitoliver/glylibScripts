//Function call rather than stand alone.

#define CUTOFF=1.0
void wiggler(assembly *A, assembly *B, int window_size, size_limit, int *anchor_resids, int clash_total);

/* Nice idea but fuck it. Allocating memory and passing into functions is hell.
struct clash_branch{
int branch_resids[] // store size at 0 and core/anchor attachment residue at 1
int clash_resids[]
int branch_points[]
int clash_linkage
double current_clash
}
*/

//Take in receptor(B) and ligand(A) and wiggle window size. So 15 degrees limit is hard-coded and can check every 5 or 1 degree out.
//Take in resid of core/anchor not to be touched.

//Find branch clash is on and fill in clash_branch details.
clash=find_vdw_clashes_return_linkage(assembly A, assembly B, *linkage)

// this is to decide whether to call this wiggler or not
// Need to know distance from linkage. Just subtract coords
if clash/dist_from_linkage_atom < CUTOFF //clash normalized to distance from anchor. Big clash too close is worse than big clash far.

for (i=clash_linkage, i>branch_resids[0], i--) // (Store size of array at 0)

	// Manipulate assembly so function returns the "best" structure.
	Tor_result=generate_torsion_window_permutations(window_size, size_limit, assembly *A, i){ // i is the class linkage, change clash_linkage

	if (Tor_result==9){
		// we have resolved the clash
		// quit
		i=branch_resids[1];
	}
	if (Tor_result==1){
		// we have improved the clash
		if (i==clash_linkage){ i--;} // if at the start or clash_linkage hasn't changed
		else { i=clash_linkage; } // restart at clash point. Either it is the same and something lower has changed, or have new clash resid further out.
	}
	if (Tor_result==0){
		// the clash is the same/linkage is unchanged
		i--; // move on to next linkage
	}

	if now at branch point{
		Tor_result = generate_torsion_window_permutations()
		

	
// ALL PERMUTATIONS METHOD:
// Create every permutation. With 15 window size would be 6 permutations per linkage. With 5 linkages: 7,776 structures, with 10 linkages then 60 million. Not feasable.

// LEAST PERTUBATION ETHOS:
// Fix with the least pertubation of the linkages.
// Also want to fix with linkage pertubation as close to clash as possible? Yes and if cannot fix move away towards core, create all permutations and keep best structure?. If hit branch point wiggle it:
// If it fixes clash and causes no other clash fine
// If it fixes clash but causes clash in other branch go to new clash point, and wiggle down towards branch point
	//if cannot fix go back and reject original branch point wiggle
// Try another wiggle at the branch point
// If wiggle at branch point does not fix it carry on towards core. Will have at least two branches in the stack. 
// Check each branch. Keep best structure overall and go and wiggle down from each clash point
// If can't get any better move closer to core.
//
// If at any stage have improved clash score at a linkage go back to clash point/new clash point and start again. 
// If have two on the stack go down each one.


// FAST ETHOS:
// Start at core? If improve but not fix move on... I need to work this out.
