import os
import json

# parser = argparse.ArgumentParser()
# parser.add_argument("--trigger")
# parser.add_argument("--year")

from subprocess import Popen, PIPE

# Start a process to run the dasgoclient command and read the output
# dasgoclient -query="dataset dataset=/*EGamma*/*/NANOAOD"

with open('dasgoclient_query_bqcd.json') as f:  # Replace 'data.json' with the actual file name
    data = json.load(f)

stdout = Popen('dasgoclient -query="dataset dataset=/QCD_bEnriched_HT*CP5*/*UL*/NANOAODSIM"', shell=True, stdout=PIPE).stdout
output = stdout.read()

# Split the output into a list of datasets
datasets = output.split()

print("datasets\n")
    #mass = dataset[dataset.find("M-") + 2 : dataset.find("00_Tune") + 2]
	#period = dataset[dataset.find("20UL") + 4 : dataset.find("20UL") + 6]

# #for dataset in datasets:
#     if ("/EGamma/Run2018D-UL2018_MiniAODv2_NanoAODv9-v3/NANOAOD" in dataset.decode('utf-8')) :
#         #period = dataset[dataset.find("Run")+3:dataset.find("Run")+8]
#         name ="Data_Trigger_SingleElectron_Year_2018D.txt"
#         print("found\n")
#         open('textfiles/Data/' + name, 'w')
#         req = 'dasgoclient --query="file dataset=' + dataset.decode('utf-8') + '" > textfiles/PlainQCD/' + name
#         os.system(req)
#         print("Making " + name + "\n")
#         os.system(req) 

# Iterate over each dataset

for dataset in datasets:
    mass = dataset.decode('utf-8')[dataset.decode('utf-8').find("HT") + 2 : dataset.decode('utf-8').find("_Tune")]
    mass = mass.replace("to", "-")
    period = dataset.decode('utf-8')[dataset.decode('utf-8').find("20UL") + 4:dataset.decode('utf-8').find("20UL") + 6]
    version = dataset.decode('utf-8').find("v9") -1
    apv = False
    v9 = False
    Jme = False
    bGen = False
    PSWeights = False
    MLM = False
    
    if b"APV" in dataset:
        apv = True
    if version != -2 and dataset.decode('utf-8')[version] != "_":
        v9 = True
    if b"JMENano" in dataset:
        Jme = True
    if b"BGenFilter" in dataset:
        bGen = True
    if b"PSWeights" in dataset:
        PSWeights = True
    if b"MLM" in dataset:
        MLM = True
    #validPeriods = ["16", "17", "18"]
    validPeriods = ["17"]

    # print("v9:", v9)
    # print("not Jme:", not Jme)
    # print("not bGen:", not bGen)
    # print("period in validPeriods:", period in validPeriods)
    # print("PSWeights:", PSWeights)

    # Check if the dataset is a specific case
    if (v9 and not Jme and not bGen and period in validPeriods):

        if((period == "16" or apv) and MLM):
            continue

        # print(dataset.decode('utf-8')+"\n")
        # period = dataset[dataset.find("Run") + 3:dataset.find("Run") + 8]
        apvstr = "APV" if apv else ""
        psstr = "PSWeights_" if PSWeights else ""
        name = "PlainQCD_HTCut_" + psstr + mass + "_Run_2_Year_" + "20" + period + apvstr + ".txt"

        # fetch datase information
        query_string = dataset.decode('utf-8').split("NanoAOD", 1)[0] + "MiniAOD"

        for record in data:
            if query_string in record['DAS']:
                # Extract the relevant portions for the print statement
                process_name_parts = record['process_name'].split('_')
                print(f"QCD_{mass}  {record['cross_section']}   {record['total_uncertainty']}")

        # Open a text file in write mode
        open('textfiles/BQCD/' + name, 'w')
        req = 'dasgoclient --query="file dataset=' + dataset.decode('utf-8') + '" > textfiles/BQCD/' + name
        print("Making " + name + "\n")
        os.system(req)

print("Request complete")