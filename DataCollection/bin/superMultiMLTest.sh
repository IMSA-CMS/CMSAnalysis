#!/bin/bash

# Define paths
#/# TMVA_WEIGHTS="./TMVAWeights"
TMVA_WEIGHTS="/eos/uscms/store/user/jpalamad/rootBackups/MLWeightsBackup"
DATASET_WEIGHTS="dataset/weights"
TEMP_BACKUP="temp_backup"

# Create a temporary backup directory
mkdir -p "$TEMP_BACKUP"

# Backup existing weight files
cp "$DATASET_WEIGHTS/TMVAClassification_BDT.class.C" "$TEMP_BACKUP/"
cp "$DATASET_WEIGHTS/TMVAClassification_BDT.weights.xml" "$TEMP_BACKUP/"

# Loop through each model in TMVAWeights
for MODEL_DIR in "$TMVA_WEIGHTS"/*/; do
    echo "Processing model in $MODEL_DIR"
    folder_name=$(basename "$MODEL_DIR")

    # Copy the model's weight files to dataset/weights/
    cp "$MODEL_DIR/TMVAClassification_BDT.class.C" "$DATASET_WEIGHTS/"
    cp "$MODEL_DIR/TMVAClassification_BDT.weights.xml" "$DATASET_WEIGHTS/"

    #/# TMVA_OUTFILE="TMVAOutputs/${folder_name}.root"
    TMVA_OUTFILE="/eos/uscms/store/user/jpalamad/rootBackups/MLOutputsBackup/${folder_name}.root"
    maxPunziValue=$(root -l -b -q "cliPunzi.C(\"$TMVA_OUTFILE\")" | tail -n 1)
    echo "Max Punzi value: $maxPunziValue"
    echo

    # Run the testing script
    echo "Running tests..."
    root -q "MLTest.C(\"BDTFiles/strippedSG_numFiles1.root\", \"BDTFiles/strippedBG_DY10_numFiles1.root\", $maxPunziValue)"
    root -q "MLTest.C(\"BDTFiles/strippedSG_numFiles1.root\", \"BDTFiles/strippedBG_DY50_numFiles5.root\", $maxPunziValue)"
    root -q "MLTest.C(\"BDTFiles/strippedSG_numFiles1.root\", \"BDTFiles/strippedBG_QCD100-200_numFiles1.root\", $maxPunziValue)"
    root -q "MLTest.C(\"BDTFiles/strippedSG_numFiles1.root\", \"BDTFiles/strippedBG_QCD200-300_numFiles1.root\", $maxPunziValue)"
    root -q "MLTest.C(\"BDTFiles/strippedSG_numFiles1.root\", \"BDTFiles/strippedBG_QCD300-500_numFiles1.root\", $maxPunziValue)"
    root -q "MLTest.C(\"BDTFiles/strippedSG_numFiles1.root\", \"BDTFiles/strippedBG_QCD500-700_numFiles1.root\", $maxPunziValue)"
    root -q "MLTest.C(\"BDTFiles/strippedSG_numFiles1.root\", \"BDTFiles/strippedBG_QCD700-1000_numFiles1.root\", $maxPunziValue)"
    root -q "MLTest.C(\"BDTFiles/strippedSG_numFiles1.root\", \"BDTFiles/strippedBG_QCD1000-1500_numFiles1.root\", $maxPunziValue)"
    root -q "MLTest.C(\"BDTFiles/strippedSG_numFiles1.root\", \"BDTFiles/strippedBG_QCD1500-2000_numFiles1.root\", $maxPunziValue)"
    root -q "MLTest.C(\"BDTFiles/strippedSG_numFiles1.root\", \"BDTFiles/strippedBG_QCD2000-inf_numFiles1.root\", $maxPunziValue)"
done

# Restore original weight files
echo "Restoring original weight files..."
cp "$TEMP_BACKUP/TMVAClassification_BDT.class.C" "$DATASET_WEIGHTS/"
cp "$TEMP_BACKUP/TMVAClassification_BDT.weights.xml" "$DATASET_WEIGHTS/"

# Clean up temporary backup directory
rm -rf "$TEMP_BACKUP"

echo "Testing complete and original files restored."