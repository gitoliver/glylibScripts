#!/bin/bash
rm lengthHeightEleStack.csv
for f in `ls -d -- */`
do
    cd $f
    mkdir lengthHeightStack
    rm ptrajtmp.in
    res=${f[@]:$((0-4)):3}
    num=`grep -m 1 $res CPLX.pdb | cut -c 24-28 | sed 's/\ //g'`
    echo "trajin md5000frames.nc netcdf 1 5001 1" >> ptrajtmp.in
    echo "trajout lengthHeightStack/tmp.pdb multi pdb" >> ptrajtmp.in
    
    cpptraj -p CPLX.prmtop -i ptrajtmp.in
    cd lengthHeightStack
    echo "Doing $f"
    for g in `ls`
    do
        getLengthElevationStackAngles.exe $g $num > $g.tmp
    done
    grep "length" *.tmp | cut -d = -f 2 > length.tmp
    length=`meanStdev.exe length.tmp norm` 
    grep "ele" *.tmp | cut -d = -f 2 > ele.tmp
    ele=`meanStdev.exe ele.tmp norm`
    grep "height" *.tmp | cut -d = -f 2 > height.tmp
    height=`meanStdev.exe height.tmp norm`
    grep "stack" *.tmp | cut -d = -f 2 > stack.tmp
    stack=`meanStdev.exe stack.tmp norm`
    rm -r tmp.pdb*
    cd ../../
    echo "System,Length, Height, Elevation, Stack," >> lengthHeightEleStack.csv 
    echo "$f,$length,$height,$ele,$stack" >> lengthHeightEleStack.csv
done
cat lengthHeightEleStack.csv
