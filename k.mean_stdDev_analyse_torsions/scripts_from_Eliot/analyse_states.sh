#!/bin/bash
source ../../0.INPUTS_EDIT_ME
cp ../../$topName .
file=$1
name=${file%%.*}
tail -n +2 $file > tmp.dat
count=`wc -l tmp.dat | cut -d\  -f1`
rm tmp1.dat
for ((i=$count ; i>0 ; i--)); 
do 
    echo "trajin ../../"$trajName"" >> tmp1.dat; 
done

paste -d\  tmp1.dat tmp.dat > $file
mkdir $2.$name
cat *ptraj.in | sed 's/dihedrals/'$2.$name'/g'  >> $file
cpptraj -p $topName -i $file
echo "$file"
cd $2.$name
for f in `ls *.csv`
do
    tail -n +2 $f | sed "s/^ *//;s/ *$//;s/ \{1,\}/,/g" | cut -d, -f2 > $f.dat
    dev=`/scratch1/oliver/bin/Stdev.exe $f.dat`
    if [ $dev -gt 50 ]; then
        meanStdev.exe $f.dat fix >> ../../../RESULTS.txt
    else
        meanStdev.exe $f.dat norm >> ../../../RESULTS.txt 
    fi
done
rm *.dat
