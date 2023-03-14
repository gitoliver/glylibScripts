#!/bin/bash

echo "set bg white" > input.cmd
#echo "colourdef purp 1.0 0.1 0.9 1.0" >> input.cmd
#echo "colourdef C 0.3 0.3 0.3 1.0" >> input.cmd
#echo "colordef dgrey 0.28 0.28 0.28 1.0" >> input.cmd
#echo "colordef sblue 0.53 0.81 0.92 1.0" >> input.cmd
#echo "colourdef O 1.0 0.6 0.12 1.0" >> input.cmd
#echo "colourdef S 0.48 0.64 0.48 1.0" >> input.cmd
#echo "colourdef G 0.64 0.72 1 1.0" >> input.cmd
cnt=0

echo "" >> input.cmd

for f in `ls -d -- */`
do
    echo "open "$f"avg.pdb" >> input.cmd
    echo "~display #$cnt" >> input.cmd
#    echo "define plane thickness 0.5 padding 12 raiseTool false color dgrey #$cnt@H?@H??" >> input.cmd
#    echo "define plane thickness 0.5 padding 12 raiseTool false color sblue #$cnt@H?@H??" >> input.cmd
    cnt=$(($cnt+1))
    echo "open "$f"C1.xplor" >> input.cmd
    echo "open "$f"C4.xplor" >> input.cmd
#    echo "open "$f"COM.xplor" >> input.cmd
    echo "volume #$cnt show style surface level 1.0 color \"dark grey\" transparency 0.65" >> input.cmd
    cnt=$(($cnt+1))
    echo "volume #$cnt show style surface level 1.0 color \"dark red\" transparency 0.65" >> input.cmd
#    cnt=$(($cnt+1))
#    echo "volume #$cnt show style surface level 1.0 color $clr" >> input.cmd
    cnt2=$(($cnt-2))
    echo "~select 0-$cnt" >> input.cmd
    echo "select 0,$cnt2-$cnt" >> input.cmd
    echo "match #$cnt2 #0 active" >> input.cmd
    cnt=$(($cnt+1))
    echo "open "$f"ligand.pdb" >> input.cmd
    echo "color \"navy blue\" #$cnt" >> input.cmd
    echo "~select 0-$cnt" >> input.cmd
    echo "select 0,$cnt" >> input.cmd
    echo "match #$cnt@CL5,OL2 #0@CL5,OL2 active" >> input.cmd
    echo "" >> input.cmd
    cnt=$(($cnt+1))
done
echo "define plane thickness 0.5 padding 12 raiseTool false color \"dim grey\" #0@H?@H??" >> input.cmd
echo "define plane thickness 0.5 padding 12 color \"sea green\" #0@H?@H??" >> input.cmd
#echo "color grey :LNK" >> input.cmd
#echo "color red @OL2" >> input.cmd
#echo "~display :LNK" >> input.cmd
#echo "display #0" >> input.cmd
echo "select 0-$(($cnt-1))" >> input.cmd
echo "~show H" >> input.cmd
echo "windowsize 600 600" >> input.cmd
echo "done"
echo "Turn by \"turn #3@CL5,OL2 10\""
