#ifndef HIGHESTLEPTONJETDELTARCUT_HH
#define HIGHESTLEPTONJETDELTARCUT_HH

#include "CMSAnalysis/Filters/interface/Cut.hh"
#include "CMSAnalysis/Utility/interface/LeptonJet.hh"
#include "CMSAnalysis/Modules/interface/LeptonJetReconstructionModule.hh"
#include <algorithm> // For std::sort

class HighestLeptonJetDeltaRCut : public Cut
{
public:
    HighestLeptonJetDeltaRCut(std::shared_ptr<LeptonJetReconstructionModule> lepRecoMod) : lepRecoMod(lepRecoMod) {}

    bool checkEventInternal(const Event& event, const EventInput* input) const
    {
        auto leptonJets = lepRecoMod->getLeptonJets();

        // Ensure there are lepton jets to evaluate
        if (leptonJets.empty())
        {
            return false; // No lepton jets, so it fails the cut
        }

        // Sort lepton jets by descending transverse momentum (pT)
        std::sort(leptonJets.begin(), leptonJets.end(),
                  [](const LeptonJet& a, const LeptonJet& b) {
                      return a.getPt() > b.getPt(); // Highest pT first
                  });

        // Get the highest (first after sorting) lepton jet
        const auto& highestLeptonJet = leptonJets[0];
        double deltaR = highestLeptonJet.getDeltaR();

        double leptonJetDeltaRCut = readLeptonJetDeltaRCutFromFile("/uscms/home/jpalamad/analysis/CMSSW_14_0_4/src/CMSAnalysis/Filters/src/leptonJetDeltaRCut.txt");

        // Check if delta R is within the allowed range
        return deltaR > leptonJetDeltaRCut;
    }

    private:
        std::shared_ptr<LeptonJetReconstructionModule> lepRecoMod;

        double readLeptonJetDeltaRCutFromFile(const std::string& filename) const {
            std::ifstream file(filename);
            if (!file.is_open()) {
                throw std::runtime_error("Failed to open file: " + filename);
            }

            double LeptonJetDeltaRCut;
            file >> LeptonJetDeltaRCut;

            if (file.fail()) {
                throw std::runtime_error("Error reading the delta R cut from file: " + filename);
            }

            return LeptonJetDeltaRCut;
        }
};

#endif // HIGHESTLEPTONJETDELTARCUT_HH