#include "CMSAnalysis/Analysis/interface/FitFunctionCollection.hh"
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

void ParameterizationFunctionGrapher()
{
	std::string signalParametersFile = "/uscms/home/bhobbs/Analysis/CMSSW_15_0_4/src/CMSAnalysis/Analysis/bin/fitting/H++SignalParameterFunctions.txt";  
	std::string backgroundParametersFile = "/uscms/home/bhobbs/Analysis/CMSSW_15_0_4/src/CMSAnalysis/Analysis/bin/fitting/OtherBackgroundFunctions.txt"; 
	std::vector<std::string> channelsToCheck = {"eeee", "uuuu"};



	auto signalParameters = FitFunctionCollection::getFunctionsSortedByChannel(signalParametersFile); 
	auto backgroundParameters = FitFunctionCollection::loadFunctions(backgroundParametersFile);
	auto backgroundParameterFunctions = backgroundParameters.getFunctions();

	std::cout << "Organizing functions...\n";
	for (const auto& [key, value] : signalParameters)
	{
		std::string << 
	}

	// Alphabetize parameters for each channel
	std::vector<std::string> functionNames = {};
	for (std::string channel : channelsToCheck)
	{
		std::vector<FitFunction> alphabetizedChannelFunctions = {};
		auto& channelFunctions = signalParameters[channel];

		
		std::vector<std::string> functionNames = {};
		std::cout << "signalParameters[channel] size: " << signalParameters[channel].size() << "\n";
		//std::cout << "channelFunctions size: " << channelFunctions.size() << "\n";
		for (auto signalFunction : channelFunctions)
		{
			functionNames.push_back(signalFunction.getParameterName());
		}
		std::sort(functionNames.begin(), functionNames.end());
		//std::cout << "functionNames size: " << functionNames.size() << "\n";
		//std::cout << "Current Parameter: " << signalFunction.getParameterName() << "\n";
		for (std::string sortedFunctionName : functionNames)
		{
			std::cout << "Current Parameter: " << sortedFunctionName << "\n";
			for (auto signalFunction: channelFunctions)
			{
				std::cout << signalFunction.getParameterName() << "\n";

				if (sortedFunctionName == signalFunction.getParameterName())
				{
					alphabetizedChannelFunctions.push_back(signalFunction);
				}
			}
		}
		signalParameters[channel] = alphabetizedChannelFunctions;
	}

	// Create DSCB signal functions for each channel and draws
	std::vector<FitFunction> signalDSCBFunctions;
	for (std::string channel : channelsToCheck)
	{
		// Checks number of functions
		std::cout << channel << " has "
          << signalParameters[channel].size()
          << " signal parameter functions\n";

		for (size_t j = 0; j < signalParameters[channel].size(); ++j) {
    		std::cout << "  [" << j << "] "
              	<< signalParameters[channel][j].getParameterName()
              	<< "\n";
		}

		std::cout << "Creating " << channel << "DSCB signal functions...\n";
		FitFunction::FunctionType dscb = FitFunction::FunctionType::DoubleSidedCrystalBall;
		FitFunction dscbFunction = FitFunction::createFunctionOfType(dscb, channel, "", 0, 2000, channel);
		std::cout << channel << " DSCB parameters: \n";
		for (int i = 0; i < 7; i++)
		{
			double parameterValue = signalParameters[channel][dscbParameterIndices[i]].getFunction()->Eval(mass);
			std::cout << "TEST STATEMENT" << "\n";
			dscbFunction.getFunction()->SetParameter(i, parameterValue);
			std::cout << dscbFunction.getFunction()->GetParameter(i) << "\n"; 
		}
		dscbFunction.getFunction()->Draw("L");
		
	}

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