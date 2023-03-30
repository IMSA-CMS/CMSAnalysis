#ifndef TRIGGERARRAY_HH
#define TRIGGERARRAY_HH

#include <memory>
#include <vector>
#include <map>
#include <iostream>


template <class... Args> class TriggerArray
{
  public:
    TriggerArray(std::vector<std::string> iNames);
    virtual std::vector<std::string> getNames() const {return names;}
    std::map<std::string, bool> checkEvent(Args&&... args); // Calls checkTrigger() and increments pass and total
    std::map<std::string, double> passRatios() const;
    void printTriggerResults();
    virtual ~TriggerArray() {};

  protected:
    virtual std::map<std::string, bool> checkTrigger(Args&... arg) = 0;
    virtual std::map<std::string, bool> checkTrigger() = 0;    // map of trigger and true if the trigger is passed

  private:
    struct efficiency
    {
        int passed = 0;
        int total = 0;
    };
    
    const std::vector<std::string> names;
    std::map<std::string, efficiency> triggerData;
};

#endif