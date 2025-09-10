#!/bin/bash

# Usage:
# ./run_cleanfile_all.sh /path/to/input_dir /path/to/output_dir

INPUT_DIR="$1"
OUTPUT_DIR="$2"
CLEAN_SCRIPT_PATH="CRAB/cleanfile.C"

if [[ -z "$INPUT_DIR" || -z "$OUTPUT_DIR" ]]; then
    echo "Usage: $0 <input_directory> <output_directory>"
    exit 1
fi

if [[ ! -f "$CLEAN_SCRIPT_PATH" ]]; then
    echo "Error: cleanfile.C not found at $CLEAN_SCRIPT_PATH"
    exit 2
fi

mkdir -p "$OUTPUT_DIR"

for file in "$INPUT_DIR"/*.root; do
    filename=$(basename "$file")
    output_file="$OUTPUT_DIR/$filename"

    echo "Processing $filename..."

    root -l -b -q "$CLEAN_SCRIPT_PATH(\"$file\", \"$output_file\")"

    echo "Finished $filename -> $output_file"
done

echo "All files processed."
