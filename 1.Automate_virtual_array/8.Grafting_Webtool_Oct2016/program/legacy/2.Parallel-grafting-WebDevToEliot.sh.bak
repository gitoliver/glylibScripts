#!/bin/bash


num_files=`grep pdb -c search_results.txt`;
num_cpus=20
source installerInputs.txt
source userInputs.txt


echo "#!/bin/bash" > submit.sh
echo "" >> submit.sh
echo "#PBS -q docking" >> submit.sh
echo "#PBS -l nodes=1:ppn=1" >> submit.sh
echo "#PBS -N Grafting" >> submit.sh
echo "#PBS -j oe " >> submit.sh
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
echo "Copying files"
i=1;
cpu=1;
running=0;
while [[ $i -le $num_files ]];
do
    filename=`sed -n ''$i'p' search_results.txt`
    rm -r $i.temp 2>/dev/null; mkdir $i.temp; mkdir $i.temp/RESULTS_library_search/;
    cp $filename $i.temp/RESULTS_library_search/; cp submit.sh LINK.txt ligand.pdb receptor.pdb $i.temp
    cd $i.temp
     date >> log.file
     free -m >> log.file
     echo "Grafting... $i of $num_files"
     source ../../folderName
     echo "cd /scratch1/webdev/grafting/runs/$folder/0.grafting/$i.temp/" >> submit.sh
     echo "grafting.exe $wiggleWindow $linkerLib >> grafting.log.file$i" >> submit.sh
     cp -r ../../../$folder /scratch1/webdev/grafting/runs/ 
     ssh 192.168.1.210 "qsub /scratch1/webdev/grafting/runs/$folder/0.grafting/$i.temp/submit.sh"
    cd ../
    let i+=1;
    let running+=1;
    while [ $running -ge $num_cpus ]
    do
        echo "Waiting..."
        sleep 5s
        running=`ssh 192.168.1.210 "ps ax | grep grafting.exe | grep -v "grep" -c"`
    done
done

echo -ne "Waiting for all jobs to complete..."
while [ $running -gt 0 ]
do
    echo -ne "."
    sleep 5s
    running=`ssh 192.168.1.210 "ps ax | grep grafting.exe | grep -v "grep" -c"`
done    
echo -e "\nFin"

#Bring everything back from eliot
rsync -av /scratch1/webdev/grafting/runs/$folder/0.grafting/ .

mv *temp/RESULTS_library_search/* RESULTS_grafting/
mv RESULTS_grafting/*fit* fittingSpacer3Dfiles/ 2>/dev/null
mv RESULTS_grafting/*spacer* Spacers 2>/dev/null # no need to hear if there isn't anything there
mv RESULTS_grafting/*overlap* Overlaps 2>/dev/null # no need to hear if there isn't anything there
mv RESULTS_grafting/*Sp* Spacer3Dfiles/ 2>/dev/null # no need to hear if there isn't anything there
mv RESULTS_grafting/*budcore* Structures/ 2>/dev/null
cat *temp/grafted-IDs.txt > grafted-IDs.txt
3.createSummary.sh
#rm -r *temp 2>/dev/null
