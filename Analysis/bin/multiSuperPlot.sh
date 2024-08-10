#!/bin/bash

# varNames=("Eta" "Leading Pt" "nParticles" "Delta R")
varNames=("1st Highest mu- Pt" "1st Highest Lepton Jet Pt" "Input Delta Pt Values" "Input Leading Pt Values")

# Define the fileList variable
fileList="RecoConfig1_DPTrigger_MuPtCut_Inputs"

# Define the list of regions and their corresponding abbreviations
declare -A regions
regions["High Mass and Different Sign"]="HMOS"
regions["High Mass and Same Sign"]="HMSS"
regions["Low Mass and Same Sign"]="LMSS"
# Add more regions and abbreviations as needed

# Loop through each Eta value
for varName in "${varNames[@]}"; do
    # Loop through each region and its abbreviation
    for region in "${!regions[@]}"; do
        abbreviation=${regions[$region]}
        # Construct the output filename
        outputFile="${fileList}_$(echo $varName | tr -d ' ')_${abbreviation}.root"
        
        # Run the ROOT macro
        # root -q "SuperPlot.C(\"Input ${varName} Values ${region}\", \"MultiSuperPlots/superplot_${outputFile}\")"
        root -q "SuperPlot.C(\"${varName} ${region}\", \"MultiSuperPlots/superplot_${outputFile}\")"
    done
done
