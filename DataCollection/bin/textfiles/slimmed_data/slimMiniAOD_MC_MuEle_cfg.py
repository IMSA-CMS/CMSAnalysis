## import skeleton process
import FWCore.ParameterSet.Config as cms

process = cms.Process("DAS")

process.load("FWCore.MessageService.MessageLogger_cfi")

process.maxEvents = cms.untracked.PSet( input = cms.untracked.int32(1000) )

process.source = cms.Source("PoolSource",
    fileNames = cms.untracked.vstring(
      'root://cmseos.fnal.gov//store/user/cmsdas/2022/pre_exercises/Set2/DYJetsToLL.root'
    )
)


process.out = cms.OutputModule("PoolOutputModule",
    fileName = cms.untracked.string('slimMiniAOD_MC_MuEle.root'),
    outputCommands = cms.untracked.vstring(['drop *', 'keep *_slimmedMuons__*', 'keep *_slimmedElectrons__*'])
)

process.end = cms.EndPath(process.out)
