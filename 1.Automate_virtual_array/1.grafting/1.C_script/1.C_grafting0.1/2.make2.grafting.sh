#!/bin/bash
# creates 2.grafting.sh which breaks up the pdbs to be grafted into groups of size loop_interval and calls grafting. This way grafting does not eat all the memory and seg fault when using swap. 
# 
interval=1;
num_files=`grep pdb -c search_results.txt`;
let num_loops="(($num_files+($interval-1))/$interval)"; # round off

echo "#!/bin/bash" > 2.grafting.sh
#echo "mv RESULTS_library_search/ RESULTS_library_search.bak" >> 2.grafting.sh
echo "rm -rf Structures/ Overlaps/ Spacers/ Spacer3Dfiles/ RESULTS_grafting/ RESULTS_library_search/ 2>/dev/null" >> 2.grafting.sh
#echo "mkdir RESULTS_library_search/" >> 2.grafting.sh
echo "mkdir RESULTS_grafting/" >> 2.grafting.sh
echo "mkdir RESULTS_library_search/" >> 2.grafting.sh
echo "mkdir Structures/" >> 2.grafting.sh
echo "mkdir Overlaps/ Spacers/ Spacer3Dfiles/" >> 2.grafting.sh
echo "rm grafting.log.file* 2>/dev/null" >> 2.grafting.sh
echo "rm log.file 2>/dev/null" >> 2.grafting.sh
echo "rm grafted-IDs.txt 2>/dev/null" >> 2.grafting.sh
echo "echo \"Copying files\"" >> 2.grafting.sh


echo "i=1;" >> 2.grafting.sh
echo "j=1;" >> 2.grafting.sh
echo "loop_interval=$interval;" >> 2.grafting.sh

for ((i=1;i<=$num_loops;i++))
do
	echo "num_loops=$num_loops, loops left=$num_loops-$i);"
	echo "let k=(\$i*\$loop_interval) # had to use k as couldn't get \$i*\$loop_interval to evaluate in while line" >> 2.grafting.sh
	#echo "echo \"about to do up to \$k\"; sleep 2s;" >> 2.grafting.sh
	echo "while [[ \$j -le \$k && \$j -le $num_files ]];" >> 2.grafting.sh
	echo "do" >> 2.grafting.sh
	echo "	filename=\`sed -n ''\$j'p' search_results.txt\`" >> 2.grafting.sh
	echo "	cp \$filename RESULTS_library_search/" >> 2.grafting.sh
	echo "	let j+=1;" >> 2.grafting.sh
	#echo "	echo \"j=\$j\"" >> 2.grafting.sh
	echo "done" >> 2.grafting.sh
	echo "date >> log.file" >> 2.grafting.sh
	echo "free -m >> log.file" >> 2.grafting.sh
	echo "echo \"\" >> log.file" >> 2.grafting.sh 
	echo " echo \"Grafting...\" ">> 2.grafting.sh
	echo "2.grafting.exe >> grafting.log.file\$k" >> 2.grafting.sh
	echo " echo \"...Finished \$k\"" >> 2.grafting.sh
	echo "mv RESULTS_library_search/* RESULTS_grafting/" >> 2.grafting.sh
	echo "let i+=1;" >> 2.grafting.sh
	
	echo "mv RESULTS_grafting/*spacer* Spacers 2>/dev/null # no need to hear if there isn't anything there" >> 2.grafting.sh
	echo "mv RESULTS_grafting/*overlap* Overlaps 2>/dev/null # no need to hear if there isn't anything there" >> 2.grafting.sh
        echo "mv RESULTS_grafting/*Sp* Spacer3Dfiles/ 2>/dev/null # no need to hear if there isn't anything there" >> 2.grafting.sh
	echo "mv RESULTS_grafting/*budcore* Structures/ 2>/dev/null" >> 2.grafting.sh

	echo "" >> 2.grafting.sh
done
echo "3.createSummary.sh" >> 2.grafting.sh
chmod +x 2.grafting.sh
echo "finished! Search grafting.log.file for \"CHECK STRUCTURE\" as these glycans will have problems and may segfault"
