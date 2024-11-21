
# /input directory:
Used to mimic all our input files, starting with the root CMSSW_VERSION
CRAB will only include the paths to the right of whatever you inputted, so if you include
'DataCollection/bin/textfiles/', it will dump all the files at the root directory, when we
really need a path that looks like '$CMSSW_BASE/src/CMSAnalysis/DataCollection/...

This folder including the input files in that same format in order for the created tarballs
not to be ~100MB big.
Any new input files will need to be added here as well in order to run CRAB jobs on them.

This is an example command to copy over all .txt files into this input directory:
> This will look weird when viewed as markdown cuz the \*'s get converted into italics
rsync -am --include='*.txt' --include='*/' --exclude='*' DataCollection/bin/textfiles CRAB/input/src/CMSAnalysis/DataCollection/bin
