echo > $2.js
if [ $# -ne 3 ]; then echo "Usage: 2.create_2-8_torsions.sh folder prefix linkage"; else 
echo "var folder = \"$1/\";" >> $2.js 
echo "var prefix = \"$2\";" >> $2.js 
echo "var end = \".pdb\";" >> $2.js 
echo "var name = prefix.concat(end);" >> $2.js 
echo "var i = $3;" >> $2.js 
echo "var j = i-1;" >> $2.js 
echo "glycan = loadPdb(name);" >> $2.js 
echo "var state=\"\";" >> $2.js 
echo "//State A" >> $2.js 
echo "glycan.setDihedral(i, \"C1\", i, \"C2\", j, \"O8\", j, \"C8\", -79.5);" >> $2.js 
echo "glycan.setDihedral(i, \"C2\", j, \"O8\", j, \"C8\", j, \"H8\", -29.9);" >> $2.js 
echo "glycan.setDihedral(j, \"H7\", j, \"C7\", j, \"C6\", j, \"H6\", -67.8);" >> $2.js 
echo "glycan.setDihedral(j, \"H8\", j, \"C8\", j, \"C7\", j, \"H7\", 69.9);" >> $2.js 
echo "glycan.setDihedral(j, \"O9\", j, \"C9\", j, \"C8\", j, \"C7\", -174.2);" >> $2.js 
echo "state=\"-A\";" >> $2.js 
echo "glycan.printPdbFile(folder + prefix + state + end);" >> $2.js 
echo "//State B" >> $2.js 
echo "glycan.setDihedral(i, \"C1\", i, \"C2\", j, \"O8\", j, \"C8\", -73.6);" >> $2.js 
echo "glycan.setDihedral(i, \"C2\", j, \"O8\", j, \"C8\", j, \"H8\", -8.8);" >> $2.js 
echo "glycan.setDihedral(j, \"H7\", j, \"C7\", j, \"C6\", j, \"H6\", -67.9);" >> $2.js 
echo "glycan.setDihedral(j, \"H8\", j, \"C8\", j, \"C7\", j, \"H7\", 70.8);" >> $2.js 
echo "glycan.setDihedral(j, \"O9\", j, \"C9\", j, \"C8\", j, \"C7\", 59.0);" >> $2.js 
echo "state=\"-B\";" >> $2.js 
echo "glycan.printPdbFile(folder + prefix + state + end);" >> $2.js 
echo "//State C" >> $2.js 
echo "glycan.setDihedral(i, \"C1\", i, \"C2\", j, \"O8\", j, \"C8\", -172.0);" >> $2.js 
echo "glycan.setDihedral(i, \"C2\", j, \"O8\", j, \"C8\", j, \"H8\", -9.3);" >> $2.js 
echo "glycan.setDihedral(j, \"H7\", j, \"C7\", j, \"C6\", j, \"H6\", -58.9);" >> $2.js 
echo "glycan.setDihedral(j, \"H8\", j, \"C8\", j, \"C7\", j, \"H7\", 75.8);" >> $2.js 
echo "glycan.setDihedral(j, \"O9\", j, \"C9\", j, \"C8\", j, \"C7\", -171.5);" >> $2.js 
echo "state=\"-C\";" >> $2.js 
echo "glycan.printPdbFile(folder + prefix + state + end);" >> $2.js 
echo "//State D" >> $2.js 
echo "glycan.setDihedral(i, \"C1\", i, \"C2\", j, \"O8\", j, \"C8\", -72.1);" >> $2.js 
echo "glycan.setDihedral(i, \"C2\", j, \"O8\", j, \"C8\", j, \"H8\", 9.1);" >> $2.js 
echo "glycan.setDihedral(j, \"H7\", j, \"C7\", j, \"C6\", j, \"H6\", -72.1);" >> $2.js 
echo "glycan.setDihedral(j, \"H8\", j, \"C8\", j, \"C7\", j, \"H7\", -61.5);" >> $2.js 
echo "glycan.setDihedral(j, \"O9\", j, \"C9\", j, \"C8\", j, \"C7\", -61.1);" >> $2.js 
echo "state=\"-D\";" >> $2.js 
echo "glycan.printPdbFile(folder + prefix + state + end);" >> $2.js 
echo "//State E" >> $2.js 
echo "glycan.setDihedral(i, \"C1\", i, \"C2\", j, \"O8\", j, \"C8\", -42.9);" >> $2.js 
echo "glycan.setDihedral(i, \"C2\", j, \"O8\", j, \"C8\", j, \"H8\", 44.3);" >> $2.js 
echo "glycan.setDihedral(j, \"H7\", j, \"C7\", j, \"C6\", j, \"H6\", -61.7);" >> $2.js 
echo "glycan.setDihedral(j, \"H8\", j, \"C8\", j, \"C7\", j, \"H7\", -178.3);" >> $2.js 
echo "glycan.setDihedral(j, \"O9\", j, \"C9\", j, \"C8\", j, \"C7\", 61.5);" >> $2.js 
echo "state=\"-E\";" >> $2.js 
echo "glycan.printPdbFile(folder + prefix + state + end);" >> $2.js 
fi
