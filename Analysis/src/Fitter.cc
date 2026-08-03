#include "CMSAnalysis/Analysis/interface/Fitter.hh"
#include "CMSAnalysis/Analysis/interface/FitFunction.hh"
#include <Fit/FitResult.h>
#include <TCanvas.h>
#include <TFitResult.h>
#include <TFitResultPtr.h>
#include <TGraphErrors.h>
#include <TH1.h>
#include <TPaveStats.h>
#include <TROOT.h>
#include <TStyle.h>
#include <algorithm>
#include <array>
#include <boost/algorithm/cxx17/reduce.hpp>
#include <cmath>
#include <stdexcept>
#include <utility>

FitFunctionCollection Fitter::fitFunctions(const std::vector<std::pair<TH1*, FitFunction>>& histogramPairs,
    std::string rootFileName)
{
    TFile* rootFile = TFile::Open(rootFileName.c_str(), "RECREATE");
    FitFunctionCollection functions;
    for (const auto& histPair : histogramPairs)
    {
        auto histogram = histPair.first;
        auto func = histPair.second;

        fitSingleFunction(histogram, func);

        auto* inner = func.getFunction();

        for (auto par = 0; par < inner->GetNpar(); par++)
        {
            // Set error to at least 1% of the parameter value to avoid zero error
            // Not sure if this is a good idea
            const auto error = std::max(inner->GetParError(par), 0.01 * inner->GetParameter(par));
            inner->SetParError(par, error);
        }

        functions.insert(func);
    }
    rootFile->Close();
    delete rootFile;
    // functions.saveFunctions(fitTextFile, true);
    return functions;
}

void Fitter::fitSingleFunction(TH1* histogram, FitFunction& function, TFile* rootFile)
{
    if (!histogram)
    {
        throw std::runtime_error(std::string("fitter::fitFunctions attempted histogram that does not exist: ") + histogram->GetName());
    }

    switch (function.getFunctionType())
    {
    case FitFunction::FunctionType::ExpressionFormula:
        fitExpressionFormula(histogram, function);
        break;
    case FitFunction::FunctionType::DoubleSidedCrystalBall:
        fitDSCB(histogram, function);
        break;
    case FitFunction::FunctionType::PowerLaw:
        fitPowerLaw(histogram, function);
        break;
    case FitFunction::FunctionType::DoubleGaussian:
        fitDoubleGaussian(histogram, function);
        break;
    case FitFunction::FunctionType::GausLogPowerNorm:
        fitGausLogPowerNorm(histogram, function);
        break;
    case FitFunction::FunctionType::Voigt:
        fitVoigt(histogram, function);
        break;
    }

    if (rootFile)
    {
        auto name = function.getName();
        auto canvas = TCanvas(name.c_str(), name.c_str(), 0, 0, 1500, 500);
        histogram->Scale(1.0 / histogram->GetBinWidth(1)); // Scaling needed to display
        histogram->Draw();

        rootFile->WriteObject(&canvas, name.c_str());

        canvas.Close();
    }
}

// not sure if this is used at all
void Fitter::fitExpressionFormula(TH1 *histogram, FitFunction &fitFunction)
{
    TFitResultPtr result =
        histogram->Fit(fitFunction.getFunction(), "SQRWIDTH", "", fitFunction.getMin(), fitFunction.getMax());
    gStyle->SetOptFit(1111);
}

void Fitter::fitDSCB(TH1 *histogram, FitFunction &fitFunction)
{
    TF1 *f1 = fitFunction.getFunction();
    TFitResultPtr gausResult = histogram->Fit("gaus", "SWLQR", "", fitFunction.getMin(), fitFunction.getMax());
    auto params = gausResult->Parameters();

    f1->SetNpx(1000);
    double norm = histogram->Integral(); //("width");

    f1->SetParameters(2.82606, 2.5, 1.08, 1.136, params[1], params[2], norm);


    f1->SetParLimits(0, 0, 10);
    f1->SetParLimits(1, 0, 10);
    f1->SetParLimits(2, 1, 10);
    f1->SetParLimits(3, 1, 10);
    f1->SetParLimits(4, fitFunction.getMin(), fitFunction.getMax());
    f1->FixParameter(6, norm);

    f1->SetRange(fitFunction.getMin(), fitFunction.getMax());
    f1->SetLineColor(kRed);
    histogram->Fit(f1, "SWLQRBWIDTH");
    f1->SetParError(6, norm / (sqrt(histogram->GetEntries())));


    gStyle->SetOptFit(111111);

    TPaveStats *st = dynamic_cast<TPaveStats *>(histogram->FindObject("stats"));
    st->SetX1NDC(0.1);
    st->SetX2NDC(0.5);
}

void Fitter::fitPowerLaw(TH1 *histogram, FitFunction &fitFunction)
{
    std::array<double, 3> initalParams = {{1e17, 0, -5}};
    fitFunction.getFunction()->SetParameters(initalParams.data());

    // setting this as L (log likelihood) fit
    // gives much worse fits for some graphs but seg faults without L sometimes?
    // Root says better when histogram represents counts
    TFitResultPtr result =
        histogram->Fit(fitFunction.getFunction(), "SWLQR", "", fitFunction.getMin(), fitFunction.getMax());

    // shouldn't even be doing this? we're minimizing log likelihood, not chi2
    double chi2 = __DBL_MAX__;
    while (chi2 - result->Chi2() > 0.000001)
    {
        chi2 = result->Chi2();
        fitFunction.getFunction()->SetParameters(result->Parameter(0), result->Parameter(1), result->Parameter(2));
        result = histogram->Fit(fitFunction.getFunction(), "SWLQR", "", fitFunction.getMin(), fitFunction.getMax());
    }

    gStyle->SetOptFit(1111);
}

void Fitter::fitDoubleGaussian(TH1 *histogram, FitFunction &fitFunction)
{
    const auto mean = histogram->GetMean();
    const auto std = histogram->GetStdDev();

    TFitResultPtr LowGaus = histogram->Fit("gaus", "SWLQWIDTH", "", fitFunction.getMin(), mean);
    TFitResultPtr HighGaus = histogram->Fit("gaus", "SWLQWIDTH", "", mean, fitFunction.getMax());

    double LowGausMul;
    double LowGausMean;
    double LowGausSigma;
    if ((int)LowGaus == 0)
    {
        LowGausMul = LowGaus->Parameters().at(0);
        LowGausMean = LowGaus->Parameters().at(1);
        LowGausSigma = LowGaus->Parameters().at(2);
    }
    else
    {
        LowGausMul = NAN;
        LowGausMean = mean - std;
        LowGausSigma = std;
    }

    double HighGausMul;
    double HighGausMean;
    double HighGausSigma;
    if ((int)HighGaus == 0)
    {
        HighGausMul = HighGaus->Parameters().at(0);
        HighGausMean = HighGaus->Parameters().at(1);
        HighGausSigma = HighGaus->Parameters().at(2);
    }
    else
    {
        HighGausMul = NAN;
        HighGausMean = mean + std;
        HighGausSigma = std;
    }

    TF1 *f1 = fitFunction.getFunction();

    f1->SetParameters(LowGausMul, LowGausMean, LowGausSigma, HighGausMul, HighGausMean, HighGausSigma);

    f1->SetNpx(1000);

    TFitResultPtr res;

    f1->SetParLimits(0, 0.0, 0.1);
    f1->SetParLimits(1, mean - 2 * std, mean + std);
    f1->SetParLimits(2, 0.0, 2 * std);
    f1->SetParLimits(3, 0.0, 0.1);
    f1->SetParLimits(4, mean - std, mean + 2 * std);
    f1->SetParLimits(5, 0.0, 2 * std);

    res = histogram->Fit(f1, "SWLQGWIDTH", "", fitFunction.getMin(), fitFunction.getMax());

    if (f1->GetParameter(4) < f1->GetParameter(1))
    {
        auto h1 = f1->GetParameter(0);
        auto h2 = f1->GetParameter(1);
        auto h3 = f1->GetParameter(2);
        f1->SetParameter(0, f1->GetParameter(3));
        f1->SetParameter(1, f1->GetParameter(4));
        f1->SetParameter(2, f1->GetParameter(5));
        f1->SetParameter(3, h1);
        f1->SetParameter(4, h2);
        f1->SetParameter(5, h3);
    }

    gStyle->SetOptFit(1111);
}

void Fitter::fitGausLogPowerNorm(TH1 *const hist, FitFunction &func)
{
    TF1 *const f1 = func.getFunction();

    // Params: mult, u, sigma1, s, n
    f1->SetParameters(1, hist->GetMean(), hist->GetStdDev(), 1, 2);
    f1->SetParLimits(1, 0, func.getMax());

    f1->SetNpx(1000);

    hist->Fit(f1, "SWLQWIDTH", "", func.getMin(), func.getMax());

    gStyle->SetOptFit(1111);
}

void Fitter::fitVoigt(TH1 *histogram, FitFunction &fitFunction)
{
    TF1 *f1 = fitFunction.getFunction();
    const double fitMin = fitFunction.getMin();
    const double fitMax = fitFunction.getMax();
    const double fitRange = fitMax - fitMin;
    const double mean = histogram->GetMean();
    const double stdDev = histogram->GetStdDev();
    const double norm = histogram->Integral("width");
    //sketchy
    f1->SetParameters(norm/2, mean, stdDev, stdDev);
    f1->SetParLimits(0, 0.0, norm);
    f1->SetParLimits(1, fitMin, fitMax);
    f1->SetParLimits(2, 1e-6, fitRange);
    f1->SetParLimits(3, 1e-6, fitRange);
    f1->SetNpx(1000);
    histogram->Fit(f1, "SWLQWIDTH", "", fitMin, fitMax);
    gStyle->SetOptFit(1111);
}

FitFunction Fitter::fitPowerLawToGraph(TGraph* graph, std::string name)
{
    auto function =
        FitFunction::createFunctionOfType(FitFunction::FunctionType::PowerLaw, name, "", 0, 2000);

    auto *func = function.getFunction();

    func->SetParLimits(1, -10000, 0);
    for (int n = 0; n < 4; ++n)
    {
        graph->Fit(func, "SQ");
    }

    func->SetRange(0, 2000);
    return function;
}

FitFunctionCollection Fitter::parameterizeFunction(std::string name, const std::unordered_map<double, TF1*>& xData, 
    TFile* rootFile)
{
    FitFunctionCollection paramFunctions;
    
    if (xData.empty())
    {
        return paramFunctions;
    }
    
    auto nPoints = xData.size();
    auto nParams = xData.begin()->second->GetNpar();
    for (int i = 0; i < nParams; ++i)
    {
        std::vector<double> xValues;
        std::vector<double> yValues;
        std::vector<double> errors;

        for (const auto& [x, func] : xData)
        {
            xValues.push_back(x);
            yValues.push_back(func->GetParameter(i));
            errors.push_back(func->GetParError(i));
        }

        TGraphErrors graph(nPoints, xValues.data(), yValues.data(), nullptr, errors.data());
        std::string fullName = name + " parameter " + xData.begin()->second->GetParName(i);
        graph.SetTitle(fullName.c_str());

        auto fit = fitPowerLawToGraph(&graph, fullName);
        paramFunctions.insert(fit);

        auto *const canvas = new TCanvas(fullName.c_str(), fullName.c_str(), 0, 0, 2000, 500);

        graph.Draw("AP");

        gStyle->SetOptFit(1111);

        rootFile->WriteObject(canvas, fullName.c_str());
        canvas->Close();
        delete canvas;
    }
    return paramFunctions;
}