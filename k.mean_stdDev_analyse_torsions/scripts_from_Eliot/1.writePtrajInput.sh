#!/bin/bash
source 0.INPUTS_EDIT_ME
i=$number_residues
echo "trajin "$trajName" "$trajtype"" > ptraj.in
echo "" >> ptraj.in 
echo "dihedral "$i"ome9 :"$i"@O9 :"$i"@C9 :"$i"@C8 :"$i"@C7 out ./dihedrals/"$i"_O9-C9-C8-C7_ome9.csv" >> ptraj.in
echo "dihedral "$i"ome8 :"$i"@H8 :"$i"@C8 :"$i"@C7 :"$i"@H7 out ./dihedrals/"$i"_H8-C8-C7-H7_ome8.csv" >> ptraj.in 
echo "dihedral "$i"ome7 :"$i"@H7 :"$i"@C7 :"$i"@C6 :"$i"@H6 out ./dihedrals/"$i"_H7-C7-C6-H6_ome7.csv" >> ptraj.in

for ((i=$number_residues; i>2; i--)) 
do
        j=$(($i-1))
        echo "" >> ptraj.in
	echo "dihedral "$i""$j"phi :"$i"@C1 :"$i"@C2 :"$j"@O8 :"$j"@C8 out ./dihedrals/"$i"-"$j"_C1-C2-O8-C8_phi.csv" >> ptraj.in
	echo "dihedral "$i""$j"psi :"$i"@C2 :"$j"@O8 :"$j"@C8 :"$j"@H8 out ./dihedrals/"$i"-"$j"_C2-O8-C8-H8_psi.csv" >> ptraj.in
	echo "dihedral "$i""$j"ome8 :"$j"@H8 :"$j"@C8 :"$j"@C7 :"$j"@H7 out ./dihedrals/"$i"-"$j"_H8-C8-C7-H7_ome8.csv" >> ptraj.in
	echo "dihedral "$i""$j"ome7 :"$j"@H7 :"$j"@C7 :"$j"@C6 :"$j"@H6 out ./dihedrals/"$i"-"$j"_H7-C7-C6-H6_ome7.csv" >> ptraj.in
	echo "dihedral "$i""$j"ome9 :"$j"@O9 :"$j"@C9 :"$j"@C8 :"$j"@C7 out ./dihedrals/"$i"-"$j"_O9-C9-C8-C7_ome9.csv" >> ptraj.in
done

echo "#!/bin/bash" > 2.run_cpptraj.sh
echo "rm -r dihedrals.bak/" >> 2.run_cpptraj.sh
echo "mv RESULTS.txt RESULTS.txt.bak" >> 2.run_cpptraj.sh
echo "mv dihedrals dihedrals.bak" >> 2.run_cpptraj.sh
echo "mkdir dihedrals" >> 2.run_cpptraj.sh
echo "cpptraj -p "$topName" -i ptraj.in" >> 2.run_cpptraj.sh
chmod +x 2.run_cpptraj.sh
echo "ptraj.in created, Now run 2.run_cpptraj.sh"
