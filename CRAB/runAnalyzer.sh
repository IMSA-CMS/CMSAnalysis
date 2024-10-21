cmsenv

# Testing
find . -type f
echo 'CMSSW_BASE:'
echo $CMSSW_BASE
ls -a /

# Testing
find . -type f > simpleoutput.txt
echo 'CMSSW_BASE:' > simpleoutput.txt
echo $CMSSW_BASE > simpleoutput.txt
ls -a / > simpleoutput.txt

runAnalyzer input=ZZ/ZZ_Decay_2e2mu_Run_2.txt output=test.root numFiles=3 analysis=HiggsBackground

# Testing
ls -a / > simpleoutput.txt