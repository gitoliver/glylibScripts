#!/bin/bash
#CFG2gmml runs 3 times to fix all the sulfates
./0.rename_v5.0.sh 
./1.CFG2gmml output0.txt > output1.txt
./1.CFG2gmml output1.txt > output2.txt
./1.CFG2gmml output2.txt > output3.txt
./2.fix_sulfates.sh
./3.create_ab_term.sh
