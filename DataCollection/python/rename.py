import os

folder = "bin/textfiles/DarkPhoton"
for count, filename in enumerate(os.listdir(folder)):
	if "FSR" in filename:
		continue

	index = filename.index("DpMass_") + 11
	newfilename = filename[0:index] + "FSR_0_0_" + filename[index:]
	# print(newfilename)
	os.rename("bin/textfiles/DarkPhoton/" + filename, "bin/textfiles/DarkPhoton/" + newfilename)
