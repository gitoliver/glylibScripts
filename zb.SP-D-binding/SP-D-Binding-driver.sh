#!/bin/bash

if [ $# -eq 1 ]; then

PATH=$PATH:/home/oliver/Dropbox/1.glylib_scripts/zb.SP-D-binding

/home/oliver/Dropbox/1.glylib_scripts/zb.SP-D-binding/SP-D-Binding.exe $1 > results.txt

grep Residues results.txt | sort -u | cut -c 11-21 > tmp

# Use translation table to convert from amber residue names to N-glycosylation sites
for line in `cat tmp`
do
    res1=`echo $line | cut -d , -f1`  
    res2=`echo $line | cut -d , -f2`
    res3=`echo $line | cut -d , -f3`
    Tres1=`grep $res1 LookupTable.txt | cut -f3`
    Tres2=`grep $res2 LookupTable.txt | cut -f3`
    Tres3=`grep $res3 LookupTable.txt | cut -f3`
    Mono1=`grep $res1 LookupTable.txt | cut -f1`
    Mono2=`grep $res2 LookupTable.txt | cut -f1`
    Mono3=`grep $res3 LookupTable.txt | cut -f1`

    echo "$Tres1($Mono1) -> $Tres2($Mono2) -> $Tres3($Mono3)"
done

else
echo "***************************************************************"
echo "* Usage:SP-D-Binding-driver.sh snapshots/                     *"
echo "***************************************************************"

fi
