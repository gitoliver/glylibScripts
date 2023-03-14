#!/bin/bash
IFS=$'\n'
sh 1.make_2.create.sh
unset IFS
sh 2.create_2-8_torsions.sh tmp 215-A- 1
gems 215-A-.js
