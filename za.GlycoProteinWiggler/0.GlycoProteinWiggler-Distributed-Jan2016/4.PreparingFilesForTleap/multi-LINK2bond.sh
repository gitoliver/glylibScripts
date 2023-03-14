#!/bin/bash
if [ $# -eq 3 ]; then
    >bonding.out
    reslist=$1
    LINK=$2
    lastResid=$3
    Glyresidues=`grep -c "." $LINK`
    Proresidues=`grep -c "." $reslist`

    for NLNresid in `cat $reslist`
    do
        LINK2BOND.sh $LINK temp $lastResid $NLNresid
        cat temp >> bonding.out
        lastResid=$(( $lastResid + $Glyresidues )) #offset by number of glycan residues
    done
    rm temp
    echo "Results in bonding.out"
else
    echo "Usage: $0 <list of NLN residue numbers> <LINK cards for solo glycan> <last protein residue number>"
    echo "Example: $0 resList M9-LINK.txt 445"
fi

