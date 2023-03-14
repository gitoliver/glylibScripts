#!/bin/bash
echo -e "Node \tState \tPeople" 
x=1
while [ $x -ne 0 ]
do
    date
    date >> zombie.log
    qstat -n >> zombie.log
    for f in `pbsnodes -a | grep Mom | sed 's/     Mom = //g'`
    do
        ssh $f 'job_counter.sh '$f';exit'
    done
    sleep 1m
done
