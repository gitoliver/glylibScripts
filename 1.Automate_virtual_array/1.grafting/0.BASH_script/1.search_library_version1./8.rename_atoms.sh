#/usr/bin/env bash

source motif1.txt
source motif2.txt

mkdir 2.atomsrenamed
for f in `ls 1.search_results`
do
	grep "LINK.*$motif1.*$motif2" $f > LINK_$f.txt
#	awk '{print"",$2" ",$3}' LINK_$f.txt > motif1_$f.tmp
#	awk '{print"",$5" ",$6}' LINK_$f.txt > motif2_$f.tmp
	echo "motif1=\"" > temp1
	echo "\"" > temp2
	cat LINK_$f.txt | cut -c 14-26 > temp3
	paste temp1 temp3 temp2 | sed 's/\t//g' > motif.sed
	awk '{print $1}' temp3 > a1.atom
	

