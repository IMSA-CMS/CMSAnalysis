import sys
import os

# A simple program that displays the contents of a dataset in a text file. 
# Enter the 1) name of the dataset and the 2) name of the text file. 
# Example command: python3 viewDataset.py /DYJetsToLL*/*/MINIAODSIM datasetContents.txt

# Make updates:
# First run version of dataset dataset, capture output as list, if there's only one thing as list, run file dataset. 
# If there is more than one thing, say here's all possible datasets and print it out. 
# Use the right call that lets us get the output.

os.system('dasgoclient -query="file dataset=' + sys.argv[1] + '" > ' + sys.argv[2]) 

