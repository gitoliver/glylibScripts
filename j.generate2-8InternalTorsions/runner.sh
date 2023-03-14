#!/bin/bash
infolder=$1
link=$2
outfolder=$3

mkdir $outfolder

ls $infolder | sed 's/\.pdb//g' > inputs.txt
for prefix in `cat inputs.txt`
do
    echo "doing $prefix"
    cp $infolder/$prefix.pdb .
    2.create_2-8_torsions.sh $outfolder $prefix $link
    gems $prefix.js
    rm $prefix.pdb
    rm $prefix.js
done
rm inputs.txt
