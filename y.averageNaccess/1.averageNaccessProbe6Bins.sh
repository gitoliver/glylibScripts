#!/bin/bash

if [[ $# -eq 4 ]]; then
sf=$1 #startframe
ef=$2 #endframe
os=$3 #offset
nb=$4 #number of bins
tj='../md.nc' #trajectory file name
pt='../CPLX.prmtop' #prmtop file name

mkdir 0.mdSnapShots/ 1.mdSS-naccess/

ff=0 #first frame
lf=0 #last frame
gap=$(($ef / $nb))

#Go through making bins of the data
for (( i=1; i<=$nb; i++ ))
do
    ff=$(($lf+1))
    lf=$(($lf+$gap))
    echo "trajin $tj netcdf $ff $lf $os" > $i.ptrajBin.in
    echo "trajout 0.mdSnapShots/$i-mdSS multi pdb" >> $i.ptrajBin.in
    cpptraj -p $pt -i $i.ptrajBin.in
    cd 0.mdSnapShots/
    for f in `ls $i-mdSS*`
    do
        g=`echo $f | sed 's/\./-/g'`;
        mv $f $g.pdb;
        /home/oliver/programs/naccess/naccess $g.pdb -p 6
        sleep 0.1s
    done
    mv *.asa *.rsa *.log ../1.mdSS-naccess/
    cd ../
done
rm *ptrajBin.in

else
echo ""
echo "NumInputs=$#, should be 4"
echo "Usage: ./1.averageNaccessProbe6Bins.sh startframe endframe offset numberOfBins"
echo "Example: ./binRMSF.sh 1 25000 25 100"
echo ""
fi

