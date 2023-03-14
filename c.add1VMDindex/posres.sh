#!/bin/bash
# Script written by Oliver Grant(olivercgrant@gmail.com) 10July2012
# Converts from vmd TkC indexes to restraint file for amber and gromacs.

echo "
# Script to add 1 to each atom number in index.ndx.
# Required as vmd indices are 1 off from atom numbers in gromacs and AMBER.
# Requires input.txt to contain atom numbers copied from vmd TkC console. No other strings.
"
for f in `cat input.txt`; 
do 
	echo "$(($f+1))"; 
done | sed ':a;N;$!ba;s/\n/ /g' > gromacs.ndx # adds one and puts into one row

# Now creating a restraint file for amber
for f in `cat gromacs.ndx`;
do
	printf '%s%8s%8s\n' "ATOM "  "$f" "$f";
done > ambermask.txt
echo "END" >> ambermask.txt
echo "END" >> ambermask.txt

# Now creating a restraint file for gromacs
echo "; position restraints produced using vmd TkC and posre.sh script" > gmxposre.itp
echo "" >> gmxposre.itp
echo "[ position_restraints ]" >> gmxposre.itp
echo ";  i funct       fcx        fcy        fcz" >> gmxposre.itp
force=1000;
for f in `cat gromacs.ndx`;
do
	printf '%4s%5s%11s%11s%11s\n' "$f" "1" "$force" "$force" "$force";
done >> gmxposre.itp

echo "# Created:";
echo "# gmxposre.itp (a position restraint file for gromacs)";
echo "# gromacs.ndx (an index file for gromacs)";
echo "# ambermask.txt (a position restraint file for amber input files)";
echo "# Remember to specify strength of restraints in your amber input"
