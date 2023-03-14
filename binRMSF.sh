#!/bin/bash

if [[ $# -eq 6 ]]; then
sf=$1 #startframe
ef=$2 #endframe
os=$3 #offset
nb=$4 #number of bins
resmask=$5
tag=$6
tj='../md-combined.nc' #trajectory file name
pt='../CPLX.prmtop' #prmtop file name

# Get average structure
echo "trajin  $tj netcdf $sf $ef $os" > avrPtraj.in
echo "average average.pdb pdb" >> avrPtraj.in
cpptraj -p $pt -i avrPtraj.in

ff=0 #first frame
lf=0 #last frame
gap=$(($ef / $nb))

#Go through making bins of the data
for (( i=1; i<=$nb; i++ ))
do
    ff=$(($lf+1))
    lf=$(($lf+$gap))
    echo "trajin $tj netcdf $ff $lf $os" > $i.ptrajBin.in
    echo "reference average.pdb" >> $i.ptrajBin.in
    echo "rms reference out fitting $resmask" >> $i.ptrajBin.in 
    echo "atomicfluct out $i.apf $resmask bymask bfactor $i" >> $i.ptrajBin.in
    cpptraj -p $pt -i $i.ptrajBin.in
done

#Merge files into one
for f in `ls *.apf | sort -n`
do
    tail -n1 $f >> $tag.txt
done

rm fitting average.pdb avrPtraj.in *ptrajBin* *.apf

else
echo ""
echo "NumInputs=$#, should be 6"
echo "Usage: ./binRMSF.sh startframe endframe offset numberOfBins residueMask tag"
echo "Example: ./binRMSF.sh 1 25000 25 10 :99-100 B-arm"
echo ""
fi
