#!/bin/bash

num_files=`grep pdb -c search_results.txt`;
source installerInputs.txt
source userInputs.txt

echo "#!/bin/bash" > submit.sh
echo "" >> submit.sh
echo "#PBS -q docking" >> submit.sh
echo "#PBS -l nodes=1:ppn=1" >> submit.sh
echo "#PBS -N Grafting" >> submit.sh
echo "#PBS -o out " >> submit.sh
echo "#PBS -e err " >> submit.sh
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

#echo "Copying files"
source ../folderName
#cp -r ../../$folder /scratch1/webdev/grafting/runs/

i=1;
num_cpus=10;
running=0;
while [[ $i -le $num_files ]];
do
    filename=`sed -n ''$i'p' search_results.txt`
    rm -r $i.temp 2>/dev/null; mkdir $i.temp; mkdir $i.temp/RESULTS_library_search/;
    cp $filename $i.temp/RESULTS_library_search/; cp spacer_id.txt submit.sh LINK.txt ligand.pdb receptor.pdb $i.temp
    cd $i.temp
     date >> log.file
     free -m >> log.file
     printf "STATUS -> Running job $i of $num_files\n" > ../job-stats.txt
     cp ../status.txt ../updated-status.txt; cat ../job-stats.txt >> ../updated-status.txt
     echo "cd /scratch1/webdev/grafting/runs/$folder/0.grafting/$i.temp/" >> submit.sh
     echo "/programs/e12/grafting/grafting/grafting.exe $wiggleWindow $linkerLib >> grafting.log.file$i" >> submit.sh
     #cp -r ../$i.temp /scratch1/webdev/grafting/runs/$folder/0.grafting/
     ssh 192.168.1.210 "qsub /scratch1/webdev/grafting/runs/$folder/0.grafting/$i.temp/submit.sh"
    cd ../
    let running+=1;
    while [ $running -ge $num_cpus ]
    do
        sleep 1s
        finished=`tail /scratch1/webdev/grafting/runs/$folder/0.grafting/*.temp/grafting.log.file* | grep -ac "Finished all"`
        running=$(($i - $finished))
        #printf "Waiting on CPUS\nJobs running:$running\nJobs finished:$finished\n" >> job-stats.txt
        printf "Job is queued; waiting on resources...\n" >> job-stats.txt
        cp status.txt updated-status.txt; cat job-stats.txt >> updated-status.txt 
        #echo "running=$running,finished=$finished,i=$i" 
        if [ $running -lt 0 ]; then
            echo "CRITICAL ERROR: running=$running. Stopping!" >> status.txt
            echo "stop=1" > stop.txt
            exit
        fi
        #running=`ssh 192.168.1.210 "ps ax | grep grafting.exe | grep -v "grep" -c"`
    done
    let i+=1;
done

let i-=1; #As would now be 102 for 101 jobs
printf "STATUS -> Waiting for all $num_files jobs to finish...\n" > job-stats.txt
cp status.txt updated-status.txt; cat job-stats.txt >> updated-status.txt
while [ $running -gt 0 ]
do
    echo -ne "."
    sleep 2s
    finished=`tail /scratch1/webdev/grafting/runs/$folder/0.grafting/*.temp/grafting.log.file* | grep -ac "Finished all"`
    running=$(($i - $finished))
    if [ $running -lt 0 ]; then
        echo "CRITICAL ERROR: running=$running. Stopping job." >> status.txt
        echo "stop=1" > stop.txt
        exit
    fi
    #running=`ssh 192.168.1.210 "ps ax | grep grafting.exe | grep -v "grep" -c"`
done    
printf "STATUS -> Generating summary.\nNote: this may take up to 10 minutes\n" > job-stats.txt
cp status.txt updated-status.txt; cat job-stats.txt >> updated-status.txt
#Bring everything back from eliot
#rsync -av /scratch1/webdev/grafting/runs/$folder/0.grafting/ .

mv *temp/RESULTS_library_search/* RESULTS_grafting/
mv RESULTS_grafting/*fit* fittingSpacer3Dfiles/ 2>/dev/null
mv RESULTS_grafting/*spacer* Spacers 2>/dev/null # no need to hear if there isn't anything there
mv RESULTS_grafting/*overlap* Overlaps 2>/dev/null # no need to hear if there isn't anything there
mv RESULTS_grafting/*Sp* Spacer3Dfiles/ 2>/dev/null # no need to hear if there isn't anything there
mv RESULTS_grafting/*budcore* Structures/ 2>/dev/null
cat *temp/grafted-IDs.txt > grafted-IDs.txt
3.createSummary.sh
printf "STATUS -> Everything successfully completed.\n" > job-stats.txt
cp status.txt updated-status.txt; cat job-stats.txt >> updated-status.txt
