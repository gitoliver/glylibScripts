#!/bin/bash

cp -r z.originals 0.grafting_NOwgl

cd 0.grafting_NOwgl
0.make_motifs_2.1 
1.search_library.sh_2.2
2.make2.grafting.sh 
cd ../

cp -r 0.grafting_NOwgl 2.grafting_10wgl
cp -r 0.grafting_NOwgl 4.grafting_20wgl


cd 0.grafting_NOwgl
nohup ./2.grafting.sh &
cd ../

cd 2.grafting_10wgl
sed -i 's/grafting.exe 0/grafting.exe 10/g' 2.grafting.sh
nohup ./2.grafting.sh &
cd ../

cd 4.grafting_20wgl
sed -i 's/grafting.exe 0/grafting.exe 20/g' 2.grafting.sh
nohup ./2.grafting.sh &
cd ../
