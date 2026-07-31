#ifndef FIT_FUNCTION_COLLECTION_HH
#define FIT_FUNCTION_COLLECTION_HH

#include "FitFunction.hh"
#include <unordered_map>
#include <set>

class FitFunctionCollection
{
  public:
    static FitFunctionCollection loadFunctions(const std::string &fileName);

    FitFunctionCollection();
    FitFunctionCollection(std::vector<FitFunction> &functions);
    // FitFunctionCollection(size_t size);

    FitFunctionCollection parameterizeFunctions(FitFunction::FunctionType funcType);
    void insert(const std::string& key, FitFunction function);
    void insert(FitFunction function);
    size_t size() const;
    FitFunction &get(const std::string &key);
    void saveFunctions(const std::string &fileName, bool append = false);

    std::set<std::string> findUniqueNames(std::string parameter); //
    std::vector<FitFunction> getFunctions(std::string name); //
    std::vector<FitFunction> getFunctions(std::string parameter, std::string name); //

    FitFunction& operator[](const std::string& key);
    FitFunctionCollection& operator+=(const FitFunctionCollection& other);

    bool checkFunctionsSimilar();
    std::unordered_map<std::string, FitFunction>& getFunctions();
    
  private:
    std::unordered_map<std::string, FitFunction> functions;
};

#endif