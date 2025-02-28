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
#include <map>
#include "CMSAnalysis/Analysis/interface/HistVariable.hh"

RootFileInput::RootFileInput(std::string fileName, std::vector<HistVariable> iHistVariables, 
std::map<std::string, std::string> HistVariableToFileMapping) : 
	histVariables(iHistVariables), 
	fileSource(fileName), 
	HistVariableToFileMapping(HistVariableToFileMapping)
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
	//std::cout << "RootFileInput Hit 1 " << std::endl;
	/*
	std::string name = "";
	std::cout << "histype: " << histType.getName() << std::endl;
	for(HistVariable histVar : histVariables) 
	{
		//std::cout << histVar.getName() << std::endl;
	    if(histVar.getName() == histType.getName()) 
		{
			name = histVar.getHistName();
	    }
	}
	*/

	std::string name;
	if (HistVariableToFileMapping.find(histType.getName()) != HistVariableToFileMapping.end()) {
		name = HistVariableToFileMapping.at(histType.getName());
	} else {
		throw std::runtime_error("HistVariable name [" + histType.getName() + "] not found in mapping [" + fileSource + "]!");
	}

	TH1* hist;
	uint pos = name.find("/");
	auto file = getFile(fileSource);
	//std::cout << "RootFileInput Hit 2 " << std::endl;
	// TH1* emptyHist = new TH1F("h1", "empty", 1, 0.0, 0.0);
	if (pos != std::string::npos)
	{

		std::string folder = name.substr(0,pos);
		std::string histName = name.substr(pos+1);
		TDirectory* dir = (TDirectory*)file->GetDirectory(folder.c_str());
		if (dir)
		{
		
			dir->cd();
			hist = dynamic_cast<TH1*>(dir->Get(histName.c_str()));
			//std::cout << "RootFileInput Hit 3 " << std::endl;
			if (!hist)
			{
				
				delete file;
				return nullptr;
			}
			delete dir;
		}
		else
		{
			
			//We need the nullptr in when adding histograms to know to
			//skip the histogram and not break histogram addition
			std::cout << "No directory named " << folder << " found in file " << fileSource <<"\n";
			// std::cout << "No directory named " + folder + " found in file: "<< fileSource <<"\n";

			delete dir;
			delete hist;
			delete file;
			return nullptr;
		}
	}
	else
	{
	    //std::cout << "RootFileInput ELSE Hit 1 " << std::endl;
		//std::cout << "Here" << std::endl;
		hist = dynamic_cast<TH1*>(file->Get(name.c_str()));
		
	}

	if (!hist || hist->IsZombie())
	{ 
		std::cout << "RootFileInput Zombie Hit 1 " << std::endl;
		throw std::runtime_error("File [" + fileSource + "] doesn't contain histogram [" + histType.getName() + "]");

		if (hist->IsZombie())
		{
			std::cout << "RootFileInput Zombie Hit 2 " << std::endl;
			throw std::runtime_error("File [" + fileSource + "] doesn't contain histogram [" + histType.getName() + "]. Hist is a Zombie.");
		}
		else
		{
			std::cout << "RootFileInput Zombie Else Hit 2 " << std::endl;
			throw std::runtime_error("File [" + fileSource + "] doesn't contain histogram [" + histType.getName() + "]");
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
		//std::cout << "Hist Clone Entries: " << response->GetEntries() << std::endl;

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
			name = histVar.getName();
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
