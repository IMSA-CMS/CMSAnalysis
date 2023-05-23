#include "CMSAnalysis/Modules/interface/LeptonJetDataStripModule.hh"
#include "CMSAnalysis/Modules/interface/LeptonJetReconstructionModule.hh"
#include "CMSAnalysis/Modules/interface/LeptonJetMatchingModule.hh"
#include "TFile.h"
#include "TTree.h"

#include <time.h>

void LeptonJetDataStripModule::initialize()
{
    tree = new TTree("Signal", "Lepton Jet Data");
    tree2 = new TTree("Matched Signal", "Lepton Jet Data");
    for (TTree *treeId : {tree, tree2})
    {
        // treeId->Branch("jetIndex", &jetIndex, "jetIndex/I");
        treeId->Branch("nParticles", &nParticles, "nParticles/I");
        treeId->Branch("pt", &pt, "pt/F");
        treeId->Branch("phi", &phi, "phi/F");
        treeId->Branch("eta", &eta, "eta/F");
        // treeId->Branch("mass", &mass, "mass/F");
        treeId->Branch("deltaR", &deltaR, "deltaR/F");

        std::cout << treeId->GetListOfBranches();
    }
}

LeptonJetDataStripModule::LeptonJetDataStripModule(const std::string outputFileName, std::shared_ptr<LeptonJetReconstructionModule> iRecomod, std::shared_ptr<LeptonJetMatchingModule> iMatchmod) : recomod(iRecomod),
                                                                                                                                                                                                    matchmod(iMatchmod)
{
}

void LeptonJetDataStripModule::writeAll()
{
    tree->Write();
    tree2->Write();
    // file->Close();
}

void LeptonJetDataStripModule::finalize()
{
}

bool LeptonJetDataStripModule::process()
{
    auto leptonJets = recomod->getLeptonJets();            // inputs from LeptonJet
    auto matchedLeptonJets = matchmod->getMatchingPairs(); // inputs from QCDM2000?

    int jetIterator = 0;

    for (const auto &leptonJet : leptonJets)
    {
        nParticles = leptonJet.getNumParticles();
        pt = leptonJet.getPt();
        // jetIndex = jetIterator;
        phi = leptonJet.getPhi();
        eta = leptonJet.getEta();
        // mass = leptonJet.getMass();
        deltaR = 0;

        for (Particle p : leptonJet.getParticles())
        {
            for (Particle q : leptonJet.getParticles())
            {
                if (p.getDeltaR(q) > deltaR)
                {
                    deltaR = p.getDeltaR(q);
                }
            }
        }

        tree->Fill();

        jetIterator++;
    }

    for (const auto &leptonJet : matchedLeptonJets)
    {
        const auto &fil = leptonJet.second;
        nParticles = fil.getNumParticles();
        pt = fil.getPt();
        // jetIndex = jetIterator;
        phi = fil.getPhi();
        eta = fil.getEta();
        // mass = fil.getMass();
        deltaR = 0;

        for (Particle p : fil.getParticles())
        {
            for (Particle q : fil.getParticles())
            {
                if (p.getDeltaR(q) > deltaR)
                {
                    deltaR = p.getDeltaR(q);
                }
            }
        }

        tree2->Fill();

        jetIterator++;
    }

    return true;
}