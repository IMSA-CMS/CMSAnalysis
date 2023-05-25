import os 
import subprocess

f = open('python/Higgs/test.txt', 'r')

for line in f.readlines():
	NAME = 'H++toLL/'
	line = line.strip()

	output = str(subprocess.check_output('dasgoclient -query="file dataset=' + line + '"', shell=True))
	output = output[2:len(output)-3]
	output = output.replace('\\n', "\n")

	decay = 'Lepton_All'
	NAME += decay + '_'

	Mass = line[int(line.index("_M") + 3) : int(line.index("_Tune"))]
	NAME += "Mass_" + Mass + "_"

	PERIOD = 'Period_'
	if 'APV' in line:
		PERIOD += "2016APV"
	elif "20UL16" in line:
		PERIOD += "2016"
	elif "20UL17" in line:
		PERIOD += "2017"
	elif "20UL18" in line:
		PERIOD += "2018"

	NAME += PERIOD

	NAME += "_Run_2.txt"

	newFile = open('bin/textfiles/' + NAME, 'w')
	newFile.write(output)
	newFile.close()