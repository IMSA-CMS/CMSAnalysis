from subprocess import run
from multiprocessing import Process
from itertools import permutations

#this file doesn't work yet

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

def makeCut(firstPt, secondPt, thirdPt, fourthPt):
    # rewrite cut file here
    run('scram b -j')

    # jobs grouped by process
    # If a job only has one pickfile in it, make sure to add a comma at the end so that python thinks it is a tuple
    higgs = ("Higgs/HiggsPick300.txt", "Higgs/HiggsPick500.txt", "Higgs/HiggsPick700.txt", "Higgs/HiggsPick900.txt", "Higgs/HiggsPick1100.txt", "Higgs/HiggsPick1300.txt")
    
    ttbar = ("TTBar/TTBarPick500.txt", )
    zz = ("ZZ/ZZPick4.txt", )

    dy50 = ("DY/DYPick50.txt", )

    qcd50_100 = ("QCD/QCDPick50.txt", "QCD/QCDPick100.txt")
    qcd200_500 = ("QCD/QCDPick200.txt", "QCD/QCDPick500.txt")
    qcd700_1000 = ("QCD/QCDPick700.txt", "QCD/QCDPick1000.txt")
    qcd1500_2000 = ("QCD/QCDPick1500.txt", "QCD/QCDPick2000.txt")


    # List of jobs to run on from those above
    jobsList = [higgs, ttbar, zz, dy50, qcd50_100, qcd200_500, qcd700_1000, qcd1500_2000]
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

    #calculate cut significance here and compare 


if __name__ == '__main__':
    #run significance with no cut here (also get number of events from files) - maybe just hardcode these significances
    first = 0
    second = 0
    third = 0
    fourth = 0
    perms = [''.join(p) for p in permutations('1234')]
    permMap = {}
    while first < 1000:
        while second < 1000:
            while third < 1000:
                while fourth < 1000:
                    permMap[0] = first
                    permMap[1] = second
                    permMap[2] = third
                    permMap[3] = fourth
                    for perm in perms:
                        makeCut(permMap[int(perm[0]) - 1], permMap[int(perm[1]) - 1], permMap[int(perm[2]) - 1], permMap[int(perm[3]) - 1])
                    fourth = fourth + 1
                third = third + 1
            second = second + 1
        first = first + 1
