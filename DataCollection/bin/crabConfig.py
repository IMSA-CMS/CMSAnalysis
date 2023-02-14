from CRABClient.UserUtilities import config
config = config()

config.General.requestName = 'DY50_Data_Processing_Test5' #must be unique for each request
config.General.transferOutputs = True
config.General.transferLogs = False

config.JobType.pluginName = 'Analysis'
config.JobType.psetName = 'dummyPSet.py'
config.JobType.scriptExe = 'crabScript.sh'
config.JobType.scriptArgs = ['input=textfiles/SingleMassSnowmass/DY/DYPick50.txt', 'output=dy50.root', 'numFiles=1']
config.JobType.outputFiles = ['crabOutput.txt']

config.Data.publication = False
config.Data.outputDatasetTag =  config.General.requestName
config.Data.userInputFiles = ['textfiles/SingleMassSnowmass/DY/DYPick50.txt']
config.Data.splitting = 'FileBased'
config.Data.unitsPerJob = 1

config.Site.storageSite = 'T3_US_FNALLPC'