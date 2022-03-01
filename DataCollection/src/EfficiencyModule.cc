#include "CMSAnalysis/DataCollection/interface/EfficiencyModule.hh"

void EfficiencyModule::finalize()
{
    for(auto& pair : counters)
    {
        std::cout<<pair.first<<": "<<getCounter(pair.first)<<"\n";
        std::cout<<pair.first<<" efficiency : "<<getEfficiency(pair.first)<<"\n";
    }
}

void EfficiencyModule::writeAll() {}

bool EfficiencyModule::process ()
{
    for (auto pair:counters)
    {
        pair.second.push_back(0);
    }
    total.push_back(0);
    doCounters();
    total.back()++;
    return true;
}

void EfficiencyModule::incrementCounter(std::string name, double increment)
{
    if(counters.find(name) == counters.end())
    {
        std::vector<double> newCounter;
        newCounter.resize(total.size());
        counters.insert({name,newCounter});
    }
    counters[name].back() += increment;
}

double EfficiencyModule::getCounter(std::string name) const
{
    double totalCounters = 0; 
    for (size_t i = 0; i < counters.at(name).size(); i++)
    {
        totalCounters += counters.at(name)[i];
    }
    return totalCounters;
}

double EfficiencyModule::getEfficiency(std::string name) const
{
    double totalCounters = 0;
    for (size_t i = 0; i < total.size(); i++)
    {
        totalCounters += total[i];
    }
    return getCounter(name)/totalCounters;
}

