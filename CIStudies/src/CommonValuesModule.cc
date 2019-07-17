#include "CIAnalysis/CIStudies/interface/CommonValuesModule.hh"

#include "DataFormats/Candidate/interface/Candidate.h"

CommonValuesModule::CommonValuesModule(const MatchingModule& matchingModule) :
  matching(matchingModule)
{
}

bool CommonValuesModule::process(const edm::EventBase& event)
{
  auto matchingPairs = matching.getMatchingBestPairs();

  if (matchingPairs.size() >= 2)
    {
      invariantMass.genSim = findInvariantMass(matchingPairs[0].bestGenParticle, matchingPairs[1].bestGenParticle);
      invariantMass.reco = findInvariantMass(matchingPairs[0].bestRecoParticle, matchingPairs[1].bestRecoParticle);
    }

  bool flipGenSim = lowEtaFlip(matchingPairs[0].bestGenParticle, matchingPairs[1].bestGenParticle);
  if (!flipGenSim) //no flip
    {
      collinsSoper.genSim = findCollinsSoper(matchingPairs[0].bestGenParticle, matchingPairs[1].bestGenParticle);
    }
  else //flip
    {
      collinsSoper.genSim = findCollinsSoper(matchingPairs[1].bestGenParticle, matchingPairs[0].bestGenParticle);
    }

  bool flipReco = lowEtaFlip(matchingPairs[0].bestRecoParticle, matchingPairs[1].bestRecoParticle);
  if (!flipReco) //no flip
    {
      collinsSoper.reco = findCollinsSoper(matchingPairs[0].bestRecoParticle, matchingPairs[1].bestRecoParticle);
    }
  else //flip
    {
      collinsSoper.reco = findCollinsSoper(matchingPairs[1].bestRecoParticle, matchingPairs[0].bestRecoParticle);
    }

  return true;
}

double CommonValuesModule::calculateCosTheta(TLorentzVector Ele, TLorentzVector Elebar) const
{
  double Eleplus  = 1.0/sqrt(2.0) * (Ele.E() + Ele.Z());
  double Eleminus = 1.0/sqrt(2.0) * (Ele.E() - Ele.Z());

  double Elebarplus  = 1.0/sqrt(2.0) * (Elebar.E() + Elebar.Z());
  double Elebarminus = 1.0/sqrt(2.0) * (Elebar.E() - Elebar.Z());

  TLorentzVector Q(Ele+Elebar);

 
  double costheta = 2.0 / (Q.Mag() * sqrt(pow(Q.Mag(),2) + pow(Q.Pt(),2))) * (Eleplus * Elebarminus - Eleminus * Elebarplus);
  if (Q.Pz()<0.0) costheta = -costheta;
  return costheta;
}

double CommonValuesModule::findInvariantMass(const reco::Candidate* particle, const reco::Candidate* antiparticle) const
{
  double product = 2*particle->pt()*antiparticle->pt(); 
  double diff = cosh(particle->eta()-antiparticle->eta())-cos(particle->phi()-antiparticle->phi()); 
  double invariantMass = product*diff; 
  if (invariantMass > 0)
    {
      return sqrt(invariantMass);
    }
  else
    {
      return 0; 
    }
}

double CommonValuesModule::findCollinsSoper(const reco::Candidate* particle, const reco::Candidate* antiparticle) const
{
  TLorentzVector Ele;
  TLorentzVector Elebar;

  float Et1 = particle->et();
  float Et2 = antiparticle->et();
  float Eta1 = particle->eta();
  float Eta2 = antiparticle->eta();
  float Phi1 = particle->phi();
  float Phi2 = antiparticle->phi();
  float En1 = particle->energy();
  float En2 = antiparticle->energy();
  Ele.SetPtEtaPhiE(Et1,Eta1,Phi1,En1);
  Elebar.SetPtEtaPhiE(Et2,Eta2,Phi2,En2);

  TLorentzVector Q(Ele+Elebar);
  
  double costheta = calculateCosTheta(Ele, Elebar);


  TLorentzVector Pbeam(0.0, 0.0,  4000., 4000.); // beam momentum in lab frame
  TLorentzVector Ptarget(0.0, 0.0, -4000., 4000.); // beam momentum in lab frame


  TLorentzVector D(Ele-Elebar);
  // unit vector on R direction
  TVector3 R = Pbeam.Vect().Cross(Q.Vect());
  TVector3 Runit = R.Unit();
  // unit vector on Qt
  TVector3 Qt = Q.Vect(); Qt.SetZ(0);
  TVector3 Qtunit = Qt.Unit();
  TVector3 Dt = D.Vect(); Dt.SetZ(0);
  double tanphi = sqrt(pow(Q.Mag(), 2) + pow(Q.Perp(), 2)) / Q.Mag() * Dt.Dot(Runit) / 
    Dt.Dot(Qtunit);

  if (Q.Pz()<0.0) 
    tanphi = -tanphi;

  return costheta;
}

bool CommonValuesModule::lowEtaFlip(const reco::Candidate* particle, const reco::Candidate* antiparticle) const
{
  if (std::abs(particle->eta()) < std::abs(antiparticle->eta()))
    {
      if (particle->charge() < 0)
	{
	  return true;
	}
    }
  else 
    {
      if (antiparticle->charge() > 0)
	{
	  return true;
	}
    }

  return false;
}
