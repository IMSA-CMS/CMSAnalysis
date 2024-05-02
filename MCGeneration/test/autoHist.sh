#for i in $(seq 10 13);
#do
#    runAnalyzer input=/eos/uscms/store/user/jpalamad/input_10000_pSet$i.root output=/eos/uscms/store/user/rmandell/output/output_10000_pSet$i.root analysis=GenSim
#done

#runAnalyzer input=/eos/uscms/store/user/jpalamad/input_10000_pSet15.root output=/eos/uscms/store/user/rmandell/output/output_10000_pSet15.root analysis=GenSim
runAnalyzer input=/uscms/home/rmandell/analysis/CMSSW_12_4_3/src/CMSAnalysis/MCGeneration/test/PDF14Data.root output=/eos/uscms/store/user/rmandell/output/output_10000_pSet14.root analysis=GenSim
for i in $(seq 18 22);
do
    runAnalyzer input=/uscms/home/rmandell/analysis/CMSSW_12_4_3/src/CMSAnalysis/MCGeneration/test/PDF${i}Data.root output=/eos/uscms/store/user/rmandell/output/output_10000_pSet$i.root analysis=GenSim
done