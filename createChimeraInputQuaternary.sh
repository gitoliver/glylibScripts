#!/bin/bash


if [[ $# -eq 5 ]]; then

echo "set bg white" > input$3.cmd
#echo "colourdef purp 1.0 0.1 0.9 1.0" >> input$3.cmd
#echo "colourdef C 0.3 0.3 0.3 1.0" >> input$3.cmd
echo "colordef dgrey 0.28 0.28 0.28 1.0" >> input$3.cmd
echo "colordef sblue 0.53 0.81 0.92 1.0" >> input$3.cmd
#echo "colourdef O 1.0 0.6 0.12 1.0" >> input$3.cmd
#echo "colourdef S 0.48 0.64 0.48 1.0" >> input$3.cmd
#echo "colourdef G 0.64 0.72 1 1.0" >> input$3.cmd

rm -r lnk$3
mkdir lnk$3

cnt=0 #number of files opened in chimera
ns=0 #keeps track of model number for glycan so can superimpose linker
echo "" >> input$3.cmd

echo "open $1" >> input$3.cmd
echo "surface protein" >> input$3.cmd
echo "transparency 50 protein" >> input$3.cmd
echo "color sblue protein" >> input$3.cmd
echo "color goldenrod #0:.A" >> input$3.cmd
echo "color \"dark khaki\" #0:.C" >> input$3.cmd
echo "color \"dark grey\" #0:.D" >> input$3.cmd

for s in `ls Structures/$3* | xargs -n1 basename`
do
    grep -v TER Structures/$s > lnk$3/$s-lig$ns.pdb
    echo "open lnk$3/$s-lig$ns.pdb" >> input$3.cmd
    echo "open lnk$3/$s-lig$ns.pdb" >> input$3.cmd
    echo "open lnk$3/$s-lig$ns.pdb" >> input$3.cmd
    cnt=$(($cnt+3))
    ns=$cnt
    echo "match #$(($ns-2)):$4@C1,C2,C3,C4,C5,O5 #0:239.B@C1,C2,C3,C4,C5,O5" >> input$3.cmd
    echo "match #$(($ns-1)):$4@C1,C2,C3,C4,C5,O5 #0:239.C@C1,C2,C3,C4,C5,O5" >> input$3.cmd
    echo "match #$ns:$4@C1,C2,C3,C4,C5,O5 #0:239.D@C1,C2,C3,C4,C5,O5" >> input$3.cmd
    for f in `ls Spacer3Dfiles/$s*`
    do
        sed 's/a4/O5/g' $f | sed 's/a1/C1/g' | sed 's/a2/O1/g' | sed 's/a3/C2/g'> lnk$3/linker$cnt.pdb
        createPlane.exe $f CL4 CL5 OL2 | grep ATOM >> lnk$3/linker$cnt.pdb
        cp lnk$3/linker$cnt.pdb lnk$3/linker"$(($cnt+1))".pdb
        cp lnk$3/linker$cnt.pdb lnk$3/linker"$(($cnt+2))".pdb
        cp lnk$3/linker$cnt.pdb lnk$3/linker"$(($cnt+3))".pdb

        echo "open lnk$3/linker"$cnt".pdb" >> input$3.cmd
        cnt=$(($cnt+1))
        echo "define plane thickness 0.1 raiseTool false color dgrey #$cnt@H" >> input$3.cmd
        #echo "color \"hot pink\" #$cnt" >> input$3.cmd

        echo "open lnk$3/linker"$cnt".pdb" >> input$3.cmd
        cnt=$(($cnt+1))
        echo "match #$cnt:$5@C1,C2,O5 #$(($ns-2)):2@C1,C2,O5" >> input$3.cmd
        echo "define plane thickness 0.1 raiseTool false color dgrey #$cnt@H" >> input$3.cmd
        #echo "color \"orange red\" #$cnt" >> input$3.cmd

	echo "open lnk$3/linker"$cnt".pdb" >> input$3.cmd
        cnt=$(($cnt+1))
        echo "match #$cnt:$5@C1,C2,O5 #$(($ns-1)):2@C1,C2,O5" >> input$3.cmd
        echo "define plane thickness 0.1 raiseTool false color dgrey #$cnt@H" >> input$3.cmd

	echo "open lnk$3/linker"$cnt".pdb" >> input$3.cmd
        cnt=$(($cnt+1))
        echo "match #$cnt:$5@C1,C2,O5 #$ns:2@C1,C2,O5" >> input$3.cmd
        echo "define plane thickness 0.1 raiseTool false color dgrey #$cnt@H" >> input$3.cmd

        echo "" >> input$3.cmd
    done
done

echo "~display" >> input$3.cmd
#echo "~show #0:301-303" >> input$3.cmd
#echo "~show H" >> input$3.cmd
#echo "~show @CL5" >> input$3.cmd
#echo "windowsize 453 223" >> input$3.cmd
echo "done"

else
echo ""
echo "RORCR= Resid Of Reducing Core Residue"
echo "Usage: ./createChimeraInputDimers.sh <dimerwithligands> <RORCR in dimer file> <grafted glycan file> <RORCR in grafted glycan file> <resid of sugar in linkerfile>"
echo "Example: ./createChimeraInputDimers.sh 3PHZ.pdb 302.A 270-phi-1_omega-gt.pdb_budcore_3of1.pdb 3 3"
echo ""
fi



