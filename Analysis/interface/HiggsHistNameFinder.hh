#ifndef HIGGS_HIST_NAME_FINDER_HH
#define HIGGS_HIST_NAME_FINDER_HH

#include "CMSAnalysis/Analysis/interface/HistNameFinder.hh"

class HiggsHistNameFinder : public HistNameFinder
{
  public:
    HiggsHistNameFinder(std::string reco, std::string genSim = "", bool isData = false, bool zSelection = true, bool lowMass = true)
        : reco(std::move(reco)), genSim(std::move(genSim)), isData(isData), zSelection(zSelection), lowMass(lowMass)
    {}

    std::string getHistName(const HistVariable &histVariable) const override;

  private:
    std::string reco;
    std::string genSim;
    bool isData;
    bool zSelection;
    bool lowMass;
};

#endif // HIGGS_HIST_NAME_FINDER_HH