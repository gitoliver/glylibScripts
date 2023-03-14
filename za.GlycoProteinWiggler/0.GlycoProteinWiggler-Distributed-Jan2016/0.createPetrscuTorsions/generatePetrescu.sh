#!/bin/bash
export PATH=$PATH:/home/oliver/Dropbox/1.glylib_scripts/z.GlycoProteinWiggler/0.createPetrscuTorsions
if [ $# -eq 1 ]; then
pdb=$1
noExt="${pdb%.*}"
createTorsionAtoms.exe $pdb > tmp
grep -A6 Petrescu1 tmp | grep HETATM > $noExt-P1.pdb
grep -A6 Petrescu2 tmp | grep HETATM > $noExt-P2.pdb
grep -A6 Petrescu3 tmp | grep HETATM > $noExt-P3.pdb
grep -A6 Petrescu4 tmp | grep HETATM > $noExt-P4.pdb
grep -E -v "OME|ROH|LINK" $pdb >> $noExt-P1.pdb
grep -E -v "OME|ROH|LINK" $pdb >> $noExt-P2.pdb
grep -E -v "OME|ROH|LINK" $pdb >> $noExt-P3.pdb
grep -E -v "OME|ROH|LINK" $pdb >> $noExt-P4.pdb
echo "Críochnaithe (fin)"
else
echo "Nil me sasta, Usage: ./generatePetrescu.sh input.pdb"
fi
