#include "CMSAnalysis/DataCollection/interface/LeptonJetDataStripModule.hh"
#include "CMSAnalysis/DataCollection/interface/LeptonJetReconstructionModule.hh"
#include "CMSAnalysis/DataCollection/interface/LeptonJetMatchingModule.hh"
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
        treeId->Branch("leadingPt", &leadingPt, "pt/F");
        treeId->Branch("phi", &phi, "phi/F");
        treeId->Branch("eta", &eta, "eta/F");
        // treeId->Branch("mass", &mass, "mass/F");
        treeId->Branch("deltaR", &deltaR, "deltaR/F");
        treeId->Branch("sumPt", &sumPt, "pt/F");
        // treeId->Branch("deltaPt", &deltaPt, "pt/F");

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

    // int jetIterator = 0;

    for (const auto &leptonJet : leptonJets)
    {
        nParticles = leptonJet.getNumParticles();
        sumPt = 0;
        // jetIndex = jetIterator;
        phi = leptonJet.getPhi();
        eta = leptonJet.getEta();
        // mass = leptonJet.getMass();
        deltaR = 0;
        leadingPt = 0;
        // double runnerUpPt = 0;
        // deltaPt = 0;

        for (Particle p : leptonJet.getParticles())
        {
            for (Particle q : leptonJet.getParticles())
            {
                if (p.getDeltaR(q) > deltaR)
                {
                    deltaR = p.getDeltaR(q);
                }
            }
            if (p.getPt() > leadingPt)
            {
                // runnerUpPt = leadingPt;
                leadingPt = p.getPt();
            }
            sumPt += p.getPt();
        }
        // deltaPt = leadingPt - runnerUpPt;
        tree->Fill();
    }

    for (const auto &leptonJet : matchedLeptonJets)
    {
        const auto &fil = leptonJet.second;
        nParticles = fil.getNumParticles();
        sumPt = fil.getPt();
        // jetIndex = jetIterator;
        phi = fil.getPhi();
        eta = fil.getEta();
        // mass = fil.getMass();
        deltaR = 0;
        leadingPt = 0;
        // double runnerUpPt = 0;
        // deltaPt = 0;

        for (Particle p : fil.getParticles())
        {
            for (Particle q : fil.getParticles())
            {
                if (p.getDeltaR(q) > deltaR)
                {
                    deltaR = p.getDeltaR(q);
                }
            }
            if (p.getPt() > leadingPt)
            {
                // runnerUpPt = leadingPt;
                leadingPt = p.getPt();
            }
        }
        // deltaPt = leadingPt - runnerUpPt;
        tree2->Fill();
    }

    return true;
}