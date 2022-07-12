#include "CMSAnalysis/DataCollection/interface/Process.hh"
#include "CMSAnalysis/DataCollection/interface/IDType.hh"

Process::Process(std::string iname, std::vector<IDType> itype): 
name(iname),
type(itype)
{

}

Process::Process(std::string iname, std::unordered_map<std::string, std::vector<std::string>> itype):
name(iname)
{
    // Converts the unordered_map named type to a vector of IDType
    
    for (auto entry : itype)
    {
        IDType newType(entry.first, entry.second);
        type.push_back(newType);
    }
}

FileParams Process::getParams(std::vector<std::string> params, double crossSection) const
{
    std::map<std::string, std::string> map;
    if (params.size() != 2*type.size())
    {
        throw std::runtime_error("Error: Check the inputted data");
    }
    for (auto it = params.begin(); it != params.end(); it += 2) 
    {
        std::string category = *it;
        std::string value = *(it + 1); 
        // Probably because the vector has the
    // category followed by value for one param -- also the reason why it iterates by 2
        bool foundCategory = false;
        for (const auto& itype : type)
        {
            if (itype.getName() == category)
            {
                foundCategory = true;
                if (!itype.checkCategory(value)) 
                {
                    throw std::runtime_error("Error: Value not found");
                }
                break;
            }
        }
        if (!foundCategory) 
        {
            throw std::runtime_error("Error: Category not found");
        }
        map.insert({value, category});
    }
    // return FileParams(name, map, crossSection);
    FileParams finalParams(name, map);
    return finalParams;
}