import sys
import subprocess

def loopRun(*fileList, analysis):
	path = "Higgs/" if analysis == 0 else "DarkPhoton/" if analysis == 1 else "Muon/" if analysis == 2 else ""
	analysisBackground = "HiggsBackground" if analysis == 0 else "LeptonJetReconstruction" if analysis == 1 else "Muon" if analysis == 2 else ""
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
		#Popen(["runAnalyzer", inputString, outputString, analysisName, numFiles]) 

if __name__ == '__main__':
    # Collect job files from command-line arguments

	analysis = int(sys.argv[1])  # Convert the first argument to an integer
    jobsList = sys.argv[2:]  # Remaining arguments are job files

    #jobsList = sys.argv[1:]  # Job files are passed as arguments

    # Process each job sequentially
    loopRun(jobsList, analysis)