#include "CMSAnalysis/Modules/interface/TriggerEfficiencyModule.hh"
#include "CMSAnalysis/Modules/interface/MatchingModule.hh"
//#include "CMSAnalysis/DataCollection/interface/GenSimIdentificationModule.hh"
#include "CMSAnalysis/Utility/interface/Particle.hh"

TriggerEfficiencyModule::TriggerEfficiencyModule(const std::shared_ptr<MatchingModule> iMatchMod, double iHiggsMass, double iLowerWidth, double iUpperWidth) :
  matchMod(iMatchMod),
  //genSimMod(iGenSimMod),
  higgsMass(iHiggsMass),
  lowerWidth(iLowerWidth),
  upperWidth(iUpperWidth)
{
  addRequiredModule(iMatchMod);
}

bool TriggerEfficiencyModule::process()
{
  auto genSimParticles = getInput()->getLeptons(EventInput::RecoLevel::GenSim).getParticles();  // Vector of Particles
  std::cout<< "Trigger Efficiency Module Working" << '\n';
  //std::cout << "Number of GenSim Particles: " << genSimParticles.size() << '\n';

  for (size_t i = 0; i < genSimParticles.size(); ++i)
  {
    for (size_t j = i + 1; j < genSimParticles.size(); ++j)  // As to avoid overcounting
    {
      auto genSimParticle1 = genSimParticles[i];
      auto genSimParticle2 = genSimParticles[j];

      //std::cout << "------------------------------------------------------------------------\n";
      //std::cout << "GenSim Particle 1\n";
      //printDebugLines(genSimParticle1);
      //
      //std::cout << "GenSim Particle 2\n";
      //printDebugLines(genSimParticle2);
      //
      //std::cout << "Higgs Mass: " << higgsMass << '\n';

      if ((genSimParticle1 != genSimParticle2)  && (genSimParticle1.getCharge() == genSimParticle2.getCharge()))
      {
        ParticleCollection<GenSimParticle> genSimPair;
        genSimPair.addParticle(genSimParticle1);
        genSimPair.addParticle(genSimParticle2);

        // std::cout << "Particle Pair Invariant Mass: " << genSimPair.getInvariantMass() << "\n\n";
        
        ++expectedHiggsCount;

        if (((higgsMass - lowerWidth) <= genSimPair.getInvariantMass()) && (genSimPair.getInvariantMass() <= (higgsMass + upperWidth)))
        {
          ++genSimCount;

          ParticleCollection<Particle> matchedReco;  // ParticleCollection that we will add the matched Reco particles to in a bit

          auto matchedPairs = matchMod->getMatchingBestPairs().getPairs();  // This is a vector of Matched Pairs

          // std::cout << matchedPairs.size() << '\n';

          for (auto pair : matchedPairs)
          {
            // std::cout << "Looping Through Pairs" << '\n';
            if ((genSimParticle1 == pair.getGenParticle()) || (genSimParticle2 == pair.getGenParticle()))
            {
              matchedReco.addParticle(pair.getRecoParticle());
              // std::cout << "GenSim Particle matched with Reco Particle" << '\n';
            }
          }

          if (((higgsMass - lowerWidth) <= matchedReco.getInvariantMass()) && (matchedReco.getInvariantMass() <= (higgsMass + upperWidth)))
          {
            ++recoCount;
          }
        }
      }
    }
  }

  return true;
}

void TriggerEfficiencyModule::finalize()
{
  std::cout << "Expected GenSim H++'s: " << expectedHiggsCount << '\n';
  std::cout << "Number of GenSim H++'s that fall into the mass window of the H++: " << genSimCount << '\n';
  std::cout << "Number of Reco H++'s that fall into the mass window of the H++: " << recoCount << '\n';
  std::cout << "Fraction of H++'s that are reconstructed: " << static_cast<double>(recoCount) / expectedHiggsCount << '\n';
  std::cout << "Overall Generator Eficiency for H++: " << static_cast<double>(genSimCount) / expectedHiggsCount << '\n';
  std::cout << "Overall Trigger Efficiency for H++: " << static_cast<double>(recoCount) / genSimCount << '\n';
}

void TriggerEfficiencyModule::printDebugLines(Particle particle) const  // So I know why the code is not working
{
  if (particle.getType() == ParticleType::electron())
  {
    std::cout << "Type: Electron\n";
  }
  else if (particle.getType() == ParticleType::muon())
  {
    std::cout << "Type: Muon\n";
  }
  std::cout << "PDGID: " << GenSimParticle(particle).pdgId() << '\n';
  std::cout << "Charge: " << particle.getCharge() << '\n';
  //std::cout << "Mother PDGID: " << particle.uniqueMother().pdgId() << "\n\n";
}
  
