import FWCore.ParameterSet.Config as cms

process = cms.Process("Demo")

process.load("FWCore.MessageService.MessageLogger_cfi")

process.maxEvents = cms.untracked.PSet( input = cms.untracked.int32(5000) )

process.options = cms.untracked.PSet( SkipEvent = cms.untracked.vstring('ProductNotFound'))
process.source = cms.Source("PoolSource",
                                # replace 'myfile.root' with the source file you want to use
                                fileNames = cms.untracked.vstring(
            #'file:/afs/cern.ch/cms/Tutorials/TWIKI_DATA/TTJets_8TeV_53X.root'
            '/store/data/Run2016C/DoubleEG/MINIAOD/03Feb2017-v1/80000/EAEBC799-70EC-E611-A18A-02163E019D0D.root'
            #'/store/relval/CMSSW_10_6_14/RelValZMM_13/MINIAODSIM/106X_mc2017_realistic_v7-v1/10000/0EB976F4-F84B-814D-88DA-CB2C29A52D72.root'
            #'/store/mc/RunIISummer16NanoAODv7/HPlusPlusHMinusMinusHRTo4L_M-500_TuneCUETP8M1_13TeV-pythia8/NANOAODSIM/PUMoriond17_Nano02Apr2020_102X_mcRun2_asymptotic_v8-v1/270000/486D1D1F-9EAC-E247-8433-EFAFC0BCC5DE.root'
            #'/store/data/Run2017F/DoubleEG/NANOAOD/UL2017_MiniAODv2_NanoAODv9-v1/70000/61B08DB4-292E-734A-B082-E56243B5365A.root'
                )
                            )

process.demo = cms.EDAnalyzer('RunAnalyzerWrapper',
    analyzerType   = cms.untracked.string('HiggsBackground'),
    rootOutFile = cms.untracked.string( 'output.root' )
                              )


process.p = cms.Path(process.demo)