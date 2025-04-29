
echo "##################### SETUP #####################"

source /cvmfs/cms.cern.ch/cmsset_default.sh
cmsrel CMSSW_14_0_4

# Extract input directories

tar -xzvf uncompiled.tar.gz
tar -xzvf condor_input.tar.gz

cp -r CMSAnalysis CMSSW_14_0_4/src
cp -r condor_input CMSSW_14_0_4/src/CMSAnalysis/BDT

cd CMSSW_14_0_4/src/CMSAnalysis
cmsenv

echo "################### COMPILING #####################"

pwd

# -j2 flag necessary to prevent exceeding memory quota (j4 > 2047, j8 maybe for 8192)
scram b clean
scram b -j8

cd BDT

echo "################### RUNNING ROOT #####################"

root 'crab_BDT_MLTrain.C("condor_input/", "placeholder", "TMVA", "sgMethod=PropWeight bgMethod=PropWeight useDP=1 useNano=0 useDY=0 useQCD=1")'
