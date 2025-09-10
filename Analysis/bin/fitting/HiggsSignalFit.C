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
#define _USE_MATH_DEFINES

std::vector<HistVariable> histogramTypes = {
    HistVariable(HistVariable::VariableType::InvariantMass, "", true, false),
    HistVariable(HistVariable::VariableType::InvariantMass, "", false, true),
};

const int minData = 100;

// run in batch mode for faster processing: root -b HiggsSignalFit.C+
void HiggsSignalFit()
{
    const double min = 0;
    const double max = 2000;

    std::string fitHistsName = "H++SignalFits.root";
    std::string fitParameterValueFile = "H++SignalFunctions.txt";
    std::string parameterFits = "H++SignalParameterFits.root";
    std::string parameterFunctions = "H++SignalParameterFunctions.txt";

    remove(fitParameterValueFile.c_str());
    remove(parameterFunctions.c_str());
    std::vector<int> masses = {500, 600, 700, 800, 900, 1000, 1100, 1200, 1300, 1400, 1500};

    Fitter fitter(fitHistsName, fitParameterValueFile, parameterFits, parameterFunctions);

    std::shared_ptr<HiggsCompleteAnalysis> analysis = std::make_shared<HiggsCompleteAnalysis>();
    std::cout << "Loaded histogram\n";
    std::vector<std::string> paramNames = {"alpha_{low}", "alpha_{high}", "n_{low}", "n_{high}",
                                           "mean",        "sigma",        "norm"};

    for (const auto &histType : histogramTypes)
    {
        for (const auto &recoDecay : HiggsCompleteAnalysis::recoDecays)
        {
            auto targetChannel = analysis->getChannel(recoDecay);
            //for (const auto &genSimDecay : HiggsCompleteAnalysis::genSimDecays)
            {
                auto channel = recoDecay + "_" + recoDecay;
                std::unordered_map<std::string, double> massValues;
                std::unordered_map<std::string, TH1 *> histogramMap;
                FitFunctionCollection currentFunctions;
                std::vector<std::string> actualParams;

                for (const auto &name : paramNames)
                {
                    actualParams.push_back(channel + '/' + name);
                }

                for (auto mass : masses)
                {
                    std::cerr << "mass: " << mass << std::endl;
                    // update this to use all gensim decays, not just the same as reco
                    auto process = targetChannel->findProcess("Higgs signal " + recoDecay + " " + std::to_string(mass));
                    TH1 *selectedHist = process->getHist(histType, true);

                    if (!selectedHist || selectedHist->GetEntries() < minData)
                    {
                        continue;
                    }

                    std::string keyName = channel + "/" + std::to_string(mass) + '_' + histType.getName();

                    FitFunction func;
                    if (genSimDecay.find('t') != std::string::npos)
                    {
                        func = FitFunction::createFunctionOfType(FitFunction::DOUBLE_GAUSSIAN, keyName, "", min, max);
                    }
                    else
                    {
                        func = FitFunction::createFunctionOfType(FitFunction::DOUBLE_SIDED_CRYSTAL_BALL, keyName, "",
                                                                 min, max);
                    }
                    currentFunctions.insert(func);
                    histogramMap.insert({keyName, selectedHist});
                    massValues.insert({keyName, mass});

                    // file->Close();
                    // selectedHist->Draw();
                    // std::string wait;
                    // std::cin >> wait;
                }
                if (histogramMap.empty())
                {
                    continue;
                }
                fitter.setHistograms(histogramMap);
                fitter.loadFunctions(currentFunctions);
                fitter.fitFunctions();
                fitter.parameterizeFunctions(massValues, actualParams);
            }
        }
    }
}
