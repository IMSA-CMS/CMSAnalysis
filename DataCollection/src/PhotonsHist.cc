#include "CMSAnalysis/DataCollection/interface/GenSimRecoPrototype.hh"
#include "DataFormats/Math/interface/deltaPhi.h"
#include "CMSAnalysis/DataCollection/interface/PhotonsHist.hh"

#include <vector>

std::vector<double> PhotonsHist::protectedValue(InputModule::RecoLevel typeGenSim) const
{
  // std::cerr << "photons hist " << EventLoader.use_count() << "\n";
  if (typeGenSim == InputModule::RecoLevel::GenSim)          // typeGenSim == true, so we want the GenSim values
  {
    ParticleCollection<GenSimParticle> finalPhotons;
    auto lepVector = getInput()->getParticles(InputModule::RecoLevel::GenSim, ParticleType::photon()).getParticles();
    ParticleCollection<GenSimParticle> lepCollection;
    for (auto lepton : lepVector){
      lepCollection.addParticle(GenSimParticle(lepton));
    }
    ParticleCollection<GenSimParticle> posSignLep = lepCollection.getPosParticles();
    ParticleCollection<GenSimParticle> oppSignLep = lepCollection.getNegParticles();

   

    auto particleVectorPosSign = posSignLep.getParticles();
    auto particleVectorOppSign = oppSignLep.getParticles();
   
    auto tempPhotons = getInput()->getParticles(InputModule::RecoLevel::GenSim, ParticleType::photon()).getParticles();
    std::vector<GenSimParticle> photons(0);
    for (auto photon : tempPhotons){
      photons.push_back(GenSimParticle(photon));
    }
   
   for(auto current:photons){
      if(current.findMother(11).isNotNull() || current.findMother(-11).isNotNull() || current.findMother(13).isNotNull()|| current.findMother(-13).isNotNull())
      {
        finalPhotons.addParticle(current);
      }
    }
  auto finalPhotonList = finalPhotons.getParticles();

    for(auto currentParticle:finalPhotonList)
    {
      addPhotonUsingMother(posSignLep, oppSignLep, currentParticle);
    }

       auto posSignIv = posSignLep.calculateAllLeptonInvariantMass();
       auto oppSignIv = oppSignLep.calculateAllLeptonInvariantMass();
       if(posSignIv > oppSignIv)
       {
          return {posSignIv};
       }
      else
      {
        return{oppSignIv};
      }

  
  }
  // NOTE: When redoing the histogram files, everything in the else statement was commented out. So, I
  // left it commented out and left the if statement intact. If this is wrong, please change it.
  else                     // typeGenSim == false, so we want the Reco values
  {
    // std::vector<double> ptVector;
    // ParticleCollection leptons;
    // ParticleCollection posSignLep;
    // ParticleCollection oppSignLep;
    // auto recoPhotonParticles = 
    // leptons = getInput()->getParticles(InputModule::RecoLevel::GenSim, Particle::Type::Lepton).getParticles();
    // auto particleVectorLep = leptons.getParticles();

    // for(auto currentParticle : particleVectorLep)
    // {
    //   if(currentParticle.charge() == 1)
    //   {posSignLep.addParticle(currentParticle);}
    //   else if(currentParticle.charge() == -1)
    //   {oppSignLep.addParticle(currentParticle);}
    // }

    // auto particleVectorPosSign = posSignLep.getParticles();
    // auto particleVectorOppSign = oppSignLep.getParticles();
    // auto photons = recoPhotonParticles.getParticles();
    // // std::cout << photons.size();
    // for(auto currentParticle:photons)
    // {
    //   addPhotonUsingPhi(posSignLep, oppSignLep, currentParticle);
    // }

    // auto posSignIv = posSignLep.calculateAllLeptonInvariantMass();
    // auto oppSignIv = oppSignLep.calculateAllLeptonInvariantMass();
    // if(posSignIv > oppSignIv)
    // {
    //   return {posSignIv};
    // }
    // else
    // {
    //   return{oppSignIv};
    // }
    return {0};
    
  }

}

void PhotonsHist::addPhotonUsingIM (ParticleCollection<Particle>& pc1, ParticleCollection<Particle>& pc2, Particle photon) const
{
  ParticleCollection<Particle> particleCollectOne(pc1);
  ParticleCollection<Particle> particleCollectTwo(pc2);
  // auto og1 = particleCollectOne.calculateAllLeptonInvariantMass();
  // auto og2 = particleCollectTwo.calculateAllLeptonInvariantMass();
  // std::cout << "Original positive: " << og1 << "/n";
  // std::cout << "Original negative: " << og2 << "/n";
  particleCollectOne.addParticle(photon);
  particleCollectTwo.addParticle(photon);
  auto pc1IM = particleCollectOne.calculateAllLeptonInvariantMass();
  auto pc2IM = particleCollectTwo.calculateAllLeptonInvariantMass();
  // std::cout << "New positive: " << pc1IM << "/n";
  // std::cout << "New negative: " << pc2IM << "/n";
  if(pc1IM > pc2IM)
  {
    pc1.addParticle(photon);
  }
  else
  {
    pc2.addParticle(photon);
  }
}

void PhotonsHist::addPhotonUsingDR(ParticleCollection<Particle>& pc1, ParticleCollection<Particle>& pc2, Particle photon) const
{
  ParticleCollection<Particle> particleCollectOne(pc1);
  ParticleCollection<Particle> particleCollectTwo(pc2);
  std::vector<double> deltaRValuesP1;
  std::vector<double> deltaRValuesP2;
  double lowestDRp1 = 1000;
  double lowestDRp2 = 1000;

  auto pc1Particles = particleCollectOne.getParticles();
  auto pc2Particles = particleCollectTwo.getParticles();
  for(auto currentParticle:pc1Particles)
  {
    double deltaR = currentParticle.getDeltaR(photon);
    deltaRValuesP1.push_back(deltaR);
  }
  if(!deltaRValuesP1.empty())
  {
    std::sort(deltaRValuesP1.begin(), deltaRValuesP1.end(), [](auto a, auto b){return a < b;});
    lowestDRp1 = deltaRValuesP1[0];
  }
  

  for(auto currentParticle:pc2Particles)
  {
    double deltaR = currentParticle.getDeltaR(photon);
    deltaRValuesP2.push_back(deltaR);
  }
  if(!deltaRValuesP2.empty()) 
  {
    std::sort(deltaRValuesP2.begin(), deltaRValuesP2.end(), [](auto a, auto b){return a < b;});
    lowestDRp2 = deltaRValuesP2[0];
  }
  
  if(lowestDRp1 > lowestDRp2) 
  {
    pc2.addParticle(photon);
  }
  else
  {
    pc1.addParticle(photon);
  }
}
void PhotonsHist::addPhotonUsingPt(ParticleCollection<Particle>& pc1, ParticleCollection<Particle>& pc2, Particle photon) const
{
  ParticleCollection<Particle> particleCollectOne(pc1);
  ParticleCollection<Particle> particleCollectTwo(pc2);
  
  auto highestPt1 = particleCollectOne.getNthHighestPt(1);
  auto highestPt2 = particleCollectTwo.getNthHighestPt(1);
  
  if(highestPt2 > highestPt1) 
  {
    pc2.addParticle(photon);
  }
  else
  {
    pc1.addParticle(photon);
  }

}
void PhotonsHist::addPhotonUsingPhi(ParticleCollection<Particle>& pc1, ParticleCollection<Particle>& pc2, Particle photon) const
{
  ParticleCollection<Particle> particleCollectOne(pc1);
  ParticleCollection<Particle> particleCollectTwo(pc2);
  std::vector<double> phiValuesP1;
  std::vector<double> phiValuesP2;
  double highestPhip2 = 0; 
  double highestPhip1 = 0; 

  auto pc1Particles = particleCollectOne.getParticles();
  auto pc2Particles = particleCollectTwo.getParticles();
  for(auto currentParticle:pc1Particles)
  {
    auto fourVectorLep = currentParticle.getFourVector().Phi();
    auto fourVectorPhoton = photon.getFourVector().Phi();
    auto deltaPhi = reco::deltaPhi(fourVectorLep, fourVectorPhoton);
    phiValuesP1.push_back(deltaPhi);
  }
  if(!phiValuesP1.empty())
  {
    std::sort(phiValuesP1.begin(), phiValuesP1.end(), [](auto a, auto b){return a > b;}); 
    highestPhip1 = phiValuesP1[0];
  }
  

  for(auto currentParticle:pc2Particles)
  {
    auto fourVectorLep = currentParticle.getFourVector().Phi();
    auto fourVectorPhoton = photon.getFourVector().Phi();
    auto deltaPhi = reco::deltaPhi(fourVectorLep, fourVectorPhoton);
    phiValuesP2.push_back(deltaPhi);
  }

  if(!phiValuesP2.empty())
  {
    std::sort(phiValuesP2.begin(), phiValuesP2.end(), [](auto a, auto b){return a > b;}); 
    highestPhip2 = phiValuesP2[0];
  }
  
  if(highestPhip1 > highestPhip2)
  {
    pc1.addParticle(photon);
  }
  else
  {
    pc2.addParticle(photon);
  }

}


void PhotonsHist::addPhotonUsingEta(ParticleCollection<Particle>& pc1, ParticleCollection<Particle>& pc2, Particle photon) const
{
  ParticleCollection<Particle> particleCollectOne(pc1);
  ParticleCollection<Particle> particleCollectTwo(pc2);
  std::vector<double> etaValuesP1;
  std::vector<double> etaValuesP2;
  double lowestDifP1 = 1000;
  double lowestDifp2 = 1000;
  auto pc1Particles = particleCollectOne.getParticles();
  auto pc2Particles = particleCollectTwo.getParticles();
  for(auto currentParticle:pc1Particles)
  {
    auto lepEta = currentParticle.getEta();
    auto photonEta = photon.getEta();
    auto differencep1 = abs(lepEta-photonEta);
    etaValuesP1.push_back(differencep1);
  }

  if(!etaValuesP1.empty())
  {
    std::sort(etaValuesP1.begin(), etaValuesP1.end(), [](auto a, auto b){return a < b;}); 
    lowestDifP1 = etaValuesP1[0];
  }
  

  for(auto currentParticle:pc2Particles)
  {
    auto lepEta = currentParticle.getEta();
    auto photonEta = photon.getEta();
    auto differencep2 = abs(lepEta-photonEta);
    etaValuesP2.push_back(differencep2);
  }
  if(!etaValuesP2.empty())
  {
    std::sort(etaValuesP2.begin(), etaValuesP2.end(), [](auto a, auto b){return a < b;}); 
    lowestDifp2 = etaValuesP2[0];
  }
  
  if(lowestDifP1 > lowestDifp2)
  {
    pc2.addParticle(photon);
  }
  else
  {
    pc1.addParticle(photon);
  }

}


void PhotonsHist::addPhotonUsingIMIs800 (ParticleCollection<Particle>& pc1, ParticleCollection<Particle>& pc2, Particle photon) const
{
  ParticleCollection<Particle> particleCollectOne(pc1);
  ParticleCollection<Particle> particleCollectTwo(pc2);

  particleCollectOne.addParticle(photon);
  particleCollectTwo.addParticle(photon);
  
  if(particleCollectOne.calculateAllLeptonInvariantMass() == 800)
  {
    pc1.addParticle(photon);
  }
  else if (particleCollectTwo.calculateAllLeptonInvariantMass() == 800)
  {
    pc2.addParticle(photon);
  }
}

void PhotonsHist::addPhotonUsingMother (ParticleCollection<GenSimParticle>& pc1, ParticleCollection<GenSimParticle>& pc2, GenSimParticle& photon) const
{
  ParticleCollection<GenSimParticle> particleCollectOne(pc1);
  ParticleCollection<GenSimParticle> particleCollectTwo(pc2);

  auto pc1Particles = pc1.getParticles();
  auto pc2Particles = pc2.getParticles();

  for(auto current:pc1Particles)
  {
    if(photon.mother() == current)
    {
      pc1.addParticle(photon);
    }

  }
  for(auto current:pc2Particles)
  {
    if(photon.mother() == current)
    {
      pc2.addParticle(photon);
    }

  }
}

//Invariant Mass (picture 1)
// std::vector<double> PhotonsHist::protectedValue(bool typeGenSim) const
// {
//   if (typeGenSim)          // typeGenSim == true, so we want the GenSim values
//   {
//     ParticleCollection posSignLep;
//     ParticleCollection oppSignLep;
//     auto genPhotons = getGenSim()->getPhotons();
//     auto photons = genPhotons.getParticles();
//     auto leptons = getGenSim()->getGenParticles();
//     auto lepVector = leptons.getParticles();
//     for(auto currentParticle:lepVector)
//     {
//      if(currentParticle.charge() == 1)
//       {posSignLep.addParticle(currentParticle);}
//       else if(currentParticle.charge() == -1)
//       {oppSignLep.addParticle(currentParticle);}
//     }
//     auto particleVectorPosSign = posSignLep.getParticles();
//     auto particleVectorOppSign = oppSignLep.getParticles();
   

//     for(auto currentParticle:photons)
//     {
//       addPhoton(posSignLep, oppSignLep, currentParticle);
//     }

//     auto posSignIv = posSignLep.calculateAllLeptonInvariantMass();
//     auto oppSignIv = oppSignLep.calculateAllLeptonInvariantMass();
//     if(posSignIv > oppSignIv)
//     {
//       return {posSignIv};
//     }
//     else
//     {
//       return{oppSignIv};
//     }
//   }
//   else                     // typeGenSim == false, so we want the Reco values
//   {
//     std::vector<double> ptVector;
//     ParticleCollection leptons;
//     ParticleCollection posSignLep;
//     ParticleCollection oppSignLep;
//     auto recoPhotonParticles = getReco()->getRecoCandidates(Particle::Type::Photon);
//     leptons = getReco()->getRecoCandidates();
//     auto particleVectorLep = leptons.getParticles();

//     for(auto currentParticle : particleVectorLep)
//     {
//       if(currentParticle.charge() == 1)
//       {posSignLep.addParticle(currentParticle);}
//       else if(currentParticle.charge() == -1)
//       {oppSignLep.addParticle(currentParticle);}
//     }

//     auto particleVectorPosSign = posSignLep.getParticles();
//     auto particleVectorOppSign = oppSignLep.getParticles();
//     auto photons = recoPhotonParticles.getParticles();
//     std::cout << photons.size();
//     for(auto currentParticle:photons)
//     {
//       addPhoton(posSignLep, oppSignLep, currentParticle);
//     }

//     auto posSignIv = posSignLep.calculateAllLeptonInvariantMass();
//     auto oppSignIv = oppSignLep.calculateAllLeptonInvariantMass();
//     if(posSignIv > oppSignIv)
//     {
//       return {posSignIv};
//     }
//     else
//     {
//       return{oppSignIv};
//     }
//   }
// }



//___________________________________________________________________________________________________________
//Get only FSR photons
/**
 *  auto genPhotons = getGenSim()->getPhotons();
 *  auto photons = genPhotons.getParticles();
 *  ParticleCollection finalPhotons;
 * for(current:photons){
 *  if(current.findMother() = 11 || current.findMother() = -11 || current.findMother() = 13 || current.findMother() = -13)
 * }
 *  if(current.checkIsNull() == false){
  *   finalPhotons.add(current);
  * }
 * }
 * 
 * 
 * _____________________________________________________________________________________________________________
 * Get all photons
 * // ParticleCollection posSignLep;
    // ParticleCollection oppSignLep;
    // auto genPhotons = getGenSim()->getPhotons();
    // auto photons = genPhotons.getParticles();
    // auto leptons = getGenSim()->getGenParticles();
    // auto lepVector = leptons.getParticles();
 * 
 * **/
