#include "CMSAnalysis/Analysis/interface/FitFunction.hh"
#include <fstream>

double FitFunction::powerLaw(double *x, double *par)
{
	return par[0] * pow(x[0] - par[1], par[2]);
}

double FitFunction::DSCB(double *x, double *par)
{
	return par[0] * pow(x[0] - par[1], par[2]);
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

TF1* FitFunction::createFunctionOfType(FunctionType functionType, const std::string& name, const std::string& expFormula, double min, double max)
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
			func = new TF1(name.data(), DSCB, min, max, 3);
			break;
		case FunctionType::POWER_LAW:
			func = new TF1(name.data(), powerLaw, min, max, 3);
			break;
		default:
			throw std::invalid_argument("Not a valid FunctionType enum value");
	};

	return func;
}

std::ostream& operator<<(std::ostream& stream, FitFunction& function)
{
	TF1* func = function.getFunction();
	// std::cout << "Got functions\n";
	stream << "Name: " << func->GetName() << '\n';
	stream << "FunctionTypeEnum: " << function.getFunctionType() << '\n';
	stream << "ExpressionFormula: ";
	if (function.getFunctionType() == FitFunction::FunctionType::EXPRESSION_FORMULA)
		stream << func->GetExpFormula() << '\n';
	else
		stream << "None\n";

	// std::cout << "Got expFormula\n";
	// auto formulaName = FitFunction::getFormulaName(func->GetName());
	// auto it = std::find(FitFunction::functionList.begin(), FitFunction::functionList.end(), formulaName);
	// if (it != FitFunction::functionList.end())
	// {
	// 	stream << "Name: " << func->GetName() << '\n';
	// 	stream << "Function: " << formulaName << '\n';
	// }
	// else
	// {
	// 	stream << "Name: " << func->GetName() << '\n';
	// 	stream << "Function: " << func->GetExpFormula() << '\n';
	// }
	double min = 0;
	double max = 0;
	func->GetRange(min, max);
	stream << "Range: " << min << ' ' << max << '\n';
	stream << "NumOfParameters: " << func->GetNpar() << '\n';

	stream << "Parameters: ";
	for (int i = 0; i < func->GetNpar(); ++i)
	{
		stream << func->GetParameter(i) << ' ';
		// stream << 1 << ' ';
	}

	stream << '\n' << "ParamErrors: ";

	for (int i = 0; i < func->GetNpar(); ++i)
	{
		stream << func->GetParError(i) << ' ';
		// stream << 1 << ' ';
	}

	stream << '\n';
	// std::cout << "Got parameters\n";
	return stream;
}

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

	// if (funcType == FitFunction::FunctionType::EXPRESSION_FORMULA)
	// 	stream >> ignore >> expFormula;
	// else
	// 	stream >> ignore >> ignore;


	stream >> ignore >> min >> max;
	stream >> ignore >> params;

	// std::cout << "Params: " << params << '\n';

	// std::cout << "Read all values\n";

	std::vector<double> paramValues(params);
	std::vector<double> paramErrors(params);

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
	// std::cout << "Read all parameters\n";

	TF1* function = FitFunction::createFunctionOfType(funcType, name, expFormula, min, max);

	// std::cout << "Created function\n";
	// auto iterator = std::find(FitFunction::functionList.begin(), FitFunction::functionList.end(), formula);
	// if (iterator != FitFunction::functionList.end())
	// {
	// 	auto& funcName = *iterator;
	// 	if (funcName == "pearson")
	// 	{
	// 		function = new TF1(name.data(), FitFunction::pearson, min, max, 4);
	// 		function->SetParNames("a", "m", "mass", "n");
	// 	}
	// }	
	// else
	// {
	// 	function = new TF1(name.data(), formula.data(), min, max);
	// }

	for (int i = 0; i < params; ++i)
	{
		function->SetParameter(i, paramValues[i]);
		function->SetParError(i, paramErrors[i]);
	}

	func.setFunction(function, funcType);

	// std::cout << "Set function\n";

	return stream;
}

