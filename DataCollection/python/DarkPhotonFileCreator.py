import os 
import subprocess

f = open('python/DarkPhoton/nanoAODrun3/susy.txt', 'r')

for line in f.readlines():
	NAME = 'DarkPhoton/Run3NanoAOD/DarkPhoton_'
	line = line.strip()

	output = str(subprocess.check_output('dasgoclient -query="file dataset=' + line + '"', shell=True))
	output = output[2:len(output)-3]
	output = output.replace('\\n', "\n")

	decay = 'Decay_SUSY'
	NAME += decay + '_'

	DPMass = line[int(line.index("DPMass") + 7) : int(line.index("DPMass") + 10)]
	DPMass = DPMass.replace('p', '_')
	NAME += 'DpMass_' + DPMass + '_'

	if "FSR" in line:
		FSR = line[int(line.index("FSR", 40, 100) + 4) : int(line.index("FSR", 40, 100) + 7)]
		FSR = FSR.replace('p', '_')
		NAME += 'FSR_' + FSR + '_'
	else:
		NAME += "FSR_0_0_"

	NAME += "Format_NanoAOD_"
	HMass = line[int(line.index("-", line.index("HMass"), 100) + 1) : int(line.index("_", line.index("HMass"), 100))]
	HMass = HMass.replace('p', '_')
	NAME += "HiggsMass_" + HMass + "_"

	PERIOD = 'Period_'
	if 'APV' in line:
		PERIOD += "2016APV"
	elif "20UL16" in line:
		PERIOD += "2016"
	elif "20UL17" in line:
		PERIOD += "2017"
	elif "20UL18" in line:
		PERIOD += "2018"
	elif "postEE" in line:
		PERIOD += "2022EE"
	elif "2022" in line:
		PERIOD += "2022"

	NAME += PERIOD
	if 'Run3' in line:
		NAME += "_Run_3.txt"
	else:
		NAME += "_Run_2.txt"
	
	newFile = open('bin/textfiles/' + NAME, 'w')
	newFile.write(output)
	newFile.close()
	print(NAME)
