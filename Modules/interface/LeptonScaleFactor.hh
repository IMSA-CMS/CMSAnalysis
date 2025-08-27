#ifndef LEPTONSCALEFACTOR_HH
#define LEPTONSCALEFACTOR_HH
#include "CMSAnalysis/Utility/interface/ScaleFactor.hh"

class LeptonScaleFactor : public ScaleFactor
{

	public:
	LeptonScaleFactor(std::shared_ptr<LeptonScaleFactor> reader, ParticleType particleType);
	// virtual double getScaleFactor(const EventInput* input, SystematicType type = SystematicType::Nominal) const override;
	
	protected:
		virtual std::vector<std::string> getKey(const EventInput* input) const override;
		virtual std::string getKey(Lepton lepton) const;

	private:
		ParticleType particleType;



};
#endif