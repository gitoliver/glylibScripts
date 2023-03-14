#!/bin/bash

cp -r z.originals 0.grafting

cd 0.grafting
0.make_motifs_2.1 
1.search_library.sh_2.2
2.make2.grafting.sh 
nohup ./2.grafting.sh &
cd ../

