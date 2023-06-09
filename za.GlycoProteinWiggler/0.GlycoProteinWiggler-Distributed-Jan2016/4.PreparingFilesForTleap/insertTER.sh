#!/bin/bash

if [ $# != 3 ] || [ ! -e $file ] ; then
echo ""
        echo "###################################################################################"
        echo "Usage: $0 first-resid last-resid filename"
        echo "Example: $0 445 643 glycosylated.pdb"
        echo "filename must exist! Change HETATM to ATOM  "
        echo "###################################################################################"
else

first=$1
last=$2
file=$3

echo "Making file-backup.pdb just in case"
cp $file file-backup.pdb
cp $file buffer
i=$first;
for (( i=$first; i<$last; i++)); do
    echo $i
    linenum=`pcregrep -n -M "ATOM ................. *$i .*\nATOM ................. *$(($i +1)) " buffer | cut -f1 -d: | grep -v ATOM`
    if [ -z $linenum ] ; then # if $linenum set
        echo "TER card not inserted between resid $i and $(($i+1)). Possibly one already there?"
    else
        echo "linenum=$linenum"
        sed ''$linenum'a\TER' buffer > tmp
        mv tmp buffer
    fi
done
mv buffer result.pdb
echo "Results are in result.pdb"
fi
