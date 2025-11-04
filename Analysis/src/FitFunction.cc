#include "CMSAnalysis/Analysis/interface/FitFunction.hh"
#include "TF1.h"
#include <TMath.h>
#include <boost/algorithm/string/split.hpp>
#include <iostream>
#include <sstream>
#include <string>
#include <utility>
#include <vector>

double FitFunction::powerLaw(double *x, double *par)
{
    return par[0] * pow(x[0] - par[1], par[2]);
}

double FitFunction::DSCB(double *x, double *par)
{
    const double alpha_l = par[0];
    const double alpha_h = par[1];
    const double n_l = par[2];
    const double n_h = par[3];
    const double mean = par[4];
    const double sigma = par[5];
    const double N = par[6];
    const float t = (x[0] - mean) / sigma;
    double result;
    const double fact1TLessMinosAlphaL = alpha_l / n_l;
    const double fact2TLessMinosAlphaL = (n_l / alpha_l) - alpha_l - t;
    const double fact1THihgerAlphaH = alpha_h / n_h;
    const double fact2THigherAlphaH = (n_h / alpha_h) - alpha_h + t;

    double root2 = std::pow(2, 0.5);
    if (-alpha_l <= t && alpha_h >= t)
    {
        result = exp(-0.5 * t * t);
    }
    else if (t < -alpha_l)
    {
        result = exp(-0.5 * alpha_l * alpha_l) * pow(fact1TLessMinosAlphaL * fact2TLessMinosAlphaL, -n_l);
    }
    else // if (t > alpha_h)
    {
        result = exp(-0.5 * alpha_h * alpha_h) * pow(fact1THihgerAlphaH * fact2THigherAlphaH, -n_h);
    }

    const double lowTailNorm = (n_l / std::abs(alpha_l)) * 1 / (n_l - 1) * std::exp(-0.5 * alpha_l * alpha_l);
    const double highTailNorm = (n_h / std::abs(alpha_h)) * 1 / (n_h - 1) * std::exp(-0.5 * alpha_h * alpha_h);
    const double gaussianNormA = erf(std::abs(alpha_l / root2)) + erf(std::abs(alpha_h / root2));
    const double gaussianNormB = std::pow(M_PI / 2, 0.5) * gaussianNormA;
    const double functionNormalization = std::pow(sigma * (gaussianNormB + lowTailNorm + highTailNorm), -1);

    // globalCounter++;
    // if(globalCounter%1000 == 0){
    // //std::cout<<"Global norm: " << globalNorm << "\n";
    // }
    return N * functionNormalization * result;
}

double FitFunction::doubleGaussian(double *x, double *par)
{
    return par[0] * TMath::Gaus(x[0], par[1], par[2]) + par[3] * TMath::Gaus(x[0], par[4], par[5]);
}

FitFunction::FitFunction(const TF1 &func, FunctionType funcType, std::string channelName)
    : function(func), channelName(std::move(channelName)), functionType(funcType)
{
}

TF1 *FitFunction::getFunction()
{
    return &function;
}

void FitFunction::setFunction(const TF1 &func, FunctionType funcType)
{
    function = func;
    functionType = funcType;
}

FitFunction::FunctionType FitFunction::getFunctionType()
{
    return functionType;
}

std::string FitFunction::getName()
{
    return function.GetName();
}

double FitFunction::getMin()
{
    double min;
    double max;
    function.GetRange(min, max);
    return min;
}
double FitFunction::getMax()
{
    double min;
    double max;
    function.GetRange(min, max);
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

FitFunction FitFunction::createFunctionOfType(FunctionType functionType, const std::string &name,
                                              const std::string &expFormula, double min, double max,
                                              std::string channelName)
{
    TF1 func;
    // std::cout << "ExpressionFormula enum: " << FunctionType::EXPRESSION_FORMULA << '\n';
    // std::cout << "Function Type: " << functionType << '\n';
    // std::cout << "Got to create function\n";
    switch (functionType)
    {
    case FunctionType::ExpressionFormula:
        func = TF1(name.data(), expFormula.data(), min, max, TF1::EAddToList::kNo);
        break;
    case FunctionType::DoubleSidedCrystalBall:
        func = TF1(name.data(), DSCB, min, max, 7, 1, TF1::EAddToList::kNo);
        func.SetParNames("#alpha_{low}", "#alpha_{high}", "n_{low}", "n_{high}", "#mu", "#sigma", "norm");
        break;
    case FunctionType::PowerLaw:
        func = TF1(name.data(), powerLaw, min, max, 3, 1, TF1::EAddToList::kNo);
        break;
    case FunctionType::DoubleGaussian:
        func = TF1(name.data(), doubleGaussian, min, max, 6, 1, TF1::EAddToList::kNo);
        func.SetParNames("mul_{1}", "#mu_{1}", "#sigma_{1}", "mul_{2}", "#mu_{2}", "#sigma_{2}");
        break;
    default:
        throw std::invalid_argument("Not a valid FunctionType enum value");
    };

    return FitFunction(func, functionType, std::move(channelName));
}

// std::ostream& operator<<(std::ostream& stream, FitFunction& function)
// {
// 	TF1* func = function.getFunction();
// 	std::vector<std::string> channel_parameter = FitFunction::split(func->GetName(), '/');

// 	stream << channel_parameter[1] << "		";

// 	for (int i = 0; i < func->GetNpar(); ++i)
// 	{
// 		stream << func->GetParameter(i) << "		";
// 	}

// 	for (int i = 0; i < func->GetNpar(); ++i)
// 	{
// 		stream << func->GetParError(i) << "		";
// 		// stream << 1 << ' ';
// 	}

// 	stream << '\n';
// 	// std::cout << "Got parameters\n";
// 	return stream;
// }

std::string FitFunction::getChannelName()
{
    return channelName;
}

std::string FitFunction::getParameterName()
{
    std::vector<std::string> channel_parameters = split(getName(), '/');
    return channel_parameters[1];
}

// Helper function for splitting strings
std::vector<std::string> FitFunction::split(const std::string &str, char delimiter)
{
    std::vector<std::string> tokens;
    std::stringstream ss(str);
    std::string token;

    while (std::getline(ss, token, delimiter))
    {
        tokens.push_back(token);
    }

    return tokens;
}

// OLD CODE
std::ostream &operator<<(std::ostream &stream, FitFunction &function)
{
    TF1 *func = function.getFunction();
    // std::cout << "Got functions\n";
    stream << "Name: " << func->GetName() << '\n';
    stream << "FunctionTypeEnum: " << static_cast<int>(function.getFunctionType()) << '\n';
    stream << "ExpressionFormula: ";
    if (function.getFunctionType() == FitFunction::FunctionType::ExpressionFormula)
    {
        stream << func->GetExpFormula() << '\n';
    }
    else
    {
        stream << "None\n";
    }

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

    stream << "ParaNames: ";
    for (int i = 0; i < func->GetNpar(); ++i)
    {
        stream << func->GetParName(i) << ' ';
        // stream << 1 << ' ';
    }

    stream << '\n' << "Parameters: ";
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

// std::istream& operator>>(std::istream& stream, FitFunction& func)
// {
// 	std::string ignore;
// 	std::string name;
// 	FitFunction::FunctionType funcType;
// 	std::string expFormula;
// 	double min, max;
// 	int params;
// 	int tempFuncType;

// 	stream >> ignore >> name;
// 	stream >> ignore >> tempFuncType;
// 	std::cout << "TempFuncType = " << tempFuncType << '\n';
// 	funcType = (FitFunction::FunctionType) tempFuncType;
// 	stream >> ignore >> expFormula;

// 	stream >> ignore >> min >> max;
// 	stream >> ignore >> params;

// 	std::vector<std::string> paramNames(params);
// 	std::vector<double> paramValues(params);
// 	std::vector<double> paramErrors(params);

// 	stream >> ignore;
// 	for (int i = 0; i < params; ++i)
// 	{
// 		stream >> paramNames[i];
// 	}

// 	stream >> ignore;
// 	for (int i = 0; i < params; ++i)
// 	{
// 		stream >> paramValues[i];
// 	}

// 	stream >> ignore;
// 	for (int i = 0; i < params; ++i)
// 	{
// 		stream >> paramErrors[i];
// 	}

// 	FitFunction function = FitFunction::createFunctionOfType(funcType, name, expFormula, min, max);

// 	for (int i = 0; i < params; ++i)
// 	{
// 		function.getFunction()->SetParName(i, paramNames[i].c_str());
// 		function.getFunction()->SetParameter(i, paramValues[i]);
// 		function.getFunction()->SetParError(i, paramErrors[i]);
// 	}
// 	func = function;

// 	// std::cout << "Set function\n";

// 	return stream;
// }

std::istream &operator>>(std::istream &stream, FitFunction &func)
{
    std::string line;
    std::string name;
    FitFunction::FunctionType funcType = FitFunction::FunctionType(0);
    std::string expFormula;
    double min = 0.0, max = 0.0;
    int params = 0;
    int tempFuncType = 0;

    // --- Helper lambda to trim spaces ---
    auto trim = [](std::string &s) {
        s.erase(0, s.find_first_not_of(" \t"));
        s.erase(s.find_last_not_of(" \t") + 1);
    };

    // --- Helper to safely get next non-empty line ---
    auto getLine = [&](std::istream &in, std::string &out) -> bool {
        while (std::getline(in, out))
        {
            trim(out);
            if (!out.empty())
            {
                return true;
            }
        }
        return false;
    };

    // --- Read "Name:" line ---
    if (!getLine(stream, line))
    {
        return stream;
    }
    if (line.starts_with("Name:"))
    {
        name = line.substr(5);
    }
    trim(name);
    std::cout << "Reading function: " << name << '\n';

    // --- Read "FunctionTypeEnum:" line ---
    if (!getLine(stream, line))
    {
        return stream;
    }
    if (line.starts_with("FunctionTypeEnum:"))
    {
        tempFuncType = std::stoi(line.substr(17));
    }
    funcType = static_cast<FitFunction::FunctionType>(tempFuncType);

    // --- Read "ExpressionFormula:" line ---
    if (!getLine(stream, line))
    {
        return stream;
    }
    if (line.starts_with("ExpressionFormula:"))
    {
        expFormula = line.substr(18);
    }
    trim(expFormula);

    // --- Read "Range:" line ---
    if (!getLine(stream, line))
    {
        return stream;
    }
    if (line.starts_with("Range:"))
    {
        std::istringstream rangeStream(line.substr(6));
        rangeStream >> min >> max;
    }

    // --- Read "NumOfParameters:" line ---
    if (!getLine(stream, line))
    {
        return stream;
    }
    if (line.starts_with("NumOfParameters:"))
    {
        params = std::stoi(line.substr(16));
    }
    if (params <= 0 || params > 1000)
    {
        std::cerr << "Error: invalid NumOfParameters = " << params << " for " << name << '\n';
        return stream;
    }

    // --- Prepare containers ---
    std::vector<std::string> paramNames(params);
    std::vector<double> paramValues(params);
    std::vector<double> paramErrors(params);

    // --- Read "ParaNames:" line ---
    if (!getLine(stream, line))
    {
        return stream;
    }
    if (line.starts_with("ParaNames:"))
    {
        std::istringstream ss(line.substr(10));
        for (int i = 0; i < params && ss; ++i)
        {
            ss >> paramNames[i];
        }
    }

    // --- Read "Parameters:" line ---
    if (!getLine(stream, line))
    {
        return stream;
    }
    if (line.starts_with("Parameters:"))
    {
        std::istringstream ss(line.substr(11));
        for (int i = 0; i < params && ss; ++i)
        {
            ss >> paramValues[i];
        }
    }

    // --- Read "ParamErrors:" line ---
    if (!getLine(stream, line))
    {
        return stream;
    }
    if (line.starts_with("ParamErrors:"))
    {
        std::istringstream ss(line.substr(12));
        for (int i = 0; i < params && ss; ++i)
        {
            ss >> paramErrors[i];
        }
    }

    // --- Create FitFunction object ---
    FitFunction function = FitFunction::createFunctionOfType(funcType, name, expFormula, min, max, "");

    // --- Set parameters ---
    for (int i = 0; i < params; ++i)
    {
        function.getFunction()->SetParName(i, paramNames[i].c_str());
        function.getFunction()->SetParameter(i, paramValues[i]);
        function.getFunction()->SetParError(i, paramErrors[i]);
    }

    func = function;
    std::cout << "Successfully read: " << name << " (" << params << " parameters)\n\n";

    return stream;
}
