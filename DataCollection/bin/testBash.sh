# Define paths
TMVA_WEIGHTS="/eos/uscms/store/user/jpalamad/rootBackups/MLWeightsBackup"
DATASET_WEIGHTS="dataset/weights"
TEMP_BACKUP="temp_backup"

# Loop through each model in TMVAWeights
for MODEL_DIR in "$TMVA_WEIGHTS"/*/; do
    folder_name=$(basename "$MODEL_DIR")
    echo $folder_name
done