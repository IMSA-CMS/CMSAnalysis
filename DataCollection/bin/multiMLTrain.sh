#!/bin/bash

# Define the available options
SG_METHODS=("PropWeight" "Uniform")
BG_METHODS=("PropWeight" "Uniform")
USE_OPTIONS=(0 1)

# Loop through all combinations of the options
for sgMethod in "${SG_METHODS[@]}"; do
  for bgMethod in "${BG_METHODS[@]}"; do
    for useDP in "${USE_OPTIONS[@]}"; do
      for useNano in "${USE_OPTIONS[@]}"; do
        for useDY in "${USE_OPTIONS[@]}"; do
          for useQCD in "${USE_OPTIONS[@]}"; do

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
                mkdir TMVAWeights/$METHODNAME
                cp dataset/weights/TMVAClassification_BDT.weights.xml TMVAWeights/$METHODNAME
                cp dataset/weights/TMVAClassification_BDT.class.C TMVAWeights/$METHODNAME
            else
              echo "Skipping combination: sgMethod=$sgMethod, bgMethod=$bgMethod, useDP=$useDP, useNano=$useNano, useDY=$useDY, useQCD=$useQCD"
            fi
          done
        done
      done
    done
  done
done