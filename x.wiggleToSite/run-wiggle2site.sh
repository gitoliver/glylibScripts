folder="../0.grafting/Structures"

echo "requires ids.txt"
i=0

for id in `cat ids.txt`
do
    mkdir $i.glycan$id
    cp $folder/$id-* $i.glycan$id
    cd $i.glycan$id
     j=0
     for f in `ls $id-*`
     do
         mkdir $j.$f.site6
         grep -v TER $f > $j.$f.site6/glycan.pdb
         cp ../headGroup.pdb $j.$f.site6
         cd $j.$f.site6
          core=`echo $f | grep -oh "budcore_.*of" | cut -d _ -f2 | cut -d o -f1`
          move=`ls ../*.pdb | grep -v $core | grep -oh "budcore_.*of" | cut -d _ -f2 | cut -d o -f1 | sort -u`
          wiggle-rounders.sh glycan.pdb headGroup.pdb $move 9 $core
         cd ../
         j=$(($j + 1))

         mkdir $j.$f.site9
         grep -v TER $f > $j.$f.site9/glycan.pdb
         cp ../headGroup.pdb $j.$f.site9
         cd $j.$f.site9
          core=`echo $f | grep -oh "budcore_.*of" | cut -d _ -f2 | cut -d o -f1`
          move=`ls ../*.pdb | grep -v $core | grep -oh "budcore_.*of" | cut -d _ -f2 | cut -d o -f1 | sort -u`
          wiggle-rounders.sh glycan.pdb headGroup.pdb $move 6 $core
         cd ../
         j=$(($j + 1))
     done
     
    cd ../
    i=$(($i + 1))
done 
