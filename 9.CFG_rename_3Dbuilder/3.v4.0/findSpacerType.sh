#!/bin/bash

for id in `cut -f 1 GLYCANS.txt`; 
do  #find the id at the beginning of a line with a tab after. Grep out the spacer
    Sp=`grep -P "^"$id"\t" input.dat | grep -o "Sp.*"`;
    if [ -z "$Sp" ] # if $Sp is null, try remove the last a/b character
    then
        #echo "\$Sp is null."
        sid=`echo $id | sed 's/b//g' | sed 's/a//g'`
        Sp=`grep -P "^"$sid"\t" input.dat | grep -o "Sp.*"`; # try again with sid
        if [ -z "$Sp" ] # if $Sp is STILL null,
        then #do fuck all
            echo ""
            #echo "Couldn't find Sp linker for $id"
        else
            echo $id;
            S=`echo $Sp | sed 's/p//g'`;
            echo $S;
        fi
    else # If Sp is in there print it out, removing the p.
        #echo "\$Sp is NOT null."
        echo $id;
        S=`echo $Sp | sed 's/p//g'`;
        echo $S;
    fi     # $String is null.
done


