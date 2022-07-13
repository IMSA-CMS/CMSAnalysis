#include "CMSAnalysis/Analysis/interface/RootFileInput.hh"
#include "CMSAnalysis/DataCollection/interface/TDisplayText.h"
#include "TFile.h"
#include "TH1.h"
#include <iostream>
#include "TH2.h"
#include "TH2F.h"

RootFileInput::RootFileInput(std::string fileName, std::string iName) : name(iName), 
	file(TFile::Open(fileName.c_str(), "read")) 
	{
		if(!file)
		{
			throw std::runtime_error("Cannot open file!");
		}
	}


TH1* RootFileInput::getHist() const
{
	TH1* hist = dynamic_cast<TH1*>(file->Get(name.c_str()));
	if(dynamic_cast<TH2 *>(hist) != 0) {
		TH2* hist2D = dynamic_cast<TH2 *>(hist);
		TH1 *newhist = hist2D->ProjectionX("_px", 0, -1, "E");
		return newhist;
	}
	return hist;
}

TH1* RootFileInput::get2DHist() const
{
	TH1* hist = dynamic_cast<TH1*>(file->Get(name.c_str()));
	if(dynamic_cast<TH2 *>(hist) == 0) {
		throw std::runtime_error("no 2D hist");
	}
	return hist;
}

int RootFileInput::getTotalEvents() const
{
	TDisplayText *totalevents = file->Get<TDisplayText>("NEvents");
	return std::stoi(totalevents->GetString().Data());
}