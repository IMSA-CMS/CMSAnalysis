#!/usr/bin/env bash
# superRunAnalyzer.sh
# Usage: superRunAnalyzer.sh [analysis user path]
# If arguments not provided, the script will prompt for them.

set -euo pipefail

usage() {
	cat <<EOF
Usage:
	$0 <analysis> <user> <path>
Or run without args and you will be prompted.
EOF
	exit 1
}

# Helper: trim whitespace
_trim() { printf '%s' "$1" | awk '{$1=$1;print}'; }

# Read args or prompt
if [ "$#" -eq 3 ]; then
	analysis="$1"
	user="$2"
	path="$3"
elif [ "$#" -eq 0 ]; then
	read -r -p "Analysis name: " analysis
	read -r -p "User name: " user
	read -r -p "Path: " path
else
	usage
fi

analysis=$(_trim "$analysis")
user=$(_trim "$user")
path=$(_trim "$path")

# Basic validation
if [ -z "$analysis" ] || [ -z "$user" ] || [ -z "$path" ]; then
	echo "Error: analysis, user, and path must be provided."
	usage
fi

nohup python3 multiRunAnalyzer.py --crab --analysis "$analysis" --user "$user" --path "$path" --type "MC"
nohup python3 multiRunAnalyzer.py --crab --analysis "$analysis" --user "$user" --path "$path" --type "Data"
nohup ./HiggsSignalProcessor.sh

mv /uscms/home/bhenning/Analysis/CMSSW_15_0_4/src/CMSAnalysis/Output/Higgs*.root /uscms/home/bhenning/Analysis/CMSSW_15_0_4/src/CMSAnalysis/Output/"$path"/

exit 0
