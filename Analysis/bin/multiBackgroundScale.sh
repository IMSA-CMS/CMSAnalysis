#!/bin/bash

# Define plot names and regions
#plotNames=("Eta" "Lepton Jet Delta R" "Lepton Jet Mass" "Phi" "Pt")
plotNames=("Lepton Jet Delta R")
regions=("High Mass and Different Sign" "High Mass and Same Sign" "Low Mass and Same Sign")
outputDir="simultaneousFits"

# Create output directory if it doesn't exist
mkdir -p "$outputDir"

# Loop through each region and each plot name to generate the fit
for region in "${regions[@]}"; do
    # Format the region name for output file names
    regionName="${region// /_}"

    for plotName in "${plotNames[@]}"; do
        # Construct full plot name and output file names
        fullPlotName="${plotName} ${region}"
        outputName="${outputDir}/${regionName}_${plotName// /_}_Fit.png"
        outputNameRoot="${outputDir}/${regionName}_${plotName// /_}_Fit.root"
        outputNamePoisson="${outputDir}/${regionName}_${plotName// /_}_Poisson_Fit.png"
        outputNameRootPoisson="${outputDir}/${regionName}_${plotName// /_}_Poisson_Fit.root"

        # Run the BackgroundScaleSimultaneous and BackgroundScalePoisson functions
        # root -l -b -q "BackgroundScaleSimultaneous.C(\"$fullPlotName\", \"$outputName\")"
        root -l -b -q "BackgroundScaleSimultaneous.C(\"$fullPlotName\", \"$outputNameRoot\")"
        # root -l -b -q "BackgroundScalePoisson.C(\"$fullPlotName\", \"$outputNamePoisson\")"
        # root -l -b -q "BackgroundScalePoisson.C(\"$fullPlotName\", \"$outputNameRootPoisson\")"
    done
done

echo "Simultaneous fits have been generated in the $outputDir directory for all control regions."
