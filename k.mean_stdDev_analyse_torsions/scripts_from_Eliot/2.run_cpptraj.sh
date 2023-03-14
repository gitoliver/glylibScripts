#!/bin/bash
rm -r dihedrals.bak/
mv RESULTS.txt RESULTS.txt.bak
mv dihedrals dihedrals.bak
mkdir dihedrals
cpptraj -p 6mer_28-Neu5Ac.top -i ptraj.in
