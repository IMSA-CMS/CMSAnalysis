#ifndef TRIGGER_HH
#define TRIGGER_HH

#include <memory>
#include <iostream>

class RecoIdentificationModule;

class Trigger
{
  public:
    Trigger(std::string iName);
    std::string getName() const {return name;}
    bool checkEvent();                  // Calls checkTrigger() and increments pass and total
    double passRatio() const {return static_cast<double>(pass) / total;}
    void printTriggerResult();
    virtual ~Trigger() {};

  protected:
    virtual bool checkTrigger() = 0;    // Returns true if the trigger is passed

  private:
    const std::string name;
    int pass = 0;
    int total = 0;
};

#endif
