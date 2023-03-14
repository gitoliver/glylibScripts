#!/bin/bash
> Results.txt
for f in `ls -d *.model/ | sort -n `
do
    echo "$f" >> Results.txt
    num=`cat "$f"logfile* |  grep -c "Could"` 
    echo "Sites not glycosylated: $num" >> Results.txt
    grep "Could" "$f"logfile* >> Results.txt
    echo "**********************************************************" >> Results.txt
done
