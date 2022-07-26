
import FWCore.ParameterSet.VarParsing as VarParsing
options = VarParsing.VarParsing ('analysis')
from SHarper.MCTools.mcCmdLineOptions_cfi import registerDefaultMCOptions
registerDefaultMCOptions(options)
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
options.parseArguments()


import FWCore.ParameterSet.Config as cms


process = cms.Process('GEN')

# import of standard configurations
process.load('Configuration.StandardSequences.Services_cff')
process.load('SimGeneral.HepPDTESSource.pythiapdt_cfi')
process.load('FWCore.MessageService.MessageLogger_cfi')
process.load('Configuration.EventContent.EventContent_cff')
process.load('SimGeneral.MixingModule.mixNoPU_cfi')
process.load('Configuration.StandardSequences.GeometryRecoDB_cff')
process.load('Configuration.StandardSequences.MagneticField_38T_PostLS1_cff')
process.load('Configuration.StandardSequences.Generator_cff')
process.load('IOMC.EventVertexGenerators.VtxSmearedNominalCollision2015_cfi')
process.load('GeneratorInterface.Core.genFilterSummary_cff')
process.load('Configuration.StandardSequences.EndOfProcess_cff')
process.load('Configuration.StandardSequences.FrontierConditions_GlobalTag_cff')
process.MessageLogger.cerr.FwkReport = cms.untracked.PSet(
    reportEvery = cms.untracked.int32(500),
    limit = cms.untracked.int32(10000000)
)
process.maxEvents = cms.untracked.PSet(
    input = cms.untracked.int32(options.maxEvents)
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

# Other statements
process.genstepfilter.triggerConditions=cms.vstring("generation_step")
from Configuration.AlCa.GlobalTag import GlobalTag
process.GlobalTag = GlobalTag(process.GlobalTag, 'MCRUN2_71_V1::All')


process.generator = cms.EDFilter("Pythia8GeneratorFilter",
        comEnergy =  cms.double(options.comEnergy*1000),
        crossSection = cms.untracked.double(1e10),
        filterEfficiency = cms.untracked.double(1),
        maxEventsToPrint = cms.untracked.int32(0),
        pythiaHepMCVerbosity = cms.untracked.bool(False),
        pythiaPylistVerbosity = cms.untracked.int32(1),
        PythiaParameters = cms.PSet(
                processParameters = cms.vstring(
                        'Main:timesAllowErrors    = 10000',
                        'ParticleDecays:limitTau0 = on',
                        'ParticleDecays:tauMax = 10',
                        'Tune:pp 5',
                        'Tune:ee 3',

                        'ContactInteractions:QCffbar2mumubar = on',
                        '23:onMode = off',
                        '23:onIfAny = 13',
			'PartonLevel:ISR = on',
			'PartonLevel:FSR = on',
			'PhaseSpace:pTHatMin = '+str(options.pTMin),
                        'PhaseSpace:mHatMin = '+str(options.minMass),
                        'PhaseSpace:mHatMax = '+str(options.maxMass),
			'PhaseSpace:pTHatMax = '+str(options.pTMax),

                ),
                parameterSets = cms.vstring('processParameters')
        )
)


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
    fileName =
cms.untracked.string('file:DYToMuMu_M%d_D%d_13TeV_pythia8_GEN_Sep2_%d.root'%(options.minMass,options.pTMin,options.seed)),
    outputCommands = process.AODSIMEventContent.outputCommands
)

process.AODSIMoutput_step = cms.EndPath(process.AODSIMoutput)










# Path and EndPath definitions
process.generation_step = cms.Path(process.pgen)#*process.crossSecTreeMaker*process.pdfTreeMaker)
process.genfiltersummary_step = cms.EndPath(process.genFilterSummary)
process.endjob_step = cms.EndPath(process.endOfProcess)

# Schedule definition
process.schedule = cms.Schedule(process.generation_step,process.genfiltersummary_step,process.endjob_step,process.AODSIMoutput_step)

# filter all path with the production filter sequence
for path in process.paths:
	getattr(process,path)._seq = process.ProductionFilterSequence * getattr(process,path)._seq 

