#ifndef PILEUPSCALEFACTOR_HH
#define PILEUPSCALEFACTOR_HH
#include "CMSAnalysis/Utility/interface/ScaleFactor.hh"
#include "CMSAnalysis/Utility/interface/Lepton.hh"
#include "CMSAnalysis/Utility/interface/ParticleType.hh"

class PileUpScaleFactor : public ScaleFactor
{
	public:
	PileUpScaleFactor(ParticleType particleType);

	
	
	protected:
		virtual std::vector<std::string> getKey(const EventInput* input) const override;

	private:
		ParticleType particleType;

};
#endif