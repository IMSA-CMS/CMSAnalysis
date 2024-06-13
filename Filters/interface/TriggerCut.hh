#ifndef TRIGGERCUT_HH
#define TRIGGERCUT_HH

#include <string>
#include <map>

#include "Cut.hh"

class TriggerCut : public Cut
{
    public:
        TriggerCut(std::vector<std::string> triggers, std::string chosenDataset="SingleElectron");
        

    protected:
        bool checkEventInternal(const Event& event, const EventInput* input) const override;
    private:
        std::vector<std::string> triggers;
        
        //Map of valid triggers for each channel
        std::map<std::string, std::string> channelTriggerDict;
        //Map of double-counted triggers for each channel
        std::map<std::string, std::string> wrongChannelTriggerDict;
        //Convert to string->vector<string> map to extend to multiple valid triggers
        std::string chosenDataset;
        
};

#endif