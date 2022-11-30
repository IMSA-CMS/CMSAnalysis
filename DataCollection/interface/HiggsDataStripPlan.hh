#ifndef HIGGSDATASTRIPPLAN_HH
#define HIGGSDATASTRIPPLAN_HH

// Plan for processing Higgs events into various values such as pt1, pt2, phi1, phi2, eta1, eta2, deltaR, etc.
class HiggsDataStripPlan : public DataCollectionPlan
{
private:
	virtual void initialize() override;
};

#endif