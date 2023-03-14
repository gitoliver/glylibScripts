#!/bin/bash

if [ $# -eq 2 ]; then
 resid=$1
 folder=$2
 source /scratch1/webdev/grafting/runs/$folder/0.grafting/count.txt
 printf "$resid=$count\n" >> resConversion
 #echo "resid=$resid"
 grep "LINK...\{12,19\}[A-Z ]$resid " glycam_pdb.pdb >> tempLINK.txt
 IFS='%' # otherwise whitespace in stringi gets removed  
 stringi=`printf "%"${#resid}"s\n" $count` # eg replaceing "59" with "1" messes alignment; need " 1".
 #grep "HETATM...............* $resid" glycam_pdb.pdb | sed 's/HETATM\(...............*\) '$resid' /HETATM\1 '$stringi' /g' >> temp.pdb # does not find four digit residues if chain ID precedes e.g A1021
 grep "HETATM.\{12,19\}[A-Z ]$resid " glycam_pdb.pdb | sed 's/HETATM\(.\{12,19\}[A-Z ]\)'$resid' /HETATM\1'$stringi' /g' >> temp.pdb #Beautiful capture groups and regular expressions. Is faster with grep.
 unset IFS
 for linkresid in `grep "LINK...\{12,19\}[A-Z ]$resid " glycam_pdb.pdb | cut -c 53-56`
 do
     let count+=1
     echo "count=$count" > count.txt
#     echo "$linkresid=$count"
     sh 1.recursive-find-connections.sh $linkresid $folder
     source /scratch1/webdev/grafting/runs/$folder/0.grafting/count.txt
     #echo "Back, $linkresid, and count=$count"
 done
else
 echo "Usage 1.recursive-find-connections.sh \$resid \$folder"
fi



#linkresid=`grep "LINK.*$resid" glycam_pdb.pdb | cut -c 23-26`
#echo $linkresid
#sleep 1s
#for nextResid in `grep "LINK.................................................*$linkresid       " glycam_pdb.pdb | cut -c 23-26`
#do
#     sh 1.recursive-find-connections.sh $nextResid
#done
