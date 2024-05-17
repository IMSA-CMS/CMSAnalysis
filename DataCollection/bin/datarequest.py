import os

# parser = argparse.ArgumentParser()
# parser.add_argument("--trigger")
# parser.add_argument("--year")

from subprocess import Popen, PIPE

# Start a process to run the dasgoclient command and read the output
stdout = Popen('dasgoclient -query="dataset dataset=/*EGamma*/*/NANOAOD"', shell=True, stdout=PIPE).stdout
output = stdout.read()

# Split the output into a list of datasets
datasets = output.split()

print("datasets\n")
    #mass = dataset[dataset.find("M-") + 2 : dataset.find("00_Tune") + 2]
	#period = dataset[dataset.find("20UL") + 4 : dataset.find("20UL") + 6]

for dataset in datasets:
    if ("/EGamma/Run2018D-UL2018_MiniAODv2_NanoAODv9-v3/NANOAOD" in dataset.decode('utf-8')) :
        #period = dataset[dataset.find("Run")+3:dataset.find("Run")+8]
        name ="Data_Trigger_SingleElectron_Year_2018D.txt"
        print("found\n")
        open('textfiles/Data/' + name, 'w')
        req = 'dasgoclient --query="file dataset=' + dataset.decode('utf-8') + '" > textfiles/Data/' + name
        os.system(req)
        print("Making " + name + "\n")
        os.system(req) 

# Iterate over each dataset
for dataset in datasets:
    #mass = dataset[dataset.find("HT") + 2 : dataset.find("_Tune")]
    #mass = mass.replace("to", "-")
    #period = dataset[dataset.find("20UL") + 4:dataset.find("20UL") + 6]
    #apv = "APV" in dataset
    #v9 = "v9" in dataset

    # Check if the dataset is a specific case
    if (name == "Data_Trigger_SingleElectron_Year_2018A"):
        # period = dataset[dataset.find("Run") + 3:dataset.find("Run") + 8]
        apvstr = "APV" if apv else ""
        v9str = "v9" if v9 else ""
        name = "QCD_HTCut_" + mass + "_Run_2_Year_" + "20" + period + apvstr + ".txt"
        # Open a text file in write mode
        #open('textfiles/Data/' + name, 'w')
        #req = 'dasgoclient --query="file dataset=' + dataset + '" > textfiles/Data/' + name
        #print("Making " + name + "\n")
        #os.system(req)

print("Request complete")