from CRABClient.UserUtilities import config
config = config()

config.General.requestName = 'DY50_Data_Processing' #must be unique for each request
config.General.transferOutputs = True
config.General.transferLogs = False

config.JobType.pluginName = 'Analysis'
config.JobType.psetName = 'pset_tutorial_analysis.py'

config.Data.publication = False
config.Data.outputDatasetTag =  config.General.requestName

config.Site.storageSite = 'T3_US_FNALLPC'