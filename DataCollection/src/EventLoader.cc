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
#include "CMSAnalysis/DataCollection/interface/ProcessDictionary.hh"
#include "CMSAnalysis/DataCollection/interface/SingleFileParams.hh"
#include "CMSAnalysis/DataCollection/interface/ListFileParams.hh"

EventLoader::EventLoader(std::vector<std::shared_ptr<FileParams>> fileList, Analyzer *nAnalyzer)
{
  rootFiles = fileList;
  modules = nAnalyzer;
}

// Changes the file to the proper type
std::shared_ptr<EventFile> EventLoader::changeFileFormat(TFile *ifile, std::shared_ptr<FileParams> iparams)
{
  auto eventsBranch = dynamic_cast<TTree *>(ifile->Get("Events"));
  if (eventsBranch)
    std::cout << "File contains " << eventsBranch->GetEntries() << " events\n";
  if (dynamic_cast<TTree *>(ifile->Get("myana/mytree"))) // myana/mytree is exclusive to Delphes
  {
    return std::make_shared<DelphesEventFile>(ifile, iparams);
  }
  else if (dynamic_cast<TTree *>(ifile->Get("stripped")))
  {
    return std::make_shared<StrippedEventFile>(ifile, iparams);
  }
  else if (eventsBranch && (eventsBranch->GetBranch("patMuons_slimmedMuons__RECO.obj") || eventsBranch->GetBranch("patMuons_slimmedMuons__PAT.obj"))) // this branch is exlusive to MiniAOD
  {
    return std::make_shared<MiniAODEventFile>(ifile, iparams);
  }
  else if (eventsBranch && eventsBranch->GetBranch("recoGenParticles_genParticles__GEN.obj")) // this branch is exlusive to GenSim
  {
    return std::make_shared<GenSimEventFile>(ifile, iparams);
  }
  else if (eventsBranch && eventsBranch->GetBranch("nElectron"))
  {
    return std::make_shared<NanoAODEventFile>(ifile, iparams);
  }
  else
  {
    throw std::runtime_error("File format not recognized");
  }
}

std::vector<std::shared_ptr<FileParams>> EventLoader::fetchRootFiles(const std::string &configFile)
{
  ProcessDictionary dictionary;
  dictionary.loadProcesses(Utility::getFullPath("processes.txt"));
  auto substringFound = configFile.find(".root");
  bool isLocalFile = substringFound != std::string::npos;
  std::vector<std::shared_ptr<FileParams>> rootFiles;
  if (isLocalFile)
  {
    return{std::make_shared<SingleFileParams>(configFile)};
  }
  else
  {
    std::ifstream textFile(Utility::getFullPath(configFile));
    std::string line;
    getline(textFile, line);
    
    //need to add funcionality to skip commented lines
    if (line.substr(0, 1) == "/")
    {
      return{std::make_shared<ListFileParams>(configFile)};
    }
    else
    {
      auto fileparams = dictionary.readFile(configFile);
      /*
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
      */
      return fileparams;
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
  // display how many rrot files
  // each iteration in for loop, processing file... # of events and name of file

  int fileCounter = 0;
  int eventCounter = 0;
  bool stopNow = false;

  // loop through all of the files
  for (auto &params : rootFiles)
  {
    // Get a list of Root files for each filpar object
    auto fileList = params->getFileList();

    for (auto &fileName : fileList)
    {
      // Adds prefix necessary to read remote files
      const std::string eossrc = "root://cmsxrootd.fnal.gov//";
      fileName = eossrc + fileName;

      std::cout << "Name of file: " << fileName << "\n";

      TFile *tFile = TFile::Open(fileName.c_str(), "READ");
      // pass empty files
      if (!tFile)
      {
        std::cout << "File " << fileName << " not found!\n";
        continue;
      }
      file = changeFileFormat(tFile, params); // Makes a GenSimEventFile, DelphesEventFile or MiniAODFile shared pointer
      eventInterface.setFile(file);   // Change eventFile reference
      std::cout << "Number of events in file: " << file->getNumOfEvents() << "\n";

      // Loops through every event in the file

      while (true)
      {
        if (file->isDone())
        {
          break;
        }

        modules->processOneEvent(&eventInterface); // EventInterface will loop through all event files in analyzer
        file->nextEvent();
        ++eventCounter;
        
        if (outputEvery != 0 && eventCounter % outputEvery == 0)
        {
          std::cout << "Processed " << eventCounter << " events" << std::endl;
        }
        if (maxEvents > 0 && eventCounter >= maxEvents)
        {
          stopNow = true;
          break;
        }
      }
      
      ++fileCounter;
      if (nFiles > 0 && fileCounter >= nFiles)
      {
        stopNow = true;
      }
      if (stopNow)
      {
        break;
      }
    delete tFile;
    }

    if (stopNow)
    {
      break;
    }
  }
  std::cout << "number of Root files processed: " << fileCounter << "\n";
}
