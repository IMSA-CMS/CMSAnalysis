import os;

add_2016 = [

('ADDGravToLLM1700_Lam3', '/ADDGravToLL_LambdaT-3000_M-1700_13TeV-pythia8/RunIISummer16MiniAODv2-PUMoriond17_80X_mcRun2_asymptotic_2016_TrancheIV_v6-v1/MINIAODSIM'),
('ADDGravToLLM1700_Lam3.5', '/ADDGravToLL_LambdaT-3500_M-1700_13TeV-pythia8/RunIISummer16MiniAODv2-PUMoriond17_80X_mcRun2_asymptotic_2016_TrancheIV_v6-v1/MINIAODSIM'),
('ADDGravToLLM1700_Lam4', '/ADDGravToLL_LambdaT-4000_M-1700_13TeV-pythia8/RunIISummer16MiniAODv2-PUMoriond17_80X_mcRun2_asymptotic_2016_TrancheIV_v6-v1/MINIAODSIM'),
('ADDGravToLLM1700_Lam4.5', '/ADDGravToLL_LambdaT-4500_M-1700_13TeV-pythia8/RunIISummer16MiniAODv2-PUMoriond17_80X_mcRun2_asymptotic_2016_TrancheIV_v6-v1/MINIAODSIM'),
('ADDGravToLLM1700_Lam5', '/ADDGravToLL_LambdaT-5000_M-1700_13TeV-pythia8/RunIISummer16MiniAODv2-PUMoriond17_80X_mcRun2_asymptotic_2016_TrancheIV_v6-v1/MINIAODSIM'),
('ADDGravToLLM1700_Lam5.5', '/ADDGravToLL_LambdaT-5500_M-1700_13TeV-pythia8/RunIISummer16MiniAODv2-PUMoriond17_80X_mcRun2_asymptotic_2016_TrancheIV_v6-v1/MINIAODSIM'),
('ADDGravToLLM1700_Lam6', '/ADDGravToLL_LambdaT-6000_M-1700_13TeV-pythia8/RunIISummer16MiniAODv2-PUMoriond17_80X_mcRun2_asymptotic_2016_TrancheIV_v6-v1/MINIAODSIM'),
('ADDGravToLLM1700_Lam6.5', '/ADDGravToLL_LambdaT-6500_M-1700_13TeV-pythia8/RunIISummer16MiniAODv2-PUMoriond17_80X_mcRun2_asymptotic_2016_TrancheIV_v6-v1/MINIAODSIM'),
('ADDGravToLLM1700_Lam7', '/ADDGravToLL_LambdaT-7000_M-1700_13TeV-pythia8/RunIISummer16MiniAODv2-PUMoriond17_80X_mcRun2_asymptotic_2016_TrancheIV_v6-v1/MINIAODSIM'),
('ADDGravToLLM1700_Lam7.5', '/ADDGravToLL_LambdaT-7500_M-1700_13TeV-pythia8/RunIISummer16MiniAODv2-PUMoriond17_80X_mcRun2_asymptotic_2016_TrancheIV_v6-v1/MINIAODSIM'),
('ADDGravToLLM1700_Lam8', '/ADDGravToLL_LambdaT-8000_M-1700_13TeV-pythia8/RunIISummer16MiniAODv2-PUMoriond17_80X_mcRun2_asymptotic_2016_TrancheIV_v6-v1/MINIAODSIM'),
('ADDGravToLLM1700_Lam8.5', '/ADDGravToLL_LambdaT-8500_M-1700_13TeV-pythia8/RunIISummer16MiniAODv2-PUMoriond17_80X_mcRun2_asymptotic_2016_TrancheIV_v6-v1/MINIAODSIM'),
('ADDGravToLLM1700_Lam9', '/ADDGravToLL_LambdaT-9000_M-1700_13TeV-pythia8/RunIISummer16MiniAODv2-PUMoriond17_80X_mcRun2_asymptotic_2016_TrancheIV_v6-v1/MINIAODSIM'),
('ADDGravToLLM1700_Lam10', '/ADDGravToLL_LambdaT-10000_M-1700_13TeV-pythia8/RunIISummer16MiniAODv2-PUMoriond17_80X_mcRun2_asymptotic_2016_TrancheIV_v6-v1/MINIAODSIM'),

]

add_2017 = [

('ADDGravToLLM2000toInf_Lam10', '/ADDGravToLL_LambdaT-10000_M-2000ToInf_13TeV-pythia8/RunIIFall17MiniAODv2-PU2017_12Apr2018_94X_mc2017_realistic_v14_ext1-v1/MINIAODSIM') ,
('ADDGravToLLM300to800_Lam10', '/ADDGravToLL_LambdaT-10000_M-300To800_13TeV-pythia8/RunIIFall17MiniAODv2-PU2017_12Apr2018_94X_mc2017_realistic_v14_ext1-v1/MINIAODSIM') ,
('ADDGravToLLM800to1300_Lam10', '/ADDGravToLL_LambdaT-10000_M-800To1300_13TeV-pythia8/RunIIFall17MiniAODv2-PU2017_12Apr2018_94X_mc2017_realistic_v14_ext1-v1/MINIAODSIM') ,
('ADDGravToLLM1300to2000_Lam11', '/ADDGravToLL_LambdaT-11000_M-1300To2000_13TeV-pythia8/RunIIFall17MiniAODv2-PU2017_12Apr2018_94X_mc2017_realistic_v14_ext1-v1/MINIAODSIM') ,
('ADDGravToLLM2000toInf_Lam11', '/ADDGravToLL_LambdaT-11000_M-2000ToInf_13TeV-pythia8/RunIIFall17MiniAODv2-PU2017_12Apr2018_94X_mc2017_realistic_v14_ext1-v1/MINIAODSIM') ,
('ADDGravToLLM800to1300_Lam11', '/ADDGravToLL_LambdaT-11000_M-800To1300_13TeV-pythia8/RunIIFall17MiniAODv2-PU2017_12Apr2018_94X_mc2017_realistic_v14_ext1-v1/MINIAODSIM') ,
('ADDGravToLLM300to800_Lam12', '/ADDGravToLL_LambdaT-12000_M-300To800_13TeV-pythia8/RunIIFall17MiniAODv2-PU2017_12Apr2018_94X_mc2017_realistic_v14_ext1-v3/MINIAODSIM') ,
('ADDGravToLLM800to1300_Lam12', '/ADDGravToLL_LambdaT-12000_M-800To1300_13TeV-pythia8/RunIIFall17MiniAODv2-PU2017_12Apr2018_94X_mc2017_realistic_v14_ext1-v1/MINIAODSIM') ,
('ADDGravToLLM1300to2000_Lam5', '/ADDGravToLL_LambdaT-5000_M-1300To2000_13TeV-pythia8/RunIIFall17MiniAODv2-PU2017_12Apr2018_94X_mc2017_realistic_v14_ext1-v1/MINIAODSIM') ,
('ADDGravToLLM2000toInf_Lam5', '/ADDGravToLL_LambdaT-5000_M-2000ToInf_13TeV-pythia8/RunIIFall17MiniAODv2-PU2017_12Apr2018_94X_mc2017_realistic_v14_ext1-v1/MINIAODSIM') ,
('ADDGravToLLM300to800_Lam5', '/ADDGravToLL_LambdaT-5000_M-300To800_13TeV-pythia8/RunIIFall17MiniAODv2-PU2017_12Apr2018_94X_mc2017_realistic_v14_ext1-v1/MINIAODSIM') ,
('ADDGravToLLM800to1300_Lam5', '/ADDGravToLL_LambdaT-5000_M-800To1300_13TeV-pythia8/RunIIFall17MiniAODv2-PU2017_12Apr2018_94X_mc2017_realistic_v14_ext1-v1/MINIAODSIM') ,
('ADDGravToLLM1300to2000_Lam6', '/ADDGravToLL_LambdaT-6000_M-1300To2000_13TeV-pythia8/RunIIFall17MiniAODv2-PU2017_12Apr2018_94X_mc2017_realistic_v14_ext1-v1/MINIAODSIM') ,
('ADDGravToLLM2000toInf_Lam6', '/ADDGravToLL_LambdaT-6000_M-2000ToInf_13TeV-pythia8/RunIIFall17MiniAODv2-PU2017_12Apr2018_94X_mc2017_realistic_v14_ext1-v1/MINIAODSIM') ,
('ADDGravToLLM300to800_Lam6', '/ADDGravToLL_LambdaT-6000_M-300To800_13TeV-pythia8/RunIIFall17MiniAODv2-PU2017_12Apr2018_94X_mc2017_realistic_v14_ext1-v1/MINIAODSIM') ,
('ADDGravToLLM800to1300_Lam6', '/ADDGravToLL_LambdaT-6000_M-800To1300_13TeV-pythia8/RunIIFall17MiniAODv2-PU2017_12Apr2018_94X_mc2017_realistic_v14_ext1-v1/MINIAODSIM') ,
('ADDGravToLLM1300to2000_Lam7', '/ADDGravToLL_LambdaT-7000_M-1300To2000_13TeV-pythia8/RunIIFall17MiniAODv2-PU2017_12Apr2018_94X_mc2017_realistic_v14_ext1-v1/MINIAODSIM') ,
('ADDGravToLLM300to800_Lam7', '/ADDGravToLL_LambdaT-7000_M-300To800_13TeV-pythia8/RunIIFall17MiniAODv2-PU2017_12Apr2018_94X_mc2017_realistic_v14_ext1-v1/MINIAODSIM') ,
('ADDGravToLLM800to1300_Lam7', '/ADDGravToLL_LambdaT-7000_M-800To1300_13TeV-pythia8/RunIIFall17MiniAODv2-PU2017_12Apr2018_94X_mc2017_realistic_v14_ext1-v1/MINIAODSIM') ,
('ADDGravToLLM1300to2000_Lam8', '/ADDGravToLL_LambdaT-8000_M-1300To2000_13TeV-pythia8/RunIIFall17MiniAODv2-PU2017_12Apr2018_94X_mc2017_realistic_v14_ext1-v1/MINIAODSIM') ,
('ADDGravToLLM2000toInf_Lam8', '/ADDGravToLL_LambdaT-8000_M-2000ToInf_13TeV-pythia8/RunIIFall17MiniAODv2-PU2017_12Apr2018_94X_mc2017_realistic_v14_ext1-v1/MINIAODSIM') ,
('ADDGravToLLM300to800_Lam8', '/ADDGravToLL_LambdaT-8000_M-300To800_13TeV-pythia8/RunIIFall17MiniAODv2-PU2017_12Apr2018_94X_mc2017_realistic_v14_ext1-v1/MINIAODSIM') ,
('ADDGravToLLM800to1300_Lam8', '/ADDGravToLL_LambdaT-8000_M-800To1300_13TeV-pythia8/RunIIFall17MiniAODv2-PU2017_12Apr2018_94X_mc2017_realistic_v14_ext1-v1/MINIAODSIM') ,
('ADDGravToLLM1300to2000_Lam9', '/ADDGravToLL_LambdaT-9000_M-1300To2000_13TeV-pythia8/RunIIFall17MiniAODv2-PU2017_12Apr2018_94X_mc2017_realistic_v14_ext1-v1/MINIAODSIM') ,
('ADDGravToLLM2000toInf_Lam9', '/ADDGravToLL_LambdaT-9000_M-2000ToInf_13TeV-pythia8/RunIIFall17MiniAODv2-PU2017_12Apr2018_94X_mc2017_realistic_v14_ext1-v1/MINIAODSIM') ,
('ADDGravToLLM300to800_Lam9', '/ADDGravToLL_LambdaT-9000_M-300To800_13TeV-pythia8/RunIIFall17MiniAODv2-PU2017_12Apr2018_94X_mc2017_realistic_v14_ext1-v1/MINIAODSIM') ,
('ADDGravToLLM800to1300_Lam9', '/ADDGravToLL_LambdaT-9000_M-800To1300_13TeV-pythia8/RunIIFall17MiniAODv2-PU2017_12Apr2018_94X_mc2017_realistic_v14_ext1-v1/MINIAODSIM') ,

]

add_2018 = [

('ADDGravToLLM1300to2000_Lam100', '/ADDGravToLL_LambdaT-100000_M-1300To2000_13TeV-pythia8/RunIIAutumn18MiniAOD-102X_upgrade2018_realistic_v15-v1/MINIAODSIM') ,
('ADDGravToLLM2000toInf_Lam100', '/ADDGravToLL_LambdaT-100000_M-2000ToInf_13TeV-pythia8/RunIIAutumn18MiniAOD-102X_upgrade2018_realistic_v15-v1/MINIAODSIM') ,
('ADDGravToLLM300to800_Lam100', '/ADDGravToLL_LambdaT-100000_M-300To800_13TeV-pythia8/RunIIAutumn18MiniAOD-102X_upgrade2018_realistic_v15-v1/MINIAODSIM') ,
('ADDGravToLLM800to1300_Lam100', '/ADDGravToLL_LambdaT-100000_M-800To1300_13TeV-pythia8/RunIIAutumn18MiniAOD-102X_upgrade2018_realistic_v15-v1/MINIAODSIM') ,
('ADDGravToLLM1300to2000_Lam10', '/ADDGravToLL_LambdaT-10000_M-1300To2000_13TeV-pythia8/RunIIAutumn18MiniAOD-102X_upgrade2018_realistic_v15-v1/MINIAODSIM') ,
('ADDGravToLLM2000toInf_Lam10', '/ADDGravToLL_LambdaT-10000_M-2000ToInf_13TeV-pythia8/RunIIAutumn18MiniAOD-102X_upgrade2018_realistic_v15-v1/MINIAODSIM') ,
('ADDGravToLLM300to800_Lam10', '/ADDGravToLL_LambdaT-10000_M-300To800_13TeV-pythia8/RunIIAutumn18MiniAOD-102X_upgrade2018_realistic_v15-v1/MINIAODSIM') ,
('ADDGravToLLM800to1300_Lam10', '/ADDGravToLL_LambdaT-10000_M-800To1300_13TeV-pythia8/RunIIAutumn18MiniAOD-102X_upgrade2018_realistic_v15-v1/MINIAODSIM') ,
('ADDGravToLLM1300to2000_Lam11', '/ADDGravToLL_LambdaT-11000_M-1300To2000_13TeV-pythia8/RunIIAutumn18MiniAOD-102X_upgrade2018_realistic_v15-v1/MINIAODSIM') ,
('ADDGravToLLM2000toInf_Lam11', '/ADDGravToLL_LambdaT-11000_M-2000ToInf_13TeV-pythia8/RunIIAutumn18MiniAOD-102X_upgrade2018_realistic_v15-v1/MINIAODSIM') ,
('ADDGravToLLM300to800_Lam11', '/ADDGravToLL_LambdaT-11000_M-300To800_13TeV-pythia8/RunIIAutumn18MiniAOD-102X_upgrade2018_realistic_v15-v1/MINIAODSIM') ,
('ADDGravToLLM800to1300_Lam11', '/ADDGravToLL_LambdaT-11000_M-800To1300_13TeV-pythia8/RunIIAutumn18MiniAOD-102X_upgrade2018_realistic_v15-v1/MINIAODSIM') ,
('ADDGravToLLM1300to2000_Lam12', '/ADDGravToLL_LambdaT-12000_M-1300To2000_13TeV-pythia8/RunIIAutumn18MiniAOD-102X_upgrade2018_realistic_v15-v1/MINIAODSIM') ,
('ADDGravToLLM2000toInf_Lam12', '/ADDGravToLL_LambdaT-12000_M-2000ToInf_13TeV-pythia8/RunIIAutumn18MiniAOD-102X_upgrade2018_realistic_v15-v1/MINIAODSIM') ,
('ADDGravToLLM300to800_Lam12', '/ADDGravToLL_LambdaT-12000_M-300To800_13TeV-pythia8/RunIIAutumn18MiniAOD-102X_upgrade2018_realistic_v15-v1/MINIAODSIM') ,
('ADDGravToLLM800to1300_Lam12', '/ADDGravToLL_LambdaT-12000_M-800To1300_13TeV-pythia8/RunIIAutumn18MiniAOD-102X_upgrade2018_realistic_v15-v1/MINIAODSIM') ,
('ADDGravToLLM1300to2000_Lam4', '/ADDGravToLL_LambdaT-4000_M-1300To2000_13TeV-pythia8/RunIIAutumn18MiniAOD-102X_upgrade2018_realistic_v15-v1/MINIAODSIM') ,
('ADDGravToLLM2000toInf_Lam4', '/ADDGravToLL_LambdaT-4000_M-2000ToInf_13TeV-pythia8/RunIIAutumn18MiniAOD-102X_upgrade2018_realistic_v15-v1/MINIAODSIM') ,
('ADDGravToLLM300to800_Lam4', '/ADDGravToLL_LambdaT-4000_M-300To800_13TeV-pythia8/RunIIAutumn18MiniAOD-102X_upgrade2018_realistic_v15-v1/MINIAODSIM') ,
('ADDGravToLLM800to1300_Lam4', '/ADDGravToLL_LambdaT-4000_M-800To1300_13TeV-pythia8/RunIIAutumn18MiniAOD-102X_upgrade2018_realistic_v15-v1/MINIAODSIM') ,
('ADDGravToLLM1300to2000_Lam5', '/ADDGravToLL_LambdaT-5000_M-1300To2000_13TeV-pythia8/RunIIAutumn18MiniAOD-102X_upgrade2018_realistic_v15-v1/MINIAODSIM') ,
('ADDGravToLLM2000toInf_Lam5', '/ADDGravToLL_LambdaT-5000_M-2000ToInf_13TeV-pythia8/RunIIAutumn18MiniAOD-102X_upgrade2018_realistic_v15-v1/MINIAODSIM') ,
('ADDGravToLLM300to800_Lam5', '/ADDGravToLL_LambdaT-5000_M-300To800_13TeV-pythia8/RunIIAutumn18MiniAOD-102X_upgrade2018_realistic_v15-v1/MINIAODSIM') ,
('ADDGravToLLM800to1300_Lam5', '/ADDGravToLL_LambdaT-5000_M-800To1300_13TeV-pythia8/RunIIAutumn18MiniAOD-102X_upgrade2018_realistic_v15-v1/MINIAODSIM') ,
('ADDGravToLLM1300to2000_Lam6', '/ADDGravToLL_LambdaT-6000_M-1300To2000_13TeV-pythia8/RunIIAutumn18MiniAOD-102X_upgrade2018_realistic_v15-v1/MINIAODSIM') ,
('ADDGravToLLM2000toInf_Lam6', '/ADDGravToLL_LambdaT-6000_M-2000ToInf_13TeV-pythia8/RunIIAutumn18MiniAOD-102X_upgrade2018_realistic_v15-v1/MINIAODSIM') ,
('ADDGravToLLM300to800_Lam6', '/ADDGravToLL_LambdaT-6000_M-300To800_13TeV-pythia8/RunIIAutumn18MiniAOD-102X_upgrade2018_realistic_v15-v1/MINIAODSIM') ,
('ADDGravToLLM800to1300_Lam6', '/ADDGravToLL_LambdaT-6000_M-800To1300_13TeV-pythia8/RunIIAutumn18MiniAOD-102X_upgrade2018_realistic_v15-v1/MINIAODSIM') ,
('ADDGravToLLM1300to2000_Lam7', '/ADDGravToLL_LambdaT-7000_M-1300To2000_13TeV-pythia8/RunIIAutumn18MiniAOD-102X_upgrade2018_realistic_v15-v1/MINIAODSIM') ,
('ADDGravToLLM2000toInf_Lam7', '/ADDGravToLL_LambdaT-7000_M-2000ToInf_13TeV-pythia8/RunIIAutumn18MiniAOD-102X_upgrade2018_realistic_v15-v1/MINIAODSIM') ,
('ADDGravToLLM300to800_Lam7', '/ADDGravToLL_LambdaT-7000_M-300To800_13TeV-pythia8/RunIIAutumn18MiniAOD-102X_upgrade2018_realistic_v15-v1/MINIAODSIM') ,
('ADDGravToLLM800to1300_Lam7', '/ADDGravToLL_LambdaT-7000_M-800To1300_13TeV-pythia8/RunIIAutumn18MiniAOD-102X_upgrade2018_realistic_v15-v1/MINIAODSIM') ,
('ADDGravToLLM1300to2000_Lam8', '/ADDGravToLL_LambdaT-8000_M-1300To2000_13TeV-pythia8/RunIIAutumn18MiniAOD-102X_upgrade2018_realistic_v15-v1/MINIAODSIM') ,
('ADDGravToLLM2000toInf_Lam8', '/ADDGravToLL_LambdaT-8000_M-2000ToInf_13TeV-pythia8/RunIIAutumn18MiniAOD-102X_upgrade2018_realistic_v15-v1/MINIAODSIM') ,
('ADDGravToLLM300to800_Lam8', '/ADDGravToLL_LambdaT-8000_M-300To800_13TeV-pythia8/RunIIAutumn18MiniAOD-102X_upgrade2018_realistic_v15-v1/MINIAODSIM') ,
('ADDGravToLLM800to1300_Lam8', '/ADDGravToLL_LambdaT-8000_M-800To1300_13TeV-pythia8/RunIIAutumn18MiniAOD-102X_upgrade2018_realistic_v15-v1/MINIAODSIM') ,
('ADDGravToLLM1300to2000_Lam9', '/ADDGravToLL_LambdaT-9000_M-1300To2000_13TeV-pythia8/RunIIAutumn18MiniAOD-102X_upgrade2018_realistic_v15-v1/MINIAODSIM') ,
('ADDGravToLLM2000toInf_Lam9', '/ADDGravToLL_LambdaT-9000_M-2000ToInf_13TeV-pythia8/RunIIAutumn18MiniAOD-102X_upgrade2018_realistic_v15-v1/MINIAODSIM') ,
('ADDGravToLLM300to800_Lam9', '/ADDGravToLL_LambdaT-9000_M-300To800_13TeV-pythia8/RunIIAutumn18MiniAOD-102X_upgrade2018_realistic_v15-v1/MINIAODSIM') ,
('ADDGravToLLM800to1300_Lam9', '/ADDGravToLL_LambdaT-9000_M-800To1300_13TeV-pythia8/RunIIAutumn18MiniAOD-102X_upgrade2018_realistic_v15-v1/MINIAODSIM') ,

]

for file, dataset in add_2018:
    inputString = 'dasgoclient -query "file dataset=' + dataset + '" > textfiles/2018/' + file + '.txt'

    print "Running " + dataset
    os.system(inputString)
