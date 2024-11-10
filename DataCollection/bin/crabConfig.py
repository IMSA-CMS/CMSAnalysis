from CRABClient.UserUtilities import config
config = config()

config.General.requestName = 'DY50_Data_Processing_Test5' #must be unique for each request
config.General.transferOutputs = True
config.General.transferLogs = False

config.JobType.pluginName = 'Analysis'
config.JobType.psetName = 'dummyPSet.py'
config.JobType.scriptExe = 'crabScript.sh'
config.JobType.scriptArgs = ['input=textfiles/Run2PickFiles/DY50-inf.txt', 'output=dy50.root', 'analysis=HiggsBackground']
config.JobType.outputFiles = ['crabOutput.txt', 'dy50.root']

config.Data.publication = False
config.Data.outputDatasetTag =  config.General.requestName
config.Data.userInputFiles = ['textfiles/Run2PickFiles/DY50-inf.txt']
config.Data.splitting = 'FileBased'
config.Data.unitsPerJob = 1

config.Site.storageSite = 'T3_US_FNALLPC'