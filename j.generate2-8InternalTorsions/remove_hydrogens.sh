#!/bin/bash
cd 1.outputs/
for folder in `ls`; 
do
    cd $folder 
    for f in `ls *.pdb`; 
    do 
        sed -i '/HETATM  ...  H.*/d' $f; 
        sed -i '/CONECT/d' $f;
    done
    cd ../
done
cd ..
