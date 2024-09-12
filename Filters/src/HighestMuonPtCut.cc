#include "CMSAnalysis/Filters/interface/HighestMuonPtCut.hh"

#include "CMSAnalysis/Utility/interface/Event.hh"
#include "CMSAnalysis/Utility/interface/ParticleType.hh"
#include <fstream>
#include <string>
#include <stdexcept>

double readHighestMuonPtCutFromFile(const std::string& filename) {
    std::ifstream file(filename);
    if (!file.is_open()) {
        throw std::runtime_error("Failed to open file: " + filename);
    }

    double highestMuonPtCut;
    file >> highestMuonPtCut;

    if (file.fail()) {
        throw std::runtime_error("Error reading the highest muon pt cut from file: " + filename);
    }

    return highestMuonPtCut;
}

bool HighestMuonPtCut::checkEventInternal(const Event& event, const EventInput* input) const
{
    const std::vector<Muon>& muons = event.getMuons().getParticles();

    double highestMuonPt = 0;
    double secondHighestMuonPt = 0;

    double highestMuonPtCut = readHighestMuonPtCutFromFile("/uscms/home/jpalamad/analysis/CMSSW_14_0_4/src/CMSAnalysis/Filters/src/highestMuonPtCut.txt");
    // double secondHighestMuonPtCut = readHighestMuonPtCutFromFile("/uscms/home/jpalamad/analysis/CMSSW_14_0_4/src/CMSAnalysis/Filters/src/secondHighestMuonPtCut.txt");
    double secondHighestMuonPtCut = 10;
    // double highestMuonPtCut = 36;

    for (auto muon : muons) {
        double pt = muon.getPt();

        if (pt > highestMuonPt)
        {
            secondHighestMuonPt = highestMuonPt;
            highestMuonPt = pt;
        } else if (pt > secondHighestMuonPt) {
            secondHighestMuonPt = pt;
        }
    }

    return (highestMuonPt > highestMuonPtCut && secondHighestMuonPt > secondHighestMuonPtCut);
}
