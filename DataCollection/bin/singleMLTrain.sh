# Call the MLTrain function with the current combination of options
sgMethod="Uniform"
bgMethod="PropWeight"
useDP=1
useNano=0
useDY=0
useQCD=1

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