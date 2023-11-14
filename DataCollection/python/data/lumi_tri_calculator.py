#calculates luminosities and triggers of good run data

#Install Bricalc if you haven't
#pip uninstall brilws
#pip install --install-option="--prefix=$HOME/.local" brilws

import os

req = "export PATH=$HOME/.local/bin:/afs/cern.ch/cms/lumi/brilconda-1.1.7/bin:$PATH"
os.system(req)
# triggers=["HLT_IsoMu24_v7",
# "HLT_Ele27_WPTight_Gsf_v10" , 
# "HLT_Ele23_Ele12_CaloIdL_TrackIdL_IsoVL_v12" , 
# "HLT_Mu17_TrkIsoVVL_Mu8_TrkIsoVVL_v7", 
# "HLT_Mu8_TrkIsoVVL_Ele23_CaloIdL_TrackIdL_IsoVL"]

luminosities=[
"Cert_271036-284044_13TeV_Legacy2016_Collisions16_JSON.txt",
"Cert_294927-306462_13TeV_UL2017_Collisions17_GoldenJSON.txt",
"Cert_314472-325175_13TeV_Legacy2018_Collisions18_JSON.txt",
"2016PostAPV.txt"
]

for lumi in luminosities:
	for trigger in triggers:
		req = "brilcalc lumi -c web -i", lumi, "-u /fb >",lumi, ".out"
		os.system(req)

# req = "brilcalc lumi -c web -i Cert_314472-325175_13TeV_Legacy2018_Collisions18_JSON.txt -u /fb"
# os.system(req)

# req = "brilcalc trg -c web -r 321773 --prescale --hltpath=\"HLT_Ele27*\" > trigger.out"
# os.system(req)

#2016 Run List APV Change 281613 - Good Start for the split