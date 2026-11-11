# Analysis tools for IMSA-CMS SIR
README Author: Dean Cianciolo and George Bayliss - last updated 12/14/22

This directory contains the tools for formatting plots and making tables. It takes data from root files to do so. It contains functions that can quickly generate superimposed and stacked histograms, estimators to get expected yields, and functions to make tables of data.

# Basic SuperImpose

SuperImpose is a basic super imposing tool.

To run SuperImpose you need to type,

```
root SuperImpose.C+
```

SuperImpose.C can take any ammount of histograms and Super Impose them onto each other. They are scaled by 1/integral of events. SuperImpose.C stores the information to access the histograms in the vectors: "files,hists". These vectors contains the associates root file path and histogram within that file. (The first elements of each vector correspond to the first hist, the second elements the second, etc...). SuperImpose.C outputs a png file.

# Auto SuperImpose

Auto SuperImpose is a more advanced super imposing tool. 

```
root AutoSuperImpose.C+
```

AutoSuperImpose.C takes root files as input and chooses a list histograms from each root file. It then SuperImposes each set of histograms of a certain type on each other. Thus if 3 root files are provided and 4 histogram names, it will output 4 histograms each consisting of 3 super imposed upon each other. To control the files and histograms edit the associated vector of stringa. 

The output of AutoSuperImpose.C is a root file containing the output histograms. When running you likely want to setup the name of this file and also the names in the legend. There are strings for this in AutoSuperImpose.C too.

# Analysis Class
An Analysis class is necessary to use some features of the Analysis directory: JumboPlot, SuperPlot and Table. There may already be an analysis class that fits your needs, if not ask Dean Cianciolo for help in creating one. 
