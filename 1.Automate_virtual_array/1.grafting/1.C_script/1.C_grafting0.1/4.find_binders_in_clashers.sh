#!/bin/bash

	################################################################
	#Script written by: Oliver Grant olivercgrant@gmail.com 6Feb12 #
	#Script designed to generate output from grafting for excel    #
	#Finds total number of binders and clashers for each unique    #
	# glycan ID in BINDERS/TOTAL.txt. Reports budcore number too   #
	#As there may be more than one glycan sequence within a glycan #
	# that matches the core, i.e. multiple Neu5Ac's, each one gets #
	# grafted and assigned a budcore number based on the resid at  #
	# the non-reducing term of the branch.                         #
	################################################################

#Run from grafting folder. Move into results folder to do this.
dir=Clashes/2.total

#Put Binders and Clashers results into one file so can search 
cp $dir/CLASHERS/TOTAL.txt BINDERS_CLASHERS.tmp
cat $dir/BINDERS/TOTAL.txt >> BINDERS_CLASHERS.tmp

#Copy GLYCANS.txt putting a _ in front of the id's so can search for 1904 and not get 21904
rm tmp.txt 2>/dev/null 
cat GLYCANS.txt | sed 's/\t/ /g' | sed 's/ \+/_/g' > GLYCANS1.tmp # replace tab with space. Replace all spaces with a single _
for f in `cat GLYCANS1.tmp` ; do echo "_" >> tmp.txt ; done
paste -d "" tmp.txt GLYCANS1.tmp > GLYCANS2.tmp; #  add a _ in from of each id

#Add a and b to end of ids in EXP_BINDERS where nesscessary as indicated in GLYCANS2.tmp
cat EXP_BINDERS.txt > EXP_BINDERS.tmp
for f in `cat  EXP_BINDERS.txt`; do grep "\_$f[ab]\_" GLYCANS2.tmp | cut -d_ -f 2; done >> EXP_BINDERS.tmp

#Create header
echo -e "Budcore is resid number. Count residues back from OH to find the residue in the binding site" > RESULTS.txt
echo -e "Glycome\tBudcore\tNumber\tNumber\tExp" >> RESULTS.txt
echo -e "ID\tNumber\tClasher\tBinder\tBinder\tRBP\tSp\tSequence" >> RESULTS.txt

echo -e "Budcore is resid number. Count residues back from OH to find the residue in the binding site" > RESULTS_BINDERS.txt
echo -e "Glycome\tExperimental" >> RESULTS_BINDERS.txt
echo -e "ID\tBinder\tRBP\tSequence" >> RESULTS_BINDERS.txt

echo -e "Budcore is resid number. Count residues back from OH to find the residue in the binding site" > RESULTS_CLASHERS.txt
echo -e "Glycome\tExperimental" >> RESULTS_CLASHERS.txt
echo -e "ID\tBinder\tSequence" >> RESULTS_CLASHERS.txt

#get list of unique ID numbers that occur in BINDERS.txt

cat $dir/CLASHERS/TOTAL.txt >  combined_total.tmp
cat $dir/BINDERS/TOTAL.txt >> combined_total.tmp

cat combined_total.tmp | cut -d \/ -f 4 | cut -d - -f 1 | sort -u | sort -n | grep -v atom_equivalent > UNIQ_IDS.tmp

echo -e "Sorting results.\r"
for f in `cat UNIQ_IDS.tmp`; 
do 
	echo -e "Doing $f. \r"
	EB="N" #reset to No
	clasher="Y"
	grep "2.total/BINDERS/$f-" $dir/BINDERS/TOTAL.txt > BINDERS_$f.tmp
	grep "2.total/CLASHERS/$f-" $dir/CLASHERS/TOTAL.txt > CLASHERS_$f.tmp
	cat BINDERS_$f.tmp > BINDERS_CLASHERS_$f.tmp
	cat CLASHERS_$f.tmp >> BINDERS_CLASHERS_$f.tmp
	# g is the name of one of the rotamers. May have multiple budcores.
	g=`cat BINDERS_CLASHERS_$f.tmp | cut -d . -f 2 | head -1`
	#Find the number of budcores there will be per rotamer. for each of these budcores...	
	for i in `grep $f- BINDERS_CLASHERS.tmp | grep $g | cut -d . -f 3 | cut -d _ -f 3`
	do
		clashers=`grep -c budcore_$i. CLASHERS_$f.tmp`; 
		binders=`grep -c budcore_$i. BINDERS_$f.tmp`;
		#Only report from budcores that fit in the binding site. I.e binders > 0
		if [ $binders != 0 ]; then
			clasher="N"
			for j in `cat EXP_BINDERS.tmp`;
			do
				if [ $f == $j ]; then
					EB="Y"
				fi
			done
			# go through rotamers and find rotamer with fewest residues below plane(rbp).
			rbp=99999; # set to a high number initially
			for k in `cat BINDERS_$f.tmp | cut -d = -f 3` 
			do 
				if [ $k -lt $rbp ] ; then 
					rbp=$k; 
				fi 
			done 
			seq=`grep -e "_$f\_" GLYCANS2.tmp | cut -d _ -f 3`
                        spacer=`grep "$f-" ID-Spacers.txt | cut -f2`
			echo -e "$f\t$i\t$clashers\t$binders\t$EB\t$rbp\t$spacer\t$seq" >> RESULTS.txt
		fi
	done
	
	for j in `cat EXP_BINDERS.tmp`;
        do
               	if [ $f == $j ]; then # if it's a clasher but ID is in EXP_BINDERS
			EB="Y"	
        	fi
        done
	seq=`grep -e "_$f\_" GLYCANS2.tmp | cut -d _ -f 3`
	if [ $clasher == "Y" ]; then
		echo -e "$f\t$EB\t$seq" >> RESULTS_CLASHERS.txt
	fi
	if [ $clasher == "N" ]; then
                echo -e "$f\t$EB\t$rbp\t$seq" >> RESULTS_BINDERS.txt
        fi

done


rm *.tmp #clean up
echo "done"
