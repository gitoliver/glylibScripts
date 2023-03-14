#!/bin/bash
mkdir 0.mdSnapShots/ 1.mdSS-naccess/
echo "trajin ../md.nc netcdf 1 25000 250" > ptrajSS.in
echo "strip @H*" >> ptrajSS.in
echo "trajout 0.mdSnapShots/mdSS multi pdb" >> ptrajSS.in

cpptraj -p ../CPLX.prmtop -i ptrajSS.in

cd 0.mdSnapShots/
for f in `ls`; 
do 
    g=`echo $f | sed 's/\./-/g'`; 
    mv $f $g.pdb; 
done
for f in `ls`;
do
    /home/oliver/programs/naccess/naccess $f
done
mv *.asa *.rsa *.log ../1.mdSS-naccess/
