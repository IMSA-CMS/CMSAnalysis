i=36
echo "Current Mu pT cut: $i"
pathName="DarkPhoton_MLEval_MuPtCut_$i/"

dy=(
    "Drell-Yan/Drell-Yan_MassCut_10-50_Run_2.txt"
    "Drell-Yan/Drell-Yan_MassCut_50-inf_Run_2.txt"
)

echo "Beginning Drell-Yan"

for file in "${dy[@]}"; do
    outputFile="${file%.txt}.root"
    outputFile="${outputFile/Drell-Yan\//}"  # Remove Drell-Yan/ from the output filename if necessary
    nohupOutput="${outputFile%.root}"  # Strip the .root extension for nohup output filename
    nohup runAnalyzer input=$file output=${pathName}${outputFile} analysis=LeptonJetReconstruction numFiles=5 outputEvery=1000 > nohup_${nohupOutput}_${i}.out &
done

wait  # Wait for all background processes to finish

echo "Completed Drell-Yan"
