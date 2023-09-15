#include "CMSAnalysis/Plans/interface/FileStripPlan.hh"
#include "CMSAnalysis/Modules/interface/FileStripModule.hh"

void FileStripPlan::initialize()
{
	auto fileMod = std::make_shared<FileStripModule>("~/nobackup/DY50_stripped.root");
	getAnalyzer().addAnalysisModule(fileMod);
}