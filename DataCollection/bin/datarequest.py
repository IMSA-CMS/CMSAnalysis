
import os;

#parser = argparse.ArgumentParser()
# parser.add_argument("--trigger")
# parser.add_argument("--year")

from subprocess import Popen, PIPE

stdout = Popen('dasgoclient -query=”dataset dataset=/QCD_bEnriched_*/*UL*/NANOAODSIM”', shell=True, stdout=PIPE).stdout
output = stdout.read()

datasets = output.split()

print(datasets)
    #mass = dataset[dataset.find("M-") + 2 : dataset.find("00_Tune") + 2]
	#period = dataset[dataset.find("20UL") + 4 : dataset.find("20UL") + 6]
for dataset in datasets:
    if "/SingleElectron/Run2016B-02Apr2020_ver2-v1/NANOAOD" in dataset and "HIPM" not in dataset:
        period = dataset[dataset.find("Run")+3:dataset.find("Run")+8]
        name ="Data_Trigger_SingleElectron_Year_" + "2016B" + ".txt"
        #open('textfiles/Data/'+name, 'w') 
        req = 'dasgoclient --query="file dataset=' + dataset + '" > textfiles/Data/'+name
        print("making "+name)
        os.system(req) 

print("request complete")