#include "CMSAnalysis/Analysis/interface/RootFileInput.hh"
#include "TFile.h"
#include "TH1.h"

RootFileInput::RootFileInput(std::string fileName) :
	file(TFile::Open(fileName.c_str(), "read")) 
	{
		if(!file)
		{
			throw std::runtime_error("Cannot open file!");
		}
	}

TH1* RootFileInput::getInput(std::string name) const
{
	return dynamic_cast<TH1*>(file->Get(name.c_str()));
}