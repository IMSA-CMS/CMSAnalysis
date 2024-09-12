#!/bin/bash

for i in {28..36}
do
    echo "Current Mu pT cut: $i"
    pathName="DarkPhoton_MLEval_MuPtCut_$i/"
    pathNameStr="DarkPhoton_MLEval_MuPtCut_$i"
    echo "Output Path: Output/$pathName"

    bash /uscms/home/jpalamad/analysis/CMSSW_14_0_4/src/CMSAnalysis/Analysis/bin/multiSuperPlot.sh $pathNameStr
done


# /uscms/home/jpalamad/analysis/CMSSW_14_0_4/src/CMSAnalysis/Filters/src/highestMuonPtCut.txt