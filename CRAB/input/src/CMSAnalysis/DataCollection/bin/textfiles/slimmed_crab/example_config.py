from CRABClient.UserUtilities import config
config = config()

config.General.requestName = 'nkilmer_luminosity_crab'

config.JobType.pluginName = 'Analysis'
# Name of the CMSSW configuration file
config.JobType.psetName = '/uscms/home/nkilmer/SIR/tmp/CMSSW_11_1_8/src/CMSAnalysis/DataCollection/bin/textfiles/slimmed_data/slimMiniAOD_data_MuEle_cfg.py'

config.Data.inputDataset = '/SingleMuon/Run2016B-23Sep2016-v3/MINIAOD'
config.Data.splitting = 'LumiBased'
config.Data.unitsPerJob = 1000
config.Data.publication = False
# This string is used to construct the output dataset name
config.Data.outputDatasetTag = 'CRAB3_SlimmedMuons'

# These values only make sense for processing data
#    Select input data based on a lumi mask
config.Data.lumiMask = '/uscms/home/nkilmer/SIR/tmp/CMSSW_11_1_8/src/CMSAnalysis/DataCollection/bin/textfiles/slimmed_crab/Cert_271036-284044_13TeV_PromptReco_Collisions16_JSON_MuonPhys.txt'
#    Select input data based on run-ranges
config.Data.runRange = '273150-275376'
config.Data.outLFNDirBase = '/store/user/nkilmer/'

# Where the output files will be transmitted to
config.Site.storageSite = 'T3_US_FNALLPC'
