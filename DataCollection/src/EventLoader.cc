#include "CMSAnalysis/DataCollection/interface/EventLoader.hh"
#include "CMSAnalysis/DataCollection/interface/MiniAODEventFile.hh"
#include "CMSAnalysis/DataCollection/interface/GenEventFile.hh"
#include "CMSAnalysis/DataCollection/interface/DelphesEventFile.hh"
#include "TFile.h"
#include "TTree.h"
#include <iostream>

void EventLoader::changeFile(TFile* ifile)
{
    auto eventsBranch = dynamic_cast<TTree*>(ifile->Get("Events"));
    if (dynamic_cast<TTree*>(ifile->Get("myana/mytree")))
    {
        file = std::make_shared<DelphesEventFile> (ifile);
    }
    else if (eventsBranch && eventsBranch->GetBranch("patMuons_slimmedMuons__RECO.obj"))  //doesn't seem to recognize all MiniAODFiles
    {
        file = std::make_shared<MiniAODEventFile> (ifile);
    }
    else if (eventsBranch && eventsBranch->GetBranch("recoGenParticles_genParticles__GEN.obj"))
    {
        file = std::make_shared<GenEventFile> (ifile);
    }
    else
    {
        throw std::runtime_error ("File format not recognized");
    }
}



