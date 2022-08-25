#include "CMSAnalysis/DataCollection/interface/EventLoader.hh"
#include "CMSAnalysis/DataCollection/interface/MiniAODEventFile.hh"
#include "CMSAnalysis/DataCollection/interface/GenSimEventFile.hh"
#include "CMSAnalysis/DataCollection/interface/DelphesEventFile.hh"
#include "CMSAnalysis/DataCollection/interface/NanoAODEventFile.hh"
#include "TFile.h"
#include "TTree.h"
#include <iostream>

//Changes the file to the proper type
void EventLoader::changeFileFormat(TFile* ifile)
{
    auto eventsBranch = dynamic_cast<TTree*>(ifile->Get("Events"));
    if (dynamic_cast<TTree*>(ifile->Get("myana/mytree"))) //myana/mytree is exclusive to Delphes
    {
        file = std::make_shared<DelphesEventFile> (ifile);
    }
    else if (eventsBranch && eventsBranch->GetBranch("patMuons_slimmedMuons__RECO.obj")) //this branch is exlusive to MiniAOD
    {
        file = std::make_shared<MiniAODEventFile> (ifile);
    }
    else if (eventsBranch && eventsBranch->GetBranch("recoGenParticles_genParticles__GEN.obj")) //this branch is exlusive to GenSim
    {
        file = std::make_shared<GenSimEventFile> (ifile);
    }
    else if (eventsBranch && eventsBranch->GetBranch("nElectron"))
    {
        file = std::make_shared<NanoAODEventFile> (ifile);
    }
    else
    {
        throw std::runtime_error ("File format not recognized");
    }
}


