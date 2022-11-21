#ifndef MLVARIABLESPLAN_HH
#define MLVARIABLESPLAN_HH

class MLVariablesPlan : public DataCollectionPlan
{
public:
    MLVariablesPlan();
private:
    virtual void initialize() override;
};

#endif