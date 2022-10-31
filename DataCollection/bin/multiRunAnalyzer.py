from subprocess import run
from multiprocessing import Process

def loopRun(*fileList):
	path = "textfiles/"
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
	higgs_delphes = ("SingleMassSnowmass/Higgs/HiggsPick300.txt", "SingleMassSnowmass/Higgs/HiggsPick500.txt", "SingleMassSnowmass/Higgs/HiggsPick700.txt", "SingleMassSnowmass/Higgs/HiggsPick900.txt", "SingleMassSnowmass/Higgs/HiggsPick1100.txt", "SingleMassSnowmass/Higgs/HiggsPick1300.txt")
	higgs_run2 = ("newHiggs/Higgs300.txt", "newHiggs/Higgs500.txt", "newHiggs/Higgs700.txt", "newHiggs/Higgs900.txt", "newHiggs/Higgs1100.txt", "newHiggs/Higgs1300.txt")


	# List of jobs
	jobsList = [higgs_delphes, higgs_run2]
	# list of processes
	processes = []
	print(len(jobsList))
	for job in jobsList:
		newProcess = Process(target=loopRun, args=(job))
		processes.append(newProcess)

	# start jobs
	for process in processes:
		process.start()

	# Join jobs
	for process in processes:
		process.join()
