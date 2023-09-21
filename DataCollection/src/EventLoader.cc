#include "CMSAnalysis/DataCollection/interface/EventLoader.hh"
#include "CMSAnalysis/EventFiles/interface/MiniAODEventFile.hh"
#include "CMSAnalysis/EventFiles/interface/GenSimEventFile.hh"
#include "CMSAnalysis/EventFiles/interface/DelphesEventFile.hh"
#include "CMSAnalysis/EventFiles/interface/NanoAODEventFile.hh"
#include "CMSAnalysis/EventFiles/interface/StrippedEventFile.hh"
#include "CMSAnalysis/EventFiles/interface/EventFile.hh"
#include "CMSAnalysis/Utility/interface/Utility.hh"
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
    }
}


std::vector<std::string> EventLoader::fetchRootFiles(const std::string& configFile) 
{
  ProcessDictionary dictionary;
  dictionary.loadProcesses(Utility::getFullPath("processes.txt"));
  auto substringFound = configFile.find(".root");
  bool isLocalFile = substringFound != std::string::npos;
  std::vector<std::string> rootFiles;
  if (isLocalFile)
  {
    rootFiles.push_back(configFile);
  }
  else
  {
    std::ifstream textFile(configFile); 
    std::string line;
    getline(textFile, line);
    if(line.substr(0,1) == "/") {
      rootFiles.push_back("root://cmsxrootd.fnal.gov//" + line);
      while(getline(textFile, line)) {
        rootFiles.push_back("root://cmsxrootd.fnal.gov//" + line);
      }
      textFile.close();
    }
    else {
      auto fileparams = dictionary.readFile(configFile);
      for (auto &filepar : fileparams)
      {
        // Get a list of Root files for each filpar object
        auto tempFiles = filepar.getFileList();
        rootFiles.insert(rootFiles.end(), tempFiles.begin(), tempFiles.end());
      }
      for (auto &fileName : rootFiles)
      {
        // Adds prefix necessary to read remote files
        const std::string eossrc = "root://cmsxrootd.fnal.gov//";
        fileName = eossrc + fileName;
      }
    }
  }
  std::cout << "# of root files: " << rootFiles.size() << std::endl;
  // for(auto file : rootFiles) {
  //   std::cout << file << std::endl;
  // }
  return rootFiles;
}

void EventLoader::run(int outputEvery, int nFiles, int maxEvents)
{
  processRootFiles(outputEvery, nFiles, maxEvents);
}

void EventLoader::processRootFiles(int outputEvery, int nFiles, int maxEvents)
{
  //display how many rrot files
  //each iteration in for loop, processing file... # of events and name of file

  int fileCounter = 0;

  //loop through all of the files
  for (auto &fileName : rootFiles)
  {
    
    std::cout << "Name of file: " << fileName << "\n";
    
    
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
    std::cout << "Number of events in file: " << file->getNumOfEvents() << "\n";
      
    // Loops through every event in the file
    int count = 0;
    while (true)
    { 
      if (file->isDone())
      {
        break;
      }
      ++count; 
      modules->processOneEvent(&eventInterface); //EventInterface will loop through all event files in analyzer
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

    std::cout<<"Processed "<<count<<" Events"<<std::endl;
    
    delete tFile;

    // Checks that the correct number of files are processed
    if (nFiles != -1 && fileCounter >= nFiles)
    {
      break;
    }
  }
  std::cout << "number of root files: " << fileCounter << "\n";
}


