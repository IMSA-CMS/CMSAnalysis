#ifndef FITTER_HH
#define FITTER_HH

#include "CMSAnalysis/Analysis/interface/FitFunctionCollection.hh"
#include "CMSAnalysis/Analysis/interface/HistVariable.hh"
#include <TCanvas.h>
#include <TFile.h>
#include <TGraph.h>
#include <TH1.h>
#include <map>

struct ParameterizationData
{
    std::vector<double> x;
    std::vector<double> y;
    std::vector<double> error;
    std::string name;
};

class Fitter
{
  public:
    Fitter(const std::string &functionFile, std::string fitTextFile, const std::string &parameterRootFile,
           std::string parameterizationFuncFile);
    ~Fitter();

    void setFunctionRootOutput(const std::string &name);
    void setFunctionOutput(std::string name);
    void setParameterizationRootOutput(const std::string &name);
    void setParameterizationOutput(std::string name);

    void loadFunctions(FitFunctionCollection fitFunctions);
    // void loadHistogram(const std::vector<string>& histNames);

    void fitFunctions(std::unordered_map<std::string, TH1 *> &histograms);
    void parameterizeFunctions(std::unordered_map<std::string, double> &xData, const std::string &genSim,
                               const std::string &reco, const std::string &var);

    const FitFunctionCollection &getFunctions() const
    {
        return functions;
    }
    void setFunctions(const FitFunctionCollection &newFunctions)
    {
        functions = newFunctions;
    }

  private:
    static void fitExpressionFormula(TH1 *histogram, FitFunction &fitFunction);
    static void fitDSCB(TH1 *histogram, FitFunction &fitFunction);
    static void fitPowerLaw(TH1 *histogram, FitFunction &fitFunction);
    static void fitDoubleGaussian(TH1 *histogram, FitFunction &fitFunction);
    static void fitGausLogPowerNorm(TH1 *hist, FitFunction &func);

    // Insert blank TF1* function ptr which will have the fitted function written to
    static TFitResultPtr fitSingleFunction(TGraph *histogram, TF1 *function, size_t iterations = 1);

    // TH1* readHistogram(const std::string& name);
    std::vector<ParameterizationData> getParameterData(std::unordered_map<std::string, double> &xData);
    FitFunction parameterizeFunction(ParameterizationData &parameterData, const std::string &genSim,
                                     const std::string &reco, const std::string &var);
    // Different ways to try and get seed function for fitting
    // Gets guess inverse power law function y = a(x-b)^-c + d using three points, only guarantees the powerlaw through
    // p0 and p1, not p2
    // static TF1 *seedInversePowerLaw(double x_0, double y_0, double x_1, double y_1, double x_2, double y_2);

    TFile *fitRootFile;
    std::string fitTextFile;

    TFile *parameterRootFile;
    std::string parameterTextFile;

    FitFunctionCollection functions;

    std::map<std::string, TDirectory *> fitDirectories;
    std::map<std::string, TDirectory *> parameterDirectories;
};

#endif