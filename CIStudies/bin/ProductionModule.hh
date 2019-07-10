#ifndef PRODUCTIONMODULE_HH
#define PRODUCTIONMODULE_HH

class ProductionModule
{
public:
    virtual void process(const edm::EventBase& event) = 0;
};

#endif
