from subprocess import Popen
from multiprocessing import Process 
import sys
import os

analysis = 0

def loopRun(*fileList):

	if len(sys.argv) <= 3:
		path = "Higgs/" if analysis == 0 else "DarkPhoton_NoTriggerNoCut/" if analysis == 1 or analysis == 3 or analysis == 4 or analysis == 5 or analysis == 6 else "Muon/" if analysis == 2 else ""
	else:
		path = sys.argv[3]

	os.makedirs(os.environ['CMSSW_BASE'] + "/src/CMSAnalysis/Output/" + path, exist_ok=True)

	analysisBackground = "HiggsBackground" if analysis == 0 else "LeptonJetReconstruction" if analysis == 1 or analysis == 3 or analysis == 4 or analysis == 5 or analysis == 6 else "Muon" if analysis == 2 else ""
	#analysisBackground = "MLVariables"

	# get rid of numFiles for a full run-through
	numFiles = "numFiles=1"
	for file in fileList:
		# Filling in the parameters of runAnalyzer
		analysisSignal = "HiggsSignal" if analysis == 0 else "MuonSignal" if analysis == 2 else ""
		nameLocation = file.rfind("/") + 1
		nameEnd = len(file) - 4
		name = file[nameLocation:nameEnd]
		outputString = "output=" + path + name + ".root"

		inputString = "input=" + file

		if file[0:5] == "Higgs":
			analysisName = "analysis=" + analysisSignal
			inputString = "input=" + file
		else:
			analysisName = "analysis=" + analysisBackground
			inputString = "input=" + file
		
		# calls runAnalyzer
		print("Creating " + outputString)
		Popen(["nohup", "runAnalyzer", inputString, outputString, analysisName, numFiles]) 
	
	#runAnalyzer input="Data/Data_Trigger_SingleMuon_Year_2016B.txt" output="Data_Trigger_SingleMuon_Year_2016B.root" analysis="HiggsBackground"
if __name__ == '__main__':
	if len(sys.argv) <= 1:
		print("No analysis specified, defaulting to Higgs")
		analysis = 0
	else:
		analysisName = sys.argv[1]
		if analysisName == "Higgs":
			print("Running Higgs Analyses")
			analysis = 0
		elif analysisName == "DarkPhoton":
			print("Running Dark Photon Analyses")
			analysis = 1
		elif analysisName == "Muon":
			print("Running Muon Analyses")
			analysis = 2
		elif analysisName == "DPBackground":
			print("Running Background Analyses")
			analysis = 3
		elif analysisName == "DPAOD":
			print("Running Dark Photon NanoAOD Analyses")
			analysis = 4
		elif analysisName == "HiggsData":
			print("Running HiggsData Analyses")
			analysis = 5
		elif analysisName == "DPData":
			print("Running DPData Analysis")
			analysis = 6
		else:
			print("Argument did not match any analysis, defaulting to Higgs")
			analysis = 0

	# jobs grouped by process
	# If a job only has one pickfile in it, make sure to add a comma at the end so that python thinks it is a tuple
	
	ttBar = ("TTbar/TTbar_Boson_NA_Decay_LL_Run_2.txt", "TTbar/TTbar_Boson_W_Decay_L_Run_2.txt", "TTbar/TTbar_Boson_Z_Decay_LL_Run_2.txt")

	zz = ("ZZ/ZZ_Decay_2e2mu_Run_2.txt", "ZZ/ZZ_Decay_2e2tau_Run_2.txt", "ZZ/ZZ_Decay_2mu2tau_Run_2.txt", "ZZ/ZZ_Decay_4e_Run_2.txt", "ZZ/ZZ_Decay_4L_Run_2.txt", "ZZ/ZZ_Decay_4mu_Run_2.txt", "ZZ/ZZ_Decay_4tau_Run_2.txt")
	
	dy = ("Drell-Yan/Drell-Yan_MassCut_10-50_Run_2.txt", "Drell-Yan/Drell-Yan_MassCut_50-inf_Run_2.txt")
	#dy = ("Drell-Yan/Drell-Yan_Format_NanoAOD_MassCut_10-50_Period_2018_Run_2.txt", "Drell-Yan/Drell-Yan_Format_NanoAOD_MassCut_50-inf_Period_2018_Run_2.txt")

	multiBoson = ("MultiBoson/MultiBoson_Bosons_WW_Decay_2L_Run_2.txt", "MultiBoson/MultiBoson_Bosons_WWW_Decay_NA_Run_2.txt", "MultiBoson/MultiBoson_Bosons_WWZJets_Decay_4L_Run_2.txt",
	"MultiBoson/MultiBoson_Bosons_WZ_Decay_3L_Run_2.txt", "MultiBoson/MultiBoson_Bosons_WZZ_Decay_NA_Run_2.txt", "MultiBoson/MultiBoson_Bosons_ZZZ_Decay_NA_Run_2.txt")

	higgsSignal = ("Higgs500.txt","Higgs600.txt","Higgs700.txt","Higgs800.txt","Higgs900.txt","Higgs1000Run2.txt", 
		#"Higgs1000Run3.txt",
		"Higgs1100.txt","Higgs1200.txt","Higgs1300.txt","Higgs1400.txt","Higgs1500.txt" )

	#higgsData = ("Data/SingleMuonRun2017B-UL2017_MiniAODv2-v1.txt", "Data/SingleElectronRun2017B-UL2017_MiniAODv2-v1.txt")
	higgsData = (
		"Data/Data_Trigger_SingleMuon_Year_2016B.txt", 
		"Data/Data_Trigger_SingleMuon_Year_2016C.txt",
		"Data/Data_Trigger_SingleMuon_Year_2016D.txt", 
		"Data/Data_Trigger_SingleMuon_Year_2016E.txt", 
		"Data/Data_Trigger_SingleMuon_Year_2016F.txt",
		"Data/Data_Trigger_SingleMuon_Year_2016G.txt",
		"Data/Data_Trigger_SingleMuon_Year_2016H.txt",
		"Data/Data_Trigger_SingleMuon_Year_2017B.txt",
		"Data/Data_Trigger_SingleMuon_Year_2017C.txt",
		"Data/Data_Trigger_SingleMuon_Year_2017D.txt",
		"Data/Data_Trigger_SingleMuon_Year_2017E.txt",
		"Data/Data_Trigger_SingleMuon_Year_2017F.txt",
		"Data/Data_Trigger_SingleMuon_Year_2017G.txt",
		"Data/Data_Trigger_SingleMuon_Year_2017H.txt",
		"Data/Data_Trigger_SingleMuon_Year_2018A.txt",
		"Data/Data_Trigger_SingleMuon_Year_2018B.txt",
		"Data/Data_Trigger_SingleMuon_Year_2018C.txt",
		"Data/Data_Trigger_SingleMuon_Year_2018D.txt",
		#"Data/Data_Trigger_SingleMuon_Year_2022A.txt",
		#"Data/Data_Trigger_SingleMuon_Year_2022B.txt",
		#"Data/Data_Trigger_SingleMuon_Year_2022C.txt",
		"Data/Data_Trigger_SingleElectron_Year_2016B.txt",
		"Data/Data_Trigger_SingleElectron_Year_2016C.txt",
		"Data/Data_Trigger_SingleElectron_Year_2016D.txt",
		"Data/Data_Trigger_SingleElectron_Year_2016E.txt",
		"Data/Data_Trigger_SingleElectron_Year_2016F.txt",
		"Data/Data_Trigger_SingleElectron_Year_2016G.txt",
		"Data/Data_Trigger_SingleElectron_Year_2016H.txt",
		"Data/Data_Trigger_SingleElectron_Year_2017B.txt",
		"Data/Data_Trigger_SingleElectron_Year_2017C.txt",
		"Data/Data_Trigger_SingleElectron_Year_2017D.txt",
		"Data/Data_Trigger_SingleElectron_Year_2017E.txt",
		"Data/Data_Trigger_SingleElectron_Year_2017F.txt",
		"Data/Data_Trigger_SingleElectron_Year_2018A.txt",
		"Data/Data_Trigger_SingleElectron_Year_2018B.txt",
		"Data/Data_Trigger_SingleElectron_Year_2018C.txt",
		"Data/Data_Trigger_SingleElectron_Year_2018D.txt",
		)
	
	dpData = (
		"Data/Data_Trigger_SingleMuon_Year_2018A.txt",
	)
	
	qcd = (
		"QCD/QCD_HTCut_100-200_Run_2_Year_2018.txt", 
		"QCD/QCD_HTCut_200-300_Run_2_Year_2018.txt", 
		"QCD/QCD_HTCut_300-500_Run_2_Year_2018.txt", 
		"QCD/QCD_HTCut_500-700_Run_2_Year_2018.txt", 
		"QCD/QCD_HTCut_700-1000_Run_2_Year_2018.txt", 
		"QCD/QCD_HTCut_1000-1500_Run_2_Year_2018.txt", 
		"QCD/QCD_HTCut_1500-2000_Run_2_Year_2018.txt", 
		"QCD/QCD_HTCut_2000-inf_Run_2_Year_2018.txt"
	)

	darkPhotonSignal = ("darkPhotonBaselineRun2.txt", )

	darkPhotonNanoAOD = (
		"DarkPhoton/DarkPhoton_Decay_Higgs2DP_DpMass_0_1_FSR_0_0_Format_NanoAOD_HiggsMass_1000_Period_2018_Run_2.txt",
		"DarkPhoton/DarkPhoton_Decay_Higgs2DP_DpMass_0_2_FSR_0_0_Format_NanoAOD_HiggsMass_1000_Period_2018_Run_2.txt",
		"DarkPhoton/DarkPhoton_Decay_Higgs2DP_DpMass_0_3_FSR_0_0_Format_NanoAOD_HiggsMass_1000_Period_2018_Run_2.txt",
		"DarkPhoton/DarkPhoton_Decay_Higgs2DP_DpMass_0_3_FSR_0_0_Format_NanoAOD_HiggsMass_125_Period_2018_Run_2.txt",
		"DarkPhoton/DarkPhoton_Decay_Higgs2DP_DpMass_0_3_FSR_0_0_Format_NanoAOD_HiggsMass_300_Period_2018_Run_2.txt",
		"DarkPhoton/DarkPhoton_Decay_Higgs2DP_DpMass_0_3_FSR_0_1_Format_NanoAOD_HiggsMass_1000_Period_2018_Run_2.txt",
		"DarkPhoton/DarkPhoton_Decay_Higgs2DP_DpMass_0_3_FSR_0_3_Format_NanoAOD_HiggsMass_1000_Period_2018_Run_2.txt",
		"DarkPhoton/DarkPhoton_Decay_Higgs2DP_DpMass_0_4_FSR_0_0_Format_NanoAOD_HiggsMass_1000_Period_2018_Run_2.txt",
		"DarkPhoton/DarkPhoton_Decay_Higgs2DP_DpMass_0_6_FSR_0_0_Format_NanoAOD_HiggsMass_1000_Period_2018_Run_2.txt",
		"DarkPhoton/DarkPhoton_Decay_Higgs2DP_DpMass_0_9_FSR_0_0_Format_NanoAOD_HiggsMass_1000_Period_2018_Run_2.txt",
		"DarkPhoton/DarkPhoton_Decay_Higgs2DP_DpMass_1_2_FSR_0_0_Format_NanoAOD_HiggsMass_1000_Period_2018_Run_2.txt",
		"DarkPhoton/DarkPhoton_Decay_Higgs2DP_DpMass_1_5_FSR_0_0_Format_NanoAOD_HiggsMass_1000_Period_2018_Run_2.txt",
		"DarkPhoton/DarkPhoton_Decay_Higgs2DP_DpMass_2_5_FSR_0_0_Format_NanoAOD_HiggsMass_1000_Period_2018_Run_2.txt",
		"DarkPhoton/DarkPhoton_Decay_Higgs2DP_DpMass_4_0_FSR_0_0_Format_NanoAOD_HiggsMass_1000_Period_2018_Run_2.txt",
		"DarkPhoton/DarkPhoton_Decay_Higgs4DP_DpMass_0_3_FSR_0_0_Format_NanoAOD_HiggsMass_1000_Period_2018_Run_2.txt",
		"DarkPhoton/DarkPhoton_Decay_HiggsDPZ_DpMass_0_3_FSR_0_0_Format_NanoAOD_HiggsMass_1000_Period_2018_Run_2.txt",
		"DarkPhoton/DarkPhoton_Decay_SUSY_DpMass_0_3_FSR_0_0_Format_NanoAOD_HiggsMass_1000_Period_2018_Run_2.txt",
		"DarkPhoton/DarkPhoton_Decay_ZPrime_DpMass_0_3_FSR_0_0_Format_NanoAOD_HiggsMass_1000_Period_2018_Run_2.txt",
	)

	#background = ttBar + zz + dy + multiBoson + qcd # total 26 files
	background = ttBar + zz + multiBoson + qcd # total 24 files - multiMuPtAnalysis coniguration
	#background = dy # just dy configuration

	# List of jobs to run on from those above
	# jobsList = [ttBar, zz, dy50, multiBoson, higgsSignal, higgsData] if analysis == 0 or analysis == 2 else [darkPhotonSignal]
	
	#jobsList = [higgsSignal] if analysis == 0 or analysis == 2 else [darkPhotonSignal]
	jobsList = [ttBar, zz, dy50, multiBoson, higgsSignal, higgsData, qcd]
	#jobsList = [qcd]
	#jobsList = [higgsSignal] if analysis == 0 or analysis == 2 else [darkPhotonSignal] if analysis == 1 else [background] if analysis == 3 else [darkPhotonNanoAOD] if analysis == 4 else [dpData] if analysis == 6 else [higgsData]
	
	if os.path.exists("nohup.out") and (len(sys.argv) <= 2 or sys.argv[2] != "keep"):
		os.remove("nohup.out")

	# list of processes
	processes = []
	for job in jobsList:
		newProcess = Process(target=loopRun, args=(job))
		processes.append(newProcess)

	# start jobs
	for process in processes:
		process.start()

	# Join jobs
	for process in processes:
		process.join()
