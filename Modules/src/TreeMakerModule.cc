#include "CMSAnalysis/Modules/interface/TreeMakerModule.hh"

bool TreeMakerModule::process()
{
	addVariables();
	return true;
}