#!/bin/bash

python3 multiRunAnalyzer.py DPBackground
python3 multiRunAnalyzer.py DPData keep
python3 multiRunAnalyzer.py DarkPhoton keep

# run dy numfiles=3 separately