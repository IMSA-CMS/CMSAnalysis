import shutil # used to copy files

################## Reading in Parameters ######################
f = open('DoublyChargedHiggsParameters.txt', 'r')
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
processParameters += "\n\t\t),\n" # processParameters can now replace the declaration in the python template file
##############################################################

for i in range(500, 1501, 100): #mass range 500 to 1500 with increments of 100
    newFile = f'higgsDatacards/run3/pythiaFragmentDoublyChargedHiggsMass{i}.py'
    shutil.copy('template.py', newFile)

    newf = open(newFile, 'r')
    data = newf.readlines()
    newf.close()

    data[14] = processParameters

    newf = open(newFile, 'w')
    newf.writelines(data)
    newf.close()

    newf = open(newFile, 'r')
    data = newf.readlines()
    data[26] = f"\t\t\t'9900041:m0 = {i}'\n"
    newf.close()

    newf = open(newFile, 'w')
    newf.writelines(data)