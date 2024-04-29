#include <cmath>
#include <stdexcept>
#include <fstream>

#include "CMSAnalysis/Modules/interface/LeptonJetPTDifferential.hh"
#include "CMSAnalysis/Modules/interface/LeptonJetReconstructionModule.hh"
#include "CMSAnalysis/Modules/interface/LeptonJetMatchingModule.hh"

// This is to check the differences for lepton jet transverse momentum

LeptonJetPTDifferential::LeptonJetPTDifferential(const std::shared_ptr<LeptonJetReconstructionModule> iLepRecoMod, const std::shared_ptr<LeptonJetMatchingModule> iLepMatchMod):
    EfficiencyModule(),
    lepRecoMod(iLepRecoMod),
    lepMatchMod(iLepMatchMod)
{
    addRequiredModule(iLepRecoMod);
    addRequiredModule(iLepMatchMod);
}


bool LeptonJetPTDifferential::oppositeChargeLeptonJetFilter(LeptonJet &jet){
  auto leptons = jet.getParticles();
  std::pair<int, int> charge_count = {0, 0};
  for(auto lepton: leptons){
    (lepton.getCharge() == 1) ? ++charge_count.first : ++charge_count.second;
  }
  // if(charge_count.first == 2 || charge_count.second == 2) std::cout << "CBT";
//   std::cout << "count 1: " << charge_count.first << "count 2: " << charge_count.second << "\n";
  return charge_count.first && charge_count.second;
}

void LeptonJetPTDifferential::doCounters()
{
    auto leptonJets = getInput()->getSpecial("leptonJet");
    count += leptonJets.size();
    for (auto leptonJet : leptonJets)
    {
        LeptonJet l(leptonJet);
        if(l.getNumParticles() < 2) continue;
        auto particles = l.getParticles();
        auto lepton1 = particles[0];
        auto lepton2 = particles[1];
        if(oppositeChargeLeptonJetFilter(l)){
            pT_Difference[1] += abs(lepton1.getPt() - lepton2.getPt());
            total_jets[1]++;
        } else {
            pT_Difference[0] += abs(lepton1.getPt() - lepton2.getPt());
            total_jets[0]++;
        }
    }
    // std::cout << "\n";
}

void LeptonJetPTDifferential::finalize()
{
    EfficiencyModule::finalize();
    std::cout << "count: " << count << "\n";
    std::cout << "Average Same Sign Lepton PT Difference: " << pT_Difference[0] / total_jets[0] << "\n";
    std::cout << "Average Opposite Sign Lepton PT Difference: " << pT_Difference[1] / total_jets[1] << "\n";
    std::cout << "Num same sign: " << total_jets[0] << "\n";
    std::cout << "Num opposite sign: " << total_jets[1] << "\n";
    std::cout << "Avg size: " << ((double)(total_jets_sizes[0] + total_jets_sizes[1])) / ((double)(total_jets[0] + total_jets[1])) << "\n";
}
