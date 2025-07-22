
import CRABClient

from CRABClient.UserUtilities import config
    
config = config()
config.General.requestName = 'bdt_crabTest1'
config.General.workArea = 'crab_projects/bdt_crabTest1'
config.General.transferOutputs = True
config.JobType.psetName = 'PSet.py'
config.JobType.inputFiles = ['FrameworkJobReport.xml', 'input/']
config.JobType.scriptExe = 'training_shell.sh'
config.JobType.outputFiles = ["TMVA.root", "TMVARerun.root", "TMVAClassification_BDT.weights.xml"]
config.Data.inputDBS = 'global'
config.Data.splitting = 'FileBased'
config.Data.unitsPerJob = 1
config.Data.totalUnits = 1
config.Data.publication = False
config.Data.outLFNDirBase = '/store/user/jpalamad/bdt_crabTest1'
config.Site.storageSite = 'T3_US_FNALLPC'