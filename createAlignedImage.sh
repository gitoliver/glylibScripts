#!/bin/bash


if [[ $# -eq 2 ]]; then

rm -r Alnk$2
mkdir Alnk$2


echo "set bg white" > Ainput$2.cmd
echo "colordef dgrey 0.28 0.28 0.28 1.0" >> Ainput$2.cmd
echo "colordef sblue 0.53 0.81 0.92 1.0" >> Ainput$2.cmd

cnt=0 #number of files opened in chimera
ns=0 #keeps track of model number for glycan so can superimpose linker
echo "" >> Ainput$2.cmd

echo "open $1" >> Ainput$2.cmd
#echo "open linker$mod.pdb" >> Ainput$2.cmd
#echo "surface protein" >> Ainput$2.cmd
cnt=$(($cnt+1))
#echo "transparency 50 protein" >> Ainput$2.cmd
#echo "color sblue protein" >> Ainput$2.cmd
#echo "color goldenrod #0:.A" >> Ainput$2.cmd
lig=0
rec=1
mod=28

for s in `ls Structures/$2* | xargs -n1 basename`
do
    lig=$(($lig+1))
    grep -v TER Structures/$s > Alnk$2/lig$lig.pdb
    #echo "open Alnk$2/lig$cnt.pdb" >> Ainput$2.cmd
    #cnt=$(($cnt+1))
    
    anchor=1
    #echo "match #$ns:$4@C1,C2,O5 #0:$2@C1,C2,O5" >> Ainput$2.cmd
    for f in `ls Spacer3Dfiles/$s*`
    do
        sed 's/a4/O5/g' $f | sed 's/a1/C1/g' | sed 's/a2/O1/g' | sed 's/a3/C2/g' | grep -v "ATOM ....................3 "> Alnk$2/linker$cnt.pdb
        cat Alnk$2/lig$lig.pdb | grep -v ROH >> Alnk$2/linker$cnt.pdb
        if [[ $cnt -eq $mod ]]; then
        #if [[ $rec -eq 10 ]]; then
            cat $1 >> Alnk$2/linker$cnt.pdb
            rec=1;
        fi
        echo "open Alnk$2/linker"$cnt".pdb" >> Ainput$2.cmd
        echo "color \"grey\" #$cnt" >> Ainput$2.cmd
        if [[ $cnt -gt 1 ]]; then # if not the first one
            echo "match #$cnt:1@CL4,OL2,CL5 #$anchor:1@CL4,OL2,CL5" >> Ainput$2.cmd
        fi
        cnt=$(($cnt+1))
        rec=$(($rec+1))
        echo "" >> Ainput$2.cmd
    done
done
createPlane.exe Alnk$2/linker$anchor.pdb CL4 CL5 OL2 | grep ATOM > Alnk$2/surface.pdb 
echo "open Alnk$2/surface.pdb" >> Ainput$2.cmd
echo "~select all"
echo "select $cnt"
echo "move it around"
echo "select @H"
echo "define plane thickness 1 padding 50 raiseTool false color dgrey @H" >> Ainput$2.cmd
echo "define plane thickness 1 padding 50 raiseTool false color dgrey @H"
echo "~display :301-303" >> Ainput$2.cmd
echo "represent sphere #$mod:302.A@C1" >> Ainput$2.cmd
echo "vdwdefine +3 #$mod:302.A@C1" >> Ainput$2.cmd
echo "color goldenrod #$mod:302.A@C1" >> Ainput$2.cmd
echo "display #$mod:302.A@C1" >> Ainput$2.cmd
echo "transparency 60" >> Ainput$2.cmd
echo "transparency 0 #$mod" >> Ainput$2.cmd
echo "combine #1-$(($mod-1)),$(($mod+1))-$(($cnt-1)) newchainids false" >> Ainput$2.cmd
echo "color navy #$mod" >> Ainput$2.cmd
echo "color sblue #$mod:.B" >> Ainput$2.cmd
echo "color goldenrod #$mod:.A" >> Ainput$2.cmd
echo "~display @CL5" >> Ainput$2.cmd

#echo "~display" >> Ainput$2.cmd
#echo "~show #0:301-303" >> Ainput$2.cmd
#echo "~show H" >> Ainput$2.cmd
#echo "~show @CL5" >> Ainput$2.cmd
#echo "windowsize 453 223" >> Ainput$2.cmd
echo "done"

else
echo ""
echo "Usage: ./createChimeraInputDimers.sh <dimerwithligands> <grafted glycan file>"
echo "Example: ./createChimeraInputDimers.sh 3PHZ.pdb 270-phi-1_omega-gt.pdb"
echo "I don't have time to make this awesome, so it sucks. Run it once with if [[ $rec -eq 10 ]]"
echo "Note the number of the model you want. Change $mod to that" 
echo ""
fi



