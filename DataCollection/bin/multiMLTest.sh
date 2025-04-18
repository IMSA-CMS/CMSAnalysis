#!/bin/bash

# OLD BDT

# root -q 'MLTest.C("BDTFiles/strippedSG_numFiles1.root", "BDTFiles/strippedBG_DY10_numFiles1.root", 0.24)'
# root -q 'MLTest.C("BDTFiles/strippedSG_numFiles1.root", "BDTFiles/strippedBG_DY50_numFiles5.root", 0.24)'

#########################

root -q 'MLTest.C("BDTFiles/strippedSG_numFiles1.root", "BDTFiles/strippedBG_QCD100-200_numFiles1.root", 0.16)'
root -q 'MLTest.C("BDTFiles/strippedSG_numFiles1.root", "BDTFiles/strippedBG_QCD200-300_numFiles1.root", 0.16)'
root -q 'MLTest.C("BDTFiles/strippedSG_numFiles1.root", "BDTFiles/strippedBG_QCD300-500_numFiles1.root", 0.16)'
root -q 'MLTest.C("BDTFiles/strippedSG_numFiles1.root", "BDTFiles/strippedBG_QCD500-700_numFiles1.root", 0.16)'
root -q 'MLTest.C("BDTFiles/strippedSG_numFiles1.root", "BDTFiles/strippedBG_QCD700-1000_numFiles1.root", 0.16)'
root -q 'MLTest.C("BDTFiles/strippedSG_numFiles1.root", "BDTFiles/strippedBG_QCD1000-1500_numFiles1.root", 0.16)'
root -q 'MLTest.C("BDTFiles/strippedSG_numFiles1.root", "BDTFiles/strippedBG_QCD1500-2000_numFiles1.root", 0.16)'
root -q 'MLTest.C("BDTFiles/strippedSG_numFiles1.root", "BDTFiles/strippedBG_QCD2000-inf_numFiles1.root", 0.16)'

# Original cut: 0.24

# root -q 'MLTest.C("BDTFiles/strippedSG_numFiles1.root", "BDTFiles/strippedBG_QCD100-200_numFiles1.root", 0.24)'
# root -q 'MLTest.C("BDTFiles/strippedSG_numFiles1.root", "BDTFiles/strippedBG_QCD200-300_numFiles1.root", 0.24)'
# root -q 'MLTest.C("BDTFiles/strippedSG_numFiles1.root", "BDTFiles/strippedBG_QCD300-500_numFiles1.root", 0.24)'
# root -q 'MLTest.C("BDTFiles/strippedSG_numFiles1.root", "BDTFiles/strippedBG_QCD500-700_numFiles1.root", 0.24)'
# root -q 'MLTest.C("BDTFiles/strippedSG_numFiles1.root", "BDTFiles/strippedBG_QCD700-1000_numFiles1.root", 0.24)'
# root -q 'MLTest.C("BDTFiles/strippedSG_numFiles1.root", "BDTFiles/strippedBG_QCD1000-1500_numFiles1.root", 0.24)'
# root -q 'MLTest.C("BDTFiles/strippedSG_numFiles1.root", "BDTFiles/strippedBG_QCD1500-2000_numFiles1.root", 0.24)'
# root -q 'MLTest.C("BDTFiles/strippedSG_numFiles1.root", "BDTFiles/strippedBG_QCD2000-inf_numFiles1.root", 0.24)'

# NEW BDT

#root -q 'MLTest.C("/uscms/home/jpalamad/analysis/CMSSW_14_0_4/src/CMSAnalysis/Output/DarkPhoton_MLStrip_CompleteCuts_Min/darkPhotonBaselineRun2.root", "/uscms/home/jpalamad/analysis/CMSSW_14_0_4/src/CMSAnalysis/Output/DarkPhoton_MLStrip_CompleteCuts_Min/Drell-Yan_MassCut_10-50_Run_2.root", 0.14)'
#root -q 'MLTest.C("/uscms/home/jpalamad/analysis/CMSSW_14_0_4/src/CMSAnalysis/Output/DarkPhoton_MLStrip_CompleteCuts_Min/darkPhotonBaselineRun2.root", "/uscms/home/jpalamad/analysis/CMSSW_14_0_4/src/CMSAnalysis/Output/DarkPhoton_MLStrip_CompleteCuts_Min/Drell-Yan_MassCut_50-inf_Run_2.root", 0.14)'

#################

# root -q 'MLTest.C("/uscms/home/jpalamad/analysis/CMSSW_14_0_4/src/CMSAnalysis/Output/DarkPhoton_MLStrip_CompleteCuts_Min/darkPhotonBaselineRun2.root", "/uscms/home/jpalamad/analysis/CMSSW_14_0_4/src/CMSAnalysis/Output/DarkPhoton_MLStrip_CompleteCuts_Min/QCD_HTCut_100-200_Run_2_Year_2018.root", 0.14)'
# root -q 'MLTest.C("/uscms/home/jpalamad/analysis/CMSSW_14_0_4/src/CMSAnalysis/Output/DarkPhoton_MLStrip_CompleteCuts_Min/darkPhotonBaselineRun2.root", "/uscms/home/jpalamad/analysis/CMSSW_14_0_4/src/CMSAnalysis/Output/DarkPhoton_MLStrip_CompleteCuts_Min/QCD_HTCut_200-300_Run_2_Year_2018.root", 0.14)'
# root -q 'MLTest.C("/uscms/home/jpalamad/analysis/CMSSW_14_0_4/src/CMSAnalysis/Output/DarkPhoton_MLStrip_CompleteCuts_Min/darkPhotonBaselineRun2.root", "/uscms/home/jpalamad/analysis/CMSSW_14_0_4/src/CMSAnalysis/Output/DarkPhoton_MLStrip_CompleteCuts_Min/QCD_HTCut_300-500_Run_2_Year_2018.root", 0.14)'
# root -q 'MLTest.C("/uscms/home/jpalamad/analysis/CMSSW_14_0_4/src/CMSAnalysis/Output/DarkPhoton_MLStrip_CompleteCuts_Min/darkPhotonBaselineRun2.root", "/uscms/home/jpalamad/analysis/CMSSW_14_0_4/src/CMSAnalysis/Output/DarkPhoton_MLStrip_CompleteCuts_Min/QCD_HTCut_500-700_Run_2_Year_2018.root", 0.14)'
# root -q 'MLTest.C("/uscms/home/jpalamad/analysis/CMSSW_14_0_4/src/CMSAnalysis/Output/DarkPhoton_MLStrip_CompleteCuts_Min/darkPhotonBaselineRun2.root", "/uscms/home/jpalamad/analysis/CMSSW_14_0_4/src/CMSAnalysis/Output/DarkPhoton_MLStrip_CompleteCuts_Min/QCD_HTCut_700-1000_Run_2_Year_2018.root", 0.14)'
# root -q 'MLTest.C("/uscms/home/jpalamad/analysis/CMSSW_14_0_4/src/CMSAnalysis/Output/DarkPhoton_MLStrip_CompleteCuts_Min/darkPhotonBaselineRun2.root", "/uscms/home/jpalamad/analysis/CMSSW_14_0_4/src/CMSAnalysis/Output/DarkPhoton_MLStrip_CompleteCuts_Min/QCD_HTCut_1000-1500_Run_2_Year_2018.root", 0.14)'
# root -q 'MLTest.C("/uscms/home/jpalamad/analysis/CMSSW_14_0_4/src/CMSAnalysis/Output/DarkPhoton_MLStrip_CompleteCuts_Min/darkPhotonBaselineRun2.root", "/uscms/home/jpalamad/analysis/CMSSW_14_0_4/src/CMSAnalysis/Output/DarkPhoton_MLStrip_CompleteCuts_Min/QCD_HTCut_1500-2000_Run_2_Year_2018.root", 0.14)'
# root -q 'MLTest.C("/uscms/home/jpalamad/analysis/CMSSW_14_0_4/src/CMSAnalysis/Output/DarkPhoton_MLStrip_CompleteCuts_Min/darkPhotonBaselineRun2.root", "/uscms/home/jpalamad/analysis/CMSSW_14_0_4/src/CMSAnalysis/Output/DarkPhoton_MLStrip_CompleteCuts_Min/QCD_HTCut_2000-inf_Run_2_Year_2018.root", 0.14)'