#include "CMSAnalysis/DataCollection/interface/DataCollectionPlan.hh"
#include "CMSAnalysis/DataCollection/interface/EventLoader.hh"
#include "CMSAnalysis/DataCollection/interface/CmsswEventInterface.hh"
#include <fstream>
#include <string>

void DataCollectionPlan::runEventLoader(const std::string& inputFile, const std::string& outputFile, 
	   int outputEvery, int numFiles, int maxEvents)
{
    EventLoader eventLoader(fetchRootFiles(inputFile), &analyzer);
    analyzer.initialize();
    eventLoader.run(outputEvery, numFiles, maxEvents);
    analyzer.writeOutputFile(outputFile);
}

// void DataCollectionPlan::runAnalyzerWrapper(const std::string& inputFile, const std::string& outputFile)
// {
//   //RunAnalyzerWrapper.beginJob();
//   //RunAnalyzerWrapper.analyze(fetchEvent(inputfile), eventsetup);
//   //RunAnalyzerWrapper.endJob();

// }

std::vector<std::string> DataCollectionPlan::fetchRootFiles(const std::string &configFile) const
{
  ProcessDictionary dictionary;
  dictionary.loadProcesses(Utility::getFullPath("processes.txt"));
  /*location + "/src/CMSAnalysis/DataCollection/bin/textfiles/processes.txt");*/
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