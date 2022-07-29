# Used for input parameters
from operator import truediv
import FWCore.ParameterSet.VarParsing as VarParsing
options = VarParsing.VarParsing ('analysis')

# Used for processing params textfile
import FWCore.Utilities.FileUtils as FileUtils

#Used for generated filename
#import time
#localtime = time.localtime(time.time())

# Imports more options
import sys
sys.path.append('..')
from python.mcCmdLineOptions_cfi import registerDefaultMCOptions
registerDefaultMCOptions(options)
from python.ledMcCmndLineOptions_cfi import ledRegisterDefaultMCOptions
ledRegisterDefaultMCOptions(options)

# Takes options from user
options.register ('zPrimeModel',
                  "zPrimeSSM",
                  VarParsing.VarParsing.multiplicity.singleton,
                  VarParsing.VarParsing.varType.string,
                  "which Z' model to use")
options.register ('interferenceMode',
                  3,
                  VarParsing.VarParsing.multiplicity.singleton,
                  VarParsing.VarParsing.varType.int,          
                  "Z/gamma/Z' interference setting")
options.register ('outputFilename',
                  "0",
                  VarParsing.VarParsing.multiplicity.singleton,
                  VarParsing.VarParsing.varType.string,
                  "output file name")
options.register ('pythiaSettingsFileName',
                  "0",
                  VarParsing.VarParsing.multiplicity.singleton,
                  VarParsing.VarParsing.varType.string,
                  "pythia settings file name")
options.register ('globalTagName',
                  "0",
                  VarParsing.VarParsing.multiplicity.singleton,
                  VarParsing.VarParsing.varType.string,
                  "Global Tag Identifier")
options.parseArguments()

import FWCore.ParameterSet.Config as cms

# Imports pythia tunes
from Configuration.Generator.Pythia8CommonSettings_cfi import *

from Configuration.Generator.MCTunes2017.PythiaCP5Settings_cfi import *

from Configuration.Generator.PSweightsPythia.PythiaPSweightsSettings_cfi import *

process = cms.Process('GEN')

# Import of standard configurations
process.load('Configuration.StandardSequences.Services_cff')
process.load('SimGeneral.HepPDTESSource.pythiapdt_cfi')
process.load('FWCore.MessageService.MessageLogger_cfi')
process.load('Configuration.EventContent.EventContent_cff')
process.load('SimGeneral.MixingModule.mixNoPU_cfi')
process.load('Configuration.StandardSequences.GeometryRecoDB_cff')
#process.load('Configuration.StandardSequences.MagneticField_38T_PostLS1_cff') #Removed because of errors: 04/11/2020
process.load('Configuration.StandardSequences.MagneticField_cff')
process.load('Configuration.StandardSequences.Generator_cff')
process.load('IOMC.EventVertexGenerators.VtxSmearedNominalCollision2015_cfi')
process.load('GeneratorInterface.Core.genFilterSummary_cff')
process.load('Configuration.StandardSequences.EndOfProcess_cff')
process.load('Configuration.StandardSequences.FrontierConditions_GlobalTag_cff')

# Sets the message logger
process.MessageLogger.cerr.FwkReport = cms.untracked.PSet(
    reportEvery = cms.untracked.int32(500),
    limit = cms.untracked.int32(10000000)
)

# Sets the max events and prompts the user if no value was provided
if options.maxEvents == -1:
    maxEvents = int(input("Please enter the maximum ammount of events:\n"))
else: 
    maxEvents = options.maxEvents
process.maxEvents = cms.untracked.PSet(
    input = cms.untracked.int32(maxEvents)
)

# Input source
process.source = cms.Source("EmptySource")

process.options = cms.untracked.PSet(

)
process.source = cms.Source("EmptySource",
			    firstLuminosityBlock = cms.untracked.uint32(options.seed),
			    numberEventsInLuminosityBlock = cms.untracked.uint32(100)
)

process.RandomNumberGeneratorService.generator.initialSeed = options.seed*10000

#Sets up global tag
globalTagName = options.globalTagName
globalTag = "0"
tagsList = dict({'Run3' : '124X_mcRun3_2022_realistic_v9', 'Run2' : '106X_mcRun2_asymptotic_v13'}) #add more global tags here
tagsLatch = True
while (tagsLatch):
    if (globalTagName != "0"):
            globalTag = tagsList[globalTagName]
    if (globalTag != "0"):
        tagsLatch = False
    else: 
        globalTagName = str(input("Please enter a global tag:\n"))
        
# Other statements
process.genstepfilter.triggerConditions=cms.vstring("generation_step")
from Configuration.AlCa.GlobalTag import GlobalTag
process.GlobalTag = GlobalTag(process.GlobalTag, globalTag)

# Sets the params file and prompts the user if none was provided
pythiasettingsFile = cms.untracked.string("0")
if options.pythiaSettingsFileName != pythiasettingsFile:
    pythiasettingsFile = options.pythiasettingsFileName
else: 
    pythiasettingsFile = str(input("Please enter a pythia settings file name: \n(Format: \"fileName.txt\")\n"))
pythiaparams = FileUtils.loadListFromFile (pythiasettingsFile) 

# Sets up all the pythia stuff
process.generator = cms.EDFilter("Pythia8GeneratorFilter",
        comEnergy =  cms.double(options.comEnergy*1000), #found in mcCmdLineOptions_cfi.py
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

                        #Master Switches all should be one for large events
			            'PartonLevel:MPI = '+str(options.ULE),
			            'PartonLevel:ISR = '+str(options.ISR),
			            'PartonLevel:FSR = '+str(options.FSR),

                        #Phase Space cuts
			#'PhaseSpace:pTHatMax = '+str(options.pTMax),
			#'PhaseSpace:pTHatMin = 1300',#+str(options.pTMin), #Removed because set to default
                        'PhaseSpace:mHatMax = '+str(options.maxMass),                       
                        'PhaseSpace:mHatMin = '+str(options.minMass),

                        *pythiaparams
                ),
                parameterSets = cms.vstring('pythia8CommonSettings',
                                            'pythia8CP5Settings',
                                            'pythia8PSweightsSettings',
                                            'processParameters'
                                            )
        )
)

process.ProductionFilterSequence = cms.Sequence(process.generator)

# Set output filename and prompts user if none is provided
outputFile = cms.untracked.string("0")
if options.outputFilename != outputFile:
    outputFile = cms.untracked.string(str(options.outputFilename))
else:
    outputString = str(input("Please enter an output file name: \n(Format: \"fileName.root\")\n"))
    outputFile = cms.untracked.string('file:' + outputString)

    #For automatically generated filename
    #outputString = 'HPP_PID%d_M%d_D%d_L%d_LL%d_LR_%d_RR_%d_13TeV_pythia8_GEN_%d_%d_%d.root'%(options.pdgId,options.minMass,options.pTMin,options.Lambda,options.helicityLL,options.helicityLR,options.helicityRR,localtime.tm_mon,localtime.tm_mday,localtime.tm_year)
    #print('Output File: ' + outputString)

# Output definition
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
    fileName = outputFile,
    outputCommands = process.AODSIMEventContent.outputCommands
)

process.AODSIMoutput_step = cms.EndPath(process.AODSIMoutput)

# Path and EndPath definitions
process.generation_step = cms.Path(process.pgen)#*process.crossSecTreeMaker*process.pdfTreeMaker)
process.genfiltersummary_step = cms.EndPath(process.genFilterSummary)
process.endjob_step = cms.EndPath(process.endOfProcess)

# Schedule definition
process.schedule = cms.Schedule(process.generation_step,process.genfiltersummary_step,process.endjob_step,process.AODSIMoutput_step)

# Filter all path with the production filter sequence
for path in process.paths:
	getattr(process,path)._seq = process.ProductionFilterSequence * getattr(process,path)._seq

# Dump config
file = open('GenTest.txt','w')
file.write(str(process.dumpPython()))
file.close()

