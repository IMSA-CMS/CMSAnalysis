# Pythia8-Dark Photon and Higgs Plus Plus Event Production

This packages is designed to perform GEN-only private MC productions using the CMSSW Pythia8 release that is included in the working CMSSW release. It also includes basic TTree-making files and simplistic histogramming that is intended primarly as a starting point to springboard to do whatever specific plotting is desired.

# Updated Instructions to Run EventGenerator
These instructions were most recently updated by George Bayliss on 11/03/22.

All commands should be run from MCGeneration/test.

Try running the generator executable using cmsRun.

```
cmsRun EventGenerator_edm_cfg.py maxEvents=100 globalTag=Run2 pythiaSettings=SUSYto2ZDPortalParameters.txt output=output.root dpMass=0.3
```

Max events corresponds the ammount of events you wish to generate.

Global tag should be 'Run2' on CMSSW_11_0_2 and 'Run3' on CMSSW_12_4_3. 

Pythia Settings is a text file that has the options for the time of event you wish to generate. 

Output is the root file in which you want the output to go.

dpMass is Dark Photon mass, you can leave this blank if you aren't running something involving Dark Photons.

Running this file takes a long time and creates a lot of output you can run this in the background using nohup.

```
nohup cmsRun EventGenerator_edm_cfg.py maxEvents=100 globalTag=Run2 pythiaSettings=dpPythiaParameters.txt output=output.root dpMass=0.3 &> 
```

Don't get trigger with nohup or Marguerite will find you.

# Updated Instructions to Run HistGenerator (Legacy code)

Note: This is old code. Unless you have a specific reason to use this code, run the file you generated via runAnalyzer rather than here.

To run HistGenerator you simply put:
```
cmsRun HistGenerator_cfg.py
```
Running this command prompts you for an input file which would be root file you generated from EventGenerator and a Dimuon plugin file, DimounDarkPhoton or DimuonHiggsPlusPlus.

Once again you can input a file directly through the command line:
```
cmsRun HistGenerator_cfg.py inputFileName=output.root
```
The file generated will be called "histograms_[inputFileName].root", in this case "histograms_output.root"

To view the histograms you need to open a TBrowser:
```
root histograms_output.root
TBrowser root
```

NOTE: If you get an error saying that either Dimuon plugin is not found, try compiling from the MCGeneration directory. 
