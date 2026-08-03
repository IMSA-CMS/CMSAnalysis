#include "CMSAnalysis/Analysis/interface/FitFunction.hh"
#include "CMSAnalysis/Analysis/interface/FitFunctionCollection.hh"
#include "CMSAnalysis/Analysis/interface/Fitter.hh"
#include "CMSAnalysis/Analysis/interface/HiggsKansasStateAnalysis.hh"
#include "CMSAnalysis/Analysis/interface/HistVariable.hh"
#include "TF1.h"
#include "TGraph.h"
#include "TH1.h"
#include "TROOT.h"
#include <iostream>
#include <string>
#include <vector>

FitFunctionCollection fitChannel(const std::shared_ptr<Channel> channel, const HistVariable &histType, const std::string &genSim, const std::vector<std::string>&systs, TFile* rootFile);
FitFunctionCollection parameterize(FitFunctionCollection functions, TFile* rootFile);

const std::vector<HistVariable> histogramTypes = {
     HistVariable(HistVariable::VariableType::InvariantMass, "", true, false),
    HistVariable(HistVariable::VariableType::InvariantMass, "", false, true),
};

const int minData = 500;
const double xMin = 0;
const double xMax = 2500;

const std::string fitHistsName = "H++SignalFits.root";
const std::string fitParameterValueFile = "H++SignalFunctions.txt";
const std::string parameterFits = "H++SignalParameterFits.root";
const std::string parameterFunctions = "H++SignalParameterFunctions.txt";

// run in batch mode for faster processing: root -b HiggsSignalFit.C+
void HiggsSignalFit()
{
    gROOT->SetBatch(kTRUE);

    auto analysis = HiggsKansasStateAnalysis();
    const auto systs = analysis.getSystematics();
    std::cout << "Loaded histograms\n";

    auto rootFile = TFile::Open(fitHistsName.c_str(), "RECREATE");
    auto parameterRootFile = TFile::Open(parameterFits.c_str(), "RECREATE");
    FitFunctionCollection allFunctions;
    FitFunctionCollection parameterizations;

    for (const auto &histType : histogramTypes)
    {
        for (const auto &channel : analysis.getChannels())
        {
            if (channel->getName().find("ZPeak") != std::string::npos)
            {
                continue;
            }
            for (const auto &genSim : HiggsKansasStateAnalysis::genSimDecays)
            {
                auto fitFunctions = fitChannel(channel, histType, genSim, systs, rootFile);
                allFunctions += fitFunctions;   
                auto parameterization = parameterize(fitFunctions, parameterRootFile);
                parameterizations += parameterization;
            }
        }
    }

    allFunctions.saveFunctions(fitParameterValueFile, true);
    parameterizations.saveFunctions(parameterFunctions, true);
    rootFile->Close();
    delete rootFile;
    parameterRootFile->Close();
    delete parameterRootFile;
}

FitFunctionCollection fitChannel(const std::shared_ptr<Channel> channel, const HistVariable &histVar, const std::string &genSim,
    const std::vector<std::string>& systs, TFile* rootFile)
{
    FitFunctionCollection functions;
    double skewSum = 0;
    double maxBinPctSum = 0;
    auto n = 0;
    const auto channelName = channel->getName();

    for (const auto mass : HiggsKansasStateAnalysis::massTargets)
    {
        const auto process = channel->findProcess("Higgs signal " + genSim + " " + std::to_string(mass));
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
        return FitFunctionCollection();
    }

    std::cout << "Fitting " << genSim + "->" + channelName << "/" << histVar.getName() << "\n";

    const double skewAvg = skewSum / n;
    const double maxBinPctAvg = maxBinPctSum / n;
    const FitFunction::FunctionType funcType = //FitFunction::FunctionType::Voigt;
     //(-1.5 < skewAvg && 60 * maxBinPctAvg - skewAvg > 0.9)
                                                    //? FitFunction::FunctionType::DoubleGaussian
                                                     FitFunction::FunctionType::DoubleSidedCrystalBall;

    std::unordered_map<std::string, double> massValues;
    std::unordered_map<std::string, TH1 *> histogramMap;
    FitFunctionCollection currentFunctions;
    for (const auto mass : HiggsKansasStateAnalysis::massTargets)
    {
        const auto process = channel->findProcess("Higgs signal " + genSim + " " + std::to_string(mass));
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
                           histVar.getName();
        hist->SetTitle(title.c_str());
        // histDown->SetTitle((title + " Down").c_str());
        // histUp->SetTitle((title + " Up").c_str());

        // const std::string keyNameDown = keyName + "_Down";
        // const std::string keyNameUp = keyName + "_Up";

        // FitFunction funcDown;
        // FitFunction funcUp;
        std::map<std::string, std::string> nameParams;
        nameParams["GenSim"] = genSim;
        nameParams["Channel"] = channelName;
        nameParams["Mass"] = std::to_string(mass);
        nameParams["HistVar"] = histVar.getName();

        const auto name = FitFunction::encodeName(nameParams);
        // const auto name =
        //     genSim + "->" + channelName + "/" + std::to_string(mass) + ' ' + histVar.getName() + " " + systDesc;
        FitFunction func = FitFunction::createFunctionOfType(funcType, name, "", xMin, xMax);
        Fitter::fitSingleFunction(hist, func, rootFile); // Only add the nominal version to the Root file
        for (const auto &systName : systs)
        {
            auto systHistType = histVar;
            systHistType.setSystematic(ScaleFactor::SystematicType::Down, systName);
            FitFunction downFunction = FitFunction::createFunctionOfType(funcType, name, "", xMin, xMax);
            TH1 *sysHistdown = process->getHist(systHistType, true);
            Fitter::fitSingleFunction(sysHistdown, downFunction);

            systHistType.setSystematic(ScaleFactor::SystematicType::Up, systName);
            FitFunction upFunction = FitFunction::createFunctionOfType(funcType, name, "", xMin, xMax);
            TH1 *sysHistup = process->getHist(systHistType, true);
            Fitter::fitSingleFunction(sysHistup, upFunction);
            
            func.addSystematic(systName, *upFunction.getFunction(), *downFunction.getFunction());
        }
        functions.insert(func);
        //const std::string keyName = std::to_string(mass);
        //currentFunctions.insert(keyName, func);
        // currentFunctions.insert(keyNameDown, funcDown);
        // currentFunctions.insert(keyNameUp, funcUp);
        //histogramMap.insert({keyName, hist});
        // histogramMap.insert({keyNameDown, histDown});
        // histogramMap.insert({keyNameUp, histUp});
        //massValues.insert({keyName, mass});
        // massValues.insert({keyNameDown, mass});
        // massValues.insert({keyNameUp, mass});
    }
    //fitter.loadFunctions(currentFunctions);
    //fitter.fitFunctions(histogramMap);
    //fitter.parameterizeFunctions(massValues, genSim, channelName, histVar.getName(), histVar);
    return functions;
}

FitFunctionCollection parameterize(FitFunctionCollection functions, TFile* rootFile)
{
    std::unordered_map<double, TF1*> massMap;
    std::string channelName;
    for (auto &pair : functions.getFunctions())
    {
        auto& func = pair.second;
        auto decoded = FitFunction::decodeName(func.getName());
        const auto mass = std::stod(decoded.at("Mass"));
        massMap.insert({mass, func.getFunction()});
        decoded.erase("Mass");
        if (channelName.empty())
        {
            channelName = FitFunction::encodeName(decoded);
        }
    }
    return Fitter::parameterizeFunction(channelName, massMap, rootFile);
}