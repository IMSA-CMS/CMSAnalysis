import os;

#parser = argparse.ArgumentParser()
# parser.add_argument("--trigger")
# parser.add_argument("--year")

from subprocess import Popen, PIPE

stdout = Popen('dasgoclient --query="dataset dataset=/HPlusPlus*HTo2L*/RunIISummer20UL*/MiniAODSIM"', shell=True, stdout=PIPE).stdout
output = stdout.read()

datasets = output.split()

print(datasets)
for dataset in datasets:
	mass = dataset[dataset.find("M-") + 2 : dataset.find("00_Tune") + 2]
	period = dataset[dataset.find("20UL") + 4 : dataset.find("20UL") + 6]
	if "APV" in dataset:
		period += "APV"
	aod = dataset[-10 : -6].lower().capitalize()
	name = "H++toLL_Format_"+aod+"AOD_Lepton_All_Mass_"+mass+"_Period_20"+period+"_Run_2.txt"
	#open('textfiles/Data/'+name, 'w') 
	req = 'dasgoclient --query="file dataset=' + dataset + '" > textfiles/H++toLL/'+name
	print("making "+name)
	os.system(req) 

print("request complete")





