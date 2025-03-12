                                         
#!/bin/bash

# clean up request files (if necessary)
# rm uncompiled.tar.gz
# rm condor_input.tar.gz
# rm crab_bdt_request.jdl
# rm execute_condor.sh
# rm training_shell.sh

# Check if repo update is required
if [ "$1" != "useCachedRepo" ]; then
    echo "Updating repository..."
    mkdir -p /eos/uscms/store/user/jpalamad/condor_utils
    git clone git@github.com:IMSA-CMS/CMSAnalysis.git /eos/uscms/store/user/jpalamad/condor_utils/temp_repo/CMSAnalysis
    cd /eos/uscms/store/user/jpalamad/condor_utils/temp_repo/CMSAnalysis
    git pull
    
    cd -
    tar -czf uncompiled.tar.gz -C /eos/uscms/store/user/jpalamad/condor_utils/temp_repo CMSAnalysis
    rm -rf /eos/uscms/store/user/jpalamad/condor_utils/temp_repo
else
    echo "Using cached repository (no git update)"
fi

# Fetch input files
cp -r /uscms/home/jpalamad/analysis/CMSSW_14_0_4/src/CMSAnalysis/Output/MLStrip_numFiles20 ./condor_input
tar -czf condor_input.tar.gz ./condor_input
rm -rf ./condor_input

condor_submit $2
    