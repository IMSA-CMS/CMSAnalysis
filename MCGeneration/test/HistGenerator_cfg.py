import FWCore.ParameterSet.Config as cms

# Needed to implement command line input parameters
import FWCore.ParameterSet.VarParsing as VarParsing
options = VarParsing.VarParsing ('analysis')

# Takes command line input parameters from user
options.register ('inputFileName',
                  "0",
                  VarParsing.VarParsing.multiplicity.singleton,
                  VarParsing.VarParsing.varType.string,
                  "input file name")
options.register ('dimuonPluginName',
                  "0",
                  VarParsing.VarParsing.multiplicity.singleton,
                  VarParsing.VarParsing.varType.string,
                  "dimoun plugin name")
options.parseArguments()

# Prompts the user to enter input dimuonPluginName and inputFileName if they weren't already entered.
dimuonPluginName = options.dimuonPluginName
if dimuonPluginName == "0":
    dimuonPluginName = str(input("Please enter the dimuon plugin name: \n"))

inputFile = options.inputFileName
if inputFile == "0":
    inputFile = str(input("Please enter an input file name: \n(Format: \"fileName.root\")\n"))

# Definitions
process = cms.Process("Demo")

process.load("FWCore.MessageService.MessageLogger_cfi")

process.MessageLogger.cerr.FwkReport = cms.untracked.PSet(
    reportEvery = cms.untracked.int32(1),
    limit = cms.untracked.int32(10000000)
)

process.maxEvents = cms.untracked.PSet(input = cms.untracked.int32(2147483647))

process.source = cms.Source("PoolSource",
    fileNames = cms.untracked.vstring('file:'+ inputFile)
)

dimuon = cms.EDAnalyzer(dimuonPluginName,
                      genPartsTag=cms.InputTag("genParticles"),
                      genInfoProduct=cms.InputTag("generator"),
                      decayParticlePID=cms.int32(13),
                      debug=cms.int32(0))
process.Dimuon=dimuon.clone()
process.Dimuon.debug=False

process.TFileService = cms.Service("TFileService",
    fileName = cms.string('histograms_'+ inputFile)
)

process.p = cms.Path(process.Dimuon)
