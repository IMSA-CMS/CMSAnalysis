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

bool fitChannel(const Channel &channel, Fitter &fitter, const HistVariable &histType, const std::string &genSim);

const std::vector<HistVariable> histogramTypes = {
    HistVariable(HistVariable::VariableType::InvariantMass, "", true, false),
    HistVariable(HistVariable::VariableType::InvariantMass, "", false, true),
};

const int minData = 500;
const double xMin = 0;
const double xMax = 2000;

const std::string fitHistsName = "H++SignalFits.root";
const std::string fitParameterValueFile = "H++SignalFunctions.txt";
const std::string parameterFits = "H++SignalParameterFits.root";
const std::string parameterFunctions = "H++SignalParameterFunctions.txt";

// run in batch mode for faster processing: root -b HiggsSignalFit.C+
void HiggsSignalFit()
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
        const auto process = channel.findProcess("Higgs signal " + genSim + " " + std::to_string(mass));
        const TH1 *selectedHist = process->getHist(histVar, true);

        if (!selectedHist || selectedHist->GetEntries() < minData)
        {
            continue;
        }

        skewSum += selectedHist->GetSkewness();
        maxBinPctSum += selectedHist->GetMaximum() / selectedHist->Integral();
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
    const FitFunction::FunctionType funcType = (-1.5 < skewAvg && 60 * maxBinPctAvg - skewAvg > 0.9)
                                                   ? FitFunction::FunctionType::DoubleGaussian
                                                   : FitFunction::FunctionType::DoubleSidedCrystalBall;

    std::unordered_map<std::string, double> massValues;
    std::unordered_map<std::string, TH1 *> histogramMap;
    FitFunctionCollection currentFunctions;
    for (const auto mass : HiggsCompleteAnalysis::massTargets)
    {
        const auto process = channel.findProcess("Higgs signal " + genSim + " " + std::to_string(mass));
        TH1 *const hist = process->getHist(histVar, true);

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
