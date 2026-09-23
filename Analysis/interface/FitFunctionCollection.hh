#ifndef FIT_FUNCTION_COLLECTION_HH
#define FIT_FUNCTION_COLLECTION_HH

#include "CMSAnalysis/Analysis/interface/SimpleFitFunction.hh"
#include <memory>
#include <unordered_map>
#include <set>

class FitFunctionCollection
{
  public:
    static FitFunctionCollection loadFunctions(const std::string &fileName);

    FitFunctionCollection();
    FitFunctionCollection(std::vector<SimpleFitFunction> &functions);
    // FitFunctionCollection(size_t size);

    FitFunctionCollection parameterizeFunctions(FitFunction::FunctionType funcType);
    void insert(const std::string& key, SimpleFitFunction function);
    void insert(SimpleFitFunction function);
    size_t size() const;
    SimpleFitFunction &get(const std::string &key);
    void saveFunctions(const std::string &fileName, bool append = false);

    std::set<std::string> findUniqueNames(std::string parameter) const;
    FitFunctionCollection getFunctions(std::string name) const;
    FitFunctionCollection getFunctions(std::string parameter, std::string name) const;
    std::shared_ptr<FitFunction> getModel(const std::string &channel, double min, double max) const;

    SimpleFitFunction& operator[](const std::string& key);
    FitFunctionCollection& operator+=(const FitFunctionCollection& other);

    bool checkFunctionsSimilar();
    const std::unordered_map<std::string, std::shared_ptr<SimpleFitFunction>>& getFunctionsMap() const;
    
  private:
    
    // as selections share fitted rows if u mutate a row it affects every selection of it
    std::unordered_map<std::string, std::shared_ptr<SimpleFitFunction>> functions;
};

#endif