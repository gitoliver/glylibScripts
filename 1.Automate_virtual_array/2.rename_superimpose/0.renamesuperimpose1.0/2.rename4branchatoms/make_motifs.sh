#/usr/bin/env bash
#expects MOTIF.pdb which is a formatted pdb file produced by GLYCAM.WEB containing the "rootstock" or "core" structure


#nomenclature
#.tmp files are removed at the end, .txt files are kept
#S stands for sourceable. nmotif is the number of motifs

#counts number of motifs 
cat MOTIF.pdb | grep LINK\ | grep -c -v ROH > nmotifs.tmp
awk '{print"num=\"",$1"\""}' nmotifs.tmp > Snmotif1.tmp
cat Snmotif1.tmp | sed s'/\ //'g > Snmotif.tmp #removes the space
source Snmotif.tmp

awk '{print$1}' nmotifs.tmp > Snmotif.txt

cat MOTIF.pdb | grep LINK\ | grep -v ROH > motifs.tmp

for ((i=1;i<=$num;i++))

do
awk "NR==$i" motifs.tmp > motif_$i.tmp #read the i'th line 
awk '{print$2}' motif_$i.tmp > Smotif_resid1_aN_$i.txt #stores atom name
awk '{print$5}' motif_$i.tmp > Smotif_resid2_aN_$i.txt #stores linked atom name
awk '{print$4}' motif_$i.tmp > Smotif_resid1_rn_$i.txt #stores residue number
awk '{print$7}' motif_$i.tmp > Smotif_resid2_rn_$i.txt #stores linked residue number

done
rm -rf 9.MOTIFS
mkdir 9.MOTIFS
mv S*motif*.txt 9.MOTIFS
rm *.tmp 2>/dev/null
