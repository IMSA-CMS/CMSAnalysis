for d in textfiles/SingleMassSnowmass/*; do
	for f in $d/*; do
		if [ -f "$f" ]; then
			# echo $f
			filename="${f##*/}"
			filename="${filename%.*}"
			# echo $filename
			runAnalyzer input=$f output=fullRunRoots/${filename}.root
		fi
	done 
done