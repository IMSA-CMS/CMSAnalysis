// #ifndef FIT_FUNCTION_HH
// #define FIT_FUNCTION_HH

// #include "TF1.h"
// #include <iostream>

// class FitFunction 
// {
// public:
// 	enum FunctionType 
// 	{
// 		EXPRESSION_FORMULA,
// 		DOUBLE_SIDED_CRYSTAL_BALL,
// 		POWER_LAW,
//         DOUBLE_GAUSSIAN,
// 	};

// 	// static const std::vector<std::string> functionList;
// 	static FitFunction createFunctionOfType(FunctionType functionType, const std::string& name, const std::string& expFormula, double min, double max);

// 	static double powerLaw(double *x, double *par);
// 	static double DSCB(double* x, double* par);
// 	static double doubleGaussian(double* x, double* par);

// 	// static std::vector<FitFunction> loadFunctions(const std::string& fileName);
// 	// static void saveFunctions(std::vector<FitFunction>& functions, const std::string& fileName);

// 	// static std::string getFormulaName(const std::string& name);

// 	FitFunction();
// 	FitFunction(TF1* func, FunctionType funcType);
// 	TF1* getFunction() const;
// 	void setFunction(TF1* function, FunctionType funcType);
// 	FunctionType getFunctionType();
// 	void setFunctionType(FunctionType funcType);
// 	std::string getName();
// 	double getMin();
// 	double getMax();
// 	static std::vector<std::string> split(const std::string& str, char delimiter);
// 	std::string getChannelName();
// 	std::string getParameterName();
// private:
// 	TF1* function;
// 	FunctionType functionType;
// 	//add a map (like hashmap in java, but C++) of TF1s (storage part) --> name of systematic function that goes with it
// 	//command thats an "add systematic" command
// 	//we dont want them to put in their own TF1 functions. Let users input parameter values. User doesn't have direct access to functions
// 	//all clones of originals
// 	//creates systematics, allows user to input parameters. 
// };

// std::ostream& operator<<(std::ostream& stream, FitFunction& function);
// std::istream& operator>>(std::istream& stream, FitFunction& function);

// #endif

#ifndef FIT_FUNCTION_HH
#define FIT_FUNCTION_HH

#include "TF1.h"
#include <iostream>
#include <map>
#include <string>
#include <vector>

class FitFunction
{
  public:
    enum class FunctionType
    {
        ExpressionFormula,
        DoubleSidedCrystalBall,
        PowerLaw,
        DoubleGaussian,
        GausLogPowerNorm,
    };

    // static const std::vector<std::string> functionList;
    static FitFunction createFunctionOfType(FunctionType functionType, const std::string &name,
                                            const std::string &expFormula, double min, double max,
                                            std::string channelName);

    // static std::vector<FitFunction> loadFunctions(const std::string& fileName);
    // static void saveFunctions(std::vector<FitFunction>& functions, const std::string& fileName);

	FitFunction(const TF1& func, FunctionType funcType, std::string channelName);
	TF1* getFunction();
	void setFunction(const TF1& function, FunctionType funcType);
	FunctionType getFunctionType();
	std::string getName();
	double getMin();
	double getMax();
	std::string getChannelName();
	std::string getParameterName();

	void addSystematic(const std::string& sysName, const TF1& upFunction, const TF1& downFunction);
	void addSystematic(const std::string& sysName, const std::vector<double>& upParams, const std::vector<double>& downParams);
	const TF1* getSystematic(const std::string& sysName, bool up) const;
	std::vector<std::string> listSystematics() const;
	// implement functions into source code
	// modify input output stuff (start with implements)

private:
	TF1 function;
	std::string channelName;
	FunctionType functionType;
	std::map<std::string, std::pair<TF1, TF1>> systematics; 

    static double powerLaw(double *x, double *par);
    static double DSCB(double *x, double *par);
    static double doubleGaussian(double *x, double *par);
    static double gausLogPowerNorm(double *x, double *par);
    static std::vector<std::string> split(const std::string &str, char delimiter);
};

std::ostream &operator<<(std::ostream &stream, FitFunction &function);
std::istream &operator>>(std::istream &stream, FitFunction &function);

#endif
