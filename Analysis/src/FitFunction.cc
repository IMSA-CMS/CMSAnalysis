#include "CMSAnalysis/Analysis/interface/FitFunction.hh"
#include "TF1.h"
#include <fstream>
#include <sstream>

double FitFunction::powerLaw(double *x, double *par)
{
	return par[0] * pow(x[0] - par[1], par[2]);
}

double FitFunction::DSCB(double *x, double *par)
{
	double alpha_l = par[0]; 
	double alpha_h = par[1]; 
	double n_l     = par[2]; 
	double n_h     = par[3]; 
	double mean	= par[4]; 
	double sigma	= par[5];
	double N	= par[6];
	float t = (x[0]-mean)/sigma;
	double result;
	double fact1TLessMinosAlphaL = alpha_l/n_l;
	double fact2TLessMinosAlphaL = (n_l/alpha_l) - alpha_l -t;
	double fact1THihgerAlphaH = alpha_h/n_h;
	double fact2THigherAlphaH = (n_h/alpha_h) - alpha_h +t;

	double root2 = std::pow(2,0.5);
	if (-alpha_l <= t && alpha_h >= t)
	{
		result = exp(-0.5*t*t);
	}
	else if (t < -alpha_l)
	{
		result = exp(-0.5*alpha_l*alpha_l)*pow(fact1TLessMinosAlphaL*fact2TLessMinosAlphaL, -n_l);
	}
	else //if (t > alpha_h)
	{
		result = exp(-0.5*alpha_h*alpha_h)*pow(fact1THihgerAlphaH*fact2THigherAlphaH, -n_h);
	}
	
	double lowTailNorm = (n_l/std::abs(alpha_l)) * 1/(n_l - 1) * std::exp(-0.5 * alpha_l * alpha_l);
	double highTailNorm = (n_h/std::abs(alpha_h)) * 1/(n_h - 1) * std::exp(-0.5 * alpha_h * alpha_h);
	double gaussianNormA = erf(std::abs(alpha_l/root2)) + erf(std::abs(alpha_h/root2));  
	double gaussianNormB = std::pow(M_PI/2, 0.5) * gaussianNormA;
	double functionNormalization = std::pow(sigma * (gaussianNormB + lowTailNorm + highTailNorm ), -1);


	// globalCounter++;
	// if(globalCounter%1000 == 0){
	// //std::cout<<"Global norm: " << globalNorm << "\n";
	// }
	return N * functionNormalization * result;	
}

FitFunction::FitFunction() {}

FitFunction::FitFunction(TF1* func, FunctionType funcType)
	: function(func), functionType(funcType) {}

TF1* FitFunction::getFunction()
{
	return function;
}

void FitFunction::setFunction(TF1* func, FunctionType funcType)
{
	function = func;
	functionType = funcType;
}

void FitFunction::setFunctionType(FunctionType funcType)
{
	functionType = funcType;
}

FitFunction::FunctionType FitFunction::getFunctionType()
{
	return functionType;
}

std::string FitFunction::getName()
{
	return function->GetName();
}

double FitFunction::getMin()
{
	double min, max;
	function->GetRange(min, max);
	return min;
}
double FitFunction::getMax()
{
	double min, max;
	function->GetRange(min, max);
	return max;
}


// std::string FitFunction::getFormulaName(const std::string& name)
// {
// 	auto index = name.find_first_of('_');
// 	if (index == std::string::npos)
// 		return name;
// 	else
// 	{
// 		return name.substr(0, index);
// 	}
// }

// std::vector<FitFunction> FitFunction::loadFunctions(const std::string& fileName)
// {
// 	std::fstream file(fileName, std::ios_base::in);
// 	if (file.is_open())
// 	{
// 		int size;
// 		file >> size;
// 		std::vector<FitFunction> functions(size);
// 		for (int i = 0; i < size; ++i)
// 		{
// 			file >> functions[i];
// 		}
// 		file.close();
// 		return functions;
// 	}
// 	else
// 	{
// 		throw std::runtime_error("File not loaded successfully");
// 	}
// }
// void FitFunction::saveFunctions(std::vector<FitFunction>& functions, const std::string& fileName)
// {
// 	std::fstream file(fileName, std::ios_base::out);
// 	if (file.is_open())
// 	{
// 		file << functions.size() << '\n';
// 		for (size_t i = 0; i < functions.size(); ++i)
// 		{
// 			file << functions[i];
// 		}
// 		file.close();
// 	}
// 	else
// 	{
// 		throw std::runtime_error("File not saved successfully");
// 	}
// }

FitFunction FitFunction::createFunctionOfType(FunctionType functionType, const std::string& name, const std::string& expFormula, double min, double max)
{
	TF1* func;
	// std::cout << "ExpressionFormula enum: " << FunctionType::EXPRESSION_FORMULA << '\n';
	// std::cout << "Function Type: " << functionType << '\n';
	// std::cout << "Got to create function\n";
	switch (functionType)
	{	
		case FunctionType::EXPRESSION_FORMULA:
			func = new TF1(name.data(), expFormula.data(), min, max);
			break;
		case FunctionType::DOUBLE_SIDED_CRYSTAL_BALL:
			// std::cout << "Crystal ball\n";
			func = new TF1(name.data(), DSCB, min, max, 7);
			break;
		case FunctionType::POWER_LAW:
			func = new TF1(name.data(), powerLaw, min, max, 3);
			break;
		default:
			throw std::invalid_argument("Not a valid FunctionType enum value");
	};

	return FitFunction(func, functionType);
}

std::ostream& operator<<(std::ostream& stream, FitFunction& function)
{
	TF1* func = function.getFunction();
	std::vector<std::string> channel_parameter = FitFunction::split(func->GetName(), '/');

	stream << channel_parameter[1] << "		";

	for (int i = 0; i < func->GetNpar(); ++i)
	{
		stream << func->GetParameter(i) << "		";
	}

	for (int i = 0; i < func->GetNpar(); ++i)
	{
		stream << func->GetParError(i) << "		";
		// stream << 1 << ' ';
	}

	stream << '\n';
	// std::cout << "Got parameters\n";
	return stream;
}


std::string FitFunction::getChannelName()
{
	std::vector<std::string> channel_parameters = split(getName(), '/');
	return channel_parameters[0];
}

std::string FitFunction::getParameterName()
{
	std::vector<std::string> channel_parameters = split(getName(), '/');
	return channel_parameters[1];
}


// Helper function for splitting strings
std::vector<std::string> FitFunction::split(const std::string& str, char delimiter) {
    std::vector<std::string> tokens;
    std::stringstream ss(str);
    std::string token;
    
    while (std::getline(ss, token, delimiter)) {
        tokens.push_back(token);
    }
    
    return tokens;
}








// OLD CODE
// std::ostream& operator<<(std::ostream& stream, FitFunction& function)
// {
// 	TF1* func = function.getFunction();
// 	// std::cout << "Got functions\n";
// 	stream << "Name: " << func->GetName() << '\n';
// 	stream << "FunctionTypeEnum: " << function.getFunctionType() << '\n';
// 	stream << "ExpressionFormula: ";
// 	if (function.getFunctionType() == FitFunction::FunctionType::EXPRESSION_FORMULA)
// 		stream << func->GetExpFormula() << '\n';
// 	else
// 		stream << "None\n";

// 	// std::cout << "Got expFormula\n";
// 	// auto formulaName = FitFunction::getFormulaName(func->GetName());
// 	// auto it = std::find(FitFunction::functionList.begin(), FitFunction::functionList.end(), formulaName);
// 	// if (it != FitFunction::functionList.end())
// 	// {
// 	// 	stream << "Name: " << func->GetName() << '\n';
// 	// 	stream << "Function: " << formulaName << '\n';
// 	// }
// 	// else
// 	// {
// 	// 	stream << "Name: " << func->GetName() << '\n';
// 	// 	stream << "Function: " << func->GetExpFormula() << '\n';
// 	// }
// 	double min = 0;
// 	double max = 0;
// 	func->GetRange(min, max);
// 	stream << "Range: " << min << ' ' << max << '\n';
// 	stream << "NumOfParameters: " << func->GetNpar() << '\n';

// 	stream << "ParaNames: ";
// 	for (int i = 0; i < func->GetNpar(); ++i)
// 	{
// 		stream << func->GetParName(i) << ' ';
// 		// stream << 1 << ' ';
// 	}

// 	stream << '\n' << "Parameters: ";
// 	for (int i = 0; i < func->GetNpar(); ++i)
// 	{
// 		stream << func->GetParameter(i) << ' ';
// 		// stream << 1 << ' ';
// 	}

// 	stream << '\n' << "ParamErrors: ";

// 	for (int i = 0; i < func->GetNpar(); ++i)
// 	{
// 		stream << func->GetParError(i) << ' ';
// 		// stream << 1 << ' ';
// 	}

// 	stream << '\n';
// 	// std::cout << "Got parameters\n";
// 	return stream;
// }

// // Helper function for splitting strings
// std::vector<std::string> split(const std::string& str, char delimiter) {
//     std::vector<std::string> tokens;
//     std::stringstream ss(str);
//     std::string token;
    
//     while (std::getline(ss, token, delimiter)) {
//         tokens.push_back(token);
//     }
    
//     return tokens;
// }

// std::ostream& operator<<(std::ostream& stream, TF1* func)
// {
// 	auto formulaName = FitFunction::getFormulaName(func->GetName());
// 	auto it = std::find(FitFunction::functionList.begin(), FitFunction::functionList.end(), formulaName);
// 	if (it != FitFunction::functionList.end())
// 	{
// 		stream << "Name: " << func->GetName() << '\n';
// 		stream << "Function: " << formulaName << '\n';
// 	}
// 	else
// 	{
// 		stream << "Name: " << func->GetName() << '\n';
// 		stream << "Function: " << func->GetExpFormula() << '\n';
// 	}
// 	double min = 0;
// 	double max = 0;
// 	func->GetRange(min, max);
// 	stream << "Range: " << min << ' ' << max << '\n';
// 	stream << "NumOfParameters: " << func->GetNpar() << '\n';

// 	stream << "Parameters: ";
// 	for (int i = 0; i < func->GetNpar(); ++i)
// 	{
// 		stream << func->GetParameter(i) << ' ';
// 	}

// 	stream << '\n' << "ParamErrors: ";

// 	for (int i = 0; i < func->GetNpar(); ++i)
// 	{
// 		stream << func->GetParError(i) << ' ';
// 	}

// 	stream << '\n';
// 	return stream;
// }

std::istream& operator>>(std::istream& stream, FitFunction& func)
{
	std::string ignore;
	std::string name;
	FitFunction::FunctionType funcType;
	std::string expFormula;
	double min, max;
	int params;
	int tempFuncType;

	stream >> ignore >> name;
	stream >> ignore >> tempFuncType;
	// std::cout << "TempFuncType = " << tempFuncType << '\n';
	funcType = (FitFunction::FunctionType) tempFuncType;
	stream >> ignore >> expFormula;


	stream >> ignore >> min >> max;
	stream >> ignore >> params;

	std::vector<std::string> paramNames(params);
	std::vector<double> paramValues(params);
	std::vector<double> paramErrors(params);

	stream >> ignore;
	for (int i = 0; i < params; ++i)
	{
		stream >> paramNames[i];
	}


	stream >> ignore;
	for (int i = 0; i < params; ++i)
	{
		stream >> paramValues[i];
	}

	stream >> ignore;
	for (int i = 0; i < params; ++i)
	{
		stream >> paramErrors[i];
	}

	FitFunction function = FitFunction::createFunctionOfType(funcType, name, expFormula, min, max);

	for (int i = 0; i < params; ++i)
	{
		function.getFunction()->SetParName(i, paramNames[i].c_str());
		function.getFunction()->SetParameter(i, paramValues[i]);
		function.getFunction()->SetParError(i, paramErrors[i]);
	}
	func = function;

	// std::cout << "Set function\n";

	return stream;
}

