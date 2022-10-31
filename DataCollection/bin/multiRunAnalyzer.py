from subprocess import run
from multiprocessing import Process

def loopRun(*fileList):
	path = "textfiles/SingleMassSnowmass/"
	for file in fileList:
		# Filling in the parameters of runAnalyzer
		analysis = "HiggsBackground"
		inputString = "input=" + path + file
		nameLocation = file.rfind("/") + 1
		nameEnd = len(file) - 4
		name = file[nameLocation:nameEnd]
		outputString = "output=" + name + "_" + analysis + ".root"
		
		analysisName = "analysis=" + analysis
		# calls runAnalyzer
		print("Creating " + outputString)
		run(["nohup", "runAnalyzer", inputString, outputString, analysisName], check=True)

if __name__ == '__main__':
	# jobs grouped by process
	# If a job only has one pickfile in it, make sure to add a comma at the end so that python thinks it is a tuple
	higgs_ttbar_zz = ("Higgs/HiggsPick300.txt", "Higgs/HiggsPick500.txt", "Higgs/HiggsPick700.txt", "Higgs/HiggsPick900.txt", "Higgs/HiggsPick1100.txt", "Higgs/HiggsPick1300.txt", "TTBar/TTBarPick500.txt", "ZZ/ZZPick4.txt")

	darkPhoton = ("DarkPhoton/PhotonPick0_5.txt", "DarkPhoton/PhotonPick0_9.txt", "DarkPhoton/PhotonPick2_0.txt")
	
	dy4 = ("DY/DYPick4.txt", )
	dy10 = ("DY/DYPick10.txt", )
	dy50 = ("DY/DYPick50.txt", )

	qcd50_100 = ("QCD/QCDPick50.txt", "QCD/QCDPick100.txt")
	qcd200_500 = ("QCD/QCDPick200.txt", "QCD/QCDPick500.txt")
	qcd700_1000 = ("QCD/QCDPick700.txt", "QCD/QCDPick1000.txt")
	qcd1500_2000 = ("QCD/QCDPick1500.txt", "QCD/QCDPick2000.txt")


	# List of jobs to run on from those above
	jobsList = [higgs_ttbar_zz, dy4, dy10, dy50, qcd50_100, qcd200_500, qcd700_1000, qcd1500_2000]
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
