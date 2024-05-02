import os

# parser = argparse.ArgumentParser()
# parser.add_argument("--trigger")
# parser.add_argument("--year")

from subprocess import Popen, PIPE

# Start a process to run the dasgoclient command and read the output
stdout = Popen('dasgoclient -query="dataset dataset=/QCD_bEnriched_*/*UL*/NANOAODSIM"', shell=True, stdout=PIPE).stdout
output = stdout.read()

# Split the output into a list of datasets
datasets = output.split()

print(datasets)
    #mass = dataset[dataset.find("M-") + 2 : dataset.find("00_Tune") + 2]
	#period = dataset[dataset.find("20UL") + 4 : dataset.find("20UL") + 6]
for dataset in datasets:
    if "2018" in dataset and "v2" in dataset and "HIPM" not in dataset:
        period = dataset[dataset.find("Run")+3:dataset.find("Run")+8]
        name ="Data_Trigger_SingleElectron_Year_" + "2018" + ".txt"
        #open('textfiles/Data/'+name, 'w') 
        #req = 'dasgoclient --query="file dataset=' + dataset + '" > textfiles/Data/'+name
        print("making "+name)
        os.system(req) 

# Iterate over each dataset
for dataset in datasets:
    mass = dataset[dataset.find("HT") + 2 : dataset.find("_Tune")]
    mass = mass.replace("to", "-")
    period = dataset[dataset.find("20UL") + 4:dataset.find("20UL") + 6]
    apv = "APV" in dataset
    v9 = "v9" in dataset

    # Check if the dataset is a specific case
    if ((period == "16" and not(v9)) or (period == "17" and not(apv) and not(v9)) or (period == "18" and v9)):
        # period = dataset[dataset.find("Run") + 3:dataset.find("Run") + 8]
        apvstr = "APV" if apv else ""
        v9str = "v9" if v9 else ""
        name = "QCD_HTCut_" + mass + "_Run_2_Year_" + "20" + period + apvstr + ".txt"
        # Open a text file in write mode
        open('textfiles/NewQCD/' + name, 'w')
        req = 'dasgoclient --query="file dataset=' + dataset + '" > textfiles/NewQCD/' + name
        print("Making " + name)
        os.system(req)

print("Request complete")