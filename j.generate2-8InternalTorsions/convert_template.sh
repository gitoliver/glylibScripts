#!/bin/bash

echo "echo > \$2.js"  > 2.create_2-8_torsions.sh
echo "if [ \$# -ne 3 ]; then echo \"Usage: 2.create_2-8_torsions.sh folder prefix linkage\"; else " >> 2.create_2-8_torsions.sh

#sed 's/"/\\"/g' template.js > tmp.txt

sed 's/"/\\"/g' template.js > tmp.txt
IFS=$'\n'
for f in `cat tmp.txt` 
do
    echo "echo \"$f\" >> \$2.js " >> 2.create_2-8_torsions.sh ; 
done
#sed 's/"/\\"/g' tmp.txt | sed 's/\(/\\\(/g' | sed 's s/\)/\\\)/g' >> 2.create_2-8_torsions.sh
echo "fi" >> 2.create_2-8_torsions.sh
chmod +x 2.create_2-8_torsions.sh
