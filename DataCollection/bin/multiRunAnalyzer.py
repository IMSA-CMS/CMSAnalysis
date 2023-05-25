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
		run(["runAnalyzer", inputString, outputString, analysisName], check=True)

if __name__ == '__main__':
	# jobs grouped by process
	# If a job only has one pickfile in it, make sure to add a comma at the end so that python thinks it is a tuple
	
	dy50 = ("DY50Run2.txt", )

	qcd500 = ("Run2QCD/QCD500.txt", )

	zz = ("SingleMassSnowmass/ZZ/ZZPick4.txt", )

	ttBar = ("SingleMassSnowmass/TTBar/TTBarPick500.txt", )
	# List of jobs to run on from those above
	jobsList = [dy50, qcd500, zz, ttBar]
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
