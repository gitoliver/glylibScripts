#!/bin/bash

folder=$1

export python=/usr/local/bin/python2.7
GRAFTING_HOME=/programs/site_deps/Grafting/program
export GRAFTING_HOME
PATH=$GRAFTING_HOME:$PATH
export GEMSHOME=/programs/temp/gems_grafting_2015Dec16-1
PATH=$PATH:$GEMSHOME
export LD_LIBRARY_PATH=/programs/lib:/user/local/bin:$LD_LIBRARY_PATH

############# Create ligand.pdb, LINK.txt, receptor.pdb ################
#Remove any hydrogens. Slows program and crashes grafting
grep -v "^.............H" complex.pdb | grep -v "^............H" > cocomplex.pdb

#Creates complex.pdb with glycan in GLYCAM nomenclature. Has flaws I deal with below.
/usr/local/bin/python2.7 $GEMSHOME/testbin/pdb2glycam.py -pdb cocomplex.pdb > pdb2glycam.out
#Separate out receptor part:
grep "^ATOM" glycam_pdb.pdb > receptor.pdb

#Find carbohydrate and put into ligand.pdb. 
#Find carb LINKs and put into LINK.txt
#Carb has been reunumbered from original by pdb2glycam. 
#Grafting requires ROH as resid 1 and for ROH to be topmost in LINK and HETATM sections. (Actually ROH or OME or TBT).
#Residues listed in pdb2glycam output pdb file will either start or end in the ROH.
#Must find if ROH is first or last residue number and find other residues
#Have no idea what pdb2glycan has renumbered and renamed. ie Hard to find the carb part
#I look for ROH and number of cycles pdb2glycam found, then use that info to grep out carb part from glycam_pdb.pdb

#Rerun pdb2glycam to find out what residues in new file are carb. Sounds silly, but best right now.
mv glycam_pdb.pdb rerun.pdb
/usr/local/bin/python2.7 $GEMSHOME/testbin/pdb2glycam.py -pdb rerun.pdb > pdb2glycam1.out
sed -n -e '/Cycles after discarding/,$p' pdb2glycam1.out | sed -e '/Detailed information/,$d' | grep _ > carbs
numRes=`grep -c "." carbs`

>tmp
line=1
for resname in `cut -d _ -f3 carbs`
do
    resid=`sed -n ''$line'p' carbs | cut -d _ -f5`
    grep " $resname .* $resid " glycam_pdb.pdb | grep -v "LINK"  >> tmp
    let line+=1
done

#Find reducing residue number and recursively look through LINK cards for connected residues. Renumber from 1.
>temp.pdb
>tempLINK.txt
echo "count=1" > count.txt
resid=`grep HETATM glycam_pdb.pdb | grep -m1 'ROH\|OME\|TBT' | cut -c 23-26 | sed 's/ //g'`
/programs/site_deps/Grafting/program/1.recursive-find-connections.sh $resid $folder > resConversion
#Now renumber residues in LINK section
for f in `cat resConversion`
do
    old=`echo $f | cut -d = -f1`
    new=`echo $f | cut -d = -f2`
    IFS='%' # otherwise whitespace in stringi gets removed  
    stringi=`printf "%"${#old}"s\n" $new`
    sed -i 's/ '$old' / '$stringi' /g' tempLINK.txt
    unset IFS
done

mv tempLINK.txt ligand.pdb
cat temp.pdb >> ligand.pdb

#Found this in one pdb file. Don't know how common it is.
sed -i 's/O1L/O1 /g' ligand.pdb

#Reorder LINK cards so O are on left and C are on right. 
sed -i 's/LINK\(..............................\)\(..............................\)/LINK\2\1/g' ligand.pdb
#Get rid of chain IDs. This could be part of previous command, but I got frustrated.
sed -i 's/LINK\(.................\)\(.\)\(............\)\(.................\)\(.\)\(............\)/LINK\1 \3\4 \6/g' ligand.pdb

# Now take fixed LINK cards

grep LINK ligand.pdb | cut -c 1-70 > LINK.txt # cut out bond length info. breaks grafting 
 
#########################Check everything went ok#######################

check1=$numRes
check2=`grep -c "." ligand.pdb`
check3=`grep -c "." LINK.txt`
check4=`grep -c "." receptor.pdb`
structure=`grep -A1 Oligosacc pdb2glycam.out | grep -v Oligo`

if [ $check1 -eq 1 ] || [ $check2 -eq 0 ] || [ $check3 -eq 0 ] ; then
    echo "stop=1" > stop.txt
    echo "No carbohydrate residue detected in input file" >> status.txt
    echo "Here is what was detected: $structure" >> status.txt
    echo "Please read http://glycam.org/docs/othertoolsservice/utilities/grafting/instructions/preparing-input-file/" >> status.txt
    echo "If there is still a problem, please forward this status file to glycam@uga.edu" >> status.txt
else
    echo "Carbohydrate identified in input file:" >> status.txt
    printf "$structure\n" >> status.txt
    cat LINK.txt >> status.txt
    echo "If this is not GLYCAM nomenclature, our conversion program has failed, and the program will not find a match in the library. Please forward this status file to glycam@uga.edu" >> status.txt
fi
if [ $check4 -eq 0 ]; then
    echo "stop=1" > stop.txt
    echo "No protein residues detected in input file, you must place protein residues in the ATOM section" >> status.txt
fi
echo "" >> status.txt

######################## Edit List of Binders ##########################

if [ -f EXP_BINDERS.txt ]; then
    for binder in `cat EXP_BINDERS.txt`
    do
        printf ''$binder'\n'$binder'a\n'$binder'b\n' >> temp.txt
    done
    mv temp.txt EXP_BINDERS.txt
fi

########################################################################

