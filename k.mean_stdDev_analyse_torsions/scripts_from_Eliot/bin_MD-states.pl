#!/usr/bin/perl
#                          ARGV[0]     ARGV[1]
# Usage convert_angle.pl file_list output_file_name
use strict;

open(fileList,@ARGV[0]);

my (@fileAry,@dataAry,@tempAry);
my $i=0;
my ($j,$col,$row,$arrayLength,$colLength,$temp);

print "\n";

# Test that variables are defined and fail if not, also make sure that the input file exists and the output file does not
if (defined($ARGV[0]) == 0 | defined($ARGV[1]) == 0) {die "WARNING: Usage Error\n ./create_Qliinc_input.pl input_file[files to be processed] output_file[file name]\n";}

elsif (-e "@ARGV[0]"){
	if (-e "@ARGV[1]"){die "WARNING: Output File\n The output file, @ARGV[1], already exists.\n Delete or move this file before re-running this script\n";}

	else {print "Processing files in $ARGV[0] and output is sent to $ARGV[1]\n";}
}

else{ 
	if (-e "@ARGV[1]"){die "WARNING: I/O Files\n The input file, @ARGV[0], does not exist.\n This file needs to contain the list of combined files in column-order.\n\n The output file, @ARGV[1], already exists.\n Delete or move this file before re-running this script\n";}

	else {die "WARNING: Input File\n The file \"@ARGV[0]\" does not exist.\n This file needs to contain the list of combined files in column-order.\n";}
}

#open(ERR,">>@ARGV[1].err");

### Subroutines
sub progress_bar {
    my ( $got, $total, $width, $char, $outStyle ) = @_;
    $width ||= 25;
    $char  ||= '=';
    my $num_width = length $total;
    local $| = 1;
if($outStyle==0){ # Files are being read
    printf "|%-${width}s| Finished reading %${num_width}.0f files of %.0f (%.2f%%)\r",
        $char x (($width-1)*$got/$total). '>', $got, $total, 100*$got/
+$total;
}
if($outStyle==1){ # Processing progress
    printf "|%-${width}s| Processing (%.2f%%)\r",
        $char x (($width-1)*$got/$total). '>', 100*$got/
+$total;
}
}

sub conditionalCheck { # Function for dividing up -180 to 180.  Note that the (a) is the state that crosses the -180/180 barrier or starts from -180
	my ($val,$method)= @_;
        if($method == 0){ # Method for one state
# Binned states, 120=>-120 (a), -120=>0 (b), 0=>120 (c)
        switch: for ($_=$val){ 
                if (($_ > -180) || ($_ <= 180)) { return "a"; };
        }}
	elsif($method == 1){ # Method for phi with three regions
# Binned states, 120=>-120 (a), -120=>0 (b), 0=>120 (c)
        switch: for ($_=$val){
                if (($_ > 120) || ($_ <= -120)) { return "a"; };
                if (($_ > -120) && ($_ <= 0)) { return "b"; };
                if (($_ > 0) && ($_ <= 120)) { return "c"; };
        }}
	elsif($method == 2){ # Method for psi with four regions
# Binned states, 120=>-120 (a),-120=>-35 (b), -35=>35 (c), 35=>120 (d)
        switch: for ($_=$val){
                if (($_ > 120) || ($_ <= -120)) { return "a"; };
                if (($_ > -120) && ($_ <= -35)) { return "b"; };
                if (($_ > -35) && ($_ <= 35)) { return "c"; };
                if (($_ > 35) && ($_ <= 120)) { return "d"; };
        }}
	elsif($method == 3){ # Method for omega7 with two regions
# Binned states, -20=>-120 (a), -120=>-20 (b)
        switch: for ($_=$val){
                if (($_ > -20) || ($_ <= -120)) { return "a"; };
                if (($_ > -120) && ($_ <= -20)) { return "b"; };
        }}
	elsif($method == 4){ # Method for omega8 with three regions
# Binned states, 140=>-140 (a), -140=>0 (b), 0=>140 (c)
        switch: for ($_=$val){
                if (($_ > 140) || ($_ <= -140)) { return "a"; };
                if (($_ > -140) && ($_ <= 0)) { return "b"; };
                if (($_ > 0) && ($_ <= 140)) { return "c"; };
        }}
        elsif($method == 5){ # Method for omega9 with three regions
# Binned states, 120=>-120 (a), -120=>0 (b), 0=>120 (c)
        switch: for ($_=$val){ 
                if (($_ > 125) || ($_ <= -125)) { return "a"; };
                if (($_ > 25) && ($_ <= 125)) { return "b"; };
                if (($_ > -125) && ($_ <= 25)) { return "c"; };
        }}
	elsif($method == 6){
	switch: for ($_=$val){
		if (($_ >= 125) || ($_ < -125)) { return "a"; };
                if (($_ >= -125) && ($_ < -25)) { return "b"; };
                if (($_ >= -25) && ($_ < 125)) { return "c"; };
	}}
        elsif($method == 7){
        switch: for ($_=$val){
                if (($_ >= 30) || ($_ < 180)) { return "a"; };
                if (($_ >= -30) && ($_ < 30)) { return "b"; };
                if (($_ >= -180) && ($_ < -30)) { return "c"; };
        }}
	else {# Default of 360 / 60 into 6 evenly spaced increments
# Binned States, 150=>-150 (a), -150=>-90 (b), -90=>-30 (c), -30=>30 (d), 30=>90 (e), 90=>150 (f)
        switch: for ($_=$val){
                if (($_ > 150) || ($_ <= -150)) { return "a"; };
                if (($_ > -150) && ($_ <= -90)) { return "b"; };
                if (($_ > -90) && ($_ <= -30)) { return "c"; };
                if (($_ > -30) && ($_ <= 30)) { return "d"; };
                if (($_ > 30) && ($_ <= 90)) { return "e"; };
                if (($_ > 90) && ($_ <= 150)) { return "f"; };
        }}
}

###
# Build the input file array @fileAry
my @tmpAry = <fileList>;
close(fileList);

for($i=0;$i<=$#tmpAry;$i++){
	$temp=$tmpAry[$i];
	chop $temp;
	($fileAry[$i][0],$fileAry[$i][1])=split(/\s+/,$temp,2);
}

# Tests to make sure there are at least two files to combine together in the input file list (ARGV[0])
if ($#{@fileAry} < 1) { die "WARNING: Quantity of Input Files\n There are $#{@fileAry} files listed in $ARGV[0].\n You need at least 2 files.\n"}

# Outputs the list of input files
print "The input files listed are:\n";
for($i=0;$i<=$#{@fileAry};$i++){
	$temp=$fileAry[$i][0];
#	chop $temp;
	if (-e "$temp"){
	        print $temp." with splitting type ".$fileAry[$i][1]."\n";
		} # If the file exists, increment to the next file
	else {
		die "WARNING: The File ".$fileAry[$i][0]." Does Not Exist\n The file, $temp, listed in $ARGV[0] does not exist.\n";
		} # If the file does not exist, then exit
}



# Establishes the first file as the array with the the timestep column (column 0)
open(firstFile,$fileAry[0][0]);
$i=0;
while(<firstFile>){
	($tempAry[0][$i],$tempAry[1][$i],$tempAry[2][$i])=split(/\s+/,$_);
	($dataAry[0][$i],$dataAry[1][$i])=($tempAry[1][$i],$tempAry[2][$i]);
	$i++;
}
close(firstFile);


# Variabls for extending the array
$arrayLength=$i; # Assigns the last value of "i" to the array length
$col=2; #starts from column 3 - columns 1 and 2 were filled by the firstFile
print "\nThe number of timestep points is $arrayLength\n";
$colLength=($#{@fileAry} + 1);
print "The number of non-timestep columns is $colLength\n";


# Add columns to the dataAry[col][row]
for($i=1;$i<$colLength;$i++){
	open(fileName,$fileAry[$i][0]);
	$j=0;

	while(<fileName>){
		($tempAry[0][$j],$tempAry[1][$j],$tempAry[2][$j])=split(/\s+/,$_);
		$j++;
	}
	close(fileName);

	for($row=0;$row<$arrayLength;$row++){
		$dataAry[$col][$row]=$tempAry[2][$row];
	}

	$col++; #Sequences the column for each fileName
        &progress_bar(($i + 1),$colLength,25,'=',0)
}
print "\n";
my @binAry; # binAry[i][j][k], i = unique state index, j=0= state string, j=1= count, j=2= each timestep indexed by k
# Binned States, 150=>-150 (a), -150=>-90 (b), -90=>-30 (c), -30=>30 (d), 30=>90 (e), 90=>150 (f)
# Bottom limit is >, upper limit is <=
#
my $iPrevious=0;
#my $previousCount=0;
#my $forCount=0;
#my $newCount=0;
for($row=0;$row<$arrayLength;$row++){
	my $stateString=&conditionalCheck($dataAry[1][$row],$fileAry[0][1]);
        for($col=2;$col<=$colLength;$col++){
        	$stateString=$stateString.&conditionalCheck($dataAry[$col][$row],$fileAry[($col - 1)][1]);
        }
	my $foundState=0; # Variable for state not found in array (0) or found (1)
	if($row >= 1){ # Checks the array when not the first value entered
#		print "Length of binAry is $#{@binAry} for row $row\n";
		if ($stateString eq $binAry[$iPrevious][0][0]){
			$i=$iPrevious; my $mi=($#{$binAry[$i][2]} + 1);$binAry[$i][1][0]=($binAry[$i][1][0] + 1); $binAry[$i][2][$mi]=($row + 1); $foundState=1;
#			$previousCount++; print ERR "Previous count used $previousCount times\n";
			}
		else {
			for($i=0;$i<=$#{@binAry};$i++){
				if($stateString eq $binAry[$i][0][0]){
					my $mi=($#{$binAry[$i][2]} + 1); $binAry[$i][1][0]=($binAry[$i][1][0] + 1); $binAry[$i][2][$mi]=($row + 1); $foundState=1; $iPrevious=$i; last;
				}
			}
#			$forCount++; print ERR "For count used $forCount times\n";
		}
		if($foundState==0){
			$binAry[$i][0][0]=$stateString; $binAry[$i][1][0]=1; $binAry[$i][2][0]=($row + 1); $iPrevious=$i;# Set the first time a new state is identified as popn 1
#		$newCount++; print ERR "New state added $newCount times\n";
		}
	}
	elsif($row == 0){ $binAry[$row][0][0]=$stateString; $binAry[$row][1][0]=1; $binAry[$row][2][0]=($row + 1);}
        &progress_bar(($row + 1),$arrayLength,25,'=',1)

}
print "\nFinished... File output is named @ARGV[1]\n";
# Output Section
open(OUT,">>@ARGV[1]");

for($i=0;$i<=$#{@binAry};$i++){
	printf OUT ("%".$colLength."s","$binAry[$i][0][0]");
	printf OUT ("%14s"," $binAry[$i][1][0]");
	printf OUT ("%14.1f"," ".(100 * $binAry[$i][1][0] / $arrayLength));
	printf OUT ("\n");
}
print OUT "\n";
close(OUT);

for($i=0;$i<=$#{@binAry};$i++){
	my $outFilename=$binAry[$i][0][0].".dat";
        open(OUT2,">$outFilename");
	printf OUT2 ("# ".$binAry[$i][0][0]." ".$binAry[$i][1][0]." / ".$arrayLength."\n");
	my $previousTimestep=$binAry[$i][2][0];
	my $firstTimestep=$binAry[$i][2][0];
	for($j=1;$j<=$#{$binAry[$i][2]};$j++){
		if($previousTimestep == ($binAry[$i][2][$j] - 1)){
			$previousTimestep=$binAry[$i][2][$j];
		}
		else{
			printf OUT2 ($firstTimestep." ".$previousTimestep." 1\n");
			$previousTimestep=$binAry[$i][2][$j];
			$firstTimestep=$binAry[$i][2][$j];
		}
	}
	if($#{$binAry[$i][2]} < $j){
                printf OUT2 ($firstTimestep." ".$previousTimestep." 1\n");
	}
	elsif($#{$binAry[$i][2]} eq 0){
                printf OUT2 ($firstTimestep." ".$previousTimestep." 1\n");
	}

	close(OUT2);
}
#close(ERR);
