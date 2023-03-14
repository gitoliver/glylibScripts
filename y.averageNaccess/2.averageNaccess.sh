#!/bin/bash

cd 1.mdSS-naccess/
for f in `ls *.rsa` 
do
    sed 's/\ \ */,/g' $f > $f.csf
done

for res in `grep "^RES" $f.csf | cut -d , -f2-3`
do
   i=0
   for file in `ls *.csf | sort -k2 -t\- -n`
   do
       i=$(($i+1))
       sa=`grep -h "$res" $file | cut -d , -f4`
       #echo "$i,$sa"
       echo "$i,$sa" >> $res.out
   done
done
cd ../
