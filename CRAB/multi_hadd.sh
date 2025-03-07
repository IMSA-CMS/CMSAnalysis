#!/bin/bash

# Check if argument is provided
if [ -z "$1" ]; then
    echo "Usage: $0 <folder_name>"
    exit 1
fi

OUTPUT_DIR="Output/$1"
mkdir -p "$OUTPUT_DIR"

for dir in */; do
    dir=${dir%/}  # Remove trailing slash
    echo "Combining $dir/"
    
    # Ensure directory structure exists
    mkdir -p "$dir/Tau"
    
    # Find and list root files
    find "$dir/Tau/" -type f -name "*.root" > "$dir/files.txt"
    
    # Combine files
    hadd -f "$dir/$dir.root" @"$dir/files.txt"
    
    # Copy output
    mkdir -p "$CMSSW_BASE/src/CMSAnalysis/$OUTPUT_DIR/MLStrip_numFiles20/"
    cp "$dir/$dir.root" "$CMSSW_BASE/src/CMSAnalysis/$OUTPUT_DIR/MLStrip_numFiles20/"
done
