#ifndef MLMASSSIGNFILTER_HH
#define MLMASSSIGNFILTER_HH

#include "CMSAnalysis/Utility/interface/EventBasic.hh"
#include "CMSAnalysis/Utility/interface/Filter.hh"

class MLMassSignFilter : public Filter
{
    public:
		MLMassSignFilter(double minJetMassi, EventBasic eb): minJetMass(minJetMassi), targetSig(eb) {}
	protected:
        std::string getFilterString(const EventInput* input) const override;
	private:
		const double minJetMass;
        EventBasic targetSig;
};

#endif