#include "CMSAnalysis/Analysis/interface/FitFunction.hh"
#include "CMSAnalysis/Analysis/interface/FitFunctionCollection.hh"
#include "CMSAnalysis/Analysis/interface/Fitter.hh"
#include "CMSAnalysis/Analysis/interface/HiggsCompleteAnalysis.hh"
#include "CMSAnalysis/Analysis/interface/HiggsKansasStateAnalysis.hh"
#include "TF1.h"
#include "TGraph.h" 
#include "TH1.h"
#include <iostream>
#include <map>
#include <string>
#include <vector>
#include "TROOT.h"


FitFunction fitProcess(const Process &process, const HistVariable &histVar,
                const std::string &channelName, int min, int max, const std::vector<std::string>& systs, TFile* rootFile);

const std::vector<HistVariable> histogramTypes = {
    HistVariable(HistVariable::VariableType::InvariantMass, "", true, false),
    HistVariable(HistVariable::VariableType::InvariantMass, "", false, true),
};

const std::string fitHistsName = "H++BackgroundFits.root";
const std::string fitParameterValueFile = "H++BackgroundFunctions.txt";
// These don't do anything
const std::string parameterFits = "H++BackgroundParameterFits.root";
const std::string parameterFunctions = "H++BackgroundParameterFunctions.txt";

const std::map<std::string, std::pair<int, int>> bgsToRange = {
    {"Drell-Yan Background", {0, 1500}},            // 140-500
    {"QCD Background", {0, 1500}},                  // 200-2000
    {"ZZ Background", {0, 1500}},                   // 90-930
    {"WJets Background", {0, 1500}},                //
    {"t#bar{t}, Multiboson Background", {0, 1500}}, //
};

const int minData = 10;

// run in batch mode for faster processing: root -b HiggsBackgroundFit.C+
void HiggsBackgroundFit()
{
    gROOT->SetBatch(kTRUE);
    const auto analysis = HiggsKansasStateAnalysis();
    //remove(fitParameterValueFile.c_str());
    //remove(parameterFunctions.c_str());

    //Fitter fitter(fitHistsName, fitParameterValueFile, parameterFits, parameterFunctions);

    const auto systs = analysis.getSystematics();
    std::cout << "Loaded histograms\n";

    auto rootFile = TFile::Open(fitHistsName.c_str(), "RECREATE");
    FitFunctionCollection allFunctions; 

    for (const auto &histVar : histogramTypes)
    {
        for (const auto &channel : analysis.getChannels())
        {
            for (const auto &bgAndRange : bgsToRange)
            {
                if (channel->getName().find("ZPeak") != std::string::npos)
                {
                continue;
                } 
                const auto process = channel->findProcess(bgAndRange.first);
                //auto func = fitProcess(*process, fitter, histVar, channel->getName(), bgAndRange.second.first,
                           //bgAndRange.second.second, systs);
                //allFunctions += func;
                FitFunction func = fitProcess(*process, histVar, channel->getName(),
                bgAndRange.second.first, bgAndRange.second.second, systs, rootFile);
                // Fit systematics
                allFunctions.insert(func);
            }
        }
    }
    allFunctions.saveFunctions(fitParameterValueFile, true);
    rootFile->Close();
    delete rootFile;
}


FitFunction fitProcess(const Process &process, const HistVariable &histVar, const std::string &channelName,
                int min, int max, const std::vector<std::string>& systs, TFile* rootFile)
{
    TH1 *const selectedHist = process.getHist(histVar, true);
    if (!selectedHist || selectedHist->GetEntries() < minData)
    {
        return FitFunction();
    }

    //std::string systDesc;
    //switch (histVar.getSystematicType())
    //{
    // case ScaleFactor::SystematicType::Nominal:
    //     systDesc = "Nominal";
    //     break;
    // case ScaleFactor::SystematicType::Up:
    //     systDesc = histVar.getSystematicName() + " Up";
    //     break;
    // case ScaleFactor::SystematicType::Down:
    //     systDesc = histVar.getSystematicName() + " Down";
    //     break;
    //}

    std::map<std::string, std::string> nameParams;
    nameParams["process"] = process.getName();
    nameParams["channel"] = channelName;
    nameParams["histVar"] = histVar.getName();
    //nameParams["systematic"] = systDesc;

    const std::string name = FitFunction::encodeName(nameParams);
    const auto title = "Higgs " + channelName + " " + process.getName();
    selectedHist->SetTitle(title.c_str());

    // const std::string name = process.getName() + "->" + channelName + "/" + histVar.getName() + " " + systDesc;

    std::cout << "Fitting " + name + "\n";
    FitFunction::FunctionType type = FitFunction::FunctionType::GausLogPowerNorm;
    FitFunction func =
        FitFunction::createFunctionOfType(type, name, "", min, max);

        Fitter::fitSingleFunction(selectedHist, func, rootFile);
    
        for (const auto &systName : systs)
                {
                        auto systHistVar = histVar;

                        systHistVar.setSystematic(ScaleFactor::SystematicType::Down, systName);
                        TH1 *histDown = process.getHist(systHistVar, true);

                        FitFunction downFunction = FitFunction::createFunctionOfType(type, name, "", min, max);
                        Fitter::fitSingleFunction(histDown, downFunction);

                        systHistVar.setSystematic(ScaleFactor::SystematicType::Up, systName);
                        TH1 *histUp = process.getHist(systHistVar, true);

                        
                        FitFunction upFunction = FitFunction::createFunctionOfType(type, name, "", min, max);
                        Fitter::fitSingleFunction(histUp, upFunction);

                        func.addSystematic(systName, *upFunction.getFunction(), *downFunction.getFunction());
                        //fitProcess(*process, fitter, systHistVar, channel->getName(), bgAndRange.second.first,
                                   //bgAndRange.second.second);
                    
                }

    //FitFunctionCollection currentFunctions;
    //currentFunctions.insert(name, func);
    //std::unordered_map<std::string, TH1 *> histogramMap = {{name, selectedHist}};

    //fitter.loadFunctions(currentFunctions);
    //fitter.fitFunctions(histogramMap);
    return func;
}