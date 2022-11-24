#ifndef MLVARIABLESPLAN_HH
#define MLVARIABLESPLAN_HH

// A plan for seperating data into lepton jets and recording values for each of these jets like phi,eta,pt,deltaR, etc.
class MLVariablesPlan : public DataCollectionPlan
{
private:
    virtual void initialize() override;
};

#endif