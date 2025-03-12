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
    parser.add_argument("--useCachedRepo", action="store_true")
    
    args = parser.parse_args()
    
    script_filename = "training_shell.sh" # it's a shell script as in it redirects to another script and it is also a shell script as in .sh and I think this is quite notable
    analysis_directory = os.environ["CMSSW_BASE"] + "/src/CMSAnalysis"
    bdt_directory = analysis_directory + "/BDT/"
    
    # script_content: script to run on condor
    
    script_content = (f"""
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
scram b -j

cd BDT

echo "################### RUNNING ROOT #####################"

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
request_memory = 32768
request_cpus = 4
Executable = {script_filename}
should_transfer_files = YES
when_to_transfer_output = ON_EXIT
Output = condor_logs/bdt_$(Cluster)_$(Process).stdout
Error = condor_logs/bdt_$(Cluster)_$(Process).stderr
Log = condor_logs/bdt_$(Cluster)_$(Process).log
transfer_input_files = uncompiled.tar.gz,condor_input.tar.gz
transfer_output_files = {args.outputFile}.root,{args.outputFile}Rerun.root,TMVAClassification_BDT.class.C,TMVAClassification_BDT.weights.xml
Queue 1
""")
    # request_cpus = 4 
    
    print("JDL file generated successfully")
    
    execute_filename = "execute_condor.sh"

    open(execute_filename, "w").write(f"""                                         
#!/bin/bash

# clean up request files (if necessary)
# rm uncompiled.tar.gz
# rm condor_input.tar.gz
# rm crab_bdt_request.jdl
# rm execute_condor.sh
# rm training_shell.sh

# Check if repo update is required
if [ "$1" != "useCachedRepo" ]; then
    echo "Updating repository..."
    mkdir -p /eos/uscms/store/user/{os.environ["USER"]}/condor_utils
    git clone git@github.com:IMSA-CMS/CMSAnalysis.git /eos/uscms/store/user/{os.environ["USER"]}/condor_utils/temp_repo/CMSAnalysis
    cd /eos/uscms/store/user/{os.environ["USER"]}/condor_utils/temp_repo/CMSAnalysis
    git pull
    
    cd -
    tar -czf uncompiled.tar.gz -C /eos/uscms/store/user/{os.environ["USER"]}/condor_utils/temp_repo CMSAnalysis
    rm -rf /eos/uscms/store/user/{os.environ["USER"]}/condor_utils/temp_repo
else
    echo "Using cached repository (no git update)"
fi

# Fetch input files
cp -r {args.input} ./condor_input
tar -czf condor_input.tar.gz ./condor_input
rm -rf ./condor_input

condor_submit $2
    """)

    # Pass "useCachedRepo" only if the flag is set
    use_cached_flag = "useCachedRepo" if args.useCachedRepo else ""

    submit = Popen(
        ["bash", bdt_directory + execute_filename, use_cached_flag, jdl_filename],
        cwd=bdt_directory
    )
    submit.wait()
