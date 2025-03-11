
# ##################### SETUP #####################

ls

echo "########################################"

pwd
ls

# # Extract xxhash locally
# wget https://github.com/Cyan4973/xxHash/archive/v0.8.2.tar.gz
# tar xzf v0.8.2.tar.gz
# cd xxHash-0.8.2
# make
# # Now set LD_LIBRARY_PATH in your job script:
# export LD_LIBRARY_PATH=$LD_LIBRARY_PATH:$(pwd)

# # Extract root locally
# wget https://root.cern/download/root_v6.32.10.Linux-almalinux9.5-x86_64-gcc11.5.tar.gz
# tar -xzvf root_v6.32.10.Linux-almalinux9.5-x86_64-gcc11.5.tar.gz
# source root/bin/thisroot.sh

# ##################### EXECUTION #####################

source /cvmfs/cms.cern.ch/cmsset_default.sh

cmsrel CMSSW_14_0_4

# Extract input directories

tar -xzvf uncompiled.tar.gz
tar -xzvf condor_input.tar.gz

cp -r condor_input CMSSW_14_0_4/src/CMSAnalysis/BDT
cp -r CMSAnalysis CMSSW_14_0_4/src

cd CMSSW_14_0_4/src/CMSAnalysis

cmsenv

# -j2 flag necessary to prevent exceeding memory quota (j4 > 2047, j8 maybe for 8192)
scram b clean
scram b -j8

cd BDT

# echo "########################################"
# printenv

echo "########################################"
find $CMSSW_BASE/src/CMSAnalysis/Modules/interface -name "LeptonJetMLStripModule.hh"
echo $ROOT_INCLUDE_PATH

root 'crab_BDT_MLTrain.C("condor_input/", "placeholder", "TMVA", "sgMethod=PropWeight bgMethod=PropWeight useDP=1 useNano=0 useDY=0 useQCD=1")'
