#!/bin/bash

echo "" > logfile
PATH=$PATH:/home/oliver/Dropbox/1.glylib_scripts/l.compare_structures_find_clash

for f in `ls original_files | sed 's/\.pdb//g'`
do
    isInternal=`grep LINK pdb_Files/$f* | grep -c "8S.*8S."`
    if [ $isInternal -gt 0 ]; then
        for g in `ls removed_internal_clashes/$f-*`
        do
            compare_find_clash.exe pdb_Files.bak/$f.pdb $g >> logfile
        done
    fi
done

grep "Internal Bonds:" logfile | cut -d : -f2 > removelist

for h in `cat removelist`
do
    echo "removing $h"
    rm $h
done
echo "fin"
