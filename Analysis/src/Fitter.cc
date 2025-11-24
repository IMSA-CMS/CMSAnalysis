#include "CMSAnalysis/Analysis/interface/Fitter.hh"
#include "CMSAnalysis/Analysis/interface/HistVariable.hh"
#include <Fit/FitResult.h>
#include <TCanvas.h>
#include <TFitResult.h>
#include <TFitResultPtr.h>
#include <TGraphErrors.h>
#include <TH1.h>
#include <TPaveStats.h>
#include <TStyle.h>
#include <algorithm>
#include <array>
#include <boost/algorithm/cxx17/reduce.hpp>
#include <cmath>
#include <stdexcept>
#include <utility>

Fitter::Fitter(const std::string &functionFile, std::string fitTextFile, const std::string &parameterRootFile,
               std::string parameterizationFuncFile)
    : fitRootFile(TFile::Open(functionFile.c_str(), "RECREATE")), fitTextFile(std::move(fitTextFile)),
      parameterRootFile(TFile::Open(parameterRootFile.c_str(), "RECREATE")),
      parameterTextFile(std::move(parameterizationFuncFile))
{
}

Fitter::~Fitter()
{
    if (fitRootFile->IsOpen())
    {
        fitRootFile->Close();
    }
    delete fitRootFile;
    if (parameterRootFile->IsOpen())
    {
        parameterRootFile->Close();
    }
    delete parameterRootFile;
}

void Fitter::setFunctionRootOutput(const std::string &name)
{
    if (fitRootFile->IsOpen())
    {
        fitRootFile->Close();
    }
    delete fitRootFile;

    fitRootFile = TFile::Open(name.c_str(), "RECREATE");
}

void Fitter::setFunctionOutput(std::string name)
{
    fitTextFile = std::move(name);
}

void Fitter::setParameterizationRootOutput(const std::string &name)
{
    if (parameterRootFile->IsOpen())
    {
        parameterRootFile->Close();
    }
    delete parameterRootFile;

    parameterRootFile = TFile::Open(name.c_str(), "RECREATE");
}

void Fitter::setParameterizationOutput(std::string name)
{
    parameterTextFile = std::move(name);
}

void Fitter::loadFunctions(FitFunctionCollection fitFunctions)
{
    functions = std::move(fitFunctions);
}

void Fitter::fitFunctions(std::unordered_map<std::string, TH1 *> &histograms)
{
    for (auto &funcPair : functions.getFunctions())
    {
        FitFunction &func = funcPair.second;
        TH1 *histogram = histograms[funcPair.first];
        if (!histogram)
        {
            throw std::runtime_error("fitter::fitFunctions attempted histogram that does not exist: " + funcPair.first);
        }

        switch (func.getFunctionType())
        {
        case FitFunction::FunctionType::ExpressionFormula:
            fitExpressionFormula(histogram, func);
            break;
        case FitFunction::FunctionType::DoubleSidedCrystalBall:
            fitDSCB(histogram, func);
            break;
        case FitFunction::FunctionType::PowerLaw:
            fitPowerLaw(histogram, func);
            break;
        case FitFunction::FunctionType::DoubleGaussian:
            fitDoubleGaussian(histogram, func);
            break;
        default:
            throw std::invalid_argument("Not a valid FunctionType enum value");
        }

        auto *inner = func.getFunction();
        for (auto par = 0; par < inner->GetNpar(); par++)
        {
            const auto error = std::max(inner->GetParError(par), 0.01 * inner->GetParameter(par));
            inner->SetParError(par, error);
        }

        const auto full = func.getChannelName() + "/" + func.getName();
        const auto split = full.find_last_of('/');
        const std::string dir = full.substr(0, split);
        const auto name = full.substr(split + 1);

        auto canvas = TCanvas(name.c_str(), name.c_str(), 0, 0, 1500, 500);
        histogram->Draw();

        if (!fitDirectories.contains(dir))
        {
            fitDirectories[dir] =
                fitRootFile->mkdir(dir.c_str(), "", true)->GetDirectory(dir.substr(dir.find('/') + 1).c_str());
        }
        fitDirectories.at(dir)->WriteObject(&canvas, name.c_str());

        canvas.Close();
    }
    functions.saveFunctions(fitTextFile, true);
}

// TH1* Fitter::readHistogram(const std::string& name)
// {
// 	return dynamic_cast<TH1*>(histogramFile->FindObjectAny(name.c_str()));
// }

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
        std::cout << "Chi2: " << result->Chi2() << '\n';
        chi2 = result->Chi2();
        fitFunction.getFunction()->SetParameters(result->Parameter(0), result->Parameter(1), result->Parameter(2));
        result = histogram->Fit(fitFunction.getFunction(), "SWLQR", "", fitFunction.getMin(), fitFunction.getMax());
    }

    gStyle->SetOptFit(1111);
}

void Fitter::fitDoubleGaussian(TH1 *histogram, FitFunction &fitFunction)
{

    auto mean = histogram->GetMean();
    auto std = histogram->GetStdDev();

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

    for (int n = 0; n < 4; n++)
    {
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

        if (100 * f1->GetParameter(3) < f1->GetParameter(0))
        {
            auto mean = f1->GetParameter(1);
            auto std = f1->GetParameter(2);
            f1->SetParameter(0, f1->GetParameter(0) / 2);
            f1->SetParameter(1, mean - std);
            f1->SetParameter(3, f1->GetParameter(0) / 2);
            f1->SetParameter(4, mean + std);
            f1->SetParameter(5, std);
        }
        else if (100 * f1->GetParameter(0) < f1->GetParameter(3))
        {
            auto mean = f1->GetParameter(4);
            auto std = f1->GetParameter(5);
            f1->SetParameter(0, f1->GetParameter(3) / 2);
            f1->SetParameter(1, mean - std);
            f1->SetParameter(2, std);
            f1->SetParameter(3, f1->GetParameter(3) / 2);
            f1->SetParameter(4, mean + std);
            f1->SetParameter(5, std);
        }

        f1->SetParLimits(0, 0.0, 0.1);
        f1->SetParLimits(1, mean - 2 * std, mean + std);
        f1->SetParLimits(2, 0.0, 2 * std);
        f1->SetParLimits(3, 0.0, 0.1);
        f1->SetParLimits(4, mean - std, mean + 2 * std);
        f1->SetParLimits(5, 0.0, 2 * std);

        res = histogram->Fit(f1, "SWLQWIDTH", "", fitFunction.getMin(), fitFunction.getMax());
    }

    gStyle->SetOptFit(1111);
}

std::vector<ParameterizationData> Fitter::getParameterData(std::unordered_map<std::string, double> &xData)
{
    if (!functions.checkFunctionsSimilar())
    {
        throw std::invalid_argument("FitFunctionCollection is not comprised on similar functions");
    }

    const int params = functions.getFunctions().begin()->second.getFunction()->GetNpar();
    auto data = std::vector<ParameterizationData>(params);

    for (int i = 0; i < params; ++i)
    {
        data[i] = ParameterizationData{.x = std::vector<double>(functions.size()),
                                       .y = std::vector<double>(functions.size()),
                                       .error = std::vector<double>(functions.size()),
                                       .name = functions.getFunctions().begin()->second.getFunction()->GetParName(i)};
    }

    int i = 0;
    for (auto &pair : functions.getFunctions())
    {
        for (int j = 0; j < params; ++j)
        {
            data[j].x[i] = xData[pair.first];
            data[j].y[i] = pair.second.getFunction()->GetParameter(j);
            data[j].error[i] = pair.second.getFunction()->GetParError(j);
        }
        ++i;
    }
    return data;
}

FitFunction Fitter::parameterizeFunction(ParameterizationData &parameterData, const std::string &genSim,
                                         const std::string &reco, const std::string &var)
{
    const auto channel = reco + "_" + genSim;
    const auto fullName = channel + "/" + parameterData.name;
    auto *const canvas = new TCanvas(fullName.c_str(), fullName.c_str(), 0, 0, 2000, 500);

    auto graph = TGraphErrors(parameterData.x.size(), parameterData.x.data(), parameterData.y.data(), nullptr,
                              parameterData.error.data());

    auto function = FitFunction::createFunctionOfType(FitFunction::FunctionType::PowerLaw, "", "", 0, 2000, channel);

    auto *func = function.getFunction();
    func->SetParameters(boost::algorithm::reduce(parameterData.y) / parameterData.y.size(), 0, 0);
    func->SetParLimits(1, -10000, 0);
    for (int n = 0; n < 4; n++)
    {
        graph.Fit(func, "SQ");
    }

    func->SetRange(0, 2000);
    graph.SetTitle((genSim + " #rightarrow " + reco + " " + var + " ^{}" + parameterData.name).c_str());
    graph.SetMarkerStyle(15);
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
                                   const std::string &reco, const std::string &var)
{
    std::vector<ParameterizationData> totalParameterData = getParameterData(xData);
    FitFunctionCollection paramFunctions;

    for (auto &param : totalParameterData)
    {
        FitFunction func = parameterizeFunction(param, genSim, reco, var);
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
