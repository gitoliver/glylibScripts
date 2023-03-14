#!/bin/bash

if [ $# -ne 5 ]; then

printf "\nUsage wiggle-rounders.sh glycan.pdb target.pdb residueToMove targetResidue coreresidue\n"
printf "Example wiggle-rounders.sh glycan121.pdb headGroup.pdb 16 10 28\n\n"

else

rm -r Round* best*
mkdir Round1 Round2 Round3

driverWigglerToSite.exe $1 $2 $3 $4 $5 > log.file1
mv best_structure.pdb round1.pdb
mv best_structure* Round1/

driverWigglerToSite.exe round1.pdb $2 $3 $4 $5  > log.file2
mv best_structure.pdb round2.pdb
mv best_structure* Round2/

driverWigglerToSite.exe round2.pdb $2 $3 $4 $5  > log.file3
mv best_structure.pdb round3.pdb
mv best_structure* Round3/

echo done

fi
