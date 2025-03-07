import CRABClient

from CRABClient.UserUtilities import config

import argparse
import os

if __name__ == "__main__":
    parser = argparse.ArgumentParser()
    parser.add_argument("--trainingID")
    parser.add_argument("--input")
    parser.add_argument("--outputDir")
    parser.add_argument("--outputFile")
    parser.add_argument("--trainingArgs")
    
    args = parser.parse_args()
    
    script_filename = "training_shell.sh" # it's a shell script as in it redirects to another script and it is also a shell script as in .sh and I think this is quite notable
    script_content = (
        'CMSSW_BASE="/srv"\n'
        f'root \'crab_BDT_MLTrain.C("{args.input}", "{args.outputDir}", "{args.outputFile}", "{args.trainingArgs}")\'\n'
    )
    
    try:
        with open(script_filename, "w") as f:
            f.write(script_content)
        print(f"Shell script '{script_filename}' has been generated successfully.")
    except Exception as e:
        print(f"An error occurred: {e}")
    
    config = config()
    config.General.requestName = args.trainingID
    config.General.workArea = f'crab_projects/{args.trainingID}'
    config.General.transferOutputs = True
    config.JobType.psetName = 'PSet.py'
    config.JobType.inputFiles = ['FrameworkJobReport.xml', 'input/']
    config.JobType.scriptExe = script_filename
    config.JobType.outputFiles = [f"{args.outputFile}.root", f"{args.outputFile}Rerun.root", "TMVAClassification_BDT.weights.xml"]
    config.Data.inputDBS = 'global'
    config.Data.splitting = 'FileBased'
    config.Data.unitsPerJob = 1
    config.Data.totalUnits = 1
    config.Data.publication = False
    config.Data.outLFNDirBase = f'/store/user/jpalamad/{args.outputDir}'
    config.Site.storageSite = 'T3_US_FNALLPC'