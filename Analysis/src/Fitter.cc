#include "CMSAnalysis/Analysis/interface/Fitter.hh"
#include "CMSAnalysis/Analysis/interface/SimpleFitFunction.hh"
#include "CMSAnalysis/Analysis/interface/FitFunctionParameterization.hh"
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

FitFunctionCollection Fitter::fitFunctions(const std::vector<std::pair<TH1 *, SimpleFitFunction>> &histogramPairs,
                                           std::string rootFileName)
{
    TFile *rootFile = TFile::Open(rootFileName.c_str(), "RECREATE");
    FitFunctionCollection functions;
    for (const auto &histPair : histogramPairs)
    {
        auto histogram = histPair.first;
        auto func = histPair.second;

        fitSingleFunction(histogram, func);

        auto *inner = func.getFunction();

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

void Fitter::fitSingleFunction(TH1 *histogram, SimpleFitFunction &function, TFile *rootFile)
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
void Fitter::fitExpressionFormula(TH1 *histogram, SimpleFitFunction &fitFunction)
{
    TFitResultPtr result =
        histogram->Fit(fitFunction.getFunction(), "SQRWIDTH", "", fitFunction.getMin(), fitFunction.getMax());
    gStyle->SetOptFit(1111);
}

void Fitter::fitDSCB(TH1 *histogram, SimpleFitFunction &fitFunction)
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

void Fitter::fitPowerLaw(TH1 *histogram, SimpleFitFunction &fitFunction)
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

void Fitter::fitDoubleGaussian(TH1 *histogram, SimpleFitFunction &fitFunction)
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

void Fitter::fitGausLogPowerNorm(TH1 *const hist, SimpleFitFunction &func)
{
    TF1 *const f1 = func.getFunction();

    // Params: mult, u, sigma1, s, n
    f1->SetParameters(1, hist->GetMean(), hist->GetStdDev(), 1, 2);
    f1->SetParLimits(1, 0, func.getMax());

    f1->SetNpx(1000);

    hist->Fit(f1, "SWLQWIDTH", "", func.getMin(), func.getMax());

    gStyle->SetOptFit(1111);
}

void Fitter::fitVoigt(TH1 *histogram, SimpleFitFunction &fitFunction)
{
    TF1 *f1 = fitFunction.getFunction();
    const double fitMin = fitFunction.getMin();
    const double fitMax = fitFunction.getMax();
    const double fitRange = fitMax - fitMin;
    const double mean = histogram->GetMean();
    const double stdDev = histogram->GetStdDev();
    const double norm = histogram->Integral("width");
    // sketchy
    f1->SetParameters(norm / 2, mean, stdDev, stdDev);
    f1->SetParLimits(0, 0.0, norm);
    f1->SetParLimits(1, fitMin, fitMax);
    f1->SetParLimits(2, 1e-6, fitRange);
    f1->SetParLimits(3, 1e-6, fitRange);
    f1->SetNpx(1000);
    histogram->Fit(f1, "SWLQWIDTH", "", fitMin, fitMax);
    gStyle->SetOptFit(1111);
}

SimpleFitFunction Fitter::fitPowerLawToGraph(TGraph *graph, std::string name)
{
    auto function =
        SimpleFitFunction::createFunctionOfType(FitFunction::FunctionType::PowerLaw, name, "", 0, 2000);

    auto *func = function.getFunction();

    func->SetParLimits(1, -10000, 0);
    for (int n = 0; n < 4; ++n)
    {
        graph->Fit(func, "SQ");
    }

    func->SetRange(0, 2000);
    return function;
}

FitFunctionParameterization Fitter::parameterizeFunction(std::string name,
    const std::unordered_map<double, SimpleFitFunction *> &xData, TFile *rootFile)
{
    if (xData.empty())
        return FitFunctionParameterization();

    auto *firstFunction = xData.begin()->second;

    if (!firstFunction)
    {
        return FitFunctionParameterization();
    }

    auto *firstTF1 = firstFunction->getFunction();
    double min = 0;
    double max = 0;
    firstTF1->GetRange(min, max);
    const char *rawFormula = firstTF1->GetExpFormula();
    const std::string expFormula = rawFormula == nullptr ? "" : rawFormula;

    //just add the additional metadata
    auto parameterizationMetadata = FitFunction::decodeName(name);
    parameterizationMetadata["IsParameterization"] = "true";
    const std::string parameterizationName = FitFunction::encodeName(parameterizationMetadata);

    FitFunctionParameterization parameterization(
        parameterizationName, parameterizationMetadata["Channel"], firstFunction->getFunctionType(),
        expFormula, min, max);

    const auto nParams = firstTF1->GetNpar();
    const auto systematics = firstFunction->listSystematics();
    // auto nPoints = xData.size();
    // auto nParams = xData.begin()->second->GetNpar();
    // up
    
    for (int i = 0; i < nParams; ++i)
    {
        std::vector<double> xValues;
        std::vector<double> yValues;
        std::vector<double> errors;

        for (const auto &[x, fitFunction] : xData)
        {
            if (!fitFunction)
            {
                continue;
            }

            auto *func = fitFunction->getFunction();

            xValues.push_back(x);
            yValues.push_back(func->GetParameter(i));
            errors.push_back(func->GetParError(i));
        }

        if (xValues.empty())
        {
            continue;
        }

        TGraphErrors graph(xValues.size(), xValues.data(), yValues.data(), nullptr, errors.data());
        std::string fullName = name + " parameter " + firstTF1->GetParName(i);
        graph.SetTitle(fullName.c_str());

        std::map<std::string, std::string> metadata;
        metadata["ParameterIndex"] = std::to_string(i);
        metadata["Parameter"] = firstTF1->GetParName(i);
        auto fit = fitPowerLawToGraph(&graph, FitFunction::encodeName(metadata));

        // Systematics part
        for (const auto &systematic : systematics)
        {
            SimpleFitFunction upFit;
            SimpleFitFunction downFit;
            bool hasUp = false;
            bool hasDown = false;

            // this is for up
            std::vector<double> upXValues;
            std::vector<double> upYValues;
            std::vector<double> upErrors;

            for (const auto &[x, fitFunction] : xData)
            {
                if (!fitFunction)
                    continue;

                const TF1 *systematicFunction =
                    fitFunction->getSystematic(systematic, true);

                if (!systematicFunction)
                    continue;

                upXValues.push_back(x);
                upYValues.push_back(systematicFunction->GetParameter(i));
                upErrors.push_back(systematicFunction->GetParError(i));
            }

            if (!upXValues.empty())
            {
                TGraphErrors upGraph(
                    upXValues.size(),
                    upXValues.data(),
                    upYValues.data(),
                    nullptr,
                    upErrors.data());

                std::string upName =
                    name + " " + systematic + " Up parameter " + firstTF1->GetParName(i);

                upGraph.SetTitle(upName.c_str());

                upFit = fitPowerLawToGraph(&upGraph, upName);
                hasUp = true;
            }

            // this is for down
            std::vector<double> downXValues;
            std::vector<double> downYValues;
            std::vector<double> downErrors;

            for (const auto &[x, fitFunction] : xData)
            {
                if (!fitFunction)
                    continue;

                const TF1 *systematicFunction =
                    fitFunction->getSystematic(systematic, false);

                if (!systematicFunction)
                    continue;

                downXValues.push_back(x);
                downYValues.push_back(systematicFunction->GetParameter(i));
                downErrors.push_back(systematicFunction->GetParError(i));
            }

            if (!downXValues.empty())
            {
                TGraphErrors downGraph(
                    downXValues.size(),
                    downXValues.data(),
                    downYValues.data(),
                    nullptr,
                    downErrors.data());

                std::string downName =
                    name + " " + systematic + " Down parameter " + firstTF1->GetParName(i);

                downGraph.SetTitle(downName.c_str());

                downFit = fitPowerLawToGraph(&downGraph, downName);
                hasDown = true;
            }

            // put the Up and Down functions to thecentral fit
            if (hasUp && hasDown)
            {
                fit.addSystematic(
                    systematic,
                    *upFit.getFunction(),
                    *downFit.getFunction());
            }
        }
        parameterization.insert(fit);

        auto *const canvas = new TCanvas(fullName.c_str(), fullName.c_str(), 0, 0, 2000, 500);

        graph.Draw("AP");

        gStyle->SetOptFit(1111);

        if (rootFile)
        {
            rootFile->WriteObject(canvas, fullName.c_str());
        }

        // rootFile->WriteObject(canvas, fullName.c_str());
        canvas->Close();
        delete canvas;
    }
    return parameterization;
}
