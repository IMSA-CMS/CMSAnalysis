#include "CMSAnalysis/DataCollection/interface/DataCollectionPlan.hh"
#include "CMSAnalysis/DataCollection/interface/EventLoader.hh"

void DataCollectionPlan::runEventLoader(const std::string& inputFile, const std::string& outputFile, 
	   int outputEvery, int numFiles)
{
    EventLoader eventLoader(fetchRootFiles(inputFile), &analyzer);
    analyzer.initialize();
    eventLoader.run(outputEvery, numFiles);
    analyzer.writeOutputFile(outputFile);
}

void DataCollectionPlan::runAnalyzerWrapper(){

}

std::vector<std::string> DataCollectionPlan::fetchRootFiles(const std::string &configFile) const
{
  ProcessDictionary dictionary;
  dictionary.loadProcesses("textfiles/processes.txt");
  auto substringFound = configFile.find(".root");
  bool isLocalFile = substringFound != std::string::npos;
  std::vector<std::string> rootFiles;
  if (isLocalFile)
  {
    rootFiles.push_back(configFile);
  }
  else
  {
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
  std::cout << "# of root files: " << rootFiles.size() << std::endl;
  return rootFiles;
}