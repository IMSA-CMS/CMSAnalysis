import FWCore.ParameterSet.Config as cms

# Allows user to enter an input file
import FWCore.ParameterSet.VarParsing as VarParsing
options = VarParsing.VarParsing ('analysis')

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

# Defines the process and loads the message logger
process = cms.Process("Demo")

process.load("FWCore.MessageService.MessageLogger_cfi")

process.MessageLogger.cerr.FwkReport = cms.untracked.PSet(
    reportEvery = cms.untracked.int32(1),
    limit = cms.untracked.int32(10000000)
)

process.maxEvents = cms.untracked.PSet( input = cms.untracked.int32(10000) )

# Sets input file as the source and prompts user if none was provided
inputFile = cms.untracked.string("0")
if options.inputFileName != inputFile:
    inputFile = str(options.inputFileName)
else: 
     inputFile = str(input("Please enter an input file name: \n(Format: \"fileName.root\")\n"))
process.source = cms.Source("PoolSource",
                        fileNames = cms.untracked.vstring('file:'+ inputFile)
                            )

# Attaches EDanalyzer (HistGenerator_cfi)
import sys
sys.path.append('..')
from python.HistGenerator_cfi import *

dimuonPlugin = cms.untracked.string("0")
if options.dimuonPluginName != dimuonPlugin:
    dimuonPlugin = str(options.inputFileName)
else: 
     dimuonPlugin = str(input("Please enter the dimuon plugin name: \n"))

dimuon = GetDimuon(dimuonPlugin)
process.Dimuon=dimuon.clone()
#process.Dimuon.isCI=False
process.Dimuon.debug=False
#process.Dimuon.status=23

# Defines output file
process.TFileService = cms.Service("TFileService",
                                   fileName = cms.string('histograms_'+ inputFile)
)

# Defines the path
process.p = cms.Path(process.Dimuon)
