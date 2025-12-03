#!/usr/bin/env bash
# HiggsSignalProcessor.sh
# Run runanalyzer (or the ROOT macro runanalyzer.cc) for all run2pick files whose names start with "Higgs".
set -euo pipefail

cmsenv

# Search current directory for files starting with "Higgs" and containing "run2pick"
# Use Bash globbing to populate an array; avoid leaving the pattern if no match
shopt -s nullglob
files=()
#for f in textfiles/Run2PickFiles/Higgs*.txt; do 
#	files+=("${f#textfiles/}")
#done

# Add only Higgs500..Higgs900 files (avoid duplicates)
for n in 900; do
    for f in textfiles/Run2PickFiles/Higgs${n}*.txt; do
        rel="${f#textfiles/}"
        # skip if already present
        found=0
        for e in "${files[@]}"; do
            # POSIX-compatible test (works under sh and bash)
            if [ "$e" = "$rel" ]; then
                found=1
                break
            fi
        done
        # append if not found
        if [ "$found" -eq 0 ]; then
            files+=("$rel")
        fi
    done
done
shopt -u nullglob

if [ ${#files[@]} -eq 0 ]; then
	echo "No run2pick files starting with 'Higgs' found in $(pwd)."
	exit 0
fi


for f in "${files[@]}"; do
    echo "=== Processing: $f ==="
    outfile="$(basename "$f")"
    outfile="../../../../../nobackup/${outfile%.txt}.root"
    echo "=== Writing to: $outfile ==="
    nohup runAnalyzer input="$f" output="$outfile" analysis=HiggsSignal
done