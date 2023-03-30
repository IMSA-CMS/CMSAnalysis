#!/bin/sh
inputQuery=$1
if [[ -z "$2" ]]; then
   outputFile="dasgoOutput.txt"
else
    outputFile=$2
fi
> $outputFile
tempfile="/dev/shm/dasgoSearch.txt"
echo "Searching for query"
dasgoclient -query='dataset dataset='$inputQuery >$tempfile
echo "Found datasets matching" $inputQuery
for line in `cat $tempfile`
do
    dasgoclient -query='file dataset='$line >> $outputFile
done
echo "Root files printed to" $outputFile

#dasgoSearch.sh
#This script collects all the root files for any dasgoclient query and aggregates them into a single text file
#Your command line should look like: ./dasgoSearch.sh "[input query goes here]" "[output text file goes here]"
#An output file is not necessary, if you choose not to specify one the output will go to dasgoOutput.txt
