#include "CMSAnalysis/Analysis/interface/Fitter.hh"
#include <TStyle.h>
#include <TH1.h>
#include <TFitResult.h>
#include <TCanvas.h>
#include <TPaveStats.h>
#include <TGraphErrors.h>

Fitter::Fitter(const std::string& histogramFile, const std::string& functionFile, const std::string& parameterFile) 
	: histogramFile(TFile::Open(histogramFile.c_str())), 
	fitOutputFile(TFile::Open(functionFile.c_str(), "RECREATE")), 
	parameterFile(parameterFile) {}

Fitter::~Fitter() 
{
	histogramFile->Close();
	fitOutputFile->Close();
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
		TH1* histogram = readHistogram(funcPair.first);
		if (histogram == nullptr)
		{
			std::cout << "Could not find histogram " << funcPair.first << '\n';
			continue;
		}
		TCanvas* canvas;

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

		fitOutputFile->WriteObject(canvas, func.getName().c_str());
	}

	functions.saveFunctions(parameterFile);
}

void Fitter::setFunctionOutput(const std::string& fileName)
{
	fitOutputFile = TFile::Open(fileName.c_str(), "RECREATE");
}

void Fitter::setParameterOutput(const std::string& fileName)
{
	parameterFile = fileName;
}

TH1* Fitter::readHistogram(const std::string& name)
{
	return dynamic_cast<TH1*>(histogramFile->FindObjectAny(name.c_str()));
}

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
	std::cout << "fitting crystal ball\n";
	TF1* f1 = fitFunction.getFunction();
	TFitResultPtr gausResult = histogram->Fit("gaus", "SQ", "", 250,2000);
	auto params = gausResult->Parameters();
	// TF1* f1 = new TF1 ("f1", DoubleSidedCrystalballFunction, 0, 2000, 6);
	f1->SetNpx(1000);
	double mass = params[1];
	// alpha low, alpha high, n low, n high, mean, sigma, norm
	FitFunction::globalNorm = histogram->Integral("width");

	f1->SetParameters(2.82606, 2.5, 1.08, 1.136, params[1], params[2]);
	f1->SetParNames("alpha_{low}","alpha_{high}","n_{low}", "n_{high}", "mean", "sigma"); 
	f1->SetParLimits(0,0, 10);
	f1->SetParLimits(1,0, 10);
	f1->SetParLimits(2,1,10);
	f1->SetParLimits(3,1,10);
	f1->SetParLimits(4,400,1600);
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
	TFitResultPtr results = histogram->Fit(f1, "SE");
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

std::unique_ptr<std::vector<ParameterizationData>> Fitter::getParameterData(std::unordered_map<std::string, double>& xData)
{
	if (functions.checkFunctionsSimilar() && functions.size() > 0)
	{
		int params = functions.functions.begin()->second.getFunction()->GetNpar();
		auto data = std::make_unique<std::vector<ParameterizationData>>(params);
		for (int i = 0; i < params; ++i) {
			data->push_back(ParameterizationData {
				new double[functions.size()],
				new double[functions.size()],
				new double[functions.size()],
				new double[functions.size()],
				i,
				functions.functions.begin()->second.getFunction()->GetParName(i)
			});
		}

		int i = functions.functions.begin()->second.getFunction()->GetNpar();
		for (auto& pair : functions.functions)
		{
			for (int j = 0; j < params; ++j) 
			{
				data->at(j).x[i] = xData[pair.first];
				data->at(j).y[i] = pair.second.getFunction()->GetParameter(j);
				data->at(j).error[i] = pair.second.getFunction()->GetParError(j);
				data->at(j).zero[i] = 0;
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
	auto graph = new TGraphErrors(parameterData.size, parameterData.x, parameterData.y, parameterData.zero, parameterData.error);
	FitFunction function = FitFunction::createFunctionOfType(FitFunction::FunctionType::POWER_LAW, parameterData.name, "", 0, 2000);

	graph->Fit(function.getFunction(), "S");

	return function;
}

FitFunctionCollection Fitter::parameterizeFunctions(std::unordered_map<std::string, double>& xData)
{
	std::unique_ptr<std::vector<ParameterizationData>> totalParameterData = getParameterData(xData);
	FitFunctionCollection functions;

	for (size_t i = 0; i < totalParameterData->size(); ++i)
	{
		FitFunction func = parameterizeFunction(totalParameterData->at(i));
		functions.insert(func);
	}

	return functions;
}