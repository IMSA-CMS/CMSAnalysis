#ifndef FILTER_HH
#define FILTER_HH

#include <string>

class InputModule;
class Filter
{
    public:
    virtual std::string getFilterString(const InputModule* input) const = 0;
    virtual ~Filter(){}
};

#endif 