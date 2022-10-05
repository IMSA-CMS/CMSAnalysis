import FWCore.ParameterSet.Config as cms
process = cms.Process('GEN')

# Needed to implement command line input parameters
import FWCore.ParameterSet.VarParsing as VarParsing
options = VarParsing.VarParsing ('analysis')

# Takes command line input parameters from user
options.register ('outputFileName',
                  "0",
                  VarParsing.VarParsing.multiplicity.singleton,
                  VarParsing.VarParsing.varType.string,
                  "output file name")
options.register ('pythiaSettingsFile',
                  "0",
                  VarParsing.VarParsing.multiplicity.singleton,
                  VarParsing.VarParsing.varType.string,
                  "pythia settings file name")
options.register ('globalTagKey',
                  "0",
                  VarParsing.VarParsing.multiplicity.singleton,
                  VarParsing.VarParsing.varType.string,
                  "Global Tag Identifier")
options.parseArguments()

# The 4 input parameters below, maxEvents, globalTagKey, pythiaSettingsFile, and outputFile
# are necessary for the program to run, thus if they are not entered it prompts the user to enter them.
maxEvents = options.maxEvents
if maxEvents == -1: # -1 is the default value
    maxEvents = int(input("Please enter the ammount of events:\n"))
process.maxEvents = cms.untracked.PSet(input = cms.untracked.int32(maxEvents))

globalTagKey = options.globalTagKey
globalTag = "0"
globalTagsList = dict({'Run3' : '124X_mcRun3_2022_realistic_v9', 'Run2' : '106X_mcRun2_asymptotic_v13'})
isValidTag = False
while not isValidTag:
    if globalTagKey != "0":
            globalTag = globalTagsList[globalTagKey]
    if globalTag != "0":
        isValidTag = True
    else: 
        globalTagKey = str(input("Please enter a global tag:\n"))

import FWCore.Utilities.FileUtils as FileUtils
pythiaSettingsFile = options.pythiaSettingsFile
if pythiaSettingsFile == "0":
    pythiaSettingsFile = str(input("Please enter a pythia settings file name: \n(Format: \"fileName.txt\")\n"))
importedPythiaSettings = FileUtils.loadListFromFile (pythiaSettingsFile) 

outputFileName = options.outputFileName
if outputFileName == "0":
    outputFileName = input("Please enter an output file name: \n(Format: \"fileName.root\")\n") 
    
# Pythia definitions
from Configuration.Generator.Pythia8CommonSettings_cfi import *
from Configuration.Generator.MCTunes2017.PythiaCP5Settings_cfi import *
from Configuration.Generator.PSweightsPythia.PythiaPSweightsSettings_cfi import *

process.load('Configuration.StandardSequences.Services_cff')
process.load('SimGeneral.HepPDTESSource.pythiapdt_cfi')
process.load('FWCore.MessageService.MessageLogger_cfi')
process.load('Configuration.EventContent.EventContent_cff')
process.load('SimGeneral.MixingModule.mixNoPU_cfi')
process.load('Configuration.StandardSequences.GeometryRecoDB_cff')
process.load('Configuration.StandardSequences.MagneticField_cff')
process.load('Configuration.StandardSequences.Generator_cff')
process.load('IOMC.EventVertexGenerators.VtxSmearedNominalCollision2015_cfi')
process.load('GeneratorInterface.Core.genFilterSummary_cff')
process.load('Configuration.StandardSequences.EndOfProcess_cff')
process.load('Configuration.StandardSequences.FrontierConditions_GlobalTag_cff')

process.generator = cms.EDFilter("Pythia8GeneratorFilter",
    comEnergy =  cms.double(13000), 
    crossSection = cms.untracked.double(1e10),
    filterEfficiency = cms.untracked.double(1),
    maxEventsToPrint = cms.untracked.int32(0),
    pythiaHepMCVerbosity = cms.untracked.bool(False),
    pythiaPylistVerbosity = cms.untracked.int32(1),
    PythiaParameters = cms.PSet(
        pythia8CommonSettingsBlock,
        pythia8CP5SettingsBlock,
        pythia8PSweightsSettingsBlock,
        processParameters = cms.vstring(
            *importedPythiaSettings
        ),
        parameterSets = cms.vstring(
            'pythia8CommonSettings',
            'pythia8CP5Settings',
            'pythia8PSweightsSettings',
            'processParameters'
        )
    )
)

# More definitions
process.MessageLogger.cerr.FwkReport = cms.untracked.PSet(
    reportEvery = cms.untracked.int32(500),
    limit = cms.untracked.int32(10000000)
)

import random
process.source = cms.Source("EmptySource",
			    firstLuminosityBlock = cms.untracked.uint32(random.randrange(1,10000)),
			    numberEventsInLuminosityBlock = cms.untracked.uint32(100))

process.RandomNumberGeneratorService.generator.initialSeed = random.randrange(1,10000)

process.genstepfilter.triggerConditions=cms.vstring("generation_step")
from Configuration.AlCa.GlobalTag import GlobalTag
process.GlobalTag = GlobalTag(process.GlobalTag, globalTag)
process.ProductionFilterSequence = cms.Sequence(process.generator)

process.AODSIMoutput = cms.OutputModule("PoolOutputModule",
    SelectEvents = cms.untracked.PSet(
        SelectEvents = cms.vstring('generation_step')
    ),
    compressionAlgorithm = cms.untracked.string('LZMA'),
    compressionLevel = cms.untracked.int32(4),
    dataset = cms.untracked.PSet(
        dataTier = cms.untracked.string('GEN-SIM'),
        filterName = cms.untracked.string('')
    ),
    eventAutoFlushCompressedSize = cms.untracked.int32(15728640),
    fileName = cms.untracked.string('file:' + str(outputFileName)),
    outputCommands = process.AODSIMEventContent.outputCommands
)

process.AODSIMoutput_step = cms.EndPath(process.AODSIMoutput)

process.generation_step = cms.Path(process.pgen)
process.genfiltersummary_step = cms.EndPath(process.genFilterSummary)
process.endjob_step = cms.EndPath(process.endOfProcess)

process.schedule = cms.Schedule(process.generation_step,process.genfiltersummary_step,process.endjob_step,process.AODSIMoutput_step)

for path in process.paths:
	getattr(process,path)._seq = process.ProductionFilterSequence * getattr(process,path)._seq

# Dump config
file = open('GenTest.txt','w')
file.write(str(process.dumpPython()))
file.close()

