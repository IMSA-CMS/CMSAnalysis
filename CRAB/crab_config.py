import CRABClient
from CRABClient.UserUtilities import config
config = config()
config.General.requestName = 'QCD_HTCut_2000-Inf_Run_2_Year_2018'
config.General.workArea = 'crab_projects/Test2'
config.General.transferOutputs = True
config.JobType.psetName = 'PSet.py'
config.JobType.inputFiles = ['FrameworkJobReport.xml', 'input/']
config.JobType.scriptExe = 'runAnalyzer.sh'
config.JobType.outputFiles = ['QCD_HTCut_2000-Inf_Run_2_Year_2018.root']
config.Data.inputDataset = '/Tau/Run2016H-UL2016_MiniAODv2_NanoAODv9-v1/NANOAOD'
config.Data.inputDBS = 'global'
config.Data.splitting = 'FileBased'
config.Data.unitsPerJob = 1
config.Data.totalUnits = 1
config.Data.publication = False
config.Data.outputDatasetTag = 'Test2'
config.Site.storageSite = 'T3_US_FNALLPC'
