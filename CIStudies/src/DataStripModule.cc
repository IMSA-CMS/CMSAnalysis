#include "CIAnalysis/CIStudies/interface/DataStripModule.hh"
#include "CIAnalysis/CIStudies/interface/LeptonJetReconstructionModule.hh"
#include "CIAnalysis/CIStudies/interface/LeptonJetMatchingModule.hh"
#include "TFile.h"
#include "TTree.h"

#include <time.h>

void DataStripModule::initialize()
{
    
}

DataStripModule::DataStripModule(const std::string outputFileName, std::shared_ptr<LeptonJetReconstructionModule> iRecomod, std::shared_ptr<LeptonJetMatchingModule> iMatchmod):
file(new TFile(outputFileName.c_str())),
tree(new TTree("DataStripTree", "Lepton Jet Data")),
tree2(new TTree("DataStripTree2", "Lepton Jet Data")),
recomod(iRecomod),
matchmod(iMatchmod)
{
    for (TTree* treeId : {tree, tree2})
    {
        treeId->Branch("jetIndex", &jetIndex, "jetIndex/I");
        treeId->Branch("nParticles", &nParticles, "nParticles/I");
        treeId->Branch("pt", &pt, "pt/F");
        treeId->Branch("phi", &phi, "phi/F");
        treeId->Branch("eta", &eta, "eta/F");
        treeId->Branch("mass", &mass, "mass/F");
        std::cout << treeId->GetListOfBranches();
    }
}

void DataStripModule::writeAll()
{
    tree->Write();
    tree2->Write();
    file->Close();
}

void DataStripModule::finalize()
{

}

bool DataStripModule::process()
{
    auto leptonJets = recomod->getLeptonJets();
    auto matchedLeptonJets = matchmod->getMatchingPairs();
    int jetIterator = 0;

    for(const auto& leptonJet : leptonJets)
        {
            nParticles = leptonJet.getNumParticles();
            pt = leptonJet.getPt();
            jetIndex = jetIterator;
            phi = leptonJet.getPhi();
            eta = leptonJet.getEta();
            mass = leptonJet.getMass();

            bool isMatched = false;
            for(const auto& matchedJet : matchedLeptonJets)
            {
                if(matchedJet.second == leptonJet)
                {
                    tree->Fill();
                    isMatched = true;
                    break;
                }
            }

            if(!isMatched)
            {
                tree2->Fill();
            }

            jetIterator++;

        }

    //std::cout<<std::to_string(leptonJets.size()).length();
    return true;
}