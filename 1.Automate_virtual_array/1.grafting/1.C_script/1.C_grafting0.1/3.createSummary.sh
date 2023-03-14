#!/bin/bash
#BINDERS SUMMARY
printf "  ID  | Budcore# | #fitRot | #MaxFitSp | Wiggled? | Exp Binder? | Spr |Sequence\n" > Summary.txt
printf " ID | Exp | CCG | Match? | Sequence\n" > stats.txt
printf "" > statsIncorrect.txt
good=0
bad=0
corPredBinders=0;
for id in `cat grafted-IDs.txt | sort | uniq`; 
do
    #echo "Doing $id"
    #totExpBinders=`cat EXP_BINDERS.txt | sed 's/[^0-9]*//g' | sort | uniq | grep -c "."`
    #totExpBinders=`grep -c "." EXP_BINDERS.txt`
    expBinder=`grep "^$id$" EXP_BINDERS.txt`
    #echo $expBinder
    if [ -z "$expBinder" ]; then
        expBinder='N'
    else
        expBinder='Y'
    fi
    predBinder='N' # if any spacer of any rotamers of any of the buds fit then=Y
    for bud in `ls Structures/$id-* | grep -o '..of.' | sed 's/_//g' | sort | uniq`
    do
       # echo -ne "Working on $id $bud\n"
        printf "%4s|" $id >> Summary.txt 
        printf "  %6s  |" $bud >> Summary.txt

        rotamers=`grep -l TOTAL Overlaps/$id-* | grep $bud | grep -c "."`
        fittingrotamers=`grep -l BINDER Overlaps/$id-* | grep $bud | grep -c "."`
        printf " %2d of%2d |" $fittingrotamers $rotamers >> Summary.txt

        maxFitRot=0
        for rot in `ls Structures/$id-* | xargs -n1 basename | grep $bud`
        do
            fitRot=`grep -l BINDER Overlaps/$rot*`
            if [[ -n "$fitRot" ]]; then # if not NULL
                numFitSpacers=`grep TOTAL Spacers/$rot* | cut -d = -f 2`
                if [ $numFitSpacers -gt $maxFitRot ]; then
                    maxFitRot=$numFitSpacers
                    predBinder='Y'
                fi
            fi
        done
        printf "    %3d    |" $maxFitRot >> Summary.txt
                
        wiggled=`grep "PostWiggle.*BINDER" Overlaps/$id-*$bud*`
        if [ -z "$wiggled" ]; then
            printf "    N     |" >> Summary.txt
        else
            printf "    Y     |" >> Summary.txt
        fi

        printf "      %1c      |" $expBinder >> Summary.txt 

        spacer=` grep -A1 "^$id$" spacer_id.txt | grep S`
        printf " %3s |" $spacer >> Summary.txt

	sequence=`grep "^$id"$'\t'"" GLYCANS.txt | cut -f2`
        printf " $sequence" >> Summary.txt
        printf "\n" >> Summary.txt
    done
    if [[ $expBinder == $predBinder ]]; then
        match=Good
        good=$(($good+1))
    else
        match=Bad
        bad=$(($bad+1))
    fi
    #if [[ $expBinder == 'Y' ]]; then 
    #    if [[ $predBinder == 'Y' ]]; then
            #printf "%c,%c\n" $expBinder $predBinder
    #        corPredBinders=$(($corPredBinders+1));
    #    fi
    #fi
    printf " %5s|  %1c  |  %1c  |  %4s  | %s\n" $id $expBinder $predBinder $match $sequence >> stats.txt
done

bTotal=0
nbTotal=0
ncb=0 # number of correct binders
ncnb=0 #number of correct non binders
for id in `cat grafted-IDs.txt | sed 's/[^0-9]*//g' | sort | uniq `
do
    expBinder=`grep "^$id[a-b]*$" EXP_BINDERS.txt`
    if [ -z "$expBinder" ]; then
        nbTotal=$(($nbTotal+1))
        inCorPredNonBind=`grep " $id[a-b]*|  N  |  Y" stats.txt`
        if [ -z "$inCorPredNonBind" ]; then
            ncnb=$(($ncnb+1))
        else 
            echo "id=$id, non-binder, incorrect" >> statsIncorrect.txt
        fi
    else
        bTotal=$((bTotal+1))
        corPredBinder=`grep " $id[a-b]*|  Y  |  Y" stats.txt`
        if [ -n "$corPredBinder" ]; then
            ncb=$(($ncb+1))
        else
            echo "id=$id, binder, incorrect" >> statsIncorrect.txt
        fi
    fi
    
done


#total=$(($good+$bad))
printf "Correctly predicted %d non-binders out of %d non-binders\n" $ncnb $nbTotal >> stats.txt

printf "Correctly predicted %d binders out of %d experimental binders\n" $ncb $bTotal >> stats.txt

printf "Experimental binders which were not grafted:\n" >> stats.txt
for id in `cat EXP_BINDERS.txt | sed 's/[^0-9]*//g' | sort | uniq `
do
    found=`grep "^$id$" grafted-IDs.txt`
    foundX=`grep "^$id[a-b]" grafted-IDs.txt`
    if [ -z "$found" ]; then
        if [ -z "$foundX" ]; then
            printf "%s, " $id >> stats.txt
        fi
    fi
done
printf "\n" >> stats.txt
cat stats.txt
