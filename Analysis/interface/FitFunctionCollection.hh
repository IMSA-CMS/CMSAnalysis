#ifndef FIT_FUNCTION_COLLECTION_HH
#define FIT_FUNCTION_COLLECTION_HH

#include <unordered_map>
#include "CMSAnalysis/Analysis/interface/FitFunction.hh"

class FitFunctionCollection
{
public:
	static FitFunctionCollection loadFunctions(const std::string& fileName);

	std::unordered_map<std::string, FitFunction> functions;

	FitFunctionCollection();
	FitFunctionCollection(std::vector<FitFunction>& functions);
	FitFunctionCollection(size_t size);

	FitFunctionCollection parameterizeFunctions(FitFunction::FunctionType funcType);
	void insert(const std::string& key, FitFunction& function);
	void insert(FitFunction& function);
	size_t size() const;
	FitFunction& get(const std::string& index);
	void saveFunctions(const std::string& fileName, bool append = false);

	FitFunction& operator[](const std::string& index);
	// FitFunction& operator[](int index);
	bool checkFunctionsSimilar();
};

#endif