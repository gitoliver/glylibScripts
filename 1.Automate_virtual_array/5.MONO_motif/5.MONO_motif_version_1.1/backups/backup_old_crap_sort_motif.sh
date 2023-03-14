#/usr/bin/env bash
#this now crazily messed up code finds the resname and linkage info from the MOTIF.pdb and creates motif1.txt and motif2.txt

#needs to be generalized for n number of motifs. Also now that I know exactly what I want I can re-write it.

#expects MOTIF.pdb
#outputs m*_name.txt and m*_linkage.txt which can be sourced as variables into the search_library.sh script
#only works for disacc motifs because of awk and grep -v ROH

cat MOTIF.pdb | grep LINK\ | grep -v ROH > motif.txt #takes relevant LINK from MOTIF.pdb

#FOR RESIDUE 1
awk '{print"m1_link=\"",$2"\""}' motif.txt > linkage1.tmp
cat linkage1.tmp | sed 's/\ //g' > m1_linkage.tmp #removes the space before atom name
#now gets the linkage residue name
awk '{print"",$3}' motif.txt > m1.tmp
cat m1.tmp | cut -c 3-4 > m11.tmp
awk '{print"m1_name=\".",$1"\""}' m11.tmp > m111.tmp
cat m111.tmp | sed 's/\ //g' > m1_name.tmp
source m1_name.tmp 
source m1_linkage.tmp
echo "motif1=\"$m1_link  $m1_name\"" > motif1.txt


#FOR RESIDUE 2
awk '{print"m2_link=\"",$5"\""}' motif.txt > linkage2.tmp
cat linkage2.tmp | sed 's/\ //g' > m2_linkage.tmp #removes the space before atom name
#now gets the linkage residue name
awk '{print"",$6}' motif.txt > m2.tmp
cat m2.tmp | cut -c 3-4 > m22.tmp
awk '{print"m2_name=\".",$1"\""}' m22.tmp > m222.tmp
cat m222.tmp | sed 's/\ //g' > m2_name.tmp
source m2_name.tmp
source m2_linkage.tmp
echo "motif2=\"$m2_link  $m2_name\"" > motif2.txt
rm *.tmp
cat *.txt
