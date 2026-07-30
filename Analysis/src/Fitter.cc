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

FitFunctionCollection Fitter::fitFunctions(std::unordered_map<std::string, std::pair<TH1*, FitFunction>> &histogramPairs,
    std::string rootFileName)
{
    TFile* rootFile = TFile::Open(rootFileName.c_str(), "RECREATE");
    FitFunctionCollection functions;
    for (auto& histPair : histogramPairs)
    {
        FitFunction& func = histPair.second;
        TH1* histogram = histPair.first;

        fitSingleFunction(histogram, func.getFunction(), rootFileName);

        auto* inner = func.getFunction();

        for (auto par = 0; par < inner->GetNpar(); par++)
        {
            // Set error to at least 1% of the parameter value to avoid zero error
            // Not sure if this is a good idea
            const auto error = std::max(inner->GetParError(par), 0.01 * inner->GetParameter(par));
            inner->SetParError(par, error);
        }

        const auto full = func.getName();
        auto decoded = FitFunction::decodeName(full);
        const std::string dir = decoded["channel"];
        const auto name = decoded["histVar"] + " " + decoded["systematic"];
        // const auto split = full.find_last_of('/');
        // const std::string dir = full.substr(0, split);
        // const auto name = full.substr(split + 1);
        // std::cout << "Name: " << name << '\n';
        auto canvas = TCanvas(name.c_str(), name.c_str(), 0, 0, 1500, 500);
        histogram->Scale(1.0 / histogram->GetBinWidth(1));
        histogram->Draw();
        // std::cout << "Directory: " << dir << '\n';

        if (!rootFile->GetDirectory(dir.c_str()))
        {
            rootFile->mkdir(dir.c_str(), "", true);
        }
        rootFile->GetDirectory(dir.c_str())->WriteObject(&canvas, name.c_str());


        canvas.Close();

        functions.insert(func);
    }
    rootFile->Close();
    delete rootFile;
    functions.saveFunctions(fitTextFile, true);
}

void Fitter::fitSingleFunction(TH1* histogram, TF1* function, size_t iterations)
{
    if (!histogram || !function)
    {
        throw std::runtime_error("fitter::fitFunctions attempted histogram that does not exist: " + histogram->GetName());
    }

    switch (function->getFunctionType())
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
    // TCanvas *c2 = new TCanvas(fitFunction.getName().c_str(),fitFunction.getName().c_str(),0,0,1500,500);
    TF1 *f1 = fitFunction.getFunction();
    // std::cout << "2:1\n";
    // histogram->Draw();
    // std::string wait;
    // std::cin >> wait;
    // std::cout << "Test\n" << std::endl;
    // std::cout << "NEntries: " << histogram->GetEntries() << std::endl;
    // std::cout << "starting\n";
    TFitResultPtr gausResult = histogram->Fit("gaus", "SWLQR", "", fitFunction.getMin(), fitFunction.getMax());
    // std::cout << "finished\n";
    // std::cout << "2:2\n";
    auto params = gausResult->Parameters();
    // std::cout << "2:3\n";

    // TF1* f1 = new TF1 ("f1", DoubleSidedCrystalballFunction, 0, 2000, 6);
    f1->SetNpx(1000);
    // alpha low, alpha high, n low, n high, mean, sigma, norm
    double norm = histogram->Integral(); //("width");
    // std::cout << "2:4\n";

    f1->SetParameters(2.82606, 2.5, 1.08, 1.136, params[1], params[2], norm);
    // std::cout << "2:5\n";

    f1->SetParLimits(0, 0, 10);
    f1->SetParLimits(1, 0, 10);
    f1->SetParLimits(2, 1, 10);
    f1->SetParLimits(3, 1, 10);
    f1->SetParLimits(4, fitFunction.getMin(), fitFunction.getMax());
    f1->FixParameter(6, norm);

    // if(name.substr(8,8) == "eee_eeee" || name.substr(9,8) == "eee_eeee"){
    // 	std::cout<<"EXCEPTION EXCEPTED\n";
    // 	if(name.substr(5,3) == "500"){
    // 		f1->SetParameters(1.624,1.439,1.288,3.151,498.9,5.451);
    // 	}
    // 	else if(name.substr(6,3) == "1300" || name.substr(6,3) == "1500" ){
    // 		f1->SetParLimits(6,3,25);
    // 		std::cout<<"exception found";
    // 	}
    // 	else{
    // 		f1->SetParLimits(6,3,20);
    // 	}
    // }
    f1->SetRange(fitFunction.getMin(), fitFunction.getMax());
    f1->SetLineColor(kRed);
    // std::cout << "2:7\n";
    // std::cout << "staring\n";
    histogram->Fit(f1, "SWLQRBWIDTH");
    // std::cout << "finished\n";
    f1->SetParError(6, norm / (sqrt(histogram->GetEntries())));
    // std::cout << "2:8\n";

    gStyle->SetOptFit(111111);
    // file->WriteObject(c1, name);
    //  std::string Graphname = name + "DBSCball"+ ".png";
    TPaveStats *st = dynamic_cast<TPaveStats *>(histogram->FindObject("stats"));
    st->SetX1NDC(0.1);
    st->SetX2NDC(0.5);
    // histogram->GetXaxis()->SetRange(900, 1100);
    // histogram->SetTitle(("H++ Monte Carlo Invariant Mass Distribution at 1100 GeV for " +
    // fitFunction.getName().substr(5, 4)).c_str()); histogram->GetXaxis()->SetTitle("Same Sign Lepton Pair Invariant
    // Mass (GeV/c^2)"); histogram->GetYaxis()->SetTitle("Number of Events"); histogram->GetYaxis()->SetTitle("Number of
    // Events");

    // c1->SaveAs(Graphname.c_str());
    // c1->Close();
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

void Fitter::fitPowerLawToGraph(TGraph* graph, FitFunction &fitFunction)
{
    auto function =
        FitFunction::createFunctionOfType(FitFunction::FunctionType::PowerLaw, fullName, "", 0, 2000);

    auto *func = function.getFunction();
    func->SetParameters(boost::algorithm::reduce(parameterData.y) / parameterData.y.size(), 0, 0);
    func->SetParLimits(1, -10000, 0);
    for (int n = 0; n < 4; ++n)
    {
        graph.Fit(func, "SQ");
    }

    func->SetRange(0, 2000);
    graph.SetTitle((genSim + " #rightarrow " + reco + " " + var + " ^{}" + parameterData.name).c_str());
    graph.SetMarkerStyle(15);
}

FitFunctionCollection Fitter::parameterizeFunction(std::string name, const std::unordered_map<double, TF1*>& xData)
{
    auto nPoints = xData.size();


    // const auto channel = reco + "_" + genSim;
    // // genSim + "/" + std::to_string(mass) + ' ' + histVar.getName() + " " + systDesc

    // std::string sys;
    // switch (histVar.getSystematicType())
    // {
    // case ScaleFactor::SystematicType::Nominal:
    //     sys = "Nominal";
    //     break;
    // case ScaleFactor::SystematicType::Up:
    //     sys = histVar.getSystematicName() + " Up";
    //     break;
    // case ScaleFactor::SystematicType::Down:
    //     sys = histVar.getSystematicName() + " Down";
    //     break;
    // }
    // std::string proj;
    // if (histVar.isXProjection())
    // {
    //     proj += "X";
    // }
    // if (histVar.isYProjection())
    // {
    //     proj += "Y";
    // }

    // std::map<std::string, std::string> nameParams;
    // nameParams["Reco"] = reco;
    // nameParams["GenSim"] = genSim;
    // nameParams["Systematic"] = sys;
    // if (!proj.empty())
    // {
    //     nameParams["Projection"] = proj;
    // }
    // nameParams["Parameter"] = parameterData.name;

    // const auto fullName = FitFunction::encodeName(nameParams);

    // const auto fullName = channel + "/" + parameterData.name + " " + desc;
    auto *const canvas = new TCanvas(name.c_str(), name.c_str(), 0, 0, 2000, 500);

    // Get data from map
    std::vector<double> xValues;
    std::vector<double> yValues;


    auto graph = TGraphErrors(xData.size(), xData.data(), parameterData.y.data(), nullptr,
                              parameterData.error.data());

    
    graph.Draw("AP");

    gStyle->SetOptFit(1111);

    if (!parameterDirectories.contains(channel))
    {
        parameterDirectories[channel] = parameterRootFile->mkdir(channel.c_str());
    }

    parameterDirectories.at(channel)->WriteObject(canvas, (var + " " + parameterData.name).c_str());
    canvas->Close();

    return function;
}

void Fitter::parameterizeFunctions(std::unordered_map<std::string, double> &xData, const std::string &genSim,
                                   const std::string &reco, const std::string &var, const HistVariable &histVar)
{
    std::vector<ParameterizationData> totalParameterData = getParameterData(xData);
    FitFunctionCollection paramFunctions;

    for (auto &param : totalParameterData)
    {
        FitFunction func = parameterizeFunction(param, genSim, reco, var, histVar);
        paramFunctions.insert(func);
    }

    paramFunctions.saveFunctions(parameterTextFile, true);
}

// TF1 *Fitter::seedInversePowerLaw(double x_0, double y_0, double x_1, double y_1, double x_2, double y_2)
// {
//     // static double range = 1.0;
//     TF1 *powerLaw = new TF1("", "[0]*(x-[1])^[2]");
//     powerLaw->SetParameter(0, y_0);
//     powerLaw->SetParameter(1, x_0 - 1);
//     powerLaw->SetParameter(2, -1);

//     powerLaw->SetParLimits(2, -10, 0);

//     // powerLaw->SetParameter(1, 0);
//     // powerLaw->SetParameter(3, y_2);
//     // double logOne = std::log((y_1 - y_2) / (y_0 - y_2));
//     // double logTwo = std::log(x_1 / x_2);
//     // double c = logOne / logTwo;
//     // double a = (y_0 - y_2) / (std::pow(x_0, c));
//     // powerLaw->SetParameter(2, c);
//     // powerLaw->SetParameter(0, a);

//     return powerLaw;
// }
