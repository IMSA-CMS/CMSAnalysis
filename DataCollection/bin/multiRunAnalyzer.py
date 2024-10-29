from subprocess import Popen
from multiprocessing import Process
import sys
import os
import subprocess
import argparse

# def run_batch(batch, analysis):
#     # Construct the command to invoke run_jobs.py with a batch of jobs
#     cmd = ["python3", "batch_run.py", str(analysis)] + batch
#     print(f"Running batch: {batch}")
#     # Start the batch in a new process using subprocess
#     subprocess.Popen(cmd, stdout=subprocess.PIPE, stderr=subprocess.PIPE)

def loopRun(crab, path, fileCount, *fileList):

	if not path:
		path = "Higgs/" if analysis == 0 else "DarkPhoton_MLStrip_CompleteCuts_ValidationConfig3_Output_numFiles3/" if analysis == 1 or analysis == 3 or analysis == 4 or analysis == 5 or analysis == 6 or analysis == 7 else "Muon/" if analysis == 2 else ""

	os.makedirs(os.environ['CMSSW_BASE'] + "/src/CMSAnalysis/Output/" + path, exist_ok=True)

	analysisBackground = "HiggsBackground" if analysis == 0 else "LeptonJetReconstruction" if analysis == 1 or analysis == 3 or analysis == 4 or analysis == 5 or analysis == 6 or analysis == 7 else "Muon" if analysis == 2 else ""
	#analysisBackground = "MLVariables"

	# get rid of numFiles for a full run-through
	numFiles = "numFiles=" + fileCount if fileCount != None else None
	for file in fileList[0]:
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
		if crab:
			crab_directory = os.environ['CMSSW_BASE'] + "/src/CMSAnalysis/CRAB/"
			prefix = "--"
			print(file)
			generate = Popen(["python3", "crab_config_generator.py", prefix + inputString, f'--output={name}.root', prefix + analysisName, f'--folder={path[0:-1]}' if nameLocation != 0 else "", "--" + numFiles if numFiles != None else ""], cwd=crab_directory)
			generate.wait()
			submit = Popen(['crab', 'submit', '-c', 'crab_config.py'], cwd=crab_directory)
			submit.wait()
		else:
			Popen(["runAnalyzer", inputString, outputString, analysisName, numFiles])
	
	#runAnalyzer input="Data/Data_Trigger_SingleMuon_Year_2016B.txt" output="Data_Trigger_SingleMuon_Year_2016B.root" analysis="HiggsBackground"
if __name__ == '__main__':
	parser = argparse.ArgumentParser()
	parser.add_argument("--crab", help="Turn on CRAB Processing Mode", action='store_true')
	parser.add_argument("--analysis", help="Determines which analysis to use (Higgs, DarkPhoton, etc.)")
	parser.add_argument("--keep", help="If true, appends to the current nohup.out file instead of clearing", action='store_true')
	parser.add_argument("--path", help="Custom Output Path like Higgs (no backslash)")
	parser.add_argument("--numFiles", help="Number of files to run over")
	
	args = parser.parse_args()
	print(args)

	if not args.analysis:
		print("No analysis specified, defaulting to Higgs")
		analysis = 0
	else:
		analysisName = args.analysis
		if analysisName == "Higgs":
			print("Running Higgs Analyses")
			analysis = 0
		elif analysisName == "DarkPhoton":
			print("Running Dark Photon Analyses")
			analysis = 1
		elif analysisName == "Muon":
			print("Running Muon Analyses")
			analysis = 2
		else:
			print("Argument did not match any analysis, defaulting to Higgs")
			analysis = 0

	# jobs grouped by process
	# If a job only has one pickfile in it, make sure to add a comma at the end so that python thinks it is a tuple
	
	ttBar = ("TTbar/TTbar_Boson_NA_Decay_LL_Run_2.txt", "TTbar/TTbar_Boson_W_Decay_L_Run_2.txt", "TTbar/TTbar_Boson_Z_Decay_LL_Run_2.txt")

	zz = ("ZZ/ZZ_Decay_2e2mu_Run_2.txt", "ZZ/ZZ_Decay_2e2tau_Run_2.txt", "ZZ/ZZ_Decay_2mu2tau_Run_2.txt", "ZZ/ZZ_Decay_4e_Run_2.txt", "ZZ/ZZ_Decay_4L_Run_2.txt", "ZZ/ZZ_Decay_4mu_Run_2.txt", "ZZ/ZZ_Decay_4tau_Run_2.txt")
	
	dy50 = ("Drell-Yan/Drell-Yan_MassCut_10-50_Run_2.txt", "Drell-Yan/Drell-Yan_MassCut_50-inf_Run_2.txt")

	multiBoson = ("MultiBoson/MultiBoson_Bosons_WW_Decay_2L_Run_2.txt", "MultiBoson/MultiBoson_Bosons_WWW_Decay_NA_Run_2.txt", "MultiBoson/MultiBoson_Bosons_WWZJets_Decay_4L_Run_2.txt",
	"MultiBoson/MultiBoson_Bosons_WZ_Decay_3L_Run_2.txt", "MultiBoson/MultiBoson_Bosons_WZZ_Decay_NA_Run_2.txt", "MultiBoson/MultiBoson_Bosons_ZZZ_Decay_NA_Run_2.txt")

	higgsSignal = ("Higgs500.txt","Higgs600.txt","Higgs700.txt","Higgs800.txt","Higgs900.txt","Higgs1000Run2.txt", "Higgs1000Run3.txt","Higgs1100.txt","Higgs1200.txt","Higgs1300.txt","Higgs1400.txt","Higgs1500.txt" )

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
		# "Data/Data_Trigger_SingleMuon_Year_2017G.txt",
		# "Data/Data_Trigger_SingleMuon_Year_2017H.txt",
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
	
	qcd = ("QCD/QCD_HTCut_100-200_Run_2_Year_2018.txt", "QCD/QCD_HTCut_200-300_Run_2_Year_2018.txt", "QCD/QCD_HTCut_300-500_Run_2_Year_2018.txt", "QCD/QCD_HTCut_500-700_Run_2_Year_2018.txt", "QCD/QCD_HTCut_700-1000_Run_2_Year_2018.txt", "QCD/QCD_HTCut_1000-1500_Run_2_Year_2018.txt", "QCD/QCD_HTCut_1500-2000_Run_2_Year_2018.txt", "QCD/QCD_HTCut_2000-Inf_Run_2_Year_2018.txt")
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

	###########this one ######### background = qcd
	#background = qcd
	background = ttBar + zz + dy50 + multiBoson + qcd

	#background = ttBar + zz + multiBoson + qcd # total 24 files - multiMuPtAnalysis coniguration
	#background = ttBar + zz + multiBoson + bQCD
	#background = dy # just dy configuration
	#background = bQCD

	# List of jobs to run on from those above
	# jobsList = [ttBar, zz, dy50, multiBoson, higgsSignal, higgsData] if analysis == 0 or analysis == 2 else [darkPhotonSignal]
	
	#jobsList = [higgsSignal] if analysis == 0 or analysis == 2 else [darkPhotonSignal]
	#jobsList = [ttBar, zz, dy50, multiBoson, higgsSignal, higgsData, qcd]
	jobsList = [qcd]
	
	if os.path.exists("nohup.out") and not args.keep:
		os.remove("nohup.out")

	# batch_size = 3
	# for i in range(0, len(jobsList), batch_size):
	# 	print("In")
	# 	# Create a batch from the list
	# 	batch = jobsList[i:i + batch_size]
	# 	flat_batch = [file for job in batch for file in job]

	# 	# Specify the path to your Python script (batch_run.py)
	# 	script_path = "batch_run.py"
    
  #   	# Specify the output file where stdout and stderr will be logged
	# 	output_file = "nohup.out"
    
  #   	# Open the output file to log stdout and stderr
	# 	with open(output_file, "w") as f:
  #      	# Run the command with 'nohup' effect and redirect output to the file
	# 		print("1")
	# 		process = subprocess.Popen(["python", script_path], stdout=f, stderr=subprocess.STDOUT, preexec_fn=os.setpgrp)
	# 		process.wait()
    	
	# 	# Call run_jobs.py with the batch
	# 	#run_batch(flat_batch, analysis)


	# list of processes
	processes = []
	for job in jobsList:
		newProcess = Process(target=loopRun, args=(args.crab, args.path, args.numFiles, job))
		processes.append(newProcess)

	# start jobs
	for process in processes:
		process.start()

	# Join jobs
	for process in processes:
		process.join()
