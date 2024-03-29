from subprocess import run
from multiprocessing import Process 

def loopRun(*fileList):
	path = ""
	for file in fileList:
		# Filling in the parameters of runAnalyzer
		analysisBackground = "LeptonJetReconstruction"
		nameLocation = file.rfind("/") + 1
		print (nameLocation)
		nameEnd = len(file) - 4
		print(nameEnd)
		name = file[nameLocation:nameEnd]
		print(name)
		#process = file[0:file.rfind("/")]
		outputString = "output=" + name + ".root"

		analysisName = "analysis=" + analysisBackground
		inputString = "input=" + path + file
		numFiles = "numFiles=1"
		
		# calls runAnalyzer
		print("Creating " + outputString)
		#run(["nohup", "runAnalyzer", inputString, outputString, analysisName, numFiles], check=True)
		run(["nohup", "runAnalyzer", inputString, outputString, analysisName, "numFiles=3"], check=True)

if __name__ == '__main__':
	# jobs grouped by process
	# If a job only has one pickfile in it, make sure to add a comma at the end so that python thinks it is a tuple
	
	ttBar = ("TTbar/TTbar_Boson_NA_Decay_LL_Run_2.txt", "TTbar/TTbar_Boson_W_Decay_L_Run_2.txt", "TTbar/TTbar_Boson_Z_Decay_LL_Run_2.txt")

	
	zz = ("ZZ/ZZ_Decay_2e2mu_Run_2.txt", "ZZ/ZZ_Decay_2e2tau_Run_2.txt", "ZZ/ZZ_Decay_2mu2tau_Run_2.txt", "ZZ/ZZ_Decay_4e_Run_2.txt", "ZZ/ZZ_Decay_4L_Run_2.txt", "ZZ/ZZ_Decay_4mu_Run_2.txt", "ZZ/ZZ_Decay_4tau_Run_2.txt")
	
	dy50 = ("Drell-Yan/Drell-Yan_MassCut_10-50_Run_2.txt", "Drell-Yan/Drell-Yan_MassCut_50-inf_Run_2.txt")

	multiBoson = ("MultiBoson/MultiBoson_Bosons_WW_Decay_2L_Run_2.txt", "MultiBoson/MultiBoson_Bosons_WWW_Decay_NA_Run_2.txt", "MultiBoson/MultiBoson_Bosons_WWZJets_Decay_4L_Run_2.txt",
	"MultiBoson/MultiBoson_Bosons_WZ_Decay_3L_Run_2.txt", "MultiBoson/MultiBoson_Bosons_WZZ_Decay_NA_Run_2.txt", "MultiBoson/MultiBoson_Bosons_ZZZ_Decay_NA_Run_2.txt")

	#multiBoson = ("MultiBoson/MultiBoson_Bosons_WWZJets_Decay_4L_Run_2.txt","MultiBoson/MultiBoson_Bosons_WZ_Decay_3L_Run_2.txt", "MultiBoson/MultiBoson_Bosons_WZZ_Decay_NA_Run_2.txt", "MultiBoson/MultiBoson_Bosons_ZZZ_Decay_NA_Run_2.txt")

	darkPhotonSignal = ("darkPhotonBaselineRun2.txt", )

	#higgsData = ("Data/SingleMuonRun2017B-UL2017_MiniAODv2-v1.txt", "Data/SingleElectronRun2017B-UL2017_MiniAODv2-v1.txt")
	Data = ("Data/Data_Trigger_SingleMuon_Year_2018A.txt", )

	# List of jobs to run on from those above
	jobsList = [multiBoson]
	#jobsList = [zz]

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
