# RunAnalyzer Framework

This package is designed to perform analysis and histogramming of remote Delphes, MiniAOD and  NanoAOD root files and local Stripped and MCGeneration root files. It makes heavy use of the ROOT framework and outputs histograms in a root file.

# Updated Instructions to Run Analyzer on Local Files
These instructions were most recently updated by George Bayliss on 12/12/22.

All commands should be run from DataCollection/bin.

You can run the analyzer exectable with the command "runAnalyzer".

```
runAnalyzer input=input.root output=output.root analysis=GenSim
```

Input corresponds to root the file you are inputing if you are running a local one. See below for remote root files.

Output is the root file in which you want the output to go.

Analysis refers to the Plan which you wish to connect to the process. A Plan determines what analysis will be done and which histograms will be made. You should figure out what Plan you are using.

Our framework is pretty fast however if you are running more than 10,000 events it may be a good idea to use nohup.

```
nohup runAnalyzer input=input.root output=output.root analysis=GenSim & 
```

Don't use it too much though.

# Updated Instructions to Run Analyzer on Remote Files

Analyzer accesses remote files via the use of "pickfile" text files. These pickfiles specify which root files Analyzer should search for. 

If you already have a pickfile you can simply put it in the input section of the command line.


```
runAnalyzer input=pickfile.txt output=output.root analysis=GenSim
```

When running remote files you should not that some pickfiles will access hundreds of rootfiles. To prevent this you can specify the ammount of files you wish to run. 

```
runAnalyzer input=pickfile.txt output=output.root analysis=GenSim numFiles=1
```

Finally if you do not already have a pick file you can leave the input option blank and the command line will prompt you for some information and automatically create a pickfile then run using it.

# Updated Instructions for Module Options

Module Options is command line input which allows for extra options to be added to any module in the process. To use Module Options simply add it to your command line input.

```
runAnalyzer input=input.root output=output.root analysis=GenSim moduleOptions=moduleOptions.txt
```

The command line takes a text file input which should be formatted as such:

```
SettingName:SettingValue
```

and a # for any line that is a comment.

As of 12/12/22 this feature is still under development so ask George if you have any questions.

# Updated Instructions to Run EventDump

EventDump is a module that prints out the contents of every event. It is useful to check specific interactions or decays and for debugging purposes. It runs with Analyzer. 

As of 12/12/22 this feature is only implemented for GenSim events.

If you wish to run EventDump on a file simply include it in the plan file then run the analyzer as normal. 

```
auto eventMod = make_shared<GenSimEventModule>();
analyzer.addAnalysisModule(eventDump);
```

The output goes to a file called GenSimEventDump.txt in bin. Sometimes this file takes a little time to update if you process a lot of events so if the events do not appear right away wait a minute.

GenSimEventModule also has a numEvents parmeter if you wish to only process a certain number events.

```
auto eventMod = make_shared<GenSimEventModule>(100);
analyzer.addAnalysisModule(eventDump);
```
