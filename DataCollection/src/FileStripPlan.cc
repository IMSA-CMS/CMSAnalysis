#include "CMSAnalysis/DataCollection/interface/FileStripPlan.hh"
#include "CMSAnalysis/DataCollection/interface/FileStripModule.hh"

FileStripPlan::FileStripPlan()
{
	auto fileMod = std::make_shared<FileStripModule>("~/nobackup/DY10_stripped_test.root");
	getAnalyzer().addAnalysisModule(fileMod);
}
