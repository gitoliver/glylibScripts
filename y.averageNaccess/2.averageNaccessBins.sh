#!/bin/bash
if [[ $# -eq 3 ]]; then
nb=$1 #number of bins
res=$2 #residue to analyse
resNum=$3 
cd 1.mdSS-naccess/
for (( i=1; i<=$nb; i++ ))
do
    > sa #creates empty file
    for f in `ls $i-mdSS-*.rsa | sort -k3 -t\- -n` 
    do
        grep -h "RES $res *$resNum" $f | cut -c 14-22 | sed 's/ //g' >> sa
    done
    /home/oliver/Dropbox/1.glylib_scripts/k.mean_stdDev_analyse_torsions/meanStdev.exe sa norm > $i.$res.$resNum.average
    cat $i.$res.$resNum.average >> ../$res.$resNum.averages
done
cat ../$res.$resNum.averages
echo "results are in $res.$resNum.averages"
else
echo ""
echo "NumInputs=$#, should be 3"
echo "Usage: ./2.averageNaccessBins.sh numberOfBins resName resNum"
echo "Example: ./2.averageNaccessBins.sh 100 0MA 98"
echo ""
fi

