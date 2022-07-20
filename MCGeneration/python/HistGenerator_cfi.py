import FWCore.ParameterSet.Config as cms


dimuon = cms.EDAnalyzer('Dimuon', #This cannot be found
                      genPartsTag=cms.InputTag("genParticles"),
                      genInfoProduct=cms.InputTag("generator"),
                      decayParticlePID=cms.int32(13),
                      debug=cms.int32(0),
)

