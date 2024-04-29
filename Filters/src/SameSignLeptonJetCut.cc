#include "CMSAnalysis/Filters/interface/SameSignLeptonJetCut.hh"
#include "CMSAnalysis/Utility/interface/Event.hh"
#include "CMSAnalysis/Utility/interface/LeptonJet.hh"

bool oppositeChargeLeptonJetFilter(LeptonJet &jet){
  auto leptons = jet.getParticles();
  std::pair<int, int> charge_count = {0, 0};
  for(auto lepton: leptons){
    (lepton.getCharge() == 1) ? ++charge_count.first : ++charge_count.second;
  }
  // if(charge_count.first == 2 || charge_count.second == 2) std::cout << "CBT";
  return charge_count.first && charge_count.second;
}

bool SameSignLeptonJetCut::checkEventInternal(const Event& event, const EventInput* input) const
{
	const auto leptonJets = event.getSpecial("leptonJet");

  int count = 0;
  int same_sign_jets = 0;
  int opp_sign_jets = 0;
  for(auto leptonJet : leptonJets){
    count++;
    auto l = LeptonJet(leptonJet);
		if (oppositeChargeLeptonJetFilter(l)) 
		{
			opp_sign_jets++;
		} else same_sign_jets++;
  }
  // if(count > 1) std::cout << "here";
  // this will cut out double jet events w/ one same sign and one opp sign, don't think it matters too much tho
	return same_sign_jets == 0;
}

