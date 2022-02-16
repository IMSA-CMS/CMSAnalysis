import os;

dy_muons_2016 = [

 	    ('DYMuM50to120', '/ZToMuMu_NNPDF30_13TeV-powheg_M_50_120/RunIISummer16MiniAODv2-PUMoriond17_80X_mcRun2_asymptotic_2016_TrancheIV_v6-v1/MINIAODSIM'),
            ('DYMuM120to200', '/ZToMuMu_NNPDF30_13TeV-powheg_M_120_200/RunIISummer16MiniAODv2-PUMoriond17_80X_mcRun2_asymptotic_2016_TrancheIV_v6-v1/MINIAODSIM'),
 	    ('DYMuM200to400', '/ZToMuMu_NNPDF30_13TeV-powheg_M_200_400/RunIISummer16MiniAODv2-PUMoriond17_80X_mcRun2_asymptotic_2016_TrancheIV_v6-v1/MINIAODSIM'),
            ('DYMuM400to800', '/ZToMuMu_NNPDF30_13TeV-powheg_M_400_800/RunIISummer16MiniAODv2-PUMoriond17_80X_mcRun2_asymptotic_2016_TrancheIV_v6-v1/MINIAODSIM'),
 	    ('DYMuM800to1400', '/ZToMuMu_NNPDF30_13TeV-powheg_M_800_1400/RunIISummer16MiniAODv2-PUMoriond17_80X_mcRun2_asymptotic_2016_TrancheIV_v6-v1/MINIAODSIM'),
            ('DYMuM1400to2300', '/ZToMuMu_NNPDF30_13TeV-powheg_M_1400_2300/RunIISummer16MiniAODv2-PUMoriond17_80X_mcRun2_asymptotic_2016_TrancheIV_v6-v1/MINIAODSIM'),
            ('DYMuM2300to3500', '/ZToMuMu_NNPDF30_13TeV-powheg_M_2300_3500/RunIISummer16MiniAODv2-PUMoriond17_80X_mcRun2_asymptotic_2016_TrancheIV_v6-v1/MINIAODSIM'),
            ('DYMuM3500to4500', '/ZToMuMu_NNPDF30_13TeV-powheg_M_3500_4500/RunIISummer16MiniAODv2-PUMoriond17_80X_mcRun2_asymptotic_2016_TrancheIV_v6-v1/MINIAODSIM'),
            ('DYMuM4500to6000', '/ZToMuMu_NNPDF30_13TeV-powheg_M_4500_6000/RunIISummer16MiniAODv2-PUMoriond17_80X_mcRun2_asymptotic_2016_TrancheIV_v6-v1/MINIAODSIM'),
            ('DYMuM6000toInf', '/ZToMuMu_NNPDF30_13TeV-powheg_M_6000_Inf/RunIISummer16MiniAODv2-PUMoriond17_80X_mcRun2_asymptotic_2016_TrancheIV_v6-v1/MINIAODSIM')
]  
   	
dy_muons_2017 = [

 	    ('DYMuM50to120', '/ZToMuMu_NNPDF31_13TeV-powheg_M_50_120/RunIIFall17MiniAODv2-PU2017_12Apr2018_94X_mc2017_realistic_v14-v2/MINIAODSIM'),
            ('DYMuM120to200', '/ZToMuMu_NNPDF31_13TeV-powheg_M_120_200/RunIIFall17MiniAODv2-PU2017_12Apr2018_94X_mc2017_realistic_v14-v1/MINIAODSIM'),
 	    ('DYMuM200to400', '/ZToMuMu_NNPDF31_13TeV-powheg_M_200_400/RunIIFall17MiniAODv2-PU2017_12Apr2018_94X_mc2017_realistic_v14-v1/MINIAODSIM'),
            ('DYMuM400to800', '/ZToMuMu_NNPDF31_13TeV-powheg_M_400_800/RunIIFall17MiniAODv2-PU2017_12Apr2018_94X_mc2017_realistic_v14-v1/MINIAODSIM'),
 	    ('DYMuM800to1400', '/ZToMuMu_NNPDF31_13TeV-powheg_M_800_1400/RunIIFall17MiniAODv2-PU2017_12Apr2018_94X_mc2017_realistic_v14-v1/MINIAODSIM'),
            ('DYMuM1400to2300', '/ZToMuMu_NNPDF31_13TeV-powheg_M_1400_2300/RunIIFall17MiniAODv2-PU2017_12Apr2018_94X_mc2017_realistic_v14-v1/MINIAODSIM'),
            ('DYMuM2300to3500', '/ZToMuMu_NNPDF31_13TeV-powheg_M_2300_3500/RunIIFall17MiniAODv2-PU2017_12Apr2018_94X_mc2017_realistic_v14-v1/MINIAODSIM'),
            ('DYMuM3500to4500', '/ZToMuMu_NNPDF31_13TeV-powheg_M_3500_4500/RunIIFall17MiniAODv2-PU2017_12Apr2018_94X_mc2017_realistic_v14-v1/MINIAODSIM'),
            ('DYMuM4500to6000', '/ZToMuMu_NNPDF31_13TeV-powheg_M_4500_6000/RunIIFall17MiniAODv2-PU2017_12Apr2018_94X_mc2017_realistic_v14-v1/MINIAODSIM'),
            ('DYMuM6000toInf', '/ZToMuMu_NNPDF31_13TeV-powheg_M_6000_Inf/RunIIFall17MiniAODv2-PU2017_12Apr2018_94X_mc2017_realistic_v14-v1/MINIAODSIM')
]
     	
dy_muons_2018 = [

    ('DYMuM50to120', '/ZToMuMu_NNPDF31_13TeV-powheg_M_50_120/RunIIAutumn18MiniAOD-102X_upgrade2018_realistic_v15-v2/MINIAODSIM'),
    ('DYMuM120to200', '/ZToMuMu_NNPDF31_13TeV-powheg_M_120_200/RunIIAutumn18MiniAOD-102X_upgrade2018_realistic_v15-v2/MINIAODSIM'),
    ('DYMuM200to400', '/ZToMuMu_NNPDF31_13TeV-powheg_M_200_400/RunIIAutumn18MiniAOD-102X_upgrade2018_realistic_v15-v2/MINIAODSIM'),
    ('DYMuM400to800', '/ZToMuMu_NNPDF31_13TeV-powheg_M_400_800/RunIIAutumn18MiniAOD-102X_upgrade2018_realistic_v15-v2/MINIAODSIM'),
    ('DYMuM800to1400', '/ZToMuMu_NNPDF31_13TeV-powheg_M_800_1400/RunIIAutumn18MiniAOD-102X_upgrade2018_realistic_v15-v1/MINIAODSIM'),
    ('DYMuM1400to2300', '/ZToMuMu_NNPDF31_13TeV-powheg_M_1400_2300/RunIIAutumn18MiniAOD-102X_upgrade2018_realistic_v15-v2/MINIAODSIM'),
    ('DYMuM2300to3500', '/ZToMuMu_NNPDF31_13TeV-powheg_M_2300_3500/RunIIAutumn18MiniAOD-102X_upgrade2018_realistic_v15-v2/MINIAODSIM'),
    ('DYMuM3500to4500', '/ZToMuMu_NNPDF31_13TeV-powheg_M_3500_4500/RunIIAutumn18MiniAOD-102X_upgrade2018_realistic_v15-v2/MINIAODSIM'),
    ('DYMuM4500to6000', '/ZToMuMu_NNPDF31_13TeV-powheg_M_4500_6000/RunIIAutumn18MiniAOD-102X_upgrade2018_realistic_v15-v2/MINIAODSIM'),
    ('DYMuM6000toInf', '/ZToMuMu_NNPDF31_13TeV-powheg_M_6000_Inf/RunIIAutumn18MiniAOD-102X_upgrade2018_realistic_v15-v2/MINIAODSIM')

]

dy_electrons_2016 = [

 	    ('DYEM50to120', '/ZToEE_NNPDF30_13TeV-powheg_M_50_120/RunIISummer16MiniAODv2-PUMoriond17_80X_mcRun2_asymptotic_2016_TrancheIV_v6-v1/MINIAODSIM'),
            ('DYEM120to200', '/ZToEE_NNPDF30_13TeV-powheg_M_120_200/RunIISummer16MiniAODv2-PUMoriond17_80X_mcRun2_asymptotic_2016_TrancheIV_v6-v1/MINIAODSIM'),
 	    ('DYEM200to400', '/ZToEE_NNPDF30_13TeV-powheg_M_200_400/RunIISummer16MiniAODv2-PUMoriond17_80X_mcRun2_asymptotic_2016_TrancheIV_v6-v1/MINIAODSIM'),
            ('DYEM400to800', '/ZToEE_NNPDF30_13TeV-powheg_M_400_800/RunIISummer16MiniAODv2-PUMoriond17_80X_mcRun2_asymptotic_2016_TrancheIV_v6-v1/MINIAODSIM'),
 	    ('DYEM800to1400', '/ZToEE_NNPDF30_13TeV-powheg_M_800_1400/RunIISummer16MiniAODv2-PUMoriond17_80X_mcRun2_asymptotic_2016_TrancheIV_v6-v1/MINIAODSIM'),
            ('DYEM1400to2300', '/ZToEE_NNPDF30_13TeV-powheg_M_1400_2300/RunIISummer16MiniAODv2-PUMoriond17_80X_mcRun2_asymptotic_2016_TrancheIV_v6-v1/MINIAODSIM'),
            ('DYEM2300to3500', '/ZToEE_NNPDF30_13TeV-powheg_M_2300_3500/RunIISummer16MiniAODv2-PUMoriond17_80X_mcRun2_asymptotic_2016_TrancheIV_v6-v1/MINIAODSIM'),
            ('DYEM3500to4500', '/ZToEE_NNPDF30_13TeV-powheg_M_3500_4500/RunIISummer16MiniAODv2-PUMoriond17_80X_mcRun2_asymptotic_2016_TrancheIV_v6-v1/MINIAODSIM'),
            ('DYEM4500to6000', '/ZToEE_NNPDF30_13TeV-powheg_M_4500_6000/RunIISummer16MiniAODv2-PUMoriond17_80X_mcRun2_asymptotic_2016_TrancheIV_v6-v1/MINIAODSIM'),
            ('DYEM6000toInf', '/ZToEE_NNPDF30_13TeV-powheg_M_6000_Inf/RunIISummer16MiniAODv2-PUMoriond17_80X_mcRun2_asymptotic_2016_TrancheIV_v6-v1/MINIAODSIM')
]

dy_electrons_2017 = [

 	    ('DYEM50to120', '/ZToEE_NNPDF31_13TeV-powheg_M_50_120/RunIIFall17MiniAODv2-PU2017_12Apr2018_94X_mc2017_realistic_v14-v1/MINIAODSIM'),
            ('DYEM120to200', '/ZToEE_NNPDF31_13TeV-powheg_M_120_200/RunIIFall17MiniAODv2-PU2017_12Apr2018_94X_mc2017_realistic_v14-v1/MINIAODSIM'),
 	    ('DYEM200to400', '/ZToEE_NNPDF31_13TeV-powheg_M_200_400/RunIIFall17MiniAODv2-PU2017_12Apr2018_94X_mc2017_realistic_v14-v1/MINIAODSIM'),
            ('DYEM400to800', '/ZToEE_NNPDF31_13TeV-powheg_M_400_800/RunIIFall17MiniAODv2-PU2017_12Apr2018_94X_mc2017_realistic_v14-v1/MINIAODSIM'),
 	    ('DYEM800to1400', '/ZToEE_NNPDF31_13TeV-powheg_M_800_1400/RunIIFall17MiniAODv2-PU2017_12Apr2018_94X_mc2017_realistic_v14-v1/MINIAODSIM'),
            ('DYEM1400to2300', '/ZToEE_NNPDF31_13TeV-powheg_M_1400_2300/RunIIFall17MiniAODv2-PU2017_12Apr2018_94X_mc2017_realistic_v14-v1/MINIAODSIM'),
            ('DYEM2300to3500', '/ZToEE_NNPDF31_13TeV-powheg_M_2300_3500/RunIIFall17MiniAODv2-PU2017_12Apr2018_94X_mc2017_realistic_v14-v1/MINIAODSIM'),
            ('DYEM3500to4500', '/ZToEE_NNPDF31_13TeV-powheg_M_3500_4500/RunIIFall17MiniAODv2-PU2017_12Apr2018_94X_mc2017_realistic_v14-v1/MINIAODSIM'),
            ('DYEM4500to6000', '/ZToEE_NNPDF31_13TeV-powheg_M_4500_6000/RunIIFall17MiniAODv2-PU2017_12Apr2018_94X_mc2017_realistic_v14-v1/MINIAODSIM'),
            ('DYEM6000toInf', '/ZToEE_NNPDF31_13TeV-powheg_M_6000_Inf/RunIIFall17MiniAODv2-PU2017_12Apr2018_94X_mc2017_realistic_v14-v1/MINIAODSIM')
]

dy_electrons_2018 = [

 	    ('DYEM50to120', '/ZToEE_NNPDF31_TuneCP5_13TeV-powheg_M_50_120/RunIIAutumn18MiniAOD-102X_upgrade2018_realistic_v15-v1/MINIAODSIM'),
            ('DYEM120to200', '/ZToEE_NNPDF31_TuneCP5_13TeV-powheg_M_120_200/RunIIAutumn18MiniAOD-102X_upgrade2018_realistic_v15-v1/MINIAODSIM'),
 	    ('DYEM200to400', '/ZToEE_NNPDF31_TuneCP5_13TeV-powheg_M_200_400/RunIIAutumn18MiniAOD-102X_upgrade2018_realistic_v15-v1/MINIAODSIM'),
            ('DYEM400to800', '/ZToEE_NNPDF31_TuneCP5_13TeV-powheg_M_400_800/RunIIAutumn18MiniAOD-102X_upgrade2018_realistic_v15-v1/MINIAODSIM'),
 	    ('DYEM800to1400', '/ZToEE_NNPDF31_TuneCP5_13TeV-powheg_M_800_1400/RunIIAutumn18MiniAOD-102X_upgrade2018_realistic_v15-v1/MINIAODSIM'),
            ('DYEM1400to2300', '/ZToEE_NNPDF31_TuneCP5_13TeV-powheg_M_1400_2300/RunIIAutumn18MiniAOD-102X_upgrade2018_realistic_v15-v1/MINIAODSIM'),
            ('DYEM2300to3500', '/ZToEE_NNPDF31_TuneCP5_13TeV-powheg_M_2300_3500/RunIIAutumn18MiniAOD-102X_upgrade2018_realistic_v15-v1/MINIAODSIM'),
            ('DYEM3500to4500', '/ZToEE_NNPDF31_TuneCP5_13TeV-powheg_M_3500_4500/RunIIAutumn18MiniAOD-102X_upgrade2018_realistic_v15-v1/MINIAODSIM'),
            ('DYEM4500to6000', '/ZToEE_NNPDF31_TuneCP5_13TeV-powheg_M_4500_6000/RunIIAutumn18MiniAOD-102X_upgrade2018_realistic_v15-v1/MINIAODSIM'),
            ('DYEM6000toInf', '/ZToEE_NNPDF31_TuneCP5_13TeV-powheg_M_6000_Inf/RunIIAutumn18MiniAOD-102X_upgrade2018_realistic_v15-v1/MINIAODSIM')
]

dibosons_2016 = [

('WZ', '/WZ_TuneCUETP8M1_13TeV-pythia8/RunIISummer16MiniAODv2-PUMoriond17_80X_mcRun2_asymptotic_2016_TrancheIV_v6-v1/MINIAODSIM'),
('WZ_ext', '/WZ_TuneCUETP8M1_13TeV-pythia8/RunIISummer16MiniAODv2-PUMoriond17_80X_mcRun2_asymptotic_2016_TrancheIV_v6_ext1-v1/MINIAODSIM'),

('ZZ', '/ZZ_TuneCUETP8M1_13TeV-pythia8/RunIISummer16MiniAODv2-PUMoriond17_80X_mcRun2_asymptotic_2016_TrancheIV_v6-v1/MINIAODSIM'),
('ZZ_ext', '/ZZ_TuneCUETP8M1_13TeV-pythia8/RunIISummer16MiniAODv2-PUMoriond17_80X_mcRun2_asymptotic_2016_TrancheIV_v6_ext1-v1/MINIAODSIM'),

]

dibosons_electrons_2016 = [

('WW200to600', '/WWTo2L2Nu_Mll_200To600_13TeV-powheg/RunIISummer16MiniAODv2-PUMoriond17_80X_mcRun2_asymptotic_2016_TrancheIV_v6-v1/MINIAODSIM'),
('WW600to1200', '/WWTo2L2Nu_Mll_600To1200_13TeV-powheg/RunIISummer16MiniAODv2-PUMoriond17_80X_mcRun2_asymptotic_2016_TrancheIV_v6-v1/MINIAODSIM'),
('WW1200to2500', '/WWTo2L2Nu_Mll_1200To2500_13TeV-powheg/RunIISummer16MiniAODv2-PUMoriond17_80X_mcRun2_asymptotic_2016_TrancheIV_v6-v1/MINIAODSIM'),
('WW2500', '/WWTo2L2Nu_Mll_2500ToInf_13TeV-powheg/RunIISummer16MiniAODv2-PUMoriond17_80X_mcRun2_asymptotic_2016_TrancheIV_v6-v1/MINIAODSIM')

]

dibosons_2017 = [

('WZ', '/WZ_TuneCP5_13TeV-pythia8/RunIIFall17MiniAODv2-PU2017_12Apr2018_94X_mc2017_realistic_v14-v1/MINIAODSIM'),
('WZ3LNu', '/WZTo3LNu_TuneCP5_13TeV-amcatnloFXFX-pythia8/RunIIFall17MiniAODv2-PU2017_12Apr2018_94X_mc2017_realistic_v14-v1/MINIAODSIM'),
('WZ2L2Q', '/WZTo2L2Q_13TeV_amcatnloFXFX_madspin_pythia8/RunIIFall17MiniAODv2-PU2017_12Apr2018_94X_mc2017_realistic_v14-v1/MINIAODSIM'),

('ZZ', '/ZZ_TuneCP5_13TeV-pythia8/RunIIFall17MiniAODv2-PU2017_12Apr2018_94X_mc2017_realistic_v14-v1/MINIAODSIM'),
('ZZ4L', '/ZZTo4L_13TeV_powheg_pythia8/RunIIFall17MiniAODv2-PU2017_12Apr2018_94X_mc2017_realistic_v14-v1/MINIAODSIM'),
('ZZ2L2Q', '/ZZTo2L2Q_13TeV_amcatnloFXFX_madspin_pythia8/RunIIFall17MiniAODv2-PU2017_12Apr2018_94X_mc2017_realistic_v14-v1/MINIAODSIM'),
('ZZ2L2Nu', '/ZZTo2L2Nu_13TeV_powheg_pythia8/RunIIFall17MiniAODv2-PU2017_12Apr2018_94X_mc2017_realistic_v14-v1/MINIAODSIM'),

#WW same as 2018
 
]

dibosons_2018 = [

('WZ', '/WZ_TuneCP5_13TeV-pythia8/RunIIAutumn18MiniAOD-102X_upgrade2018_realistic_v15-v3/MINIAODSIM'),
('WZ3LNu', '/WZTo3LNu_TuneCP5_13TeV-powheg-pythia8/RunIIAutumn18MiniAOD-102X_upgrade2018_realistic_v15_ext1-v2/MINIAODSIM'),
('WZ2L2Q', '/WZTo2L2Q_13TeV_amcatnloFXFX_madspin_pythia8/RunIIAutumn18MiniAOD-102X_upgrade2018_realistic_v15-v1/MINIAODSIM'),


('ZZ', '/ZZ_TuneCP5_13TeV-pythia8/RunIIAutumn18MiniAOD-102X_upgrade2018_realistic_v15-v2/MINIAODSIM'),
('ZZ2L2Nu', '/ZZTo2L2Nu_TuneCP5_13TeV_powheg_pythia8/RunIIAutumn18MiniAOD-102X_upgrade2018_realistic_v15_ext1-v2/MINIAODSIM'),
('ZZ2L2Nu_ext', '/ZZTo2L2Nu_TuneCP5_13TeV_powheg_pythia8/RunIIAutumn18MiniAOD-102X_upgrade2018_realistic_v15_ext2-v2/MINIAODSIM'),
('ZZ2L2Q', '/ZZTo2L2Q_13TeV_amcatnloFXFX_madspin_pythia8/RunIIAutumn18MiniAOD-102X_upgrade2018_realistic_v15-v1/MINIAODSIM'),
('ZZ4L', '/ZZTo4L_TuneCP5_13TeV_powheg_pythia8/RunIIAutumn18MiniAOD-102X_upgrade2018_realistic_v15_ext1-v2/MINIAODSIM'),
('ZZ4L_ext', '/ZZTo4L_TuneCP5_13TeV_powheg_pythia8/RunIIAutumn18MiniAOD-102X_upgrade2018_realistic_v15_ext2-v2/MINIAODSIM'),

#WW same as 2017
('M200to600WW', '/WWTo2L2Nu_MLL_200To600_NNPDF31_13TeV-powheg/RunIIFall17MiniAODv2-PU2017_12Apr2018_94X_mc2017_realistic_v14-v1/MINIAODSIM'),
('M600to1200WW', '/WWTo2L2Nu_MLL_600To1200_v3_NNPDF31_13TeV-powheg/RunIIFall17MiniAODv2-PU2017_12Apr2018_94X_mc2017_realistic_v14-v1/MINIAODSIM'),
('M1200to2500WW', '/WWTo2L2Nu_MLL_1200To2500_NNPDF31_13TeV-powheg/RunIIFall17MiniAODv2-PU2017_12Apr2018_94X_mc2017_realistic_v14-v1/MINIAODSIM'),
('M2500toInfWW', '/WWTo2L2Nu_MLL_2500ToInf_NNPDF31_13TeV-powheg/RunIIFall17MiniAODv2-PU2017_12Apr2018_94X_mc2017_realistic_v14-v1/MINIAODSIM'),

('Wjets', '/WJetsToLNu_TuneCP5_13TeV-madgraphMLM-pythia8/RunIIAutumn18MiniAOD-102X_upgrade2018_realistic_v15-v2/MINIAODSIM')

]

tops_electrons__2016 = [

 	('ttbar_lep_500to800', '/TTToLL_MLL_500To800_TuneCUETP8M1_13TeV-powheg-pythia8/RunIISummer16MiniAODv2-PUMoriond17_80X_mcRun2_asymptotic_2016_TrancheIV_v6-v1/MINIAODSIM'),
 	('ttbar_lep_800to1200', '/TTToLL_MLL_800To1200_TuneCUETP8M1_13TeV-powheg-pythia8/RunIISummer16MiniAODv2-PUMoriond17_80X_mcRun2_asymptotic_2016_TrancheIV_v6-v1/MINIAODSIM'),
 	('ttbar_lep_1200to1800', '/TTToLL_MLL_1200To1800_TuneCUETP8M1_13TeV-powheg-pythia8/RunIISummer16MiniAODv2-PUMoriond17_80X_mcRun2_asymptotic_2016_TrancheIV_v6-v1/MINIAODSIM'),
	('ttbar_lep_1800toInf', '/TTToLL_MLL_1800ToInf_TuneCUETP8M1_13TeV-powheg-pythia8/RunIISummer16MiniAODv2-PUMoriond17_80X_mcRun2_asymptotic_2016_TrancheIV_v6-v1/MINIAODSIM'),

	('Wantitop', '/ST_tW_antitop_5f_inclusiveDecays_13TeV-powheg-pythia8_TuneCUETP8M1/RunIISummer16MiniAODv2-PUMoriond17_80X_mcRun2_asymptotic_2016_TrancheIV_v6_ext1-v1/MINIAODSIM'),
    	('tW', '/ST_tW_top_5f_inclusiveDecays_13TeV-powheg-pythia8_TuneCUETP8M1/RunIISummer16MiniAODv2-PUMoriond17_80X_mcRun2_asymptotic_2016_TrancheIV_v6_ext1-v1/MINIAODSIM')

]

tops_muons_2016 = [

    ('ttbar_lep', '/TTTo2L2Nu_TuneCUETP8M2_ttHtranche3_13TeV-powheg-pythia8/RunIISummer16MiniAODv3-PUMoriond17_94X_mcRun2_asymptotic_v3-v2/MINIAODSIM'),
    ('ttbar_lep50to500', '/TTTo2L2Nu_TuneCUETP8M2_ttHtranche3_13TeV-powheg-pythia8/RunIISummer16MiniAODv3-PUMoriond17_94X_mcRun2_asymptotic_v3-v2/MINIAODSIM'),
    ('ttbar_lep_500to800', '/TTToLL_MLL_500To800_TuneCUETP8M1_13TeV-powheg-pythia8/RunIISummer16MiniAODv3-PUMoriond17_94X_mcRun2_asymptotic_v3-v2/MINIAODSIM'),
    ('ttbar_lep_800to1200', '/TTToLL_MLL_800To1200_TuneCUETP8M1_13TeV-powheg-pythia8/RunIISummer16MiniAODv3-PUMoriond17_94X_mcRun2_asymptotic_v3-v1/MINIAODSIM'),
    ('ttbar_lep_1200to1800', '/TTToLL_MLL_1200To1800_TuneCUETP8M1_13TeV-powheg-pythia8/RunIISummer16MiniAODv3-PUMoriond17_94X_mcRun2_asymptotic_v3-v2/MINIAODSIM'),
    ('ttbar_lep1800toInf', '/TTToLL_MLL_1800ToInf_TuneCUETP8M1_13TeV-powheg-pythia8/RunIISummer16MiniAODv3-PUMoriond17_94X_mcRun2_asymptotic_v3-v2/MINIAODSIM'),

    ('Wantitop', '/ST_tW_antitop_5f_inclusiveDecays_13TeV-powheg-pythia8_TuneCUETP8M1/RunIISummer16MiniAODv3-PUMoriond17_94X_mcRun2_asymptotic_v3_ext1-v1/MINIAODSIM'),
    ('tW', '/ST_tW_top_5f_inclusiveDecays_13TeV-powheg-pythia8_TuneCUETP8M1/RunIISummer16MiniAODv3-PUMoriond17_94X_mcRun2_asymptotic_v3_ext1-v1/MINIAODSIM'),

]

tops_2017 = [

   	('ttbar_lep_500to800_ext', '/TTToLL_MLL_500To800_41to65_NNPDF31_13TeV-powheg/RunIIFall17MiniAODv2-PU2017_12Apr2018_94X_mc2017_realistic_v14-v1/MINIAODSIM'),
   	('ttbar_lep_500to800', '/TTToLL_MLL_500To800_0to20_NNPDF31_13TeV-powheg/RunIIFall17MiniAODv2-PU2017_12Apr2018_94X_mc2017_realistic_v14-v1/MINIAODSIM'),
 	('ttbar_lep_800to1200', '/TTToLL_MLL_800To1200_NNPDF31_13TeV-powheg/RunIIFall17MiniAODv2-PU2017_12Apr2018_94X_mc2017_realistic_v14-v1/MINIAODSIM'),
 	('ttbar_lep_1200to1800', '/TTToLL_MLL_1200To1800_NNPDF31_13TeV-powheg/RunIIFall17MiniAODv2-PU2017_12Apr2018_94X_mc2017_realistic_v14-v1/MINIAODSIM'),
	('ttbar_lep_1800toInf','/TTToLL_MLL_1800ToInf_NNPDF31_13TeV-powheg/RunIIFall17MiniAODv2-PU2017_12Apr2018_94X_mc2017_realistic_v14-v1/MINIAODSIM'),

        ('Wantitop', '/ST_tW_antitop_5f_inclusiveDecays_TuneCP5_13TeV-powheg-pythia8/RunIIFall17MiniAODv2-PU2017_12Apr2018_94X_mc2017_realistic_v14-v1/MINIAODSIM'),
     	('tW', '/ST_tW_top_5f_inclusiveDecays_TuneCP5_13TeV-powheg-pythia8/RunIIFall17MiniAODv2-PU2017_12Apr2018_94X_mc2017_realistic_v14-v1/MINIAODSIM')

]

tops_2018 = [

    #same as tops_2017
    ('M500to800ttbar_ext', '/TTToLL_MLL_500To800_41to65_NNPDF31_13TeV-powheg/RunIIFall17MiniAODv2-PU2017_12Apr2018_94X_mc2017_realistic_v14-v1/MINIAODSIM'),
    ('M500to800ttbar', '/TTToLL_MLL_500To800_0to20_NNPDF31_13TeV-powheg/RunIIFall17MiniAODv2-PU2017_12Apr2018_94X_mc2017_realistic_v14-v1/MINIAODSIM'),
    ('M800to1200ttbar', '/TTToLL_MLL_800To1200_NNPDF31_13TeV-powheg/RunIIFall17MiniAODv2-PU2017_12Apr2018_94X_mc2017_realistic_v14-v1/MINIAODSIM'),
    ('M1200to1800ttbar', '/TTToLL_MLL_1200To1800_NNPDF31_13TeV-powheg/RunIIFall17MiniAODv2-PU2017_12Apr2018_94X_mc2017_realistic_v14-v1/MINIAODSIM'),
    ('M1800toInfttbar','/TTToLL_MLL_1800ToInf_NNPDF31_13TeV-powheg/RunIIFall17MiniAODv2-PU2017_12Apr2018_94X_mc2017_realistic_v14-v1/MINIAODSIM'),

    ('Wantitop', '/ST_tW_antitop_5f_NoFullyHadronicDecays_TuneCP5_13TeV-powheg-pythia8/RunIIAutumn18MiniAOD-102X_upgrade2018_realistic_v15_ext1-v3/MINIAODSIM'),
    ('tW', '/ST_tW_top_5f_NoFullyHadronicDecays_TuneCP5_13TeV-powheg-pythia8/RunIIAutumn18MiniAOD-102X_upgrade2018_realistic_v15_ext1-v3/MINIAODSIM'),

]

qcd_2016 = [

('qcd50to80', '/QCD_Pt_50to80_TuneCUETP8M1_13TeV_pythia8/RunIISummer16MiniAODv2-PUMoriond17_80X_mcRun2_asymptotic_2016_TrancheIV_v6-v1/MINIAODSIM'),
('qcd80to120', '/QCD_Pt_80to120_TuneCUETP8M1_13TeV_pythia8/RunIISummer16MiniAODv2-PUMoriond17_80X_mcRun2_asymptotic_2016_TrancheIV_v6-v1/MINIAODSIM'),
('qcd120to170', '/QCD_Pt_120to170_TuneCUETP8M1_13TeV_pythia8/RunIISummer16MiniAODv2-PUMoriond17_80X_mcRun2_asymptotic_2016_TrancheIV_v6-v1/MINIAODSIM'),
('qcd170to300', '/QCD_Pt_170to300_TuneCUETP8M1_13TeV_pythia8/RunIISummer16MiniAODv2-PUMoriond17_80X_mcRun2_asymptotic_2016_TrancheIV_v6-v1/MINIAODSIM'),
('qcd300to470', '/QCD_Pt_300to470_TuneCUETP8M1_13TeV_pythia8/RunIISummer16MiniAODv2-PUMoriond17_80X_mcRun2_asymptotic_2016_TrancheIV_v6-v1/MINIAODSIM'),
('qcd470to600', '/QCD_Pt_470to600_TuneCUETP8M1_13TeV_pythia8/RunIISummer16MiniAODv2-PUMoriond17_80X_mcRun2_asymptotic_2016_TrancheIV_v6-v1/MINIAODSIM'),
('qcd600to800', '/QCD_Pt_600to800_TuneCUETP8M1_13TeV_pythia8/RunIISummer16MiniAODv2-PUMoriond17_80X_mcRun2_asymptotic_2016_TrancheIV_v6-v1/MINIAODSIM'),
('qcd800to1000', '/QCD_Pt_800to1000_TuneCUETP8M1_13TeV_pythia8/RunIISummer16MiniAODv2-PUMoriond17_80X_mcRun2_asymptotic_2016_TrancheIV_v6-v1/MINIAODSIM'),
('qcd1000to1400', '/QCD_Pt_1000to1400_TuneCUETP8M1_13TeV_pythia8/RunIISummer16MiniAODv2-PUMoriond17_80X_mcRun2_asymptotic_2016_TrancheIV_v6-v1/MINIAODSIM'),
('qcd1400to1800', '/QCD_Pt_1400to1800_TuneCUETP8M1_13TeV_pythia8/RunIISummer16MiniAODv2-PUMoriond17_80X_mcRun2_asymptotic_2016_TrancheIV_v6-v1/MINIAODSIM'),
('qcd1800to2400', '/QCD_Pt_1800to2400_TuneCUETP8M1_13TeV_pythia8/RunIISummer16MiniAODv2-PUMoriond17_80X_mcRun2_asymptotic_2016_TrancheIV_v6-v1/MINIAODSIM'),
('qcd2400to3200', '/QCD_Pt_2400to3200_TuneCUETP8M1_13TeV_pythia8/RunIISummer16MiniAODv2-PUMoriond17_80X_mcRun2_asymptotic_2016_TrancheIV_v6-v1/MINIAODSIM'),
('qcd3200', '/QCD_Pt_3200toInf_TuneCUETP8M1_13TeV_pythia8/RunIISummer16MiniAODv2-PUMoriond17_80X_mcRun2_asymptotic_2016_TrancheIV_v6-v3/MINIAODSIM')

]

GJets_electrons_2016 = [

('GJets_HT40To100',' /GJets_HT-40To100_TuneCUETP8M1_13TeV-madgraphMLM-pythia8/RunIISummer16MiniAODv2-PUMoriond17_80X_mcRun2_asymptotic_2016_TrancheIV_v6-v1/MINIAODSIM'),
('GJets_HT40To100_ext',' //GJets_HT-40To100_TuneCUETP8M1_13TeV-madgraphMLM-pythia8/RunIISummer16MiniAODv2-PUMoriond17_80X_mcRun2_asymptotic_2016_TrancheIV_v6_ext1-v1/MINIAODSIM'),
('GJets_HT100To200','/GJets_HT-100To200_TuneCUETP8M1_13TeV-madgraphMLM-pythia8/RunIISummer16MiniAODv2-PUMoriond17_80X_mcRun2_asymptotic_2016_TrancheIV_v6-v1/MINIAODSIM'),
('GJets_HT100To200_ext','/GJets_HT-100To200_TuneCUETP8M1_13TeV-madgraphMLM-pythia8/RunIISummer16MiniAODv2-PUMoriond17_80X_mcRun2_asymptotic_2016_TrancheIV_v6_ext1-v1/MINIAODSIM'),
('GJets_HT200To400','/GJets_HT-200To400_TuneCUETP8M1_13TeV-madgraphMLM-pythia8/RunIISummer16MiniAODv2-PUMoriond17_80X_mcRun2_asymptotic_2016_TrancheIV_v6-v1/MINIAODSIM'),
('GJets_HT200To400_ext','/GJets_HT-200To400_TuneCUETP8M1_13TeV-madgraphMLM-pythia8/RunIISummer16MiniAODv2-PUMoriond17_80X_mcRun2_asymptotic_2016_TrancheIV_v6_ext1-v1/MINIAODSIM'),
('GJets_HT400To600','/GJets_HT-400To600_TuneCUETP8M1_13TeV-madgraphMLM-pythia8/RunIISummer16MiniAODv2-PUMoriond17_80X_mcRun2_asymptotic_2016_TrancheIV_v6-v1/MINIAODSIM'),
('GJets_HT400To600_ext','/GJets_HT-400To600_TuneCUETP8M1_13TeV-madgraphMLM-pythia8/RunIISummer16MiniAODv2-PUMoriond17_80X_mcRun2_asymptotic_2016_TrancheIV_v6_ext1-v1/MINIAODSIM'),
('GJets_HT600ToInf','/GJets_HT-600ToInf_TuneCUETP8M1_13TeV-madgraphMLM-pythia8/RunIISummer16MiniAODv2-PUMoriond17_80X_mcRun2_asymptotic_2016_TrancheIV_v6-v1/MINIAODSIM'),
('GJets_HT600ToInf_ext','/GJets_HT-600ToInf_TuneCUETP8M1_13TeV-madgraphMLM-pythia8/RunIISummer16MiniAODv2-PUMoriond17_80X_mcRun2_asymptotic_2016_TrancheIV_v6_ext1-v1/MINIAODSIM'),

]


for file, dataset in (dibosons_2018 + tops_2018) :
    inputString = 'dasgoclient -query "file dataset=' + dataset + '" > textfiles/2018/' + file + '.txt'

    print "Running " + dataset
    os.system(inputString)





