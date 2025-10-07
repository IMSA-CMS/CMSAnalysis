#include "CMSAnalysis/Analysis/interface/Fitter.hh"
#include <Fit/FitResult.h>
#include <TCanvas.h>
#include <TFitResult.h>
#include <TFitResultPtr.h>
#include <TGraphErrors.h>
#include <TH1.h>
#include <TPaveStats.h>
#include <TStyle.h>
#include <array>
#include <cmath>

Fitter::Fitter()
{
}

Fitter::Fitter(const std::string &functionFile, const std::string &fitTextFile)
    : fitRootFile(TFile::Open(functionFile.c_str(), "RECREATE")), fitTextFile(fitTextFile)
{
}

Fitter::Fitter(const std::string &functionFile, const std::string &fitTextFile, const std::string &parameterRootFile,
               const std::string &parameterizationFuncFile)
    : fitRootFile(TFile::Open(functionFile.c_str(), "RECREATE")), fitTextFile(fitTextFile),
      parameterRootFile(TFile::Open(parameterRootFile.c_str(), "RECREATE")), parameterTextFile(parameterizationFuncFile)
{
}

Fitter::~Fitter()
{
    if (fitRootFile->IsOpen())
    {
        fitRootFile->Close();
    }
    if (parameterRootFile->IsOpen())
    {
        parameterRootFile->Close();
    }
}

void Fitter::setFunctionRootOutput(const std::string &name)
{
    if (fitRootFile->IsOpen())
    {
        fitRootFile->Close();
    }

    fitRootFile = TFile::Open(name.c_str(), "RECREATE");
}

void Fitter::setFunctionOutput(const std::string &name)
{
    fitTextFile = name;
}

void Fitter::setParameterizationRootOutput(const std::string &name)
{
    if (parameterRootFile->IsOpen())
    {
        parameterRootFile->Close();
    }
    parameterRootFile = TFile::Open(name.c_str(), "RECREATE");
}

void Fitter::setParameterizationOutput(const std::string &name)
{
    parameterTextFile = name;
}

void Fitter::loadFunctions(const FitFunctionCollection &fitFunctions)
{
    functions = fitFunctions;
}

// void Fitter::loadHistogram(const std::vector<std::string>& histNames)
// {
// 	histograms = histNames;
// }

void Fitter::fitFunctions()
{
    for (auto &funcPair : functions.getFunctions())
    {
        FitFunction &func = funcPair.second;
        TH1 *histogram = histograms[funcPair.first];
        if (!histogram)
        {
            throw std::runtime_error("fitter::fitFunctions attempted histogram that does not exist: " + funcPair.first);
        }

        TCanvas *canvas;
        switch (func.getFunctionType())
        {
        case FitFunction::FunctionType::EXPRESSION_FORMULA:
            canvas = fitExpressionFormula(histogram, func);
            break;
        case FitFunction::FunctionType::DOUBLE_SIDED_CRYSTAL_BALL:
            canvas = fitDSCB(histogram, func);
            break;
        case FitFunction::FunctionType::POWER_LAW:
            canvas = fitPowerLaw(histogram, func);
            break;
        case FitFunction::FunctionType::DOUBLE_GAUSSIAN:
            canvas = fitDoubleGaussian(histogram, func);
            break;
        default:
            throw std::invalid_argument("Not a valid FunctionType enum value");
        }
        size_t pos = func.getName().find('/');
        if (pos != std::string::npos)
        {
            std::string dir = func.getName().substr(0, pos);
            std::string name = func.getName().substr(pos + 1);
            auto pos = fitDirectories.find(dir);
            if (pos == fitDirectories.end())
            {
                fitDirectories[dir] = fitRootFile->mkdir(dir.c_str());
            }
            fitDirectories[dir]->WriteObject(canvas, name.c_str());
        }
        else
        {
            fitRootFile->WriteObject(canvas, func.getName().c_str());
        }
        canvas->Close();
    }
    functions.saveFunctions(fitTextFile, true);
}

// TH1* Fitter::readHistogram(const std::string& name)
// {
// 	return dynamic_cast<TH1*>(histogramFile->FindObjectAny(name.c_str()));
// }

// not sure if this is used at all
TCanvas *Fitter::fitExpressionFormula(TH1 *histogram, FitFunction &fitFunction)
{
    TCanvas *c1 = new TCanvas(fitFunction.getName().c_str(), fitFunction.getName().c_str(), 0, 0, 1500, 500);
    c1->SetLogy();

    TFitResultPtr result =
        histogram->Fit(fitFunction.getFunction(), "SQRWIDTH", "", fitFunction.getMin(), fitFunction.getMax());

    gStyle->SetOptFit(1111);

    return c1;
}

TCanvas *Fitter::fitDSCB(TH1 *histogram, FitFunction &fitFunction)
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

    f1->SetParNames("alpha_{low}", "alpha_{high}", "n_{low}", "n_{high}", "mean", "sigma", "norm");
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
    TCanvas *c1 = new TCanvas(fitFunction.getName().c_str(), fitFunction.getName().c_str(), 0, 0, 1500, 500);
    // std::cout << "2:7\n";
    // std::cout << "staring\n";
    histogram->Fit(f1, "SWLQRBWIDTH");
    // std::cout << "finished\n";
    f1->SetParError(6, norm / (sqrt(histogram->GetEntries())));
    // std::cout << "2:8\n";

    gStyle->SetOptFit(111111);
    // file->WriteObject(c1, name);
    //  std::string Graphname = name + "DBSCball"+ ".png";
    TPaveStats *st = (TPaveStats *)histogram->FindObject("stats");
    st->SetX1NDC(0.1);
    st->SetX2NDC(0.5);
    // histogram->GetXaxis()->SetRange(900, 1100);
    // histogram->SetTitle(("H++ Monte Carlo Invariant Mass Distribution at 1100 GeV for " +
    // fitFunction.getName().substr(5, 4)).c_str()); histogram->GetXaxis()->SetTitle("Same Sign Lepton Pair Invariant
    // Mass (GeV/c^2)"); histogram->GetYaxis()->SetTitle("Number of Events"); histogram->GetYaxis()->SetTitle("Number of
    // Events");

    // c1->SaveAs(Graphname.c_str());
    // c1->Close();

    return c1;
}
TCanvas *Fitter::fitPowerLaw(TH1 *histogram, FitFunction &fitFunction)
{
    std::array<double, 3> initalParams = {{1e17, 0, -5}};
    fitFunction.getFunction()->SetParameters(initalParams.data());

    TCanvas *c1 = new TCanvas(fitFunction.getName().c_str(), fitFunction.getName().c_str(), 0, 0, 1500, 500);
    c1->SetLogy();

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

    return c1;
}

TCanvas *Fitter::fitDoubleGaussian(TH1 *histogram, FitFunction &fitFunction)
{
    TF1 *f1 = fitFunction.getFunction();

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

    f1->SetParameters(LowGausMul, LowGausMean, LowGausSigma, HighGausMul, HighGausMean, HighGausSigma);
    f1->SetParNames("mul_{1}", "\\mu_{1}", "\\sigma_{1}", "mul_{2}", "\\mu_{2}", "\\sigma_{2}");

    f1->SetNpx(1000);

    TCanvas *c1 = new TCanvas(fitFunction.getName().c_str(), fitFunction.getName().c_str(), 0, 0, 1500, 500);

    TFitResultPtr res;

    for (int n = 0; n < 10; n++)
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
    return c1;
}

std::vector<ParameterizationData> Fitter::getParameterData(std::unordered_map<std::string, double> &xData)
{
    if (!functions.checkFunctionsSimilar())
    {
        throw std::invalid_argument("FitFunctionCollection is not comprised on similar functions");
    }

    int params = functions.getFunctions().begin()->second.getFunction()->GetNpar();
    auto data = std::vector<ParameterizationData>(params);

    for (int i = 0; i < params; ++i)
    {
        data[i] = ParameterizationData{std::vector<double>(functions.size()),
                                       std::vector<double>(functions.size()),
                                       std::vector<double>(functions.size()),
                                       std::vector<double>(functions.size()),
                                       i,
                                       functions.getFunctions().begin()->second.getFunction()->GetParName(i)};
    }

    int i = 0;
    for (auto &pair : functions.getFunctions())
    {
        for (int j = 0; j < params; ++j)
        {
            data[j].x[i] = xData[pair.first];
            data[j].y[i] = pair.second.getFunction()->GetParameter(j);
            data[j].error[i] = pair.second.getFunction()->GetParError(j);
            data[j].zero[i] = 0;
        }
        ++i;
    }
    return data;
}

FitFunction Fitter::parameterizeFunction(ParameterizationData &parameterData)
{
    auto *canvas = new TCanvas(parameterData.name.c_str(), parameterData.name.c_str(), 0, 0, 2000, 500);
    auto *graph = new TGraphErrors(parameterData.x.size(), parameterData.x.data(), parameterData.y.data(),
                                   parameterData.zero.data(), parameterData.error.data());

    auto *func = new TF1("Power Law", FitFunction::powerLaw, 0, 2000, 3);
    func->SetParameter(1, 0);
    func->SetParLimits(1, -10000, 0);
    for (int n = 0; n < 10; n++)
    {
        graph->Fit(func, "SQ");
    }

    func->SetRange(0, 2000);
    func->SetName(parameterData.name.c_str());
    graph->SetTitle(parameterData.name.c_str());
    graph->SetMarkerStyle(15);
    graph->Draw("AP");

    auto function = FitFunction(func, FitFunction::FunctionType::POWER_LAW);

    gStyle->SetOptFit(1111);

    size_t pos = function.getName().find('/');
    if (pos != std::string::npos)
    {
        std::string dir = function.getName().substr(0, pos);
        std::string name = function.getName().substr(pos + 1);
        auto pos = parameterDirectories.find(dir);
        if (pos == parameterDirectories.end())
        {
            parameterDirectories[dir] = parameterRootFile->mkdir(dir.c_str());
        }
        std::cout << "writing object " << name << "\n";

        parameterDirectories[dir]->WriteObject(canvas, name.c_str());
    }
    else
    {
        parameterRootFile->WriteObject(canvas, parameterData.name.c_str());
    }

    canvas->Close();

    return function;
}

void Fitter::parameterizeFunctions(std::unordered_map<std::string, double> &xData, const std::string &channel)
{
    std::vector<ParameterizationData> totalParameterData = getParameterData(xData);
    FitFunctionCollection paramFunctions;

    for (auto param : totalParameterData)
    {
        param.name = channel + "/" + param.name;
        std::cout << "Parameterizing " << param.name << '\n';
        FitFunction func = parameterizeFunction(param);
        paramFunctions.insert(func);
    }

    paramFunctions.saveFunctions(parameterTextFile, true);
}

TF1 *Fitter::seedInversePowerLaw(double x_0, double y_0, double x_1, double y_1, double x_2, double y_2)
{
    // static double range = 1.0;
    TF1 *powerLaw = new TF1("", "[0]*(x-[1])^[2]");
    powerLaw->SetParameter(0, y_0);
    powerLaw->SetParameter(1, x_0 - 1);
    powerLaw->SetParameter(2, -1);

    powerLaw->SetParLimits(2, -10, 0);

    // powerLaw->SetParameter(1, 0);
    // powerLaw->SetParameter(3, y_2);
    // double logOne = std::log((y_1 - y_2) / (y_0 - y_2));
    // double logTwo = std::log(x_1 / x_2);
    // double c = logOne / logTwo;
    // double a = (y_0 - y_2) / (std::pow(x_0, c));
    // powerLaw->SetParameter(2, c);
    // powerLaw->SetParameter(0, a);

    return powerLaw;
}
