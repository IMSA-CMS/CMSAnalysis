#!/bin/bash

for ind in $(seq 1 1 5)
do
    i="00${ind}"
    val="0.0${ind}"
    echo "Current LeptonJet DeltaR cut: $i"
    pathName="DarkPhoton_MLEval_1MuPt50_2MuPt10_DeltaRCut_$i/"
    pathNameStr="DarkPhoton_MLEval_1MuPt50_2MuPt10_DeltaRCut_$i"
    echo "Output Path: Output/$pathName"

    # Populate the txt file with the current cut
    echo $val > /uscms/home/jpalamad/analysis/CMSSW_14_0_4/src/CMSAnalysis/Filters/src/leptonJetDeltaRCut.txt

    dy=(
        "Drell-Yan/Drell-Yan_MassCut_10-50_Run_2.txt"
        "Drell-Yan/Drell-Yan_MassCut_50-inf_Run_2.txt"
    )

    # Run Drell-Yan analysis in the background
    for file in "${dy[@]}"; do
        outputFile="${file%.txt}.root"
        outputFile="${outputFile/Drell-Yan\//}"  # Remove Drell-Yan/ from the output filename if necessary
        echo output="${pathName}${outputFile}"
        nohup runAnalyzer input="$file" output="${pathName}${outputFile}" analysis=LeptonJetReconstruction numFiles=3 outputEvery=20000 &
    done

    # Run other scripts in the background
    python3 multiRunAnalyzer.py DarkPhoton keep $pathName &
    python3 multiRunAnalyzer.py DPBackground keep $pathName &
    nohup runAnalyzer input=Data/Data_Trigger_SingleMuon_Year_2018A.txt output=${pathName}Data_Trigger_SingleMuon_Year_2018A.root analysis=LeptonJetReconstruction numFiles=3 &

    ## Run additional replacements if uncommented
    ## runAnalyzer input=QCD/QCD_HTCut_1500-2000_Run_2_Year_2018.txt output=${pathName}QCD_HTCut_1500-2000_Run_2_Year_2018.root analysis=LeptonJetReconstruction numFiles=2 &
    ## runAnalyzer input=ZZ/ZZ_Decay_2e2tau_Run_2.txt output=${pathName}ZZ_Decay_2e2tau_Run_2.root numFiles=5 analysis=LeptonJetReconstruction outputEvery=1000 &

    # Wait for all background processes to complete
    wait

    echo "All processes completed for Mu pT cut: $i. Generating plots."

    # Run the plotting script
    bash /uscms/home/jpalamad/analysis/CMSSW_14_0_4/src/CMSAnalysis/Analysis/bin/multiSuperPlot.sh $pathNameStr
done
