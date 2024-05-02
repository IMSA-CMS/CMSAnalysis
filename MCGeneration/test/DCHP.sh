#!/bin/bash

echo "Event 1 Started" >> DCHP_status.txt
cmsRun ~/analysis/CMSSW_12_4_3/src/CMSAnalysis/MCGeneration/test/EventGenerator_edm_cfg.py pythiaSettings=./params/DCHP_pSet1.txt output=input_10000_pSet1.root globalTag=Run2 maxEvents=10000
echo "Event 1 Completed" >> DCHP_status.txt
cmsRun ~/analysis/CMSSW_12_4_3/src/CMSAnalysis/MCGeneration/test/EventGenerator_edm_cfg.py pythiaSettings=./params/DCHP_pSet2.txt output=input_10000_pSet2.root globalTag=Run2 maxEvents=10000
echo "Event 2 Completed" >> DCHP_status.txt
cmsRun ~/analysis/CMSSW_12_4_3/src/CMSAnalysis/MCGeneration/test/EventGenerator_edm_cfg.py pythiaSettings=./params/DCHP_pSet3.txt output=input_10000_pSet3.root globalTag=Run2 maxEvents=10000
echo "Event 3 Completed" >> DCHP_status.txt
cmsRun ~/analysis/CMSSW_12_4_3/src/CMSAnalysis/MCGeneration/test/EventGenerator_edm_cfg.py pythiaSettings=./params/DCHP_pSet5.txt output=input_10000_pSet5.root globalTag=Run2 maxEvents=10000
echo "Event 5 Completed" >> DCHP_status.txt