#!/bin/bash

if [[ $# -eq 1 ]]; then

echo "set bg white" > input.cmd
echo "colourdef G 0.64 0.72 1 1.0" >> input.cmd

echo "" >> input.cmd

inputfile=$1
# $glycanFolder = path and name of folder containing glycans
# $receptor = path and name of receptor file
# $numberSites = number of residues to be glycosylated
# $site1 = residue id of first site
# $site1glyan = glycan id to be added to first site

source $inputfile

echo "open $receptor" >> input.cmd
modelnumber=0

for ((x=1; x<=$numberSites; x++))
do
    tmp=site"$x"glycan
    subst="$tmp[@]"
    tmpx=site"$x"
    substx="$tmpx[@]"
    #echo "${!subst}"
    echo "display #0:"${!substx}"" >> input.cmd

    for f in `ls $glycanFolder${!subst}-*`
    do
        modelnumber=$(($modelnumber+1))
        echo "open "$f"" >> input.cmd
        echo "match #$modelnumber:OME@O,CH3,CB #0:"${!substx}"@ND2,CG,CB" >> input.cmd
        echo "display #$modelnumber" >> input.cmd
        echo "delete #$modelnumber:OME@O,CH3,CB" >> input.cmd
        echo "" >> input.cmd
        echo "findclash #0 test #$modelnumber overlapcutoff 1.0 savefile "$f"-clashes" >> input.cmd
        echo "" >> input.cmd
    done
done 

echo "windowsize 600 600" >> input.cmd
echo "Finished! Now do:"
echo "chimera input.cmd"

else
echo "#########################################"
echo "Usage: ./createChimeraInputs.sh setup.txt"
echo "#########################################"
fi
