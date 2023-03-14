#/usr/bin/env bash

#function: searches LINK records in pdbs in RESULTS_search_library folder and creates a pdb file for each of the branches (The number of branches is defined by the number of 0 residues -1.)


#Code: 
#for this pdb find resids of core:
#source each Smotif1_$i.txt.
#grep core residue names and store resids as variables to check against later.

#For each 0 residue:
#1.find connecting residue in LINK. f
#2.Add resid to list. 
#3.if not core resid or ROH loop to #1. 
#3.else create branch from list 


#output
#a folder containing each of the branches from the core

current=complex/
source 9.MOTIFS/Snmotif.txt #allows $num (number of disacc motifs) variable to be used

for ((i=1;i<=$num;i++)); #for each disacc motif
do
	echo "$i=i"
	source 9.MOTIFS/Smotif1_$i.txt #allows user to find motifs in each file
	source 9.MOTIFS/Smotif2_$i.txt

	find ./$current -exec grep -l "LINK.*$motif1.*$motif2" {} \; > search_results.txt

	for f in `more search_results.txt`; #for the file in current
	do 
		echo "starting $f"
		grep -h "LINK\ " $f > LINK.tmp #store all link info for this structre
		grep -h "LINK\ .*$motif1.*$motif2" $f > LINK_core.tmp #store core info
		#get resids of core residues in this library structure
		awk '{print"core1resid=\"",$4"\""}' LINK_core.tmp > tmpcore1resid.tmp # 
		cat tmpcore1resid.tmp | sed 's/\ //g' > core1resid.tmp #removes the whitespace. one step later
		source core1resid.tmp # now can use $core1resid variable 
		#repeat for residue2 of core
		awk '{print"core2resid=\"",$7"\""}' LINK_core.tmp | sed 's/\ //g' > core2resid.tmp
		source core2resid.tmp # now can use $core2resid variable
	
		echo "calling rename atoms"
		echo "core1resid is $core1resid and core2resid:$core2resid"
		echo  $core1resid > core_rn.txt
		echo  $core2resid > branch_rn.txt
		#find the atom names of these two residues
		cat LINK.tmp | grep "\ \ $core1resid\ \ \ .*\ \ \ $core2resid" | cut -c 14-15 > core_aN.txt
		cat LINK.tmp | grep "\ \ $core1resid\ \ \ .*\ \ \ $core2resid" | cut -c 45-46 > branch_aN.txt
# just for debug
#cp core_aN.txt core_aN_"$j".txt
#cp branch_aN.txt branch_aN_"$j".txt
#cp core_rn.txt core_rn_"$j".txt
#cp branch_rn.txt branch_rn_"$j".txt
		cp $f branch.pdb				
		/home/oliver/Dropbox/1.glylib_scripts/1.Automate_virtual_array/1.search_library/rename_atoms.exe
		#call superimpose function?
		mv superimposed.pdb "$f"_motif_"$i"_aligned.txt #now that renaming is done
	done
done
