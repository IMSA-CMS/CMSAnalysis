#include "CMSAnalysis/Analysis/interface/Process.hh"
#include "CMSAnalysis/Analysis/interface/SingleProcess.hh"
#include "CMSAnalysis/Analysis/interface/HistVariable.hh"
#include "CMSAnalysis/Analysis/interface/RateSystematic.hh"
#include "TH1F.h"
#include "TH2F.h"
#include "TList.h"
#include <algorithm>
#include <iostream>
#include <string>
#include <vector>
#include <string>
#include <algorithm>
#include "TList.h"
#include "CMSAnalysis/Utility/interface/ScaleFactor.hh" 
#include "CMSAnalysis/Analysis/interface/ShapeSystematic.hh"


TH1* Process::getHist(const HistVariable& histType, bool scaleToExpected) const
{
	if (processes.size() == 0)
	{
		return new TH1D(name.c_str(), name.c_str(), 1, 0.0, 0.0);
	}

	int maxBinNum = 0;
	double maxBarWidth = 0.0;
	for (const auto& singleProcess : processes)
	{
		TH1 *hist = nullptr;
		try
		{
			hist = singleProcess.getHist(histType, scaleToExpected);
		}
		catch (std::runtime_error& error)
		{
			std::cout << "Error: " << error.what() << '\n';
			continue;
		}
		if (!hist)
		{
			continue;
		}
		/*
		if (!hist || hist->IsZombie()) 
		{
			return nullptr;
			//throw std::runtime_error("Histogram not found in process: " + this->name + "\nIn singleProcess number: " + singleProcessNumber);
		}
		*/
		if (hist->IsZombie()) 
		{
			return nullptr;
			//throw std::runtime_error("Histogram not found in process: " + this->name + "\nIn singleProcess number: " + singleProcessNumber);
		}
		//std::cout << "numBins: " << hist->GetNbinsX() << "\n";
		maxBinNum = std::max(hist->GetNbinsX(), maxBinNum);
		maxBarWidth = std::max(hist->GetXaxis()->GetBinWidth(maxBinNum), maxBarWidth);
	}
	if (maxBinNum == 0)
	{
		return new TH1D(name.c_str(), name.c_str(), 1, 0.0, 0.0);
	}
	TH1 *newHist = new TH1F(name.c_str(), name.c_str(), maxBinNum, 0, maxBinNum * maxBarWidth);
	TList* toMerge = new TList;
	for (const auto& singleProcess : processes)	
	{
		//std::cout << "Process: " << singleProcess.getName() << std::endl;
		TH1 *toAdd = singleProcess.getHist(histType, scaleToExpected);
		//Add only if the hisogram exists

		if (toAdd)
		{
			toMerge->Add(toAdd);
		}

		//std::cout << toAdd->GetName() << " has " << toAdd->GetNbinsX() << std::endl;
	}
	newHist->Merge(toMerge);
	newHist->SetLineColor(color);
	newHist->SetFillColor(color);

	if (!newHist)
	{
		return nullptr;
	}
	//If you want yield to print while running SuperPlot uncomment the print statement (only prints the yield for the first MassTarget in the process)
	//std::cout << "Total yield for mass target " << processes.at(0).getMassTarget() << " is " << getYield("processes.at(0).getMassTarget()") << std::endl;
	//std::cout << "Process Databin of " << newHist->GetName() << " is: " << newHist->GetEntries() << std::endl;
	return newHist;
}

TH2* Process::get2DHist(const HistVariable& histType) const
{
	int maxBinNum = 0;
	int yMaxBinNum = 0;
	double xMaxBarWidth = 0.0;
	double yMaxBarWidth = 0.0;
	int singleProcessNumber = 0;
	//std::cout << processes.size() << std::endl;
	for (const auto& singleProcess : processes)
	{
		singleProcessNumber++;
		if (singleProcess.get2DHist(histType) == 0) {
			throw std::runtime_error("Histogram not found in process: " + this->name + "\nIn singleProcess number: " + singleProcessNumber);
		}
		if (singleProcess.get2DHist(histType)->GetNbinsX() > maxBinNum)
		{
			maxBinNum = singleProcess.get2DHist(histType)->GetNbinsX();
		}
		if ((singleProcess.get2DHist(histType)->GetXaxis()->GetBinWidth(maxBinNum)) > xMaxBarWidth)
		{
			xMaxBarWidth = (singleProcess.getHist(histType, false)->GetXaxis()->GetBinWidth(maxBinNum));
		}
		if (singleProcess.get2DHist(histType)->GetNbinsY() > yMaxBinNum)
		{
			yMaxBinNum = singleProcess.get2DHist(histType)->GetNbinsY();
		}
		if ((singleProcess.get2DHist(histType)->GetYaxis()->GetBinWidth(yMaxBinNum)) > yMaxBarWidth)
		{
			yMaxBarWidth = (singleProcess.getHist(histType, false)->GetYaxis()->GetBinWidth(yMaxBinNum));
		}
	}
	TH2* hist = new TH2F(name.c_str(), name.c_str(), maxBinNum, 0, maxBinNum * xMaxBarWidth, yMaxBinNum, 0, yMaxBinNum * yMaxBarWidth);
	TList* toMerge = new TList;
	for (const auto& singleProcess : processes)	
	{
		toMerge->Add(dynamic_cast<TH2 *>(singleProcess.get2DHist(histType)));
	}
	hist->Merge(toMerge);
	hist->SetLineColor(color);
	hist->SetFillColor(color);
	//If you want yield to print while running SuperPlot uncomment the print statement (only prints the yield for the first MassTarget in the process)
	//std::cout << "Total yield for mass target " << processes.at(0).getMassTarget() << " is " << getYield(processes.at(0).getMassTarget()) << std::endl;
	return hist;
}

TH1* Process::getSingleProcessHist(const HistVariable& histType, const std::string& singleProcessName, bool scaleToExpected) const
{
	return getSingleProcess(singleProcessName).getHist(histType, scaleToExpected);
}

const SingleProcess& Process::getSingleProcess(const std::string& singleProcessName) const
{

	for (const auto& singleProcess : processes)
	{
	
		if (singleProcess.getName() == singleProcessName)
			return singleProcess;
	}

	throw std::invalid_argument("There is no SingleProcess named " + singleProcessName + " within this Process object");
}

double Process::getYield(const HistVariable& dataType) const
{
	double totalYield = 0;
	for(const auto& singleProcess : processes)
	{
		totalYield += singleProcess.getExpectedYield(dataType);
	}
	return totalYield;
}

void Process::addProcess(SingleProcess process)
{
	// if(process.checkValidity())
	// {
	// 	processes.push_back(process);
	// } 
	processes.push_back(process);
}

void Process::addSystematic(std::shared_ptr<Systematic> systematic)
{
	systematics.addSystematic(systematic);
}

std::pair<TH1*, TH1*> Process::getSystematicHist(HistVariable histType, bool scaleToExpected)
{
	auto hist = getHist(histType, scaleToExpected);
	return systematics.getUncertainties(hist);
}

int Process::getNEvents() 
{
	int total = 0;
	for(auto singleProcess : processes) {
		total += singleProcess.getTotalEvents();
	}
	return total;
}

std::pair<TH1*, TH1*> Process::combineSystematics(std::vector<std::shared_ptr<Process>> processes,
	TH1* original)
{
	// get all multisystematics in a vector
	std::vector<std::shared_ptr<MultiSystematic>> multiSystematics;
	for (auto process : processes)
	{
		multiSystematics.push_back(std::make_shared<MultiSystematic>(process->systematics));
		//std::cout << "Size: " << multiSystematics.size() << std::endl;
	}
	return MultiSystematic::combineSystematics(multiSystematics, original);
}


 std::shared_ptr<Systematic> Process::calcSystematic(HistVariable histType, std::string systematicName, bool isShape)
{
    if (isShape)
        return calcShapeSystematic(histType, systematicName);
    else
        return calcRateSystematic(histType, systematicName);
}

 std::shared_ptr<Systematic> Process::calcRateSystematic(HistVariable histType, std::string systematicName)
{
    HistVariable histUp = histType;
    HistVariable histDown = histType;
    
    histUp.setSystematic(ScaleFactor::SystematicType::Up, systematicName);
    TH1* up = getHist(histUp, true);

    histDown.setSystematic(ScaleFactor::SystematicType::Down, systematicName);
    TH1* down = getHist(histDown, true);


	if (!up || !down)
    {
        std::cerr << "Could not create rate systematic histograms for " << systematicName << " in process " << name << std::endl;
        return nullptr;
    }

	//return std::pair {up,down};

    histType.setSystematic(ScaleFactor::SystematicType::Nominal, "");
    TH1* nominal = getHist(histType, true);

    if (!nominal || !up || !down || nominal->Integral() == 0)
    {
        std::cerr << "Missing histograms or zero integral in process: " << name << std::endl;
        return nullptr;
    }

    double upYield = up->Integral() / nominal->Integral();
    double downYield = down->Integral() / nominal->Integral();

    return std::make_shared<RateSystematic>(systematicName, upYield, downYield);
}

 std::shared_ptr<Systematic> Process::calcShapeSystematic(HistVariable histType, std::string systematicName)
{
    
	HistVariable histUp = histType;
    HistVariable histDown = histType;
    
    histUp.setSystematic(ScaleFactor::SystematicType::Up, systematicName);
    TH1* up = getHist(histUp, true);

    histDown.setSystematic(ScaleFactor::SystematicType::Down, systematicName);
    TH1* down = getHist(histDown, true);

    if (!up || !down)
    {
        std::cerr << "Could not create shape systematic histograms for " << systematicName << " in process " << name << std::endl;
        return nullptr;
    }

    return std::make_shared<ShapeSystematic>(systematicName, up, down);
}