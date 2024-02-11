#ifndef FIT_FUNCTION_HH
#define FIT_FUNCTION_HH

#include "TF1.h"
#include <iostream>

class FitFunction 
{
public:
	enum FunctionType 
	{
		EXPRESSION_FORMULA,
		DOUBLE_SIDED_CRYSTAL_BALL,
		POWER_LAW
	};

	// static const std::vector<std::string> functionList;
	static TF1* createFunctionOfType(FunctionType functionType, const std::string& name, const std::string& expFormula, double min, double max);

	static double powerLaw(double *x, double *par);
	static double DSCB(double* x, double* par);

	// static std::vector<FitFunction> loadFunctions(const std::string& fileName);
	// static void saveFunctions(std::vector<FitFunction>& functions, const std::string& fileName);

	// static std::string getFormulaName(const std::string& name);

	FitFunction();
	FitFunction(TF1* func, FunctionType funcType);
	TF1* getFunction();
	void setFunction(TF1* function, FunctionType funcType);
	FunctionType getFunctionType();
	void setFunctionType(FunctionType funcType);
	std::string getName();
	double getMin();
	double getMax();
private:
	TF1* function;
	FunctionType functionType;
};

std::ostream& operator<<(std::ostream& stream, FitFunction& function);
// std::ostream& operator<<(std::ostream& stream, TF1* function);
std::istream& operator>>(std::istream& stream, FitFunction& function);

#endif