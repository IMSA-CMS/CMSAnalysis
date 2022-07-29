# Pythia8-Dark Photon and Higgs Plus Plus Event Production

This packages is designed to perform GEN-only private MC productions using the CMSSW Pythia8 release that is included in the working CMSSW release. It also includes basic TTree-making files and simplistic histogramming that is intended primarly as a starting point to springboard to do whatever specific plotting is desired.

# Updated Instructions to Run EventGenerator
These instructions were most recently updated by George Bayliss on 7/18/22.

All commands should be run from MCGeneration/test.

Try running the generator executable using cmsRun.

```
cmsRun EventGenerator_edm_cfg.py
```
The terminal will then prompt you for 
1. A value for the maximum amount of events. (You probably want a low number for your first run as large values can take a long time to run).
2. A global tag. For Run2 you must run in cmssw_11_0_2 and for Run3 you must run in cmssw_12_4_3.
3. A pythia settings file. There are two of these text files, dpPythiaParameters.txt and hppPythiaParameters.txt, corresponding to Dark Photons and Higgs Plus Plus respectively.
4. A root output file. 

Once you input all of these you should see output on the screen, however be patient as it can take a minute or two to run the first time.

Now, you can expedite this process by inputing the parameters in the cmsRun command:

```
cmsRun EventGenerator_edm_cfg.py maxEvents=1000 globalTagName=Run2 pythiaSettingsFileName=dpPythiaParameters.txt outputFileName=output.root
```
Of course, there are many more settings you can change and you can find all the options at the top of EventGenerator_edm_cfg and also in mcCommandLineOptions_cfi and ledMcCmndLineOptions_cfi which are located in the python directory.

Finally, you can clearly see that running this file creates a lot of screen output. If you wish to redirect this to a .txt file and run it in the background, you can do it like so:

```
cmsRun EventGenerator_edm_cfg.py maxEvents=1000 globalTagName=Run2 pythiaSettingsFileName=dpPythiaParameters.txt outputFileName=output.root &> outFile.txt &
```

Note that when doing this you must input all of the above parameters in the command line as the terminal will no longer prompt you for them.

# Updated Instructions to Run HistGenerator

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
