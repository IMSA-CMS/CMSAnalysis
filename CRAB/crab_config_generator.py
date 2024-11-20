import argparse
import os

if __name__ == "__main__":
    parser = argparse.ArgumentParser()
    parser.add_argument("--input")
    parser.add_argument("--output")
    parser.add_argument("--analysis")
    parser.add_argument("--numFiles")
    parser.add_argument("--skipFiles")
    parser.add_argument("--folder")

    args = parser.parse_args()

    if os.path.exists("crab_config.py"):
        os.remove("crab_config.py")
    if os.path.exists("runAnalyzer.sh"):
        os.remove("runAnalyzer.sh")
    script_file = open("runAnalyzer.sh", "w")
    script_file.write("cmsenv\nrunAnalyzer crab=1 ")
    if args.input:
        script_file.write("input=" + args.input + " ")
    if args.output:
        script_file.write("output=" + args.output + " ")
    if args.numFiles:
        script_file.write("numFiles=" + args.numFiles + " ")
    if args.skipFiles:
        script_file.write("skipFiles=" + args.skipFiles + " ")
    if args.analysis:
        script_file.write("analysis=" + args.analysis + " ")
    script_file.write("\n")
    script_file.close()

    config_file = open("crab_config.py", "w")
    config_file.write("")
    config_file.write(
        f"""import CRABClient

from CRABClient.UserUtilities import config
config = config()
config.General.requestName = '{args.output[0:len(args.output) - 5]}'
config.General.workArea = '{"crab_projects" + ("/" + args.folder if args.folder else "")}'
config.General.transferOutputs = True
config.JobType.psetName = 'PSet.py'
config.JobType.inputFiles = ['FrameworkJobReport.xml', 'input/']
config.JobType.scriptExe = 'runAnalyzer.sh'
config.JobType.outputFiles = ['{args.output}']
config.Data.inputDataset = '/Tau/Run2016H-UL2016_MiniAODv2_NanoAODv9-v1/NANOAOD'
config.Data.inputDBS = 'global'
config.Data.splitting = 'FileBased'
config.Data.unitsPerJob = 1
config.Data.totalUnits = 1
config.Data.publication = False
{f"config.Data.outputDatasetTag = '{args.folder}'" if args.folder else ""}
config.Site.storageSite = 'T3_US_FNALLPC'
"""
    )
    config_file.close()


# config.General.requestName = 'root_output_test'
# config.General.workArea = 'crab_projects'
# config.General.transferOutputs = True

# # config.JobType.pluginName = 'Analysis'
# config.JobType.psetName = 'PSet.py'

# # empty xml file to not run cmsRun
# config.JobType.inputFiles = ['FrameworkJobReport.xml', 'input/']
# config.JobType.scriptExe = 'runAnalyzer.sh'
# config.JobType.outputFiles = ['simpleoutput.txt', 'test.root']

# config.Data.inputDataset = '/Tau/Run2016H-UL2016_MiniAODv2_NanoAODv9-v1/NANOAOD'
# config.Data.inputDBS = 'global'
# config.Data.splitting = 'FileBased'
# config.Data.unitsPerJob = 1
# config.Data.totalUnits = 1
# config.Data.publication = False
# config.Data.outputDatasetTag = 'CRAB3_tutorial_Aug2021_MC_analysis'

# # Should be replaced w/ T3_US_FNALLPC if you have access, this is just public site for testing
# config.Site.storageSite = "T3_US_FNALLPC"
