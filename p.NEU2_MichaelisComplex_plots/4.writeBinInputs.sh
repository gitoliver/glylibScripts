#!/bin/bash
source 0.INPUTS_EDIT_ME

i=$number_residues

cd dihedrals/
for ((i=$number_residues; i>2; i--))
do
        j=$(($i-1))
        mkdir linkage-"$i"-"$j"
        cat ../ptraj.in | grep "$i"-"$j" > "$i"-"$j"ptraj.in
        mv "$i"-"$j"* linkage-"$i"-"$j"
        echo ""$i"-"$j" linkage" >> ../RESULTS.txt
        cd linkage-"$i"-"$j"
        mkdir dihedrals/
        > BinInputs"$i"-"$j".in
        file=`ls *.csv | grep *phi*`; echo $file $phi >> BinInputs"$i"-"$j".in
        file=`ls *.csv | grep *psi*`; echo $file $psi >> BinInputs"$i"-"$j".in
        file=`ls *.csv | grep *ome7*`; echo $file $ome7 >> BinInputs"$i"-"$j".in
        file=`ls *.csv | grep *ome8*`; echo $file $ome8 >> BinInputs"$i"-"$j".in
        file=`ls *.csv | grep *ome9*`; echo $file $ome9 >> BinInputs"$i"-"$j".in
        bin_MD-states.pl BinInputs"$i"-"$j".in OUT_"$i"-"$j".dat
        sort -nr -k 2 OUT_"$i"-"$j".dat -o OUT_"$i"-"$j".dat
        find_states_above_cutoff.sh OUT_"$i"-"$j".dat
        cd ../
done
echo "Finished. Results are in RESULTS.txt"
