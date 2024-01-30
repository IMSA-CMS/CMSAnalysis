
#include "CMSAnalysis/Analysis/interface/FitFunction.hh"

void temp()
{
    
	std::vector<FitFunction> functions;
	TF1 funcOne("testOne", "x*[Parameter]", 0, 10);
	funcOne.SetParameter(0, 1);
	funcOne.SetParError(0, 1);

	TF1 funcTwo("testTwo", "[ParameterTwo]*sin(x)+[ParameterThree]", 0, 5);
	funcOne.SetParameter(0, 2);
	funcOne.SetParError(0, 2);
	funcOne.SetParameter(1, 3);
	funcOne.SetParError(1, 3);

	functions.push_back(FitFunction(funcOne));
	functions.push_back(FitFunction(funcTwo));
    std::cout << "Got to end of functions\n";
	FitFunction::saveFunctions(functions, "testFunctions.txt");
	auto functionsTwo = FitFunction::loadFunctions("testFunctions.txt");

	std::cout << functionsTwo[0] << functionsTwo[1];
}