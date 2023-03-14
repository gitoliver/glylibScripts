#!/bin/bash

	#########################################################
	#Author: Oliver Grant olivercgrant@gmail.com 10Feb2012  #
	#Script is designed to take a Bomb Subs input file and a#
	# bomb input file and  output a series of input files to# 
	# be run in parallel					#
	#########################################################

if [ $# != 4 ]; then
echo ""
	echo "###################################################################################"
	echo "Usage ./1.make_parallel_BOMB.sh <SubsInputFile> <BombInputsFile> <zmatrix> <ncpus>"
	echo "###################################################################################"
	echo "SubsInputFile same as serial BOMB job"
	echo "BombInputsFile contains a list of commands you want to send to BOMB"
	echo "zmatrix same as serial BOMB job"
else

#added later, should incorporate new variables into code to make readable
SubsInputFile=$1
BombInputsFile=$2
zmatrix=$3
ncpus=$4
outfolder=job
rm -r $outfolder*

#find out the number of substitutions.
x=`sed -n 3p $SubsInputFile | cut -d : -f 3`

declare -i num_subs
declare -i end_line
declare -i grpsub

num_subs=10#$x # x will have a zero preceding number.Convert to int
echo "Number of substitutions in position one=$num_subs"
end_line=$[($num_subs/10)+4]

echo "Number of the last line containing substitutions for position one in $SubsInputFile is $end_line"

#take everything from input file except the substitutions
#replace the number of subs with 00001.
cat $SubsInputFile | sed "4,$end_line d" | sed 3s"/$x/00001/" > text.tmp
identifier=`awk ' NR == 1{print}' $SubsInputFile`

i=4;
k=1;
#go through each line starting from line 4 until last line with subs
while [ $i -le $end_line ]
	do
	j=1
	#There will be 10 items per line until last line where k==$num_subs
	while [ $j -le 10 ] && [ $k -le $num_subs ]
		do
		#using , as the delimiter, cut out each sub
		sub=`sed -n $i\p $SubsInputFile | cut -d , -f $j | sed s'/\ //'g` # remove spaces
		#echo "sub=$sub"
		#For groups, split them up into components
		l=${sub:0:1} # first letter is $l
		if [ $l == "-" ]; then # if this is a group.
			echo "Breaking up group $sub into it's parts"
			num_subs=$(($num_subs-1)); #discount group as a sub
			line=`grep -n "\ $sub" $BOMBdir/Rlist | cut -d : -f 1` #get line number of group start
			line=$(($line+1)) # go to next line
			grpsub=1
			while [ $grpsub != 0 ] #becomes zero when hits a blank line in Rlist. 
				do
				grpsub=10#`sed -n $line\p $BOMBdir/Rlist | cut -c 3-5`
				#echo "grpsub=$grpsub"
				line=$(($line+1)) # go to next line

				mkdir $outfolder.$k
                		sed "4 i $grpsub" text.tmp > $outfolder.$k/$1_$k
				#Replace the inputsubs file name in the input file for bomb
                        	cat $BombInputsFile | sed s"/$1/$1_$k/" > $outfolder.$k/$BombInputsFile
                        	cp $3 $outfolder.$k/$3 # copies the zmatrix file into the folder

				k=$(($k+1)); # there is one more sub now
				num_subs=$(($num_subs+1)); # there is one more sub now
				done

		else # it is a regular sub
			#echo "$sub"
			#put each sub into a unique input file
			mkdir $outfolder.$k
			sed "4 i $sub" text.tmp > $outfolder.$k/$1_$k
			#Replace the inputsubs file name in the input file for bomb
			cat $BombInputsFile | sed s"/$1/$1_$k/" > $outfolder.$k/$BombInputsFile
			cp $zmatrix $outfolder.$k/$zmatrix # copies the zmatrix file into the folder
			k=$[$k+1]; # one more done
		fi
		j=$[$j+1]; # move onto next sub or group of subs
		#echo "$k done, $num_subs to do"
		done
	i=$[$i+1]; # a line is complete, move to the next
	echo "lines done $i"
	done
#rm text.tmp

echo "outfolder=$outfolder, num_subs=$num_subs, ncpus=$ncpus, BombInputsFile=$BombInputsFile"
#now run bomb
#Usage shawparBOMB <foldernameprefix> <njobs> <ncpus> <BombInputsFile> <identifier>
shawparBOMB $outfolder $num_subs $ncpus $BombInputsFile $identifier
fi
