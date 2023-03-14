#!/bin/bash

if [[ $# -eq 2 ]]; then

inputfile=$1
outputfile=$2

echo "set bg_color white" > $outputfile 
echo "colourdef G 0.64 0.72 1 1.0" >> $outputfile

echo "" >> $outputfile

echo $inputfile
echo $outputfile 
# $glycanFolder = path and name of folder containing glycans
# $receptor = path and name of receptor file
# $numberSites = number of residues to be glycosylated
# $site1 = residue id of first site
# $site1glyan = glycan id to be added to first site

source $inputfile
echo $glycanFolder

echo "open $receptor" >> $outputfile
modelnumber=0

for ((x=1; x<=$numberSites; x++))
do
    tmp=site"$x"glycan
    subst="$tmp[@]"
    tmpx=site"$x"
    substx="$tmpx[@]"
    #echo "${!subst}"
    echo "display #0:"${!substx}"" >> $outputfile

    for f in $glycanFolder${!subst}_*pdb
    do
        modelnumber=$(($modelnumber+1))
        echo "open "$f"" >> $outputfile
        echo "match #$modelnumber:ASN@N,CA,CB #0:"${!substx}"@N,CA,CB" >> $outputfile 
        echo "display #$modelnumber" >> $outputfile
        echo "delete "${!substx}"@ND2,CG,OD1" >> $outputfile
        echo "delete #$modelnumber:ASN@N,CA,CB" >> $outputfile
#        echo "match #$modelnumber:OME@O,CH3 #0:"${!substx}"@ND2,CG" >> $outputfile
#        echo "display #$modelnumber" >> $outputfile
#        echo "delete #$modelnumber:OME@O,CH3" >> $outputfile
        echo "" >> $outputfile
        echo "findclash #0 test #$modelnumber overlapcutoff 1.0 savefile "$f"-clashes" >> $outputfile
        echo "" >> $outputfile
    done
done 

echo "windowsize 600 600" >> $outputfile
echo "Finished! Now do:"
echo "chimera $outputfile"

else
echo "#########################################################"
echo "Usage: ./createChimeraInputs.sh setup.txt output_file.txt"
echo "#########################################################"
fi
