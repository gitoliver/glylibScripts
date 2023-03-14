#!/bin/bash

echo "trajin md5000frames.nc netcdf" > ptrajSDF.in
#echo "#Measure distance" >> ptrajSDF.in
#echo "distance d1 :1@CL5 :3@C1 out dCL5-GlyC1.dat" >> ptrajSDF.in
echo "#Now do SDF" >> ptrajSDF.in
echo "center @1-5 mass origin" >> ptrajSDF.in
echo "image origin center familiar" >> ptrajSDF.in
echo "#trajout centered_imaged.nc netcdf" >> ptrajSDF.in
echo "rms first out rms.dat @1-5" >> ptrajSDF.in
echo "grid C1.xplor 100 0.5 100 0.5 100 0.5 :3@C1" >> ptrajSDF.in
#echo "grid COM.xplor 100 0.5 100 0.5 100 0.5 :3@C1,C2,C3,C4,O5,C5" >> ptrajSDF.in
echo "grid C4.xplor 100 0.5 100 0.5 100 0.5 :3@C4" >> ptrajSDF.in
echo "# Write out an avg of the center linker MeOH portion." >> ptrajSDF.in
echo "strip @6-999999" >> ptrajSDF.in
echo "translate x -0.25 y -0.25 z -0.25" >> ptrajSDF.in
echo "average avg.pdb pdb" >> ptrajSDF.in

for f in `ls -d -- */`
do
    echo "Doing $f"
    cd $f
    num=`grep "0VA" CPLX.pdb | head -1 | cut -c 26`
    if [[ -z $num ]]; then
        num=`grep "0YB" CPLX.pdb | head -1 | cut -c 26`
    fi
    sed "s/3@C/$num@C/g" ../ptrajSDF.in > ptrajSDF.in
    ptraj CPLX.prmtop < ptrajSDF.in
    cd ../
done
echo "done"
