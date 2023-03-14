#!/bin/bash
if [ $# -eq 1 ] || [ $# -eq 2 ]; then
    file=$1
    file2=$2
    steps=$((`grep -c "." $file` - 1))
    timefactor=10/1000
    steps=$(($steps * $timefactor))
    mid=$(($steps / 2))
    name=${file%%.*}
    name2=${file2%%.*}
    tpop=`grep "C4-C3-C2-C1.csv.dat Transistion" pops.txt | grep -o Pop=.... | cut -d = -f2`
    gpop=`grep "C4-C3-C2-C1.csv.dat Ground" pops.txt | grep -o Pop=.... | cut -d = -f2`
    echo "#file created by write_gnu_inputs.sh" > commands4.gnu
    #echo "set key samplen 0.2 spacing 0.4 bottom center font 'calibri,8' title 'Legend' enhanced box linewidth 1" >> commands4.gnu
    #echo "set key left bottom Left reverse title 'Legend' font 'calibri,8' box 3" >> commands4.gnu
    echo "set terminal gif font 'calibri,20' size 800,600 enhanced" >> commands4.gnu
    echo "unset key" >> commands4.gnu
    #echo "set key bottom center font 'calibri,10'" >> commands4.gnu
    echo "set output '$name.gif'" >> commands4.gnu
    echo "set title '$name and $name2'" >> commands4.gnu
    echo "set xlabel \"Timestep (ns)\"" >> commands4.gnu
    echo "set ylabel \"Torsion Angle (degrees)\"" >> commands4.gnu
    echo "set yrange [60:180]">> commands4.gnu
#    echo "set autoscale y" >> commands4.gnu
    echo "set xrange [0:$steps]" >> commands4.gnu
    echo "set pointsize 1.0" >> commands4.gnu
    echo "set arrow from 0,110 to $steps,110 nohead lc rgb 'black'" >> commands4.gnu
    echo "set label 1 \"Ground-State: $gpop%\" font 'calibri,18' at $mid,103 center textcolor rgbcolor 'black'" >> commands4.gnu
    echo "set label 2 \"Michaelis-Complex: $tpop%\" font 'calibri,18' at $mid,113 center textcolor rgbcolor 'black'" >> commands4.gnu
    #echo "unset key" >> commands4.gnu
    echo "set label 3 \"Red:$name\" font 'calibri,15' at $mid, 71 center textcolor rgbcolor 'red'" >> commands4.gnu
    echo "set label 4 \"Black:$name2\" font 'calibri,15' at $mid, 65 center" >> commands4.gnu
    echo -n "plot \"$file\" u (\$1 * $timefactor):2 w dots" >> commands4.gnu
    if [ $# -eq 2 ]; then
        echo ",\\" >> commands4.gnu
        echo "\"$file2\" u (\$1 * $timefactor):(\$2*-1) w dots lt -1 title 'Black: $name2';" >> commands4.gnu
    fi
    echo "" >> commands4.gnu
    echo "quit" >> commands4.gnu
    echo "Running gnuplot"
    gnuplot  commands4.gnu
    display $name.gif
else
    echo "Usage write_gnu_inputs.sh C4-C3-C2-C1.csv C6-O6-C2-C1.csv # don't mess with the order"
fi
