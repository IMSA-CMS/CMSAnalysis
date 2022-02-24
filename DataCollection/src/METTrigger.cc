#include "CMSAnalysis/DataCollection/interface/METTrigger.hh"
#include "CMSAnalysis/DataCollection/interface/METModule.hh"

METTrigger::METTrigger(std::shared_ptr<METModule> iMETMod, const double iMETCutoff) :
  RecoTrigger("MET Trigger"),
  mETMod(iMETMod),
  mETCutoff(iMETCutoff)
{
}

bool METTrigger::checkTrigger(const InputModule* input)
{
  auto mET = mETMod->getMet();

  //fails trigger if empty
  if (!mET)
  {
    return false;
  }
  
  //passes trigger if missing transverse energy is greater than the cutoff
  return (mET > mETCutoff);
}
