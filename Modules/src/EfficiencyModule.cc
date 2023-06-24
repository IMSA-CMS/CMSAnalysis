#include "CMSAnalysis/Modules/interface/EfficiencyModule.hh"

void EfficiencyModule::finalize()
{
    for(auto& pair : counters)
    {
        std::cout<<pair.first<<": "<<getCounter(pair.first)<<"\n";
        std::cout<<pair.first<<" efficiency : "<<getEfficiency(pair.first)<<"\n";
    }
}

bool EfficiencyModule::process ()
{
    doCounters();
    ++total;
    return true;
}

void EfficiencyModule::incrementCounter(std::string name, double increment)
{
    // if (increment != 0)
    // {
    //     std::cout << name << "+" << std::to_string(increment) << std::endl;
    // }
    if(counters.find(name) == counters.end())
    {
        double newCounter = 0;
        counters.insert({name,newCounter});
    }
    counters[name] += increment;
}

double EfficiencyModule::getCounter(std::string name) const
{
    return counters.at(name);
}

double EfficiencyModule::getEfficiency(std::string name) const
{
    return getCounter(name)/total;
}
