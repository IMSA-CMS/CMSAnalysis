#ifndef TRIGGERCUT_HH
#define TRIGGERCUT_HH

#include <string>

#include "Cut.hh"

class TriggerCut : public Cut
{
    public:
        TriggerCut(std::vector<std::string> triggers);

    protected:
        bool checkEventInternal(const Event& event, const EventInput* input) const override;
    private:
        std::vector<std::string> triggers;
};

#endif