#!/bin/bash

# InvariantMassPlot Setup and Execution Script
# Run this script on the LPC cluster after SSH'ing with: cms
# Usage: bash run_invariant_mass_plot.sh

set -e

SCRIPT_DIR="$( cd "$( dirname "${BASH_SOURCE[0]}" )" && pwd )"
cd "$SCRIPT_DIR"

echo "=========================================="
echo "Invariant Mass Distribution Plot Generator"
echo "=========================================="
echo ""
echo "This script will:"
echo "  1. Set up the CMSSW 15.0.4 environment"
echo "  2. Compile the InvariantMassPlot.C macro"
echo "  3. Generate InvariantMassPlot.png and .pdf"
echo ""

# Set up CMSSW environment
cd /home/jjones/lpc/analysis/CMSSW_15_0_4/src
eval `scram runtime -sh`

# Return to the script directory
cd "$SCRIPT_DIR"

# Run the ROOT macro
echo "Compiling and executing macro..."
root -b -q 'InvariantMassPlot.C+'

echo ""
echo "=========================================="
echo "Plot generation complete!"
echo "=========================================="
echo ""
echo "Output files created:"
ls -lh InvariantMassPlot.png InvariantMassPlot.pdf 2>/dev/null || echo "  (files not found - check for errors above)"
echo ""
