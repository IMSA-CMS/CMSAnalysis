#include "CMSAnalysis/Analysis/interface/FitFunctionCollection.hh"
#include "CMSAnalysis/Analysis/interface/PlotFormatter.hh"
#include "CMSAnalysis/Analysis/interface/HistVariable.hh"
#include "CMSAnalysis/Analysis/interface/HiggsCompleteAnalysis.hh"
#include <TF1.h>
#include <fstream>
#include <string>


// Indices in signal_params for each parameter (Note, this is copied from RooPDF_DSCB_test in LimitSetting 
// since parameters are also alphabetized)
const int ALPHA_L = 1;
const int ALPHA_H = 0;
const int N_L = 5;
const int N_H = 4;
const int MEAN = 2;
const int SIGMA = 3;
const int NORM = 6;

const double mass = 1000;

std::map<int, int> dscbParameterIndices = {
	{0, ALPHA_L}, {1, ALPHA_H}, {2, N_L}, 
	{3, N_H}, {4, MEAN}, {5, SIGMA}, {6, NORM}};



bool containsSubstring(const std::string& text, const std::string& pattern) {
    return text.find(pattern) != std::string::npos;
}


FitFunction createSignalDSCB(std::string channel, std::string xOrY, FitFunctionCollection *signalCollection, double mass)
{
	const double min = 0;
	const double max = 2000;


	auto functionsInCollection = signalCollection->getFunctions();
	std::vector<FitFunction> channelFunctions = {};
	std::vector<FitFunction> sortedChannelFunctions = {};

	// Find functions in channel
	for (auto pair : functionsInCollection)
	{
		auto function = pair.second;
		//std::cout << "Channel name: " << function.getChannel() << "\n";
		std::cout << "Function name: " << function.getName() << "\n";
		bool isXOrY = containsSubstring(function.getName(), xOrY);
		if (function.getChannel() == channel && isXOrY)
		{
			channelFunctions.push_back(function);
		}
	}

	if (channelFunctions.size() == 0)
	{
		std::cout << "No functions found for " << channel << " channel\n";
	}

	
	// Determine order of parameters
	std::vector<std::string> functionNames = {};
	for (auto function : channelFunctions)
	{
		functionNames.push_back(function.getParameterName());
	}
	std::sort(functionNames.begin(), functionNames.end());
	
	// Sort functions alphabetically
	for (std::string functionName: functionNames)
	{
		for (auto function : channelFunctions)
		{
			if (function.getParameterName() == functionName)
			{
				sortedChannelFunctions.push_back(function);	
			}
		}
	}

	// Print Names of functions:
	for (auto function : sortedChannelFunctions)
	{
		std::cout << "Parameter name: " << function.getParameterName() << "\n";
	}

	// Evaluate parameters at mass point:
	double alpha_l = sortedChannelFunctions[ALPHA_L].evaluate(mass);
	double alpha_h = sortedChannelFunctions[ALPHA_H].evaluate(mass);
	double n_l = sortedChannelFunctions[N_L].evaluate(mass);
	double n_h = sortedChannelFunctions[N_H].evaluate(mass);
	double mean = sortedChannelFunctions[MEAN].evaluate(mass);
	double sigma = sortedChannelFunctions[SIGMA].evaluate(mass);
	double norm = sortedChannelFunctions[NORM].evaluate(mass);

	// Construct DSCB
	FitFunction signalDSCB = FitFunction::createFunctionOfType(FitFunction::FunctionType::DoubleSidedCrystalBall, channel, "", min, max,"eeee");
	signalDSCB.getFunction()->SetParameter(0, alpha_l);
	signalDSCB.getFunction()->SetParameter(1, alpha_h);
	signalDSCB.getFunction()->SetParameter(2, n_l);
	signalDSCB.getFunction()->SetParameter(3, n_h);
	signalDSCB.getFunction()->SetParameter(4, mean);
	signalDSCB.getFunction()->SetParameter(5, sigma);
	signalDSCB.getFunction()->SetParameter(6, norm);

	return signalDSCB;



}

void graphFunctionFromCollection(FitFunctionCollection *functionCollection, std::string functionName)
{
	auto functions = functionCollection->getFunctions();
	bool functionFound = false;
	for (auto pair : functions)
	{
		if (functionName == pair.first)
		{
			auto function = pair.second;
			TF1* tf1Reference = function.getFunction();
			tf1Reference->SetLineColor(kBlue);
			tf1Reference->SetRange(0, 2000);
			tf1Reference->DrawCopy("L");
			functionFound = true;
		}
	}
	if (functionFound)
	{
		std::cout << "Graphing " << functionName << "...\n";
	}
	else
	{
		std::cout << functionName << " not found!\n";
	}
}





void ParameterizationFunctionGrapher()
{
	std::map<std::string, std::string> xOrYMap = {{"X", "h_mll1"}, {"Y", "h_mll2"}};
	std::string x = xOrYMap["X"];
	std::string y = xOrYMap["Y"];

	// auto analysis = std::make_shared<HiggsCompleteAnalysis>();
	// PlotFormatter plotter;
	// plotter.completePlot(analysis, HistVariable(HistVariable::VariableType::InvariantMass, "", true, false),
	// 	"Mass", "Events", false, true, false, "eeee");
	std::string signalParametersFile = "/uscms/home/bhobbs/Analysis/CMSSW_15_0_4/src/CMSAnalysis/Analysis/bin/fitting/H++SignalParameterFunctions.txt";  
	std::string backgroundParametersFile = "fitting/H++BackgroundFunctions.txt";
	//std::string backgroundParametersFile = "/uscms/home/bhobbs/Analysis/CMSSW_15_0_4/src/CMSAnalysis/Analysis/bin/fitting/OtherBackgroundFunctions.txt"; 
	//std::vector<std::string> channelsToCheck = {"eeee", "uuuu"};
	auto signalParameters = FitFunctionCollection::loadFunctions(signalParametersFile); 
	auto backgroundParameters = FitFunctionCollection::loadFunctions(backgroundParametersFile);
	auto backgroundParameterFunctions = backgroundParameters.getFunctions();

	// Arguments for graphing individual signal and background functions
	std::string signalFunction = "eeee_eeee/#mu h_mll1";
	//std::string signalXOrY = "X";
	std::string backgroundFunction = "eeee/ZZ Background h_mll1";
	
	// Arguments for graphing signal DSCB
	std::string signalXOrY = "X";
	std::string channel = "eeee_eeee";
	double mass = 1000;


	
	// Graph a signal function
	//graphFunctionFromCollection(&signalParameters, signalFunction);

	// Graph a background function
	//graphFunctionFromCollection(&backgroundParameters, backgroundFunction); 

	// Graph signal channel function
	// auto signalDSCB = createSignalDSCB(channel, xOrYMap[signalXOrY], &signalParameters, mass);
	// signalDSCB.getFunction()->DrawCopy("LSAME");



	// Create signal channel functions
	auto eeeeChannel = createSignalDSCB("eeee_eeee", x, &signalParameters, mass);
	//auto uuuuChannel = createSignalDSCB("mmmm_mmmm", x, &signalParameters, mass); 
	std::vector<FitFunction> signalFitFunctions = {};
	signalFitFunctions.push_back(eeeeChannel);
	//signalFitFunctions.push_back(uuuuChannel);

	std::vector<FitFunction> backgroundFitFunctions = {};
	std::vector<std::string> backgroundChannels = {"eeee"};
	std::vector<std::string> exclude = {"ZPeak", "Up", "Down", "Y Projection"};
	for (std::string channel : backgroundChannels)
	{
		for (auto& pair : backgroundParameterFunctions)
		{
			auto& function = pair.second;
			// std::cout << "function.getChannel(): " << function.getChannel() << "\n";
			// std::cout << "function.getParameterName(): " << function.getParameterName() << "\n";
			bool excludeFromGraph = false;
			for (std::string stringToExclude : exclude)
			{
				if (containsSubstring(function.getChannel(), stringToExclude) || 
				containsSubstring(function.getParameterName(), stringToExclude))
				{
					excludeFromGraph = true;
				}
			}
			if (containsSubstring(function.getChannel(), channel) && !excludeFromGraph)
			{
				backgroundFitFunctions.push_back(function);
			}
		}
	}
	std::cout << "backgroundFitFunctions size: " << backgroundFitFunctions.size() << "\n";
	std::vector<TF1*> signalAndBackgroundTf1s = {};
	for (auto& signalFitFunction : signalFitFunctions)
	{
		auto tf1Reference = signalFitFunction.getFunction();
		signalAndBackgroundTf1s.push_back(tf1Reference);
	}
	for (auto& backgroundFitFunction : backgroundFitFunctions)
	{
		auto tf1Reference = backgroundFitFunction.getFunction();
		signalAndBackgroundTf1s.push_back(tf1Reference);
	}
	auto analysis = std::make_shared<HiggsCompleteAnalysis>();
	PlotFormatter plotter = PlotFormatter(true, "test");
	std::cout << "\n \n \n \neeee tf1: " << signalAndBackgroundTf1s[0]->GetName() << "\n \n \n \n";
	plotter.completePlot(analysis, HistVariable(HistVariable::VariableType::InvariantMass, "", true, false),
		"Mass", "Events", false, true, false, "eeee", signalAndBackgroundTf1s);


	// std::cout << "Organizing functions...\n";
	// for (const auto& [key, value] : signalParameters)
	// {
	// 	std::string << 
	// }

	// // Alphabetize parameters for each channel
	// std::vector<std::string> functionNames = {};
	// for (std::string channel : channelsToCheck)
	// {
	// 	std::vector<FitFunction> alphabetizedChannelFunctions = {};
	// 	auto& channelFunctions = signalParameters[channel];

		
	// 	std::vector<std::string> functionNames = {};
	// 	std::cout << "signalParameters[channel] size: " << signalParameters[channel].size() << "\n";
	// 	//std::cout << "channelFunctions size: " << channelFunctions.size() << "\n";
	// 	for (auto signalFunction : channelFunctions)
	// 	{
	// 		functionNames.push_back(signalFunction.getParameterName());
	// 	}
	// 	std::sort(functionNames.begin(), functionNames.end());
	// 	//std::cout << "functionNames size: " << functionNames.size() << "\n";
	// 	//std::cout << "Current Parameter: " << signalFunction.getParameterName() << "\n";
	// 	for (std::string sortedFunctionName : functionNames)
	// 	{
	// 		std::cout << "Current Parameter: " << sortedFunctionName << "\n";
	// 		for (auto signalFunction: channelFunctions)
	// 		{
	// 			std::cout << signalFunction.getParameterName() << "\n";

	// 			if (sortedFunctionName == signalFunction.getParameterName())
	// 			{
	// 				alphabetizedChannelFunctions.push_back(signalFunction);
	// 			}
	// 		}
	// 	}
	// 	signalParameters[channel] = alphabetizedChannelFunctions;
	// }

	// // Create DSCB signal functions for each channel and draws
	// std::vector<FitFunction> signalDSCBFunctions;
	// for (std::string channel : channelsToCheck)
	// {
	// 	// Checks number of functions
	// 	std::cout << channel << " has "
    //       << signalParameters[channel].size()
    //       << " signal parameter functions\n";

	// 	for (size_t j = 0; j < signalParameters[channel].size(); ++j) {
    // 		std::cout << "  [" << j << "] "
    //           	<< signalParameters[channel][j].getParameterName()
    //           	<< "\n";
	// 	}

	// 	std::cout << "Creating " << channel << "DSCB signal functions...\n";
	// 	FitFunction::FunctionType dscb = FitFunction::FunctionType::DoubleSidedCrystalBall;
	// 	FitFunction dscbFunction = FitFunction::createFunctionOfType(dscb, channel, "", 0, 2000, channel);
	// 	std::cout << channel << " DSCB parameters: \n";
	// 	for (int i = 0; i < 7; i++)
	// 	{
	// 		double parameterValue = signalParameters[channel][dscbParameterIndices[i]].getFunction()->Eval(mass);
	// 		std::cout << "TEST STATEMENT" << "\n";
	// 		dscbFunction.getFunction()->SetParameter(i, parameterValue);
	// 		std::cout << dscbFunction.getFunction()->GetParameter(i) << "\n"; 
	// 	}
	// 	dscbFunction.getFunction()->Draw("L");
		
	// }

	// std::cout << "signalDSCBFunctions parameters:\n";
	// for (int i = 0; i < 7; i++)
	// {
	// 	std::cout << signalDSCBFunctions[0]->getFunction()->GetParameter(i) << "\n";
	// }

	// signalDSCBFunctions[0]->getFunction()->Draw("L");
	// Draw functions
	// for (auto signalFunction : signalDSCBFunctions)
	// {
	// 	std::cout << "Graphing signal function..." << "\n";
	// 	signalFunction.getFunction()->Draw("SAMEL");
	// }

	// for (auto pair : backgroundParameterFunctions)
	// {
	// 	//auto functionName = pair.first;
	// 	std::cout << "Graphing background function..." << "\n";
	// 	auto function = pair.second;
	// 	TF1* tf1Reference = function.getFunction();
	// 	TF1 tf1Function = *tf1Reference;
	// 	tf1Function.Draw("SAMEL");
	// }


}