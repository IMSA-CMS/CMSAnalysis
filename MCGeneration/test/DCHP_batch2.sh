#!/bin/bash

echo "Event 7 Started" >> DCHP_status.txt
cmsRun ~/analysis/CMSSW_12_4_3/src/CMSAnalysis/MCGeneration/test/EventGenerator_edm_cfg.py pythiaSettings=./params/DCHP_pSet7.txt output=input_10000_pSet7.root globalTag=Run2 maxEvents=10000
echo "Event 7 Completed" >> DCHP_status.txt
cmsRun ~/analysis/CMSSW_12_4_3/src/CMSAnalysis/MCGeneration/test/EventGenerator_edm_cfg.py pythiaSettings=./params/DCHP_pSet8.txt output=input_10000_pSet8.root globalTag=Run2 maxEvents=10000
echo "Event 8 Completed" >> DCHP_status.txt
cmsRun ~/analysis/CMSSW_12_4_3/src/CMSAnalysis/MCGeneration/test/EventGenerator_edm_cfg.py pythiaSettings=./params/DCHP_pSet9.txt output=input_10000_pSet9.root globalTag=Run2 maxEvents=10000
echo "Event 9 Completed" >> DCHP_status.txt
cmsRun ~/analysis/CMSSW_12_4_3/src/CMSAnalysis/MCGeneration/test/EventGenerator_edm_cfg.py pythiaSettings=./params/DCHP_pSet10.txt output=input_10000_pSet10.root globalTag=Run2 maxEvents=10000
echo "Event 10 Completed" >> DCHP_status.txt