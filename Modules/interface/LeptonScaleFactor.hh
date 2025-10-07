#ifndef LEPTONSCALEFACTOR_HH
#define LEPTONSCALEFACTOR_HH
#include "CMSAnalysis/Utility/interface/ScaleFactor.hh"
#include "CMSAnalysis/Utility/interface/Lepton.hh"
#include "CMSAnalysis/Utility/interface/ParticleType.hh"

class LeptonScaleFactor : public ScaleFactor
{

	public:
	LeptonScaleFactor(std::string name, std::shared_ptr<ScaleFactorReader> reader, ParticleType particleType);
	// virtual double getScaleFactor(const EventInput* input, SystematicType type = SystematicType::Nominal) const override;
	
	protected:
		virtual std::vector<std::string> getKey(const EventInput* input) const override;
		virtual std::string getKey(Lepton lepton) const;

	private:
		ParticleType particleType;



};
#endif