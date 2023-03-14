#!/bin/bash
# Pass in node to $1


#Count the number of processes running
ps -elf | grep g09 | grep -v "grep" | awk '{print $3}' > tmp.dat
ps -elf | grep smmp | grep -v "grep" | grep -v mpirun |awk '{print $3}' >> tmp.dat
ps -elf | grep sander | grep -v "grep" | grep -v mpirun | awk '{print $3}' >> tmp.dat
ps -elf | grep pmemd | grep -v "grep" | grep -v mpirun | awk '{print $3}' >> tmp.dat
ps -elf | grep mdrun | grep -v "grep" | grep -v mpirun | awk '{print $3}' >> tmp.dat
num=`grep -c "." tmp.dat`
numGauss=`ps -elf | grep g09 | grep -v "grep" | awk '{print $3}' | grep -c "."`

state=`pbsnodes -a | grep -A2 "Mom = $1" | grep state | sed 's/     state = //g'`
if [ "$state" == "free" ]; then
norm=0
fi
if [ "$state" == "job-busy" ]; then
state="busy"
norm=8
fi

if [ $numGauss -eq 2 ]; then
    if [ $num -gt 2 ]; then
        echo -n -e "$1 \t$state \t"
        for f in `sort tmp.dat | uniq`
            do
            echo -n -e "$f "
            done
	echo ""
    fi
elif [ $num -ne $norm ]; then
    echo -n -e "$1 \t$state \t"
    for f in `sort tmp.dat | uniq`
        do
        echo -n -e "$f "
        done
    echo ""
fi
rm tmp.dat
