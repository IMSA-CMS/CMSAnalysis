import FWCore.ParameterSet.Config as cms

def GetDimuon(DimuonType):
    return cms.EDAnalyzer(DimuonType,
                      genPartsTag=cms.InputTag("genParticles"),
                      genInfoProduct=cms.InputTag("generator"),
                      decayParticlePID=cms.int32(13),
                      debug=cms.int32(0),
    )