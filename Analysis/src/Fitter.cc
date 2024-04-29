#include "CMSAnalysis/Analysis/interface/Fitter.hh"
#include <TStyle.h>
#include <TH1.h>
#include <TFitResult.h>
#include <TCanvas.h>
#include <TPaveStats.h>
#include <TGraphErrors.h>

Fitter::Fitter() {}

Fitter::Fitter(const std::string& functionFile,  const std::string& fitParameterFile) 
	: fitOutputFile(TFile::Open(functionFile.c_str(), "RECREATE")),
	fitParameterFile(fitParameterFile) {}

Fitter::Fitter(const std::string& functionFile, const std::string& fitParameterFile, const std::string& parameterRootFile, const std::string& parameterizationFuncFile)
	: fitOutputFile(TFile::Open(functionFile.c_str(), "RECREATE")), 
	fitParameterFile(fitParameterFile),
	parameterRootFile(TFile::Open(parameterRootFile.c_str(), "RECREATE")),
	parameterizationFunctionFile(parameterizationFuncFile) {}

Fitter::~Fitter() 
{
	if (fitOutputFile->IsOpen())
		fitOutputFile->Close();
	if (parameterRootFile->IsOpen())
		parameterRootFile->Close();
}

void Fitter::setFunctionRootOutput(const std::string& name)
{
	if (fitOutputFile->IsOpen())
		fitOutputFile->Close();

	fitOutputFile = TFile::Open(name.c_str(), "RECREATE");
}

void Fitter::setFunctionOutput(const std::string& name)
{
	fitParameterFile = name;
}

void Fitter::setParameterizationRootOutput(const std::string& name)
{
	if (parameterRootFile->IsOpen())
		parameterRootFile->Close();
	parameterRootFile = TFile::Open(name.c_str(), "RECREATE");
}

void Fitter::setParameterizationOutput(const std::string& name)
{
	parameterizationFunctionFile = name;
}

void Fitter::loadFunctions(const FitFunctionCollection& fitFunctions)
{
	functions = fitFunctions;
}

// void Fitter::loadHistogram(const std::vector<std::string>& histNames)
// {
// 	histograms = histNames;
// }

void Fitter::fitFunctions()
{
	for (auto& funcPair : functions.functions)
	{
		FitFunction& func = funcPair.second;
		// std::cout << "1\n";
		TH1* histogram = histograms[funcPair.first];
		// std::cout << "2\n";
		TCanvas* canvas;
		// std::cout << func << '\n';

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
			default:
				throw std::invalid_argument("Not a valid FunctionType enum value");
		}
		// std::cout << "3\n";
		fitOutputFile->WriteObject(canvas, func.getName().c_str());
		// std::cout << "4\n";
		canvas->Close();
	}
	// std::cout << "5\n";
	functions.saveFunctions(fitParameterFile, true);
	// std::cout << "6\n";
}

// TH1* Fitter::readHistogram(const std::string& name)
// {
// 	return dynamic_cast<TH1*>(histogramFile->FindObjectAny(name.c_str()));
// }

TCanvas* Fitter::fitExpressionFormula(TH1* histogram, FitFunction& fitFunction)
{
	TCanvas *c1 = new TCanvas(fitFunction.getName().c_str(),fitFunction.getName().c_str(),0,0,1500,500);
	c1->SetLogy();

	TFitResultPtr result = histogram->Fit(fitFunction.getFunction(), "SL", "", fitFunction.getMin(), fitFunction.getMax());

	gStyle->SetOptFit(1111);

	return c1;
}
TCanvas* Fitter::fitDSCB(TH1* histogram, FitFunction& fitFunction)
{
	TCanvas *c2 = new TCanvas(fitFunction.getName().c_str(),fitFunction.getName().c_str(),0,0,1500,500);
	TF1* f1 = fitFunction.getFunction();
	// std::cout << "2:1\n";
	// histogram->Draw();
	// std::string wait;
	// std::cin >> wait;
	TFitResultPtr gausResult = histogram->Fit("gaus", "SQ", "", 250,2000);
	// std::cout << "2:2\n";
	auto params = gausResult->Parameters();
	// std::cout << "2:3\n";

	// TF1* f1 = new TF1 ("f1", DoubleSidedCrystalballFunction, 0, 2000, 6);
	f1->SetNpx(1000);
	// alpha low, alpha high, n low, n high, mean, sigma, norm
	double norm = histogram->Integral("width");
	// std::cout << "2:4\n";

	f1->SetParameters(2.82606, 2.5, 1.08, 1.136, params[1], params[2], norm);
	// std::cout << "2:5\n";

	f1->SetParNames("alpha_{low}","alpha_{high}","n_{low}", "n_{high}", "mean", "sigma", "norm"); 
	f1->SetParLimits(0,0, 10);
	f1->SetParLimits(1,0, 10);
	f1->SetParLimits(2,1,10);
	f1->SetParLimits(3,1,10);
	f1->SetParLimits(4,400,1600);
	f1->FixParameter(6,norm);

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
	f1->SetRange(0, 2000);
	f1->SetLineColor(kRed);
	TCanvas *c1 = new TCanvas(fitFunction.getName().c_str(),fitFunction.getName().c_str(),0,0,1500,500);
	// std::cout << "2:7\n";
	
	TFitResultPtr results = histogram->Fit(f1, "SEB");
	f1->SetParError(6, norm / (sqrt(histogram->GetEntries())));
	// std::cout << "2:8\n";

	gStyle->SetOptFit(111111);
	//file->WriteObject(c1, name);
	// std::string Graphname = name + "DBSCball"+ ".png";
	TPaveStats *st = (TPaveStats*)histogram->FindObject("stats");
	st->SetX1NDC(0.1);
	st->SetX2NDC(0.5);
	//hist->GetXaxis()->SetRange(mass-450, mass + 350);
	// c1->SaveAs(Graphname.c_str());
	// c1->Close();

	return c1;
}
TCanvas* Fitter::fitPowerLaw(TH1* histogram, FitFunction& fitFunction)
{
	double initalParams[3] = {std::pow(10, 17), -400, -7};
	fitFunction.getFunction()->SetParameters(initalParams);

	TCanvas *c1 = new TCanvas(fitFunction.getName().c_str(),fitFunction.getName().c_str(),0,0,1500,500);
	c1->SetLogy();

	TFitResultPtr result = histogram->Fit(fitFunction.getFunction(), "SL", "", fitFunction.getMin(), fitFunction.getMax());

	double chi2 = __DBL_MAX__;
	while (result->Chi2() < chi2)
	{
		chi2 = result->Chi2();
		fitFunction.getFunction()->SetParameters(result->Parameter(0), result->Parameter(1), result->Parameter(2));
		result = histogram->Fit(fitFunction.getFunction(), "SL", "", 150, 1500);
	}

	gStyle->SetOptFit(1111);

	return c1;
}

std::vector<ParameterizationData> Fitter::getParameterData(std::unordered_map<std::string, double>& xData)
{
	if (functions.checkFunctionsSimilar())
	{
		int params = functions.functions.begin()->second.getFunction()->GetNpar();
		auto data = std::vector<ParameterizationData>(params);

		for (int i = 0; i < params; ++i) {
			data[i] = ParameterizationData {
				std::vector<double>(functions.size()),
				std::vector<double>(functions.size()),
				std::vector<double>(functions.size()),
				std::vector<double>(functions.size()),
				i,
				functions.functions.begin()->second.getFunction()->GetParName(i)
			};
		}

		int i = 0;
		for (auto& pair : functions.functions)
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
	else
	{
		throw std::invalid_argument("FitFunctionCollection is not comprised on similar functions");
	}
}

FitFunction Fitter::parameterizeFunction(ParameterizationData& parameterData)
{
	std::string in;
	TCanvas* canvas = new TCanvas(parameterData.name.c_str(), parameterData.name.c_str(),0,0,2000,500);
	auto graph = new TGraphErrors(parameterData.x.size(), parameterData.x.data(), parameterData.y.data(), parameterData.zero.data(), parameterData.error.data());
	// std::cout << parameterData.name << '\n';
	// for (size_t i = 0; i < parameterData.x.size(); ++i) 
	// {
	// 	// std::cout << "Actual Data: " << parameterData.x[i] << ' ' << parameterData.y[i] << '\n';
	// 	// std::cout << parameterData.x[i] << '\n';
	// 	// std::cout << parameterData.y[i] << '\n';

	// 	double x, y;
	// 	graph->GetPoint(i, x, y);
	// 	// std::cout << "Graph: " << x << ' ' << y << '\n';

	// }

	TF1* powerLaw = new TF1(parameterData.name.c_str(), "[0]*(x-[1])^[2] + [3]", 0, 2000);
	FitFunction function(powerLaw, FitFunction::FunctionType::EXPRESSION_FORMULA);
	powerLaw->SetParameters(.5, 0, 2, 0);
	graph->SetTitle(parameterData.name.c_str());
	graph->SetMarkerStyle(15);
	graph->Draw("AP");
	auto results = graph->Fit(function.getFunction(), "S");

	gStyle->SetOptFit(1111);
	parameterRootFile->WriteObject(canvas, parameterData.name.c_str());
	canvas->Close();

	return function;
}

void Fitter::parameterizeFunctions(std::unordered_map<std::string, double>& xData, const std::vector<std::string>& paramNames)
{
	std::vector<ParameterizationData> totalParameterData = getParameterData(xData);
	FitFunctionCollection functions;

	// FitFunction funcOne = parameterizeFunction(totalParameterData[1], parameterRootFile);
	// functions.insert(funcOne);
	// FitFunction funcTwo = parameterizeFunction(totalParameterData[0], parameterRootFile);
	// functions.insert(funcTwo);

	for (size_t i = 0; i < totalParameterData.size(); ++i)
	{
		totalParameterData[i].name = paramNames[i];
		FitFunction func = parameterizeFunction(totalParameterData[i]);
		functions.insert(func);
	}

	functions.saveFunctions(parameterizationFunctionFile, true);
}