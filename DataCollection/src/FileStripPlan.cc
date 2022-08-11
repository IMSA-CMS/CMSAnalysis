#include "CMSAnalysis/DataCollection/interface/FileStripPlan.hh"
#include "CMSAnalysis/DataCollection/interface/FileStripModule.hh"

FileStripPlan::FileStripPlan()
{
	auto fileMod = std::make_shared<FileStripModule>("name.root");
	getAnalyzer().addAnalysisModule(fileMod);

}