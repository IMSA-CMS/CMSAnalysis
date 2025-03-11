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
    
    script_content = (f"""
# ##################### SETUP #####################

ls

# Extract directory
tar -xzvf precompile.tar.gz
cd CMSSW_14_0_4/src/CMSAnalysis/BDT

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
cmsenv

scram b clean
scram b -j

# echo "########################################"
# printenv

echo "########################################"
find $CMSSW_BASE/src/CMSAnalysis/Modules/interface -name "LeptonJetMLStripModule.hh"
echo $ROOT_INCLUDE_PATH

root \'crab_BDT_MLTrain.C(\"{args.input}\", \"{args.outputDir}\", \"{args.outputFile}\", \"{args.trainingArgs}\")\'
"""
    )

#     script_content = (f"""
# source /cvmfs/cms.cern.ch/cmsset_default.sh
# cmsenv
# """
#     )
    
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
Executable = {script_filename}
should_transfer_files = YES
when_to_transfer_output = ON_EXIT
Output = condor_logs/bdt_$(Cluster)_$(Process).stdout
Error = condor_logs/bdt_$(Cluster)_$(Process).stderr
Log = condor_logs/bdt_$(Cluster)_$(Process).log
transfer_input_files = precompile.tar.gz
transfer_output_files = {args.outputFile}.root,{args.outputFile}Rerun.root,TMVAClassification_BDT.class.C,TMVAClassification_BDT.weights.xml
Queue 1
""")
    
    print("JDL file generated successfully")
    
    execute_filename = "execute_condor.sh"
    open(execute_filename, "w").write(f"""                                         
#!/bin/bash
condor_submit $1
""")
    
    submit = Popen(
        ["bash", bdt_directory + execute_filename, jdl_filename], cwd=bdt_directory
    )
    submit.wait()
