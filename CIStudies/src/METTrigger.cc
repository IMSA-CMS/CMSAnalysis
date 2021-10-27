#include "CIAnalysis/CIStudies/interface/METTrigger.hh"
#include "CIAnalysis/CIStudies/interface/METModule.hh"

METTrigger::METTrigger(std::shared_ptr<METModule> iMETMod, const double iMETCutoff) :
  RecoTrigger("MET Trigger"),
  mETMod(iMETMod),
  mETCutoff(iMETCutoff)
{
}

bool METTrigger::checkTrigger(std::shared_ptr<InputModule> input)
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
