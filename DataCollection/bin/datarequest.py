import os;

#parser = argparse.ArgumentParser()
# parser.add_argument("--trigger")
# parser.add_argument("--year")

from subprocess import Popen, PIPE

stdout = Popen('dasgoclient -query="dataset dataset=/*SingleMuon*/*/NANOAOD"', shell=True, stdout=PIPE).stdout
output = stdout.read()

datasets = output.split()

print(datasets)
    #mass = dataset[dataset.find("M-") + 2 : dataset.find("00_Tune") + 2]
	#period = dataset[dataset.find("20UL") + 4 : dataset.find("20UL") + 6]
    
dataset = "/SingleMuon/Run2016C-UL2016_MiniAODv1_NanoAODv2-v1/NANOAOD"
period = dataset[dataset.find("Run")+3:dataset.find("Run")+8]
name ="Data_Trigger_SingleMuon_Year_2016C"
#open('textfiles/Data/'+name, 'w') 
req = 'dasgoclient --query="file dataset=' + dataset + '" > textfiles/Data/'+name
print("making "+name)
os.system(req) 

print("request complete")