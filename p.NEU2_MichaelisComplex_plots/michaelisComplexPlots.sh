#!/bin/bash

if [ $# -eq 1 ]; then
echo "Let's go!"
export PATH=$PATH:~/Dropbox/1.glylib_scripts/p.NEU2_MichaelisComplex_plots/
0.write_ptraj_inputs.sh $1
1.get_pop.sh
2.write_gnu_inputs.sh C4-C3-C2-C1.csv C6-O6-C2-C1.csv
echo "done!"
else
echo -e "\n##########\nYou need to specificy the residue number of the sialic acid, here is the bonding.txt file:"
cat bonding.txt
echo -e "Usage: \"michaelisComplexPlots.sh 4\" where 4 is the residue number\n##########\n"
fi
