import CRABClient
from CRABClient.UserUtilities import config 
# using datetime module
import datetime;


config = config()

config.General.requestName = 'root_output_test_4'
config.General.workArea = 'crab_projects'
config.General.transferOutputs = True

# config.JobType.pluginName = 'Analysis'
config.JobType.psetName = 'PSet.py'

# empty xml file to not run cmsRun
config.JobType.inputFiles = ['FrameworkJobReport.xml', 'input/']
config.JobType.scriptExe = 'runAnalyzer.sh'
config.JobType.outputFiles = ['simpleoutput.txt', 'test.root']

config.Data.inputDataset = '/Tau/Run2016H-UL2016_MiniAODv2_NanoAODv9-v1/NANOAOD'
config.Data.inputDBS = 'global'
config.Data.splitting = 'FileBased'
config.Data.unitsPerJob = 1
config.Data.totalUnits = 1
config.Data.publication = False
config.Data.outputDatasetTag = 'CRAB3_tutorial_Aug2021_MC_analysis'

# Should be replaced w/ T3_US_FNALLPC if you have access, this is just public site for testing
config.Site.storageSite = "T2_US_Nebraska"