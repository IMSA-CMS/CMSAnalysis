import FWCore.ParameterSet.Config as cms

quadlepton = cms.EDAnalyzer('Quadlepton',
                        genPartsTag=cms.InputTag("genParticles"),
                        genInfoProduct=cms.InputTag("generator"),
                        decayParticlePID=cms.int32(13),
                        debug=cms.int32(0)

                        #Original
                        #genPartsTag=cms.InputTag("genParticles"),
                        #decayParticlePID=cms.int32(11),
                        #debug=cms.int32(0)
)
