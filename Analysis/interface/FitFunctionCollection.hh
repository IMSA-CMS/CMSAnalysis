#ifndef FIT_FUNCTION_COLLECTION_HH
#define FIT_FUNCTION_COLLECTION_HH

#include "CMSAnalysis/Analysis/interface/FitFunction.hh"
#include <unordered_map>

class FitFunctionCollection
{
  public:
    static FitFunctionCollection loadFunctions(const std::string &fileName);

    FitFunctionCollection();
    FitFunctionCollection(std::vector<FitFunction> &functions);
    // FitFunctionCollection(size_t size);

    FitFunctionCollection parameterizeFunctions(FitFunction::FunctionType funcType);
    void insert(const std::string &key, FitFunction &function);
    void insert(FitFunction &function);
    size_t size() const;
    FitFunction &get(const std::string &key);
    void saveFunctions(const std::string &fileName, bool append = false);

    FitFunction &operator[](const std::string &key);
    // FitFunction& operator[](int index);
    bool checkFunctionsSimilar();
    std::unordered_map<std::string, FitFunction> &getFunctions();

  private:
    std::unordered_map<std::string, FitFunction> functions;
};

#endif