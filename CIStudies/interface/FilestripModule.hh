#ifndef FILESTRIPMODULE_HH
#define FILESTRIPMODULE_HH

#include <vector>

#include "AnalysisModule.hh"
#include "DataFormats/PatCandidates/interface/Muon.h"
#include "DataFormats/PatCandidates/interface/Electron.h"
#include "DataFormats/PatCandidates/interface/MET.h"
#include "DataFormats/HepMCCandidate/interface/GenParticle.h"
#include "TFile.h"
#include "TTree.h"

class FilestripModule : public AnalysisModule
{
    public:
        FilestripModule();

        virtual bool process(const edm::EventBase& event) override;
        virtual void writeAll() override;
    private:
         TFile* file;
        TTree* tree;
        std::vector<pat::Muon> muons;
        // std::vector<pat::Electron> electrons;
        std::vector<pat::MET> mets;
         std::vector<reco::GenParticle> genParticles;
};

#endif