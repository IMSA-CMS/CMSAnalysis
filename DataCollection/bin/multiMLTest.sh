#!/bin/bash

root -q 'MLTest.C("BDTFiles/strippedSG_numFiles1.root", "BDTFiles/strippedBG_DY10_numFiles1.root", 0.24)'
root -q 'MLTest.C("BDTFiles/strippedSG_numFiles1.root", "BDTFiles/strippedBG_DY50_numFiles5.root", 0.24)'
root -q 'MLTest.C("BDTFiles/strippedSG_numFiles1.root", "BDTFiles/strippedBG_QCD100-200_numFiles1.root", 0.24)'
root -q 'MLTest.C("BDTFiles/strippedSG_numFiles1.root", "BDTFiles/strippedBG_QCD200-300_numFiles1.root", 0.24)'
root -q 'MLTest.C("BDTFiles/strippedSG_numFiles1.root", "BDTFiles/strippedBG_QCD300-500_numFiles1.root", 0.24)'
root -q 'MLTest.C("BDTFiles/strippedSG_numFiles1.root", "BDTFiles/strippedBG_QCD500-700_numFiles1.root", 0.24)'
root -q 'MLTest.C("BDTFiles/strippedSG_numFiles1.root", "BDTFiles/strippedBG_QCD700-1000_numFiles1.root", 0.24)'
root -q 'MLTest.C("BDTFiles/strippedSG_numFiles1.root", "BDTFiles/strippedBG_QCD1000-1500_numFiles1.root", 0.24)'
root -q 'MLTest.C("BDTFiles/strippedSG_numFiles1.root", "BDTFiles/strippedBG_QCD1500-2000_numFiles1.root", 0.24)'
root -q 'MLTest.C("BDTFiles/strippedSG_numFiles1.root", "BDTFiles/strippedBG_QCD2000-inf_numFiles1.root", 0.24)'