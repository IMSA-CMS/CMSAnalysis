#include "CMSAnalysis/Analysis/interface/RootFileInput.hh"
#include "CMSAnalysis/Utility/interface/TDisplayText.h"
#include "TFile.h"
#include "TH1.h"
#include <iostream>
#include "TH2.h"
#include "TH2F.h"
#include "TFile.h"
#include <memory>
#include <string>
#include <sstream>
#include <vector>
#include "CMSAnalysis/Analysis/interface/HistVariable.hh"

RootFileInput::RootFileInput(std::string fileName, std::vector<HistVariable> iHistVariables) : histVariables(iHistVariables), fileSource(fileName),
	file(TFile::Open(fileName.c_str(), "read")) 
	{
		if(!file)
		{
			throw std::runtime_error("Cannot open file!");
		}
	}


TH1* RootFileInput::getHist(std::string histType) const
{
	std::string name = "";
	for(HistVariable histVar : histVariables) {
	    if(histVar.getName() == histType) {
			name = histVar.getHistName();
	    }
	}

	TH1* hist;
	uint pos = name.find("/");
	if (pos != std::string::npos)
	{
		std::string folder = name.substr(0,pos);
		std::string histName = name.substr(pos+1);
		TDirectory* dir = (TDirectory*)file->GetDirectory(folder.c_str());
		if (dir)
		{
			dir->cd();
			hist = dynamic_cast<TH1*>(dir->Get(histName.c_str()));
		}
		else
		{
			throw std::runtime_error("can't find directory " + folder);
		}
	}
	else
	{
		hist = dynamic_cast<TH1*>(file->Get(name.c_str()));
	}

	if (!hist)
	{
		throw std::runtime_error("File doesn't contain: " + name);
	}
	if(dynamic_cast<TH2 *>(hist) != 0) {
		TH2* hist2D = dynamic_cast<TH2 *>(hist);
		TH1 *newhist = hist2D->ProjectionX("_px", 0, -1, "E");
		//std::cout << "returning newHist " << newhist << std::endl;
		return newhist;
	}	
	return hist;
}

TH1* RootFileInput::get2DHist(std::string histType) const
{
	std::string name = "";
	for(HistVariable histVar : histVariables) {
	    if(histVar.getName() == histType) {
			name = histVar.getHistName();
	    }
	}
	TH1* hist = (TH2F *)file->Get(name.c_str());
	if (!hist)
	{
		throw std::runtime_error("File doesn't contain: " + name);
	}
	//Since only windowEstimator uses this, windowEstimator will proceed with using 1D hists instead of 2D if no 2D hist.
	return hist;
}

int RootFileInput::getTotalEvents() const
{
	TDisplayText *totalevents = file->Get<TDisplayText>("NEvents");
	return std::stoi(totalevents->GetString().Data());
}
