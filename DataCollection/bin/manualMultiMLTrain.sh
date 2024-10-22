#!/bin/bash

# Define the combinations array
COMBINATIONS=(
  "Uniform PropWeight 0 1 1 0"
)

# Loop through the combinations
for combo in "${COMBINATIONS[@]}"; do
  # Read the values from the combo string
  read -r sgMethod bgMethod useDP useNano useDY useQCD <<< "$combo"

  if [[ ($useDP -eq 1 || $useNano -eq 1) && ($useDY -eq 1 || $useQCD -eq 1) ]]; then
      # Call the MLTrain function with the current combination of options
      echo "Running MLTrain with sgMethod=$sgMethod, bgMethod=$bgMethod, useDP=$useDP, useNano=$useNano, useDY=$useDY, useQCD=$useQCD"
      root -l -q "MLTrain.C(\"$sgMethod\", \"$bgMethod\", $useDP, $useNano, $useDY, $useQCD)"

      METHODNAME="TMVA_sg${sgMethod}_bf${bgMethod}_${useDP}_${useNano}_${useDY}_${useQCD}"
      FILENAME="${METHODNAME}.root"

      # BDT rerun file
      cp TMVA.root TMVAOutputs
      mv TMVAOutputs/TMVA.root TMVAOutputs/$FILENAME

      # BDT weight files
      mkdir tempTMVAWeights/$METHODNAME
      cp dataset/weights/TMVAClassification_BDT.weights.xml tempTMVAWeights/$METHODNAME
      cp dataset/weights/TMVAClassification_BDT.class.C tempTMVAWeights/$METHODNAME
  else
    echo "Skipping combination: sgMethod=$sgMethod, bgMethod=$bgMethod, useDP=$useDP, useNano=$useNano, useDY=$useDY, useQCD=$useQCD"
  fi
done
