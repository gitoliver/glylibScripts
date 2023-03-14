#/usr/bin/env bash

# function: searches LINK records in pdbs in RESULTS_search_library folder and creates a pdb file for each of the branches (The number of branches is defined by the number of 0 residues -1.)


# PROGRAM FLOW:
# For each file in current: 
# Find the branch from the lowest core resid to ROH. call it CORE2ROH
# Break up the structure into branches starting at the 0residues and following the link cards adding residues to the branch until it hits a core sugar. 
# Only store resids that are not included in CORE2ROH. If it reaches the ROH add this branch to a CORE2ROH branch. Any branches that end in ROH will all become one branch merged with the CORE2ROH.
# Take each of these branches and rename the atoms involved in the glycosidic link to the core. Name them a1-4. 
# Move a1 to the origin, move a1-a2 vector to the x-axis, rotate the a2-a3 vector to the x-axis.
# Do this with the crystal structure complex at the same time.
# Delete the core atoms of the branch 

# OUTPUT
# A folder containing each of the branches from the core

# COUNTERS
# i is the number of disacc motifs
# j is the branch number
# k just loops the the CORE2ROH resid's to make sure there are no duplicates when merging ROH terminating branches with CORE2ROH

current=RESULTS_library_search/
rm $current/*branch* #removes previous results
source 9.MOTIFS/Snmotif.txt #allows $num (number of disacc motifs) variable to be used

for ((i=1;i<=$num;i++)); #for each disacc motif
do
	echo "motif=$i"
	source 9.MOTIFS/Smotif1_$i.txt #allows user to find motifs in each file
	source 9.MOTIFS/Smotif2_$i.txt

	find ./$current -exec grep -l "LINK.*$motif1.*$motif2" {} \; > search_results.txt

	for f in `more search_results.txt`; #for each file in FINAL_search_results
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
	
		#count the number of residues that start with a 0. This -1 is equal to the number of branches.
		cat $f | grep LINK\ | grep  "0.." > 0resids_LINK.tmp # stores 0resids link info
		cat $f | grep LINK\ | grep  -c "0.." > 0resids_count.tmp
		awk '{print"num0resids=\"",$1"\""}' 0resids_count.tmp | sed s'/\ //'g >  0resids_Scount.tmp
		source 0resids_Scount.tmp #allows num0residus counter to be used

#######################################################LOWESTCORE_ri_to_ROH_structure_creation########################################################
		#create the core2ROH structure for this libary result. Any 0resid branch that ends in an ROH will be appended to this file.
		#if smallest core resid is == 2 then don't bother as all 0resid branches will end in a core and this file will not be needed.
		#find lowest core resid and go from there to the ROH
               	if [ $core1resid -gt $core2resid ]; then
                	lowest_core_ri="$core2resid"
                else
                	lowest_core_ri="$core1resid"
		fi
                current_ri=$lowest_core_ri
		echo "lowest_core_ri=$lowest_core_ri which should be $core1resid or $core2resid"
                #loop through resids until ROH (resid1) is reached 
		if [ $lowest_core_ri -gt 2 ]; then
                       	cat $f | grep "HETATM" | grep "\ \ \ \ \ $current_ri\ \ \ " > CORE2ROH_branch.tmp
               		until [ $current_ri -eq "1" ];
                       	do
				#echo " we are finding the core2ROH branch. current_ri is $current_ri. We should keep saying this until it equals 2"
                               	cat LINK.tmp | cut -c 26-55 | grep "\ \ \ $current_ri" | cut -c 1-2 > next_current_ri.tmp
                               	awk '{print"current_ri=\"",$1"\""}' next_current_ri.tmp | sed s'/\ //'g > current_ri.tmp
                               	source current_ri.tmp # source new* connecting residue number
				CORE2ROH[$current_ri]=$current_ri
                               	cat $f | grep "HETATM" | grep "\ \ \ \ \ $current_ri\ \ \ " >> CORE2ROH_branch.tmp
                       	done
		# just for now until debug is done:
		rm add2CORE2ROH_branch_*.tmp
		#	echo "CORE2ROH"
		#	cat CORE2ROH_branch.tmp
		fi
##################################################################-----END-----#########################################################################

		for ((j=1;j<=$num0resids;j++)); # for each 0 starting branch in each pdb file in FINAL_search_results
		do
			#find the resid of the 0 residue		
			echo "###############################STARTING A BRANCH#########################################################"
			awk "NR==$j" 0resids_LINK.tmp | cut -c 55 > tmpcurrent_ri.tmp # cuts current residue number
			awk '{print"current_ri=\"",$1"\""}' tmpcurrent_ri.tmp | sed s'/\ //'g > current_ri.tmp
			source current_ri.tmp # source current residue number
			#echo "current ri of starting branch is $current_ri"
			prev_ri=$current_ri #for piece below until loop to discern if we entered this first loop
			#find the resid of the connecting residues. loop until hit core resid or ROH (resid 1)
			cat $f | grep "HETATM" | grep "\ \ \ \ \ $current_ri\ \ \ " > "$f"_motif_"$i"_branch_"$j".pdb	
			until [ $current_ri -eq $core1resid ] || [ $current_ri -eq $core2resid ] || [ $current_ri -eq "1" ];
			do
				#grep pdb info in branch file
				#echo "Entered first loop, therefore 0resid is not a core"
				#echo "First loop: current_ri=$current_ri. It should equal $core1resid or $core2resid or 1 or keep going"
				cat LINK.tmp | cut -c 26-55 | grep "\ \ \ $current_ri" | cut -c 1-2 > next_current_ri.tmp
				awk '{print"current_ri=\"",$1"\""}' next_current_ri.tmp | sed s'/\ //'g > current_ri.tmp
				prev_ri=$current_ri
				source current_ri.tmp # source new* connecting residue number
				#this loop is tricky. It makes sure that for branches that will end in an ROH duplicates won't occur when merged with the CORE2ROH branch
                                write=1
                                for ((k=1;k<$lowest_core_ri;k++));
                                       	do
						echo "CORE2ROH resid=${CORE2ROH[$k]}"
                                               	if [ ${CORE2ROH[$k]} -eq $current_ri ]; then write=0 #if the current_ri matches any of the CORE2ROH ri then don't write
                                               	fi
                                       	done
                                if [ $write -eq 1 ]; then
                                       	cat $f | grep "HETATM" | grep "\ \ \ \ \ $current_ri\ \ \ " >> "$f"_motif_"$i"_branch_"$j".pdb #store pdb info so far
                                fi
			done			
			#echo "Passed first Loop"
			#cat "$f"_motif_"$i"_branch_"$j".pdb
			if [ $prev_ri -eq $current_ri ]; then # true when we started on a core resid
				rm "$f"_motif_"$i"_branch_"$j".pdb	
				echo "Entered 2nd condition, therefore started on a core. current_ri=$current_ri. THIS BRANCH HAS BEEN REMOVED"
			else	
###########################################################-Structures which ended in a CORE-##################################################################
				if [ $current_ri -eq $core1resid ] || [ $current_ri -eq $core2resid ]; then #true only for structures which ended in a core resid
				mv "$f"_motif_"$i"_branch_"$j".pdb branch.pdb
				echo ""$f"_motif_"$i"_branch_"$j""
				echo "ENDED ON A CORE"
			#	echo "core_rn is $current_ri, should match core1resid:$core1resid or core2resid:$core2resid"
			#	echo "prev_ri is $prev_ri"
				echo $current_ri > core_rn.txt
				echo $prev_ri > branch_rn.txt
				#find the atom names of these two residues
				cat LINK.tmp | grep "\ \ $current_ri\ \ \ .*\ \ \ $prev_ri" | cut -c 14-15 > core_aN.txt
				cat LINK.tmp | grep "\ \ $current_ri\ \ \ .*\ \ \ $prev_ri" | cut -c 45-46 > branch_aN.txt
# just for debug
#cp core_aN.txt core_aN_"$j".txt
#cp branch_aN.txt branch_aN_"$j".txt
#cp core_rn.txt core_rn_"$j".txt
#cp branch_rn.txt branch_rn_"$j".txt
				/home/oliver/Dropbox/1.glylib_scripts/1.Automate_virtual_array/1.search_library/rename_superimpose.exe
				echo "$f"_motif_"$i"_branch_"$j"
				#cp superimposed_cplx.pdb "$f"_branch_"$j"_superimposed_cplx.pdb
				#cp superimposed_branch.pdb "$f"_branch_"$j"_superimposed_branch.pdb
				cat superimposed_branch.pdb | grep -v "\ \ \ $current_ri\ \ " > superimposed_branch_pruned_tmp.pdb
	                        cat superimposed_branch_pruned_tmp.pdb | grep -v "\ c9\ " > superimposed_branch_pruned.pdb
                        	cat superimposed_cplx.pdb >> superimposed_branch_pruned.pdb
				mv superimposed_branch_pruned.pdb "$f"_motif_"$i"_branch_"$j".pdb #now that renaming is done
				#rm branch.pdb
				fi
##################################################################-----END-----##############################################################################
			fi
##########################################################-Structures which ended in a ROH-#####################################################################
			#for structures with no core residue add the CORE2ROH pdb. 
			if [ $current_ri -eq "1" ]; then #i.e if 0.resid-end stopped at an ROH instead of a core.
				echo "Contains 0.resid branch which terminated in a ROH"
				#append the main core2ROH branch to this structure and remove atom
				echo "none of these should be in core2ROH"
				cat "$f"_motif_"$i"_branch_"$j".pdb
				cat "$f"_motif_"$i"_branch_"$j".pdb >> add2CORE2ROH_branch_"$j".tmp
				rm "$f"_motif_"$i"_branch_"$j".pdb
			fi
##################################################################-----END-----##############################################################################
echo "########################################################## FINISHED BRANCH ############################################################################"
		done

#################################### DEAL with all ROH ending structures #######################################################################################
#This happens only once for each library search result:
		if [ $lowest_core_ri -gt 2 ]; then
			cat add2CORE2ROH_branch*.tmp > "$f"_motif_"$i"_branch_0.txt
			cat CORE2ROH_branch.tmp >> "$f"_motif_"$i"_branch_0.txt # ROH has to be at EOF
			#call a C function which eliminates duplicates
			mv "$f"_motif_"$i"_branch_0.txt branch.pdb
			echo ""$f"_motif_"$i"_branch_"0""
			echo ""DEALING WITH ROH ENDING BRANCHES""
			echo "3core1resid=$core1resid"
			echo "3core2resid=$core2resid"
			echo "lowest_core_ri=$lowest_core_ri"
			#set core_ri to the lowest core
			core_ri=$lowest_core_ri
			#find next sacc
			cat LINK.tmp | cut -c 26-55 | grep "\ \ \ $core_ri" | cut -c 1-2 > tmpnext_ri.tmp
                        awk '{print"next_ri=\"",$1"\""}' tmpnext_ri.tmp | sed s'/\ //'g > next_ri.tmp
                        source next_ri.tmp
			echo "core_ri2 is $core_ri"
			echo "branch_ri is $next_ri"
                        echo $core_ri > core_rn.txt
                        echo $next_ri > branch_rn.txt
                        cat LINK.tmp | grep "\ \ $next_ri\ \ \ .*\ \ \ $core_ri" | cut -c 14-15 > branch_aN.txt
                        cat LINK.tmp | grep "\ \ $next_ri\ \ \ .*\ \ \ $core_ri" | cut -c 45-46 > core_aN.txt
                        /home/oliver/Dropbox/1.glylib_scripts/1.Automate_virtual_array/1.search_library/rename_superimpose.exe
			 echo "$f"_motif_"$i"_branch_"$j"
                        #cp superimposed_cplx.pdb "$f"_branch_0_superimposed_cplx.pdb
                        #cp superimposed_branch.pdb "$f"_branch_0_superimposed_branch.pdb
			# Delete core atoms of branch
			cat superimposed_branch.pdb | grep -v "\ \ \ $core_ri\ \ " > superimposed_branch_pruned_tmp.pdb
			cat superimposed_branch_pruned_tmp.pdb | grep -v "\ c9\ " > superimposed_branch_pruned.pdb
			cat superimposed_cplx.pdb >> superimposed_branch_pruned.pdb
                        cp superimposed_branch_pruned.pdb "$f"_motif_"$i"_branch_0.pdb #now that renaming is done
			# rm branch.pdb
		fi
#############################################################################################################################################################
		
	done
done
rm *.tmp #*.txt
echo "finished"
 #			awk "NR==$i" 0resids_LINK.tmp | cut -c 49-51 > current_rN.tmp #cut the 0residue name 
  #                      awk '{print"0_rN=\"",$3"\""}' 0resids_LINK.tmp | sed 's/\ //g' > 0_rN.tmp #source the 0residue name
   #                     source 0_rN.tmp
