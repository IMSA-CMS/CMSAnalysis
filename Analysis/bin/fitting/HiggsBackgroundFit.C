#include "CMSAnalysis/Analysis/interface/FitFunction.hh"
#include "CMSAnalysis/Analysis/interface/FitFunctionCollection.hh"
#include "CMSAnalysis/Analysis/interface/Fitter.hh"
#include "CMSAnalysis/Analysis/interface/HiggsCompleteAnalysis.hh"
#include "TF1.h"
#include "TGraph.h"
#include "TH1.h"
#include <iostream>
#include <map>
#include <string>
#include <vector>

void fitProcess(const Process &process, Fitter &fitter, const HistVariable &histVar, const std::string &channelName,
                int min, int max);

const std::vector<HistVariable> histogramTypes = {
    HistVariable(HistVariable::VariableType::InvariantMass, "", true, false),
    HistVariable(HistVariable::VariableType::InvariantMass, "", false, true),
};

const std::string fitHistsName = "H++BackgroundFitsOld.root";
const std::string fitParameterValueFile = "H++BackgroundFunctionsOld.txt";
// These don't do anything
const std::string parameterFits = "H++BackgroundParameterFits.root";
const std::string parameterFunctions = "H++BackgroundParameterFunctions.txt";

const std::map<std::string, std::pair<int, int>> bgsToRange = {
    {"Drell-Yan Background", {0, 2000}},            // 140-500
    {"QCD Background", {0, 2000}},                  // 200-2000
    {"ZZ Background", {0, 2000}},                   // 100-800
    {"WJets Background", {0, 2000}},                //
    {"t#bar{t}, Multiboson Background", {0, 2000}}, //
};

const int minData = 500;

// run in batch mode for faster processing: root -b HiggsBackgroundFit.C+
void HiggsBackgroundFit()
{
    remove(fitParameterValueFile.c_str());
    remove(parameterFunctions.c_str());

    Fitter fitter(fitHistsName, fitParameterValueFile, parameterFits, parameterFunctions);

    const auto analysis = HiggsCompleteAnalysis();
    const auto systs = analysis.getSystematics();
    std::cout << "Loaded histograms\n";

    for (const auto &histVar : histogramTypes)
    {
        for (const auto &channel : analysis.getChannels())
        {
            for (const auto &bgAndRange : bgsToRange)
            {
                const auto process = channel->findProcess(bgAndRange.first);
                fitProcess(*process, fitter, histVar, channel->getName(), bgAndRange.second.first,
                           bgAndRange.second.second);

                // Fit systematics
                for (const auto &systName : systs)
                {
                    for (const auto &systType : {ScaleFactor::SystematicType::Down, ScaleFactor::SystematicType::Up})
                    {
                        auto systHistVar = histVar;

                        systHistVar.setSystematic(systType, systName);
                        fitProcess(*process, fitter, systHistVar, channel->getName(), bgAndRange.second.first,
                                   bgAndRange.second.second);
                    }
                }
            }
        }
    }
}

void fitProcess(const Process &process, Fitter &fitter, const HistVariable &histVar, const std::string &channelName,
                int min, int max)
{
    TH1 *const selectedHist = process.getHist(histVar, true);
    if (selectedHist->GetEntries() < minData)
    {
        return;
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

    const auto title = "Higgs " + channelName + " " + process.getName() + " " + systDesc;
    selectedHist->SetTitle(title.c_str());

    std::cout << "Fitting " + channelName + " " + process.getName() + "/" + histVar.getName() + " (" + systDesc + ")\n";

    const std::string name = process.getName() + "/" + histVar.getName() + " " + systDesc;

    FitFunction func =
        FitFunction::createFunctionOfType(FitFunction::FunctionType::GausLogPowerNorm, name, "", min, max, channelName);

    FitFunctionCollection currentFunctions;
    currentFunctions.insert("", func);
    std::unordered_map<std::string, TH1 *> histogramMap = {{"", selectedHist}};

    fitter.loadFunctions(currentFunctions);
    fitter.fitFunctions(histogramMap);
}