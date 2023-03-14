#!/bin/bash

# A correctly predicted binder could bind via 1 or more motifs.
# An incorrectly predicted non-binder would be predicted to bind by only one motif. 

if [ $# -eq 1 ]; then 
folder=$1
total=0
cor=0
ngc=0 #not grafted count
nic=0 #number of incorrect

echo "Experimental Binders"
echo "Glycan id | Number of Binding motifs" > countResults.txt
for f in `cat EXP_BINDERS.count`
do
    total=$(($total + 1))
    nm=`grep "Good" */"$folder"/stats.txt | grep -c -P " $f[a-b]*\|"`
    if [ $nm -gt 0 ]; then
        cor=$(( $cor + 1 ))
    else
        grafted=`grep -P " $f[a-b]*\|" */"$folder"/stats.txt | grep -c "."`
        if [ $grafted -eq 0 ]; then
            notgrafted[$ngc]=$f
            ngc=$(($ngc + 1))
        else
            incorlist[$nic]=$f
            nic=$(( $nic + 1 ))
        fi
    fi
    printf "   %4d   | %3d\n" "$f" "$nm" >> countResults.txt
done
printf "%d out of %d correct\n" "$cor" "$(( $total - $ngc ))"
if [ $nic -gt 0 ]; then
    echo "$nic incorrect predictions: ${incorlist[@]}"
fi
if [ $ngc -gt 0 ]; then
    echo "$ngc experimental binders did not contain any motif: ${notgrafted[@]}"
fi
echo "#######################################################################" >> countResults.txt
printf "Experimental Non-Binders\n"
echo "Incorrectly predicted to bind:" >> countResults.txt
total=0
cor=0
# Remove the a's and b's as we don't want to double count.
for f in `cat */"$folder"/grafted-IDs.txt | sed 's/a//g;s/b//g' | sort | uniq`
do
    isExpBind=`grep "^$f$" EXP_BINDERS.count`
    if [ -z $isExpBind ]; then # if it isn't an Exp binder
        total=$(($total + 1))
        # if either the a or the b is predicted to bind it's incorrect:
        isPredBind=`grep "Bad" */"$folder"/stats.txt | grep -c -P " $f[a-b]*\|"` 
        if [ $isPredBind -eq 0 ]; then # if it isn't predicted to be a binder
            cor=$(($cor + 1))
        else
            echo "$f" >> countResults.txt 
        fi
    fi
done
printf "%3d non-binders correct out of %3d\n" "$cor" "$total"
printf "For more info:\ncat countResults.txt\n"

else
    printf "Usage example ./countCorrBinders.sh 0.grafting_NOwgl\n "
fi 
