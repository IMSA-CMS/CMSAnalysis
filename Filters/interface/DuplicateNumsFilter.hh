#ifndef DUPLICATENUMSFILTER_HH
#define DUPLICATENUMSFILTER_HH

#include <memory>
#include "CMSAnalysis/Utility/interface/Filter.hh"
#include "CMSAnalysis/Utility/interface/ParticleCollection.hh"
#include "CMSAnalysis/Modules/interface/EventInput.hh"

class DuplicateNumsFilter : public Filter
{
public: 
  DuplicateNumsFilter();

protected:
  virtual std::string getFilterString() const override;
};

#endif