#include "CMSAnalysis/DataCollection/interface/EventLoader.hh"
#include "CMSAnalysis/EventFiles/interface/MiniAODEventFile.hh"
#include "CMSAnalysis/EventFiles/interface/GenSimEventFile.hh"
#include "CMSAnalysis/EventFiles/interface/DelphesEventFile.hh"
#include "CMSAnalysis/EventFiles/interface/NanoAODEventFile.hh"
#include "CMSAnalysis/EventFiles/interface/StrippedEventFile.hh"
#include "TFile.h"
#include "TTree.h"
#include <iostream>

//Changes the file to the proper type
std::shared_ptr<EventFile> EventLoader::changeFileFormat(TFile* ifile)
{
    auto eventsBranch = dynamic_cast<TTree*>(ifile->Get("Events"));
    if (eventsBranch) std::cout << "File contains " << eventsBranch->GetEntries() << " events\n";
    if (dynamic_cast<TTree*>(ifile->Get("myana/mytree"))) //myana/mytree is exclusive to Delphes
    {
        return std::make_shared<DelphesEventFile> (ifile);
    }
    else if (dynamic_cast<TTree*>(ifile->Get("stripped")))
    {
        return std::make_shared<StrippedEventFile> (ifile);
    }
    else if (eventsBranch && (eventsBranch->GetBranch("patMuons_slimmedMuons__RECO.obj") || eventsBranch->GetBranch("patMuons_slimmedMuons__PAT.obj"))) //this branch is exlusive to MiniAOD
    {
        return std::make_shared<MiniAODEventFile> (ifile);
    }
    else if (eventsBranch && eventsBranch->GetBranch("recoGenParticles_genParticles__GEN.obj")) //this branch is exlusive to GenSim
    {
        return std::make_shared<GenSimEventFile> (ifile);
    }
    else if (eventsBranch && eventsBranch->GetBranch("nElectron"))
    {
        return std::make_shared<NanoAODEventFile> (ifile);
    }
    else
    {
        throw std::runtime_error ("File format not recognized");
        return nullptr;
    }
}

void EventLoader::run(int outputEvery, int nFiles, int maxEvents){
  processRootFiles(outputEvery, nFiles, maxEvents);
}

void EventLoader::processRootFiles(int outputEvery, int nFiles, int maxEvents)
{

  int fileCounter = 0;

  //loop through all of the files
  for (auto &fileName : rootFiles)
  {
    ++fileCounter;
    TFile *tFile = TFile::Open(fileName.c_str(), "READ");
    //pass empty files
    if (!tFile) 
    {
      std::cout << "File " << fileName << " not found!\n";
      continue;
    }
    file = changeFileFormat(tFile); // Makes a GenSimEventFile, DelphesEventFile or MiniAODFile shared pointer
    eventInterface.setFile(file); //Change eventFile reference
    // Loops through every event in the file
    int count = 0;
    while (true)
    { 
      count++; 
      if (file->isDone())
      {
        break;
      }
      analyzer->processOneEvent(&eventInterface); //EventInterface will loop through all event files in analyzer
      file->nextEvent();
      if (outputEvery != 0 && count%outputEvery == 0)
      {
        std::cout<<"Processed "<<count<<" Events"<<std::endl;
      }
      if (count == maxEvents)
      {
        break;
      }
    }

    delete tFile;

    // Checks that the correct number of files are processed
    if (nFiles != -1 && fileCounter >= nFiles)
    {
      break;
    }
  }
}


