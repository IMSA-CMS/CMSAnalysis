#include "CMSAnalysis/DataCollection/interface/EventLoader.hh"
#include "CMSAnalysis/DataCollection/interface/MiniAODEventFile.hh"
#include "CMSAnalysis/DataCollection/interface/DelphesEventFile.hh"
#include "CMSAnalysis/DataCollection/interface/NanoAODEventFile.hh"
#include "TFile.h"
#include "TTree.h"

void EventLoader::changeFile(TFile* ifile)
{
    if (dynamic_cast<TTree*>(ifile->Get("Parentage")))
    {
        file = std::make_shared<MiniAODEventFile> (ifile);
    }
    else if (dynamic_cast<TTree*>(ifile->Get("myana/mytree")))
    {
        file = std::make_shared<DelphesEventFile> (ifile);
    }
    else if (dynamic_cast<TTree*>(ifile->Get("Events")))
    {
        file = std::make_shared<NanoAODEventFile> (ifile);
    }
    else
    {
        throw std::runtime_error ("File format not recognized");
    }
}


