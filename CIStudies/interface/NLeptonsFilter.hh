#ifndef NLEPTONSFILTER_HH
#define NLEPTONSFILTER_HH

#include <memory>
#include "FilterModule.hh"

class Selector;

class NLeptonsFilter : public FilterModule
{
public:
  NLeptonsFilter(std::shared_ptr<Selector> selector = nullptr);

protected:
  virtual std::string makeFilterString() override;

private:
  std::shared_ptr<Selector> leptonSelector;
};

#endif
