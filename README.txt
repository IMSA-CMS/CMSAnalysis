Author/Philanthropist: Ayan Mallik

The combinedGenRecoAnalyzer takes input text files from the CMS paper to produce root files that can later be plotted. When you run the program, you can choose which combination of mass cuts/lamda/etc that you want to use as input files. Here are the following options:

Year:	2016	2017
Lepton:	E	Mu
Mass:	800	1300	1800	2000
Lamda(2016 options):	16	22	34
Lamda(2017 options):	16	24	32	40	100k
Interference:	Con	Des
Helicity:	LL	LR	RR

You can choose multiple options within the same field (i.e. both constructive and destructive interference using the same tab structure that I exemplified above. The program is whitespace-sensitive, so don't add unnecessary spaces.	  

To create the root file using your chosen text files, type this command: combinedGenRecoAnalyzer. The default name of the output file is genOutput.root, but you can specify a different name so that you don't overwrite same-named output files. To specify, add the following to the command: output = outputFileName (no .txt neccesary).
