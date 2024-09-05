#!/bin/bash

# Check if the fileList variable is passed as an argument
if [ -z "$1" ]; then
    echo "Usage: $0 <fileList>"
    exit 1
fi

# Assign the first argument to the fileList variable
fileList="$1"

# Define the varNames array
#varNames=("1st Highest mu- Pt" "1st Highest Lepton Jet Pt" "Input Delta Pt Values" "Input Leading Pt Values")
varNames=("1st Highest mu- Pt" "1st Highest mu- Eta" "2nd Highest mu- Pt" "2nd Highest mu- Eta" "mu- Opposite Sign Invariant Mass" "1st Highest Lepton Jet Pt" "1st Highest Lepton Jet Eta" "1st Highest Lepton Jet Lepton Jet Delta R")

# Define the list of regions and their corresponding abbreviations
declare -A regions
# regions["High Mass and Different Sign"]="HMOS"
# regions["High Mass and Same Sign"]="HMSS"
# regions["Low Mass and Same Sign"]="LMSS"
# Add more regions and abbreviations as needed

# Loop through each variable name
# for varName in "${varNames[@]}"; do
#     # Loop through each region and its abbreviation
#     for region in "${!regions[@]}"; do
#         abbreviation=${regions[$region]}
#         # Construct the output filename
#         outputFile="${fileList}_$(echo $varName | tr -d ' ')_${abbreviation}.root"
        
#         # Run the ROOT macro
#         root -q "/uscms/home/jpalamad/analysis/CMSSW_14_0_4/src/CMSAnalysis/Analysis/bin/SuperPlot.C(\"/uscms/homes/j/jpalamad/analysis/CMSSW_14_0_4/src/CMSAnalysis/Output/${fileList}/\", \"${varName} ${region}\", \"/uscms/homes/j/jpalamad/analysis/CMSSW_14_0_4/src/CMSAnalysis/Analysis/bin/MuPtAnalysisNT/superplot_${outputFile}\")"
#     done
# done
for varName in "${varNames[@]}"; do
    # Construct the output filename
    outputFile="${fileList}_$(echo $varName | tr -d ' ').root"
    
    # Run the ROOT macro
    root -q "/uscms/home/jpalamad/analysis/CMSSW_14_0_4/src/CMSAnalysis/Analysis/bin/SuperPlot.C(\"/uscms/homes/j/jpalamad/analysis/CMSSW_14_0_4/src/CMSAnalysis/Output/${fileList}/\", \"${varName}\", \"/uscms/homes/j/jpalamad/analysis/CMSSW_14_0_4/src/CMSAnalysis/Analysis/bin/MuPtAnalysisLeptonJetDeltaR/superplot_${outputFile}\")"
done
