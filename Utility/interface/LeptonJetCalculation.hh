#ifndef LEPTONJETCALCULATIONHH
#define LEPTONJETCALCULATIONHH

#include "CMSAnalysis/Utility/interface/LeptonJet.hh"
#include <vector>

class LeptonJetCalculation
{
    public:
        static std::vector<double> calculateLeadingPt(LeptonJet jet);
        static std::vector<double> calculateDeltaPt(LeptonJet jet);
        static std::vector<double> calculateSumPt(LeptonJet jet);
        static std::vector<double> calculateDeltaR(LeptonJet jet);
        static std::vector<double> calculateNParticles(LeptonJet jet);
        static std::vector<double> calculateEta(LeptonJet jet);
        static std::vector<double> calculateMaxIsolation(LeptonJet jet);
};

#endif