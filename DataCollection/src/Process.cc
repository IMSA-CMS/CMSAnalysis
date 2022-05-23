#include "CMSAnalysis/DataCollection/interface/Process.hh"
#include "CMSAnalysis/DataCollection/interface/IDType.hh"

Process::Process(std::string iname, std::vector<IDType> itype): 
name(iname),
type(itype)
{

}
FileParams Process::getParams(std::vector<std::string> params, double crossSection) const
{
    std::unordered_map<std::string, std::string> map;
    if (params.size() != 2*type.size())
    {
        throw std::runtime_error("Error: Check the inputted data");
    }
    for (auto it = params.begin(); it != params.end(); it += 2) 
    {
        std::string category = *it;
        std::string value = *(it + 1);
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
    return FileParams(name, map, crossSection);
}