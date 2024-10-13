import CRABClient
from CRABClient.UserUtilities import config 
# using datetime module
import datetime;


config = config()

config.General.requestName = 'exe_test_7'
config.General.workArea = 'crab_projects'
config.General.transferOutputs = True

# config.JobType.pluginName = 'Analysis'
config.JobType.psetName = 'PSet.py'

# empty xml file to not run cmsRun
config.JobType.inputFiles = ['FrameworkJobReport.xml', '../../../']
														#  '$CMSSW_BASE/src', ]
config.JobType.scriptExe = 'runAnalyzer.sh'
config.JobType.outputFiles = ['simpleoutput.txt']

config.Data.inputDataset = '/WJetsToLNu_1J_TuneCP5_13TeV-amcatnloFXFX-pythia8/RunIISummer20UL17RECO-106X_mc2017_realistic_v6-v1/AODSIM'
config.Data.inputDBS = 'global'
config.Data.splitting = 'FileBased'
config.Data.unitsPerJob = 10
config.Data.publication = False
config.Data.outputDatasetTag = 'CRAB3_tutorial_Aug2021_MC_analysis'

config.Site.storageSite = "T2_US_MIT"
