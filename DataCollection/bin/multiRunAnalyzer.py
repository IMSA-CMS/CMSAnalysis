from subprocess import Popen
from multiprocessing import Process
import sys
import os
import subprocess
import argparse

reprocessMap = {
    "WWW": [2],
}

def loopRun(crab, path, fileCount, fileList):
def loopRun(crab, path, fileCount, skipFiles, fileList, outputFile):
    fileList = [f"Run2PickFiles/{file}" for file in fileList]
    if not path:
        path = (
            "Higgs/"
            if analysis == 0
            else (
                "DarkPhoton_MLStrip_CompleteCuts_ValidationConfig3_Output_numFiles3/"
                if analysis == 1
                or analysis == 3
                or analysis == 4
                or analysis == 5
                or analysis == 6
                or analysis == 7
                else "Muon/" if analysis == 2 else ""
            )
        )

    os.makedirs(
        os.environ["CMSSW_BASE"] + "/src/CMSAnalysis/Output/" + path, exist_ok=True
    )

    analysisBackground = (
        "HiggsBackground"
        if analysis == 0
        else (
            "LeptonJetReconstruction"
            if analysis == 1
            or analysis == 3
            or analysis == 4
            or analysis == 5
            or analysis == 6
            or analysis == 7
            else "Muon" if analysis == 2 
            else "MLVariables" if analysis == 8
            else ""
        )
    )
    # analysisBackground = "MLVariables"

    print("File list: ", fileList)
    # get rid of numFiles for a full run-through
    numFiles = "numFiles=" + fileCount if fileCount != None else ""
    
    skipFiles = "skipFiles=" + skipFiles if skipFiles != None else ""
    
    for file in fileList:
        # Filling in the parameters of runAnalyzer
        print("File: " + file)
        analysisSignal = (
            "HiggsSignal" if analysis == 0 else "MuonSignal" if analysis == 2 else ""
        )
        nameLocation = file.rfind("/") + 1
        nameEnd = len(file) - 4
        name = file[nameLocation:nameEnd]
        outputString = "output=" + path + name + ".root"
        inputString = "input=" + file
        offset = len("Run2PickFiles/")
        if file[0+offset:5+offset] == "Higgs":
            analysisName = "analysis=" + analysisSignal
            inputString = "input=" + file
        else:
            analysisName = "analysis=" + analysisBackground
            inputString = "input=" + file
            
        # calls runAnalyzer
        if crab:
            crab_directory = os.environ["CMSSW_BASE"] + "/src/CMSAnalysis/CRAB/"
            print(file)
            totalFiles = int(subprocess.check_output(["getFileList", file, "count"]))
            # 20 works for most jobs, TTbar and DY50-inf should use 5
            # theoretically could all the way down to 1,
            # but it might take longer to submit than just nohup
            maxNumFiles = 5
            totalFiles = min(int(fileCount), totalFiles) if fileCount != None else totalFiles
            
            # uncomment this for selective reproccessing
            # if name not in reprocessMap:
            #     continue
            # for j in reprocessMap[name]:
            #     i = j * maxNumFiles

            for i in range(
                0,
                totalFiles,
                maxNumFiles,
            ):
                output = f"{name}_{int(i / maxNumFiles)}.root"
                print("Creating " + output)
                generate = Popen(
                    [
                        "python3",
                        "crab_config_generator.py",
                        f"--{inputString}",
                        f"--output={output}",
                        f"--{analysisName}",
                        f"--folder={path + file[14:-4]}",
                        f"--numFiles={min(maxNumFiles, totalFiles - i)}",
                        f"--skipFiles={i}",
                    ],
                    cwd=crab_directory
                )
                generate.wait()
                submit = Popen(
                    ["crab", "submit", "-c", f"gen/{output[:-5]}_crab_config.py"], cwd=crab_directory
                )
                # submit.wait()
        else:
            with open(outputFile, "a") as out:
                print("Creating " + outputString)
                Popen(
                    [
                        "nohup",
                        "runAnalyzer",
                        inputString,
                        outputString,
                        analysisName,
                        numFiles,
                        skipFiles
                    ],
                    stdout=out, stderr=out
                )

if __name__ == "__main__":
    parser = argparse.ArgumentParser()
    parser.add_argument(
        "--crab", help="Turn on CRAB Processing Mode", action="store_true"
    )
    parser.add_argument(
        "--analysis", help="Determines which analysis to use (Higgs, DarkPhoton, etc.)"
    )
    parser.add_argument(
        "--keep",
        help="If true, appends to the current output file (default=nohup.out) instead of clearing",
        action="store_true",
    )
    parser.add_argument("--path", help="Custom Output Path like Higgs/")
    parser.add_argument("--numFiles", help="Number of files to run over")
    parser.add_argument("--skipFiles", help="Number of files to skip over")
    parser.add_argument("--outputFile", help="Specify an output log file", default="nohup.out")

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
        elif analysisName == "MLStrip":
            print("Running Strip Analysis")
            analysis = 8
        else:
            print("CLI argument did not match any analysis, defaulting to Higgs")
            analysis = 0

    root_directory = os.environ["CMSSW_BASE"] + "/src/CMSAnalysis/"
    if args.crab:
        copyInput = Popen(
            [
                "rsync",
                "-am",
                "--include='*.txt'",
                "--include='*.json'",
                "--include='*/'",
                "--exclude='*'",
                "--mkpath",
                "DataCollection/bin/textfiles",
                "CRAB/input/src/CMSAnalysis/DataCollection/bin",
            ],
            cwd=root_directory,
        )
        copyInput.wait()
    
    # jobs grouped by process
    # If a job only has one pickfile in it, make sure to add a comma at the end so that python thinks it is a tuple

    ttBar = (
        "TTbar.txt", # use job count ~5
        "TTW.txt",
        "TTZ.txt",
    )

    zz = ("ZZ.txt",)

    dy = (
        "DY10-50.txt",
        "DY50-inf.txt", # files 60-80 exceed 24hr wall clock time, use ~5 job count size
    )

    multiBoson = (
        "WW.txt",
        "WWW.txt",
        "WWZ.txt",
        "WZ.txt",
        "WZZ.txt",
        "ZZZ.txt",
    )

    higgsSignal = (
        "Higgs500.txt",
        "Higgs600.txt",
        "Higgs700.txt",
        "Higgs800.txt",
        "Higgs900.txt",
        "Higgs1000.txt",
        "Higgs1100.txt",
        "Higgs1200.txt",
        "Higgs1300.txt",
        "Higgs1400.txt",
        "Higgs1500.txt",
	)

    # NOTE: Muon only for Data - DP processing
    data = (
        "Electron2016.txt",
		"Electron2016APV.txt",
		"Electron2017.txt",
		"Electron2018.txt",
		"Muon2016.txt",
		"Muon2016APV.txt",
		"Muon2017.txt",
		"Muon2018.txt",
    )
    
    dataMu = (
		"Muon2016.txt",
		"Muon2016APV.txt",
		"Muon2017.txt",
		"Muon2018.txt",
    )

    qcd = (
        "QCD100-200.txt",
        "QCD200-300.txt",
        "QCD300-500.txt",
        "QCD500-700.txt",
        "QCD700-1000.txt",
        "QCD1000-1500.txt",
        "QCD1500-2000.txt",
        "QCD2000-inf.txt",
    )

    wjets = (
        "WJets.txt", # ~2000 files, use higher numFiles
    )

    darkPhotonSignal = ("darkPhotonBaselineRun2.txt",)
    
    darkPhotonNanoAOD = (
        "darkPhotonDecay_Higgs4DP.txt", #
        "darkPhotonDecay_HiggsDPZ.txt", #
        "darkPhotonDecay_SUSY.txt", #
        "darkPhotonDecay_ZPrime.txt", #
        "darkPhoton_DpMass0_1.txt",
        "darkPhoton_DpMass0_2.txt",
        "darkPhoton_DpMass0_4.txt",
        "darkPhoton_DpMass0_6.txt",
        "darkPhoton_DpMass0_9.txt",
        "darkPhoton_DpMass1_2.txt",
        "darkPhoton_DpMass1_5.txt",
        "darkPhoton_DpMass2_5.txt",
        "darkPhoton_DpMass4_0.txt",
        "darkPhotonHiggs125.txt",
        "darkPhotonHiggs300.txt",
        "darkPhotonRun2FSR_0_1.txt",
        "darkPhotonRun2FSR_0_3.txt",
    )
    
    lowStatistics = (
		"QCD50-100.txt",
        "WWW.txt",
        "TTbar.txt",
        "WWZ.txt",
        "ZZZ.txt",
    )

    background = ttBar + zz + dy + multiBoson + qcd # total 26 files

    ###########this one ######### background = qcd
    # background = qcd
    # background = ttBar + zz + dy + multiBoson + qcd

    # background = ttBar + zz + multiBoson + qcd # total 24 files - multiMuPtAnalysis coniguration
    # background = ttBar + zz + multiBoson + bQCD
    # background = dy # just dy configuration
    # background = bQCD

    # List of jobs to run on from those above
    # jobsList = [ttBar, zz, dy50, multiBoson, higgsSignal, higgsData] if analysis == 0 or analysis == 2 else [darkPhotonSignal]

    # jobsList = [higgsSignal] if analysis == 0 or analysis == 2 else [darkPhotonSignal]

    # COMMENTED FOR ML STRIP
    #jobsList = [darkPhotonSignal, ttBar, zz, dy, multiBoson, higgsSignal, data, qcd, wjets]  
    jobsList = [dy, qcd, darkPhotonSignal]  
    #jobsList = [darkPhotonSignal, multiBoson]  
    # could further improve this by adding every sub-job as a separate entry
    if args.crab:
        temp = []
        for job in jobsList:
            for file in job:
                temp.append((file,)) # add as a tuple

        jobsList = temp

    if os.path.exists(args.outputFile) and not args.keep:
        os.remove(args.outputFile)

    # list of processes
    processes = []
    for job in jobsList:
        newProcess = Process(
            target=loopRun, args=(args.crab, args.path, args.numFiles, args.skipFiles, job, args.outputFile)
        )
        processes.append(newProcess)

    # start jobs
    for process in processes:
        process.start()

    # Join jobs
    for process in processes:
        process.join()
