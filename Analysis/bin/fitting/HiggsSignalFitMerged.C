#include "CMSAnalysis/Analysis/interface/FitFunction.hh"
#include "CMSAnalysis/Analysis/interface/FitFunctionCollection.hh"
#include "CMSAnalysis/Analysis/interface/Fitter.hh"
#include "CMSAnalysis/Analysis/interface/HiggsCompleteAnalysis.hh"
#include "CMSAnalysis/Analysis/interface/HistVariable.hh"
#include "TF1.h"
#include "TGraph.h"
#include "TH1.h"
#include <iostream>
#include <string>
#include <vector>
#include "TFile.h"
#include "TObjString.h"
#include "CMSAnalysis/Utility/interface/CrossSectionReader.hh"
#include "CMSAnalysis/Utility/interface/Utility.hh"
#include <unordered_map>
//lot of them apparently are not used so a lot are not included from second file

bool useFileHistograms = true;

std::string path = "/eos/uscms/store/user/greddy/DCH_files/inputs_nopair/hist_MY/";
std::string processedPath = "/uscms/home/bhenning/nobackup/HiggsWithSystematics1/";

TH1* combineHists (std::vector<std::string> fileNames, std::string channel, std::string histName);
std::vector<std::string> years = {"2016", "2017", "2018"};
//added

bool fitChannel(const Channel &channel, Fitter &fitter, const HistVariable &histType, const std::string &genSim);

//added
TH1* combineHists (std::vector<std::string> fileNames, std::string channel, std::string histName)
{
	std::unordered_map<std::string, int> codeMap = {{"eeee", 1}, {"eeem", 2}, {"eemm", 4}, {"emem", 7}, {"emmm", 9}, {"mmmm", 16}};
	TH1* hist = nullptr;
	int totalEvents = 0;
	for (auto fileName:fileNames)
	{
		for (auto year:years)
		{
			std::cout << "Starting loop\n";
			auto fullName = path + fileName + "_" + year + ".root";
			std::cout << "trying file " << fullName << std::endl;
			auto file = TFile::Open(fullName.c_str());
			std::cout<<"opening file "<<fileName<< std::endl;
			if (!file)
			{	
				std::cout<<"file "<<fullName<<" not found\n";
				continue;
			}
			auto directory = file -> GetDirectory(channel.c_str());
			if (!directory)
			{	
				std::cout<<"directory "<<channel<<" not found\n";
				continue;
			}
			auto selectedHist = directory->Get<TH1>("h_mDCH1");
			// auto tree = directory -> Get<TTree> ("Events");
			// std::string drawCommand = "mll1>>" + histName + "(2000, 0, 2000)";
			// std::string cutCommand = std::string("gen_cat==") + std::to_string(codeMap [channel]);
			// tree -> Draw (drawCommand.c_str(), cutCommand.c_str());
			// TH1* selectedHist = dynamic_cast <TH1*> (gDirectory -> FindObject(histName.c_str()));
			auto nEvents = file->Get<TH1>("hNWEvts");
			if (!selectedHist)
	
			{	
				std::cout<<"Histogram "<<histName<<" not found\n";
				continue;
			}
			std::cout << "nEvents" << nEvents->GetBinContent(1) << std::endl;
			std::cout << "Integral of selected histogram: " << selectedHist->Integral() << std::endl;
			totalEvents += nEvents->GetBinContent(1);
			//totalEvents = 1;

			if (!hist)
			{
				hist = dynamic_cast<TH1*>(selectedHist -> Clone());
				
			}
				else
				{
					hist -> Add(selectedHist);
				
				}
		}
	}
	hist -> Scale(1.0 / totalEvents);
	return hist;
}


const std::vector<HistVariable> histogramTypes = {
    HistVariable(HistVariable::VariableType::InvariantMass, "", true, false),
    HistVariable(HistVariable::VariableType::InvariantMass, "", false, true),
};

const int minData = 500;
const double xMin = 0;
const double xMax = 2000;

const std::string fitHistsName = "H++SignalFitsVoigt.root";
const std::string fitParameterValueFile = "H++SignalFunctionsVoigt.txt";
const std::string parameterFits = "H++SignalParameterFitsVoigt.root";
const std::string parameterFunctions = "H++SignalParameterFunctionsVoigt.txt";

// run in batch mode for faster processing: root -b HiggsSignalFit.C+
void HiggsSignalFitMerged() //changed to actual name
{
    remove(fitParameterValueFile.c_str());
    remove(parameterFunctions.c_str());

    Fitter fitter(fitHistsName, fitParameterValueFile, parameterFits, parameterFunctions);

    auto analysis = HiggsCompleteAnalysis();
    const auto systs = analysis.getSystematics();
    std::cout << "Loaded histograms\n";

    for (const auto &histType : histogramTypes)
    {
        for (const auto &channel : analysis.getChannels())
        {
            if (channel->getName().find("ZPeak") != std::string::npos)
            {
                continue;
            }
            for (const auto &genSim : HiggsCompleteAnalysis::genSimDecays)
            {
                bool ok = fitChannel(*channel, fitter, histType, genSim);
                if (!ok)
                {
                    continue;
                }

                // Fit systematics
                for (const auto &systName : systs)
                {
                    for (const auto &systType : {ScaleFactor::SystematicType::Down, ScaleFactor::SystematicType::Up})
                    {
                        auto systHistType = histType;
                        systHistType.setSystematic(systType, systName);
                        fitChannel(*channel, fitter, systHistType, genSim);
                    }
                }
            }
        }
    }
}

bool fitChannel(const Channel &channel, Fitter &fitter, const HistVariable &histVar, const std::string &genSim)
{
    double skewSum = 0;
    double maxBinPctSum = 0;
    auto n = 0;
    const auto channelName = channel.getName();

    for (const auto mass : HiggsCompleteAnalysis::massTargets)
    {
        TH1* hist = nullptr;

        if (!useFileHistograms)
        {
            const auto process =
                channel.findProcess("Higgs signal " + genSim + " " + std::to_string(mass));

            hist = process->getHist(histVar, true);
        }
        else
        {
            std::string histName =
                (histVar.getName().find("1") != std::string::npos)
                ? "h_mDCH1"
                : "h_mDCH2";

            hist = combineHists(
                {"HppM" + std::to_string(mass)},
                channel.getName(),
                histName
            );
        }

        if (!hist || hist->GetEntries() < minData)
        {
            continue;
        }

        skewSum += hist->GetSkewness();
        maxBinPctSum += hist->GetMaximum() / hist->Integral();
        ++n;
    }

    if (n < 2)
    {
        return false;
    }

    std::string systDesc;
    switch (histVar.getSystematicType())
    {
    case ScaleFactor::SystematicType::Nominal:
        systDesc = "Nominal";
        break;
    case ScaleFactor::SystematicType::Up:
        systDesc = histVar.getSystematicName() + " Up";
        break;
    case ScaleFactor::SystematicType::Down:
        systDesc = histVar.getSystematicName() + " Down";
        break;
    }

    std::cout << "Fitting " << genSim + "->" + channelName << "/" << histVar.getName() + " (" + systDesc + ")\n";

    const double skewAvg = skewSum / n;
    const double maxBinPctAvg = maxBinPctSum / n;
    const FitFunction::FunctionType funcType = FitFunction::FunctionType::Voigt;
    // (-1.5 < skewAvg && 60 * maxBinPctAvg - skewAvg > 0.9)
                                                //    ? FitFunction::FunctionType::DoubleGaussian
                                                //    : FitFunction::FunctionType::DoubleSidedCrystalBall;

    std::unordered_map<std::string, double> massValues;
    std::unordered_map<std::string, TH1 *> histogramMap;
    FitFunctionCollection currentFunctions;
    for (const auto mass : HiggsCompleteAnalysis::massTargets)
    {
        TH1* hist = nullptr;

        if (!useFileHistograms)
        {
            const auto process =
                channel.findProcess("Higgs signal " + genSim + " " + std::to_string(mass));

            hist = process->getHist(histVar, true);
        }
        else
        {
            std::string histName =
                (histVar.getName().find("1") != std::string::npos)
                ? "h_mDCH1"
                : "h_mDCH2";

            hist = combineHists(
                {"HppM" + std::to_string(mass)},
                channel.getName(),
                histName
            );
        }

        if (!hist || hist->GetEntries() < minData)
        {
            continue;
        }

        // auto systs = process->getSystematicHist(histType, true);
        // auto *const histDown = systs.second;
        // auto *const histUp = systs.first;

        // std::cout << "NEntry: " << histDown->GetEntries() << "\n";

        const auto title = "Higgs signal " + genSim + " #rightarrow " + channelName + " " + std::to_string(mass) + " " +
                           histVar.getName() + " " + systDesc;
        hist->SetTitle(title.c_str());
        // histDown->SetTitle((title + " Down").c_str());
        // histUp->SetTitle((title + " Up").c_str());

        // const std::string keyNameDown = keyName + "_Down";
        // const std::string keyNameUp = keyName + "_Up";

        // FitFunction funcDown;
        // FitFunction funcUp;
        const auto name =
            genSim + "->" + channelName + "/" + std::to_string(mass) + ' ' + histVar.getName() + " " + systDesc;
        FitFunction func = FitFunction::createFunctionOfType(funcType, name, "", xMin, xMax, channelName);

        const std::string keyName = std::to_string(mass);
        currentFunctions.insert(keyName, func);
        // currentFunctions.insert(keyNameDown, funcDown);
        // currentFunctions.insert(keyNameUp, funcUp);
        histogramMap.insert({keyName, hist});
        // histogramMap.insert({keyNameDown, histDown});
        // histogramMap.insert({keyNameUp, histUp});
        massValues.insert({keyName, mass});
        // massValues.insert({keyNameDown, mass});
        // massValues.insert({keyNameUp, mass});
    }
    fitter.loadFunctions(currentFunctions);
    fitter.fitFunctions(histogramMap);
    fitter.parameterizeFunctions(massValues, genSim, channelName, histVar.getName(), histVar);
    return true;
}
