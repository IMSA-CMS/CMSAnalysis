                                         
#!/bin/bash

# Update repo

mkdir /eos/uscms/store/user/jpalamad/condor_utils/temp_repo
git clone git@github.com:IMSA-CMS/CMSAnalysis.git /eos/uscms/store/user/jpalamad/condor_utils/temp_repo
tar -czf uncompiled.tar.gz /eos/uscms/store/user/jpalamad/condor_utils/temp_repo/CMSAnalysis
rm -rf /eos/uscms/store/user/jpalamad/condor_utils/temp_repo

# Fetch input files

cp -r /uscms/home/jpalamad/analysis/CMSSW_14_0_4/src/CMSAnalysis/Output/MLStrip_numFiles1 ./condor_input
tar -czf condor_input.tar.gz ./condor_input
rm -rf ./condor_input

condor_submit $1

# clean up request files

rm uncompiled.tar.gz
rm condor_input.tar.gz
rm crab_bdt_request.jdl
rm execute_condor.sh
rm training_shell.sh
