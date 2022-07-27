## import skeleton process
import FWCore.ParameterSet.Config as cms

process = cms.Process("DAS")

process.load("FWCore.MessageService.MessageLogger_cfi")

#process.maxEvents = cms.untracked.PSet( input = cms.untracked.int32(1000) )

process.source = cms.Source("PoolSource",
    fileNames = cms.untracked.vstring(
      #'/store/data/Run2016C/DoubleEG/MINIAOD/03Feb2017-v1/80000/EAEBC799-70EC-E611-A18A-02163E019D0D.root'
    )
)


process.out = cms.OutputModule("PoolOutputModule",
    fileName = cms.untracked.string('slimMiniAOD_data_MuEle.root'),
    outputCommands = cms.untracked.vstring(['keep *'])#'drop *', 'keep *_slimmedMuons__*', 'keep *_slimmedElectrons__*', 'keep *_generator__*', 'keep *_prunedGenParticles__*', 'keep *_slimmedPhotons__*', 'keep *_slimmedJets__*', 'keep *_slimmedMETs__*', 'keep *_TriggerResults__*'])
)

process.end = cms.EndPath(process.out)
