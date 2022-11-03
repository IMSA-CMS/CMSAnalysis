#include "CMSAnalysis/Analysis/interface/SimpleProcess.hh"
#include "CMSAnalysis/Analysis/interface/HistogramFinder.hh"
#include "CMSAnalysis/Analysis/interface/HistVariable.hh"
#include "TH1.h"
#include <unordered_map>
#include <memory>
#include "TFile.h"
#include "TH2.h"
#include <vector>


void SimpleProcess::addFile(std::string fileName, std::shared_ptr<HistogramFinder> histogramFinder) {
    if(checkValidity(fileName, histogramFinder)) {
	files.push_back(fileName);
	histFinders.push_back(histogramFinder);
    }
}

TH1* SimpleProcess::getHist(HistVariable histType) {
	int maxBinNum = 0;
	double maxBarWidth = 0.0;
	int count = 0;
	for (std::string file : files)
	{
		if (getHistFromFile(file, histFinders.at(count), histType) == 0) {
			throw std::runtime_error("Histogram not found in file: " + file);
		}
		if (getHistFromFile(file, histFinders.at(count), histType)->GetNbinsX() > maxBinNum)
		{
			maxBinNum = getHistFromFile(file, histFinders.at(count), histType)->GetNbinsX();
		}
		if ((getHistFromFile(file, histFinders.at(count), histType)->GetXaxis()->GetBinWidth(maxBinNum)) > maxBarWidth)
		{
			maxBarWidth = (getHistFromFile(file, histFinders.at(count), histType)->GetXaxis()->GetBinWidth(maxBinNum));
		}
		count++;
	}
	TH1* hist = new TH1F(name.c_str(), name.c_str(), maxBinNum, 0, maxBinNum * maxBarWidth);
	TH1* toAdd;
	TList* toMerge = new TList;
	count = 0;
	for (std::string file : files)	
	{
		toAdd = getHistFromFile(file, histFinders.at(count), histType);
		toMerge->Add(toAdd);
		count++;
	}
	hist->Merge(toMerge);
	hist->SetLineColor(color);
	hist->SetFillColor(color);
	return hist;    
}

TH1* SimpleProcess::getHistFromFile(std::string file, std::shared_ptr<HistogramFinder> histFinder, HistVariable histType) {
    TH1* hist;
    std::string fileName = filePath + file;
    TFile* openedFile;
    openedFile = TFile::Open(fileName.c_str(), "read");
    if(!openedFile) {
        throw std::runtime_error("Cannot open file!");
    }
    hist = dynamic_cast<TH1*>(openedFile->Get(histFinder->getHistName(histType).c_str()));
    if(dynamic_cast<TH2 *>(hist) != 0) {
        TH2* hist2D = dynamic_cast<TH2 *>(hist);
        TH1 *newhist = hist2D->ProjectionX("_px", 0, -1, "E");
        hist = newhist;
    }
    return hist;
}

bool SimpleProcess::checkValidity(std::string file, std::shared_ptr<HistogramFinder> histFinder) {
    bool validFile = true;
    TH1* hist;
    std::unordered_map<HistVariable, std::string> nameMap = histFinder->getMap();
    for (auto item : nameMap) {
        hist = getHistFromFile(file, histFinder, item.first);
        if(hist == 0) {
            validFile = false;
        }
    }
    return validFile;
}
