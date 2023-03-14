#!/bin/bash
> pops.txt
for f in `ls *.csv`
do
    tail -n +2 $f | sed "s/^ *//;s/ *$//;s/ \{1,\}/,/g" | cut -d, -f2 > $f.dat
    getpop.exe $f.dat >> pops.txt
done
