# Import configurations
import FWCore.ParameterSet.Config as cms

# set up process
process = cms.Process("Dummy")

process.source = cms.Source("EmptySource",
)
#                            fileNames = cms.untracked.vstring(),
                        #    eventsToProcess = cms.untracked.VEventRange("1:1484800-1:1484810"),
#                            eventsToSkip = cms.untracked.VEventRange("1:1484806-1:1484806")
#                             )
