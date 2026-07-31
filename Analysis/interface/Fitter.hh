#ifndef FITTER_HH
#define FITTER_HH

#include "CMSAnalysis/Analysis/interface/FitFunctionCollection.hh"
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
    static void fitSingleFunction(TH1* histogram, FitFunction& function);

    FitFunctionCollection fitFunctions(const std::vector<std::pair<TH1*, FitFunction>>& histogramPairs,
        std::string rootFileName);
    // FitFunctionCollection parameterizeFunctions(std::unordered_map<double, TF1*>& xData, const std::string &genSim,
    //     const std::string &reco, const std::string &var, const HistVariable &histVar);
    FitFunctionCollection parameterizeFunction(std::string name, const std::unordered_map<double, TF1*>& xData, 
        std::string rootFileName);

  private:
    static void fitExpressionFormula(TH1 *histogram, FitFunction &fitFunction);
    static void fitDSCB(TH1 *histogram, FitFunction &fitFunction);
    static void fitPowerLaw(TH1 *histogram, FitFunction &fitFunction);
    static void fitDoubleGaussian(TH1 *histogram, FitFunction &fitFunction);
    static void fitGausLogPowerNorm(TH1 *hist, FitFunction &func);
    static void fitVoigt(TH1 *histogram, FitFunction &fitFunction);

    static FitFunction fitPowerLawToGraph(TGraph* graph, std::string name);

    // TH1* readHistogram(const std::string& name);

    // Different ways to try and get seed function for fitting
    // Gets guess inverse power law function y = a(x-b)^-c + d using three points, only guarantees the powerlaw through
    // p0 and p1, not p2
    // static TF1 *seedInversePowerLaw(double x_0, double y_0, double x_1, double y_1, double x_2, double y_2);

    // TFile *fitRootFile;
    // std::string fitTextFile;

    // TFile *parameterRootFile;
    // std::string parameterTextFile;

    // FitFunctionCollection functions;

    // std::map<std::string, TDirectory *> fitDirectories;
    // std::map<std::string, TDirectory *> parameterDirectories;
};

#endif