#!/bin/bash
number_residues=$1
i=$number_residues #clarity

topFile=CPLX.prmtop
trajFile=md5000frames.nc

echo "writing ptraj_ts.in"
echo "trajin $trajFile netcdf" > ptraj_ts.in
echo "" >> ptraj_ts.in
echo "dihedral car1 :"$i"@C6 :"$i"@O6 :"$i"@C2 :"$i"@C1 out C6-O6-C2-C1.csv" >> ptraj_ts.in
echo "dihedral car2 :"$i"@C4 :"$i"@C3 :"$i"@C2 :"$i"@C1 out C4-C3-C2-C1.csv" >> ptraj_ts.in 
echo "Now running this:"
echo "cpptraj -p $topFile -i ptraj_ts.in"
cpptraj -p $topFile -i ptraj_ts.in

echo "Requires $topFile $trajFile and bonding.txt to work"
