import os
import shutil # used to copy files
import subprocess

################## Reading in Parameters ######################
f = open('Higgsto2ZDPortalParameters.txt', 'r')
lines = f.readlines()
f.close()

params = []
for line in lines:
    if not(line[0] == '#' or line[0] == '\n'): #real all non-commented and non-empty lines into an array
        params.append("'" + line.strip() + "'")

processParameters = "\t\tprocessParameters = cms.vstring(\n"

for line in params:
    processParameters += '\t\t\t' + line + ',\n'

processParameters = processParameters[:-2]

##############################################################

# darkphoton stuff mass
# masses = [0.1, 0.25, 0.3, 0.4, 0.6, 0.9, 1.2, 1.5, 2.5, 4.0]
# for mass in masses:
#     newProcessParameters = processParameters
#     savedpath = os.getcwd()
#     os.chdir("DarkRadiation")
#     branching_ratios = subprocess.run(["./DarkRadiation", str(mass)], stdout = subprocess.PIPE).stdout.decode("utf-8")
#     branching_ratios = branching_ratios.split("\n")
#     os.chdir(savedpath)
#     for line in branching_ratios:
#         newProcessParameters += ",\n\t\t\t'" + line + "'"
#     newProcessParameters = newProcessParameters[:-7]
#     newProcessParameters += "\n\t\t),\n"
#     print(newProcessParameters)
#     newFile = f'darkPhotonDatacards/variedDPMass/pythiaFragmentDarkPhotonMass{mass}.py'
#     shutil.copy('template.py', newFile)

#     newf = open(newFile, 'r')
#     data = newf.readlines()
#     newf.close()

#     data[14] = newProcessParameters

#     newf = open(newFile, 'w')
#     newf.writelines(data)
#     newf.close()

#     newf = open(newFile, 'r')
#     data = newf.readlines()
#     data[70] = f"\t\t\t'4900022:m0 = {mass}',\n"
#     newf.close()

#     newf = open(newFile, 'w')
#     newf.writelines(data)

# vary higgs mass
# masses = [125, 300, 1000]
# for mass in masses:
#     newFile = f'darkPhotonDatacards/variedHiggsMass/pythiaFragmentDarkPhotonHiggsMass{mass}.py'
#     shutil.copy('darkPhotonDatacards/baseline.py', newFile)

#     newf = open(newFile, 'r')
#     data = newf.readlines()
#     data[17] = f"\t\t\t'25:m0 = {mass}',\n"
#     data[22] = f"\t\t\t'25:mMin = {mass},\n"
#     newf.close()

#     newf = open(newFile, 'w')
#     newf.writelines(data)

# vary alphafSR
# alphaFSR = [0, 0.1, 0.3]
# for fsr in alphaFSR:
#     newFile = f'darkPhotonDatacards/variedAlphaFSR/pythiaFragmentDarkPhotonAlphaFSR{fsr}.py'
#     shutil.copy('darkPhotonDatacards/baseline.py', newFile)

#     newf = open(newFile, 'r')
#     data = newf.readlines()
#     data[82] = f"\t\t\t'HiddenValley:alphaFSR = {fsr}',\n"
#     newf.close()

#     newf = open(newFile, 'w')
#     newf.writelines(data)

#vary portal
portals = ['Higgsto2ZD', 'Higgsto4ZD', 'HiggstoZZD', 'SUSYto2ZD', 'zPrime']
for portal in portals:
    newFile = f'darkPhotonDatacards/variedPortal/pythiaFragmentDarkPhoton{portal}.py'
    shutil.copy('template.py', newFile)

    portalFile = open(f'{portal}PortalParameters.txt', 'r')
    lines = portalFile.readlines()
    portalFile.close()

    params = []
    for line in lines:
        if not(line[0] == '#' or line[0] == '\n'): #real all non-commented and non-empty lines into an array
            params.append("'" + line.strip() + "'")

    processParameters = "\t\tprocessParameters = cms.vstring(\n"

    for line in params:
        processParameters += '\t\t\t' + line + ',\n'

    processParameters = processParameters[:-2]

    newProcessParameters = processParameters
    savedpath = os.getcwd()
    os.chdir("DarkRadiation")
    branching_ratios = subprocess.run(["./DarkRadiation", str(0.3)], stdout = subprocess.PIPE).stdout.decode("utf-8")
    branching_ratios = branching_ratios.split("\n")
    os.chdir(savedpath)
    for line in branching_ratios:
        newProcessParameters += ",\n\t\t\t'" + line + "'"
    newProcessParameters = newProcessParameters[:-7]
    newProcessParameters += "\n\t\t),\n"
    print(newProcessParameters)


    newf = open(newFile, 'r')
    data = newf.readlines()
    newf.close()
    
    data[14] = newProcessParameters

    newf = open(newFile, 'w')
    newf.writelines(data)
    newf.close()