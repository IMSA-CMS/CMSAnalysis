#!/bin/bash
# This shell script is meant to run RunAnalyzer over multiple files
# Specifically, estimate backgrounds for ZZ, ttbar, Drell-Yan, bbbar
echo "Start Multi-RunAnalyzer Script"
echo "File Path:" $1

# Efficiency doesn't matter, so I'll just run everything
masses=(200 500 800 1000 1300)

for i in ${masses[@]}; do
    # Start from a temporary text file
    echo "Current Mass Run: " $i

    # Change this when I'm more confident
    echo "Procs:  Higgs" > tempScriptFile.txt
    
    # Year may need to change based on file
    echo "Year:   2020" >> tempScriptFile.txt
    echo "Lepton: both" >> tempScriptFile.txt

    # Masses
    echo "Mass:   $i" >> tempScriptFile.txt
    echo "Lambda: M$i" >> tempScriptFile.txt
    
    # Other options that might need to be changed
    echo "Inter:  con" >> tempScriptFile.txt
    echo "Helic:  Higgs++to2Leptons" >> tempScriptFile.txt

    # runAnalyzer input=tempScriptFile.txt output=$1/HiggsPlusPlusBackgroundEstimate$i.root
done

# Other Pick Files
# premade=(BBbarPickFiles.txt ZZPickFiles.txt DYPickFiles.txt TTBarPickFiles.txt)
# names=(BBbar ZZ DY TTbar)
premade=(TTBarExperimental.txt)
names=(TTbar)
count=0

for i in ${premade[@]}; do
    # Echo the current background run
    echo "Current Background Run: " $i

    # Make sure to run with only one file, disable when you want to!! [09/16/2021]
    runAnalyzer numFiles=1 input=textfiles/$i output=$1/${names[count]}BackgroundEstimateOneFile.root
    count=$count+1
done

echo "Finished"
