#!/bin/bash
for f in `ls *.csv`
do
#	echo "set term x11" > $f.plot
        echo "set term post enh eps solid color \"Arial\" 14" > $f.plot
#	echo "set term png" > $f.plot
        echo "set output \"$f.eps\"" >> $f.plot
	echo "set title \"$f\"" >> $f.plot
#	echo "set key off" >> $f.plot
	echo "set multiplot" >> $f.plot
	echo "set size 1.0,1.0" >> $f.plot
	echo "set origin 0,0" >> $f.plot
	echo "set xlabel \"Timestep (ps)\"" >> $f.plot
	echo "set ylabel \"Torsion Angle (degrees)\"" >> $f.plot
        echo "set yrange -180,180" >> f.plot
	echo "set ytics 60" >> $f.plot
	#echo "set xtics -180,180" >> $f.plot
	echo "plot \"./$f\" using 1:2" >> $f.plot
	echo "Processing $f"
done
for f in `ls *.csv.plot`
do
	gnuplot -persist $f
done
for f in `ls *.eps`
do
	convert $f $f.png
	display $f &
done
