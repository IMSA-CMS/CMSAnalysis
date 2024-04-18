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

RootFileInput::RootFileInput(std::string fileName, std::vector<HistVariable> iHistVariables) : histVariables(iHistVariables), fileSource(fileName)
{}
	
TFile* RootFileInput::getFile(std::string fileSource) const
{
	auto file = TFile::Open(fileSource.c_str(), "read");
	if(!file)
	{
		throw std::runtime_error("Cannot open file " + fileSource + "!");
	}
	else
	{
		return file;
	}
}


TH1* RootFileInput::getHist(std::string histType) const
{
	//TH1::AddDirectory(kFALSE);
	//TH1* response;
	//response->SetDirectory(0);
	std::cout << "start" << "\n";
	std::string name = "";
	for(HistVariable histVar : histVariables) {
	    if(histVar.getName() == histType) {
			name = histVar.getHistName();
	    }
	}

	TH1* hist;
	uint pos = name.find("/");
	auto file = getFile(fileSource);
	std::cout << "fileName: " << fileSource << "\n";
	//TH1* emptyHist = new TH1D("h1", "empty", 1, 0.0, 0.0);
	if (pos != std::string::npos)
	{
		std::string folder = name.substr(0,pos);
		std::cout << "folder: " << folder << "\n";
		std::string histName = name.substr(pos+1);
		std::cout << "name: " << histName << "\n";
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
	if (!hist || hist->IsZombie())
	{ 
		throw std::runtime_error("File doesn't contain: " + name);
	}
	if(dynamic_cast<TH2 *>(hist) != 0) {
		TH2* hist2D = dynamic_cast<TH2 *>(hist);
		TH1 *newhist = hist2D->ProjectionX("_px", 0, -1, "E");
		//std::cout << "returning newHist " << newhist << std::endl;
		return newhist;
	}	
	// if (hist->GetEntries() < 2.0)
	// {
	// 	//return emptyHist;
	// }
	// else 
	// {
		//TH1* response = new TH1D();
		// returns a clone to prevent constantly overriding the same hist pointer
		//std::cout << "(RootFileInput hist) numBins: " << hist->GetNbinsX() << "\n";
		//hist->SetDirectory(0);
		TH1* response = (TH1*)hist->Clone();
		
		//response = (TH1*)hist->Clone("response");
		//response->Add(hist);
		//response->SetDirectory(0);

		//TH1* response = file->Copy(hist);
		std::cout << "(RootFileInput response) numBins before: " << response->GetNbinsX() << "\n";
		//delete hist;
		//file->Close();
		//delete file;
		std::cout << "(RootFileInput response) numBins after: " << response->GetNbinsX() << "\n";
		return response;
//	}
}

TH1* RootFileInput::get2DHist(std::string histType) const
{
	std::string name = "";
	for(HistVariable histVar : histVariables) {
	    if(histVar.getName() == histType) {
			name = histVar.getHistName();
	    }
	}
	auto file = getFile(fileSource);
	TH1* hist = (TH2F *)file->Get(name.c_str());
	if (!hist)
	{
		throw std::runtime_error("File doesn't contain: " + name);
	}
	//Since only windowEstimator uses this, windowEstimator will proceed with using 1D hists instead of 2D if no 2D hist.
	TH1* response = dynamic_cast<TH1*>(hist->Clone());
	delete hist;
	file->Close();
	return response;
}

TH1* RootFileInput::getHistFromName(std::string histName) const
{
	auto file = getFile(fileSource);
	if(!file)
	{
		throw std::runtime_error("Cannot open file!");
	}
	auto dir = file->GetDirectory("_hists");
	if(!dir)
	{
		throw std::runtime_error("Cannot open directory!");
	}
	auto hist = dynamic_cast<TH1*>(dir->Get(histName.c_str()));
	if (!hist)
	{
		throw std::runtime_error("Histogram " + histName + " not found!");
	}
	return hist;
}

int RootFileInput::getTotalEvents() const
{
	auto file = getFile(fileSource);
	TDisplayText *totalevents = file->Get<TDisplayText>("NEvents");
	return std::stoi(totalevents->GetString().Data());
	file->Close();
}
