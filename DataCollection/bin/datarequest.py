import os;

#parser = argparse.ArgumentParser()
# parser.add_argument("--trigger")
# parser.add_argument("--year")

from subprocess import Popen, PIPE

stdout = Popen('dasgoclient --query="dataset dataset=/*/Run2017B*UL*v2-v1/MINIAOD"', shell=True, stdout=PIPE).stdout
output = stdout.read()

datasets = output.split()

print(datasets)
for dataset in datasets:
	parser = dataset.split("/")
	name = parser[1]+parser[2]+".txt"
	open('textfiles/Data/'+name, 'w') 
	req = 'dasgoclient --query="file dataset=' + dataset + '" > textfiles/Data/'+name
	print("making "+dataset)
	os.system(req) 

print("request complete")





