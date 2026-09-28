#ifndef FITTER_HH
#define FITTER_HH

#include "CMSAnalysis/Analysis/interface/FitFunctionCollection.hh"
#include "CMSAnalysis/Analysis/interface/FitFunctionParameterization.hh"
#include "CMSAnalysis/Analysis/interface/HistVariable.hh"
#include <TCanvas.h>
#include <TFile.h>
#include <TGraph.h>
#include <TH1.h>
#include <map>

class Fitter
{
  public:
    // Insert blank TF1* function ptr which will have the fitted function written to
    static void fitSingleFunction(TH1* histogram, SimpleFitFunction& function, TFile* rootFile = nullptr);

    static FitFunctionCollection fitFunctions(std::vector<std::pair<TH1*, SimpleFitFunction>>& histogramPairs,
        std::string rootFileName);
    // FitFunctionCollection parameterizeFunctions(std::unordered_map<double, TF1*>& xData, const std::string &genSim,
    //     const std::string &reco, const std::string &var, const HistVariable &histVar);
    static FitFunctionParameterization parameterizeFunction(std::string name,
        const std::unordered_map<double, SimpleFitFunction*>& xData, TFile* rootFile);

  private:
    static void fitExpressionFormula(TH1 *histogram, SimpleFitFunction &fitFunction);
    static void fitDSCB(TH1 *histogram, SimpleFitFunction &fitFunction);
    static void fitPowerLaw(TH1 *histogram, SimpleFitFunction &fitFunction);
    static void fitDoubleGaussian(TH1 *histogram, SimpleFitFunction &fitFunction);
    static void fitGausLogPowerNorm(TH1 *hist, SimpleFitFunction &func);
    static void fitVoigt(TH1 *histogram, SimpleFitFunction &fitFunction);

    static SimpleFitFunction fitPowerLawToGraph(TGraph* graph, std::string name);
};

#endif