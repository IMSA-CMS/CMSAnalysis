#!/bin/sh
source /cvmfs/cms.cern.ch/cmsset_default.sh
cmsenv
echo runAnalyzer "$1" "$2" "$3"
