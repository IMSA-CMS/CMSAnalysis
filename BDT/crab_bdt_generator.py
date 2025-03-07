import argparse
import os

if __name__ == "__main__":
    parser = argparse.ArgumentParser()
    # parser.add_argument("--input")
    # parser.add_argument("--output")
    # parser.add_argument("--analysis")
    # parser.add_argument("--numFiles")
    # parser.add_argument("--skipFiles")
    # parser.add_argument("--folder")
    
    parser.add_argument("--input")
    parser.add_argument("--output")
    parser.add_argument("--trainingArgs")

    args = parser.parse_args()

    prefix = "gen/" + args.output[:-5] + '_'
    config_name = prefix + "crab_config.py"
    script_name = prefix + "runAnalyzer.sh"

    if os.path.exists(config_name):
        os.remove(config_name)
    if os.path.exists(script_name):
        os.remove(script_name)
    script_file = open(script_name, "w")
    script_file.write('CMSSW_BASE=\"/srv\"\n') # change version number when updating
    script_file.write("runAnalyzer crab=1 ")
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

    config_file = open(config_name, "w")
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
config.JobType.scriptExe = '{prefix}runAnalyzer.sh'
config.JobType.outputFiles = ['{args.output}']
config.Data.inputDataset = '/Tau/Run2016H-UL2016_MiniAODv2_NanoAODv9-v1/NANOAOD'
config.Data.inputDBS = 'global'
config.Data.splitting = 'FileBased'
config.Data.unitsPerJob = 1
config.Data.totalUnits = 1
config.Data.publication = False
{f"config.Data.outLFNDirBase = '/store/user/{os.environ['USER']}/{args.folder}'" if args.folder else ""}
config.Site.storageSite = 'T3_US_FNALLPC'
"""
# replace storageSite with T3_CH_CERNBOX if on lxplus
    )
    config_file.close()