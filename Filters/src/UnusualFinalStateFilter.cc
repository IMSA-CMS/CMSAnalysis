#include "CMSAnalysis/Filters/interface/UnusualFinalStateFilter.hh"
#include "CMSAnalysis/Modules/interface/EventInput.hh"

#include "DataFormats/Common/interface/Handle.h"
#include "DataFormats/FWLite/interface/Event.h"
#include "SimDataFormats/GeneratorProducts/interface/GenEventInfoProduct.h"

UnusualFinalStateFilter::UnusualFinalStateFilter()
{
}

std::string UnusualFinalStateFilter::getFilterString(const EventInput* inputMod) const
{
  auto recoParticles = inputMod->getLeptons(EventInput::RecoLevel::Reco);

  if (recoParticles.getNumParticles() > 4)  // Keep all of the events with more than 4 final state leptons
  {
    return (std::to_string(static_cast<int>(recoParticles.getNumParticles())) + "-lepton");
  }

  else if (recoParticles.getNumParticles() == 3)  // Keep all of the events with 3 final state leptons of all the same-sign charge
  {
    if ((recoParticles.getParticles()[0].getCharge() == recoParticles.getParticles()[1].getCharge()) &&
      (recoParticles.getParticles()[0].getCharge() == recoParticles.getParticles()[2].getCharge()))
    {
      return "SameSign3-lepton";
    }
  }

  else if (recoParticles.getNumParticles() == 4)  // Keep all of the events with 4 final state leptons that are not 2 positive and 2 negative
  {
    int pos = 0;  // Number of positively charged leptons
    int neg = 0;  // Numver of negatively charged leptons

    for (auto lepton : recoParticles.getParticles())
    {
      if (lepton.getCharge() > 0)
      {
        pos++;
      }
      else if (lepton.getCharge() < 0)
      {
        neg++;
      }
    }

    if ((pos != 2) || (neg != 2))
    {
      return "ChargeImbalance4-lepton";
    }
  }

  return "";
}
