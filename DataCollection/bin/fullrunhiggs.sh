for f in textfiles/SingleMassSnowmass/Higgs/*; do
	if [ -f "$f" ]; then
		# echo $f
		filename="${f##*/}"
		filename="${filename%.*}"
		# echo $filename
		runAnalyzer input=$f output=higgsRunRoots/${filename}.root
	fi
done