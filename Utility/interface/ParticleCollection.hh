#ifndef PARTICLECOLLECTION_HH
#define PARTICLECOLLECTION_HH

#include <utility>
#include <functional>

#include <vector>
#include "TLorentzVector.h"
#include "CMSAnalysis/Utility/interface/Particle.hh"
#include "CMSAnalysis/Utility/interface/Utility.hh"
#include "CMSAnalysis/Utility/interface/GenSimParticle.hh"
#include "CMSAnalysis/Utility/interface/ParticleType.hh"

namespace reco
{
  class Candidate;
}
// Particle container. Essentially holds them within a vector,
// but allows numerous useful operations on them
template <typename T = Particle>
class ParticleCollection
{
public:
  ParticleCollection() {}

  // Copy/conversion constructor
  template <typename U>
  ParticleCollection(const ParticleCollection<U>& pc1);
  ParticleCollection(std::vector<T> collectionVector);
  int size() const {return particles.size();}
  void addParticle(T particle) { particles.push_back(particle); }
  const std::vector<T> &getParticles() const { return particles; }
  double getNumParticles() const { return particles.size(); }
  ParticleCollection<T> getPosParticles() const;
  ParticleCollection<T> getNegParticles() const;
  double getNumPosParticles() const { return getPosParticles().getNumParticles(); }
  double getNumNegParticles() const { return getNegParticles().getNumParticles(); }
  double getInvariantMass(bool sameType = true) const;
  double getHighestInvariantMass() const;
  double getLeadingTransverseMomentum() const;
  double getNthHighestPt(int n) const;
  double getLeadingPt() const;
  double getCollinsSoper() const;
  bool isBB() const;
  bool isBE() const;
  double calculateAllLeptonInvariantMass() const;
  double calculateSameSignInvariantMass( bool sameType = false) const;
  std::vector<double> calculateSameSignInvariantMasses(bool sameType) const;
  double calculateOppositeSignInvariantMass(bool sameType = false) const;
  double calculateRecoveredInvariantMass(int nLeptons, int motherPDGID) const;
  T getLeadingPtLepton() const;
  int getLeptonTypeCount(const ParticleType &leptonType) const;
  void clear() { particles.clear(); }
  T &operator[](int i) { return particles.at(i); }
  const T &operator[](int i) const { return particles.at(i); }
  auto begin() { return particles.begin(); }
  auto end() { return particles.end(); }
  auto cbegin() const { return particles.begin(); }
  auto cend() const { return particles.end(); }
  auto begin() const { return cbegin(); }
  auto end() const { return cend(); }
  void sort() { std::sort(begin(), end(), std::greater<T>()); };
  // 1->2  0->1
  std::pair<T, T> chooseParticles(bool oppositeSigns, bool sameType) const; // picks particles given if they are opposite signs or not, sameType = false does not exclude same-type particles
  std::pair<T, T> chooseParticles(bool sameType) const;// picks particles with greatest invariant mass

private:
  std::vector<T> particles;
  std::pair<T, T> chooseParticlesByPhi(bool oppositeSigns) const; // picks particles by the Phi angle
  bool checkSigns(T particle1, T particle2) const;
  double calculateInvariantMass(T particle1, T particle2) const;
  double calculateLeadingTransverseMomentum(T particle1, T particle2) const;
  double calculateCollinsSoper(T particle1, T particle2) const;
  double calculateCosTheta(TLorentzVector Ele, TLorentzVector Elebar) const;

  bool lowEtaFlip(T particle, T antiparticle) const;
};

template <typename T>
template <typename U>
inline ParticleCollection<T>::ParticleCollection(const ParticleCollection<U>& pc1)
{
  for (auto particle : pc1.getParticles())
  {
    particles.push_back(particle);
  }
}

template <typename T>
inline ParticleCollection<T>::ParticleCollection(std::vector<T> collectionVector) : particles(collectionVector) {}

template <typename T>
inline ParticleCollection<T> ParticleCollection<T>::getPosParticles() const
{
  ParticleCollection<T> positives; // All of the positively-charged particles in the ParticleCollection

  for (auto particle : getParticles())
  {
    if (particle.getCharge() > 0)
    {
      positives.addParticle(particle); // Add all of the positively-charged particles to positives
    }
  }


  return positives;
} // ParticleCollection of just the positively charged particles

template <typename T>
inline ParticleCollection<T> ParticleCollection<T>::getNegParticles() const
{
  ParticleCollection<T> negatives; // All of the negatively-charged particles in the ParticleCollection

  for (auto particle : getParticles())
  {
    if (particle.getCharge() < 0)
    {
      negatives.addParticle(particle); // Add all of the negatively-charged particles to negatives
    }
  }


  return negatives;
} // ParticleCollection of just the negatively charged particles

template <typename T>
inline double ParticleCollection<T>::getInvariantMass(bool sameType) const
{
  auto particlePair = chooseParticles(true, sameType);
  if (particlePair.first.isNotNull() && particlePair.second.isNotNull())
  {
    return calculateInvariantMass(particlePair.first, particlePair.second);
  }
  else
  {
    return -1;
  }
}

template <typename T>
inline double ParticleCollection<T>::getHighestInvariantMass() const
{
  auto particlePair = chooseParticles(true);
  if (particlePair.first.isNotNull() && particlePair.second.isNotNull())
  {
    return calculateInvariantMass(particlePair.first, particlePair.second);
  }
  else
  {
    return -1;
  }
}

template <typename T>
inline double ParticleCollection<T>::getLeadingTransverseMomentum() const
{
  auto particlePair = chooseParticles(true);
  if (particlePair.first.isNotNull() && particlePair.second.isNotNull())
  {

    return calculateLeadingTransverseMomentum(particlePair.first, particlePair.second);
  }
  else
  {

    return -1;
  }
}

template <typename T>
inline double ParticleCollection<T>::getNthHighestPt(int n) const
{
  if (n < 1)
  {
    throw std::invalid_argument("Not a positive integer");
  }

  if (n > getNumParticles())
  {
    return 0; // If the nth highest pt particle doesn't exist, return 0
  }

  auto particlesVec = getParticles(); // Vector of Particles

  // Sort the vector of particles by pt (greatest to least)
  std::sort(particlesVec.begin(), particlesVec.end(), [](auto a, auto b)
            { return a.getPt() > b.getPt(); });


  return particlesVec[n - 1].getPt(); // n-1 since the first element is 0, 2nd element is 1, etc.
}

template <typename T>
inline double ParticleCollection<T>::getLeadingPt() const
{
  double highestPt = 0;
  for (auto particle : particles)
  {
    double pt = particle.getPt();
    if (pt > highestPt)
    {
      highestPt = pt;
    }
  }
  
  return highestPt;
}

template <typename T>
inline double ParticleCollection<T>::getCollinsSoper() const
{
  auto particlePair = chooseParticles(true);

  // guarantees that the particle has to have a negative charge or the antiparticle has to have a positve charge
  // if both particles have the same sign (i.e. electrons), the situation is solved at the start of calculateCollinsSoper()
  if (particlePair.first.isNotNull() && particlePair.second.isNotNull())
  {
    if (particlePair.first.getCharge() < 0) // no flip
    {

      return calculateCollinsSoper(particlePair.first, particlePair.second);
    }
    else // flip
    {

      return calculateCollinsSoper(particlePair.second, particlePair.first);
    }
  }
  else
  {

    return -2;
  }
}
template <typename T>
inline bool ParticleCollection<T>::isBB() const
{
  auto particlePair = chooseParticles(true);
  if (particlePair.first.isNotNull() && particlePair.second.isNotNull())
  {
    if (particlePair.first.getBarrelState() == Particle::BarrelState::Barrel && particlePair.second.getBarrelState() == Particle::BarrelState::Barrel)
    {
      return true;
    }
  }
  return false;
}

template <typename T>
inline bool ParticleCollection<T>::isBE() const
{
  auto particlePair = chooseParticles(true);
  if (particlePair.first.isNotNull() && particlePair.second.isNotNull())
  {
    if ((particlePair.first.getBarrelState() == Particle::BarrelState::Barrel && particlePair.second.getBarrelState() == Particle::BarrelState::Endcap) || (particlePair.first.getBarrelState() == Particle::BarrelState::Endcap && particlePair.second.getBarrelState() == Particle::BarrelState::Barrel))
    {
      return true;
    }
    // if both particles are muons, then EE counts as BE
    if ((particlePair.first.getType() == ParticleType::muon() && particlePair.second.getType() == ParticleType::muon()) && (particlePair.first.getBarrelState() == Particle::BarrelState::Endcap && particlePair.second.getBarrelState() == Particle::BarrelState::Endcap))
    {
      return true;
    }
  }
  return false;
}
template <typename T>
inline double ParticleCollection<T>::calculateAllLeptonInvariantMass() const
{
  reco::Candidate::LorentzVector total;

  for (auto particle : particles)
  {
    auto newVec = particle.getFourVector();
    total += newVec;
  }


  return total.M();
}

template <typename T>
inline double ParticleCollection<T>::calculateSameSignInvariantMass( bool sameType) const
{
 // std::cout << "Particle Collection SameSignIM Vector Size:" << particles.size();
  T iPointer = Particle::nullParticle();
  T jPointer = Particle::nullParticle();
  std::pair<T, T> particlePair = {iPointer, jPointer};
  //std::cout << "Line B0";
  // if (usingPhi)
  // {
  //   //std::cout << "Line B1";
  //   particlePair = chooseParticlesByPhi(false); // we want same sign particles with best phi angle
  // }
  //else
  //{
    //std::cout << "Line B2";
    particlePair = chooseParticles(false, sameType); // we want same sign particles with highest invariant mass
  //}
  //std::cout << "Line B3";
  if (particlePair.first.isNotNull() && particlePair.second.isNotNull())
  {
    return calculateInvariantMass(particlePair.first, particlePair.second);
  }
  else
  {
    return -1;
  }
}

template <typename T>
inline std::vector<double> ParticleCollection<T>::calculateSameSignInvariantMasses(bool sameType) const
{
  std::vector<double> sameSignInvariantMasses;

  if (getPosParticles().getNumParticles() >= 2)
  {
    double inv = getPosParticles().calculateSameSignInvariantMass(sameType);
    // std::cout << inv << '\t';
    sameSignInvariantMasses.push_back(inv);
    // sameSignInvariantMasses.push_back(getPosParticles().calculateSameSignInvariantMass(usingPhi));
  }
  else
  {
    // std::cout << "0\t";
    //sameSignInvariantMasses.push_back(0);
  }

  if (getNegParticles().getNumParticles() >= 2)
  {
    double inv = getNegParticles().calculateSameSignInvariantMass(sameType);
    // std::cout << inv << '\n';
    sameSignInvariantMasses.push_back(inv);
    // sameSignInvariantMasses.push_back(getNegParticles().calculateSameSignInvariantMass(usingPhi));
  }
  else
  {
    // std::cout << "0\n";
    //sameSignInvariantMasses.push_back(0);
  }

  return sameSignInvariantMasses;
}
template <typename T>
inline double ParticleCollection<T>::calculateOppositeSignInvariantMass(bool sameType) const
{
  auto particlePair = chooseParticles(true, sameType); // we want opposite sign particles with highest invariant mass
  if (particlePair.first.isNotNull() && particlePair.second.isNotNull())
  {
    return calculateInvariantMass(particlePair.first, particlePair.second);
  }
  else
  {
    return -1;
  }
}

template <typename T>
inline double ParticleCollection<T>::calculateRecoveredInvariantMass(int nLeptons, int motherPDGID) const
{
  double maxInvariantMass = 0;

  if (nLeptons < 0)
  {
    throw std::out_of_range("Use a valid number of leptons"); // Use valid nLeptons number
  }

  else if (nLeptons > getNumParticles())
  {
    std::cout << "Not enough leptons; running on " << getNumParticles() << " leptons.\n";
    return calculateRecoveredInvariantMass(getNumParticles(), motherPDGID);
  }

  auto allLeptons = getParticles();       // Vector of Particles
  int allLeptonsSize = getNumParticles(); // Size of above vector

  // Create a vector of ints that are the indices of allLeptons
  std::vector<int> indices;
  for (int i = 0; i < allLeptonsSize; i++)
  {
    indices.push_back(i);
  }

  std::vector<int> combination;
  std::vector<std::vector<int>> totalCombinations;

  Utility::getAllCombinations(0, nLeptons, combination, indices, totalCombinations);

  for (auto leptonIndices : totalCombinations)
  {
    // Create a new ParticleCollection and add back the leptons
    ParticleCollection<T> leptons;
    std::vector<Particle> part;
    for (auto index : leptonIndices)
    {
      leptons.addParticle(allLeptons[index]);
      part.push_back(allLeptons[index]);
    }

    bool daughterOfHPlusPlus = GenSimParticle::sharedMother(motherPDGID, part).isNotNull(); // If all of the particles map their mother to the specified mother particle, this is true

    if ((leptons.calculateAllLeptonInvariantMass() > maxInvariantMass) && (daughterOfHPlusPlus))
    {
      maxInvariantMass = leptons.calculateAllLeptonInvariantMass();
    }
  }

  return maxInvariantMass;
}
template <typename T>
inline T ParticleCollection<T>::getLeadingPtLepton() const
{
  double highestPt = getLeadingPt();
  for (auto part : particles)
  {
    if (part.getPt() == highestPt)
    {
      return part;
    }
  }

  throw std::runtime_error("No leading pT lepton!");
}

template <typename T>
inline int ParticleCollection<T>::getLeptonTypeCount(const ParticleType &leptonType) const // Finds the number of a certain lepton type (electrons/muons)
{
  int count = 0;
  for (auto particle : particles)
  {
    if (particle.getType() == leptonType)
    {
      ++count;
    }
  }

  return count;
}

template <typename T>
inline typename std::pair<T, T> ParticleCollection<T>::chooseParticles(bool sameType) const
{
  auto particlePair = chooseParticles(true, sameType); // opposite signs

  if (!(particlePair.first.isNotNull() && particlePair.second.isNotNull())) // If the particle pair is empty, then test same signs
  {
    particlePair = chooseParticles(false, sameType);
  }

  return particlePair;
}

template <typename T>
inline typename std::pair<T, T> ParticleCollection<T>::chooseParticles(bool oppositeSigns, bool sameType) const
{
  double maxInvariantMass = 0;
  T iPointer = Particle::nullParticle();
  T jPointer = Particle::nullParticle();

  for (int i = 0; i < static_cast<int>(particles.size()) - 1; ++i)
  {
    for (int j = i + 1; j < static_cast<int>(particles.size()); ++j)
    {
      if (checkSigns(particles[i], particles[j]) == oppositeSigns) // Check if the particle pairs' signs match with what we want
      {
        if (calculateInvariantMass(particles[i], particles[j]) > maxInvariantMass)
        {
          maxInvariantMass = calculateInvariantMass(particles[i], particles[j]);
          iPointer = particles[i];
          jPointer = particles[j];
        }
      }
    }
  }

  return {iPointer, jPointer};
}

template <typename T>
inline typename std::pair<T, T> ParticleCollection<T>::chooseParticlesByPhi(bool oppositeSigns) const
{
  double bestAngle = 0; // We want the two particles whose phi angle difference is closest to 180, since that's what H++ decay leptons do
  T iPointer = Particle::nullParticle();
  T jPointer = Particle::nullParticle();

  for (int i = 0; i < static_cast<int>(particles.size()) - 1; ++i)
  {
    for (int j = i + 1; j < static_cast<int>(particles.size()); ++j)
    {
      if (checkSigns(particles[i], particles[j]) == oppositeSigns) // Check if the particle pairs' signs match with what we want
      {
        double angleDifference = abs(particles[i].getPhi() - particles[j].getPhi());
        if (angleDifference > 180)
        {
          angleDifference = 360 - angleDifference;
        }

        if (angleDifference > bestAngle)
        {
          bestAngle = angleDifference;
          iPointer = particles[i];
          jPointer = particles[j];
        }
      }
    }
  }

  return {iPointer, jPointer};
}
template <typename T>
inline bool ParticleCollection<T>::checkSigns(T particle1, T particle2) const
{
  if (particle1.getCharge() != particle2.getCharge())
  {
    return true;
  }

  return false;
}
template <typename T>
inline double ParticleCollection<T>::calculateCosTheta(TLorentzVector Ele, TLorentzVector Elebar) const
{
  double Eleplus = 1.0 / sqrt(2.0) * (Ele.E() + Ele.Z());
  double Eleminus = 1.0 / sqrt(2.0) * (Ele.E() - Ele.Z());

  double Elebarplus = 1.0 / sqrt(2.0) * (Elebar.E() + Elebar.Z());
  double Elebarminus = 1.0 / sqrt(2.0) * (Elebar.E() - Elebar.Z());

  TLorentzVector Q(Ele + Elebar);

  double costheta = 2.0 / (Q.Mag() * sqrt(pow(Q.Mag(), 2) + pow(Q.Pt(), 2))) * (Eleplus * Elebarminus - Eleminus * Elebarplus);
  if (Q.Pz() < 0)
    costheta = -costheta;
  return costheta;
}
template <typename T>
inline bool ParticleCollection<T>::lowEtaFlip(T particle, T antiparticle) const
{
  if (std::abs(particle.getEta()) < std::abs(antiparticle.getEta()))
  {
    if (particle.getCharge() < 0)
    {
      return true;
    }
  }

  else
  {
    if (antiparticle.getCharge() > 0)
    {
      return true;
    }
  }

  return false;
}
template <typename T>
inline double ParticleCollection<T>::calculateCollinsSoper(T particle, T antiparticle) const
{
  // if same-sign problem occurs, we trust the charge of the particle with abs(eta) closer to 0
  if (!checkSigns(particle, antiparticle) && lowEtaFlip(particle, antiparticle))
  {
    T temp = particle;
    particle = antiparticle;
    antiparticle = temp;
  }

  TLorentzVector Ele;
  TLorentzVector Elebar;

  float Et1 = particle.getEt();
  float Et2 = antiparticle.getEt();
  float Eta1 = particle.getEta();
  float Eta2 = antiparticle.getEta();
  float Phi1 = particle.getPhi();
  float Phi2 = antiparticle.getPhi();
  float En1 = particle.getEnergy();
  float En2 = antiparticle.getEnergy();
  Ele.SetPtEtaPhiE(Et1, Eta1, Phi1, En1);
  Elebar.SetPtEtaPhiE(Et2, Eta2, Phi2, En2);

  return calculateCosTheta(Ele, Elebar);
}
template <typename T>
inline double ParticleCollection<T>::calculateInvariantMass(T particle1, T particle2) const
{
  auto vec1 = particle1.getFourVector();
  auto vec2 = particle2.getFourVector();

  auto sum = vec1 + vec2;

  // USED TO BE POSITIVE, JUST WANTED TO SEE WHAT HAPPENED MAY GOD HAVE MERCY
  // I APOLOGIZE TO THE FLYING SPAGHETTI MONSTER FOR THIS BLASPHEMY [ATANG, 11/07]
  // Problem solved :)
  return sum.M();

}
template <typename T>
inline double ParticleCollection<T>::calculateLeadingTransverseMomentum(T particle1, T particle2) const
{
  double pt1 = particle1.getPt();
  double pt2 = particle2.getPt();
  if (pt1 > pt2)
  {
    return pt1;
  }
  else
  {
    return pt2;
  }
}
#endif
