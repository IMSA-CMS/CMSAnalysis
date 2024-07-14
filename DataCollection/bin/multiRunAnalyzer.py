from subprocess import run, Popen
from multiprocessing import Process 
import sys

def loopRun(*fileList):

	for file in fileList:
		# Filling in the parameters of runAnalyzer
		analysisBackground = "HiggsBackground"
		analysisSignal = "HiggsSignal" 
		nameLocation = file.rfind("/") + 1
		nameEnd = len(file) - 4
		name = file[nameLocation:nameEnd]
		outputString = "output=" + name + ".root"
				
		if file[0:5] == "Higgs":	
			analysisName = "analysis=" + analysisSignal
			inputString = "input=" + file
		else:
			analysisName = "analysis=" + analysisBackground
			inputString = "input=" + file	
		numFiles = "numFiles=1"
		
		# calls runAnalyzer
		print("Creating " + outputString)
		Popen(["nohup", "runAnalyzer", inputString, outputString, analysisName, numFiles]) #testing 
		# run(["nohup", "runAnalyzer", inputString, outputString, analysisName], check=True)
	
	#runAnalyzer input="Data/Data_Trigger_SingleMuon_Year_2016B.txt" output="Data_Trigger_SingleMuon_Year_2016B.root" analysis="HiggsBackground"
if __name__ == '__main__':
	analysis = 0
	try:
		name = str(sys.argv[1])
		if(name != 'Higgs'):
			analysis = 1
	except:
		print("No analysis was specified, defaulting to Higgs.")
	# jobs grouped by process
	# If a job only has one pickfile in it, make sure to add a comma at the end so that python thinks it is a tuple
	
	ttBar = ("TTbar/TTbar_Boson_NA_Decay_LL_Run_2.txt", "TTbar/TTbar_Boson_W_Decay_L_Run_2.txt", "TTbar/TTbar_Boson_Z_Decay_LL_Run_2.txt")

	zz = ("ZZ/ZZ_Decay_2e2mu_Run_2.txt", "ZZ/ZZ_Decay_2e2tau_Run_2.txt", "ZZ/ZZ_Decay_2mu2tau_Run_2.txt", "ZZ/ZZ_Decay_4e_Run_2.txt", "ZZ/ZZ_Decay_4L_Run_2.txt", "ZZ/ZZ_Decay_4mu_Run_2.txt", "ZZ/ZZ_Decay_4tau_Run_2.txt")
	
	dy50 = ("Drell-Yan/Drell-Yan_MassCut_10-50_Run_2.txt", "Drell-Yan/Drell-Yan_MassCut_50-inf_Run_2.txt")

	multiBoson = ("MultiBoson/MultiBoson_Bosons_WW_Decay_2L_Run_2.txt", "MultiBoson/MultiBoson_Bosons_WWW_Decay_NA_Run_2.txt", "MultiBoson/MultiBoson_Bosons_WWZJets_Decay_4L_Run_2.txt",
	"MultiBoson/MultiBoson_Bosons_WZ_Decay_3L_Run_2.txt", "MultiBoson/MultiBoson_Bosons_WZZ_Decay_NA_Run_2.txt", "MultiBoson/MultiBoson_Bosons_ZZZ_Decay_NA_Run_2.txt")

	higgsSignal = ("Higgs1400.txt", )
	# higgsSignal = ("Higgs500.txt","Higgs600.txt","Higgs700.txt","Higgs800.txt","Higgs900.txt","Higgs1000.txt","Higgs1100.txt","Higgs1200.txt","Higgs1300.txt","Higgs1400.txt","Higgs1500.txt" )
	qcd = ("QCD/QCD_HTCut_100-200_Run_2_Year_2018.txt", "QCD/QCD_HTCut_200-300_Run_2_Year_2018.txt", "QCD/QCD_HTCut_300-500_Run_2_Year_2018.txt", "QCD/QCD_HTCut_500-700_Run_2_Year_2018.txt", "QCD/QCD_HTCut_700-1000_Run_2_Year_2018.txt", "QCD/QCD_HTCut_1000-1500_Run_2_Year_2018.txt", "QCD/QCD_HTCut_1500-2000_Run_2_Year_2018.txt", "QCD/QCD_HTCut_2000-Inf_Run_2_Year_2018.txt")

	#higgsData = ("Data/SingleMuonRun2017B-UL2017_MiniAODv2-v1.txt", "Data/SingleElectronRun2017B-UL2017_MiniAODv2-v1.txt")
	higgsData = (
		"Data/Data_Trigger_SingleMuon_Year_2016B.txt", 
		# "Data/Data_Trigger_SingleMuon_Year_2016C.txt",
		# "Data/Data_Trigger_SingleMuon_Year_2016D.txt", 
		# "Data/Data_Trigger_SingleMuon_Year_2016E.txt", 
		# "Data/Data_Trigger_SingleMuon_Year_2016F.txt",
		# "Data/Data_Trigger_SingleMuon_Year_2016G.txt",
		# "Data/Data_Trigger_SingleMuon_Year_2016H.txt",
		# "Data/Data_Trigger_SingleMuon_Year_2017B.txt",
		# "Data/Data_Trigger_SingleMuon_Year_2017C.txt",
		# "Data/Data_Trigger_SingleMuon_Year_2017D.txt",
		# "Data/Data_Trigger_SingleMuon_Year_2017E.txt",
		# "Data/Data_Trigger_SingleMuon_Year_2017F.txt",
		# "Data/Data_Trigger_SingleMuon_Year_2017G.txt",
		# "Data/Data_Trigger_SingleMuon_Year_2017H.txt",
		# "Data/Data_Trigger_SingleMuon_Year_2018A.txt",
		# "Data/Data_Trigger_SingleMuon_Year_2018B.txt",
		# "Data/Data_Trigger_SingleMuon_Year_2018C.txt",
		# "Data/Data_Trigger_SingleMuon_Year_2018D.txt",
		# "Data/Data_Trigger_SingleMuon_Year_2022A.txt",
		# "Data/Data_Trigger_SingleMuon_Year_2022B.txt",
		# "Data/Data_Trigger_SingleMuon_Year_2022C.txt",
		# "Data/Data_Trigger_SingleElectron_Year_2016B.txt",
		# "Data/Data_Trigger_SingleElectron_Year_2016C.txt",
		# "Data/Data_Trigger_SingleElectron_Year_2016D.txt",
		# "Data/Data_Trigger_SingleElectron_Year_2016E.txt",
		# "Data/Data_Trigger_SingleElectron_Year_2016F.txt",
		# "Data/Data_Trigger_SingleElectron_Year_2016G.txt",
		# "Data/Data_Trigger_SingleElectron_Year_2016H.txt",
		# "Data/Data_Trigger_SingleElectron_Year_2017B.txt",
		# "Data/Data_Trigger_SingleElectron_Year_2017C.txt",
		# "Data/Data_Trigger_SingleElectron_Year_2017D.txt",
		# "Data/Data_Trigger_SingleElectron_Year_2017E.txt",
		# "Data/Data_Trigger_SingleElectron_Year_2017F.txt"
		)

	# List of jobs to run on from those above
	# jobsList = [ttBar, zz, dy50, multiBoson, higgsSignal, higgsData, qcd]
	jobsList = [higgsSignal]

	# list of processes
	processes = []
	# for job in jobsList:
	# 	newProcess = Process(target=loopRun, args=(job))
	# 	processes.append(newProcess)

	# start jobs
	# for process in processes:
	# 	process.start()

	# Join jobs
	# for process in processes:
	# 	process.join()
