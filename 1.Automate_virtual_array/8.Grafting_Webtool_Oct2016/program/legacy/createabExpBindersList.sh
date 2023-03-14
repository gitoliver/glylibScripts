#!/bin/bash
if [ -f EXP_BINDERS.txt ]; then
    for binder in `cat EXP_BINDERS.txt`
    do
        printf ''$binder'\n'$binder'a\n'$binder'b\n' >> temp.txt
    done
    mv temp.txt EXP_BINDERS.txt
fi

