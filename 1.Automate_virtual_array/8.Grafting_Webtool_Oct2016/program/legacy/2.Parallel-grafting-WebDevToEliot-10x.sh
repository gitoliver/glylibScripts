#!/bin/bash

num_files=`grep pdb -c search_results.txt`;
source installerInputs.txt
source userInputs.txt

echo "#!/bin/bash" > submit.sh
echo "" >> submit.sh
echo "#PBS -q docking" >> submit.sh
echo "#PBS -l nodes=1:ppn=1" >> submit.sh
echo "#PBS -N Grafting" >> submit.sh
echo "#PBS -o grafting.out " >> submit.sh
echo "#PBS -e grafting.err " >> submit.sh
echo "export PATH=/usr/lib64/qt-3.3/bin:/usr/local/bin:/bin:/usr/bin:/usr/local/sbin:/usr/sbin:/sbin:/programs/e12/grafting/grafting" >> submit.sh
echo "" >> submit.sh
#echo "cd \$PBS_O_WORKDIR" >> submit.sh

rm -rf Structures/ Overlaps/ Spacers/ Spacer3Dfiles/ RESULTS_grafting/ RESULTS_library_search/ fittingSpacer3Dfiles/ 2>/dev/null
mkdir fittingSpacer3Dfiles/
mkdir RESULTS_grafting/
#mkdir RESULTS_library_search/
mkdir Structures/
mkdir Overlaps/ Spacers/ Spacer3Dfiles/
touch Spacers/tmp
rm grafting.log.file* 2>/dev/null
rm log.file 2>/dev/null
rm grafted-IDs.txt 2>/dev/null

source ../folderName
#cp -r ../../$folder /scratch1/webdev/grafting/runs/

num_cpus=32;
files_Per_CPU=$(( $num_files / $num_cpus ))
remainder=$(( $num_files % $num_cpus )) #Assign remainder to a CPU
if [[ $files_Per_CPU -eq 0 ]]; then
    num_cpus=1; #Have less files than num_cpus; just run on one CPU.
fi

printf "STATUS -> Running $num_files jobs on $num_cpus CPUs\n" > job-stats.txt
cp status.txt updated-status.txt; cat job-stats.txt >> updated-status.txt

i=1
file=1;
while [[ $i -le $num_cpus ]];
do
    if [[ $i -eq $num_cpus ]]; then
        let "files_Per_CPU += $remainder" # add remainder to last CPU
    fi
    #Make a directory to run grafting on this set of files
    rm -r $i.temp 2>/dev/null; mkdir $i.temp; mkdir $i.temp/RESULTS_library_search/;
    cp spacer_id.txt submit.sh LINK.txt ligand.pdb receptor.pdb $i.temp 2>/dev/null # spacer_id.txt is not in some libraries
    cd $i.temp
     date >> log.file
     free -m >> log.file
     > grafting.log.file$i # create the log file so tail does not complain
     j=1
     while [[ $j -le $files_Per_CPU ]];
     do
         echo "file=$file, j=$j, i=$i, fpCPU=$files_Per_CPU, num_cpus=$num_cpus, num_files=$num_files, remainder=$remainder"
         filename=`sed -n ''$file'p' ../search_results.txt`
         cp $filename RESULTS_library_search/
         let "j += 1"
         let "file += 1"
     done
     printf "cd /scratch1/webdev/grafting/runs/$folder/0.grafting/$i.temp/\n" >> submit.sh
     printf "/programs/e12/grafting/grafting/grafting.exe $wiggleWindow $linkerLib >> grafting.log.file$i\n" >> submit.sh
     ssh 192.168.1.210 "qsub /scratch1/webdev/grafting/runs/$folder/0.grafting/$i.temp/submit.sh"
     let running+=1;
    cd ../
    let i+=1;
done 

if [ $running -gt 0 ]; then
    printf "STATUS -> Waiting for all $num_files jobs to finish...\n" > job-stats.txt
    cp status.txt updated-status.txt; cat job-stats.txt >> updated-status.txt
else
    printf "PROBLEM: NO JOBS RUNNING: running=$running, num_cpus=$num_cpus\n" >> status.txt 
fi

while [ $running -gt 0 ]
do
    sleep 10s
    finished=`tail /scratch1/webdev/grafting/runs/$folder/0.grafting/*.temp/grafting.log.file* | grep -ac "Finished all"`
    running=$(($num_cpus - $finished))
    if [ $running -lt 0 ] || [ $finished -gt $num_cpus ] ; then
        echo "CRITICAL ERROR: running=$running, finished=$finished. Stopping!" >> status.txt
        echo "stop=1" > stop.txt
        exit
    fi
    printf "STATUS -> $finished batch(s) complete, $running batch(s) running \n" > job-stats.txt
    cp status.txt updated-status.txt; cat job-stats.txt >> updated-status.txt
done

printf "STATUS -> Generating summary.\nNote: this may take up to 10 minutes\n" > job-stats.txt
cp status.txt updated-status.txt; cat job-stats.txt >> updated-status.txt

mv *temp/RESULTS_library_search/* RESULTS_grafting/
mv RESULTS_grafting/*fit* fittingSpacer3Dfiles/ 2>/dev/null
mv RESULTS_grafting/*spacer* Spacers 2>/dev/null # no need to hear if there isn't anything there
mv RESULTS_grafting/*overlap* Overlaps 2>/dev/null # no need to hear if there isn't anything there
mv RESULTS_grafting/*Sp* Spacer3Dfiles/ 2>/dev/null # no need to hear if there isn't anything there
mv RESULTS_grafting/*budcore* Structures/ 2>/dev/null
cat *temp/grafted-IDs.txt > grafted-IDs.txt
3.createSummary.sh
if [ -f alpha-anomer.pdb ]; then
    mv alpha-anomer.pdb Structures/
fi
printf "STATUS -> Everything successfully completed.\n" > job-stats.txt
cp status.txt updated-status.txt; cat job-stats.txt >> updated-status.txt
