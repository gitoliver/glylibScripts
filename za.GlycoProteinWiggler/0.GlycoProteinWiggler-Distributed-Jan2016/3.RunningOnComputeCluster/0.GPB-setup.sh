#!/bin/bash

if [ $# -gt 4 ]; then
models=$1
glycans=$2
glylist=$3
reslist=$4
jobName=$5

#models=gp120models
#glycans=glycans
#glylist=glylist.txt
#reslist=reslist.txt
h=1
numRes=`wc -l < $reslist`
if [ -n "$6" ]
then h=$6
fi

for gly in `cat $glylist`
do
    for mod in `ls $models`
    do
        mkdir $h.$gly.$jobName
        cp -r submit.template $glycans $models/$mod $reslist $h.$gly.$jobName
        cd $h.$gly.$jobName
        sed -i 's/Name/'$jobName'/g' submit.template
        cp $mod current.pdb
        echo -e "#!/bin/bash\nBUILDERHOME=/scratch1/oliver/glycoroteinBuilder/z.GlycoProteinWiggler/" > run.sh
        k=0
        i=1
        j=1
        while [ $k -lt $numRes ]
        do
            ((k+=5))
            echo "Protein:" > input$i.txt
            echo "current.pdb" >> input$i.txt
            echo "" >> input$i.txt
            echo "Glycans:" >> input$i.txt
            echo "$glycans/" >> input$i.txt
            echo "" >> input$i.txt
            echo "Protein Residue list:" >> input$i.txt
            sed ''$j','$k'!d' $reslist >> input$i.txt
            echo "END" >> input$i.txt
            echo "" >> input$i.txt
            echo "Glycan id list:" >> input$i.txt
            echo -e "$gly\n$gly\n$gly\n$gly\n$gly\nEND" >> input$i.txt

            echo -e "\$BUILDERHOME/glycoProteinWiggler.exe input$i.txt > logfile$i.txt &\nwait" >> run.sh
            if [ $k -lt $numRes ]; then
                echo -e "mv glycosylated.pdb result$i.pdb\ncp result$i.pdb current.pdb" >> run.sh
            fi
            j=$(($k+1))
            ((i++))
        done
        chmod +x run.sh
        echo "./run.sh" >> submit.template
        qsub submit.template
        cd ../
        #echo -e "cd $h.$gly.$jobName \nnohup \$PBS_O_WORKDIR/$h.$gly.$jobName/run.sh & \ncd ../" >> submit.sh
        ((h++))
    done
done
echo "Criochnaithe"

else
echo "Usage ./0.GPB-setup.sh models/ glycans/ glylist reslist jobName [folderNumber]"
fi
