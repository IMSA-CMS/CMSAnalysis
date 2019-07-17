#ifndef MATCHINGMODULE_HH
#define MATCHINGMODULE_HH

#include "ProductionModule.hh"
#include "GenSimIdentificationModule.hh"
#include "RecoIdentificationModule.hh"

#include <vector>

#include "TLorentzVector.h"
#include "DataFormats/HepMCCandidate/interface/GenParticle.h"
#include "DataFormats/Candidate/interface/Candidate.h"
#include "DataFormats/Candidate/interface/CompositeRefCandidateT.h"

#include "DataFormats/PatCandidates/interface/Muon.h"
#include "DataFormats/PatCandidates/interface/Electron.h"

class MatchingModule : public ProductionModule
{
public:
  MatchingModule(const GenSimIdentificationModule& genSimModule, const RecoIdentificationModule& recoModule, double deltaRCut = 0.1);
  virtual bool process(const edm::EventBase& event) override;

struct MatchedList
  {
    const reco::GenParticle* bestGenParticle;
    const reco::RecoCandidate* bestRecoParticle;
    double deltaR;
    double phiError;
    double etaError;
    double ptError;
  };

  const std::vector<MatchedList>& getMatchingBestPairs() const {return matchingBestPairs;} 

private:
  const GenSimIdentificationModule& genSim;
  const RecoIdentificationModule& reco;
  double deltaRCutoff;

  std::vector<MatchedList> matchingBestPairs;

  template<typename T>
  bool checkIsNull (std::vector<T*> matching) const;

  double findDeltaPhi(double recoPhi, double genPhi) const;
  double findDeltaR(double recoEta, double genEta, double recoPhi, double genPhi, double phiDif) const;
  double calculateError(double exp, double theo) const;
};


template<typename T>
inline bool MatchingModule::checkIsNull (std::vector<T*> matching)const
{
  for (auto& particle : matching)
    {
      if (particle!=NULL)
	{
	  return false;
	}
    }
  return true;
}



#endif


