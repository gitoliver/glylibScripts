#!/bin/bash

ls pdb_Files/ | sed 's/\.pdb//g' > list.txt

for f in `cat list.txt`;
do
    isInternal=`grep LINK pdb_Files/$f.pdb | grep -c "8S.*8S."`
    if [ $isInternal -gt 0 ]; then
        grep LINK pdb_Files/$f.pdb | grep "O8" | sed -n '1p' | cut -c26 > tmp.txt
        res=`cat tmp.txt`
        num_links=`grep LINK pdb_Files/$f.pdb | grep -c "O8"`
        sum=$(($res + $num_links -1 )) # Leave the outermost residue alone
        mkdir tmp"$sum"   
        cp pdb_Files/$f* tmp"$sum"
        rm pdb_Files/$f* # remove the originals        

        for (( i = $sum ; i > $res  ; i-- ));
        do
            j=$((i-1))
            runner.sh tmp$i $i tmp$j
        done
        mkdir output"$f"
        cp tmp$j/* output"$f"
        rm -r tmp*
    fi
done
rm -r 1.outputs/
mkdir 1.outputs/
mv output* 1.outputs/
remove_hydrogens.sh
#mv 1.outputs/output*/* pdb_Files/
echo "Separately run clean_internal_clashes.sh with the original files in a folder called pdb_Files.bak and the 2-8 internal files in a folder called pdb_Files"
echo "This will clean any structures with internal bonds"
