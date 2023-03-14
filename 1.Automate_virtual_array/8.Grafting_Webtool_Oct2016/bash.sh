#source ~/.bashrc
#!/bin/bash

if [ -f /etc/bashrc ]; then
	. /etc/bashrc
fi

# Check if job is already running in folder:
if [ -d "0.grafting" ]; then
  # Control will enter here if $DIRECTORY exists
  exit
fi

# User specific aliases and functions
alias python=/usr/local/bin/python2.7

GRAFTING_HOME=/programs/site_deps/Grafting/program
export GRAFTING_HOME
PATH=$GRAFTING_HOME:$PATH
export GEMSHOME=/programs/temp/gems_grafting_2016Jan27
PATH=$PATH:$GEMSHOME

folderName=${PWD##*/}
printf "folder=$folderName\n" > folderName
cp -r z.originals 0.grafting
cd 0.grafting

uploadFile=`cat /scratch1/webdev/grafting/runs/$folderName/0.grafting/UPLOADED_PDB_NAME`
source /scratch1/webdev/grafting/runs/$folderName/0.grafting/userInputs.txt


echo "stop=0" > stop.txt #Keep going unless changed by subscripts
printf "___________________________________________________________\n\n" > status.txt
echo "Run information:" >> status.txt
echo "Job ID	-> $folderName" >> status.txt
echo "Complex	-> $uploadFile" >> status.txt
echo "Library	-> $lib" >> status.txt
if [ -s email ] ;then
    mail=`cat email`
echo "Email	-> $mail" >> status.txt
else
echo "Email	-> None provided" >> status.txt
fi
printf "___________________________________________________________\n\n" >> status.txt
cp status.txt updated-status.txt

cat status.txt | mail -s "Webtool job initiated" olivercgrant@gmail.com

/programs/site_deps/Grafting/program/0.preProcessUserInputs.sh $folderName

source /scratch1/webdev/grafting/runs/$folderName/0.grafting/stop.txt
if [ $stop -eq 0 ]; then
    echo "Library search:" >> status.txt
    /programs/site_deps/Grafting/program/2.search_library.sh_2.5
fi
source /scratch1/webdev/grafting/runs/$folderName/0.grafting/stop.txt
if [ $stop -eq 0 ]; then
    echo "Running on the Glycam Server:" >> status.txt
    /programs/site_deps/Grafting/program/3.Parallel-grafting-WebDevToEliot-v3.sh
fi
source /scratch1/webdev/grafting/runs/$folderName/0.grafting/stop.txt
if [ $stop -eq 2 ]; then
    printf "\nERROR: Duplicate job attempt. Clicking back on wait page and then continue can cause this.\nPlease return to page 1 and begin again\n" >> status.txt
fi
if [ $stop -eq 0 ]; then
    #include status.txt at bottom of stats.txt
    printf "\n******************************************************************************\n" >> stats.txt
    cat updated-status.txt >> stats.txt
else
    if [ -s email ] ;then
        mail=`cat email`
        cat status.txt | mail -s "Error: Grafting terminated" $mail
        cat status.txt | mail -s "Error: Grafting terminated" olivercgrant@gmail.com
    else
        echo "Job terminated" >> status.txt
        cat status.txt | mail -s "Error: Grafting terminated" olivercgrant@gmail.com
    fi
fi
if [ $stop -gt 0 ]; then
    cp updated-status.txt stats.txt
fi
cd ../
