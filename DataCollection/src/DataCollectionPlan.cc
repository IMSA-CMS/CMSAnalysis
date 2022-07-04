#include "CMSAnalysis/DataCollection/interface/DataCollectionPlan.hh"

void DataCollectionPlan::runAnalyzer(const std::string& inputFile, const std::string& outputFile, 
	   int outputEvery, int numFiles)
{
    analyzer.run(inputFile, outputFile, outputEvery, numFiles);
}