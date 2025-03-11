# use this in EOS area for combining jobs
# example usage: [path]/hadd.sh Higgs1000
# will need to chmod hadd.sh for permissions
echo combining $1/
find $1/Tau/ -type f -name "*.root" > $1/files.txt
hadd -f $1/$1.root @$1/files.txt
cp $1/$1.root $CMSSW_BASE/src/CMSAnalysis/Output/Higgs/
