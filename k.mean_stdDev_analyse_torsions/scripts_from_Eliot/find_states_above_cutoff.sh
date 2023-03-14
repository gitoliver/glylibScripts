#!/bin/bash
source ../0.INPUTS_EDIT_ME
statesFile=$1
i=0;
for state in `cut -d\  -f1 $1`; 
do 
    popn=`grep $state $statesFile | cut -c28-38 | sed 's/ //g'`; # Moving target 
    if [ $popn -gt $cutoff ]; then 
        echo "$state: $popn% " >> ../RESULTS.txt
        analyse_states.sh $state.dat $i;
        i=$(($i+1)) 
    fi
done
