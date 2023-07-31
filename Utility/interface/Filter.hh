#ifndef FILTER_HH
#define FILTER_HH

#include <string>

class EventInput;
class Filter
{
    public:
    virtual std::string getFilterString(const EventInput* input) const = 0;
    virtual ~Filter(){}
};

#endif 