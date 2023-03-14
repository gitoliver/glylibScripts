#!/bin/bash
#BINDERS SUMMARY
printf " ID | motif id | #fitRot | #MaxFitSp | Wiggled? | Exp Binder? | Spr |Sequence\n" > Summary.txt
printf "  ID  | Exp | CCG | Match? | Spr | Sequence\n" > stats.txt
printf "" > statsIncorrect.txt
good=0
bad=0
numGoodSprPred=0
numBadSprPred=0
corPredBinders=0;
for id in `cat grafted-IDs.txt | sort | uniq`; 
do
    printf "%4s|" $id >> Summary.txt
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
    predBinder='N' # if any rotamers and spacer fit of any of the buds then=Y
    numBud=0
    maxFitRot=0
    #Check to make sure structures were generated. Branches can induce conformation changes preventing binding.
   
    shopt -s nullglob
    files=(Structures/$id-*) 
    if [ ${#files[@]} -eq 0 ]; then
        shopt -u nullglob
        sequence=`grep "^$id"$'\t'"" GLYCANS.txt | cut -f2`
        spacer=`grep -A1 "^$id$" spacer_id.txt | grep -v "^$id$"`
        printf " No Structures, see logfile. Check ligand.pdb vs library glycan conformation |      %1c      | %3s | %20s \n" $expBinder $spacer $sequence >> Summary.txt
    else
        shopt -u nullglob
        for bud in `ls Structures/$id-* | grep -o '..of.' | sed 's/_//g' | sort | uniq`
        do
           # echo -ne "Working on $id $bud\n"
            numBud=$(($numBud + 1))
            if [[ $numBud -gt 1 ]]; then
                #printf " -- |" >> Summary.txt #fill id gap with blank for multiple budcores
                printf "%4s|" $id >> Summary.txt #fills in id for each line. allows excel sorting
            fi
            budNum=`echo $bud | cut -d o -f1`
            printf "    %2s    |" $budNum >> Summary.txt

            rotamers=`grep -l TOTAL Overlaps/$id-* | grep $bud | grep -c "."`
            fittingrotamers=`grep -l BINDER Overlaps/$id-* | grep $bud | grep -c "."`
            printf " %2d of%2d |" $fittingrotamers $rotamers >> Summary.txt
            if [ $fittingrotamers -gt $maxFitRot ]; then
                maxFitRot=$fittingrotamers
            fi

            #Check if there are spacers used and if so are there any shapes that fit.
            maxFitSpr=0
            spacer=` grep -A1 "^$id$" spacer_id.txt | grep -v "^$id$"`
            if [ -z "$spacer" ]; then # If there were no spacers found for glycan e.g glycome screen
                if [ $fittingrotamers -gt 0 ]; then # if there are fitting rotamers
                    predBinder='Y'
                    maxFitSpr=100
                fi
            else
                for rot in `ls Structures/$id-* | xargs -n1 basename | grep $bud`
                do
                    fitRot=`grep -l BINDER Overlaps/$rot*`
                    if [[ -n "$fitRot" ]]; then # if not NULL
                        numFitSpacers=`grep TOTAL Spacers/$rot* | cut -d = -f 2`
                        if [ $numFitSpacers -gt $maxFitSpr ]; then
                            maxFitSpr=$numFitSpacers
                            if [  $maxFitSpr -gt 5 ]; then
                                predBinder='Y'
                            fi
                        fi
                    fi
                done
            fi
            printf "    %3d    |" $maxFitSpr >> Summary.txt
                    
            wiggled=`grep "PostWiggle.*BINDER" Overlaps/$id-*$bud*`
            if [ -z "$wiggled" ]; then
                printf "    N     |" >> Summary.txt
            else
                printf "    Y     |" >> Summary.txt
            fi

            printf "      %1c      |" $expBinder >> Summary.txt 
     
            spacer=` grep -A1 "^$id$" spacer_id.txt | grep -v "^$id$"`
            if [ -z "$spacer" ]; then
                 printf " N/A |" >> Summary.txt
            else
                printf " %3s |" $spacer >> Summary.txt
            fi

            sequence=`grep "^$id"$'\t'"" GLYCANS.txt | cut -f2`
            printf " %20s " $sequence >> Summary.txt
            printf "\n" >> Summary.txt
        done
    fi
    #Determine if spacers had a good or bad effect on the results
    sprEff='N'
    if [ -n "$spacer" ]; then
        if [ $expBinder == 'Y' ]; then
            if [ $maxFitRot -gt 0 ]; then
                if [ $predBinder == 'N' ]; then
                    printf " BadSpacerPred $id $spacer \n"
                    sprEff='Y'
                fi
            fi
        elif [ $expBinder == 'N' ]; then
            if [ $maxFitRot -gt 0 ]; then
                if [ $predBinder == 'N' ]; then
                   printf " GoodSpacerPred $id $spacer \n"
                   sprEff='Y'
                fi
            fi
        fi
    fi
    #printf "%4s|  %6s  |" $id $bud >> Summary.txt
    if [[ $expBinder == $predBinder ]]; then
        match=Good
        good=$(($good+1))
    else
        match=Bad
        bad=$(($bad+1))
    fi
    #if [[ $expBinder == 'Y' ]]; then 
    #    if [[ $predBinder == 'Y' ]]; then
    #        printf "%c,%c\n" $expBinder $predBinder
    #        corPredBinders=$(($corPredBinders+1));
    #    fi
    #fi
    printf " %5s|  %1c  |  %1c  |  %4s  |  %1c  | %s\n" $id $expBinder $predBinder $match $sprEff $sequence >> stats.txt
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
            goodSprPred=`grep " $id[a-b]*|  N  |  N  |  Good  |  Y" stats.txt`
            if [ -n "$goodSprPred" ]; then
                numGoodSprPred=$(($numGoodSprPred + 1))
            fi
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
            badSprPred=`grep " $id[a-b]*|  Y  |  N  |   Bad  |  Y" stats.txt`
            if [ -n "$badSprPred" ]; then
                numBadSprPred=$(($numBadSprPred+ 1))
            fi
        fi
    fi
    
done


#total=$(($good+$bad))
printf "Correctly predicted %d non-binders out of %d non-binders\n" $ncnb $nbTotal >> stats.txt

printf "Correctly predicted %d binders out of %d experimental binders\n" $ncb $bTotal >> stats.txt

printf "Good Spacer effects on Exp non-binders %d\nBad Spacer Effects on Exp binders %d\n" $numGoodSprPred $numBadSprPred >> stats.txt

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
numbicc=`grep -c "Branch Induces Conformational Change" Summary.txt`
printf "Branch-induced conformation change in %d glycans\n" $numbicc >> stats.txt
cat stats.txt
sed 's/|/,/g' Summary.txt > Summary.csv
