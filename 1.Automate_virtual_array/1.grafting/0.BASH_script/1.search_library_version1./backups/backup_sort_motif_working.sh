#/usr/bin/env bash

#expects MOTIF.pdb
#outputs m*_name.txt and m*_linkage.txt which can be sourced as variables into the search_library.sh script
#only works for disacc motifs because of awk and grep -v ROH

cat MOTIF.pdb | grep LINK\ | grep -v ROH > motif.tmp #takes relevant LINK from MOTIF.pdb

#FOR RESIDUE 1
awk '{print"m1_link=\"",$2"\""}' motif.tmp > m1_linkage.tmp
cat m1_linkage.tmp | sed 's/\ //g' > m1_linkage.txt #removes the space before atom name
#now gets the linkage residue name
awk '{print"",$3}' motif.tmp > m1.tmp
cat m1.tmp | cut -c 3-4 > m11.tmp
awk '{print"m1_name=\".",$1"\""}' m11.tmp > m111.tmp
cat m111.tmp | sed 's/\ //g' > m1_name.txt

#FOR RESIDUE 2
awk '{print"m2_link=\"",$5"\""}' motif.tmp > m2_linkage.tmp
cat m2_linkage.tmp | sed 's/\ //g' > m2_linkage.txt #removes the space before atom name
#now gets the linkage residue name
awk '{print"",$6}' motif.tmp > m2.tmp
cat m2.tmp | cut -c 3-4 > m22.tmp
awk '{print"m2_name=\".",$1"\""}' m22.tmp > m222.tmp
cat m222.tmp | sed 's/\ //g' > m2_name.txt
rm *.tmp
cat *.txt
