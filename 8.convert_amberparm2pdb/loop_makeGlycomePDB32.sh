#!/bin/bash
folder=Files/

mkdir pdbs/
ls ./$folder -ABv1 | grep top > fileList.txt

for filename in `more fileList.txt`;
do
echo "working on $filename"
name=${filename%.*} # name = filename without extension
makeGlycomePDB Files/$name.top Files/$name.rst pdbs/$name.pdb

done
