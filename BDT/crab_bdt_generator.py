import argparse
import os
from subprocess import Popen

from multiprocessing import Process
import sys
import subprocess


if __name__ == "__main__":
    parser = argparse.ArgumentParser()
    parser.add_argument("--trainingID")
    parser.add_argument("--input")
    parser.add_argument("--outputDir")
    parser.add_argument("--outputFile")
    parser.add_argument("--trainingArgs")
    
    args = parser.parse_args()
    
    script_filename = "training_shell.sh" # it's a shell script as in it redirects to another script and it is also a shell script as in .sh and I think this is quite notable
    analysis_directory = os.environ["CMSSW_BASE"] + "/src/CMSAnalysis"
    bdt_directory = analysis_directory + "/BDT/"
    
    # script_content: script to run on condor
    
    script_content = (f"""
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

root \'crab_BDT_MLTrain.C(\"condor_input/\", \"{args.outputDir}\", \"{args.outputFile}\", \"{args.trainingArgs}\")\'
"""
    )
    
    try:
        with open(script_filename, "w") as f:
            f.write(script_content)
        print(f"Shell script '{script_filename}' has been generated successfully.")
    except Exception as e:
        print(f"An error occurred: {e}")
        
    print("Condor Config file generated successfully.")
        
    jdl_filename = "crab_bdt_request.jdl"
    open(jdl_filename, "w").write(f"""                                         
universe = vanilla
request_memory = 8192
request_cpus = 4
Executable = {script_filename}
should_transfer_files = YES
when_to_transfer_output = ON_EXIT
Output = condor_logs/bdt_$(Cluster)_$(Process).stdout
Log = condor_logs/bdt_$(Cluster)_$(Process).log
transfer_input_files = uncompiled.tar.gz,condor_input.tar.gz
transfer_output_files = {args.outputFile}.root,{args.outputFile}Rerun.root,TMVAClassification_BDT.class.C,TMVAClassification_BDT.weights.xml
Queue 1
""")
    
    # Error = condor_logs/bdt_$(Cluster)_$(Process).stderr
    
    print("JDL file generated successfully")
    
    execute_filename = "execute_condor.sh"
    open(execute_filename, "w").write(f"""                                         
#!/bin/bash

# Update repo

mkdir /eos/uscms/store/user/{os.environ["USER"]}/condor_utils/temp_repo
git clone git@github.com:IMSA-CMS/CMSAnalysis.git /eos/uscms/store/user/{os.environ["USER"]}/condor_utils/temp_repo
tar -czf uncompiled.tar.gz /eos/uscms/store/user/{os.environ["USER"]}/condor_utils/temp_repo/CMSAnalysis
rm -rf /eos/uscms/store/user/{os.environ["USER"]}/condor_utils/temp_repo

# Fetch input files

cp -r {args.input} ./condor_input
tar -czf condor_input.tar.gz ./condor_input
rm -rf ./condor_input

condor_submit $1

# clean up request files

rm uncompiled.tar.gz
rm condor_input.tar.gz
rm crab_bdt_request.jdl
rm execute_condor.sh
rm training_shell.sh
""")
    
    submit = Popen(
        ["bash", bdt_directory + execute_filename, jdl_filename], cwd=bdt_directory
    )
    submit.wait()
