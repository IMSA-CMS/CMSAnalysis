#include "CMSAnalysis/Analysis/interface/SimpleProcess.hh"
#include "CMSAnalysis/Analysis/interface/HistVariable.hh"
#include "TH1.h"
#include <unordered_map>
#include <memory>
#include "TFile.h"
#include "TH2.h"
#include <vector>


void SimpleProcess::addFile(std::string fileName, std::vector<HistVariable> histVariables) {
    if(checkValidity(fileName, histVariables)) {
	files.push_back(fileName);
	histVariableVec.push_back(histVariables);
    }
}

TH1* SimpleProcess::getHist(std::string histType) {
	int maxBinNum = 0;
	double maxBarWidth = 0.0;
	int count = 0;
	for (std::string file : files)
	{
		if (getHistFromFile(file, histVariableVec.at(count), histType) == 0) {
			throw std::runtime_error("Histogram not found in file: " + file);
		}
		if (getHistFromFile(file, histVariableVec.at(count), histType)->GetNbinsX() > maxBinNum)
		{
			maxBinNum = getHistFromFile(file, histVariableVec.at(count), histType)->GetNbinsX();
		}
		if ((getHistFromFile(file, histVariableVec.at(count), histType)->GetXaxis()->GetBinWidth(maxBinNum)) > maxBarWidth)
		{
			maxBarWidth = (getHistFromFile(file, histVariableVec.at(count), histType)->GetXaxis()->GetBinWidth(maxBinNum));
		}
		count++;
	}
	TH1* hist = new TH1F(name.c_str(), name.c_str(), maxBinNum, 0, maxBinNum * maxBarWidth);
	TH1* toAdd;
	TList* toMerge = new TList;
	count = 0;
	for (std::string file : files)	
	{
		toAdd = getHistFromFile(file, histVariableVec.at(count), histType);
		toMerge->Add(toAdd);
		count++;
	}
	hist->Merge(toMerge);
	hist->SetLineColor(color);
	hist->SetFillColor(color);
	return hist;    
}

TH1* SimpleProcess::getHistFromFile(std::string file, std::vector<HistVariable> histVariables, std::string histType) {
    TH1* hist;
    std::string fileName = filePath + file;
    TFile* openedFile;
    openedFile = TFile::Open(fileName.c_str(), "read");
    if(!openedFile) {
        throw std::runtime_error("Cannot open file!");
    }
    std::string name = "";
    for(HistVariable histVar : histVariables) {
	if(histVar.getName() == histType) {
	    name = histVar.getHistName();
	}
    }
    hist = dynamic_cast<TH1*>(openedFile->Get(name.c_str()));
    if(dynamic_cast<TH2 *>(hist) != 0) {
        TH2* hist2D = dynamic_cast<TH2 *>(hist);
        TH1 *newhist = hist2D->ProjectionX("_px", 0, -1, "E");
        hist = newhist;
    }
    return hist;
}

bool SimpleProcess::checkValidity(std::string file, std::vector<HistVariable> histVariables) {
    bool validFile = true;
    TH1* hist;
    for (auto histVar : histVariables) {
        hist = getHistFromFile(file, histVariables, histVar.getName());
        if(hist == 0) {
            validFile = false;
        }
    }
    return validFile;
}
