import re

# Read the results file
with open("WindowAnalysis_results.txt", "r") as f:
    content = f.read()

# Look for cases where we can trace sensitivity across window sizes
# Since we optimized, we only see the best window. We need to modify the code
# to output all tested windows to verify the peak.

print("The current output only shows the BEST window, not all tested windows.")
print("We need to modify the code to output sensitivity for ALL window sizes")
print("to verify that 150 GeV is actually a peak and not just the max tested.")
