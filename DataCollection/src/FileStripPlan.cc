#include "CMSAnalysis/DataCollection/interface/FileStripPlan.hh"
#include "CMSAnalysis/DataCollection/interface/FileStripModule.hh"

void FileStripPlan::initialize()
{
	auto fileMod = std::make_shared<FileStripModule>("~/nobackup/DY50_stripped.root");
	getAnalyzer().addAnalysisModule(fileMod);
}