
var folder = "$1/";
var prefix = "$2";
var end = ".pdb";
var name = prefix.concat(end);
var i = $3;
var j = i-1;
glycan = loadPdb(name);
var state="";
//State A
glycan.setDihedral(i, "C1", i, "C2", j, "O8", j, "C8", -79.5);
glycan.setDihedral(i, "C2", j, "O8", j, "C8", j, "H8", -29.9);
glycan.setDihedral(j, "H7", j, "C7", j, "C6", j, "H6", -67.8);
glycan.setDihedral(j, "H8", j, "C8", j, "C7", j, "H7", 69.9);
glycan.setDihedral(j, "O9", j, "C9", j, "C8", j, "C7", -174.2);
state="-A";
glycan.printPdbFile(folder + prefix + state + end);

//State B
glycan.setDihedral(i, "C1", i, "C2", j, "O8", j, "C8", -73.6);
glycan.setDihedral(i, "C2", j, "O8", j, "C8", j, "H8", -8.8);
glycan.setDihedral(j, "H7", j, "C7", j, "C6", j, "H6", -67.9);
glycan.setDihedral(j, "H8", j, "C8", j, "C7", j, "H7", 70.8);
glycan.setDihedral(j, "O9", j, "C9", j, "C8", j, "C7", 59.0);
state="-B";
glycan.printPdbFile(folder + prefix + state + end);

//State C
glycan.setDihedral(i, "C1", i, "C2", j, "O8", j, "C8", -172.0);
glycan.setDihedral(i, "C2", j, "O8", j, "C8", j, "H8", -9.3);
glycan.setDihedral(j, "H7", j, "C7", j, "C6", j, "H6", -58.9);
glycan.setDihedral(j, "H8", j, "C8", j, "C7", j, "H7", 75.8);
glycan.setDihedral(j, "O9", j, "C9", j, "C8", j, "C7", -171.5);

state="-C";
glycan.printPdbFile(folder + prefix + state + end);

//State D
glycan.setDihedral(i, "C1", i, "C2", j, "O8", j, "C8", -72.1);
glycan.setDihedral(i, "C2", j, "O8", j, "C8", j, "H8", 9.1);
glycan.setDihedral(j, "H7", j, "C7", j, "C6", j, "H6", -72.1);
glycan.setDihedral(j, "H8", j, "C8", j, "C7", j, "H7", -61.5);
glycan.setDihedral(j, "O9", j, "C9", j, "C8", j, "C7", -61.1);
state="-D";
glycan.printPdbFile(folder + prefix + state + end);

//State E
glycan.setDihedral(i, "C1", i, "C2", j, "O8", j, "C8", -42.9);
glycan.setDihedral(i, "C2", j, "O8", j, "C8", j, "H8", 44.3);
glycan.setDihedral(j, "H7", j, "C7", j, "C6", j, "H6", -61.7);
glycan.setDihedral(j, "H8", j, "C8", j, "C7", j, "H7", -178.3);
glycan.setDihedral(j, "O9", j, "C9", j, "C8", j, "C7", 61.5);
state="-E";
glycan.printPdbFile(folder + prefix + state + end);
