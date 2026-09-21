#ifndef FIT_FUNCTION_COLLECTION_HH
#define FIT_FUNCTION_COLLECTION_HH

#include "SimpleFitFunction.hh"
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

    std::set<std::string> findUniqueNames(std::string parameter); //
    FitFunctionCollection getFunctions(std::string name); //
    FitFunctionCollection getFunctions(std::string parameter, std::string name); //

    SimpleFitFunction& operator[](const std::string& key);
    FitFunctionCollection& operator+=(const FitFunctionCollection& other);

    bool checkFunctionsSimilar();
    std::unordered_map<std::string, SimpleFitFunction>& getFunctionsMap();
    
  private:
    std::unordered_map<std::string, SimpleFitFunction> functions;
};

#endif