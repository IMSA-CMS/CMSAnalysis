import FWCore.ParameterSet.Config as cms

process = cms.Process('NoSplit')

# process.source = cms.Source("PoolSource", fileNames = cms.untracked.vstring('root://cms-xrd-global.cern.ch///store/mc/RunIISummer20UL17RECO/WJetsToLNu_1J_TuneCP5_13TeV-amcatnloFXFX-pythia8/AODSIM/106X_mc2017_realistic_v6-v1/230000/1883111F-FFE4-3D44-8AF0-865BB0C2BE4E.root'))
process.source = cms.Source("PoolSource", fileNames = cms.untracked.vstring(''))
process.maxEvents = cms.untracked.PSet(input = cms.untracked.int32(-1))
process.options = cms.untracked.PSet(wantSummary = cms.untracked.bool(True))
# process.output = cms.OutputModule("PoolOutputModule",
#     outputCommands = cms.untracked.vstring("drop *", "keep recoTracks_*_*_*"),
#     fileName = cms.untracked.string('output.root'),
# )
# process.out = cms.EndPath(process.output)
