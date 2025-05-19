#include <string>
#include <unordered_map>
#include <stdexcept>

class FilePathMapper 
{
public:
    // Add or update a mapping
    void addMapping(const std::string& variableName, const std::string& filePath) 
    {
        mappings[variableName] = filePath;
    }

    // Get the file path for a variable
    std::string getFilePath(const std::string& variableName) const 
    {
        auto it = mappings.find(variableName);
        if (it == mappings.end()) 
        {
            throw std::runtime_error("File path for variable '" + variableName + "' not found in mappings.");
        }
        return it->second;
    }

private:
    std::unordered_map<std::string, std::string> mappings;
};