import FWCore.ParameterSet.Config as cms
import FWCore.PythonUtilities.LumiList as LumiList
from FWCore.ParameterSet.VarParsing import VarParsing
options = VarParsing ('analysis')

options.register ('input',
              '',
              VarParsing.multiplicity.singleton,
              VarParsing.varType.string,
              "Input Datasets")
options.register ('output',
              'output.root',
			  VarParsing.multiplicity.singleton,
              VarParsing.varType.string,
              "Output File Name")
options.register ('analysis',
              '',
			  VarParsing.multiplicity.singleton,
              VarParsing.varType.string,
              "Analysis Plan")
options.register ('events',
              1000,
			  VarParsing.multiplicity.singleton,
              VarParsing.varType.int,
              "Maximum events to process")
options.register ('luminosityFilter',
              '',
              VarParsing.multiplicity.singleton,
              VarParsing.varType.string,
              "Luminosity Filter")
options.parseArguments()


process = cms.Process("Demo")

process.load("FWCore.MessageService.MessageLogger_cfi")

process.maxEvents = cms.untracked.PSet( input = cms.untracked.int32(options.events))

process.options = cms.untracked.PSet( SkipEvent = cms.untracked.vstring('ProductNotFound'))

#----------------------------------------------------------------------------------------------------------
if options.input=="":
	print("INPUT FILE MISSING!")

#dataFileName = "../bin/textfiles/Data/" + options.input
#BTagCSVRun2017B-UL2017_MiniAODv2-v1.txt
#with open(dataFileName, 'r') as file:
  #  datatxt = file.read()
#dataArray = datatxt.split()

import subprocess
getFileList = "../bin/getFileList"
shellCommand = f"{getFileList} {options.input}"
try: 
    datatxt = subprocess.run(shellCommand, shell=True, check=True, capture_output=True)
except subprocess.CalledProcessError as e:
    print(f"Error: {e}")
dataArray = datatxt.split()

#----------------------------------------------------------------------------------------------------------
import os

for rfile in dataArray:
	req = 'dasgoclient --query="run file="'+rfile
	os.system(req)

process.source = cms.Source("PoolSource", fileNames = cms.untracked.vstring(dataArray))

#'file:/afs/cern.ch/cms/Tutorials/TWIKI_DATA/TTJets_8TeV_53X.root'
#'/store/data/Run2016C/DoubleEG/MINIAOD/03Feb2017-v1/80000/EAEBC799-70EC-E611-A18A-02163E019D0D.root'
#'/store/relval/CMSSW_10_6_14/RelValZMM_13/MINIAODSIM/106X_mc2017_realistic_v7-v1/10000/0EB976F4-F84B-814D-88DA-CB2C29A52D72.root'
#'/store/data/Run2018A/DoubleMuon/MINIAOD/12Nov2019_UL2018-v2/100000/0004C5E1-8BFE-4044-82CB-C0FC878D9C54.root'
#'/store/mc/RunIISummer16NanoAODv7/HPlusPlusHMinusMinusHRTo4L_M-500_TuneCUETP8M1_13TeV-pythia8/NANOAODSIM/PUMoriond17_Nano02Apr2020_102X_mcRun2_asymptotic_v8-v1/270000/486D1D1F-9EAC-E247-8433-EFAFC0BCC5DE.root'
#'/store/data/Run2017F/DoubleEG/NANOAOD/UL2017_MiniAODv2_NanoAODv9-v1/70000/61B08DB4-292E-734A-B082-E56243B5365A.root'
#'file:data/Cert_314472-325175_13TeV_Legacy2018_Collisions18_JSON.txt'
 
                            
#Luminosity Filter
if options.luminosityFilter!="":
	process.source.lumisToProcess = LumiList.LumiList(filename = 'data/Cert_314472-325175_13TeV_Legacy2018_Collisions18_JSON.txt').getVLuminosityBlockRange()

process.demo = cms.EDAnalyzer('RunAnalyzerWrapper',
    analyzerType   = cms.untracked.string(options.analysis),
    rootOutFile = cms.untracked.string(options.output)
                              )


process.p = cms.Path(process.demo)


