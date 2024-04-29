runAnalyzer input=/eos/uscms/store/user/jpalamad/input_10000_pSet3.root output=/eos/uscms/store/user/jpalamad/output/output_10000_pSet3.root analysis=GenSim

for i in $(seq 5 9);
do
    runAnalyzer input=/eos/uscms/store/user/jpalamad/input_10000_pSet$i.root output=/eos/uscms/store/user/jpalamad/output/output_10000_pSet$i.root analysis=GenSim
done