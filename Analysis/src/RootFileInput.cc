#include "CMSAnalysis/Analysis/interface/RootFileInput.hh"
#include "CMSAnalysis/Utility/interface/TDisplayText.h"
#include "TFile.h"
#include "TH1.h"
#include <iostream>
#include "TH2.h"
#include "TH2F.h"
#include "TFile.h"
#include "TSystem.h"
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

	//std::cout << "Reading file: " << fileSource << std::endl;

	if(!file)
	{
		throw std::runtime_error("Cannot open file " + fileSource + "!");
	}
	else
	{
		return file;
	}
}


TH1* RootFileInput::getHist(HistVariable histType) const
{
	TH1::AddDirectory(kFALSE);

	std::string name = "";
	std::cout << "histype: " << histType.getName() << std::endl;
	for(HistVariable histVar : histVariables) 
	{
		std::cout << histVar.getName() << std::endl;
	    if(histVar.getName() == histType.getName()) 
		{
			name = histVar.getHistName();
			std::cout << "found" << std::endl;
	    }
	}
	//std::cout << std::endl << "Hist name: " << name << std::endl;
	TH1* hist;
	uint pos = name.find("/");
	auto file = getFile(fileSource);
	std::cout << "Filesource: " << fileSource << std::endl;
	TH1* emptyHist = new TH1F("h1", "empty", 1, 0.0, 0.0);
	std::cout << "POS: " << pos << std::endl;
	std::cout << "NPOS: " << std::string::npos << std::endl;
	if (pos != std::string::npos)
	{
		std::cout << "entered if statement" << std::endl;
		std::string folder = name.substr(0,pos);
		std::cout << "folder: " << folder << std::endl;
		std::string histName = name.substr(pos+1);
		std::cout << "histName: " << histName << std::endl;
		TDirectory* dir = (TDirectory*)file->GetDirectory(folder.c_str());
		if (dir)
		{
			std::cout << "1" << std::endl;
			std::cout << "Directory Name: " << dir->GetName() << std::endl;
			dir->cd();
			hist = dynamic_cast<TH1*>(dir->Get(histName.c_str()));
			if (!hist)
			{
				std::cout << "Bad Hist" << std::endl;	
				delete file;
				std::cout << "Bad Hist 2" << std::endl;	
				//delete dir;
				std::cout << "Bad Hist 4" << std::endl;	
				return nullptr;
			}
			delete dir;
		}
		else
		{
			std::cout << "2" << std::endl;
			//We need the nullptr in when adding histograms to know to
			//skip the histogram and not break histogram addition
			//std::cout << "No histogram named " + name + " found in directory\n";
			delete dir;
			delete hist;
			delete file;
			return nullptr;
		}
	}
	else
	{
		std::cout << "Here" << std::endl;
		hist = dynamic_cast<TH1*>(file->Get(name.c_str()));
		
	}

	if (!hist || hist->IsZombie())
	{ 
		throw std::runtime_error("File [" + fileSource + "] doesn't contain histogram [" + histType.getHistName() + "]");

		if (hist->IsZombie())
		{
			throw std::runtime_error("File [" + fileSource + "] doesn't contain histogram [" + histType.getHistName() + "]. Hist is a Zombie.");
		}
		else
		{
			throw std::runtime_error("File [" + fileSource + "] doesn't contain histogram [" + histType.getHistName() + "]");
		}
	}
	if(dynamic_cast<TH2 *>(hist) != 0)
	{
		TH2* hist2D = dynamic_cast<TH2 *>(hist);
		TH1 *newhist = hist2D->ProjectionX("_px", 0, -1, "E");
		return newhist;
	}	
	// if (hist->GetEntries() < 2.0)
	// {
	// 	delete hist;
	// 	delete file;
	// 	return emptyHist;
	// }
	// else 
	// {
		TH1* response = new TH1F("Hist Clone", hist->GetTitle(), hist->GetXaxis()->GetNbins(), hist->GetXaxis()->GetXmin(), hist->GetXaxis()->GetXmax());
		response->Add(hist);

		delete hist;
		delete file;
		return response;
	// }
}

TH1* RootFileInput::get2DHist(HistVariable histType) const
{
	std::string name = "";
	for(HistVariable histVar : histVariables) {
	    if(histVar.getName() == histType.getName()) {
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
	delete file;
	return std::stoi(totalevents->GetString().Data());
}
