#!/bin/bash

#Take only glycans linked via Sp types
grep -v "ol" input.dat | grep "Sp" input.dat > tmp1

#Now create a unique tag including the glycan id to look for. Solves greping for id "3" and getting crap back.
echo -n "" > tmp2
for f in `cut -f1 tmp1`; do echo 'k'$f'k' >> tmp2; done
paste tmp2 tmp1 > tmp3

#place a space at the end of each line in tmp3 so when searching for "Sp.." you'll still get Sp0, etc
sed -i 's/$/ /' tmp3 

#Now match up id's to spacer type.
echo -n "" > tmp4
for f in `cut -d b -f1 tmp1 | cut -d a -f1`; do echo "$f" >> tmp4 ; grep "k$f\k" tmp3 | grep -o "Sp.." >> tmp4 ; echo "" >> tmp4 ;done

#Take only those glycans with an Sp linkage.
grep -B1 "Sp" tmp4 | grep -v '\-\-' > tmp5

for f in `grep -P "a\t" GLYCANS.txt | cut -f1` 
do
    g=`echo $f | cut -d a -f1`
    sp=`grep "k$g\k" tmp3 | grep -o "Sp.."`
    echo -e "$f\n$sp" >> tmp5
done

for f in `grep -P "b\t" GLYCANS.txt | cut -f1`
do
    g=`echo $f | cut -d b -f1`
    sp=`grep "k$g\k" tmp3 | grep -o "Sp.."`
    echo -e "$f\n$sp" >> tmp5
done

sed 's/Sp/S/g' tmp5 > spacer_id.txt

#rm tmp*
