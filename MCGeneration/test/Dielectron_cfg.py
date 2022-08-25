import FWCore.ParameterSet.Config as cms

process = cms.Process("Demo")

process.load("FWCore.MessageService.MessageLogger_cfi")

process.MessageLogger.cerr.FwkReport = cms.untracked.PSet(
    reportEvery = cms.untracked.int32(1),
    limit = cms.untracked.int32(10000000)
)

process.maxEvents = cms.untracked.PSet( input = cms.untracked.int32(200000) )
#filedir = '/DYToMuMu/szaleski-M200_13TeV_pythia8_GEN-c932f08e13e94abe45c501c7b6a032e1/USER'
process.source = cms.Source("PoolSource",
            # replace 'myfile.root' with the source file you want to use
                        fileNames = cms.untracked.vstring(
                                #  'file:$CMSSW_BASE/src/GenStudy/Dimuon/test/DYToMuMu_M-1_13TeV_pythia8_GEN.root
#                                'file:/afs/cern.ch/user/s/szaleski/work/private/CMSSW_744_MCGen/src/GenStudy/Dimuon/test/CIToEE_M1500_D0_L16000_8TeV_pythia8_GEN_LL-1_LR0_RR0_Jun30.root'
#                                'file:/afs/cern.ch/user/s/sturdy/work/public/WSUAnalysis/DiLeptons/CIToMuMu_M0_D0_L20000_Des_13TeV_pythia8_GEN_0_7.root',
                                #'file:%s/DYToMuMu_M0_D0_13TeV_pythia8_GEN_0.root'%(filedir),
#                                'root://cmsxrootd.fnal.gov//store/mc/Summer12_DR53X/CIToMuMu_Con_Lambda-11_M-300_TuneZ2star_8TeV-pythia6/AODSIM/PU_S10_START53_V19E-v1/10000/1248C92B-8FF6-E211-A6E8-7845C4FC371F.root',

                                  'file:/afs/cern.ch/work/s/szaleski/private/CMSSW_744_MCGen/src/GenStudy/Dimuon/test/CIToEE_M300_D0_L22000_13TeV_pythia8_GEN_LL0_LR1_RR0_JuL14.root'
#                                'root://cmsxrootd.fnal.gov//store/user/szaleski/DYToEE/EE_DY_pythia8_GEN/160701_161807/0000/CIToEE_M300_D0_L16000_8TeV_pythia8_GEN_LL-1_LR0_RR0_Jun30_10.root',
#                                'root://cmsxrootd.fnal.gov//store/user/szaleski/DYToEE/EE_DY_pythia8_GEN/160701_161807/0000/CIToEE_M300_D0_L16000_8TeV_pythia8_GEN_LL-1_LR0_RR0_Jun30_1.root',


 #                               'file:%s/DYToMuMu_M0_D200_13TeV_pythia8_GEN_25.root'%(filedir),
                                )
                            )

from GenStudy.Dimuon.Dimuon_cfi import *
process.Dimuon=dimuon.clone()
process.Dimuon.isCI=True
process.Dimuon.debug=True
process.Dimuon.status=23
process.Dimuon.leptonId=11

process.TFileService = cms.Service("TFileService",
                                   fileName = cms.string("Pythia8_Jul18_CIEE_13TeV_CIM300.root")                                   
#                                   fileName = cms.string("Pythia8_Jul6_CIEE_13TeV_CIM1500LLCon.root")
#                                   fileName = cms.string("Pythia8_Jul6_CIEE_13TeV_CIM300LLConTest.root")
)



process.p = cms.Path(process.Dimuon)
