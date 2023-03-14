#!/bin/bash

if [ $# -ne 2 ]; then
    echo "Usage: visualize.sh glycan budcore"
else
echo "colourdef C 0.4 0.7 0.8 0.2" > input.cmd
glycan=$1-
bud=$2
rm -rf planes
mkdir planes
rm -rf visual
mkdir visual
cp Structures/$glycan*$bud.pdb visual/
echo "$glycan"
echo "open receptor.pdb" >> input.cmd
i=1
for file in `ls visual/`
do
    sed -i 's/TER//g' visual/$file
    echo "open visual/$file" >> input.cmd
    i=$(($i+1));
    grep "ATOM.....................1" visual/$file > plane.in
    grep "ATOM.....................2" visual/$file >> plane.in
    createPlane.exe plane.in C2 C1 O1 > planes/plane_$file
    echo "open $i planes/plane_$file" >> input.cmd;
    echo "define plane raiseTool false color C #$i :XXX" >> input.cmd
    i=$(($i+1));
done
echo "~colour" >> input.cmd
echo "represent sphere ::ROH" >> input.cmd
echo "colour red ::ROH" >> input.cmd
echo "colour grey #0 " >> input.cmd
echo "~show :XXX" >> input.cmd  

fi
echo "Finished"
