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
        treeId->Branch("nParticles", &nParticles, "nParticles/I");
        treeId->Branch("leading_pt", &leading_pt, "pt/F");
        treeId->Branch("phi", &phi, "phi/F");
        treeId->Branch("eta", &eta, "eta/F");
        treeId->Branch("deltaR", &deltaR, "deltaR/F");
        treeId->Branch("sum_pt", &sum_pt, "sum_pt/F");
        treeId->Branch("delta_pt", &delta_pt, "delta_pt/F");

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
}

void LeptonJetDataStripModule::finalize()
{
}

bool LeptonJetDataStripModule::process()
{
    auto leptonJets = recomod->getLeptonJets();            // inputs from LeptonJet
    auto matchedLeptonJets = matchmod->getMatchingPairs(); // inputs from QCDM2000?

    for (const auto &leptonJet : leptonJets)
    {
        nParticles = leptonJet.getNumParticles();
        leading_pt = 0;
        double pt2 = 0;
        phi = leptonJet.getPhi();
        eta = leptonJet.getEta();
        deltaR = 0;

        for (Particle p : leptonJet.getParticles())
        {
            for (Particle q : leptonJet.getParticles())
            {
                if (p.getDeltaR(q) > deltaR)
                {
                    deltaR = p.getDeltaR(q);
                }
                if (p.getPt() > leading_pt)
                {
                    pt2 = leading_pt;
                    leading_pt = p.getPt();
                }
            }
        }

        sum_pt = leptonJet.getPt();
        delta_pt = leading_pt - pt2;

        tree->Fill();
    }

    for (const auto &matchedPair : matchedLeptonJets)
    {
        auto leptonJet = matchedPair.second;
        nParticles = leptonJet.getNumParticles();
        leading_pt = 0;
        double pt2 = 0;
        phi = leptonJet.getPhi();
        eta = leptonJet.getEta();
        deltaR = 0;
        pt2 = 0;

        for (Particle p : leptonJet.getParticles())
        {
            for (Particle q : leptonJet.getParticles())
            {
                if (p.getDeltaR(q) > deltaR)
                {
                    deltaR = p.getDeltaR(q);
                }
                if (p.getPt() > leading_pt)
                {
                    pt2 = leading_pt;
                    leading_pt = p.getPt();
                }
            }
        }

        sum_pt = leptonJet.getPt();
        delta_pt = leading_pt - pt2;

        tree2->Fill();
    }

    return true;
}