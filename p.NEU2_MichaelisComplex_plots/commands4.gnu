#file created by write_gnu_inputs.sh
set terminal gif font 'calibri,20' size 800,600 enhanced
unset key
set output 'C4-C3-C2-C1.gif'
set title 'C4-C3-C2-C1 and C6-O6-C2-C1'
set xlabel "Timestep (ns)"
set ylabel "Torsion Angle (degrees)"
set yrange [60:180]
set xrange [0:10]
set pointsize 1.0
set arrow from 0,110 to 10,110 nohead lc rgb 'black'
set label 1 "Ground-State: 24.0%" font 'calibri,18' at 5,103 center textcolor rgbcolor 'black'
set label 2 "Michaelis-Complex: 76.0%" font 'calibri,18' at 5,113 center textcolor rgbcolor 'black'
set label 3 "Red:C4-C3-C2-C1" font 'calibri,15' at 5, 71 center textcolor rgbcolor 'red'
set label 4 "Black:C6-O6-C2-C1" font 'calibri,15' at 5, 65 center
plot "C4-C3-C2-C1.csv" u ($1 * 10/1000):2 w dots,\
"C6-O6-C2-C1.csv" u ($1 * 10/1000):($2*-1) w dots lt -1 title 'Black: C6-O6-C2-C1';

quit
