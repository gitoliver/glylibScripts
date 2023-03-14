#!/bin/bash

rm -r ../3.linkerLib
mkdir ../3.linkerLib
switch=0
for f in `ls -d -- */`
do
    echo "Doing $f"
    cd $f
    l=${#f} # length of $f
    x=$(($l-5)) # length of $f minus 5
    name=${f:0:$x}${f:$(($l-2)):1}
    echo "trajin md5000frames.nc netcdf 1 5000 100" > ptraj50ss.in
    echo "strip :MOH" >> ptraj50ss.in
    echo "strip @H*" >> ptraj50ss.in
    echo "trajout randomSS/$name.pdb multi pdb" >> ptraj50ss.in
    rm -r randomSS/
    mkdir randomSS/
    cpptraj CPLX.prmtop < ptraj50ss.in
    cp randomSS/$name.pdb.1 ligand.pdb
    if [[ $switch -eq 0 ]]; then
        folder=`echo $name | cut -d . -f2`
        mkdir ../../3.linkerLib/$folder
        mv randomSS/* ../../3.linkerLib/$folder/
        switch=1
    else
        mv randomSS/* ../../3.linkerLib/$folder/
        switch=0
    fi
    cd ../
done
