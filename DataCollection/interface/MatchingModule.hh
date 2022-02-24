#ifndef MATCHINGMODULE_HH
#define MATCHINGMODULE_HH

#include "ProductionModule.hh"
//#include "GenSimIdentificationModule.hh"
//#include "RecoIdentificationModule.hh"
#include "MatchingPairCollection.hh"

#include "TLorentzVector.h"
#include "DataFormats/HepMCCandidate/interface/GenParticle.h"
#include "DataFormats/Candidate/interface/Candidate.h"
#include "DataFormats/Candidate/interface/CompositeRefCandidateT.h"

#include "DataFormats/PatCandidates/interface/Muon.h"
#include "DataFormats/PatCandidates/interface/Electron.h"

// MatchingModule creates a MatchingPairCollection by matching the gen sim particles with the reco particles 
class MatchingModule : public ProductionModule
{
public:
  MatchingModule(double deltaRCut = 0.1);
  virtual bool process() override;

  const MatchingPairCollection& getMatchingBestPairs() const {return matchingBestPairs;} 
protected:
  virtual bool match(std::vector<Particle>genSimparticles, std::vector<Particle> recoCandidates);
private:
  //const std::shared_ptr<GenSimIdentificationModule> genSim;
  //const std::shared_ptr<RecoIdentificationModule> reco;

  // matching fails if the deltaR between the gen sim/reco pair
  // does not lie under the deltaRCutoff
  double deltaRCutoff; 

  MatchingPairCollection matchingBestPairs;

  // template accepts a vector with either GenParticles and RecoCandidates
  // checks if the MatchingPairCollection is filled
  //template<typename T>
  //bool checkIsNull (std::vector<T*> matching) const;

  //E.Hu (7/1/20) - created Particle wrapper class, thus a template is no longer needed
  bool checkIsNull(std::vector<Particle> matching) const;
};


//template<typename T>
//inline bool MatchingModule::checkIsNull (std::vector<T*> matching)const
//{
  //for (auto& particle : matching)
    //{
      //if (particle)
	//{
	  //return false;
	//}
    //}

#endif


