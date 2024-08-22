#!/bin/bash

for i in {28..28}
do
    echo "Current Mu pT cut: $i"
    pathName="DarkPhoton_MLEval_MuPtCut_$i/"
    pathNameStr="DarkPhoton_MLEval_MuPtCut_$i"
    echo "Output Path: Output/$pathName"

    echo $i > /uscms/home/jpalamad/analysis/CMSSW_14_0_4/src/CMSAnalysis/Filters/src/highestMuonPtCut.txt # populate txt file with current pt cut

    python3 multiRunAnalyzer.py DPBackground keep $pathName
    python3 multiRunAnalyzer.py DarkPhoton keep $pathName
    nohup runAnalyzer input=Data/Data_Trigger_SingleMuon_Year_2018A.txt output=${pathName}Data_Trigger_SingleMuon_Year_2018A.root analysis=LeptonJetReconstruction numFiles=1 > nohup.out

    dy=(
        "Drell-Yan/Drell-Yan_MassCut_10-50_Run_2.txt"
        "Drell-Yan/Drell-Yan_MassCut_50-inf_Run_2.txt"
    )

    echo "Beginning Drell-Yan"

    for file in "${dy[@]}"; do
        outputFile="${file%.txt}.root"
        outputFile="${outputFile/Drell-Yan\//}"  # Remove Drell-Yan/ from the output filename if necessary
        runAnalyzer input="$file" output="${pathNameStr}${outputFile}" analysis=LeptonJetReconstruction numFiles=3 > nohup.out
    done

    
    echo "Completed Drell-Yan"

    command_name="runAnalyzer"

    # Loop until the command is no longer running
    while pgrep -x "$command_name" > /dev/null
    do
        sleep 5  # Wait for 5 seconds before checking again
    done

    echo "$command_name has finished. Beginning replacement"

    runAnalyzer input=QCD/QCD_HTCut_1500-2000_Run_2_Year_2018.txt output=${pathName}QCD_HTCut_1500-2000_Run_2_Year_2018.root analysis=LeptonJetReconstruction numFiles=2 > nohup.out

    echo "Replacement completed. Beginning next iteration"

    killall $command_name

    bash /uscms/home/jpalamad/analysis/CMSSW_14_0_4/src/CMSAnalysis/Analysis/bin/multiSuperPlot.sh $pathNameStr
done


# /uscms/home/jpalamad/analysis/CMSSW_14_0_4/src/CMSAnalysis/Filters/src/highestMuonPtCut.txt