#include "CMSAnalysis/Analysis/interface/RootFileInput.hh"
#include "CMSAnalysis/Utility/interface/TDisplayText.h"
#include "TFile.h"
#include "TH1.h"
#include <iostream>
#include "TH2.h"
#include "TH2F.h"
#include "TSystem.h"
#include <memory>
#include <string>
#include <sstream>
#include <vector>
#include <map>
#include "CMSAnalysis/Analysis/interface/HistVariable.hh"
#include "CMSAnalysis/Analysis/interface/HistNameFinder.hh"

RootFileInput::RootFileInput(std::string fileName, 
std::shared_ptr<HistNameFinder> ihistVariableToFileMapping) : 
	fileSource(fileName), 
	histVariableToFileMapping(ihistVariableToFileMapping)
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

	std::string name = histVariableToFileMapping->getHistName(histType);


	TH1* hist;
	uint pos = name.find("/");
	auto file = getFile(fileSource);
	std::string histName = name;
	TDirectory* dir = file;
	//std::cout << "RootFileInput Hit 2 " << std::endl;
	// TH1* emptyHist = new TH1F("h1", "empty", 1, 0.0, 0.0);
	while (pos != std::string::npos)
	{
		std::string folder = histName.substr(0,pos);
		histName = histName.substr(pos+1);
		dir = (TDirectory*)dir->GetDirectory(folder.c_str());
		if (!dir) 
		{
			std::cout << "No directory named " << folder << " found in file " << fileSource <<"\n";
			// std::cout << "No directory named " + folder + " found in file: "<< fileSource <<"\n";

			delete dir;
			delete hist;
			delete file;
			return nullptr;
		}
		dir->cd();
		pos = name.find("/");
	}

	hist = dynamic_cast<TH1*>(dir->Get(histName.c_str()));
	//std::cout << "RootFileInput Hit 3 " << std::endl;
	if (!hist)
	{
		
		delete file;
		return nullptr;
	}
	if (dir != file)
	{
		delete dir;
	}
		
	

	// if (!hist || hist->IsZombie())
	// { 
	// 	std::cout << "RootFileInput Zombie Hit 1 " << std::endl;
	// 	throw std::runtime_error("File [" + fileSource + "] doesn't contain histogram [" + histType.getName() + "]");

	// 	if (hist->IsZombie())
	// 	{
	// 		std::cout << "RootFileInput Zombie Hit 2 " << std::endl;
	// 		throw std::runtime_error("File [" + fileSource + "] doesn't contain histogram [" + histType.getName() + "]. Hist is a Zombie.");
	// 	}
	// 	else
	// 	{
	// 		std::cout << "RootFileInput Zombie Else Hit 2 " << std::endl;
	// 		throw std::runtime_error("File [" + fileSource + "] doesn't contain histogram [" + histType.getName() + "]");
	// 	}
	// }
	/*
	if(dynamic_cast<TH2 *>(hist) != 0)
	{
		TH2* hist2D = dynamic_cast<TH2 *>(hist);
		TH1 *newhist = hist2D->ProjectionX("_px", 0, -1, "E");
		return newhist;
	}	
	*/
	// if (hist->GetEntries() < 2.0)
	// {
	// 	delete hist;
	// 	delete file;
	// 	return emptyHist;
	// }
	// else 
	// {
		TH1* response = new TH1F("Hist Clone", hist->GetTitle(), hist->GetXaxis()->GetNbins(), hist->GetXaxis()->GetXmin(), hist->GetXaxis()->GetXmax());

		// if (histType.is2DHistX())
		// {
		// 	TH2* hist2D = dynamic_cast<TH2 *>(hist);
		// 	if (hist2D == nullptr)
		// 	{
		// 		throw std::runtime_error("Cast error");
		// 	}
		// 	response = hist2D->ProjectionX("_px", 0, -1, "E");
		// }
		// else if (histType.is2DHistY())
		// {
		// 	TH2* hist2D = dynamic_cast<TH2 *>(hist);
		// 	if (hist2D == nullptr)
		// 	{
		// 		throw std::runtime_error("Cast error");
		// 	}
		// 	response = hist2D->ProjectionY("_py", 0, -1, "E");
		// }
		// else
		{
			response->Add(hist);
		}
		//std::cout << "Hist Clone Entries: " << response->GetEntries() << std::endl;

		delete hist;
		delete file;
		return response;
	// }
}


TH1* RootFileInput::get2DHist(HistVariable histType) const
{
	std::string name = histVariableToFileMapping->getHistName(histType);

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
	bool end = false; 
	int index = 1;
	int events = 0;
	while(!end){
		std::string s = "NEvents;" + std::to_string(index);
    	const char* fName = s.c_str();
		TDisplayText *totalevents = file->Get<TDisplayText>(fName);
		if(totalevents)
		{
			events+=std::stoi(totalevents->GetString().Data());
			index+=2;
		}
		else{
			end = true;
		}

	}
	delete file;
	return events;
}	
